// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/UIContent/HUD/Skill/GsUISkillButtonActive.h"
#include "GsUISkillButtonCostume.generated.h"

class UImage;

UCLASS()
class T1PROJECT_API UGsUISkillButtonCostume : public UGsUISkillButtonActive
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UImage* _imgLock;

protected:
	virtual void OnClickSlot() override;

public:
	virtual void SetData(const FGsSkill* InData) override;
	virtual void SetEmptySlot() override;

protected:
	void SetLockImage(bool bIsLocked);

protected:
	bool IsLearnedCostumeSkill() const;
	bool IsUnlockCostumeSkill() const;
};
