// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameObject/Define/GsGameObjectDefine.h"

/**
 * Skill 사용 가능 체크 클래스 (내캐릭터 전용 기능 클래스)
 */
class T1PROJECT_API IGsSkillCheckBase
{
public:
	// EGsSkilAvailableResultType::Pass = 사용 가능 상태	
public:
	IGsSkillCheckBase()				= default;
	virtual ~IGsSkillCheckBase()	= default;

public:
	virtual void Initialize() {};
	virtual void Finalize() {};

public:
	// 상속시 구현부 메서드
	virtual bool ProcessCheck(const class FGsSkill* NextSkill) = 0;

public:
	virtual EGsSkillCheckResultType GetErrType() const = 0;
	virtual FText GetErrMessage() const = 0;
};



