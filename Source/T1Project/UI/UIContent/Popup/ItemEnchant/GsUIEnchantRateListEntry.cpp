// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUIEnchantRateListEntry.h"
#include "Components/TextBlock.h"


void UGsUIEnchantRateListEntry::SetData(FGsItemEnchantManager::DisplayRateInfo InDisplayInfo)
{
	FText resultText;
	switch (InDisplayInfo._enchantType)
	{
	case ItemEnchantResult::ENCHANT_FAILED_DOWN:
		FText::FindText(TEXT("ItemUIText"), TEXT("ItemUi_Enchant_DownProbability"), resultText);
		break;
	case ItemEnchantResult::ENCHANT_PRESERVED:
		FText::FindText(TEXT("ItemUIText"), TEXT("ItemUi_Enchant_PreserveProbability"), resultText);
		break;
	case ItemEnchantResult::ENCHANT_SUCCEEDED:
	case ItemEnchantResult::ENCHANT_SUCCEEDED_CURSED:
		FText::FindText(TEXT("ItemUIText"), TEXT("ItemUi_Enchant_EnchantProbability"), resultText);
		break;
	case ItemEnchantResult::ENCHANT_SUCCEEDED_BLESSED:
		FText::FindText(TEXT("ItemUIText"), TEXT("ItemUi_Enchant_BlessProbability"), resultText);
		break;
	}
	_textBlockTitle->SetText(resultText);

	FText currentLevelText = FText::Format(FText::FromString(TEXT("+{0}")), FText::AsNumber(InDisplayInfo._currentLevel));
	FText expectLevelText = FText::Format(FText::FromString(TEXT("+{0}")), FText::AsNumber(InDisplayInfo._expectLevel));
	_textBlockCurrent->SetText(currentLevelText);
	_textBlockExpect->SetText(expectLevelText);

	FText markText;
	FText::FindText(TEXT("MarkFormatingText"), TEXT("Percent"), markText);
	float calc = FMath::Floor(InDisplayInfo._rate * 10) / 10.0f;
	if (0 == (calc - (int32)calc))
	{
		_textBlockPercent->SetText(FText::FromString(FString::Printf(TEXT("%d"), (int32)calc) + markText.ToString()));
	}
	else
	{
		_textBlockPercent->SetText(FText::FromString(FString::Printf(TEXT("%.1f"), calc) + markText.ToString()));
	}

	/*
	FNumberFormattingOptions option;
	option.SetMaximumFractionalDigits(1);
	option.SetMinimumFractionalDigits(0);

	
	FText percentText = FText::Format(FText::FromString(TEXT("{0}%")), FText::AsNumber(InExpectPercent, &option));

	_textBlockCurrent->SetText(currentLevelText);
	_textBlockExpect->SetText(expectLevelText);
	_textBlockPercent->SetText(percentText);
	*/
}

void UGsUIEnchantRateListEntry::SetTitleText(ItemEnchantResult InResult)
{
	FText resultText;
	switch (InResult)
	{
	case ItemEnchantResult::ENCHANT_FAILED_DOWN:
		FText::FindText(TEXT("ItemUIText"), TEXT("ItemUi_Enchant_DownProbability"), resultText);
		break;
	case ItemEnchantResult::ENCHANT_PRESERVED:
		FText::FindText(TEXT("ItemUIText"), TEXT("ItemUi_Enchant_PreserveProbability"), resultText);
		break;
	case ItemEnchantResult::ENCHANT_SUCCEEDED:
	case ItemEnchantResult::ENCHANT_SUCCEEDED_CURSED:
		FText::FindText(TEXT("ItemUIText"), TEXT("ItemUi_Enchant_EnchantProbability"), resultText);
		break;
	case ItemEnchantResult::ENCHANT_SUCCEEDED_BLESSED:
		FText::FindText(TEXT("ItemUIText"), TEXT("ItemUi_Enchant_BlessProbability"), resultText);
		break;
	}

	_textBlockTitle->SetText(resultText);
}

void UGsUIEnchantRateListEntry::SetCurrentText(const FText& InText)
{
	_textBlockCurrent->SetText(InText);
}

void UGsUIEnchantRateListEntry::SetExpectText(const FText& InText)
{
	_textBlockExpect->SetText(InText);
}

void UGsUIEnchantRateListEntry::SetPercentText(int32 InProbability)
{
	FText markText;
	FText::FindText(TEXT("MarkFormatingText"), TEXT("Percent"), markText);

	float calc = InProbability / 10.0f;
	calc = FMath::Floor(calc) / 10.0f;
	if (0 == (calc - (int32)calc))
	{
		_textBlockPercent->SetText(FText::FromString(FString::Printf(TEXT("%d"), (int32)calc) + markText.ToString()));
	}
	else
	{
		_textBlockPercent->SetText(FText::FromString(FString::Printf(TEXT("%.1f"), calc) + markText.ToString()));
	}
}
