// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUITrayTickerStatChange.h"
#include "Components/TextBlock.h"
#include "UI/UIControlLib/ContentWidget/GsButton.h"
#include "UTIL/GsGameObjectUtil.h"
#include "LevelUp/GsLevelUpStatListViewEntry.h"
#include "UMG/Public/Components/ListViewBase.h"
#include "UMG/Public/Components/ListView.h"
#include "UMG/Public/Animation/WidgetAnimation.h"
#include "Management/ScopeGlobal/GsUIManager.h"
#include "Item/Data/GsItemStatData.h"
#include "Management/ScopeGame/GsEventProgressManager.h"
#include "Management/ScopeGlobal/GsOptionManager.h"
#include "StatChange/GsUIStatChangePanel.h"

void UGsUITrayTickerStatChange::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	_closeButton->OnClicked.AddDynamic(this, &UGsUITrayTickerStatChange::OnClickClose);	
}

void UGsUITrayTickerStatChange::NativeConstruct()
{
	Super::NativeConstruct();

	_closeButton->SetIsEnabled(true);
}

void UGsUITrayTickerStatChange::NativeDestruct()
{
	_closeButton->SetIsEnabled(false);

	if (FGsEventProgressManager* eventProgressManager = GSEventProgress())
	{
		eventProgressManager->OnReqFinishEvent(EGsEventProgressType::UI_TRAY_LEVEL_UP);
	}

	Super::NativeConstruct();
}

FReply UGsUITrayTickerStatChange::NativeOnTouchStarted(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent)
{
	if (_closeButton->bIsEnabled)
	{
		OnClickClose();
	}	

	return FReply::Handled();
}

void UGsUITrayTickerStatChange::Play(StatUpdateReason inReason, const TMap<StatType, TPair<int, int>>& inStatDiffMap)
{	
	float option = 0.f;
	bool isVisible = false;
	GOption()->GetValue(EGsOptionItem::CombatPower, option, isVisible);
	_statChangePanel->Play(inReason, option, inStatDiffMap, [this]()->void {
		OnFinishAnimation();
		});
}

void UGsUITrayTickerStatChange::OnClickClose()
{
	_closeButton->SetIsEnabled(false);
}

void UGsUITrayTickerStatChange::OnFinishAnimation()
{
	GUI()->RemoveWidget(this);
}
