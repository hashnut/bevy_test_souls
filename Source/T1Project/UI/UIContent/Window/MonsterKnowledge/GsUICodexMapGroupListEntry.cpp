// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/UIContent/Window/MonsterKnowledge/GsUICodexMapGroupListEntry.h"
#include "UI/UIContent/Helper/GsUIHelper.h"
#include "Paper2D/Classes/PaperSprite.h"
#include "Runtime/UMG/Public/Components/TextBlock.h"
#include "Runtime/UMG/Public/Components/ProgressBar.h"
#include "Runtime/UMG/Public/Components/WidgetSwitcher.h"
#include "Image.h"


void UGsUICodexMapGroupListItem::SetDataInternal(CodexMapGroupId InId, bool InIsMaxLevel, FText&& InLevel, FText&& InMapGroupName, UTexture2D* InMapGroupTexture, float InProgressBarPercent)
{
	_mapGroupId = InId;
	_isMaxLevel = InIsMaxLevel;
	_level = MoveTemp(InLevel);
	_mapGroupName = MoveTemp(InMapGroupName);
	//_mapGroupSprite = InMapGroupSprite;
	_mapGroupTexture = InMapGroupTexture;
	_progressBarPercent = InProgressBarPercent;
}

void UGsUICodexMapGroupListItem::SetData(CodexMapGroupId InId, bool InIsMaxLevel, FText&& InLevel, FText&& InMapGroupName, const FSoftObjectPath& InMapGroupImagePath, float InProgressBarPercent)
{
	//UPaperSprite* mapGroupSprite = FGsUIHelper::LoadSpriteFromPath(InMapGroupImagePath);
	//SetDataInternal(InId, InIsMaxLevel, MoveTemp(InLevel), MoveTemp(InMapGroupName), mapGroupSprite, InProgressBarPercent);
	
	UTexture2D* mapGroupTexture = FGsUIHelper::LoadTextureFromPath(InMapGroupImagePath);
	SetDataInternal(InId, InIsMaxLevel, MoveTemp(InLevel), MoveTemp(InMapGroupName), mapGroupTexture, InProgressBarPercent);
}

void UGsUICodexMapGroupListItem::SetIsSelected(bool InIsSelected)
{
	_isSelected = InIsSelected;
}



void UGsUICodexMapGroupListEntry::NativeOnInitialized()
{
	Super::NativeOnInitialized();
}

void UGsUICodexMapGroupListEntry::NativeOnListItemObjectSet(UObject* InListItem)
{
	UGsUICodexMapGroupListItem* item = Cast<UGsUICodexMapGroupListItem>(InListItem);
	if (nullptr == item)
	{
		GSLOG(Error, TEXT("nullptr == item"));
		return;
	}

	if (item->GetIsMaxLevel())
	{
		_imageMaxLevel->SetVisibility(ESlateVisibility::HitTestInvisible);
		_textLevel->SetVisibility(ESlateVisibility::Collapsed);
	}
	else
	{
		_imageMaxLevel->SetVisibility(ESlateVisibility::Collapsed);
		_textLevel->SetVisibility(ESlateVisibility::HitTestInvisible);
	}

	_textLevel->SetText(item->GetLevel());
	_textMapGroupName->SetText(item->GetMapGroupName());
	//if (item->GetPaperSprite())
	//	_imageMapGroup->SetBrushFromAtlasInterface(item->GetPaperSprite());
	if (item->GetTexture2D())
		_imageMapGroup->SetBrushFromTexture(item->GetTexture2D());

	_progressBarlevel->SetPercent(item->GetProgressBarPercent());
	_progressBarlevelVfx->SetPercent(item->GetProgressBarPercent());


	// 선택되었을 경우, 외곽선 이미지 하이라이트
	_widgetSwitcherSelected->SetActiveWidgetIndex(item->GetIsSelected());

	IUserObjectListEntry::NativeOnListItemObjectSet(InListItem);
}

void UGsUICodexMapGroupListEntry::SetSelectImage(bool IsOn)
{
	_widgetSwitcherSelected->SetActiveWidgetIndex(IsOn);
}
