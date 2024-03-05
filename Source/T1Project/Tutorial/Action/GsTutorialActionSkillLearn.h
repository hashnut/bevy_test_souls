// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Tutorial/Action/GsTutorialActionBase.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"

struct FGsSchemaTutorialActionUIInteraction;
class UGsUIWindowSkill;
class UWidget;
struct IGsMessageParam;

/**
 * 스킬 튜토리얼에서 습득 버튼 선택기능. 이미 배운스킬, 스킬북 없는 경우 등 예외 사항 시 종료를 위해 별도로 만들었음
 */
class T1PROJECT_API FGsTutorialActionSkillLearn final : public FGsTutorialActionBase
{
public:
	using Super = FGsTutorialActionBase;

private:
	const FGsSchemaTutorialActionUIInteraction* _actionUITable = nullptr;
	SkillId _skillId = INVALID_SKILL_ID;

public:
	FGsTutorialActionSkillLearn() : Super()
	{
	}
	virtual ~FGsTutorialActionSkillLearn() = default;

	
public:
	virtual void OnPrevStart(const IGsMessageParam* InParam) override;
	virtual bool OnStart() override;
	virtual void OnEnd(bool bInSendMessage = true) override;

	virtual void OnLearnSkill(SkillId InSkillId) override;
};
