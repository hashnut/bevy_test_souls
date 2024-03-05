#include "GsRegionMapIconObject.h"

#include "GameObject/ObjectClass/GsGameObjectBase.h"

// 초기화
void UGsRegionMapIconObject::SetTargetObject(UGsGameObjectBase* In_targetObj)
{
	_targetObject = In_targetObj;
}

FVector2D UGsRegionMapIconObject::GetPos()
{
	if (nullptr == _targetObject)
	{
		return FVector2D::ZeroVector;
	}

	AActor* targetActor = _targetObject->GetActor();
	if (nullptr == targetActor)
	{
		return FVector2D::ZeroVector;
	}
	FVector pos = targetActor->GetActorLocation();
	return FVector2D(pos);
}

// 정리
void UGsRegionMapIconObject::Finalize()
{
	_targetObject = nullptr;
}