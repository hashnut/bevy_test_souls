// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/UIContent/Common/Reward/RewardInfo/GsUIRewardInfoMainSubBase.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "GsUIRewardInfoMainSubRandom.generated.h"

class UPanelWidget;
class UGsUIRewardIconListDefault;
class UGsUIRewardIconListRandom;

/**
 * 서브 보상이 있을 수 있고(SUB_RANDOM_ONE), 랜덤 연출 필요 시
 */
UCLASS()
class T1PROJECT_API UGsUIRewardInfoMainSubRandom : public UGsUIRewardInfoMainSubBase
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UPanelWidget* _panelMain;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUIRewardIconListDefault* _iconListMain;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UPanelWidget* _panelSub;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUIRewardIconListRandom* _iconListSub;

public:
	virtual void SetEnableToggleGroup(bool bInEnable) override; // 이거 무조건 false여야 하는데? FIX:
	virtual void SetDetailPopup(bool bInEnable, bool bInCenterPopup = true) override;
	
	virtual void SetReceivedIndex(ItemBagIndex InIndex) override;
	virtual bool IsRandomEffect() const override { return true; }

protected:
	virtual void SetUIInter(const FGsRewardUnboxingData* InMainData, const FGsRewardUnboxingData* InSubData) override;
};
