// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUIFairyStat.h"
#include "TextBlock.h"

void UGsUIFairyStat::SetText(FText name, FText val, FairyLevelId level, bool isLock)
{
	_txtStatName->SetText(name);
	_txtStatValue->SetText(val);
	_isLock->SetVisibility(isLock ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed);

	if (level != INVALID_LEVEL)
	{
		FText levelFormat;
		FText::FindText(TEXT("FairyText"), TEXT("FairyUI_LevelFormat"), levelFormat);
		_textLevel = FText::Format(levelFormat, level);
	}
}