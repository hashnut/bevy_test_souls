// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Tutorial/Action/GsTutorialActionBase.h"

struct FGsSchemaTutorialActionUIInteraction;
class UGsUIWindowSkill;
class UWidget;

/**
 * 스킬 튜토리얼에서 스킬 리스트 선택 기능
 */
class T1PROJECT_API FGsTutorialActionSkillSelect final : public FGsTutorialActionBase
{
public:
	using Super = FGsTutorialActionBase;

private:
	const FGsSchemaTutorialActionUIInteraction* _actionUITable = nullptr;
	TWeakObjectPtr<UGsUIWindowSkill> _window;
	SkillId _skillId = INVALID_SKILL_ID;

public:
	FGsTutorialActionSkillSelect() : Super()
	{
	}
	virtual ~FGsTutorialActionSkillSelect() = default;

public:
	virtual void OnPrevStart(const IGsMessageParam* InParam) override;
	virtual bool OnStart() override;
	virtual void OnEnd(bool bInSendMessage = true) override;

private:
	void OnTutorialSkillListEvent(UWidget* InTargetWidget);
};
