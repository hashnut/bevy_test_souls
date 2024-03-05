// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUITrayTickerUnlock.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Components/CanvasPanel.h"

#include "PaperSprite.h"

#include "UI/UIContent/Helper/GsUIHelper.h"

#include "DataSchema/Unlock/GsSchemaUnlockContents.h"

#include "Management/ScopeGame/GsUnlockManager.h"


void UGsUITrayTickerUnlock::NativeOnInitialized()
{
	Super::NativeOnInitialized();
}

void UGsUITrayTickerUnlock::NativeConstruct()
{
	Super::NativeConstruct();

	_isClosing = false;

	_textBlockContentsName->SetVisibility(ESlateVisibility::Collapsed);
	_textBlockContentsDesc->SetVisibility(ESlateVisibility::Collapsed);
	_imageContentsIcon->SetVisibility(ESlateVisibility::Hidden);
	_panelIcon->SetVisibility(ESlateVisibility::Collapsed);
}

FReply UGsUITrayTickerUnlock::NativeOnTouchStarted(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent)
{
	if (!_isClosing && SetReservedData())
	{
		PlayOpenAnimation();
	}
	else
	{
		if (!_isClosing)
		{
			_isClosing = true;
			Close();
		}
	}

	return FReply::Handled();
}

void UGsUITrayTickerUnlock::SetText(const FText& InContentsName)
{
	_textBlockContentsName->SetVisibility(ESlateVisibility::HitTestInvisible);
	_textBlockContentsName->SetText(InContentsName);
}

void UGsUITrayTickerUnlock::SetText(const FText& InContentsName, const FText& InContentsDesc)
{
	_textBlockContentsName->SetVisibility(ESlateVisibility::HitTestInvisible);
	_textBlockContentsName->SetText(InContentsName);

	_textBlockContentsDesc->SetVisibility(ESlateVisibility::HitTestInvisible);
	_textBlockContentsDesc->SetText(InContentsDesc);
}

void UGsUITrayTickerUnlock::SetImage(const FSoftObjectPath& InIconPath)
{
	_panelIcon->SetVisibility(ESlateVisibility::HitTestInvisible);
	_imageContentsIcon->SetVisibility(ESlateVisibility::HitTestInvisible);
	TWeakObjectPtr<UImage> weakThis(_imageContentsIcon);
	FGsUIHelper::AsyncLoadSpriteFromPath(InIconPath, FStreamableDelegateParam::CreateWeakLambda(this, [weakThis](UObject* inRes)
		{
			if (UImage* strongThis = weakThis.Get())
			{
				strongThis->SetBrushFromAtlasInterface(Cast<UPaperSprite>(inRes));
			}
		}));
}

bool UGsUITrayTickerUnlock::SetReservedData()
{
	if (const FGsSchemaUnlockContents* unlockData = GSUnlock()->PopReservedTickerData())
	{
		_textBlockContentsName->SetVisibility(ESlateVisibility::Collapsed);
		_textBlockContentsDesc->SetVisibility(ESlateVisibility::Collapsed);
		_imageContentsIcon->SetVisibility(ESlateVisibility::Hidden);
		_panelIcon->SetVisibility(ESlateVisibility::Collapsed);

		const FText& name = unlockData->contentsName;
		const FText& desc = unlockData->unlockDesc;
		const FSoftObjectPath& iconPath = unlockData->icon;

		if (desc.IsEmpty())
		{
			SetText(name);
		}
		else
		{
			SetText(name, desc);
		}

		if (iconPath.IsValid())
		{
			SetImage(iconPath);
		}

		return true;
	}
	return false;
}
