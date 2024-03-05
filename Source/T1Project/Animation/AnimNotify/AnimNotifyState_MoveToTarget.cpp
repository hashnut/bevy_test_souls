// Fill out your copyright notice in the Description page of Project Settings.

#include "AnimNotifyState_MoveToTarget.h"

#include "GameObject/Define/GsGameObjectDefine.h"
#include "GameObject/Target/GsTargetHandlerBase.h"
#include "GameObject/Skill/GsSkillHandlerBase.h"
#include "GameObject/ObjectClass/GsGameObjectBase.h"
#include "GameObject/Movement/GsMovementHandlerBase.h"

#include "Management/ScopeGame/GsGameObjectManager.h"
#include "Management/ScopeGame/GsSkillPreviewManager.h"
#include "DrawDebugHelpers.h"


UAnimNotifyState_MoveToTarget::UAnimNotifyState_MoveToTarget(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{
}

void UAnimNotifyState_MoveToTarget::NotifyBegin(USkeletalMeshComponent* in_MeshComp, UAnimSequenceBase* in_Animation, float in_TotalDuration)
{
// 	if (false == in_MeshComp->GetWorld()->IsGameWorld())
// 	{
// 		return;
// 	}
// 
// 	_totalTime = in_TotalDuration;
// 	_destPos = FVector::ZeroVector;
// 
// 	// 스킬 미리보기용으로만 잠시 사용
// 	AActor* owner = in_MeshComp->GetOwner();
// 	if (nullptr == owner)
// 	{
// 		return;
// 	}
// 	_gameObject = GSSkillPreview()->FindObjectByActor(owner);
// 	if (nullptr == _gameObject)
// 	{
// 		return;
// 	}
// 
// 	_elapsedTime = 0.f;
// 
// 	FGsTargetHandlerBase* targetHandler = _gameObject->GetTargetHandler();
// 	if (nullptr == targetHandler)
// 	{
// 		return;
// 	}
// 
// 	// 내 플레이어, 스킬 미리보기, 타유저 타겟 대상 얻기
// 	UGsGameObjectBase* target = targetHandler->GetTarget();
// 	if (nullptr == target)
// 	{
// 		if (FGsSkillHandlerBase* skillHandler = _gameObject->GetSkillHandler())
// 		{
// 			const FGsSkillNetData* netData = skillHandler->GetNetSkillData();
// 			target = GSGameObject()->FindObject(EGsGameObjectType::Creature, netData->_targetGameId);
// 		}
// 	}
// 	if (nullptr == target)
// 	{
// 		target = _gameObject;
// 	}
// 
// 	// 목적 좌표가 미리 설정 되어 있는지 확인
// 	if (_destPos.IsNearlyZero())
// 	{
// 		// 캡슐 반경만큼 계산
// 		const FVector& inverseDir = (_gameObject->GetLocation() - target->GetLocation()).GetSafeNormal();
// 		_destPos = target->GetLocation() + inverseDir * target->GetData()->GetScaledCapsuleRadius();
// 		_velocity = (_destPos - _gameObject->GetLocation()) / _totalTime;
// 	}
}

void UAnimNotifyState_MoveToTarget::NotifyTick(USkeletalMeshComponent* in_MeshComp, UAnimSequenceBase* in_Animation, float in_FrameDeltaTime)
{
// 	if (_destPos != FVector::ZeroVector && _gameObject && _gameObject->IsVaildClass())
// 	{
// 		FGsMovementHandlerBase* movementHandler = _gameObject->GetMovementHandler();
// 		if (nullptr == movementHandler)
// 		{
// 			return;
// 		}
// 
// 		FVector movePos = _gameObject->GetLocation() + _velocity * in_FrameDeltaTime;
// 		movementHandler->SetLocationAndRotation(movePos, _velocity);
// 		
// 		DrawDebugSphere(in_MeshComp->GetWorld(), movePos, 10.f, 10.f, FColor::Red, false, 1.f);
// 	}
// 	_elapsedTime += in_FrameDeltaTime;
}

float UAnimNotifyState_MoveToTarget::GetTotalTime() const
{
	return _totalTime;
}