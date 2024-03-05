// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/UIContent/Common/Skill/GsUISkillDetailInfoBase.h"
#include "GsUISkillDetailInfoDefault.generated.h"

class UScrollBox;
class UPanelWidget;
class FGsSkillUIData;

/**
 * 스킬 상세정보 UI 기본
 */
UCLASS()
class T1PROJECT_API UGsUISkillDetailInfoDefault : public UGsUISkillDetailInfoBase
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UPanelWidget* _panelRoot;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UScrollBox* _scroll;

public:
	virtual void SetDataAll(FGsSkillUIData* InData) override;
	virtual void SetData(const FGsSkill* InData, int32 InOverrideLevel = 0) override;
	virtual void SetPassivityData(PassivityId InPassivityId) override;
};
