// Fill out your copyright notice in the Description page of Project Settings.


#include "GsBossInfoSpawnListViewEntry.h"
#include "GsUIBossDetailInfoPanel.h"
#include "UTIL/GsText.h"
#include "UTIL/GsCronUtil.h"
#include "DataSchema/Map/FieldBoss/GsSchemaFieldBossData.h"
#include "UMG/Public/Components/TextBlock.h"

void UGsBossInfoSpawnListViewEntry::NativeOnInitialized()
{
	Super::NativeOnInitialized();
}

void UGsBossInfoSpawnListViewEntry::NativeConstruct()
{
	Super::NativeConstruct();
}

void UGsBossInfoSpawnListViewEntry::NativeOnListItemObjectSet(UObject* inListItemObject)
{
	UGsBossInfoSpawnListViewEntryData* data = Cast<UGsBossInfoSpawnListViewEntryData>(inListItemObject);
	if (data)
	{							
		FString weekString;
		FString timeString;
		FGsCronUtil::GetWeek(data->_crone, weekString);
		FGsCronUtil::GetHour(data->_crone, timeString);

		_weekTextBlock->SetText(FText::FromString(weekString));
		_spawnTimeTextBlock->SetText(FText::FromString(timeString));
				
	}	
}