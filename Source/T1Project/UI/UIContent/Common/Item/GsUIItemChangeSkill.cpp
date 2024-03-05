// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/UIContent/Common/Item/GsUIItemChangeSkill.h"

#include "Components/CanvasPanel.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"

void UGsUIItemChangeSkill::OnShow()
{
	SetSkillUIPanel(true);
}

void UGsUIItemChangeSkill::OnHide()
{

}

void UGsUIItemChangeSkill::SetCurSkillName(const FText& InSkillName)
{
	_curSkill->SetText(InSkillName);
}

void UGsUIItemChangeSkill::SetNextSkillName(const FText& InSkillName)
{
	_nextSkill->SetText(InSkillName);
}

void UGsUIItemChangeSkill::SetSkillUIPanel(bool InActive)
{
	_panel->SetVisibility(true == InActive ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Collapsed);
}

void UGsUIItemChangeSkill::SetCurSkillNameColor(const FLinearColor& InColor)
{
	_curSkill->SetColorAndOpacity(InColor);
}

void UGsUIItemChangeSkill::SetNextSkillNameColor(const FLinearColor& InColor)
{
	_nextSkill->SetColorAndOpacity(InColor);
}

void UGsUIItemChangeSkill::ShowNextSkillArea(bool InShow)
{
	if (InShow)
	{
		_arrowImage->SetVisibility(ESlateVisibility::HitTestInvisible);
		_nextSkill->SetVisibility(ESlateVisibility::HitTestInvisible);
	}
	else
	{
		_arrowImage->SetVisibility(ESlateVisibility::Collapsed);
		_nextSkill->SetVisibility(ESlateVisibility::Collapsed);
	}
}