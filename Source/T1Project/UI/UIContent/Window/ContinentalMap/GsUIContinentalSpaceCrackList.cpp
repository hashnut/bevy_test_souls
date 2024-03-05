#include "GsUIContinentalSpaceCrackList.h"

#include "Management/ScopeGlobal/GsUIManager.h"

#include "Management/ScopeGame/GsWorldMapManager.h"
#include "Management/ScopeGame/GsSpaceCrackManager.h"

#include "Map/WorldMap/GsRegionGroupInfoBase.h"
#include "Map/WorldMap/GsContinentalSpaceCrackMapInfo.h"

#include "SpaceCrack/GsSpaceCrackData.h"

#include "GameFlow/GameContents/ContentsGame/GsGameStateContinentalMap.h"

#include "UI/UIControlLib/Control/GsDynamicPanelSlotHelper.h"

#include "UI/UIContent/Window/ContinentalMap/GsUIContinentalSpaceCrackListItem.h"
#include "UI/UIContent/Window/GsUIWindowContinentalMap.h"

#include "UMG/Public/Components/VerticalBox.h"
#include "UMG/Public/Components/Widget.h"
#include "UMG/Public/Components/TextBlock.h"

void UGsUIContinentalSpaceCrackList::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	_continentalSpaceCrackListScrollBoxHelper = NewObject<UGsDynamicPanelSlotHelper>(this);
	_continentalSpaceCrackListScrollBoxHelper->Initialize(_continentalSpaceCrackListItemClass, _continentalSpaceCrackListVerticalBox);
	_continentalSpaceCrackListScrollBoxHelper->OnRefreshEntry.AddDynamic(this, &UGsUIContinentalSpaceCrackList::OnListRefreshEntry);


	_toggleGroupList.OnSelectChanged.BindUObject(this, &UGsUIContinentalSpaceCrackList::OnClickList);
}
void UGsUIContinentalSpaceCrackList::BeginDestroy()
{
	if (_continentalSpaceCrackListScrollBoxHelper)
	{
		_continentalSpaceCrackListScrollBoxHelper->OnRefreshEntry.RemoveDynamic(this, &UGsUIContinentalSpaceCrackList::OnListRefreshEntry);
	}
	_continentalSpaceCrackListScrollBoxHelper = nullptr;
	_toggleGroupList.Clear();
	Super::BeginDestroy();
}
void UGsUIContinentalSpaceCrackList::OnListRefreshEntry(int32 In_index, UWidget* In_entry)
{
	UGsUIContinentalSpaceCrackListItem* item = Cast<UGsUIContinentalSpaceCrackListItem>(In_entry);
	if (item == nullptr)
	{
		return;
	}

	// sort map info
	FGsContinentalSpaceCrackMapInfo* findData =
		GSWorldMap()->FindSpaceCrackSortInfoByIndex(In_index);

	if (findData == nullptr)
	{
		return;
	}

	item->SetContinentalSpaceCrackListItemData( findData->_isDecoLine ,findData->_mapName, findData->_isSpawned);
}
void UGsUIContinentalSpaceCrackList::OnClickList(int32 In_index)
{
	FGsContinentalSpaceCrackMapInfo* findData =
		GSWorldMap()->FindSpaceCrackSortInfoByIndex(In_index);

	if (findData == nullptr)
	{
		return;
	}

	// not use deco
	if (findData->_isDecoLine == true)
	{
		return;
	}

	// 1. center move(region map item)
	TWeakObjectPtr<UGsUIWidgetBase> wigdet = GUI()->GetWidgetByKey(TEXT("WindowContinentalMap"));

	if (wigdet.IsValid() == false)
	{
		return;
	}
	TWeakObjectPtr<UGsUIWindowContinentalMap> continentalWindow = Cast<UGsUIWindowContinentalMap>(wigdet);
	if (continentalWindow.IsValid() == false)
	{
		return;
	}
	// https://jira.com2us.com/jira/browse/C2URWQ-4984
	// 던전 메뉴 안보이게
	GSWorldMap()->SetIsShowDungeonMenu(false);
	continentalWindow->MakeCenterRegionMapItem(findData->_mapId, true);

	// 2. space crack info(spawn interval, last spawn time)
	const FGsSpaceCrackActivationInfo* findSpaceCrackInfo =  GSSpaceCrack()->FindSpaceCrackActivationInfoByMapId(findData->_mapId);
	if (findSpaceCrackInfo == nullptr)
	{
		return;
	}

	// spawn interval

	FText textHourMinuteFormat;
	FText::FindText(TEXT("SystemText"), TEXT("NHoursNMinutes"), textHourMinuteFormat);

	int minTableValue = findData->_minTimeMinute;
	int maxTableValue = findData->_maxTimeMinute;

	int minHour = minTableValue / 60;
	int minMinute = minTableValue % 60;
	
	FText minText = FText::Format(textHourMinuteFormat, minHour, minMinute);

	int maxHour = maxTableValue / 60;
	int maxMinute = maxTableValue % 60;

	FText maxText = FText::Format(textHourMinuteFormat, maxHour, maxMinute);

	const FString stringIntervalFormat = FString::Printf(TEXT("%s ~ %s"), *minText.ToString(), *maxText.ToString());
	
	_textSpawnInterval = FText::FromString(stringIntervalFormat);

	if (findSpaceCrackInfo->_isLastActive == true)
	{
		FText textMonthDayHourMinuteFormat;
		FText::FindText(TEXT("SystemText"), TEXT("NMonthsNDaysNHoursNMinutes"), textMonthDayHourMinuteFormat);

		// last spawn time

		int lastSpawnMonth = findSpaceCrackInfo->_lastActivateTime.GetMonth();
		int lastSpawnDay = findSpaceCrackInfo->_lastActivateTime.GetDay();
		int lastSpawnHour = findSpaceCrackInfo->_lastActivateTime.GetHour();
		int lastSpawnMinute = findSpaceCrackInfo->_lastActivateTime.GetMinute();

		FText lastSpawnText = FText::Format(textMonthDayHourMinuteFormat, lastSpawnMonth, lastSpawnDay, lastSpawnHour, lastSpawnMinute);

		_textLastSpawnTime = lastSpawnText;

		_textBlockLastSpawnTime->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}
	else
	{
		_textBlockLastSpawnTime->SetVisibility(ESlateVisibility::Hidden);
	}
}
void UGsUIContinentalSpaceCrackList::SetListData()
{
	int currentSpaceCrackListMapId = GSWorldMap()->GetCurrentSpaceCrackListMapId();

	// make sort data
	FGsGameStateContinentalMap::MakeContinentalSpaceCrackListSortInfo(currentSpaceCrackListMapId);

	TArray<FGsContinentalSpaceCrackMapInfo*> arraySortInfo = GSWorldMap()->GetSpaceCrackSortInfo();
	int count = arraySortInfo.Num();
	
	// set list count (make list)
	_continentalSpaceCrackListScrollBoxHelper->RefreshAll(count);

	_toggleGroupList.Clear();
	_toggleGroupList.AddToggleWidgetByParentPanel(_continentalSpaceCrackListVerticalBox);

	if (count != 0)
	{
		// select first item
		_toggleGroupList.SetSelectedIndex(0, true);
	}
	
}
// total index -> find type & data index
void UGsUIContinentalSpaceCrackList::FindInfoByTotalIndex(int In_totalIndex, EGsContinentalSpaceCrackListItemType& Out_itemType,
	int& Out_dataIndex)
{
	// 0~ active info size: active item
	// active info size: line deco
	// active info size + 1: deactive item


}