// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/UIContent/Common/Reward/RewardInfo/GsUIRewardInfoMainBase.h"
#include "GsUIRewardInfoMainRandom.generated.h"

class UPanelWidget;
class UGsUIRewardIconListRandom;

/**
 * 메인 보상만 존재하며, 랜덤 연출을 사용할 경우 사용(RewardType::RANDOM_ONE)
 */
UCLASS()
class T1PROJECT_API UGsUIRewardInfoMainRandom : public UGsUIRewardInfoMainBase
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UPanelWidget* _panelMain;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUIRewardIconListRandom* _iconListMain;

public:
	virtual void SetEnableToggleGroup(bool bInEnable) override;
	virtual void SetDetailPopup(bool bInEnable, bool bInCenterPopup = true) override;
	
	virtual void SetReceivedIndex(ItemBagIndex InIndex) override;
	virtual bool IsRandomEffect() const override { return true; }

protected:
	void SetUIInter(const FGsRewardUnboxingData* InMainData);
};
