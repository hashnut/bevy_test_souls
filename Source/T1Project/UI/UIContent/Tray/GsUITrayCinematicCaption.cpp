// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUITrayCinematicCaption.h"
#include "Components/RichTextBlock.h"


void UGsUITrayCinematicCaption::NativeConstruct()
{
	Super::NativeConstruct();

	_richTextCaption->SetText(FText::GetEmpty());
	_richTextCaptionCenter->SetText(FText::GetEmpty());
}

void UGsUITrayCinematicCaption::NativeDestruct()
{
	_richTextCaption->SetText(FText::GetEmpty());
	_richTextCaptionCenter->SetText(FText::GetEmpty());

	Super::NativeDestruct();
}

void UGsUITrayCinematicCaption::SetCaption(const FText& InText, EGsCinematicCaptionType InMode)
{
	switch (InMode)
	{
	case EGsCinematicCaptionType::CENTER:
	{
		_richTextCaption->SetText(FText::GetEmpty());
		_richTextCaptionCenter->SetText(InText);
	}
	break;
	default:
	{
		_richTextCaption->SetText(InText);
		_richTextCaptionCenter->SetText(FText::GetEmpty());
	}
	break;
	}
}
