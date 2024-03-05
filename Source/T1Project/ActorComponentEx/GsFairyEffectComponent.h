#pragma once
#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Particles/WorldPSCPool.h"
#include "Engine/EngineTypes.h"
#include "GsFairyEffectComponent.generated.h"

/*
	페어리용(Actor용) 파티클 출력 컴퍼넌트
*/

class UParticleSystem;
class UParticleSystemComponent;
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class T1PROJECT_API UGsFairyEffectComponent : public UActorComponent
{
	GENERATED_BODY()
protected:
	// 이펙트 붙일 타겟
	USceneComponent* _attachTarget = nullptr;
	// 이펙트 크기
	
public:
	UGsFairyEffectComponent();

	// 로직함수
public:
	// 이펙트 출력
	UParticleSystemComponent* OnEffect(
		UParticleSystem* In_particle, 
		USceneComponent* In_targetcomponent = nullptr,
		bool In_isAutoDestroy = true,
		EPSCPoolMethod In_poolType = EPSCPoolMethod::AutoRelease,
		EAttachLocation::Type In_locationType = EAttachLocation::Type::KeepRelativeOffset,
		FVector In_location = FVector(ForceInit),
		FRotator In_rotate = FRotator::ZeroRotator,
		FVector effectScale = (FVector)(1.0f),
		FName In_attachPointName = NAME_None);


};