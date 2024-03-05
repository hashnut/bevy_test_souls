// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/UIContent/Popup/ItemEnchant/GsUIRefineOptionStatListEntry.h"
#include "Components/TextBlock.h"
#include "Components/PanelWidget.h"




void UGsUIRefineOptionStatListEntry::OnShow()
{
}

void UGsUIRefineOptionStatListEntry::OnHide()
{
}

void UGsUIRefineOptionStatListEntry::SetData(int32 InIndex, FText& InStatNameText, FText& InStatValueText, const FLinearColor& InColorData)
{
	_rootPanel->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

	FTextKey key;
	switch (InIndex)
	{
	case 0:
		key = TEXT("UI_Refinement_Option1");
		break;
	case 1:
		key = TEXT("UI_Refinement_Option2");
		break;
	case 2:
		key = TEXT("UI_Refinement_Option3");
		break;
	case 3:
		key = TEXT("UI_Refinement_Option4");
		break;
	}

	FText findText;
	FText::FindText(TEXT("ItemEnchantText"), key, findText);
	_optionNumText->SetText(findText);
	_optionNameText->SetText(InStatNameText);
	_optionValueText->SetText(InStatValueText);

	_optionNameText->SetColorAndOpacity(InColorData);
	_optionValueText->SetColorAndOpacity(InColorData);
}
