// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUIRefineOptionStatEntry.h"
#include "Components/TextBlock.h"


void UGsUIRefineOptionStatEntry::SetStatDisplayInfo(const FText& InStatNameText, const FText& InFirstValueText, const FText& InSecondValueText, FString InBonusValueString, const bool InIsBonusStat)
{
	_statNameText->SetText(InStatNameText);

	if (true == InIsBonusStat)
	{
		_statValueText->SetText(FText::FromString(InBonusValueString));
	}
	else
	{
		FString resultString;
		FText markText;
		FText::FindText(TEXT("MarkFormatingText"), TEXT("SwungDash"), markText);
		resultString = FString::Format(TEXT("{0}{1}{2}"), { InFirstValueText.ToString(),*markText.ToString(),InSecondValueText.ToString() });
		_statValueText->SetText(FText::FromString(resultString));
	}
}

