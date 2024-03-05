// Fill out your copyright notice in the Description page of Project Settings.


#include "GsEditorWidget.h"
#include "Runtime/UMG/Public/Components/TextBlock.h"
#include "Image.h"

void UGsEditorWidget::SetTitleText(FString inTtile)
{
	if (TitleText)
	{
		TitleText->SetText(FText::FromString(inTtile));
	}
}

void UGsEditorWidget::SetTitleImgColor(FColor inColor)
{
	if (TitleImg)
	{
		TitleImg->SetColorAndOpacity(inColor);
	}
}

void UGsEditorWidget::SetDescriptionText(FString inTtile)
{
	if (DescriptionText)
	{
		DescriptionText->SetText(FText::FromString(inTtile));
	}
}

void UGsEditorWidget::SetDescriptionImgColor(FColor inColor)
{
	if (DescImg)
	{
		DescImg->SetColorAndOpacity(inColor);
	}
}
