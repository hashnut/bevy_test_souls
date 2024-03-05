// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GsSkillHandlerBase.h"

class UGsGameObjectBase;

/**
 * 
 */
class T1PROJECT_API FGsSkillHandlerRemotePlayer : public FGsSkillHandlerBase
{
	using Super = FGsSkillHandlerBase;

public:
	FGsSkillHandlerRemotePlayer()			= default;
	virtual ~FGsSkillHandlerRemotePlayer()	= default;

public:
	virtual void Initialize(UGsGameObjectBase* Owner) override;
	virtual void Finalize() override;

public:
	virtual void LoadSocial() override;

protected:
	virtual void CreateSkillRunner() override;

public:
	virtual void StopSkill(int ID) override;

	virtual void Update(float Delta) override;

	// 서버 응답에 따른 처리
protected:
	virtual void NetActiveSkillStart(PD::SC::PKT_SC_START_SKILL_READ* Packet, const FGsSchemaSkillSet* skillData) override;
	virtual void NetActiveSkillEnd(PD::SC::PKT_SC_SKILL_FINISHED_READ* Packet, const FGsSchemaSkillSet* skillData) override;
};
