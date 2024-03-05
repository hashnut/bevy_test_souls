// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Tutorial/Tutorial/GsTutorialBase.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"

/**
 * 스킬 튜토리얼. 직전에 유저가 선택해서 받은 아이템의 무기타입에 맞춰 진행
 */
class T1PROJECT_API FGsTutorialSkill final : public FGsTutorialBase
{
public:
	using Super = FGsTutorialBase;

protected:
	TArray<int32> _targetSkillBookList;
	SkillId _skillId = INVALID_SKILL_ID;

public:
	FGsTutorialSkill() : Super()
	{
	}
	virtual ~FGsTutorialSkill() {};

public:
	virtual bool OnPrevStart() override;

protected:
	virtual bool StartActionInternal() override;

private:
	bool SetTargetSkillBookList();
};
