// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUIInvadeMapEntry.h"
#include "Management/ScopeGame/GsInterServerManager.h"
#include "Management/ScopeGlobal/GsLevelManager.h"
#include "UI/UIControlLib/ContentWidget/GsButton.h"
#include "UTIL/GsTimeSyncUtil.h"
#include "Components/TextBlock.h"
#include "Components/WidgetSwitcher.h"

void UGsUIInvadeMapEntry::NativeConstruct()
{
	Super::NativeConstruct();

	FText::FindText(TEXT("WorldMapText"), TEXT("RegionLevel"), _levelFormat);

	_btnGoMap->OnClicked.AddDynamic(this, &UGsUIInvadeMapEntry::OnClickGoWorldMap);
}

void UGsUIInvadeMapEntry::NativeDestruct()
{
	if(OnGoWorldMap.IsBound()) OnGoWorldMap.Unbind();
	_btnGoMap->OnClicked.RemoveDynamic(this, &UGsUIInvadeMapEntry::OnClickGoWorldMap);

	Super::NativeDestruct();
}

void UGsUIInvadeMapEntry::SetMapInfo(TWeakPtr<FGsInvadeMap> mapInfo)
{
	_mapData = mapInfo;
	_mapName = FText::GetEmpty();
	if (mapInfo.IsValid() && mapInfo.Pin()->IsAirRaidEableState(FGsTimeSyncUtil::GetServerNowDateTime()))
	{
		if (auto mapData = mapInfo.Pin())
		{
			_mapName = mapData->GetMapName();
			FText levelText = FText::Format(_levelFormat, mapData->GetValidLowLevel(), mapData->GetValidHighLevel());
			_levelTextBlock->SetText(levelText);

			auto invasionMap = GLevel()->GetCurrentLevelId();
			_mapIcon->SetActiveWidgetIndex(mapData->_id == invasionMap ? 1 : 0); // mapData->_enrolledSpot.Num() > 0 ? 1 : 0);
		}
	}	
}

bool UGsUIInvadeMapEntry::IsValidInvasionMap()
{
	return _mapData.IsValid() && _mapData.Pin()->IsAirRaidEableState(FGsTimeSyncUtil::GetServerNowDateTime());
}

void UGsUIInvadeMapEntry::OnClickGoWorldMap()
{
	if(_mapData.IsValid())
		OnGoWorldMap.ExecuteIfBound(_mapData.Pin()->_id);
}
