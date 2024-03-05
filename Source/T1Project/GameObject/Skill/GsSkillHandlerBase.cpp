// Fill out your copyright notice in the Description page of Project Settings.


#include "GsSkillHandlerBase.h"
#include "Shared/Shared/SharedPackets/PD_World_Client_Bypass.h"
#include "Shared/Client/SharedEnums/SharedSkillEnum.h"

#include "ActorEx/GsCharacterPlayer.h"
#include "ActorComponentEx/GsHommingProjectileComponent.h"

#include "Data/GsDataContainManager.h"
#include "Data/GsGlobalConstant.h"

#include "Management/ScopeGlobal/GsSkillManager.h"
#include "Management/ScopeGame/GsChatManager.h"
#include "Management/ScopeGame/GsGameObjectManager.h"
#include "Management/GsMessageHolder.h"

#include "Message/MessageParam/GsGameObjectMessageParam.h"

#include "Notify/GsSkillNotifyNodeBeamParticle.h"
#include "Notify/GsSkillNotifyNodeGuideEffect.h"

#include "GameObject/Skill/Runner/GsSkillRunnerBase.h"
#include "GameObject/Movement/GsMovementHandlerBase.h"
#include "GameObject/Stat/GsStatBase.h"
#include "GameObject/Abnormality/CrowdControl/GsCrowdControlBase.h"
#include "GameObject/ObjectClass/GsGameObjectCreature.h"
#include "GameObject/State/GsGameObjectStateManager.h"
#include "GameObject/Data/GsGameObjectDataCreature.h"
#include "IdleEmotion/GsIdleEmotionBaseRunner.h"

#include "DataSchema/Skill/GsSchemaSkillCommon.h"
#include "DataSchema/Skill/GsSchemaSkillReaction.h"
#include "DataSchema/Skill/NotifyInfo/GsSchemaSkillNotifySkillGuide.h"
#include "DataSchema/Skill/NotifyInfo/GsSchemaSkillNotifyBeamParticle.h"
#include "DataSchema/User/SkillSlot/GsSchemaUserComboSkillSlotSet.h"
#include "DataSchema/Abnormality/GsSchemaAbnormalitySet.h"

#include "Util/GsHelper.h"
#include "UTIL/GsDir.h"


float FGsSkillHandlerBase::GetSkillAttackRange(UGsGameObjectCreature* Owner, const FGsSchemaSkillSet* SkillData)
{
	// 스탯 가져오기
	FGsGameObjectDataCreature* creatureData = Owner->GetCastData<FGsGameObjectDataCreature>();
	TSharedPtr<FGsStatBase> statBase = creatureData->GetStatBase();
	if (statBase.IsValid() == false)
		return 0.0f;

	int addtiveRange = 0;
	float scaleRange = 1.f;
	
	if (const FGsStatInfo* info = statBase->GetStatInfo(StatType::BONUS_APPROACH_RANGE_RATE))
	{
		int scaleVal = info->GetStatValue();
		if (scaleVal > 0)
		{
			scaleRange = (1 + scaleVal / 10000);
		}
	}
	if (const FGsStatInfo* info = statBase->GetStatInfo(StatType::BONUS_APPROACH_RANGE))
	{
		addtiveRange = info->GetStatValue();
	}

	float skillRange = SkillData->maxApproachRange + addtiveRange;
	skillRange *= scaleRange;
// 	float radius = SkillData->collisionInfoList.Last().type == SkillCollisionType::RECT ? 
// 		SkillData->collisionInfoList.Last().height * 0.5f :
// 		SkillData->collisionInfoList.Last().maxRadius;
// 	// Float 오차, 이동 싱크등을  생각해서 0.5m Offset을 일괄 반영
// 	return FMath::Max(0.1f, ((SkillData->collisionInfoList.Last().posOffset + radius)) - 50.f);
	return FMath::Max(1.f, skillRange);
}

float FGsSkillHandlerBase::GetSkillApproachRange(class UGsGameObjectCreature* Owner, const FGsSchemaSkillSet* SkillData)
{
	// 스탯 가져오기
	FGsGameObjectDataCreature* creatureData = Owner->GetCastData<FGsGameObjectDataCreature>();
	TSharedPtr<FGsStatBase> statBase = creatureData->GetStatBase();
	int addtiveRange = 0;
	float scaleRange = 1.f;

	if (const FGsStatInfo* info = statBase->GetStatInfo(StatType::BONUS_APPROACH_RANGE_RATE))
	{
		int scaleVal = info->GetStatValue();
		if (scaleVal > 0)
		{
			scaleRange = (1 + scaleVal / 10000);
		}
	}
	if (const FGsStatInfo* info = statBase->GetStatInfo(StatType::BONUS_APPROACH_RANGE))
	{
		addtiveRange = info->GetStatValue();
	}

	float approachRange = SkillData->approachRange + addtiveRange;
	approachRange *= scaleRange;
	return FMath::Max(1.f, approachRange);
}

EGsAnimSlotType FGsSkillHandlerBase::GetRandomCombSkillSlot(bool IsCritical, int ComboIndex)
{
	if (const FGsSchemaUserComboSkillSlotSet* comboSlotSet = UGsTableUtil::FindRowById<
		UGsTableUserComboSkillSlotSet, FGsSchemaUserComboSkillSlotSet>(ComboIndex))
	{
		const TArray<EGsAnimSlotType>& slotList = (IsCritical) ? comboSlotSet->criticalAttack.randomSlotType :
			comboSlotSet->normalAttack.randomSlotType;
		int randMax = slotList.Num();
		int selectIdx = FMath::RandHelper(slotList.Num());

		return slotList[selectIdx];
	}
	return EGsAnimSlotType::DefaultSlot;
}

void FGsSkillHandlerBase::SetEmotionLock(bool set)
{
	if (_idleEmotionRunner)
	{
		_idleEmotionRunner->SetEmotionLock(set);
	}
}

void FGsSkillHandlerBase::Initialize(UGsGameObjectBase* Owner)
{
	_owner = Owner;

	LoadSkill();
	LoadSocial();
	LoadAnimation();
	CreateSkillRunner();
	CreateIdleEmotionRunner();
}

void FGsSkillHandlerBase::CreateSkillRunner()
{
	_skillRunner = new FGsSkillRunnerBase([this](int skillid, bool isEnd) { CallbackEndRunner(skillid, isEnd); });
}

void FGsSkillHandlerBase::CreateIdleEmotionRunner()
{
	if (auto data = GSkill()->GetIdleEmotionActionData(_owner)) 
	{
		_idleEmotionRunner = MakeShared<FGsIdleEmotionBaseRunner>(_owner);
		_idleEmotionRunner->InitIdleEmotion(data);
	}
}

void FGsSkillHandlerBase::LoadSkill()
{
}

void FGsSkillHandlerBase::LoadSocial()
{
}

// 현재는 스킬 데이터 정보만큼 모든 애니메이션 리소스를 선로딩
void FGsSkillHandlerBase::LoadAnimation()
{
	// 스킬 애니메이션 정보 선로딩
	//if (SkillDataContainer)
	//{
	//	// Common Action Load
	//	for (const FGsCommonActionData& el : SkillDataContainer->GetCommonSkillData())
	//	{
	//		GResource()->PreLoad(EResourceType::AnimMontage, el.AniPath);
	//	}

	//	// _reaction Load
	//	for (const FGsCommonActionData& el : SkillDataContainer->GetReactionSkillData())
	//	{
	//		GResource()->PreLoad(EResourceType::AnimMontage, el.AniPath);
	//	}

	//	// Skill Load
	//	for (const FGsSkillSetData& el : SkillDataContainer->GetSkillData())
	//	{
	//		GResource()->PreLoad(EResourceType::AnimMontage, el.AniPath);
	//	}
	//}
}

void FGsSkillHandlerBase::Finalize()
{
	if (_skillRunner)
	{
		delete _skillRunner;
		_skillRunner = nullptr;
	}
	
	if (_idleEmotionRunner)
	{
		_idleEmotionRunner->FinalizeIdleEmotion();
		_idleEmotionRunner = nullptr;
	}
}

void FGsSkillHandlerBase::Update(float Delta)
{
	if (_skillRunner->IsValid() && false == _skillRunner->Update(Delta))
	{
		_skillRunner->EndRunner();
	}

	if (_idleEmotionRunner.IsValid())
	{
		_idleEmotionRunner->Update(Delta);
	}
}

bool FGsSkillHandlerBase::OnSkill(int ID)
{
	// 리액션 중에 입력을 막는건 내 플레이어만 처리한다.
	// 네트워크 동기화 이전엔 상관없지만 
	// 여기서 리액션 검사를 하면 네트워크 동기화시 문제가 될수 있음
	return true;
}

void FGsSkillHandlerBase::StopSkill(int Id)
{
}

// 서버 응답에 따른 처리
void FGsSkillHandlerBase::NetActiveSkillStart(PD::SC::PKT_SC_START_SKILL_READ* Packet, const FGsSchemaSkillSet* skillData)
{
	_skillNetData._skillId = Packet->SkillId();
	_skillNetData._skillLevel = Packet->SkillLevel();
	_skillNetData._curPos = Packet->CurPos();
	_skillNetData._curDir = Packet->CurrDir();
	_skillNetData._aimStartPos = Packet->AimStartPos();
	_skillNetData._aimTargetPos = Packet->AimTargetPos();
	_skillNetData._targetGameId = Packet->TargetId();
	_skillNetData._skillStartType = Packet->SkillStartType();

	// 유도체 캐시 정보 초기화
	AGsCharacterBase* characterActor = _owner->GetCharacter();
	if (nullptr != characterActor)
	{
		if (UGsHommingProjectileComponent* hommingComponent = characterActor->GetHommingProjectile())
		{
			hommingComponent->ClearHommingProjectileInfo();

			// 유도체 컴퍼넌트 타겟 설정
			if (GSGameObject())
			{
				if (UGsGameObjectBase* target = GSGameObject()->FindObject(EGsGameObjectType::Creature, Packet->TargetId()))
				{
					hommingComponent->SetTargetActor(target->GetCharacter());
				}
			}
		}
	}

	// 루트 모션 스킬인가 확인
// 		if (Packet->ReactionId() != 0)
// 		{
// 			if (UGsGameObjectCreature* castObject = _owner->CastGameObject<UGsGameObjectCreature>())
// 			{
// 				castObject->StartRootmotion(Packet->CurrDir(), Packet->CurPos(), Packet->ReactionId());
// 			}
// 
// 			// 충돌 무시 설정
// 			const FGsSchemaRootmotionSet* rootMotionTable = UGsTableUtil::FindRowById<
// 				UGsTableRootmotionSet, FGsSchemaRootmotionSet>(Packet->ReactionId());
// 			if (rootMotionTable && rootMotionTable->collisionDisable)
// 			{
// 				if (nullptr != characterActor)
// 				{
// 					characterActor->SetCollisionProfileName(TEXT("Pawn"));
// 				}
// 			}
// 		}
}

void FGsSkillHandlerBase::NetBuffSkillStart(PD::SC::PKT_SC_START_SKILL_READ* Packet, const FGsSchemaSkillSet* skillData)
{
}

void FGsSkillHandlerBase::NetActiveSkillEnd(PD::SC::PKT_SC_SKILL_FINISHED_READ* Packet, const FGsSchemaSkillSet* skillData)
{
	// 루트 모션 스킬인가 확인
	if (Packet->ReactionId() != 0)
	{
		UGsGameObjectCreature* castObject = _owner->CastGameObject<UGsGameObjectCreature>();
		castObject->EndRootmotion(Packet->NewDir(), Packet->NewPos(), Packet->ReactionId());

		if (AGsCharacterBase* characterActor = _owner->GetCharacter())
		{
			characterActor->SetDefaultCollisionProfile();
		}
	}
}

void FGsSkillHandlerBase::NetBuffSkillEnd(PD::SC::PKT_SC_SKILL_FINISHED_READ* Packet, const FGsSchemaSkillSet* skillData)
{
}

const FGsSchemaSkillSet* FGsSkillHandlerBase::NetSkillStart(PD::SC::PKT_SC_START_SKILL_READ* Packet)
{
	if (const FGsSchemaSkillSet* skillData = UGsTableUtil::FindRowById<UGsTableSkillSet, FGsSchemaSkillSet>(Packet->SkillId()))
	{
		// 버프 스킬은 처리하지 않음
		if (SharedFunc::GetSkillCategorySet(skillData->category) == SkillCategorySet::BUFF)
		{
			NetBuffSkillStart(Packet, skillData);
		}
		else
		{
			NetActiveSkillStart(Packet, skillData);
		}
		return skillData;
	}

	return nullptr;
}

const FGsSchemaSkillSet* FGsSkillHandlerBase::NetSkillEnd(PD::SC::PKT_SC_SKILL_FINISHED_READ* Packet)
{
	const FGsSchemaSkillSet* skillData = UGsTableUtil::FindRowById<UGsTableSkillSet, FGsSchemaSkillSet>(Packet->SkillId());
	if (skillData)
	{
		if (skillData->category == SkillCategory::BUFF)
		{
			NetBuffSkillEnd(Packet, skillData);
		}
		else
		{
			NetActiveSkillEnd(Packet, skillData);
		}		
	}

	return skillData;
}

void FGsSkillHandlerBase::NetSkillFailure(uint32 Id, PD::Result ErrResult)
{
	// SC_ACK_START_SKILL_FAILURE 패킷은 스킬을 시작하기도 전에만 내려주는 패킷이므로 (서버 확인 완료)
	// 클라이언트에서 현재 진행 중인 스킬에 대한 처리를 하면 안된다.
	// 현재 모든 클라이언트 스킬 연출은 서버 응답을 받은후 처리.

	// 루트 모션 스킬인 경우 처리
// 	if (const FGsSchemaSkillSet* skillData = UGsTableUtil::FindRowById<UGsTableSkillSet, FGsSchemaSkillSet>(Id))
// 	{
// // 		if (false == skillData->rootMotionId.IsNull())
// // 		{
// // 		}
// 		// 루트 모션 스킬인가 확인
// 		if (const FGsSchemaRootmotionSet* rootmotionSet = skillData->rootMotionId.GetRow())
// 		{
// 			if (FGsMovementHandlerBase* movementHandler = _owner->GetMovementHandler())
// 			{
// 				const FVector& dir = _owner->GetRotation().Vector();
// 				int16 angle = FGsDir::dirToAngle(dir);
// 
// 				movementHandler->EndRootmotion(angle, _owner->GetLocation(), rootmotionSet->id);
// 			}
// 		}
// 	}

	// 상태 회복 처리
	FGsGameObjectStateManager::ProcessEvent(_owner, EGsStateBase::AttackEnd);
}

const FGsSchemaSkillNotifySet* FGsSkillHandlerBase::NetSkillNotifyAction(PD::SC::PKT_SC_SKILL_EFFECT_READ* Packet)
{
	// Runner가 실행중이 아니거나 Skill 정보가 다르다면 타이밍 이슈가 발생된것
	if (false == _skillRunner->IsValid())
	{
		GSLOG(Error, TEXT("NetSkillNotifyAction not Sync"));
		return nullptr;
	}

	const FGsSchemaSkillSet* skillSetTable = _skillRunner->GetSkillData();
	if (skillSetTable && skillSetTable->id != Packet->SkillId())
	{
		GSLOG(Error, TEXT("NetSkillNotifyAction not Sync"));
		return nullptr;
	}

	const FGsSchemaSkillNotifySet* skillNoitifySet =
		UGsTableUtil::FindRowById<UGsTableSkillNotifySet, FGsSchemaSkillNotifySet>(Packet->SkillNotifyId());
	if (nullptr == skillNoitifySet)
	{
		return nullptr;
	}

	const FGsSchemaSkillNotifyElement* element = skillNoitifySet->notifyElementList[0].GetRow();
	if (nullptr == element)
	{
		return nullptr;
	}

	switch (skillNoitifySet->type)
	{
	case SkillNotifyType::BEAM_PARTICLE_EFFECT:
	{
		const FGsSchemaSkillNotifyBeamParticle* beamParticleEffect = element->typeInfo.GetRowByType<FGsSchemaSkillNotifyBeamParticle>();
		if (nullptr == beamParticleEffect)
		{
			return nullptr;
		}
		if (TSharedPtr<FGsSkillNotifyNodeBase> skillNotify = _skillRunner->CreateSkillNode(skillNoitifySet))
		{
			FGsSkillNotifyNodeBeamParticle* castNotify = static_cast<FGsSkillNotifyNodeBeamParticle*>(skillNotify.Get());
			castNotify->Initialize(_owner, skillNoitifySet, GetNetSkillData());
			castNotify->SetData(beamParticleEffect);
			castNotify->Action(_owner);
		}
	}
	break;
	case SkillNotifyType::SKILLGUIDE_EFFECT:
	{
		const FGsSchemaSkillSet* skillSet =
			UGsTableUtil::FindRowById<UGsTableSkillSet, FGsSchemaSkillSet>(Packet->SkillId());
		if (nullptr == skillSet)
		{
			return nullptr;
		}
		const FGsSchemaSkillNotifySkillGuide* skillGuideTable = element->typeInfo.GetRowByType<FGsSchemaSkillNotifySkillGuide>();
		if (nullptr == skillGuideTable)
		{
			return nullptr;
		}
		if (TSharedPtr<FGsSkillNotifyNodeBase> skillNotify =_skillRunner->CreateSkillNode(skillNoitifySet))
		{
			FGsSkillNotifyNodeGuideEffect* castNotify = static_cast<FGsSkillNotifyNodeGuideEffect*>(skillNotify.Get());
			castNotify->Initialize();
			castNotify->SetData(skillGuideTable, &skillSet->collisionInfoList.Last());
			castNotify->Action(_owner);
		}
	}
	break;
	}
	return skillNoitifySet;
}

void FGsSkillHandlerBase::StartCommonAction(CommonActionType Type)
{

}

void FGsSkillHandlerBase::EndCommonAction()
{
}

void FGsSkillHandlerBase::OnSocial(int Id)
{
	// 높은 우선순위의 social. 
	_currentSocial = nullptr;
	if (_listSocial.Num() > 0 && Id > 0)
	{
		const FGsSchemaSkillCommon** findData = _listSocial.FindByPredicate([Id](const FGsSchemaSkillCommon* el)
		{
			return el->id == Id;
		});

		if (findData)
		{
			_currentSocial = *findData;
		}
	}

	if (FGsGameObjectStateManager* fsm = _owner->GetBaseFSM())
	{
		fsm->ProcessEvent(EGsStateBase::SocialAction);
	}
}

void FGsSkillHandlerBase::StopSocial(bool IsSocialIdleEnd)
{
	if (IsSocialIdleEnd)
	{
		SetEmotionLock(false);

		if (FGsGameObjectStateManager* fsm = _owner->GetBaseFSM())
		{
			fsm->ProcessEvent(EGsStateBase::CommonActionEnd);
		}
	}

	_currentSocial = nullptr;
}

void FGsSkillHandlerBase::StartEmotionMotion(const FGsSchemaSkillCommon* motion)
{
	// 낮은 우선순위의 Idle motion
	if (FGsGameObjectStateManager* fsm = _owner->GetBaseFSM())
	{
		bool enableTransition = false;
		if (fsm->CurrentState())
		{
			enableTransition = fsm->CurrentState()->CanTransition(_owner, static_cast<int>(EGsStateBase::Social));
		}

		if (enableTransition)
		{
			_currentSocial = motion;
			fsm->ProcessEvent(EGsStateBase::Social);
		}
	}
}

void FGsSkillHandlerBase::CompulsionCancelSkill(int Id)
{
	StopSkill(Id);
}

void FGsSkillHandlerBase::CallbackEndRunner(int inSkillID, bool inIsEnd)
{
	if (FGsGameObjectStateManager* fsm = _owner->GetBaseFSM())
	{
		fsm->ProcessEvent(EGsStateBase::AttackEnd);
	}
}

bool FGsSkillHandlerBase::IsEndTimeIdleEmotionMotion()
{
	if (_idleEmotionRunner.IsValid())
		return _idleEmotionRunner->IsEmotionMotionFinished();
	return true;
}

void FGsSkillHandlerBase::PlaySocialEmotion(UserSocialEmotion emotion)
{
	if (_idleEmotionRunner.IsValid())
	{
		if (auto emotionData = GSChat()->GetSocialMotionByType(emotion).Pin())
		{
			_idleEmotionRunner->PlayEmotionMotion(emotion, emotionData->isLoopMotion());
		}
	}
}
