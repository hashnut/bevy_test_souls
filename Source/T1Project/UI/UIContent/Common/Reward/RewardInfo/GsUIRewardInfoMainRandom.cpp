// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUIRewardInfoMainRandom.h"
#include "Components/TextBlock.h"
#include "Components/PanelWidget.h"
#include "UI/UIContent/Common/GsUICurrencySlot.h"
#include "UI/UIContent/Common/Reward/GsUIRewardIconListRandom.h"
#include "Shared/Client/SharedEnums/SharedCommonEnum.h"


void UGsUIRewardInfoMainRandom::SetEnableToggleGroup(bool bInEnable)
{
	_iconListMain->SetEnableToggleGroup(bInEnable);
}

void UGsUIRewardInfoMainRandom::SetDetailPopup(bool bInEnable, bool bInCenterPopup)
{
	_iconListMain->SetDetailPopup(bInEnable, bInCenterPopup);
}

void UGsUIRewardInfoMainRandom::SetReceivedIndex(ItemBagIndex InIndex)
{
	_iconListMain->SetReceivedIndex(InIndex);
}

void UGsUIRewardInfoMainRandom::SetUIInter(const FGsRewardUnboxingData* InMainData)
{
	// 메인보상이 없을 수 없다. 에러상황으로 false 리턴.
	if (nullptr == InMainData)
	{
		SetVisibility(ESlateVisibility::Collapsed);
		return;
	}

	// 메인 정보 갱신
	SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	_iconListMain->SetData(InMainData);

	// TEXT: 클리어 보상(랜덤)
	FText textTitle;
	FText::FindText(TEXT("QuestUIText"), TEXT("CompleteRewardRandom"), textTitle);
	SetTitleText(textTitle);
}
