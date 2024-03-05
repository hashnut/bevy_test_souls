// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/UIContent/Window/MonsterKnowledge/GsUICodexExplorationLevelEntry.h"
#include "UI/UIContent/Helper/GsUIHelper.h"
#include "Runtime/UMG/Public/Components/TextBlock.h"
#include "Runtime/UMG/Public/Components/WidgetSwitcher.h"


void UGsUICodexExplorationLevelEntry::NativeOnInitialized()
{
	Super::NativeOnInitialized();
}

void UGsUICodexExplorationLevelEntry::NativeOnListItemObjectSet(UObject* InListItem)
{
	UGsUICodexExplorationLevelItem* item = Cast<UGsUICodexExplorationLevelItem>(InListItem);
	if (nullptr == item)
	{
		GSLOG(Error, TEXT("nullptr == item"));
		return;
	}

	if (item->IsFirst())
		_widgetSwitcherLine->SetVisibility(ESlateVisibility::Collapsed);
	else
		_widgetSwitcherLine->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

	_widgetSwitcherLine->SetActiveWidgetIndex(item->IsLineOn());

	_widgetSwitcherLevel->SetActiveWidgetIndex(static_cast<uint8>(item->LevelState()));

	_textAchievedLevel->SetText(item->RequireLevelUpCount());
	_textUnAchievedLevel->SetText(item->RequireLevelUpCount());
	_textAchievedLevelGlobal->SetText(item->RequireLevelUpCount());
	_textUnAchievedLevelGlobal->SetText(item->RequireLevelUpCount());

	_textExplorationPassivity->SetText(item->Stat());

	IUserObjectListEntry::NativeOnListItemObjectSet(InListItem);
}