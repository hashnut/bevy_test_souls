#include "GsInteractionHandlerBase.h"
#include "GameObject/ObjectClass/GsGameObjectBase.h"
#include "GameObject/State/GsGameObjectStateManager.h"

UGsGameObjectBase* FGsInteractionHandlerBase::GetInteractionTarget() const
{
	if (_interactionTarget.IsValid())
	{
		return _interactionTarget.Get();
	}
	return nullptr;
}

void FGsInteractionHandlerBase::Initialize(UGsGameObjectBase* In_owner)
{
	_owner = In_owner;
}

void FGsInteractionHandlerBase::ClearTarget()
{
	_interactionTarget.Reset();
}

void FGsInteractionHandlerBase::StartInteraction(UGsGameObjectBase* inTarget)
{
	// 타입 검출 타겟 정보가 없는 상황도 고려 해야 할것
	if (false == _owner.IsValid())
	{
		return;
	}

	_interactionTarget = inTarget;
	FGsGameObjectStateManager* fsm = _owner->GetBaseFSM();
	fsm->ProcessEvent(EGsStateBase::Interaction);
}

void FGsInteractionHandlerBase::EndInteraction(UGsGameObjectBase* inTarget)
{
	if (false == _owner.IsValid())
	{
		return;
	}

	FGsGameObjectStateManager* fsm = _owner->GetBaseFSM();
	fsm->ProcessEvent(EGsStateBase::InteractionEnd);
}

void FGsInteractionHandlerBase::CancelInteration(UGsGameObjectBase* inTarget)
{
	if (false == _owner.IsValid())
	{
		return;
	}

	FGsGameObjectStateManager* fsm = _owner->GetBaseFSM();
	fsm->ProcessEvent(EGsStateBase::InteractionEnd);
}

void FGsInteractionHandlerBase::CancelInteration()
{
	if (false == _owner.IsValid())
	{
		return;
	}

	FGsGameObjectStateManager* fsm = _owner->GetBaseFSM();
	fsm->ProcessEvent(EGsStateBase::InteractionEnd);
}

bool FGsInteractionHandlerBase::IsInteraction() const
{
	return _interactionTarget.IsValid();
}