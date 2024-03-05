// Fill out your copyright notice in the Description page of Project Settings.

#include "GsStateNonPlayerAttack.h"
#include "GsStateNonPlayerHeader.h"
#include "ActorEx/GsCharacterBase.h"
#include "GameObject/Skill/Runner/GsSkillRunnerBase.h"
#include "GameObject/Skill/GsSkillHandlerBase.h"
#include "GameObject/ObjectClass/GsGameObjectNonPlayerBase.h"
#include "UTIL/GsDir.h"

uint8 FGsStateNonPlayerAttack::GetStateID() const
{
	return static_cast<uint8>(EGsStateBase::Attack);
}

FName FGsStateNonPlayerAttack::GetStateName() const
{
	return TEXT("StateNonPlayerAttack");
}

FName FGsStateNonPlayerAttack::GetBPStateName() const
{
	return NAME_None;
}

bool FGsStateNonPlayerAttack::CanTransition(UGsGameObjectBase* Owner, int StateID) const
{
	if (Super::CanTransition(Owner, StateID))
	{
		switch (static_cast<EGsStateBase>(StateID))
		{
		case EGsStateBase::Attack:
		case EGsStateBase::AttackEnd:
		case EGsStateBase::Dying:
		case EGsStateBase::Reaction:
		case EGsStateBase::Roar:
		case EGsStateBase::Social:
		case EGsStateBase::SocialAction:
		case EGsStateBase::ForwardWalk:
		case EGsStateBase::Run:
		case EGsStateBase::RunAway:
			return true;
		case EGsStateBase::Hit:
			return CanCancel(Owner);
		default:
			return false;
		}
	}
	return false;
}

void FGsStateNonPlayerAttack::ReEnter(UGsGameObjectBase* Owner)
{
	Super::ReEnter(Owner);

	Enter(Owner);
}

void FGsStateNonPlayerAttack::Enter(UGsGameObjectBase* Owner)
{
	Super::Enter(Owner);

	PlaySkill(Owner);
}

void FGsStateNonPlayerAttack::Exit(UGsGameObjectBase* Owner)
{
	Super::Exit(Owner);

	UGsGameObjectNonPlayerBase* npc = Owner->CastGameObject<UGsGameObjectNonPlayerBase>();
	if (nullptr == npc)
	{
		return;
	}

	if (UGsAnimInstanceState* anim = Owner->GetAnimInstance())
	{
		// 루트 모션 동기화
		if (anim->IsRootmotionAnimation)
		{
			anim->SetRootmotionAnimation(false);
		}
	}

	if (FGsSkillHandlerBase* skillhandler = npc->GetSkillHandler())
	{
		skillhandler->GetSkillRunner()->EndRunner();
		if (AGsCharacterBase* character = npc->GetCharacter())
		{
			if (UGsAnimInstanceState* anim = character->GetAnim())
			{
				anim->StopMontagePlay();
			}
		}
	}
	
	
}

void FGsStateNonPlayerAttack::PlaySkill(UGsGameObjectBase* Owner)
{
	UGsGameObjectNonPlayerBase* npc = Owner->CastGameObject<UGsGameObjectNonPlayerBase>();
	if (nullptr == npc)
	{
		return;		
	}

	if (FGsSkillHandlerBase* skillhandler = npc->GetSkillHandler())
	{
		if (const FGsSkillNetData* netSkillData = skillhandler->GetNetSkillData())
		{
			FGsSkillRunnerBase* runner = skillhandler->GetSkillRunner();
			const FGsSchemaSkillSet* skillTableData =
				UGsTableUtil::FindRowById<UGsTableSkillSet, FGsSchemaSkillSet>(netSkillData->_skillId);

			if (nullptr == skillTableData)
			{
				return;
			}

			// 회전 처리
			FVector dir = FGsDir::angleToDir(netSkillData->_curDir);
			Owner->LookAt(dir.Rotation(), skillTableData->smoothRotaion);

			if (UAnimMontage* resAni = runner->GetAni())
			{
				if (AGsCharacterBase* characterActor = npc->GetCharacter())
				{
					if (skillTableData->category == SkillCategory::COMBO)
					{
						characterActor->PlayAnimMontage(resAni, runner->GetSpeedScale(), skillTableData->sequenceInfo.comboInfo.sectionName);
					}
					else
					{
						characterActor->PlayAnimMontage(resAni, runner->GetSpeedScale());
					}
				}
			}
			else
			{
				GSLOG(Warning, TEXT("Animation Resource is null"));
				FGsGameObjectStateManager::ProcessEvent(Owner, EGsStateBase::AttackEnd);
			}
		}
	}
	else
	{
		FGsGameObjectStateManager::ProcessEvent(Owner, EGsStateBase::AttackEnd);
	}
}

bool FGsStateNonPlayerAttack::CanCancel(UGsGameObjectBase* Owner) const
{
	if (FGsSkillHandlerBase* skillhandler = Owner->GetSkillHandler())
	{
		if (FGsSkillRunnerBase* skillRunner = skillhandler->GetSkillRunner())
		{
			return skillRunner->CanCancel();
		}
	}
	return true;
}