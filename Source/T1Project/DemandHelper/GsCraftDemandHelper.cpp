#include "GsCraftDemandHelper.h"

#include "DataSchema/DemandInfo/GsSchemaCurrencyDemandInfoSet.h"
#include "DataSchema/DemandInfo/GsSchemaItemDemandInfoSet.h"
#include "GsCommonDemandHelper.h"
#include "Management/GsScopeGame.h"
#include "Management/GsScopeHolder.h"
#include "Management/ScopeGame/GsCraftManager.h"
#include "Management/ScopeGame/GsSupplyManager.h"
#include "Management/ScopeGame/GsUnlockManager.h"
#include "Management/ScopeGlobal/GsUIManager.h"
#include "Shared/Client/SharedEnums/SharedCommonEnum.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "T1Project.h"
#include "UI/UIContent/Popup/GsUIPopupItemDropNavigator.h"
#include "UI/UILib/Base/GsUIWidgetBase.h"
#include "Unlock/GsUnlockDefine.h"

FGsCraftDemandHelper::FGsCraftDemandHelper()
	: Super(EGsUnlockContentsType::CRAFT)
{
}

UGsSupplyManager::EGsNavigationStatus FGsCraftDemandHelper::GetNavigationStatus(const FGsSchemaCurrencyDemandInfoSet& InDemandInfo) const
{
	bool IsValidNavigation = false;
	if (InDemandInfo.contentsTypeSet.Contains(_contentsType))
	{
		// 간단 버전 네비게이트가 유효한 상태
		IsValidNavigation = true;
	}
	else
	{
		// 상세 버전 네비게이트가 유효한지 추가 체크
		const UGsCraftManager* CraftManager = GSCraft();
		if (nullptr == CraftManager)
		{
			GSLOG(Error, TEXT("nullptr == CraftManager"));
			return UGsSupplyManager::EGsNavigationStatus::None;
		}

		for (const CraftId CurrentCraftId : FGsCraftDemandHelper::MakeDisplayableCraftIds(InDemandInfo.craftIds))
		{
			if (CraftManager->CanDisplay(CurrentCraftId))
			{
				IsValidNavigation = true;
				break;
			}
		}
	}

	if (IsValidNavigation)
	{
		return FGsBaseDemandHelper::GetNavigationStatusStatic(_contentsType, InDemandInfo.currencyType);
	}

	return UGsSupplyManager::EGsNavigationStatus::None;
}

bool FGsCraftDemandHelper::TryToNavigate(const FGsSchemaCurrencyDemandInfoSet& InCurrencyDemandInfoSet, OUT FText* OutFailMessage /*= nullptr*/) const
{
	return TryToNavigate(CostType::CURRENCY, InCurrencyDemandInfoSet.currencyType, INVALID_ITEM_ID, InCurrencyDemandInfoSet.craftIds, OutFailMessage);
}

UGsSupplyManager::EGsNavigationStatus FGsCraftDemandHelper::GetNavigationStatus(const FGsSchemaItemDemandInfoSet& InDemandInfo) const
{
	bool IsValidNavigation = false;
	if (InDemandInfo.contentsTypeSet.Contains(_contentsType))
	{
		// 간단 버전 네비게이트가 유효한 상태
		IsValidNavigation = true;
	}
	else
	{
		// 상세 버전 네비게이트가 유효한지 추가 체크
		const UGsCraftManager* CraftManager = GSCraft();
		if (nullptr == CraftManager)
		{
			GSLOG(Error, TEXT("nullptr == CraftManager"));
			return UGsSupplyManager::EGsNavigationStatus::None;
		}

		for (const CraftId CurrentCraftId : FGsCraftDemandHelper::MakeDisplayableCraftIds(InDemandInfo.craftIds))
		{
			if (CraftManager->CanDisplay(CurrentCraftId))
			{
				IsValidNavigation = true;
				break;
			}
		}
	}

	if (IsValidNavigation)
	{
		return FGsBaseDemandHelper::GetNavigationStatusStatic(_contentsType, InDemandInfo.itemId);
	}

	return UGsSupplyManager::EGsNavigationStatus::None;
}

bool FGsCraftDemandHelper::TryToNavigate(const FGsSchemaItemDemandInfoSet& InItemDemandInfoSet, OUT FText* OutFailMessage /*= nullptr*/) const
{
	return TryToNavigate(CostType::ITEM, CurrencyType::NONE, InItemDemandInfoSet.itemId, InItemDemandInfoSet.craftIds, OutFailMessage);
}

TArray<CraftId> FGsCraftDemandHelper::MakeDisplayableCraftIds(const TSet<int32>& InCraftIds)
{
	TArray<CraftId> CraftIds;

	const UGsCraftManager* CraftManager = GSCraft();
	if (nullptr != CraftManager)
	{
		for (const int32& CraftIdIt : InCraftIds)
		{
			const CraftId CurrentCraftId = StaticCast<CraftId>(CraftIdIt);
			if (CraftManager->CanDisplay(CurrentCraftId))
			{
				CraftIds.Add(CurrentCraftId);
			}
		}
	}

	return CraftIds; //rvo
}

bool FGsCraftDemandHelper::TryToNavigate(const CostType InCostType, const CurrencyType InCurrencyType, const ItemId InItemId, const TSet<int32>& InCraftIds, OUT FText* OutFailMessage)
{
	TArray<CraftId> CraftIds = FGsCraftDemandHelper::MakeDisplayableCraftIds(InCraftIds);
	if (0 >= CraftIds.Num())
	{
		return false;
	}

	// 한 개 뿐인 경우, 즉시 이동
	if (1 == CraftIds.Num())
	{
		const UGsCraftManager* CraftManager = GSCraft();
		if (nullptr == CraftManager)
		{
			GSLOG(Error, TEXT("nullptr == CraftManager"));
			return false;
		}

		return CraftManager->TryToNavigate(CraftIds[0], OutFailMessage);
	}

	// 여러 개인 경우, 선택 팝업 오픈
	UGsUIManager* UIManager = GUI();
	if (nullptr == UIManager)
	{
		GSLOG(Error, TEXT("nullptr == UIManager"));
		return false;
	}

	TWeakObjectPtr<UGsUIWidgetBase> Popup = UIManager->OpenAndGetWidget(TEXT("PopupItemDropNavigator"));
	if (!Popup.IsValid())
	{
		GSLOG(Error, TEXT("!Popup.IsValid()"));
		return false;
	}

	UGsUIPopupItemDropNavigator* PopupItemDropNavigator = Cast<UGsUIPopupItemDropNavigator>(Popup.Get());
	if (nullptr == PopupItemDropNavigator)
	{
		GSLOG(Error, TEXT("nullptr == PopupItemDropNavigator"));
		return false;
	}

	{
		PopupItemDropNavigator->SetParameters({
			false,
			InCostType,
			InCurrencyType,
			InItemId
		});

		PopupItemDropNavigator->SetParametersCraftIds(MoveTemp(CraftIds));
	}

	return true;
}
