// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "GsSkillHandlerRemotePlayer.h"
#include "Shared/Shared/SharedPackets/PD_World_Client_Bypass.h"

/**
 * 
 */
class FGsBuffSkillRunnerPreviewPlayer;
class T1PROJECT_API FGsSkillHandlerSkillPreviewPlayer : public FGsSkillHandlerRemotePlayer
{
	using Super = FGsSkillHandlerRemotePlayer;

protected:
	FGsBuffSkillRunnerPreviewPlayer* _buffSkillRunner{ nullptr };

public:
	FGsSkillHandlerSkillPreviewPlayer() = default;
	virtual ~FGsSkillHandlerSkillPreviewPlayer() = default;

	virtual void Initialize(UGsGameObjectBase* Owner) override;
	virtual void Finalize() override;
	virtual void Update(float Delta) override;

protected:
	virtual void CreateSkillRunner() override;

protected:
	// 서버 응답에 따른 처리
	virtual void NetActiveSkillStart(PD::SC::PKT_SC_START_SKILL_READ* Packet, const FGsSchemaSkillSet* skillData) override;
	virtual void NetBuffSkillStart(PD::SC::PKT_SC_START_SKILL_READ* Packet, const FGsSchemaSkillSet* skillData) override;
};
