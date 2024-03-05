// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUIRewardInfoMainBase.h"
#include "Components/TextBlock.h"
#include "UI/UIContent/Common/Reward/GsUIRewardIconListDefault.h"
#include "Shared/Client/SharedEnums/SharedCommonEnum.h"


void UGsUIRewardInfoMainBase::NativeDestruct()
{
	_rewardDataMain.Clear();

	Super::Destruct();
}

bool UGsUIRewardInfoMainBase::SetData(RewardId InRewardIdMain, RewardId InRewardIdSub)
{
	_rewardDataMain.Clear();

	// Exp, 재화 아이콘 출력
	_rewardDataMain._bUseCurrencyIcon = true;

	// 메인보상이 없을 수 없다. 에러상황으로 false 리턴.
	if (false == FGsRewardHelper::UnboxingRewardData(InRewardIdMain, _rewardDataMain))
	{
		SetUIInter(nullptr);
		return false;
	}

	SetUIInter(&_rewardDataMain);

	return true;
}

bool UGsUIRewardInfoMainBase::SetData(RewardId InRewardIdMain, RewardId InRewardIdSub, const TArray<RewardBoxItemIdPair>& InList)
{
	_rewardDataMain.Clear();

	// Exp, 재화 아이콘 출력
	_rewardDataMain._bUseCurrencyIcon = true;

	// 메인보상이 없을 수 없다. 에러상황으로 false 리턴.
	if (false == FGsRewardHelper::UnboxingRewardData(InRewardIdMain, InList, _rewardDataMain))
	{
		SetUIInter(nullptr);
		return false;
	}

	SetUIInter(&_rewardDataMain);

	return true;
}

bool UGsUIRewardInfoMainBase::SetData(const FGsRewardUnboxingData* InMainData, const FGsRewardUnboxingData* InSubData)
{
	_rewardDataMain.Clear();

	// 메인보상이 없을 수 없다. 에러상황으로 false 리턴.
	if (nullptr == InMainData)
	{
		SetUIInter(nullptr);
		return false;
	}

	_rewardDataMain.SetData(InMainData);
	SetUIInter(&_rewardDataMain);

	return true;
}

void UGsUIRewardInfoMainBase::UGsUIRewardInfoMainBase::SetTitleText(const FText& InText)
{
	_textBlockTitle->SetText(InText);
}

void UGsUIRewardInfoMainBase::SetVisibleTitle(bool bIsVisible)
{
	_textBlockTitle->SetVisibility(bIsVisible ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed);
}
