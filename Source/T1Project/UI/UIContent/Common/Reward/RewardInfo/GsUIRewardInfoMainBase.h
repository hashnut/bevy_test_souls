// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/UIContent/Common/Reward/GsUIRewardInfoBase.h"
#include "GsUIRewardInfoMainBase.generated.h"

class UTextBlock;

/**
 * 메인 보상만 존재하는 UI의 Base클래스. Abstract 클래스
 * - UGsUIRewardInfoMainRandom
 */
UCLASS(Abstract)
class T1PROJECT_API UGsUIRewardInfoMainBase : public UGsUIRewardInfoBase
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockTitle;

protected:
	FGsRewardUnboxingData _rewardDataMain;

protected:
	virtual void NativeDestruct() override;

public:
	virtual bool SetData(RewardId InRewardIdMain, RewardId InRewardIdSub) override;
	virtual bool SetData(RewardId InRewardIdMain, RewardId InRewardIdSub, const TArray<RewardBoxItemIdPair>& InList) override;
	virtual bool SetData(const FGsRewardUnboxingData* InMainData, const FGsRewardUnboxingData* InSubData = nullptr) override;
	virtual void SetTitleText(const FText& InText) override;
	virtual void SetVisibleTitle(bool bIsVisible) override;

	virtual RewardType GetRewardTypeMain() const override { return _rewardDataMain._rewardType; }

protected:
	virtual void SetUIInter(const FGsRewardUnboxingData* InMainData) {}
};
