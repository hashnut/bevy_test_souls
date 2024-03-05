#include "GsWorldMapDemandHelper.h"

#include "DataSchema/DemandInfo/GsSchemaCurrencyDemandInfoSet.h"
#include "DataSchema/DemandInfo/GsSchemaItemDemandInfoSet.h"
#include "GsCommonDemandHelper.h"
#include "Management/ScopeGame/GsSupplyManager.h"
#include "Management/ScopeGlobal/GsUIManager.h"
#include "Shared/Client/SharedEnums/SharedCommonEnum.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "UI/UIContent/Popup/GsUIPopupItemDropNavigator.h"
#include "UI/UIContent/Popup/ItemDropNavigator/GsUIItemDropElement.h"
#include "UI/UILib/Base/GsUIWidgetBase.h"
#include "Unlock/GsUnlockDefine.h"

FGsWorldMapDemandHelper::FGsWorldMapDemandHelper()
	: Super(EGsUnlockContentsType::WORLDMAP)
{
}

UGsSupplyManager::EGsNavigationStatus FGsWorldMapDemandHelper::GetNavigationStatus(const FGsSchemaCurrencyDemandInfoSet& InDemandInfo) const
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
		if (FGsWorldMapDemandHelper::CanDisplay(InDemandInfo.mapIds))
		{
			IsValidNavigation = true;
		}
	}

	if (IsValidNavigation)
	{
		return FGsBaseDemandHelper::GetNavigationStatusStatic(_contentsType, InDemandInfo.currencyType);
	}

	return UGsSupplyManager::EGsNavigationStatus::None;
}

bool FGsWorldMapDemandHelper::TryToNavigate(const FGsSchemaCurrencyDemandInfoSet& InCurrencyDemandInfoSet, OUT FText* OutFailMessage /*= nullptr*/) const
{
	return FGsWorldMapDemandHelper::TryToNavigate(CostType::CURRENCY, InCurrencyDemandInfoSet.currencyType, INVALID_ITEM_ID, InCurrencyDemandInfoSet.mapIds, OutFailMessage);
}

UGsSupplyManager::EGsNavigationStatus FGsWorldMapDemandHelper::GetNavigationStatus(const FGsSchemaItemDemandInfoSet& InDemandInfo) const
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
		if (FGsWorldMapDemandHelper::CanDisplay(InDemandInfo.mapIds))
		{
			IsValidNavigation = true;
		}
	}

	if (IsValidNavigation)
	{
		return FGsBaseDemandHelper::GetNavigationStatusStatic(_contentsType, InDemandInfo.itemId);
	}

	return UGsSupplyManager::EGsNavigationStatus::None;
}

bool FGsWorldMapDemandHelper::TryToNavigate(const FGsSchemaItemDemandInfoSet& InItemDemandInfoSet, OUT FText* OutFailMessage /*= nullptr*/) const
{
	return FGsWorldMapDemandHelper::TryToNavigate(CostType::ITEM, CurrencyType::NONE, InItemDemandInfoSet.itemId, InItemDemandInfoSet.mapIds, OutFailMessage);
}

bool FGsWorldMapDemandHelper::CanDisplay(const TSet<int32>& InMapSet)
{
	if (0 >= InMapSet.Num())
	{
		return false;
	}

	return true;
}

bool FGsWorldMapDemandHelper::TryToNavigate(const CostType InCostType, const CurrencyType InCurrencyType, const ItemId InItemId, const TSet<int32>& InMapIdSet, OUT FText* OutFailMessage)
{
	if (0 >= InMapIdSet.Num())
	{
		return false;
	}

	if (1 == InMapIdSet.Num())
	{
		// 하나뿐인 경우 즉시 이동. 메세지로 변경 등 필요
		return UGsUIItemDropListContainer::TryToNavigate(*InMapIdSet.CreateConstIterator());
	}

	// 하나 이상인 경우 리스트로 출력
	TArray<TPair<MapId, CreatureId>> NavigationMap;
	for (const int32 CurrentMapId : InMapIdSet)
	{
		NavigationMap.Emplace(StaticCast<MapId>(CurrentMapId), INVALID_CREATURE_ID);
	}

	// 선택 팝업 오픈.
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

	PopupItemDropNavigator->SetParameters({
		false,
		InCostType,
		InCurrencyType,
		InItemId
	});

	PopupItemDropNavigator->SetParametersDropMapCreatures(MoveTemp(NavigationMap));

	return true;
}
