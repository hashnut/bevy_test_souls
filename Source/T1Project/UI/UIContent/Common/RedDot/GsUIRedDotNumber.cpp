// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUIRedDotNumber.h"
#include "Components/TextBlock.h"
#include "UI/UIContent/Helper/GsUIStringHelper.h"


void UGsUIRedDotNumber::SetRedDot(bool bIsRedDot, EGsIconRedDotType InType, int32 InNumber)
{
	SetVisibility(bIsRedDot ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Hidden);

	// 100 넘으면 99+ 표기
	FText textNumber;
	FGsUIStringHelper::GetAmountText(InNumber, 100, textNumber);
	_textBlockNumber->SetText(textNumber);
}
