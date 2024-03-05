// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/UIContent/Common/Reward/GsUIRewardInfoBase.h"
#include "GsUIRewardInfoMainSubBase.generated.h"

class UTextBlock;

/**
 * 서브보상이 있을 수 있는 UI의 Base클래스. Abstract 클래스
 * - UGsUIRewardInfoMainSub
 * - UGsUIRewardInfoMainSubRandom
 */
UCLASS(Abstract)
class T1PROJECT_API UGsUIRewardInfoMainSubBase : public UGsUIRewardInfoBase
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockTitle;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockSub;

protected:
	FGsRewardUnboxingData _rewardDataMain;
	FGsRewardUnboxingData _rewardDataSub;

	// 유저가 별도로 입력한 값
	FText _textTitleCustomized;
	FText _textSubTitleCustomized;

protected:
	virtual void NativeDestruct() override;

public:
	virtual bool SetData(RewardId InRewardIdMain, RewardId InRewardIdSub) override;
	virtual bool SetData(RewardId InRewardIdMain, RewardId InRewardIdSub, const TArray<RewardBoxItemIdPair>& InList) override;
	virtual bool SetData(const FGsRewardUnboxingData* InMainData, const FGsRewardUnboxingData* InSubData) override;
	virtual void SetTitleText(const FText& InText) override;
	virtual void SetSubTitleText(const FText& InText) override;
	virtual void SetVisibleTitle(bool bIsVisible) override;

	virtual RewardType GetRewardTypeMain() const override { return _rewardDataMain._rewardType; }
	virtual RewardType GetRewardTypeSub() const override { return _rewardDataSub._rewardType; }

protected:
	virtual void SetUIInter(const FGsRewardUnboxingData* InMainData, const FGsRewardUnboxingData* InSubData) {}

public:
	FText GetTitleTextByRewardType(RewardType InType, bool bHasSubReward);
	FText GetSubTitleTextByRewardType(RewardType InType);
};
