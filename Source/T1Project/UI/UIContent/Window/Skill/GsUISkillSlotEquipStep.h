// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/UIContent/Window/Skill/GsUISkillSlotEquipBase.h"
#include "GsUISkillSlotEquipStep.generated.h"


/**
 * 스킬창 스킬 상세부분의 업그레이드 스킬 단계 표시
 */
UCLASS()
class T1PROJECT_API UGsUISkillSlotEquipStep : public UGsUISkillSlotEquipBase
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidget* _uiArrow;

public:
	DECLARE_DELEGATE_OneParam(FOnClickSlot, UGsUISkillSlotEquipStep*);
	
protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidget* _effectSelected;

public:
	FOnClickSlot OnClickSkillSlot;

protected:
	virtual void NativeConstruct() override;

protected:
	virtual void OnClickSlot() override;

public:
	void SetIsSelected(bool bInEnable);
	void SetIsLearned(bool bIsLearned);
	void SetIsLastSlot(bool bIsLastSlot);
};
