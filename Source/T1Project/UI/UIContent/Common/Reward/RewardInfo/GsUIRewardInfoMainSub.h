// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/UIContent/Common/Reward/RewardInfo/GsUIRewardInfoMainSubBase.h"
#include "GsUIRewardInfoMainSub.generated.h"

class UPanelWidget;
class UGsUIRewardIconListDefault;

/**
 * 서브 보상이 있을 수 있고(SUB_PICK_ONE), 단순 정보 출력 시(RewardType::ALL, RewardType::SUB_PICK_ONE)
 */
UCLASS()
class T1PROJECT_API UGsUIRewardInfoMainSub : public UGsUIRewardInfoMainSubBase
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
	UGsUIRewardIconListDefault* _iconListSub;

public:
	virtual void SetEnableToggleGroup(bool bInEnable) override;
	virtual void SetDetailPopup(bool bInEnable, bool bInCenterPopup = true) override;
	virtual ItemBagIndex GetSelectedIndexMain() const override;
	virtual ItemBagIndex GetSelectedIndexSub() const override;

protected:
	virtual void SetUIInter(const FGsRewardUnboxingData* InMainData, const FGsRewardUnboxingData* InSubData) override;
};
