// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUIPopupMultiLevelRankPromotion.h"

#include "Components/TextBlock.h"

#include "Classes/PaperSprite.h"

#include "UI/UIControlLib/ContentWidget/GsButton.h"

#include "UI/UIContent/Helper/GsUIHelper.h"
#include "UI/UIContent/Window/MultiLevelRank/GsUIMultiLevelRankIcon.h"


void UGsUIPopupMultiLevelRankPromotion::BeginDestroy()
{
	if (nullptr != _btnClose)
	{
		_btnClose->OnClicked.RemoveDynamic(this, &UGsUIPopupMultiLevelRankPromotion::OnClickClose);
	}
	_btnClose = nullptr;

	Super::BeginDestroy();
}

void UGsUIPopupMultiLevelRankPromotion::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	_btnClose->OnClicked.AddDynamic(this, &UGsUIPopupMultiLevelRankPromotion::OnClickClose);
}

void UGsUIPopupMultiLevelRankPromotion::OnClickClose()
{
	Close();
}

void UGsUIPopupMultiLevelRankPromotion::SetData(const FSoftObjectPath& InIconPathMain, const FSoftObjectPath& InIconPathSub, int32 InCount, const FText& InPromotionName)
{
	_iconRank->SetMainImage(InIconPathMain);
	_iconRank->SetSubImage(InIconPathSub);
	_iconRank->SetSubImageCount(InCount);

	_textBlockName->SetText(InPromotionName);
}


void UGsUIPopupMultiLevelRankPromotion::OnInputCancel()
{
	OnClickClose();
}