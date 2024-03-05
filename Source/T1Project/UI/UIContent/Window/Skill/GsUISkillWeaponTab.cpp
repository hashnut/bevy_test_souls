// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUISkillWeaponTab.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "UI/UIControlLib/ContentWidget/GsSwitcherButton.h"


void UGsUISkillWeaponTab::SynchronizeProperties()
{
	Super::SynchronizeProperties();
		
	if (_imgIconOff)
	{
		_imgIconOff->SetBrushResourceObject(_brushIconOff.GetResourceObject());
	}

	if (_imgIconOn)
	{
		_imgIconOn->SetBrushResourceObject(_brushIconOn.GetResourceObject());
	}

	if (_textBlockOff)
	{
		_textBlockOff->SetText(_textName);
	}

	if (_textBlockOn)
	{
		_textBlockOn->SetText(_textName);
	}
}

void UGsUISkillWeaponTab::SetEquipMark(bool bIsVisible)
{
	_imgEquipMark->SetVisibility(bIsVisible ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed);
}
