// Fill out your copyright notice in the Description page of Project Settings.


#include "GsSkillHandlerRemotePlayer.h"
#include "Shared/Shared/SharedPackets/PD_World_Client_Bypass.h"
#include "Shared/Shared/SharedInclude/SharedFuncs.h"

#include "ActorComponentEx/GsHommingProjectileComponent.h"
#include "ActorEx/GsCharacterBase.h"

#include "GameObject/Abnormality/CrowdControl/GsCrowdControlBase.h"
#include "GameObject/Skill/Runner/GsSkillRunnerCreature.h"
#include "GameObject/State/RemotePlayer/GsStateRemotePlayerAttack.h"
#include "GameObject/State/GsGameObjectStateManager.h"
#include "GameObject/ObjectClass/GsGameObjectRemotePlayer.h"
#include "GameObject/State/GsGameObjectStateManager.h"
#include "GameObject/Movement/GsMovementHandlerBase.h"

#include "Runner/GsSkillRunnerSkillPreviewPlayer.h"
#include "IdleEmotion/GsIdleEmotionBaseRunner.h"

#include "Management/ScopeGlobal/GsSkillManager.h"

#include "DataSchema/Abnormality/GsSchemaAbnormalitySet.h"

#include "UTIL/GsDir.h"
#include "../T1Project.h"
#include "../UTIL/GsTimeUtil.h"


void FGsSkillHandlerRemotePlayer::Initialize(UGsGameObjectBase* owner)
{
	Super::Initialize(owner);
}

void FGsSkillHandlerRemotePlayer::Finalize()
{
	Super::Finalize();
}

void FGsSkillHandlerRemotePlayer::LoadSocial()
{
	Super::LoadSocial();

	_listSocial.Empty();
	GSkill()->GetPlayerSocialSet(_listSocial, _owner);
}

// Super 메서드 호출 금지
void FGsSkillHandlerRemotePlayer::CreateSkillRunner()
{
	_skillRunner = new FGsSkillRunnerCreature([this](int skillid, bool isEnd) { CallbackEndRunner(skillid, isEnd); });
}

void FGsSkillHandlerRemotePlayer::StopSkill(int ID)
{
	Super::StopSkill(ID);

	_skillRunner->EndRunner();
}

void FGsSkillHandlerRemotePlayer::Update(float Delta)
{
	if (_skillRunner->IsValid() && false == _skillRunner->Update(Delta))
	{
		if (!_idleEmotionRunner.IsValid() || _idleEmotionRunner->IsEmotionMotionFinished())
		{
			Super::Update(Delta);
		}
	}
	else
	{
		Super::Update(Delta);
	}
	
}

void FGsSkillHandlerRemotePlayer::NetActiveSkillStart(PD::SC::PKT_SC_START_SKILL_READ* Packet, const FGsSchemaSkillSet* skillData)
{
#if WITH_EDITOR
	GSLOG(Log, TEXT("[RemoteSkillDebug] [ %s ] StartSkill Ack %d, %d"), *FGsDateTimeUTIL::GetCurrentTime(),
		Packet->SkillId(), Packet->SkillStartType());
#endif
	Super::NetActiveSkillStart(Packet, skillData);

	// 캐스팅 스킬
	if (skillData->category == SkillCategory::CAST)
	{
		if (Packet->SkillStartType() == SkillStartType::CAST)
		{
			// SkillRunner가 실행중이었다면 정지
			// Remote 오브젝트의 상태 체크는 매우 느슨하므로 AttackExit에서 EndRunner 처리가 있지만
			// 그쪽에서만 호출 처리될 경우 attack -> Casting -> Idle 상태로 처리된다.
			if (_skillRunner->IsValid())
			{
				_skillRunner->EndRunner();
			}

			FGsGameObjectStateManager::ProcessEvent(_owner, EGsStateBase::Casting);
			return;
		}

		// SkillStartType::SKILL
		// 누락된 AnimNotify 선처리는 각 State클래스 에서 필요에 따라 처리 할것
		// FGsStateRemotePlayerAttack::PlayMontageMissingAnimNotify(_owner, this, skillData, skillData->castInfo.sectionName);

		// 해당 위치로 강제 이동
		// 스킬 전체를 적용시킬것인지 추후 고려
// 			if (FGsMovementHandlerBase* movementHandler = _owner->GetMovementHandler())
// 			{
// 				FVector pos = Packet->CurPos();
// 				FVector dir = FGsDir::angleToDir(Packet->CurrDir());
// 				movementHandler->DirectMoveUpdate(pos, dir);
// 			}
	}

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

void FGsSkillHandlerRemotePlayer::NetActiveSkillEnd(PD::SC::PKT_SC_SKILL_FINISHED_READ* Packet, const FGsSchemaSkillSet* skillData)
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
	else
	{
		// 캐스팅 완료 보장이 되지 않고 중간에 SC_SKILL_FINISHED가 내려올수 있음
		// 상태 회복 처리			
		if (FGsGameObjectStateManager* fsm = _owner->GetBaseFSM())
		{
			if (fsm->IsState(EGsStateBase::Casting))
			{
				fsm->ProcessEvent(EGsStateBase::AttackEnd);
			}
		}
	}

#if WITH_EDITOR
	GSLOG(Log, TEXT("[RemoteSkillDebug] [ %s ] SC_SKILL_FINISHED SkillId : %d  Reson : %d Reserve Id : %d"),
		*FGsDateTimeUTIL::GetCurrentTime(), Packet->SkillId(), Packet->SkillFinishReason(), skillData->id);
#endif

}