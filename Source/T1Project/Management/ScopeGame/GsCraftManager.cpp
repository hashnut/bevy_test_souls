#include "GsCraftManager.h"

#include "Runtime/Core/Public/Containers/UnrealString.h"
#include "Runtime/Core/Public/Misc/ScopeExit.h"

#include "../../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "Currency/GsCostPackage.h"
#include "Currency/GsCostPackageMultiple.h"
#include "Currency/GsCurrencyHelper.h"
#include "DataSchema/GsSchemaEnums.h"
#include "DataSchema/Item/Craft/GsSchemaItemCraftMaterial.h"
#include "DataSchema/Item/Craft/GsSchemaItemCraftRecipe.h"
#include "DataSchema/Item/Craft/GsSchemaItemCraftRestrict.h"
#include "DataSchema/Item/Craft/GsSchemaItemCraftTab.h"
#include "DataSchema/SupplyInfo/GsSchemaCraftSupplyInfo.h"
#include "GsUnlockManager.h"
#include "Item/Craft/GsCraftTab.h"
#include "Item/Craft/GsCraftTarget.h"
#include "Item/GsItemManager.h"
#include "Management/GsMessageHolder.h"
#include "Management/ScopeGame/GsSupplyManager.h"
#include "Message/MessageParam/GsCraftMessageParam.h"
#include "Message/MessageParam/GsMessageParam.h"
#include "Net/GsNetSendServiceCraft.h"
#include "Shared/Client/SharedEnums/PD_ResultEnum.h"
#include "Shared/Client/SharedEnums/SharedCommonEnum.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "T1Project.h"
#include "UI/UIContent/Helper/GsUIHelper.h"
#include "Unlock/GsUnlockDefine.h"

const int32 UGsCraftManager::CRAFT_FAVORITE_MAX_COUNT = 50;

void UGsCraftManager::Initialize()
{
	// GSLOG(Log, TEXT("UGsCraftManager::Initialize()"));

	RegisterMessages();
}

void UGsCraftManager::InitializeAfterResourceDownload()
{
	if (!InitliazeData())
	{
		GSLOG(Error, TEXT("!InitliazeData()"));
	}
}

void UGsCraftManager::Finalize()
{
	// GSLOG(Log, TEXT("UGsCraftManager::Finalize()"));

	UnegisterMessages();
}

void UGsCraftManager::RegisterMessages()
{
	// GSLOG(Log, TEXT("UGsCraftManager::RegisterMessages()"));

	FGsMessageHolder* MessageHolder = GMessage();
	if (nullptr == MessageHolder)
	{
		GSLOG(Error, TEXT("nullptr == MessageHolder"));
		return;
	}

	_craftDelegates.Emplace(MessageHolder->GetCraft().AddUObject(FGsCraftMessageParamNotifyCraftStatListWorldChanged::MESSAGE_ID, this, &UGsCraftManager::NotifyCraftStatListWorldChanged));
	_craftDelegates.Emplace(MessageHolder->GetCraft().AddUObject(FGsCraftMessageParamAckCraftStatList::MESSAGE_ID, this, &UGsCraftManager::AckCraftStatList));

	_craftDelegates.Emplace(MessageHolder->GetCraft().AddUObject(FGsCraftMessageParamTryChangeFavoriteCraftTarget::MESSAGE_ID, this, &UGsCraftManager::TryChangeFavoriteCraftTarget));
	_craftDelegates.Emplace(MessageHolder->GetCraft().AddUObject(FGsCraftMessageParamOnOpenTab::MESSAGE_ID, this, &UGsCraftManager::OnOpenTab));

	_systemDelegates.Emplace(MessageHolder->GetSystemParam().AddUObject(MessageSystem::RESET_TIME_CONTENTS, this, &UGsCraftManager::OnResetTimeContents));
}

void UGsCraftManager::UnegisterMessages()
{
	// GSLOG(Log, TEXT("UGsCraftManager::UnegisterMessages()"));

	FGsMessageHolder* MessageHolder = GMessage();
	if (nullptr == MessageHolder)
	{
		GSLOG(Error, TEXT("nullptr == MessageHolder"));
		return;
	}

	while (0 < _systemDelegates.Num())
	{
		MessageHolder->GetSystemParam().Remove(_systemDelegates.Pop(false));
	}

	while (0 < _craftDelegates.Num())
	{
		MessageHolder->GetCraft().Remove(_craftDelegates.Pop(false));
	}
}

bool UGsCraftManager::InitliazeData()
{
	// GSLOG(Log, TEXT("UGsCraftManager::InitliazeData()"));

	_limitCountInitialized = false;
	_favoriteInitialized = false;

	// initlaize tab datas
	{
		const UGsTable* CraftTabTable = FGsSchemaItemCraftTab::GetStaticTable();
		if (nullptr == CraftTabTable)
		{
			GSLOG(Error, TEXT("nullptr == CraftTabTable"));
			return false;
		}

		TArray<const FGsSchemaItemCraftTab*> CraftTabRows;
		if (!CraftTabTable->GetAllRows(CraftTabRows))
		{
			GSLOG(Error, TEXT("!Table->GetAllRows(Rows)"));
			return false;
		}

		// add root tab
		_allTabs.Empty(CraftTabRows.Num() + 1);
		_allTabs.Emplace(INVALID_CRAFT_TAB_ID, nullptr);

		// add all tabs
		for (const FGsSchemaItemCraftTab* Row : CraftTabRows)
		{
			// create current
			const CraftTabId CurrentCraftTabId = Row->itemCraftTabId;
			if (INVALID_CRAFT_TAB_ID == CurrentCraftTabId)
			{
				GSLOG(Error, TEXT("INVALID_CRAFT_TAB_ID == CurrentCraftTabId, '%d' 은 내부적으로 사용하는 최상위 ID 이므로 변경해주세요: %d"), INVALID_CRAFT_TAB_ID, CurrentCraftTabId);
				continue;
			}

			FGsCraftTab* CurrentCraftTab = _allTabs.Find(CurrentCraftTabId);
			if (nullptr != CurrentCraftTab)
			{
				GSLOG(Error, TEXT("nullptr != CurrentCraftTab, id 중복: %d"), CurrentCraftTabId);
				continue;
			}

			if (Row->specialTab == EGsCraftSpecialTab::Favorite)
			{
				if (INVALID_CRAFT_TAB_ID != _favoriteTabId)
				{
					// favorite 탭은 유일해야 하므로, favorite 탭이 유일한지 체크
					GSLOG(Error, TEXT("즐겨찾기 탭이 두 개 이상이므로 하나로 변경해주세요. CurrentCraftTabId:%d"), CurrentCraftTabId);
				}
				else
				{
					_favoriteTabId = CurrentCraftTabId;
				}
			}

			//GSLOG(Log, TEXT("UGsCraftManager::InitliazeData(), CurrentCraftTabId:%d, Row:%p"), CurrentCraftTabId, Row);
			_allTabs.Emplace(CurrentCraftTabId, Row);
		}

		// add child tabs
		// 맵이 변경될 경우, 포인터 등이 유효하지 않게 되므로 반복문을 분리함. 순서 유지를 위해 테이블을 재순환함.
		for (const FGsSchemaItemCraftTab* Row : CraftTabRows)
		{
			check(Row);

			// find current
			const CraftTabId CurrentCraftTabId = Row->itemCraftTabId;
			if (INVALID_CRAFT_TAB_ID == CurrentCraftTabId)
			{
				GSLOG(Error, TEXT("INVALID_CRAFT_TAB_ID == CurrentCraftTabId, 내부적으로 사용하는 최상위 id이므로 변경해주세요: %d"), CurrentCraftTabId);
				continue;
			}

			FGsCraftTab* CraftTab = _allTabs.Find(CurrentCraftTabId);
			if (nullptr == CraftTab)
			{
				GSLOG(Error, TEXT("nullptr == CraftTab, CurrentCraftTabId:%d"), CurrentCraftTabId);
				continue;
			}

			// find parent
			const CraftTabId ParentCraftTabId = Row->parentItemCraftTabId;
			FGsCraftTab* ParentCraftTab = _allTabs.Find(ParentCraftTabId);
			if (nullptr == ParentCraftTab)
			{
				GSLOG(Error, TEXT("nullptr == ParentCraftTab, ParentCraftTabId:%d"), ParentCraftTabId);
				continue;
			}

			// GSLOG(Log, TEXT("UGsCraftManager::InitliazeData(), ParentCraftTab:%d, CraftTab:%d"), ParentCraftTabId, CurrentCraftTabId);
			CraftTab->SetParentTab(ParentCraftTab);
			ParentCraftTab->AddChildTab(CraftTab);
		}
	}

	// load craft material datas
	const UGsTableItemCraftMaterial* CraftMaterialTable = Cast<UGsTableItemCraftMaterial>(FGsSchemaItemCraftMaterial::GetStaticTable());
	if (nullptr == CraftMaterialTable)
	{
		GSLOG(Error, TEXT("nullptr == CraftMaterialTable"));
		return false;
	}

	// initialize target datas
	{
		const UGsTable* CraftTargetTable = FGsSchemaItemCraftRecipe::GetStaticTable();
		if (nullptr == CraftTargetTable)
		{
			GSLOG(Error, TEXT("nullptr == CraftTargetTable"));
			return false;
		}

		TArray<const FGsSchemaItemCraftRecipe*> CraftTargetRows;
		if (!CraftTargetTable->GetAllRows(CraftTargetRows))
		{
			GSLOG(Error, TEXT("!CraftTargetTable->GetAllRows(CraftTargetRows)"));
			return false;
		}

		const UGsTableItemCraftRestrict* CraftRestrictTable = Cast<UGsTableItemCraftRestrict>(FGsSchemaItemCraftRestrict::GetStaticTable());
		if (nullptr == CraftRestrictTable)
		{
			GSLOG(Error, TEXT("nullptr == CraftRestrictTable"));
			return false;
		}

		for (const FGsSchemaItemCraftRecipe* ItemCraftRecipe : CraftTargetRows)
		{
			check(!!ItemCraftRecipe);

			const CraftId CurrentCraftTargetId = ItemCraftRecipe->itemCraftRecipeId;
			if (INVALID_CRAFT_ID == CurrentCraftTargetId)
			{
				GSLOG(Error, TEXT("INVALID_CRAFT_ID == CurrentCraftTargetId, ItemCraftRecipe->itemCraftRecipeId:%d"), ItemCraftRecipe->itemCraftRecipeId);
				continue;
			}

			const FGsSchemaItemCraftRestrict* CraftRestrict = nullptr;
			if (!CraftRestrictTable->FindRowById(CurrentCraftTargetId, CraftRestrict) || nullptr == CraftRestrict)
			{
				// GSLOG(Log, TEXT("!CraftRestrictTable->FindRowById(CurrentCraftTargetId, CraftRestrict) || nullptr == CraftRestrict, CurrentCraftTargetId:%d"), CurrentCraftTargetId);
			}

			_allCraftTargets.Emplace(CurrentCraftTargetId, FGsCraftTarget(ItemCraftRecipe, CraftRestrict));
		}

		// Map 이 변경될 경우, 이전에 획득한 변수 포인터 등이 유효하지 않게 되므로 반복문 분리.
		for (const TPair<CraftId, FGsCraftTarget>& CraftTargetPair : _allCraftTargets)
		{
			const FGsCraftTarget& CraftTarget = CraftTargetPair.Value;

			const CraftTabId CurrentCraftTabId = CraftTarget._tableData->itemCraftTabId;
			FGsCraftTab* CraftTab = _allTabs.Find(CurrentCraftTabId);
			if (nullptr == CraftTab)
			{
				GSLOG(Error, TEXT("nullptr == CraftTab, CurrentCraftTabId:%d"), CurrentCraftTabId);
				continue;
			}

			//GSLOG(Log, TEXT("UGsCraftManager::InitliazeData(), CurrentCraftTabId:%d, CraftTarget.GetCraftTabId():%d, CraftTarget.CanDisplay():%d"), CurrentCraftTabId, CraftTarget.GetCraftTabId(), CraftTarget.CanDisplay());
			CraftTab->AddCraftTarget(&CraftTarget);
		}
	}

	return true;
}

bool UGsCraftManager::TryToNavigate(const CraftId InCraftId, OUT FText* OutFailMessage) const
{
	if (!CanDisplay(InCraftId))
	{
		if (nullptr != OutFailMessage)
		{
			*OutFailMessage = NSLOCTEXT("CraftText", "ItIsNotACrafTargetItem", "제작할 수 없는 아이템입니다.");
		}
		return false;
	}

	FGsUnlockManager* UnlockManager = GSUnlock();
	if (nullptr == UnlockManager)
	{
		GSLOG(Error, TEXT("nullptr == CraftManager"));
		return false;
	}

	// 잠김체크
	if (!UnlockManager->IsContentsUnlock(EGsUnlockContentsType::CRAFT))
	{
		if (nullptr != OutFailMessage)
		{
			UnlockManager->GetUnLockMessage(EGsUnlockContentsType::CRAFT, *OutFailMessage);
		}
		return false;
	}

	const FGsCraftTarget* CraftTarget = GetCraftTarget(InCraftId);
	if (nullptr == CraftTarget)
	{
		GSLOG(Error, TEXT("nullptr == CraftTarget"));
		return false;
	}

	FGsCraftMessageParamOpenCraftWith(InCraftId).Send();
	return true;
}

const FGsCraftTab* UGsCraftManager::GetTab(const CraftTabId InCraftTabId) const
{
	return _allTabs.Find(InCraftTabId);
}

const FGsCraftTab* UGsCraftManager::GetDisplayableTab(const CraftTabId InCraftTabId) const
{
	const FGsCraftTab* CraftTab = _allTabs.Find(InCraftTabId);
	if (nullptr == CraftTab || !CraftTab->CanDisplay())
	{
		return nullptr;
	}

	return CraftTab;
}

const FGsCraftTab* UGsCraftManager::GetRootTab() const
{
	return GetTab(INVALID_CRAFT_TAB_ID);
}

const FGsCraftTab* UGsCraftManager::GetFavoriteTab() const
{
	return GetTab(_favoriteTabId);
}

FGsCraftTab* UGsCraftManager::GetFavoriteTabInner()
{
	if (INVALID_CRAFT_TAB_ID == _favoriteTabId)
	{
		return nullptr;
	}

	return GetTabInner(_favoriteTabId);
}

FGsCraftTab* UGsCraftManager::GetTabInner(const CraftTabId InCraftTabId)
{
	return const_cast<FGsCraftTab*>(GetTab(InCraftTabId));
}

const FGsCraftTab* UGsCraftManager::FindDefaultLeafTab(const FGsCraftTab* InCraftTab) const
{
	const FGsCraftTab* FoundCraftTab = InCraftTab;
	const FGsCraftTab* CraftTabIt = InCraftTab;
	while (nullptr != CraftTabIt)
	{
		FoundCraftTab = CraftTabIt;
		CraftTabIt = FoundCraftTab->FindDefaultChildTab();
	}

	return FoundCraftTab;
}

void UGsCraftManager::RequestCraftLimitCountAll() const
{
	TSet<CraftTabId> CraftTabIds;

	for (const TPair<CraftId, FGsCraftTarget>& CraftTargetPair : _allCraftTargets)
	{
		if (CraftTargetPair.Value.HasAnyLimitCount())
		{
			const FGsCraftTarget& CraftTarget = CraftTargetPair.Value;
			CraftTabIds.Add(CraftTarget.GetCraftTabId());
		}
	}

	for (const CraftTabId& CurrentCraftTabId : CraftTabIds)
	{
		FGsNetSendServiceCraft::REQ_CRAFT_STAT_LIST(CurrentCraftTabId);
	}
}

void UGsCraftManager::RequestCraftLimitCountForTabId(const CraftTabId InCraftTabId) const
{
	FGsNetSendServiceCraft::REQ_CRAFT_STAT_LIST(InCraftTabId);
}

void UGsCraftManager::RequestCraftLimitCount() const
{
	TSet<CraftTabId> CraftTabIds;

	for (const TPair<CraftId, FGsCraftTarget>& CraftTargetPair : _allCraftTargets)
	{
		const FGsCraftTarget& CraftTarget = CraftTargetPair.Value;
		if (CraftTarget.HasAnyLimitCount())
		{
			CraftTabIds.Add(CraftTarget.GetCraftTabId());
		}
	}

	for (const CraftTabId& CurrentCraftTabId : CraftTabIds)
	{
		FGsNetSendServiceCraft::REQ_CRAFT_STAT_LIST(CurrentCraftTabId);
	}
}

const FGsCraftTarget* UGsCraftManager::GetCraftTarget(const CraftId InCraftTargetId) const
{
	if (INVALID_CRAFT_ID == InCraftTargetId)
	{
		return nullptr;
	}

	const FGsCraftTarget* CraftTarget = _allCraftTargets.Find(InCraftTargetId);
	//if (nullptr == CraftTarget)
	//{
	//	GSLOG(Log, TEXT("nullptr == CraftTarget, InCraftTargetId:%d"), InCraftTargetId);
	//}

	return CraftTarget;
}

bool UGsCraftManager::IsFavorite(const CraftId InCraftTargetId) const
{
	const FGsCraftTab* CraftTab = GetFavoriteTab();
	if (nullptr == CraftTab)
	{
		GSLOG(Warning, TEXT("CraftTab 테이블에 즐겨찾기 탭을 추가해 주세요."));
		return false;
	}

	return CraftTab->HasCraftTarget(InCraftTargetId);
}

bool UGsCraftManager::CanRegisterToFavorite(const bool InUseAlert) const
{
	const FGsCraftTab* CraftTab = GetFavoriteTab();
	if (nullptr == CraftTab)
	{
		GSLOG(Warning, TEXT("CraftTab 테이블에 즐겨찾기 탭을 추가해 주세요."));
		return false;
	}

	if (CRAFT_FAVORITE_MAX_COUNT <= CraftTab->GetCraftTargetCount())
	{
		if (InUseAlert)
		{
			FText Text;
			FText::FindText(TEXT("CraftText"), TEXT("Ticker_Bookmark_Maxcount"), Text);
			FGsUIHelper::TrayMessageTicker(Text);
		}
		return false;
	}

	return true;
}

void UGsCraftManager::ResetFavoriteAll(const TArray<CraftId> InCraftIds)
{
	_favoriteInitialized = true;

	FGsCraftTab* FavoriteTab = GetFavoriteTabInner();
	if (nullptr == FavoriteTab)
	{
		GSLOG(Warning, TEXT("nullptr == FavoriteTab"));
		return;
	}

	FavoriteTab->ClearCraftTargets();

	// InCraftIds 배열은 오름차순이므로 출력 순서 규칙에 맞게 뒤부터 입력하여 내림차순으로 변환.
	for (int32 i = InCraftIds.Num() - 1; 0 <= i; --i)
	{
		const CraftId CurrentCraftId = InCraftIds[i];
		const FGsCraftTarget* CraftTarget = GetCraftTarget(CurrentCraftId);
		if (nullptr == CraftTarget)
		{
			GSLOG(Error, TEXT("제작 즐겨찾기에 알 수 없는 제작레시피ID가 할당되어 있음. CurrentCraftId:%d"), CurrentCraftId);
			continue;
		}

		FavoriteTab->AddCraftTarget(CraftTarget);
	}

	FGsCraftMessageParamOnChangeFavoriteList().Send();
}

void UGsCraftManager::RegisterFavorite(const CraftId InCraftTargetId)
{
	const FGsCraftTarget* CraftTarget = GetCraftTarget(InCraftTargetId);
	if (nullptr == CraftTarget)
	{
		GSLOG(Error, TEXT("nullptr == CraftTarget, InCraftTargetId:%d"), InCraftTargetId);
		return;
	}

	FGsCraftTab* FavoriteTab = GetFavoriteTabInner();
	if (nullptr == FavoriteTab)
	{
		GSLOG(Warning, TEXT("nullptr == FavoriteTab"));
		return;
	}

	// 제작즐겨찾기는 즐겨찾기추가일시 내림차순
	FavoriteTab->InsertCraftTarget(0, CraftTarget);

	FGsCraftMessageParamOnChangeCraftTarget(InCraftTargetId).Send();
}

void UGsCraftManager::UnregisterFavorite(const CraftId InCraftTargetId)
{
	FGsCraftTab* FavoriteTab = GetFavoriteTabInner();
	if (nullptr == FavoriteTab)
	{
		GSLOG(Warning, TEXT("nullptr == FavoriteTab"));
		return;
	}

	FavoriteTab->RemoveCraftTarget(InCraftTargetId);

	FGsCraftMessageParamOnChangeCraftTarget(InCraftTargetId).Send();
}

CraftId UGsCraftManager::FindAvaialbleCraftId(const ItemId InItemId) const
{
	const FGsCraftTarget* CraftTarget = FindAvaialbleCraftTarget(InItemId);
	return (nullptr != CraftTarget) ? CraftTarget->_craftTargetId : INVALID_CRAFT_ID;
}

const FGsCraftTarget* UGsCraftManager::FindAvaialbleCraftTarget(const ItemId InItemId) const
{
	const UGsSupplyManager* SupplyManager = GSSupply();
	if (nullptr == SupplyManager)
	{
		GSLOG(Error, TEXT("nullptr == SupplyManager"));
		return nullptr;
	}

	const TSet<int32>* CraftIdSet = SupplyManager->LookupSupplyInfoData<TSet<int32>>(InItemId);
	if (nullptr == CraftIdSet)
	{
		return nullptr;
	}

	for (const int32& CraftIdIt : *CraftIdSet)
	{
		const FGsCraftTarget* CraftTarget = GetCraftTarget(CraftIdIt);
		if (nullptr == CraftTarget)
		{
			// CraftTarget can not be null.
			GSLOG(Error, TEXT("nullptr != CraftTarget"));
			continue;
		}

		if (CraftTarget->CanDisplay())
		{
			// 첫번째로 유효한 제작을 반환.
			return CraftTarget;
		}
	}

	return nullptr;
}

void UGsCraftManager::SetCraftTargetLimitCount(const CraftTabId InCraftCategoryId, const TArray<CraftIdAmountPair>& InCraftStatListServer, const TArray<CraftIdAmountPair>& InCraftStatListServerAccount)
{
	_limitCountInitialized = true;

	for (const CraftIdAmountPair& CraftIdAmountIt : InCraftStatListServer)
	{
		FGsCraftTarget* CraftTarget = _allCraftTargets.Find(CraftIdAmountIt.mCraftId);
		if (nullptr == CraftTarget)
		{
			GSLOG(Error, TEXT("nullptr == CraftTarget, CraftIdAmountIt.mCraftId:%d"), CraftIdAmountIt.mCraftId);
			continue;
		}

		CraftTarget->SetLimitCountServerCrafted(CraftIdAmountIt.mItemAmount);
	}

	for (const CraftIdAmountPair& CraftIdAmountIt : InCraftStatListServerAccount)
	{
		FGsCraftTarget* CraftTarget = _allCraftTargets.Find(CraftIdAmountIt.mCraftId);
		if (nullptr == CraftTarget)
		{
			GSLOG(Error, TEXT("nullptr == CraftTarget, CraftIdAmountIt.mCraftId:%d"), CraftIdAmountIt.mCraftId);
			continue;
		}

		CraftTarget->SetLimitCountAccountByServerCrafted(CraftIdAmountIt.mItemAmount);
	}
}

TArray<const FGsCraftTab*> UGsCraftManager::MakeTabRouteFromRoot(const CraftId InCraftTargetId) const
{
	TArray<const FGsCraftTab*> CraftTabOrder;

	if (INVALID_CRAFT_ID == InCraftTargetId)
	{
		// 제작 대상이 특정되지 않은 경우, 첫번째 메인텝의 첫번째 서브탭
		const FGsCraftTab* CraftTab = GetRootTab();
		while (nullptr != CraftTab)
		{
			CraftTabOrder.Add(CraftTab);

			const FGsCraftTab* TempTab = CraftTab->FindDefaultChildTab();
			if (nullptr == TempTab)
			{
				break;
			}

			CraftTab = TempTab;
		}

		if (nullptr == CraftTab)
		{
			GSLOG(Error, TEXT("nullptr == CraftTab"));
		}
	}
	else
	{
		const FGsCraftTarget* CraftTarget = GetCraftTarget(InCraftTargetId);
		if (nullptr == CraftTarget)
		{
			GSLOG(Error, TEXT("nullptr == CraftTarget, InCraftTargetId:%d"), InCraftTargetId);
		}
		else
		{
			const FGsCraftTab* CraftTab = FindDefaultLeafTab(CraftTarget->_craftTab);
			if (nullptr == CraftTab)
			{
				GSLOG(Error, TEXT("nullptr == CraftTab"));
			}

			while (CraftTab != nullptr)
			{
				CraftTabOrder.Insert(CraftTab, 0);
				CraftTab = CraftTab->GetParentTab();
			}
		}
	}

	return CraftTabOrder; //nrvo
}

bool UGsCraftManager::CanDisplay(const CraftId InCraftId) const
{
	if (INVALID_CRAFT_ID == InCraftId)
	{
		return false;
	}

	const FGsCraftTarget* CraftTarget = GetCraftTarget(InCraftId);
	return (nullptr != CraftTarget) && CraftTarget->CanDisplay();
}

bool UGsCraftManager::CanCraftNow(const CraftId InCraftId, const ItemAmount InCraftCount, const bool InUseUiFeedback /*= false*/) const
{
	FText TickerErrorMessage;

	// 함수 나갈 때가 아니라, 실제로는 스코프 나갈 때라서, ON_SCOPE_EXIT 을 이용해 티커를 처리하다 보니
	// 어떤 티커 메시지도 출력되지 않는 문제가 있어 ScopeGuard 방식을 수정하겠습니다
	if (InUseUiFeedback)
	{
		ON_SCOPE_EXIT
		{
			// 함수 나갈 때, 출력해야할 티커 메세지가 유효하면 출력. 아니면 생략.
			if (!TickerErrorMessage.IsEmpty())
			{
				FGsUIHelper::TrayMessageTicker(TickerErrorMessage);
			}
		};
	}

	// 제작이 가능한 인벤토리 무게 패널티 등급들
	static const TSet<WeightPenaltyGrade> VALID_INVENTORY_WEIGHT_PENALTY_GRADES_TO_CRAFT = { WeightPenaltyGrade::NONE, WeightPenaltyGrade::FIRST };

	// 인벤 체크
	UGsItemManager* ItemManager = GItem();
	if (nullptr == ItemManager)
	{
		GSLOG(Error, TEXT("nullptr == ItemManager"));
		return false;
	}

	const bool TooHeavyToCraft = !VALID_INVENTORY_WEIGHT_PENALTY_GRADES_TO_CRAFT.Contains(ItemManager->GetWeightPenaltyGradeType());

	// 제작대상 체크
	const FGsCraftTarget* CraftTarget = GetCraftTarget(InCraftId);
	if (nullptr == CraftTarget)
	{
		GSLOG(Error, TEXT("nullptr == CraftTarget, InCraftId:%d, InCraftCount:%d, InUsePopupMsg:%d"), InCraftId, InCraftCount);
		return false;
	}

	// 재화/스택 가능한 아이템일 경우까지 체크해야 함
	const FGsCraftTargetObject* CraftTargetObject = CraftTarget->GetDefaultCraftTargetObject();
	if (nullptr == CraftTargetObject)
	{
		GSLOG(Error, TEXT("nullptr == CraftTargetObject"));
		return false;
	}

	bool enoughSlot = true;

	if (CostType::ITEM == CraftTargetObject->GetCostType())
	{
		ItemId itemId = CraftTargetObject->GetItemId();

		TArray<TPair<int32, ItemAmount>> itemTIdAmountPairList;
		itemTIdAmountPairList.Emplace(itemId, InCraftCount);
		enoughSlot = ItemManager->IsCanItemListAdd(itemTIdAmountPairList);
	}
	else if (CostType::CURRENCY == CraftTargetObject->GetCostType())
	{
		//enoughSlot = true;
	}
	else
	{
		GSLOG(Error, TEXT("Unknown CostType"));
		return false;
	}

	// 1. 티커 처리 : 슬롯이 부족한 경우
	if(!enoughSlot)
	{
		FText::FindText(TEXT("ItemCraftText"), TEXT("Craft_UI_InventoryCheck"), TickerErrorMessage);

#if WITH_EDITOR
		if (TickerErrorMessage.IsEmpty())
		{
			TickerErrorMessage = FText::FromString(TEXT("ItemCraftText.Craft_UI_InventoryCheck"));
		}
#endif

		if (InUseUiFeedback && TickerErrorMessage.IsEmpty() == false)
		{
			FGsUIHelper::TrayMessageTicker(TickerErrorMessage);
		}

		return false;
	}

	// 2. 티커 처리 : 무게가 초과한 경우
	if (TooHeavyToCraft)
	{
		FText::FindText(TEXT("ItemCraftText"), TEXT("Craft_UI_WeightCheck"), TickerErrorMessage);

#if WITH_EDITOR
		if (TickerErrorMessage.IsEmpty())
		{
			TickerErrorMessage = FText::FromString(TEXT("ItemCraftText.Craft_UI_WeightCheck"));
		}
#endif

		if (InUseUiFeedback && TickerErrorMessage.IsEmpty() == false)
		{
			FGsUIHelper::TrayMessageTicker(TickerErrorMessage);
		}

		return false;
	}

	// 제작 공통 체크
	if (!CraftTarget->CanCraftNow(&TickerErrorMessage))
	{
		if (InUseUiFeedback && TickerErrorMessage.IsEmpty() == false)
		{
			FGsUIHelper::TrayMessageTicker(TickerErrorMessage);
		}

		return false;
	}

	// 비용 체크
	TSharedRef<FGsCostPackageMultiple> CostPackageMultiple = CraftTarget->MakeCostPackageMultiple();
	CostPackageMultiple->SetMultiple(InCraftCount);
	if (!CostPackageMultiple->PlayerHasEnoughMultiple())
	{
		//GSLOG(Error, TEXT("!CostPackageMultiple->PlayerHasEnoughMultiple(), InCraftId:%d, InCraftCount:%d, InUsePopupMsg:%d"), InCraftId, InCraftCount, InUsePopupMsg);
		if (InUseUiFeedback)
		{
			// 재화부족팝업이 정상 오픈되면, 티커는 생략함. 어떠한 이유에서든지 팝업 오픈 실패하면 티커로 대체
			if (!FGsCurrencyHelper::OpenLackCostPackageMultiplePopup(*CostPackageMultiple))
			{
				FText::FindText(TEXT("NetText"), PD::ResultEnumToString(PD::Result::CRAFT_ERROR_LACK_OF_MATERIAL), TickerErrorMessage);

#if WITH_EDITOR
				if (TickerErrorMessage.IsEmpty())
				{
					TickerErrorMessage = FText::FromString(PD::ResultEnumToString(PD::Result::CRAFT_ERROR_LACK_OF_MATERIAL));
				}
#endif

			}
		}

		return false;
	}

	// 복수 구매 하드캡
	if (CRAFT_MAX_COUNT < InCraftCount)
	{
		FText::FindText(TEXT("NetText"), PD::ResultEnumToString(PD::Result::CRAFT_ERROR_EXCEED_MAX_CRAFT_COUNT), TickerErrorMessage);

#if WITH_EDITOR
		if (TickerErrorMessage.IsEmpty())
		{
			TickerErrorMessage = FText::FromString(PD::ResultEnumToString(PD::Result::CRAFT_ERROR_EXCEED_MAX_CRAFT_COUNT));
		}
#endif
		if (InUseUiFeedback && TickerErrorMessage.IsEmpty() == false)
		{
			FGsUIHelper::TrayMessageTicker(TickerErrorMessage);
		}

		return false;
	}

	return true;
}

bool UGsCraftManager::TryCraft(const CraftId InCraftTargetId, const FGsCostPackageMultiple& InCostPackageMultiple) const
{
	const int32 CraftCount = InCostPackageMultiple.GetMultiple();
	if (0 >= CraftCount)
	{
		GSLOG(Error, TEXT("0 >= CraftCount"));
		return false;
	}

	UGsItemManager* ItemManager = GItem();
	if (nullptr == ItemManager)
	{
		GSLOG(Error, TEXT("nullptr == ItemManager"));
		return false;
	}

	TArray<ItemDBIdAmountPair> InMaterialInfos;
	if (!ItemManager->MakeDisposableItems(InMaterialInfos, InCostPackageMultiple))
	{
		GSLOG(Warning, TEXT("!ItemManager->MakeDisposableItems(InMaterialInfos, InCostPackageMultiple), InCraftTargetId:%d"), InCraftTargetId);
		return false;
	}

	return FGsNetSendServiceCraft::REQ_CRAFT(InCraftTargetId, CraftCount, MoveTemp(InMaterialInfos));
}

void UGsCraftManager::NotifyCraftStatListWorldChanged(const IGsMessageParam* InParam)
{
	const FGsCraftMessageParamNotifyCraftStatListWorldChanged* Message = InParam->Cast<const FGsCraftMessageParamNotifyCraftStatListWorldChanged>();
	if (nullptr == Message)
	{
		GSLOG(Error, TEXT("nullptr == Message"));
	}

	RequestCraftLimitCountAll();
}

void UGsCraftManager::AckCraftStatList(const IGsMessageParam* InParam)
{
	const FGsCraftMessageParamAckCraftStatList* Message = InParam->Cast<const FGsCraftMessageParamAckCraftStatList>();
	if (nullptr == Message)
	{
		GSLOG(Error, TEXT("nullptr == Message"));
		return;
	}

	SetCraftTargetLimitCount(Message->_craftTabId, Message->_craftStatListServer, Message->_craftStatListServerAccount);
}

void UGsCraftManager::TryChangeFavoriteCraftTarget(const IGsMessageParam* InParam)
{
	const FGsCraftMessageParamTryChangeFavoriteCraftTarget* Message = InParam->Cast<const FGsCraftMessageParamTryChangeFavoriteCraftTarget>();

	if (Message->_on)
	{
		if (CanRegisterToFavorite(true))
		{
			FGsNetSendServiceCraft::REQ_CRAFT_FAVORITES_CREATE(Message->_craftTargetId);
		}
	}
	else
	{
		FGsNetSendServiceCraft::REQ_CRAFT_FAVORITES_DELETE(Message->_craftTargetId);
	}
}

void UGsCraftManager::OnOpenTab(const IGsMessageParam* InParam)
{
	if (!_limitCountInitialized)
	{
		RequestCraftLimitCountAll();
	}

	if (!_favoriteInitialized)
	{
		FGsNetSendServiceCraft::REQ_CRAFT_FAVORITES_LIST();
	}
}

void UGsCraftManager::OnResetTimeContents(const IGsMessageParam* InParam)
{
	const FGsPrimitiveInt32* Param = InParam->Cast<const FGsPrimitiveInt32>();
	if (nullptr == Param)
	{
		GSLOG(Error, TEXT("nullptr == Param"));
		return;
	}

	//const ResetTimeTicket CurrentResetTimeTicket = StaticCast<ResetTimeTicket>(Param->_data);
	RequestCraftLimitCount();
}
