// Fill out your copyright notice in the Description page of Project Settings.


#include "GsTargetHandlerBase.h"
#include "ActorEx/GsCharacterBase.h"
#include "GameObject/ObjectClass/GsGameObjectBase.h"
#include "GameObject/ObjectClass/GsGameObjectCreature.h"
#include "GameObject/Movement/GsMovementHandlerBase.h"
#include "Message/GsMessageGameObject.h"
#include "GameObject/Movement/GsMovementHandlerBase.h"

#include "AI/Function/GsAIFunc.h"

void FGsTargetHandlerBase::Initialize(UGsGameObjectBase* owner)
{
	_owner = owner;
}

void FGsTargetHandlerBase::ClearTarget(bool isAllClear)
{
	_target = nullptr;
}

void FGsTargetHandlerBase::ClearTarget(UGsGameObjectBase* target)
{
	if (target == _target)
	{
		ClearTarget();
	}
}

void FGsTargetHandlerBase::ClearSkillSelectTarget(int64 InGameId)
{
}

bool FGsTargetHandlerBase::SetTarget(UGsGameObjectBase* target, bool isRotate, SelectTargetFilter selectFilter)
{
	// 동일한 타겟 체크
	if (target == _target)
	{
		return false;
	}

	// 자기 자신은 타겟이 될수 없음
	if (target == _owner)
	{
		return false;
	}

	// 기존 타겟이 있다면 Clear
	if (_target.IsValid())
	{
		ClearTarget(true);
	}

	if (target && target->IsZeroHP())
	{
		return false;
	}

	if (isRotate)
	{
		_owner->LookAtTarget(target, true);
	}

	_target = target;
	_selectFilter = selectFilter;

	return true;
}

void FGsTargetHandlerBase::CallbackClearTarget(UGsGameObjectBase* target)
{
	ClearTarget(target);
}

bool FGsTargetHandlerBase::GetIsAutoTarget() const
{
	return _selectFilter == SelectTargetFilter::Auto;
}

UGsGameObjectBase* FGsTargetHandlerBase::GetTarget() const
{
	if (_target.IsValid())
	{
		// Target이 Die (MessageGameObject::COMMON_DIE) 될때 델리게이트 처리가 되어있지만 제대로 처리가 안될때가 있는것 같다.
		// @see : void UGsGameObjectCreature::BindMessage()
		// 좀더 확인이 필요
		if (_target->IsVaildClass())
		{
			return _target.Get();
		}
		else
		{
			GSLOG(Error, TEXT("Target Class Deleting! [ %s ]"), *_target->GetName());
		}
	}
	return nullptr;
}