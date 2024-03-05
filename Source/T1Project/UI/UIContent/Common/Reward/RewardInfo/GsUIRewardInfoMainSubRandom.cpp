// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUIRewardInfoMainSubRandom.h"
#include "Components/PanelWidget.h"
#include "UI/UIContent/Common/GsUICurrencySlot.h"
#include "UI/UIContent/Common/Reward/GsUIRewardIconListDefault.h"
#include "UI/UIContent/Common/Reward/GsUIRewardIconListRandom.h"
#include "Shared/Client/SharedEnums/SharedCommonEnum.h"


void UGsUIRewardInfoMainSubRandom::SetEnableToggleGroup(bool bInEnable)
{
	// 랜덤에선 토글기능 사용 안함
	_iconListMain->SetEnableToggleGroup(false);
	_iconListSub->SetEnableToggleGroup(false);
}

void UGsUIRewardInfoMainSubRandom::SetDetailPopup(bool bInEnable, bool bInCenterPopup)
{
	_iconListMain->SetDetailPopup(bInEnable, bInCenterPopup);
	_iconListSub->SetDetailPopup(bInEnable, bInCenterPopup);
}

void UGsUIRewardInfoMainSubRandom::SetReceivedIndex(ItemBagIndex InIndex)
{
	_iconListSub->SetReceivedIndex(InIndex);
}

void UGsUIRewardInfoMainSubRandom::SetUIInter(const FGsRewardUnboxingData* InMainData, const FGsRewardUnboxingData* InSubData)
{
	// 메인보상이 없을 수 없다. 에러상황으로 false 리턴.
	if (nullptr == InMainData)
	{
		SetVisibility(ESlateVisibility::Collapsed);
		return;
	}

	// 서브 보상은 없을 수 있음
	bool bHasSubReward = false;
	if (InSubData)
	{
		bHasSubReward = true;
		_panelSub->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

		// 서브 정보 갱신
		_iconListSub->SetData(InSubData);

		// 타이틀 세팅. 별도로 넣은 값이 있으면 해당값으로 세팅, 아니면 기본 규칙에 의해 세팅.
		SetSubTitleText(GetSubTitleTextByRewardType(InSubData->_rewardType));
	}
	else
	{
		_panelSub->SetVisibility(ESlateVisibility::Collapsed);
	}

	// 메인 정보 갱신
	SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	_iconListMain->SetData(InMainData);

	// 타이틀 세팅. 별도로 넣은 값이 있으면 해당 값으로 세팅, 아니면 기본 규칙에 의해 세팅.
	SetTitleText(GetTitleTextByRewardType(InMainData->_rewardType, bHasSubReward));
}