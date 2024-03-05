// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GsSkillHandlerBase.h"
#include "Engine/EngineTypes.h"

class UGsGameObjectBase;

/**
 * 
 */
class T1PROJECT_API FGsSkillHandlerNonPlayer : public FGsSkillHandlerBase
{
	using Super = FGsSkillHandlerBase;

private:
	// 글로벌 쿨타임 길이(ai용, 임시)
	float _globalCoolTimeLen;
	// 글로벌 쿨타임 인지?(ai용, 임시)
	bool _isGlobalCoolTime;

	FTimerHandle _globalCoolTimer;

	// 타이머 해제용 월드
	UWorld* World;

public:
	FGsSkillHandlerNonPlayer() :_isGlobalCoolTime(false) {};
	virtual ~FGsSkillHandlerNonPlayer() = default;

public:
	virtual void Initialize(UGsGameObjectBase* Owner) override;
	virtual void Finalize() override;

public:
	virtual void LoadSocial() override;

protected:
	virtual void CreateSkillRunner() override;
	virtual void CreateIdleEmotionRunner() override;

public:
	virtual void StopSkill(int ID = 0) override;

	// 서버 응답에 따른 처리
protected:
	virtual void NetActiveSkillStart(PD::SC::PKT_SC_START_SKILL_READ* Packet, const struct FGsSchemaSkillSet* skillData) override;
	virtual void NetActiveSkillEnd(PD::SC::PKT_SC_SKILL_FINISHED_READ* Packet, const FGsSchemaSkillSet* skillData) override;

	// get
public:
	bool GetIsGlobalCoolTime() const { return _isGlobalCoolTime; }

public:
	virtual void SetAutoIdleEmotionEnable(bool enbable) override;
};
