// Fill out your copyright notice in the Description page of Project Settings.


#include "GsStateLocalPlayerInteractionEnd.h"
#include "GsStateLocalPlayerHeader.h"
#include "ActorEx/GsCharacterBase.h"
#include "Animation/GsAnimInstanceState.h"
#include "Management/ScopeGlobal/GsSkillManager.h"
#include "Data/GsResourceManager.h"
#include "DataSchema/Skill/GsSchemaSkillCommon.h"
#include "DataSchema/GameObject/Prop/GsSchemaPropData.h"
#include "GameObject/ObjectClass/GsGameObjectLocalPlayer.h"
#include "GameObject/Interaction/GsInteractionHandlerLocalPlayer.h"
#include "GameObject/ObjectClass/GsGameObjectBase.h"
#include "GameObject/ObjectClass/GsGameObjectPropInteraction.h"
#include "Sound/GsSoundPlayer.h"
#include "Management/ScopeGlobal/GsSoundManager.h"

uint8 FGsStateLocalPlayerInteractionEnd::GetStateID() const
{
	return static_cast<uint8>(EGsStateBase::InteractionEnd);
}
FName FGsStateLocalPlayerInteractionEnd::GetStateName() const
{
	return TEXT("GsStateLocalPlayerInteractionEnd");
}
FName FGsStateLocalPlayerInteractionEnd::GetBPStateName() const
{
	return NAME_None;
}

bool FGsStateLocalPlayerInteractionEnd::CanTransition(UGsGameObjectBase* Owner, int StateID) const
{
	if (Super::CanTransition(Owner, StateID))
	{
		switch (static_cast<EGsStateBase>(StateID))
		{
		GS_STATE_GROUP_MOVE:		
		GS_STATE_GROUP_ACTION:
		GS_STATE_GROUP_ABNORLITY:
		GS_STATE_GROUP_DIE:
		case EGsStateBase::Idle:
		case EGsStateBase::Warp:
		case EGsStateBase::ChangeWeapon:
			return true;
		default:
			return false;
		}
	}
	return false;
}
void FGsStateLocalPlayerInteractionEnd::Enter(UGsGameObjectBase* Owner)
{
	Super::Enter(Owner);

	UGsGameObjectLocalPlayer* local = Owner->CastGameObject<UGsGameObjectLocalPlayer>();
	FGsInteractionHandlerLocalPlayer* interactionHandler = local->GetCastInteraction<FGsInteractionHandlerLocalPlayer>();
	FGsGameObjectStateManager* fsm = local->GetBaseFSM();

	bool immediate = true;

	// 이단계에선 예약정보는 참조할수 없다.(Clear 상태)
	if (UGsGameObjectBase* interationTarget = interactionHandler->GetInteractionTarget())
	{
		if (interationTarget->IsObjectType(EGsGameObjectType::PropInteraction))
		{
			if (UGsGameObjectPropInteraction* propObject = interationTarget->CastGameObject<UGsGameObjectPropInteraction>())
			{
				CommonActionType actionType = CommonActionType::TOUCHPROP;
				const FGsSchemaPropData* propTable = propObject->GetPropTableData();

				if (const FGsSchemaSkillCommon* commonSkill = GSkill()->GetPlayerCommonActionData(Owner,
					actionType))
				{
					UGsAnimInstanceState* anim = Owner->GetAnimInstance();
					anim->SetPhysicsAnimation(commonSkill->physicsAnimation);
					// 타임라인 개념이 모호하기 때문에 SkillRunner를 타지 않는다.

					//FGsSkillHandlerBase* skillhandler = _owner->GetSkillHandler();			
					//FGsSkillRunnerBase* skillRunner = skillhandler->GetSkillRunner();
					//skillRunner->StartRunner(Owner, commonSkill);
					if (UAnimMontage* res = GResource()->LoadSync<UAnimMontage>(commonSkill->aniPath))
					{
						AGsCharacterBase* caracterActor = Owner->GetCharacter();
						float len = caracterActor->PlayAnimMontage(res, 1.f, TEXT("End"));

						fsm->DelayProcessEvent(len * 0.8f, EGsStateBase::InteractionEnd, EGsStateBase::Idle);
						immediate = false;
					}
				}

				UGsSoundPlayer* soundPlayer = GSoundPlayer();
				if (soundPlayer)
				{
					soundPlayer->PlaySound2D("UI_Interaction_CompleteTicker");
				}
			}
		}
	}

	if (immediate)
	{
		fsm->ProcessEvent(EGsStateBase::Idle);
	}
}

void FGsStateLocalPlayerInteractionEnd::Exit(UGsGameObjectBase* Owner)
{
	Super::Exit(Owner);
}
