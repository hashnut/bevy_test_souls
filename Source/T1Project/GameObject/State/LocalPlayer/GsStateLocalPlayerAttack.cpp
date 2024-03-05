// Fill out your copyright notice in the Description page of Project Settings.

#include "GsStateLocalPlayerAttack.h"
#include "GsStateLocalPlayerHeader.h"
#include "Animation/GsAnimInstanceState.h"
#include "ControllerEx/GsPlayerController.h"
#include "ActorEx/GsCharacterBase.h"
#include "ActorComponentEx/GsHommingProjectileComponent.h"

#include "../Net/GsNetSendServiceWorld.h"
#include "GameObject/Target/GsTargetHandlerLocalPlayer.h"
#include "GameObject/Movement/GsMovementHandlerLocalPlayer.h"
#include "GameObject/Skill/GsSkillHandlerLocalPlayer.h"
#include "GameObject/Skill/Runner/GsSkillRunnerLocalPlayer.h"
#include "GameObject/Camera/GsCameraModeHandler.h"
#include "GameObject/ObjectClass/GsGameObjectLocalPlayer.h"
#include "GameObject/UseItemEffect/GsUseItemEffectHandler.h"
#include "GameObject/Data/GsGameObjectDataPlayer.h"
#include "GameObject/State/GsGameObjectStateManager.h"

#include "Util/GsDir.h"
#include "Util/GsLevelUtil.h"

#include "../Skill/GsSkill.h"

#include "Management/ScopeGame/GsCameraModeManager.h"
#include "Management/ScopeGame/GsAIManager.h"
#include "Management/ScopeGame/GsGameObjectManager.h"
#include "Management/ScopeGlobal/GsSkillManager.h"

#include "T1Project.h"


uint8 FGsStateLocalPlayerAttack::GetStateID() const
{
	return static_cast<uint8>(EGsStateBase::Attack);
}

FName FGsStateLocalPlayerAttack::GetStateName() const
{
	return TEXT("StateLocalPlayerAttack");
}

FName FGsStateLocalPlayerAttack::GetBPStateName() const
{
	return NAME_None;
}

bool FGsStateLocalPlayerAttack::CanTransition(UGsGameObjectBase* Owner, int StateID) const
{
	if (Super::CanTransition(Owner, StateID))
	{
		switch (static_cast<EGsStateBase>(StateID))
		{
		GS_STATE_GROUP_ABNORLITY:
		GS_STATE_GROUP_DIE:
		case EGsStateBase::Attack:
		case EGsStateBase::Casting:
		case EGsStateBase::Warp:		
		case EGsStateBase::AttackEnd:
		case EGsStateBase::ChangeWeapon:
			return true;
		GS_STATE_GROUP_MOVE:		
			return CanCancel(Owner);
		default:
			return false;
		}
	}
	return false;
}

void FGsStateLocalPlayerAttack::ReEnter(UGsGameObjectBase* Owner)
{
	Super::ReEnter(Owner);
	Enter(Owner);
}

void FGsStateLocalPlayerAttack::Enter(UGsGameObjectBase* Owner)
{
	//GSLOG(Log , TEXT("FGsStateLocalPlayerAttack::Enter ~~~~~~~!!!!!!!!!!!!!"));
	Super::Enter(Owner);	
	PlaySkill(Owner);
}

void FGsStateLocalPlayerAttack::Update(UGsGameObjectBase* Owner, float Delta)
{
	Super::Update(Owner, Delta);

	// 이동 스킬일 경우 이동 Update 활성
	FGsSkillHandlerBase* skill = Owner->GetSkillHandler();
	FGsSkillRunnerBase* skillRunner = skill->GetSkillRunner();
	if (skillRunner->CanMove())
	{
		if (FGsMovementHandlerBase* movement = Owner->GetMovementHandler())
		{
			FGsMovementDirtyMark mark(movement);
			movement->Update(Delta);
		}
	}

	bool isAutoMode = false;
	if (FGsAIManager* aiMgr = GSAI())
	{
		isAutoMode = aiMgr->IsAIOn();
	}

	// 카메라 모드 자동 공격 일때 회전 갱신	
	UGsGameObjectLocalPlayer* local = Owner->CastGameObject<UGsGameObjectLocalPlayer>();
	if (FGsCameraModeHandler* cameraModehandler = local->GetCameraModeHandler())
	{
		cameraModehandler->GetState()->UpdateAttack(isAutoMode);
	}
}

void FGsStateLocalPlayerAttack::Exit(UGsGameObjectBase* Owner)
{
	Super::Exit(Owner);

	UGsGameObjectLocalPlayer* local = Owner->CastGameObject<UGsGameObjectLocalPlayer>();
	FGsMovementHandlerLocalPlayer* movement = local->GetCastMovement<FGsMovementHandlerLocalPlayer>();
	if (nullptr == movement)
	{
		return;
	}

	// 이동 가능 스킬 확인 및 설정
	if (false == local->IsCrowdControlLock(CreatureActionType::CREATURE_ACTION_TYPE_MOVE_DEFAULT))
	{
		movement->SetMoveable(true);
	}
	movement->SetMoveState(false);

	UGsAnimInstanceState* anim = local->GetAnimInstance();
	if (nullptr == anim)
	{
		return;
	}

	FGsSkillHandlerLocalPlayer* skillHandler = local->GetCastSkill<FGsSkillHandlerLocalPlayer>();
	FGsSkillRunnerBase* skillRunner = skillHandler->GetSkillRunner();

	// 루트 모션 동기화
	if (anim->IsRootmotionAnimation)
	{
		local->SyncRootmotionPosition();
	}

	// 정상정으로 스킬 사용을 종료 하지 못하였을때 보낸다.
	// if (false == skillRunner->CanCancel())
	// {
	//	FGsNetSendServiceWorld::SendCancelSkill();
	// }

	// 캔슬일 경우 블렌딩 타임 설정
	// anim->SetMontageBlendOutTime(EGsAnimationBlendType::Default);

	skillRunner->EndRunner();
}

bool FGsStateLocalPlayerAttack::CanCancel(UGsGameObjectBase* Owner) const
{
	UGsGameObjectLocalPlayer* local = Owner->CastGameObject<UGsGameObjectLocalPlayer>();
	if (FGsSkillHandlerLocalPlayer* skillHandler = local->GetCastSkill<FGsSkillHandlerLocalPlayer>())
	{
		// 캐스팅 확인은 생략
// 		if (skillHandler->IsCasting())
// 		{
// 			return false;
// 		}

		if (FGsSkillRunnerBase* skillRunner = skillHandler->GetSkillRunner())
		{
			return skillRunner->CanCancel();
		}
	}
	
	return true;
}

bool FGsStateLocalPlayerAttack::IsNoTargetingSkill(const FGsSchemaSkillSet* SkillData) const
{
	if (nullptr == SkillData)
	{
		return false;
	}

	if (SkillData->category == SkillCategory::CAST)
	{
		return false;
	}

	return true;
}

bool FGsStateLocalPlayerAttack::IsInsideCastingRange(UGsGameObjectBase* Owner, const FGsSchemaSkillSet* SkillData) const
{
	FGsTargetHandlerBase* targetMgr = Owner->GetTargetHandler();
	if (UGsGameObjectBase* target = targetMgr->GetTarget())
	{
		float distanceSq = FVector::DistSquared2D(target->GetLocation(), Owner->GetLocation());
		float targetR = target->GetData()->GetScaledCapsuleRadius();

		UGsGameObjectLocalPlayer* localObject = Owner->CastGameObject<UGsGameObjectLocalPlayer>();
		float skillAttackRange = FGsSkillHandlerBase::GetSkillApproachRange(localObject, SkillData);
		if (distanceSq <= skillAttackRange * skillAttackRange)
		{
			return true;
		}
	}
	return false;
}

bool FGsStateLocalPlayerAttack::TargetCastingProcess(UGsGameObjectBase* Owner, UGsGameObjectBase* Target,
	FGsSkillHandlerLocalPlayer* Skillhandler, const FGsSchemaSkillSet* SkillData)
{
	// 캐스팅 스킬의 경우 캐스팅 중인가 확인
	if (SkillData->category == SkillCategory::CAST)
	{
		// 캐스팅 준비 이전 상황이면
		if (false == Skillhandler->IsCasting())
		{
			if (false == Skillhandler->IsInsideSkillRange(SkillData))
			{
				UGsGameObjectLocalPlayer* localObject = Owner->CastGameObject<UGsGameObjectLocalPlayer>();
				float approachRange = FGsSkillHandlerBase::GetSkillApproachRange(localObject, SkillData);
				Skillhandler->OnMoveToSkill(Target, approachRange);
				return false;
			}
			else
			{
				// 가능 거리일 경우 대상방향으로 회전
				Owner->LookAtTarget(Target, true);
				// 캐스팅 상태 전환후 바로 빠져나간다.
				FGsGameObjectStateManager::ProcessEvent(Owner, EGsStateBase::Casting);
				return false;
			}
		}
		// 캐스팅 완료된 상황이 아니면
		else if (false == Skillhandler->IsCastingComplete())
		{
			return false;
		}
	}
	return true;
}

bool FGsStateLocalPlayerAttack::SelfCastingProcess(UGsGameObjectBase* Owner, FGsSkillHandlerLocalPlayer* Skillhandler,
	const FGsSchemaSkillSet* SkillData)
{
	// 캐스팅 스킬의 경우 캐스팅 중인가 확인
	if (SkillData->category == SkillCategory::CAST)
	{
		if (false == Skillhandler->IsCasting())
		{
			// 캐스팅 상태 전환
			FGsGameObjectStateManager::ProcessEvent(Owner, EGsStateBase::Casting);
			return false;
		}
		// 캐스팅 완료된 상황이 아니면
		else if (false == Skillhandler->IsCastingComplete())
		{
			return false;
		}
	}
	return true;
}

void FGsStateLocalPlayerAttack::PlaySkill(UGsGameObjectBase* Owner)
{
	UGsGameObjectLocalPlayer* local	= Owner->CastGameObject<UGsGameObjectLocalPlayer>();
	AGsCharacterBase* characterActor = Owner->GetCharacter();
	FGsSkillHandlerLocalPlayer* skillhandler = local->GetCastSkill<FGsSkillHandlerLocalPlayer>();

	const FGsSkillNetData* netSkillData = skillhandler->GetNetSkillData();
	if (nullptr == netSkillData)
	{
		FGsGameObjectStateManager::ProcessEvent(Owner, EGsStateBase::AttackEnd);
		return;
	}
	const FGsSchemaSkillSet* skillData =
		UGsTableUtil::FindRowById<UGsTableSkillSet, FGsSchemaSkillSet>(netSkillData->_skillId);
	if (nullptr == skillData)
	{
		FGsGameObjectStateManager::ProcessEvent(Owner, EGsStateBase::AttackEnd);
		return;
	}

	FGsSkillRunnerBase* skillRunner = skillhandler->GetSkillRunner();	

	if (UAnimMontage* resAni = skillRunner->GetAni())
	{
		UGsAnimInstanceState* anim = local->GetAnimInstance();
		anim->SetPhysicsAnimation(skillData->physicsAnimation);

		EGsAnimSlotType animSlotType = EGsAnimSlotType::DefaultSlot;

		// 콤보 스킬 인가 확인
		if (skillData->category == SkillCategory::COMBO)
		{
			// 슬롯 갯수 확인
			if (resAni->SlotAnimTracks.Num() >= (int)EGsAnimSlotType::Max)
			{
				UGsHommingProjectileComponent* hommingComponent = characterActor->GetHommingProjectile();
				// 크리티컬 유무를 확인
				bool isCritical = false;
				for (TPair< AGsCharacterBase*, HitResultType> pair : hommingComponent->_mapHomingProjectileInfo)
				{
					if (pair.Value == HitResultType::CRITICAL)
					{
						isCritical = true;
						break;
					}
				}
				// 슬롯 랜덤 애니메이션 출력 처리
				animSlotType = FGsSkillHandlerBase::GetRandomCombSkillSlot(isCritical,
					skillData->sequenceInfo.comboInfo.currentStage);
			}

			characterActor->PlayAnimMontage(resAni, skillRunner->GetSpeedScale(), skillData->sequenceInfo.comboInfo.sectionName);
		}
		// 캐스팅 스킬인가 확인
		else if (skillData->category == SkillCategory::CAST)
		{
			FName playSectionName = skillData->castInfo.sectionName;
			anim->PlayAnimMontageJumpToSection(resAni, skillRunner->GetSpeedScale(), playSectionName, false);
			// 하나의 스킬 이면서 색션 점프 구간 이었다면 Runner 타이머 동기화
			if (playSectionName.IsValid())
			{
				float startSectionTime = 0.f;
				float endSectionTime = 0.f;
				int sectionIdx = resAni->GetSectionIndex(playSectionName);
				resAni->GetSectionStartAndEndTime(sectionIdx, startSectionTime, endSectionTime);
				skillRunner->JumpToRunner(Owner, startSectionTime);
			}
		}
		else
		{
			float len = characterActor->PlayAnimMontage(resAni, skillRunner->GetSpeedScale());
		}

		anim->SetSlotMontagePlay(animSlotType);
	}

	// 이동 가능 스킬 확인 및 설정
	if (false == Owner->IsCrowdControlLock(CreatureActionType::CREATURE_ACTION_TYPE_MOVE_DEFAULT))
	{
		if (FGsMovementHandlerLocalPlayer* movement = local->GetCastMovement<FGsMovementHandlerLocalPlayer>())
		{
			movement->SetMoveable(skillRunner->CanMove());
			movement->SetMoveState(skillRunner->CanMove());
		}
	}

	// 천마석(정령탄) 연출(때리는 연출)
	if (FGsGameObjectDataPlayer* data = local->GetCastData<FGsGameObjectDataPlayer>())
	{
		// 천마석 사용하는가
		// 천마석 연출보여주는 스킬인가
		if (data->GetIsActiveSpiritShot() == true &&
			skillData->spiritShotEffectShow == true)
		{
			if (FGsUseItemEffectHandler* useItemEffectHandler = local->GetUseItemEffectHandler())
			{
				useItemEffectHandler->OnUseItemEffect(ItemCategorySub::SPIRIT_SHOT, data->GetCurrentWeaponType(), data->GetSpiritShotSumLevel());
			}
		}
	}
}