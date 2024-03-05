#include "GsGameStateCraft.h"

#include "DataSchema/Item/GsSchemaItemCommon.h"
#include "GameFlow/GameContents/GsContentsMode.h"
#include "GsContentsGameBase.h"
#include "Item/Craft/GsCraftTarget.h"
#include "Item/GsItem.h"
#include "Item/GsItemManager.h"
#include "Management/GsMessageHolder.h"
#include "Management/ScopeGame/GsCraftManager.h"
#include "Management/ScopeGame/GsUnlockManager.h"
#include "Management/ScopeGlobal/GsGameFlowManager.h"
#include "Management/ScopeGlobal/GsUIManager.h"
#include "Message/GsMessageContents.h"
#include "Message/GsMessageItem.h"
#include "Message/GsMessageUserInfo.h"
#include "Message/MessageParam/GsCraftMessageParam.h"
#include "Message/MessageParam/GsItemMessageParam.h"
#include "Message/MessageParam/GsMessageParam.h"
#include "Shared/Client/SharedEnums/SharedItemEnum.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "T1Project.h"
#include "UI/UIContent/Tray/Craft/GsUICraftResultListItem.h"
#include "UI/UIContent/Tray/GsUITrayCraftResult.h"
#include "UI/UIContent/Window/GsUIWindowCraft.h"
#include "UI/UILib/Base/GsUIWidgetBase.h"
#include "Unlock/GsUnlockDefine.h"

FGsGameStateCraft::FGsGameStateCraft()
	: Super(FGsContentsMode::InGame::ContentsCraft)
{
}

void FGsGameStateCraft::Init()
{
	Super::Init();

	FGsMessageHolder* MessageHolder = GMessage();
	if (nullptr != MessageHolder)
	{
		_openCraftWithDelegate = MessageHolder->GetCraft().AddRaw(FGsCraftMessageParamOpenCraftWith::MESSAGE_ID, this, &FGsGameStateCraft::OpenCraftWith);
	}
	else
	{
		GSLOG(Error, TEXT("nullptr == MessageHolder"));
	}
}

void FGsGameStateCraft::Close()
{
	ClearHistory();

	FGsMessageHolder* MessageHolder = GMessage();
	if (nullptr != MessageHolder)
	{
		MessageHolder->GetCraft().Remove(_openCraftWithDelegate);
	}


	Super::Close();
}

void FGsGameStateCraft::Enter()
{
	Super::Enter();

	_window = UGsUIWindowCraft::OpenWindow(this);

	SetNavigationBar(CurrencyType::NONE);

	RegisterMessages();

	if (_window.IsValid())
	{
		if (!HasHistory())
		{
			// 히스토리가 없다면, 아무런 파라미터 없이 본 state 로 진입한 경우.
			// 초기화에 필요한 아무런 데이터가 없는 상태이므로, 디폴트 데이터를 히스토리에 입력해 줌.
			PushHistory(UGsUIWindowCraft::Parameters(INVALID_CRAFT_ID));
			//_window->SetParameters(UGsUIWindowCraft::Parameters(INVALID_CRAFT_ID), false);
		}
		else
		{
			// 히스토리가 있다면,
			// * OpenCraftWith() 등을 통해 초기화 데이터가 있는 상태로 진입한 경우. 현재 네비상태를 저장하지 않음.
			// * 뒤로가기 등으로 스택에서부터 윈도우가 복구되는 경우. 현재 네비상태를 저장하지 않음.
			//const bool NeedPushCurrentToHistory = !_window->IsStackRecoverFlag();
		}

		UGsUIWindowCraft::Parameters LastParameters;
		if (PopHistory(LastParameters))
		{
			_window->SetParameters(LastParameters, false);
		}
	}
	else
	{
		GSLOG(Warning, TEXT("NOT _window.IsValid()"));
	}
}

void FGsGameStateCraft::Exit()
{
	// 메세지 해지
	UnregisterMessages();

	Super::Exit();
}

bool FGsGameStateCraft::CanEnter(bool InShowUnableMessage /*= false*/) const
{
	if (GSUnlock()->IsContentsUnlock(EGsUnlockContentsType::CRAFT))
	{
		return true;
	}

	if (InShowUnableMessage)
	{
		GSUnlock()->ShowLockTicker(EGsUnlockContentsType::CRAFT);
	}
	return false;
}

bool FGsGameStateCraft::HasHistory() const
{
	return 0 < _historyStack.Num();
}

void FGsGameStateCraft::PushHistory(UGsUIWindowCraft::Parameters InParameters)
{
	if (0 < _historyStack.Num())
	{
		if (_historyStack.Top() == InParameters)
		{
			// Top 과 동일하면 스킵
			return;
		}
	}

	_historyStack.Push(MoveTemp(InParameters));
}

bool FGsGameStateCraft::PopHistory(OUT UGsUIWindowCraft::Parameters& OutHistory)
{
	if (HasHistory())
	{
		OutHistory = _historyStack.Pop(false);
		return true;
	}

	return false;
}

void FGsGameStateCraft::ClearHistory()
{
	_historyStack.Empty();
}

void FGsGameStateCraft::RegisterMessages()
{
	FGsMessageHolder* MessageHolder = GMessage();
	if (nullptr == MessageHolder)
	{
		GSLOG(Error, TEXT("nullptr == MessageHolder"));
		return;
	}

	_craftDelegates.Emplace(MessageHolder->GetCraft().AddRaw(FGsCraftMessageParamOnChangeCraftTarget::MESSAGE_ID, this, &FGsGameStateCraft::OnChangeCraftTarget));
	_craftDelegates.Emplace(MessageHolder->GetCraft().AddRaw(FGsCraftMessageParamOnChangeFavoriteList::MESSAGE_ID, this, &FGsGameStateCraft::OnChangeFavoriteList));
	_craftDelegates.Emplace(MessageHolder->GetCraft().AddRaw(FGsCraftMessageParamOnEndSpelling::MESSAGE_ID, this, &FGsGameStateCraft::OnEndSpelling));
	_craftDelegates.Emplace(MessageHolder->GetCraft().AddRaw(FGsCraftMessageParamOnCraftResult::MESSAGE_ID, this, &FGsGameStateCraft::OnCraftResult));

	_userInfoDelegates.Emplace(MessageHolder->GetUserBasicInfo().AddRaw(MessageUserInfo::CURRENCY, this, &FGsGameStateCraft::OnUser_Currency));

	_itemActionDelegates.Emplace(MessageHolder->GetItem().AddRaw(MessageItem::ADDITEMLIST, this, &FGsGameStateCraft::OnAddItemList));
	_itemActionDelegates.Emplace(MessageHolder->GetItem().AddRaw(MessageItem::UPDATEITEM, this, &FGsGameStateCraft::OnUpdateItem));
	_itemActionDelegates.Emplace(MessageHolder->GetItem().AddRaw(MessageItem::REMOVEITEM, this, &FGsGameStateCraft::OnRemoveItem));
}

void FGsGameStateCraft::UnregisterMessages()
{
	FGsMessageHolder* MessageHolder = GMessage();
	if (nullptr == MessageHolder)
	{
		GSLOG(Error, TEXT("nullptr == MessageHolder"));
		return;
	}

	for (const TPair<MessageContentCraft, FDelegateHandle>& DelegatePair : _craftDelegates)
	{
		MessageHolder->GetCraft().Remove(DelegatePair);
	}
	_craftDelegates.Empty();

	for (const TPair<MessageUserInfo, FDelegateHandle>& DelegatePair : _userInfoDelegates)
	{
		MessageHolder->GetUserBasicInfo().Remove(DelegatePair);
	}
	_userInfoDelegates.Empty();

	for (const TPair<MessageItem, FDelegateHandle>& DelegatePair : _itemActionDelegates)
	{
		MessageHolder->GetItem().Remove(DelegatePair);
	}
	_itemActionDelegates.Empty();
}

void FGsGameStateCraft::CurrencyChanged(const CurrencyType InCurrencyType)
{
	if (CurrencyType::NONE == InCurrencyType || !_window.IsValid())
	{
		return;
	}

	_window->OnChangeCurrency(InCurrencyType);
}

void FGsGameStateCraft::ItemChanged(const ItemId InItemId)
{
	if (INVALID_ITEM_ID == InItemId || !_window.IsValid())
	{
		return;
	}

	_window->OnChangeItem(InItemId);
}

void FGsGameStateCraft::OpenCraftWith(const IGsMessageParam* InParam)
{
	if (!CanEnter(true))
	{
		return;
	}

	const FGsCraftMessageParamOpenCraftWith* Param = InParam->Cast<const FGsCraftMessageParamOpenCraftWith>();
	if (nullptr == Param)
	{
		GSLOG(Error, TEXT("nullptr == Param"));
		return;
	}

	FGsGameFlowManager* GameFlowManager = GMode();
	if (nullptr == GameFlowManager)
	{
		GSLOG(Error, TEXT("nullptr == GameFlowManager"));
		return;
	}

	if (GameFlowManager->GetCurrentContentStateIndex() != FGsContentsMode::InGame::ContentsCraft)
	{
		PushHistory(UGsUIWindowCraft::Parameters(Param->_craftTargetId));

		FGsMessageHolder* MessageHolder = GMessage();
		if (nullptr == MessageHolder)
		{
			GSLOG(Error, TEXT("nullptr == MessageHolder"));
			return;
		}

		MessageHolder->GetContents().SendMessage(MessageContents::ACTIVE_WINDOW_CRAFT);
	}
	else
	{
		if (!_window.IsValid())
		{
			GSLOG(Warning, TEXT("Craft State 이지만, Windows 가 없는 상태"));
			PushHistory(UGsUIWindowCraft::Parameters(Param->_craftTargetId));
		}
		else
		{
			_window->SetParameters(UGsUIWindowCraft::Parameters(Param->_craftTargetId));
		}
	}
}

void FGsGameStateCraft::OnChangeCraftTarget(const IGsMessageParam* InParam)
{
	if (!_window.IsValid())
	{
		return;
	}

	const FGsCraftMessageParamOnChangeCraftTarget* Param = InParam->Cast<const FGsCraftMessageParamOnChangeCraftTarget>();
	if (nullptr == Param)
	{
		GSLOG(Error, TEXT("nullptr == Param"));
		return;
	}

	_window->CraftTargetUpdated(Param->_craftTargetId);
}

void FGsGameStateCraft::OnChangeFavoriteList(const IGsMessageParam*)
{
	if (!_window.IsValid())
	{
		return;
	}

	_window->InvalidateCraftTargetListEntries();
}

void FGsGameStateCraft::OnEndSpelling(const IGsMessageParam* InParam)
{
	if (!_window.IsValid())
	{
		return;
	}

	const FGsCraftMessageParamOnEndSpelling* Message = InParam->Cast<const FGsCraftMessageParamOnEndSpelling>();
	if (nullptr == Message)
	{
		GSLOG(Error, TEXT("nullptr == Message"));
		return;
	}

	_window->EndSpelling(Message->_craftTargetId, Message->_craftCount);
}

void FGsGameStateCraft::OnCraftResult(const IGsMessageParam* InParam)
{
	const FGsCraftMessageParamOnCraftResult* Message = InParam->Cast<const FGsCraftMessageParamOnCraftResult>();
	if (nullptr == Message)
	{
		GSLOG(Error, TEXT("nullptr == Message"));

		if (_window.IsValid())
		{
			// 윈도우가 유효하다면, 이벤트를 받아서 처리해야할 것이 있으므로, 어쨋든 이벤트는 호출해 주어야 함.
			_window->OnCraftResult(false);
		}

		return;
	}

	if (_window.IsValid())
	{
		_window->OnCraftResult(Message->_isSuccess);
	}

	if (!Message->_isSuccess)
	{
		GSLOG(Error, TEXT("!Message->_isSuccess"));
		return;
	}

	const UGsCraftManager* CraftManager = GSCraft();
	if (nullptr == CraftManager)
	{
		GSLOG(Error, TEXT("nullptr == CraftManager"));
		return;
	}

	const FGsCraftTarget* CraftTarget = CraftManager->GetCraftTarget(Message->_craftId);
	if (nullptr == CraftTarget)
	{
		GSLOG(Error, TEXT("nullptr == CraftTarget"));
		return;
	}

	TArray<FCraftResultParameters> CraftResultListItem;
	CraftResultListItem.Reset(Message->_results.Num());

	for (int32 i = 0; i < Message->_results.Num(); ++i)
	{
		const ItemDBIdCraftResultPair& CurrentCraftResult = Message->_results[i];
		FText ItemName = FText::GetEmpty();
		FSoftObjectPath ResultItemIconObjectPath = nullptr;

		const FGsCraftTargetObject* CraftTargetObject = CraftTarget->GetCraftTargetObjectByResult(CurrentCraftResult.mCraftResult);
		if (nullptr == CraftTargetObject)
		{
			// 제작 결과에 지정된 아이템이 없는 경우, 기본 제작 아이템을 출력
			GSLOG(Log, TEXT("nullptr == CraftTargetObject, Message->_craftId:%d, CurrentCraftResult.mCraftResult:%d"), Message->_craftId, CurrentCraftResult.mCraftResult);
			CraftTargetObject = CraftTarget->GetDefaultCraftTargetObject();
		}

		if (nullptr == CraftTargetObject)
		{
			// 제작 대상 정보가 없음.
			GSLOG(Error, TEXT("nullptr == CraftTargetObject"));
		}
		else
		{
			ItemName = CraftTargetObject->GetCommonName();
			ResultItemIconObjectPath = CraftTargetObject->GetCommonIconPath();
			if (!ResultItemIconObjectPath.IsValid())
			{
				GSLOG(Error, TEXT("!ResultItemIconObjectPath.IsValid(), Message->_craftId:%d"), Message->_craftId);
			}
		}

		// 아이템 등급 설정 (ItemDBId 로 아이템 등급 조회)
		UGsItemManager* itemManager = GItem();
		if (nullptr == itemManager)
		{
			GSLOG(Error, TEXT("nullptr == itemManager"));
			return;
		}
		ItemGrade itemGrade = itemManager->GetItemGradeByDBId(CurrentCraftResult.mItemDBId);
		
		// mItemDBId 가 0 이라면, 실패시 아무것도 돌려주지 않는 경우일 것이다.
		ItemAmount itemAmount = (CurrentCraftResult.mItemDBId == INVALID_ITEM_DB_ID) ? 0 : CurrentCraftResult.mItemAmount;

		// 제작 결과가 만약 장비 아이템이라면... 
		bool isEquipment = itemManager->IsItemEquipmentByDBId(CurrentCraftResult.mItemDBId);

		// 예를 들어 팔찌와 참의 경우, 강화 단계를 표기해야 한다
		int32 enchantLevel = itemManager->GetItemEnchantLevelByDBId(CurrentCraftResult.mItemDBId);

		FCraftResultParameters ResultListItemParameters = {
			MoveTemp(ItemName),
			itemAmount,
			MoveTemp(ResultItemIconObjectPath),
			CurrentCraftResult.mCraftResult,
			itemGrade,
			isEquipment,
			enchantLevel
		};

		CraftResultListItem.Emplace(ResultListItemParameters);
	}

	// 서버에서 주는 정보는 '성공 - 대성공 - 실패' 순으로 정렬되어 있음
	// 결과값을 랜덤하게 보여주도록 개선 요청 (CHR-19552)
	const int32 lastIndex = CraftResultListItem.Num() - 1;
	for (int32 i = 0; i <= lastIndex; ++i)
	{
		const int32 randIndex = FMath::RandRange(0, lastIndex);
		if (i != randIndex)
		{
			CraftResultListItem.Swap(i, randIndex);
		}
	}


	UGsUIManager* UiManager = GUI();
	if (nullptr == UiManager)
	{
		return;
	}

	TWeakObjectPtr<UGsUIWidgetBase> Widget = UiManager->OpenAndGetWidget(TEXT("TrayCraftResult"));
	if (!Widget.IsValid())
	{
		GSLOG(Error, TEXT("!Widget.IsValid()"));
		return;
	}

	TWeakObjectPtr<UGsUITrayCraftResult> CraftResultTray = Cast<UGsUITrayCraftResult>(Widget.Get());
	if (!CraftResultTray.IsValid())
	{
		GSLOG(Error, TEXT("!CraftResultTray.IsValid()"));
		return;
	}

	UGsUITrayCraftResult::Parameters Parameters = {
		CraftTarget->CanBeFail(),
		CraftTarget->CanBeSuccess(),
		CraftTarget->CanBeGreatSuccess(),
		MoveTemp(CraftResultListItem)
	};

	CraftResultTray->SetParameters(MoveTemp(Parameters));

	// [U2] | ejrrb10 | 만약 해당 아이템이 계정별 제한이 걸려 있는 아이템일 경우, CG_REQ_CRAFT_STAT_LIST 요청을 명시적으로 함
	// 서버 제한이 걸려 있는 경우, 서버에서 자동으로 GC_NOTIFY_CRAFT_STAT_LIST_WORLD_CHANGED 패킷을 보내주지만,
	// 계정 제한만 걸려 있는 경우에는 서버에서 자동으로 따로 패킷을 보내주지 않아, 따로 보내 주어야 함 (바람직한 상황은 아님)
	if (CraftTarget->HasAnyLimitCount())
	{
		CraftManager->RequestCraftLimitCountForTabId(CraftTarget->GetCraftTabId());
	}
}

void FGsGameStateCraft::OnUser_Currency(uint64 InCurrncyType)
{
	CurrencyChanged(StaticCast<CurrencyType>(InCurrncyType));
}

void FGsGameStateCraft::OnAddItemList(FGsItemMessageParamBase& InParam)
{
	const FGsAddItemList* Param = StaticCast<const FGsAddItemList*>(&InParam);
	check(nullptr != Param);

	UGsItemManager* ItemManager = GItem();
	if (nullptr == ItemManager)
	{
		GSLOG(Error, TEXT("nullptr == ItemManager"));
		return;
	}

	TSet<ItemId> ItemIdSet;
	for (const ItemDBId CurrentItemDBId : Param->_itemDbidList)
	{
		const ItemId CurrentItemId = ItemManager->GetTIDByDBID(CurrentItemDBId);
		if (INVALID_ITEM_ID == CurrentItemId)
		{
			GSLOG(Error, TEXT("INVALID_ITEM_ID == CurrentItemId, CurrentItemDBId: %lld"), CurrentItemDBId);
			continue;
		}

		ItemIdSet.Add(CurrentItemId);
	}

	for (const ItemId CurrentItemId : ItemIdSet)
	{
		ItemChanged(CurrentItemId);
	}
}

void FGsGameStateCraft::OnUpdateItem(FGsItemMessageParamBase& InParam)
{
	const FGsUpdateItem* Param = StaticCast<const FGsUpdateItem*>(&InParam);
	check(Param);

	UGsItemManager* ItemManager = GItem();
	if (nullptr == ItemManager)
	{
		GSLOG(Error, TEXT("nullptr == ItemManager"));
		return;
	}

	const ItemId CurrentItemId = ItemManager->GetTIDByDBID(Param->_itemDBID);
	if (INVALID_ITEM_ID == CurrentItemId)
	{
		GSLOG(Error, TEXT("INVALID_ITEM_ID == CurrentItemId, Param->_itemDBID:%lld"), Param->_itemDBID);
		return;
	}

	ItemChanged(CurrentItemId);
}

void FGsGameStateCraft::OnRemoveItem(FGsItemMessageParamBase& InParam)
{
	const FGsRemoveItem* Param = StaticCast<const FGsRemoveItem*>(&InParam);
	check(Param);

	ItemChanged(Param->_itemTID);
}
