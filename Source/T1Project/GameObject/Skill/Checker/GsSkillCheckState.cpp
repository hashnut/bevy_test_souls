// Fill out your copyright notice in the Description page of Project Settings.


#include "GsSkillCheckState.h"

#include "Skill/GsSkill.h"

#include "GameObject/State/GsStateBase.h"
#include "GameObject/State/GsGameObjectStateManager.h"
#include "GameObject/Movement/GsMovementHandlerBase.h"
#include "GameObject/ObjectClass/GsGameObjectBase.h"
#include "Management/ScopeGame/GsGameObjectManager.h"
#include "Sequence/GsSequenceManager.h"

EGsSkillCheckResultType FGsSkillCheckState::GetErrType() const
{
	return EGsSkillCheckResultType::ImpossibleState;
}

bool FGsSkillCheckState::ProcessCheck(const FGsSkill* NextSkill)
{
	UGsGameObjectBase* local = GSGameObject()->FindObject(EGsGameObjectType::LocalPlayer);
	FGsMovementHandlerBase* movementHandler = local->GetMovementHandler();
	FGsGameObjectStateManager* fsm = local->GetBaseFSM();

	IGsStateBase* state = fsm->CurrentState();
	switch (static_cast<EGsStateBase>(state->GetStateID()))
	{
	case EGsStateBase::SocialAction:	// 퀘스트 대화중
	case EGsStateBase::RunAway:
	case EGsStateBase::Dying:
	case EGsStateBase::Freeze:
	case EGsStateBase::ChangeWeapon:
	case EGsStateBase::Warp:
	case EGsStateBase::Spawn:
		return false;
	}

	// 시네마틱 플레이
	if (UGsSequencePlayer* sequencePlayer = GSequencePlayer())
	{
		if (sequencePlayer->IsPlaying())
		{
			return false;
		}
	}
	
	return true;
}

FText FGsSkillCheckState::GetErrMessage() const
{
	FText findText;
	// FText::FindText(TEXT("SkillUIText"), TEXT("Notice_SkillCheckResult_ImpossibleState"), findText);
	return findText;
}
