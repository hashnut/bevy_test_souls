// Fill out your copyright notice in the Description page of Project Settings.


#include "GsSkillHandlerNonPlayer.h"
#include "Engine/Classes/Engine/World.h"
#include "Engine/Public/TimerManager.h"

#include "Shared/Shared/SharedPackets/PD_World_Client_Bypass.h"

#include "Management/ScopeGlobal/GsSkillManager.h"
#include "Management/ScopeGame/GsGameObjectManager.h"

#include "GameObject/Abnormality/CrowdControl/GsCrowdControlBase.h"
#include "GameObject/Skill/Runner/GsSkillRunnerCreature.h"
#include "GameObject/ObjectClass/GsGameObjectNonPlayer.h"
#include "GameObject/State/GsGameObjectStateManager.h"
#include "GameObject/Data/GsGameObjectData.h"

#include "DataSchema/Abnormality/GsSchemaAbnormalitySet.h"
#include "UTIL/GsDir.h"
#include "IdleEmotion/GsIdleEmotionLocalRunner.h"

void FGsSkillHandlerNonPlayer::Initialize(UGsGameObjectBase* Owner)
{
	Super::Initialize(Owner);

	if (_owner->GetActor() != nullptr)
	{
		World = _owner->GetActor()->GetWorld();
	}
}

void FGsSkillHandlerNonPlayer::Finalize()
{
	// 쿨타임 해제
	if (_globalCoolTimer.IsValid())
	{
		// 논플레이어 액터가 널이면 어떻게 하지...
		if (_owner->GetActor() != nullptr)
		{
			if (UWorld* world = _owner->GetActor()->GetWorld())
			{
				world->GetTimerManager().ClearTimer(_globalCoolTimer);
			}
		}
		else if (nullptr != World)
		{
			World->GetTimerManager().ClearTimer(_globalCoolTimer);
		}
	}

	Super::Finalize();
}

void FGsSkillHandlerNonPlayer::LoadSocial()
{
	Super::LoadSocial();

	_listSocial.Empty();
	GSkill()->GetNpcCommonActionSet(_listSocial, _owner->GetData()->GetTableId(), CommonActionType::SOCIAL);
	GSkill()->GetNpcCommonActionSet(_listSocial, _owner->GetData()->GetTableId(), CommonActionType::IDLE_SOCIAL);
}

// Super 메서드 호출 금지
void FGsSkillHandlerNonPlayer::CreateSkillRunner()
{
	_skillRunner = new FGsSkillRunnerCreature([this](int skillid, bool isEnd) { CallbackEndRunner(skillid, isEnd); });
}

void FGsSkillHandlerNonPlayer::CreateIdleEmotionRunner()
{
	if (auto data = GSkill()->GetIdleEmotionActionData(_owner))
	{
		_idleEmotionRunner = MakeShared<FGsIdleEmotionLocalRunner>(_owner);
		_idleEmotionRunner->InitIdleEmotion(data);

		SetAutoIdleEmotionEnable(true);
	}
}

void FGsSkillHandlerNonPlayer::StopSkill(int ID)
{
	Super::StopSkill(ID);

	_skillRunner->EndRunner();
}


void FGsSkillHandlerNonPlayer::NetActiveSkillStart(PD::SC::PKT_SC_START_SKILL_READ* Packet,
	const struct FGsSchemaSkillSet* skillData)
{
	Super::NetActiveSkillStart(Packet, skillData);
	// 선판정 스킬 체크
	// 선판정의 경우는 NetSkillEnd 에서 상태변경을 진행한다,
	if (false == skillData->noTimeline)
	{
		// Attack 상태 전환
		FGsGameObjectStateManager::ProcessEvent(_owner, EGsStateBase::Attack);

		// SkillRunner 설정
		float fSpeedRate = skillData->isNormal ? GetAttackSpeedRate() : GetCastSpeedRate();
		_skillRunner->StartRunner(_owner, skillData, fSpeedRate, GetNetSkillData());
	}
}

void FGsSkillHandlerNonPlayer::NetActiveSkillEnd(PD::SC::PKT_SC_SKILL_FINISHED_READ* Packet, const FGsSchemaSkillSet* skillData)
{
	Super::NetActiveSkillEnd(Packet, skillData);

	// 선판정 스킬 체크
		// 선판정의 경우는 NetSkillEnd 에서 상태변경을 진행한다,
	if (skillData && skillData->noTimeline)
	{
		// Attack 상태 전환
		FGsGameObjectStateManager::ProcessEvent(_owner, EGsStateBase::Attack);

		// SkillRunner 설정
		float fSpeedRate = skillData->isNormal ? GetAttackSpeedRate() : GetCastSpeedRate();
		_skillRunner->StartRunner(_owner, skillData, fSpeedRate, GetNetSkillData());
	}
}

void FGsSkillHandlerNonPlayer::SetAutoIdleEmotionEnable(bool enbable)
{
	if (auto emotionHandler = StaticCastSharedPtr<FGsIdleEmotionLocalRunner>(_idleEmotionRunner))
		emotionHandler->SetEnableAutoIdleEmotionMotion(enbable);
}
