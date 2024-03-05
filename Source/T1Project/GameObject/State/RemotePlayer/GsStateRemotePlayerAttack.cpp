// Fill out your copyright notice in the Description page of Project Settings.


#include "GsStateRemotePlayerAttack.h"
#include "GsStateRemotePlayerHeader.h"
#include "Animation/AnimMontage.h"
#include "Animation/GsAnimInstanceState.h"
#include "ActorEx/GsCharacterBase.h"
#include "ActorComponentEx/GsHommingProjectileComponent.h"

#include "../../../Data/GsResourceManager.h"
#include "DataSchema/Skill/GsSchemaSkillSet.h"

#include "../Management/ScopeGlobal/GsSkillManager.h"

#include "GameObject/ObjectClass/GsGameObjectRemotePlayer.h"
#include "GameObject/Skill/GsSkillHandlerRemotePlayer.h"
#include "GameObject/Skill/Runner/GsSkillRunnerCreature.h"
#include "GameObject/UseItemEffect/GsUseItemEffectHandler.h"
#include "GameObject/Data/GsGameObjectDataPlayer.h"


uint8 FGsStateRemotePlayerAttack::GetStateID() const
{
	return static_cast<uint8>(EGsStateBase::Attack);
}

FName FGsStateRemotePlayerAttack::GetStateName() const
{
	return TEXT("GsStateRemotePlayerAttack");
}

FName FGsStateRemotePlayerAttack::GetBPStateName() const
{
	return NAME_None;
}

bool FGsStateRemotePlayerAttack::CanTransition(UGsGameObjectBase* Owner, int StateID) const
{
	if (Super::CanTransition(Owner, StateID))
	{
		switch (static_cast<EGsStateBase>(StateID))
		{
		case EGsStateBase::Attack:
		case EGsStateBase::AttackEnd:
		case EGsStateBase::Dying:
		case EGsStateBase::Dead:
		case EGsStateBase::Reaction:
		case EGsStateBase::ChangeWeapon:
		case EGsStateBase::Warp:
			return true;
		case EGsStateBase::ForwardWalk:
		case EGsStateBase::Run:
			return true;
		default:
			return false;
		}
	}
	return false;
}

void FGsStateRemotePlayerAttack::ReEnter(UGsGameObjectBase* Owner)
{
	Super::ReEnter(Owner);

	PlaySkill(Owner);
}

void FGsStateRemotePlayerAttack::Enter(UGsGameObjectBase* Owner)
{
	//GSLOG(Log , TEXT("FGsStateRemotePlayerAttack::Enter ~~~~~~~!!!!!!!!!!!!!"));
	Super::Enter(Owner);

	PlaySkill(Owner);
}

void FGsStateRemotePlayerAttack::Exit(UGsGameObjectBase* Owner)
{
	Super::Exit(Owner);

	AGsCharacterBase* character = Owner->GetCharacter();
	if (nullptr == character)
	{
		return;
	}

	UGsAnimInstanceState* anim = character->GetAnim();
	if (nullptr == anim)
	{
		return;
	}

	// 루트 모션 동기화
	if (anim->IsRootmotionAnimation)
	{
		anim->SetRootmotionAnimation(false);
	}

	FGsSkillHandlerBase* skillHandler = Owner->GetSkillHandler();
	if (nullptr == skillHandler)
	{
		return;
	}

	FGsSkillRunnerBase* skillRunner	= skillHandler->GetSkillRunner();
	if (nullptr == skillRunner)
	{
		return;
	}
	
	skillRunner->EndRunner();	
}

bool FGsStateRemotePlayerAttack::CanMove(UGsGameObjectBase* Owner) const
{
	UGsGameObjectRemotePlayer* remote	= Owner->CastGameObject<UGsGameObjectRemotePlayer>();
	FGsSkillHandlerBase* skillhandler = remote->GetSkillHandler();
	if (nullptr != skillhandler)
	{
		return skillhandler->GetSkillRunner()->CanCancel();;
	}
	else
	{
		GSLOG(Error, TEXT("<< FGsStateRemotePlayerAttack::IsMoveble =>Skill Handler DoesNotExist !!"));
	}
	return false;
}

void FGsStateRemotePlayerAttack::PlaySkill(UGsGameObjectBase* Owner)
{
	check(Owner);

	UGsGameObjectCreature* remote = Owner->CastGameObject<UGsGameObjectCreature>();
	FGsSkillHandlerBase* skillhandler = remote->GetSkillHandler();
	if (nullptr == skillhandler)
	{
		GSLOG(Error, TEXT("PlaySkill FGsSkillHandlerRemotePlayer not found"));
		FGsGameObjectStateManager::ProcessEvent(Owner, EGsStateBase::AttackEnd);
		return;
	}

	const FGsSkillNetData* netSkillData = skillhandler->GetNetSkillData();
	if (nullptr == netSkillData)
	{
		GSLOG(Error, TEXT("PlaySkill GetNetSkillData not found"));
		FGsGameObjectStateManager::ProcessEvent(Owner, EGsStateBase::AttackEnd);
		return;
	}

	const FGsSchemaSkillSet* skillTableData =
		UGsTableUtil::FindRowById<UGsTableSkillSet, FGsSchemaSkillSet>(netSkillData->_skillId);
	if (nullptr == skillTableData)
	{
		GSLOG(Error, TEXT("PlaySkill FGsSchemaSkillSet not found"));
		FGsGameObjectStateManager::ProcessEvent(Owner, EGsStateBase::AttackEnd);
		return;
	}

	FGsSkillRunnerBase* skillRunner = skillhandler->GetSkillRunner();
	if (nullptr == skillRunner)
	{
		GSLOG(Error, TEXT("PlaySkill FGsSkillRunnerBase not found"));
		FGsGameObjectStateManager::ProcessEvent(Owner, EGsStateBase::AttackEnd);
		return;
	}

	AGsCharacterBase* characterActor = Owner->GetCharacter();
	if (nullptr == characterActor)
	{
		GSLOG(Error, TEXT("PlaySkill AGsCharacterBase is nullptr"));
		FGsGameObjectStateManager::ProcessEvent(Owner, EGsStateBase::AttackEnd);
		return;
	}
	
	if (UAnimMontage* resAni = skillRunner->GetAni())
	{
		UGsAnimInstanceState* anim = remote->GetAnimInstance();
		anim->SetPhysicsAnimation(skillTableData->physicsAnimation);

		EGsAnimSlotType animSlotType = EGsAnimSlotType::DefaultSlot;

		// 스킬 미리보기에서도 사용되므로 ObjectType을 한번 검사한다.
		if (Owner->IsObjectType(EGsGameObjectType::Player))
		{
			// 천마석(정령탄) 연출(때린넘 연출)
			FGsGameObjectDataPlayer* data = remote->GetCastData<FGsGameObjectDataPlayer>();
			if (nullptr != data)
			{
				// 천마석 사용하는가 // 천마석 연출보여주는 스킬인가
				if (data->GetIsActiveSpiritShot() == true && skillTableData->spiritShotEffectShow == true)
				{
					FGsUseItemEffectHandler* useItemEffectHandler = remote->GetUseItemEffectHandler();
					if (nullptr != useItemEffectHandler)
					{
						useItemEffectHandler->OnUseItemEffect(ItemCategorySub::SPIRIT_SHOT, data->GetCurrentWeaponType(), data->GetSpiritShotSumLevel());
					}
				}
			}
		}

		FName playSectionName = NAME_None;
		// 콤보 스킬 인가 확인
		if (skillTableData->category == SkillCategory::COMBO)
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
					skillTableData->sequenceInfo.comboInfo.currentStage);
			}

			playSectionName = skillTableData->sequenceInfo.comboInfo.sectionName;
			characterActor->PlayAnimMontage(resAni, skillRunner->GetSpeedScale(), playSectionName);
		}
		// 캐스팅 스킬인가 확인
		else if (skillTableData->category == SkillCategory::CAST)
		{
			playSectionName = skillTableData->castInfo.sectionName;
			characterActor->PlayAnimMontage(resAni, skillRunner->GetSpeedScale(), playSectionName);
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
			characterActor->PlayAnimMontage(resAni, skillRunner->GetSpeedScale());
		}

		anim->SetSlotMontagePlay(animSlotType);
	}
	else
	{
		GSLOG(Error, TEXT("Animation Resource is null"));
		FGsGameObjectStateManager::ProcessEvent(Owner, EGsStateBase::AttackEnd);
	}
	
}