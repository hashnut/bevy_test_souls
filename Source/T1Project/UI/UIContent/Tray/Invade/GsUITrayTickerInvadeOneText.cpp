// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUITrayTickerInvadeOneText.h"
#include "UMG/Public/Components/TextBlock.h"
#include "Components/WidgetSwitcher.h"
#include "UMG/Public/Components/Image.h"
#include "PaperSprite.h"
#include "Engine/Texture2D.h"
#include "Engine/AssetManager.h"
#include "UI/UIContent/Helper/GsUIHelper.h"

void UGsUITrayTickerInvadeOneText::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	FScriptDelegate animationDelegate;
	animationDelegate.BindUFunction(this, TEXT("OnExitAnimation"));
	BindToAnimationFinished(_inAnim, FWidgetAnimationDynamicEvent(animationDelegate));
}

void UGsUITrayTickerInvadeOneText::SetInvadeOneTextData(const FText& inContext, const FSoftObjectPath& inIconPath)
{
	_contextTextBlock->SetText(inContext);

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
