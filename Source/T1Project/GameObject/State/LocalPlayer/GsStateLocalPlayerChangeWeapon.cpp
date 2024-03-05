// Fill out your copyright notice in the Description page of Project Settings.


#include "GsStateLocalPlayerChangeWeapon.h"
#include "DataSchema/GameObject/GsSchemaPlayerShape.h"
#include "DataSchema/GameObject/GsSchemaPlayerWeaponDetailInfo.h"
#include "Animation/GsAnimInstanceState.h"
#include "Management/ScopeGlobal/GsSkillManager.h"
#include "../GsGameObjectStateManager.h"
#include "GameObject/Skill/GsSkillHandlerLocalPlayer.h"
#include "GameObject/Skill/Runner/GsSkillRunnerBase.h"
#include "GameObject/Quest/GsQuestHandler.h"
#include "../../Skill/GsSkillHandlerBase.h"
#include "../../ObjectClass/GsGameObjectLocalPlayer.h"
#include "Animation/GsAnimInstanceState.h"


uint8 FGsStateLocalPlayerChangeWeapon::GetStateID() const
{
	return static_cast<uint8>(EGsStateBase::ChangeWeapon);
}
FName FGsStateLocalPlayerChangeWeapon::GetStateName() const
{
	return TEXT("StateLocalPlayerChangeWeapon");
}
FName FGsStateLocalPlayerChangeWeapon::GetBPStateName() const
{
	return NAME_None;
}

bool FGsStateLocalPlayerChangeWeapon::CanTransition(UGsGameObjectBase* Owner, int StateID) const
{
	if (Super::CanTransition(Owner, StateID))
	{
		if (Super::CanTransition(Owner, StateID))
		{
			switch (static_cast<EGsStateBase>(StateID))
			{
			GS_STATE_GROUP_ABNORLITY:
			GS_STATE_GROUP_DIE:
			case EGsStateBase::Warp:
			case EGsStateBase::ChangeWeaponEnd:
				return true;
			default:
				return false;
			}
		}
	}
	return false;
}

void FGsStateLocalPlayerChangeWeapon::Enter(UGsGameObjectBase* Owner)
{
	Super::Enter(Owner);

	if (FGsGameObjectStateManager* fsm = Owner->GetBaseFSM())
	{
		FGsSkillHandlerBase* skillhandler = Owner->GetSkillHandler();
		FGsSkillRunnerBase* skillRunner = skillhandler->GetSkillRunner();

		//cnr712 : 퀘스트 진행중 무기교체가 일어나면 엑티브퀘스트는 계속 진행
		// https://jira.com2us.com/jira/browse/CHR-10767
		// 자동사냥 중, 장비 교체 시 기존 공격을 취소하고 타겟 재설정 하는듯
		// -> FGsAIQuestHandler 에서 retry 하므로 빼겠습니다		
		//FGsQuestHandler* questHandler = Owner->CastGameObject<UGsGameObjectLocalPlayer>()->GetQuestHandler();
		//questHandler->SetAutoMoveStart();

		if (const FGsSchemaSkillCommon* commonSkill = GSkill()->GetPlayerCommonActionData(Owner,
			CommonActionType::WEAPON_CHANGE))
		{
			skillRunner->StartRunner(Owner, commonSkill);

			UGsAnimInstanceState* anim = Owner->GetAnimInstance();
			anim->SetPhysicsAnimation(commonSkill->physicsAnimation);

			if (UAnimMontage* resAni = skillRunner->GetAni())
			{
				float len = anim->Montage_Play(resAni);
			}
		}
	}
}

void FGsStateLocalPlayerChangeWeapon::Exit(UGsGameObjectBase* Owner)
{
	Super::Exit(Owner);
}
