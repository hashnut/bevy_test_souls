// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUIMultiLevelRankIcon.h"

#include "Components/Image.h"
#include "Components/PanelWidget.h"

#include "PaperSprite.h"

#include "UI/UIContent/Helper/GsUIHelper.h"


void UGsUIMultiLevelRankIcon::NativeConstruct()
{
	Super::NativeConstruct();

	_subIconList.Emplace(_imageSubIcon1);
	_subIconList.Emplace(_imageSubIcon2);
	_subIconList.Emplace(_imageSubIcon3);
	_subIconList.Emplace(_imageSubIcon4);
	_subIconList.Emplace(_imageSubIcon5);
}

void UGsUIMultiLevelRankIcon::NativeDestruct()
{
	_subIconList.Empty();

	Super::NativeDestruct();
}

void UGsUIMultiLevelRankIcon::SetMainImage(const FSoftObjectPath& InImagePath)
{
	TWeakObjectPtr<UImage> weakThis(_imageMainIcon);
	FGsUIHelper::AsyncLoadSpriteFromPath(InImagePath, FStreamableDelegateParam::CreateWeakLambda(this, [weakThis](UObject* inRes)
		{
			if (UImage* strongThis = weakThis.Get())
			{
				strongThis->SetBrushFromAtlasInterface(Cast<UPaperSprite>(inRes));
			}
		}));
}

void UGsUIMultiLevelRankIcon::SetSubImage(const FSoftObjectPath& InImagePath)
{
	TArray<TWeakObjectPtr<UImage>> weakImageList(_subIconList);

	FGsUIHelper::AsyncLoadSpriteFromPath(InImagePath, FStreamableDelegateParam::CreateWeakLambda(this, [weakImageList](UObject* inRes)
		{
			for (TWeakObjectPtr<UImage> weakImage : weakImageList)
			{
				if (UImage* strongThis = weakImage.Get())
				{
					strongThis->SetBrushFromAtlasInterface(Cast<UPaperSprite>(inRes));
				}
			}
		}));
}

void UGsUIMultiLevelRankIcon::SetSubImageCount(int32 InCount)
{
	if (InCount <= 0)
	{
		_panelSubIcon->SetVisibility(ESlateVisibility::Collapsed);
	}
	else
	{
		_panelSubIcon->SetVisibility(ESlateVisibility::HitTestInvisible);

		for (int32 i = 0;i < MAX_SUB_ICON_COUNT;++i)
		{
			if (i < InCount)
			{
				_subIconList[i]->SetVisibility(ESlateVisibility::HitTestInvisible);
			}
			else
			{
				_subIconList[i]->SetVisibility(ESlateVisibility::Collapsed);
			}
		}
	}
}
