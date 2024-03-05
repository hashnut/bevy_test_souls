// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "GsUITargetDetailInfoBase.generated.h"

class FGsSkillUIData;
class FGsSkill;

/**
 * 스킬, 패시비티 등의 상세정보를 표시하기 위한 UI
 * - UGsUISkillDetailInfoBase
 * - UGsUISkillDetailInfoPassivity
 */
UCLASS(Abstract)
class T1PROJECT_API UGsUITargetDetailInfoBase : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void SetDataAll(FGsSkillUIData* InData) {}
	virtual void SetData(const FGsSkill* InData, int32 InOverrideLevel = 0) {}
	virtual void SetPassivityData(PassivityId InPassivityId) {}

	// 스킬창에서만 사용함
	virtual void SetEquipMode(bool bIsEnableEquip, bool bIsEquipMode) {}
};
