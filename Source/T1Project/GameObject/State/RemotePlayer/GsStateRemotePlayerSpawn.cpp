#include "GsStateRemotePlayerSpawn.h"
#include "GsStateRemotePlayerHeader.h"

#include "ActorEx/GsCharacterBase.h"

#include "Animation/GsAnimInstanceState.h"
#include "Management/ScopeGlobal/GsSkillManager.h"
#include "GameObject/Skill/GsSkillHandlerRemotePlayer.h"
#include "GameObject/Skill/Runner/GsSkillRunnerCreature.h"
#include "../../ObjectClass/GsGameObjectRemotePlayer.h"
#include "../../ObjectClass/GsGameObjectBase.h"
#include "../../ObjectClass/GsGameObjectCreature.h"

uint8 FGsStateRemotePlayerSpawn::GetStateID() const
{
	return static_cast<uint8>(EGsStateBase::Spawn);
}

FName FGsStateRemotePlayerSpawn::GetStateName() const
{
	return TEXT("StateLocalPlayerSpawn");
}

FName FGsStateRemotePlayerSpawn::GetBPStateName() const
{
	return TEXT("Spawn");
}

bool FGsStateRemotePlayerSpawn::CanTransition(UGsGameObjectBase* Owner, int StateID) const
{
	if (Super::CanTransition(Owner, StateID))
	{
		switch (static_cast<EGsStateBase>(StateID))
		{
		case EGsStateBase::Idle:
		case EGsStateBase::Dying:
		case EGsStateBase::Dead:
		case EGsStateBase::Freeze:
		case EGsStateBase::Run:
		case EGsStateBase::ChangeWeapon:
			return true;
		default:
			return false;
		}
	}
	return false;
}

void FGsStateRemotePlayerSpawn::Enter(UGsGameObjectBase* Owner)
{
	Super::Enter(Owner);

	UGsGameObjectRemotePlayer* remote = Owner->CastGameObject<UGsGameObjectRemotePlayer>();
	UGsAnimInstanceState* anim = remote->GetCharacter()->GetAnim();
	if (FGsSkillHandlerBase* skill = remote->GetSkillHandler())
	{

		FGsSkillRunnerBase* skillRunner = skill->GetSkillRunner();
		if (const FGsSchemaSkillCommon* commonData = GSkill()->GetPlayerCommonActionData(Owner, CommonActionType::SPAWN))
		{
			skillRunner->StartRunner(Owner, commonData);

			anim->SetPhysicsAnimation(commonData->physicsAnimation);
		}

		if (FGsGameObjectStateManager* fsm = remote->GetBaseFSM())
		{
			if (UAnimMontage* resAni = skillRunner->GetAni())
			{
				if (anim)
				{
					float len = anim->Montage_Play(resAni);
					fsm->DelayProcessEvent(len * 0.8f, EGsStateBase::Spawn, EGsStateBase::Idle);
				}			
			}
			else
			{
				fsm->ProcessEvent(EGsStateBase::Idle);
			}
		}
	}
}

void FGsStateRemotePlayerSpawn::Exit(UGsGameObjectBase* Owner)
{
	Super::Exit(Owner);
}