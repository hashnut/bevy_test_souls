// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/UIContent/Window/MonsterKnowledge/GsUICodexPortrait.h"
#include "T1Project.h"

void UGsUICodexPortrait::NativeOnListItemObjectSet(UObject* InListItemObject)
{
	UGsUICodexPortraitEntryData* item = Cast<UGsUICodexPortraitEntryData>(InListItemObject);
	if (nullptr == item)
	{
		GSLOG(Error, TEXT("nullptr == item"));
		return;
	}

	SetData(item->GetData());
}
