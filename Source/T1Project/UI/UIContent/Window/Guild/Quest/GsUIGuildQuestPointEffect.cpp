// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUIGuildQuestPointEffect.h"
#include "Components/PanelWidget.h"


void UGsUIGuildQuestPointEffect::SetPointState(EGsGuildQuestPointState InState)
{
	switch (InState)
	{
	case EGsGuildQuestPointState::LOCKED:
	{
		_panelActive->SetVisibility(ESlateVisibility::Collapsed);
		_panelBoost->SetVisibility(ESlateVisibility::Collapsed);
	}
	break;
	case EGsGuildQuestPointState::ACTIVE:
	{
		_panelActive->SetVisibility(ESlateVisibility::HitTestInvisible);
		_panelBoost->SetVisibility(ESlateVisibility::Collapsed);
	}
	break;
	case EGsGuildQuestPointState::BOOST:
	{
		_panelActive->SetVisibility(ESlateVisibility::HitTestInvisible);
		_panelBoost->SetVisibility(ESlateVisibility::HitTestInvisible);

		PlayAnimationBoost();
	}
	break;
	}
}

void UGsUIGuildQuestPointEffect::OnFinishAnimationBoost()
{
	// 불타는 애니메이션 끝나면 ACTIVE 상태로 변경
	SetPointState(EGsGuildQuestPointState::ACTIVE);
}
