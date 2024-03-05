#include "GsDropSupplyHelper.h"

#include "ContentsEvent/EGsContentsEventState.h"
#include "ContentsEvent/GsContentsEventReward.h"
#include "DataSchema/GsSchemaEnums.h"
#include "DataSchema/Map/WorldMap/GsSchemaWorldMapMonsterElementData.h"
#include "DataSchema/SupplyInfo/GsSchemaCurrencySupplyInfoSet.h"
#include "DataSchema/SupplyInfo/GsSchemaDropSupplyInfo.h"
#include "DataSchema/SupplyInfo/GsSchemaDropSupplyInfoContentsEvent.h"
#include "DataSchema/SupplyInfo/GsSchemaDropSupplyInfoCreatureIds.h"
#include "DataSchema/SupplyInfo/GsSchemaItemSupplyInfoSet.h"
#include "DataSchema/SupplyInfo/GsSchemaSkillSupplyInfoSet.h"
#include "Management/GsScopeGame.h"
#include "Management/GsScopeHolder.h"
#include "Management/ScopeGame/GsContentsEventManager.h"
#include "Management/ScopeGame/GsSupplyManager.h"
#include "Management/ScopeGame/GsUnlockManager.h"
#include "Management/ScopeGlobal/GsUIManager.h"
#include "Shared/Client/SharedEnums/SharedCommonEnum.h"
#include "Shared/Client/SharedEnums/SharedEventEnum.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "UI/UIContent/Popup/GsUIPopupItemDropNavigator.h"
#include "UI/UILib/Base/GsUIWidgetBase.h"
#include "Unlock/GsUnlockDefine.h"

static constexpr int32 FIRST_SUPPLY_INFO_INDEX = 0;

FGsDropSupplyHelper::FGsDropSupplyHelper()
	: FGsBaseSupplyHelper(EGsUnlockContentsType::WORLDMAP)
{
}

FText FGsDropSupplyHelper::GetTitleText() const
{
	return NSLOCTEXT("ItemSupplyInfoText", "SupplyInfo_Drop", "몬스터처치");
}

UGsSupplyManager::EGsNavigationStatus FGsDropSupplyHelper::GetNavigationStatus(const FGsSchemaCurrencySupplyInfoSet& InSupplyInfoSet) const
{
	if (!FGsDropSupplyHelper::CanDisplay(InSupplyInfoSet.mapCreatureSetMap, InSupplyInfoSet.contentsEventMapCreatureSetMap))
	{
		return UGsSupplyManager::EGsNavigationStatus::None;
	}

	return UGsSupplyManager::EGsNavigationStatus::Movable;
}

bool FGsDropSupplyHelper::TryToNavigate(const FGsSchemaCurrencySupplyInfoSet& InSupplyInfoSet, OUT FText* OutFailMessage /*= nullptr*/) const
{
	return false;
}

UGsSupplyManager::EGsNavigationStatus FGsDropSupplyHelper::GetNavigationStatus(const FGsSchemaItemSupplyInfoSet& InSupplyInfoSet) const
{
	if (!FGsDropSupplyHelper::CanDisplay(InSupplyInfoSet.mapCreatureSetMap, InSupplyInfoSet.contentsEventMapCreatureSetMap))
	{
		return UGsSupplyManager::EGsNavigationStatus::None;
	}

	return UGsSupplyManager::EGsNavigationStatus::Movable;
}

bool FGsDropSupplyHelper::TryToNavigate(const FGsSchemaItemSupplyInfoSet& InSupplyInfoSet, OUT FText* OutFailMessage /*= nullptr*/) const
{
	return FGsDropSupplyHelper::TryToNavigateItemId(InSupplyInfoSet.mapCreatureSetMap, InSupplyInfoSet.contentsEventMapCreatureSetMap, StaticCast<ItemId>(InSupplyInfoSet.itemId), OutFailMessage);
}

UGsSupplyManager::EGsNavigationStatus FGsDropSupplyHelper::GetNavigationStatus(const FGsSchemaSkillSupplyInfoSet& InSupplyInfoSet) const
{
	if (!FGsDropSupplyHelper::CanDisplay(InSupplyInfoSet.mapCreatureSetMap, InSupplyInfoSet.contentsEventMapCreatureSetMap))
	{
		return UGsSupplyManager::EGsNavigationStatus::None;
	}

	return UGsSupplyManager::EGsNavigationStatus::Movable;
}

bool FGsDropSupplyHelper::TryToNavigate(const FGsSchemaSkillSupplyInfoSet& InSupplyInfoSet, OUT FText* OutFailMessage /*= nullptr*/) const
{	
	if (0 == InSupplyInfoSet.itemIds.Num())
	{
		GSLOG(Error, TEXT("0 == InSupplyInfoSet.itemIds.Num(), InSupplyInfoSet.skillId: %d"), InSupplyInfoSet.skillId);
		return false;
	}

	return FGsDropSupplyHelper::TryToNavigateItemId(InSupplyInfoSet.mapCreatureSetMap, InSupplyInfoSet.contentsEventMapCreatureSetMap, StaticCast<ItemId>(InSupplyInfoSet.itemIds[0]), OutFailMessage);
}

bool FGsDropSupplyHelper::CanDisplay(const TMap<int32, FGsSchemaDropSupplyInfoCreatureIds>& InMapCreatureMap, const TMap<int32, FGsSchemaDropSupplyInfo>& InContentsEventMapCreatureSetMap)
{
	if (0 < InMapCreatureMap.Num())
	{
		// 요소가 하나 이상이고, 첫번째 요소의 MapID 와 첫번째 CreatureId 가 유효한 지 체크.
		const TMap<MapId, FGsSchemaDropSupplyInfoCreatureIds>::TConstIterator Iterator = InMapCreatureMap.CreateConstIterator();
		const bool HasReward = Iterator && (INVALID_MAP_ID != Iterator.Key()) && (0 < Iterator.Value().worldmapMonsters.Num());
		if (HasReward)
		{
			return true;
		}
	}

	if (0 < InContentsEventMapCreatureSetMap.Num())
	{
		// 컨텐츠이벤트 활성화 여부로 분기
		TSet<int32> ContentsEventIdSet;
		InContentsEventMapCreatureSetMap.GetKeys(ContentsEventIdSet);
		const UGsContentsEventManager* ContentsEventManager = GSContentsEvent();
		const bool HasContentsEventReward = (nullptr != ContentsEventManager) && ContentsEventManager->IsAnyContentsEventActivated(ContentsEventType::REWARD, ContentsEventIdSet);
		if (HasContentsEventReward)
		{
			return true;
		}
	}

	return false;
}

bool FGsDropSupplyHelper::TryToNavigateItemId(const TMap<int32, FGsSchemaDropSupplyInfoCreatureIds>& InMapCreatureMap, const TMap<int32, FGsSchemaDropSupplyInfo>& InContentsEventMapCreatureSetMap, const ItemId& InItemId, OUT FText* OutFailMessage)
{
	if (INVALID_ITEM_ID == InItemId)
	{
		GSLOG(Error, TEXT("INVALID_ITEM_ID == InItemId"));
		return false;
	}

	if (!FGsDropSupplyHelper::CanDisplay(InMapCreatureMap, InContentsEventMapCreatureSetMap))
	{
		if (nullptr != OutFailMessage)
		{
			*OutFailMessage = NSLOCTEXT("DropSupplyHelper", "InvalidContentsEventMapInfo", "이동할 수 없습니다.");
		}
		return false;
	}

	// 잠김체크
	{
		FGsUnlockManager* UnlockManager = GSUnlock();
		if (nullptr == UnlockManager)
		{
			GSLOG(Warning, TEXT("nullptr == UnlockManager"));
			return false;
		}

		if (!UnlockManager->IsContentsUnlock(EGsUnlockContentsType::WORLDMAP))
		{
			if (nullptr != OutFailMessage)
			{
				UnlockManager->GetUnLockMessage(EGsUnlockContentsType::WORLDMAP, *OutFailMessage);
			}
			return false;
		}
	}

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
			true,
			CostType::ITEM,
			CurrencyType::NONE,
			InItemId
		});

		TArray<FGsSchemaWorldMapMonsterElementDataRow> DropMapCreatures;
		FGsDropSupplyHelper::MakeNavigationList(DropMapCreatures, &InMapCreatureMap, &InContentsEventMapCreatureSetMap);
		PopupItemDropNavigator->SetParametersDropMapCreatures(MoveTemp(DropMapCreatures));
	}

	return true;
}

void FGsDropSupplyHelper::MakeNavigationList(OUT TArray<FGsSchemaWorldMapMonsterElementDataRow>& OutNavigationList, const TMap<int32, FGsSchemaDropSupplyInfoCreatureIds>* InMapCreatureMap, const TMap<int32, FGsSchemaDropSupplyInfo>* InContentsEventMapCreatureSetMap)
{
	if (nullptr != InMapCreatureMap)
	{
		for (const TPair<int32, FGsSchemaDropSupplyInfoCreatureIds>& MapsPair : *InMapCreatureMap)
		{
			const MapId CurrentMapId = MapsPair.Key;
			for (const FGsSchemaWorldMapMonsterElementDataRow& WorldMapMonsterElementDataRow : MapsPair.Value.worldmapMonsters)
			{
				OutNavigationList.Emplace(WorldMapMonsterElementDataRow);
			}
		}
	}

	const UGsContentsEventManager* ContentsEventManager = GSContentsEvent();
	if (nullptr != ContentsEventManager && nullptr != InContentsEventMapCreatureSetMap)
	{
		TArray<TWeakPtr<const FGsContentsEventReward>> ActivatedContentsEventRewards = ContentsEventManager->GetContentsEvents<FGsContentsEventReward>(EGsContentsEventState::Activated);
		for (const TWeakPtr<const FGsContentsEventReward>& ActivatedContentsEventReward : ActivatedContentsEventRewards)
		{
			if (!ActivatedContentsEventReward.IsValid())
			{
				GSLOG(Error, TEXT("!ActivatedContentsEventReward.IsValid()"));
				continue;
			}

			const FGsSchemaDropSupplyInfo* DropSupplyInfo = InContentsEventMapCreatureSetMap->Find(ActivatedContentsEventReward.Pin()->GetContentsEventId());
			if (nullptr != DropSupplyInfo)
			{
				for (const TPair<int32, FGsSchemaDropSupplyInfoCreatureIds>& MapsPair : DropSupplyInfo->maps)
				{
					const MapId CurrentMapId = MapsPair.Key;
					for (const FGsSchemaWorldMapMonsterElementDataRow& WorldMapMonsterElementDataRow : MapsPair.Value.worldmapMonsters)
					{
						OutNavigationList.Emplace(WorldMapMonsterElementDataRow);
					}
				}
			}
		}
	}
}

bool FGsDropSupplyHelper::FindDropSupplyInfos(const ItemId InItemId, OUT const TMap<int32, FGsSchemaDropSupplyInfoCreatureIds>*& OutMaps, OUT const TMap<int32, FGsSchemaDropSupplyInfo>*& OutContentsEventMapCreatureSetMap)
{
	const UGsSupplyManager* SupplyManager = GSSupply();
	if (nullptr == SupplyManager)
	{
		GSLOG(Error, TEXT("nullptr == SupplyManager"));
		return false;
	}

	OutMaps = SupplyManager->LookupSupplyInfoData<TMap<int32, FGsSchemaDropSupplyInfoCreatureIds>>(InItemId);
	OutContentsEventMapCreatureSetMap = SupplyManager->LookupSupplyInfoData<TMap<int32, FGsSchemaDropSupplyInfo>>(InItemId);;

	return (nullptr != OutMaps) || (nullptr != OutContentsEventMapCreatureSetMap);
}

bool FGsDropSupplyHelper::MakeNavigationList(OUT TArray<FGsSchemaWorldMapMonsterElementDataRow>& OutNavigationList, const ItemId InItemId)
{
	const TMap<MapId, FGsSchemaDropSupplyInfoCreatureIds>* Maps = nullptr;
	const TMap<int32, FGsSchemaDropSupplyInfo>* ContentsEventMaps = nullptr;
	if (FGsDropSupplyHelper::FindDropSupplyInfos(InItemId, Maps, ContentsEventMaps))
	{
		FGsDropSupplyHelper::MakeNavigationList(OutNavigationList, Maps, ContentsEventMaps);
		return true;
	}

	return false;
}
