// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUIMainMenuBtnSimple.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Components/PanelWidget.h"
#include "UI/UIControlLib/ContentWidget/GsSwitcherButton.h"


void UGsUIMainMenuBtnSimple::SynchronizeProperties()
{
	Super::SynchronizeProperties();

	SetTextBlockName(_panelRootNameOn, _textBlockNameOn);
	SetTextBlockName(_panelRootNameOff, _textBlockNameOff);

	SetImageIcon(_imgIconOn);
	SetImageIcon(_imgIconOff);
}

void UGsUIMainMenuBtnSimple::SetTextBlockName(OUT UPanelWidget* OutRoot, UTextBlock* OutTextBlock)
{
	if (_bUseName)
	{
		if (OutRoot)
		{
			OutRoot->SetVisibility(ESlateVisibility::HitTestInvisible);
		}

		if (OutTextBlock)
		{
			OutTextBlock->SetText(_textName);
		}
	}
	else
	{
		if (OutRoot)
		{
			OutRoot->SetVisibility(ESlateVisibility::Collapsed);
		}
	}
}

void UGsUIMainMenuBtnSimple::SetImageIcon(OUT UImage* OutImage)
{
	if (OutImage)
	{
		OutImage->SetBrushSize(_brushIcon.GetImageSize());
		OutImage->SetBrushResourceObject(_brushIcon.GetResourceObject());
	}
}