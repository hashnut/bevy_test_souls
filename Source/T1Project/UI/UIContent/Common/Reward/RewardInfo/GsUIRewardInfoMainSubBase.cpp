// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUIRewardInfoMainSubBase.h"
#include "Components/TextBlock.h"
#include "UI/UIContent/Common/Reward/GsUIRewardIconListDefault.h"
#include "Shared/Client/SharedEnums/SharedCommonEnum.h"


void UGsUIRewardInfoMainSubBase::NativeDestruct()
{
	_rewardDataMain.Clear();
	_rewardDataSub.Clear();

	Super::Destruct();
}

bool UGsUIRewardInfoMainSubBase::SetData(RewardId InRewardIdMain, RewardId InRewardIdSub)
{
	_rewardDataMain.Clear();
	_rewardDataSub.Clear();

	// Exp, 재화 아이콘 출력
	_rewardDataMain._bUseCurrencyIcon = true;

	// 메인보상이 없을 수 없다. 에러상황으로 false 리턴.
	if (false == FGsRewardHelper::UnboxingRewardData(InRewardIdMain, _rewardDataMain))
	{
		SetUIInter(nullptr, nullptr);
		return false;
	}

	// 서브 보상은 없을 수 있음
	if (FGsRewardHelper::UnboxingRewardData(InRewardIdSub, _rewardDataSub))
	{
		SetUIInter(&_rewardDataMain, &_rewardDataSub);
	}
	else
	{
		SetUIInter(&_rewardDataMain, nullptr);
	}	

	return true;
}

bool UGsUIRewardInfoMainSubBase::SetData(RewardId InRewardIdMain, RewardId InRewardIdSub, const TArray<RewardBoxItemIdPair>& InList)
{
	_rewardDataMain.Clear();
	_rewardDataSub.Clear();

	// Exp, 재화 아이콘 출력
	_rewardDataMain._bUseCurrencyIcon = true;

	// 여기로 들어왔고 서브보상이 있으면, 무조건 메인보상은 ALL타입, 서브보상이 리스트를 쓰게된다
	if (INVALID_REWARD_ID == InRewardIdSub)
	{
		// 메인보상이 없을 수 없다. 에러상황으로 false 리턴.
		if (false == FGsRewardHelper::UnboxingRewardData(InRewardIdMain, InList, _rewardDataMain))
		{
			SetUIInter(nullptr, nullptr);
			return false;
		}
		
		SetUIInter(&_rewardDataMain, nullptr);
	}
	else
	{
		// 메인보상이 없을 수 없다. 에러상황으로 false 리턴.
		if (false == FGsRewardHelper::UnboxingRewardData(InRewardIdMain, _rewardDataMain))
		{
			SetUIInter(nullptr, nullptr);
			return false;
		}

		// 서브보상이 리스트를 쓴다. 리스트에 이상이 있으면 리턴
		if (false == FGsRewardHelper::UnboxingRewardData(InRewardIdSub, InList, _rewardDataSub))
		{
			SetUIInter(nullptr, nullptr);
			return false;
		}
		
		SetUIInter(&_rewardDataMain, &_rewardDataSub);
	}

	return true;
}

bool UGsUIRewardInfoMainSubBase::SetData(const FGsRewardUnboxingData* InMainData, const FGsRewardUnboxingData* InSubData)
{
	_rewardDataMain.Clear();
	_rewardDataSub.Clear();

	// Exp, 재화 아이콘 출력
	_rewardDataMain._bUseCurrencyIcon = true;

	// 메인보상이 없을 수 없다. 에러상황으로 false 리턴.
	if (nullptr == InMainData)
	{
		SetUIInter(nullptr, nullptr);
		return false;
	}

	_rewardDataMain.SetData(InMainData);
	if (InSubData)
	{
		_rewardDataSub.SetData(InSubData);
		SetUIInter(&_rewardDataMain, &_rewardDataSub);
	}
	else
	{
		SetUIInter(&_rewardDataMain, nullptr);
	}

	return true;
}

void UGsUIRewardInfoMainSubBase::SetTitleText(const FText& InText)
{ 
	_textBlockTitle->SetText(InText);
}

void UGsUIRewardInfoMainSubBase::SetSubTitleText(const FText& InText)
{
	_textBlockSub->SetText(InText);
}

void UGsUIRewardInfoMainSubBase::SetVisibleTitle(bool bIsVisible)
{
	_textBlockTitle->SetVisibility(bIsVisible ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed);
}

FText UGsUIRewardInfoMainSubBase::GetTitleTextByRewardType(RewardType InType, bool bHasSubReward)
{
	FText outText;

	switch (InType)
	{
	case RewardType::PICK_ONE:
		// TEXT: 클리어 보상(선택)
		FText::FindText(TEXT("QuestUIText"), TEXT("CompleteRewardSelect"), outText);
		break;
	case RewardType::RANDOM_ONE:
	case RewardType::PICK_REWARD_BOX_LIST:
		// TEXT: 클리어 보상(랜덤)
		FText::FindText(TEXT("QuestUIText"), TEXT("CompleteRewardRandom"), outText);
		break;
	default:
		if (bHasSubReward)
		{
			// TEXT: 클리어 보상(기본)
			FText::FindText(TEXT("QuestUIText"), TEXT("CompleteRewardNormal"), outText);
		}
		else
		{
			// TEXT: 클리어 보상
			FText::FindText(TEXT("QuestUIText"), TEXT("CompleteReward"), outText);
		}
		break;
	}

	return outText;
}

FText UGsUIRewardInfoMainSubBase::GetSubTitleTextByRewardType(RewardType InType)
{
	// 원래 PICK_ONE, RANDOM_ONE은 들어오면 안되지만 자연스럽게 보이기 위해 설정
	FText outText;

	switch (InType)
	{
	case RewardType::PICK_ONE:
	case RewardType::SUB_PICK_ONE:
		// TEXT: 클리어 보상(선택)
		FText::FindText(TEXT("QuestUIText"), TEXT("CompleteRewardSelect"), outText);
		break;
	case RewardType::RANDOM_ONE:
	case RewardType::SUB_RANDOM_ONE:
	case RewardType::PICK_REWARD_BOX_LIST:
		// TEXT: 클리어 보상(랜덤)
		FText::FindText(TEXT("QuestUIText"), TEXT("CompleteRewardRandom"), outText);
		break;
	default:
		// 에러 상황으로 텍스트 없음
		break;
	}
	
	return outText;
}
