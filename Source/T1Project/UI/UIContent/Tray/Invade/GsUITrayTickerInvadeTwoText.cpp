// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUITrayTickerInvadeTwoText.h"
#include "UMG/Public/Components/TextBlock.h"
#include "Components/WidgetSwitcher.h"
#include "UMG/Public/Components/Image.h"
#include "PaperSprite.h"
#include "Engine/Texture2D.h"
#include "Engine/AssetManager.h"
#include "UI/UIContent/Helper/GsUIHelper.h"

void UGsUITrayTickerInvadeTwoText::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	FScriptDelegate animationDelegate;
	animationDelegate.BindUFunction(this, TEXT("OnExitAnimation"));
	BindToAnimationFinished(_inAnim, FWidgetAnimationDynamicEvent(animationDelegate));
}

void UGsUITrayTickerInvadeTwoText::SetInvadeTwoText(const FText& inContext1, const FText& inContext2, const FSoftObjectPath& inIconPath)
{
	if (inContext1.IsEmpty())
	{
		_contextTextBlock1->SetVisibility(ESlateVisibility::Collapsed);
	}
	else
	{
		_contextTextBlock1->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		_contextTextBlock1->SetText(inContext1);
	}

	if (inContext2.IsEmpty())
	{
		_contextTextBlock2->SetVisibility(ESlateVisibility::Collapsed);
	}
	else
	{
		_contextTextBlock2->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		_contextTextBlock2->SetText(inContext2);
	}	
	
	UPaperSprite* image = FGsUIHelper::LoadSpriteFromPath(inIconPath);
	if (image)
	{
		_iconImage->SetBrushFromAtlasInterface(image);
		_iconSwitcher->SetActiveWidgetIndex(1);
	}
	else
	{
		_iconSwitcher->SetActiveWidgetIndex(0);
	}

	PlayAnimation(_inAnim);
}
