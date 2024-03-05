// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/UIContent/Popup/MonsterKnowledgeCollection/GsUICodexGemRequirementListEntry.h"
#include "Runtime/UMG/Public/Components/TextBlock.h"
#include "Runtime/UMG/Public/Components/WidgetSwitcher.h"
#include "T1Project.h"

void UGsUICodexGemRequirementListEntry::NativeOnListItemObjectSet(UObject* InListItemData)
{
	UGsUICodexGemRequirementListItem* itemData = Cast<UGsUICodexGemRequirementListItem>(InListItemData);
	if (nullptr == itemData)
	{
		GSLOG(Error, TEXT("nullptr == itemData"));
		return;
	}

	_textKnowledgeLevel->SetText(itemData->RequireLevel());
	_textRequireKnowledge->SetText(itemData->RequireKnowledge());
	_widgetSwitcherRequirement->SetActiveWidgetIndex(itemData->IsCompleted());

	IUserObjectListEntry::NativeOnListItemObjectSet(InListItemData);
}
