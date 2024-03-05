// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/UIContent/Window/SpiritShot/GsUISpiritShotLine.h"
#include "Runtime/UMG/Public/Components/WidgetSwitcher.h"
#include "T1Project.h"

void UGsUISpiritShotLine::SetLineEffect(bool InIsLast /*= false*/)
{
	_lineEffectSwitcher->SetActiveWidgetIndex(InIsLast);
	
	// 하위 위젯이 Collapsed 되어 있어 처리 (언리얼 버그로 추정된다;)
	UWidget* activeWidget = _lineEffectSwitcher->GetActiveWidget();
	if (nullptr == activeWidget)
	{
		GSLOG(Error, TEXT("nullptr == activeWidget"));
		return;
	}
	activeWidget->SetVisibility(ESlateVisibility::HitTestInvisible);
}
