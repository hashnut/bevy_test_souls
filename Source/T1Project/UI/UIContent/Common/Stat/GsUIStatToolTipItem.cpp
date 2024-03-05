// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUIStatToolTipItem.h"
#include "T1Project.h"
#include "PaperSprite.h"
#include "Components/CanvasPanel.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "UI/UIContent/Helper/GsUIHelper.h"



void UGsUIStatToolTipItem::NativeConstruct()
{
	Super::NativeConstruct();

	SetStatGroupUIPanel(false);

	_statName->SetText(FText::FromString(""));
}

void UGsUIStatToolTipItem::NativeDestruct()
{

	Super::NativeDestruct();
}

void UGsUIStatToolTipItem::SetStatGroupUIPanel(bool InActive)
{
	_rootPanel->SetVisibility(true == InActive ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Collapsed);
}

void UGsUIStatToolTipItem::SetStatName(const FText InStatName)
{
	_statName->SetText(InStatName);
}

void UGsUIStatToolTipItem::SetStatImage(const FString InImagePath)
{
	UPaperSprite* sprite = FGsUIHelper::LoadSpriteFromPath(InImagePath);
	if (nullptr == sprite)
	{
		_statImage->SetVisibility(ESlateVisibility::Collapsed);
		GSLOG(Warning, TEXT("[UGsUIStatToolTipItem::SetStatImage] UPaperSprite Load Failed : %s"), *InImagePath);
	}
	else
	{
		_statImage->SetVisibility(ESlateVisibility::Visible);
		_statImage->SetBrushFromAtlasInterface(sprite);
	}	
}

