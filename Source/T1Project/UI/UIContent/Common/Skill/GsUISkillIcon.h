// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "GsUISkillIcon.generated.h"

class UImage;
class FGsSkill;

/**
 * 스킬 아이콘
 */
UCLASS()
class T1PROJECT_API UGsUISkillIcon : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUIIcon", meta = (BindWidget))
	UImage* _imgIcon;

protected:
	SkillId _skillId = INVALID_SKILL_ID;

public:
	void SetData(const FGsSkill* InSkill);

public:
	void SetIconImageSprite(class UPaperSprite* InSprite);
	void SetGrayscale(bool bInIsGray);

	SkillId GetSkillId() const { return _skillId; }
};
