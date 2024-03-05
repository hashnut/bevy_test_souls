// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUIRewardInfoMainSub.h"
#include "Components/PanelWidget.h"
#include "UI/UIContent/Common/GsUICurrencySlot.h"
#include "UI/UIContent/Common/Reward/GsUIRewardIconListDefault.h"
#include "Shared/Client/SharedEnums/SharedCommonEnum.h"


void UGsUIRewardInfoMainSub::SetEnableToggleGroup(bool bInEnable)
{
	_iconListMain->SetEnableToggleGroup(bInEnable);
	_iconListSub->SetEnableToggleGroup(bInEnable);
}

void UGsUIRewardInfoMainSub::SetDetailPopup(bool bInEnable, bool bInCenterPopup)
{
	_iconListMain->SetDetailPopup(bInEnable, bInCenterPopup);
	_iconListSub->SetDetailPopup(bInEnable, bInCenterPopup);
}

ItemBagIndex UGsUIRewardInfoMainSub::GetSelectedIndexMain() const
{
	return _iconListMain->GetSelectedIndex();
}

ItemBagIndex UGsUIRewardInfoMainSub::GetSelectedIndexSub() const
{
	return _iconListSub->GetSelectedIndex();
}

void UGsUIRewardInfoMainSub::SetUIInter(const FGsRewardUnboxingData* InMainData, const FGsRewardUnboxingData* InSubData)
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
