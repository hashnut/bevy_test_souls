// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Tutorial/Action/GsTutorialActionBase.h"

struct FGsSchemaTutorialActionUIInteraction;
class UGsUIWindowSkill;
class UWidget;

/**
 * 스킬 장착
 */
class T1PROJECT_API FGsTutorialActionSkillEquip final : public FGsTutorialActionBase
{
public:
	using Super = FGsTutorialActionBase;

private:
	const FGsSchemaTutorialActionUIInteraction* _actionUITable = nullptr;

public:
	FGsTutorialActionSkillEquip() : Super()
	{
	}
	virtual ~FGsTutorialActionSkillEquip() = default;

	
public:
	virtual bool OnStart() override;
	virtual void OnEnd(bool bInSendMessage = true) override;
};
