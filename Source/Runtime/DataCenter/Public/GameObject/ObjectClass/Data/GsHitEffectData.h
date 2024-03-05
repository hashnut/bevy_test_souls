#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GameObject/Define/HitEffectVisibleType.h"
#include "GameObject/Define/HitEffectAttachType.h"
#include "GameObject/Define/HitEffectIgnoreType.h"
#include "Particles/ParticleSystemComponent.h"
#include "Sound/GsSchemaSoundResData.h"
#include "GsHitEffectData.generated.h"

/**
 * 히트 이펙트 데이터를 관리하기위한 구조
 */
UCLASS(BlueprintType, Category = "HitEffectData")
class DATACENTER_API UGsHitEffectData : public UDataAsset
{
	GENERATED_BODY()
public:
	UGsHitEffectData() = default;
public:

	UPROPERTY(EditAnywhere, Category = "HitEffectData")
	HitEffectVisibleType		Visible;

	UPROPERTY(EditAnywhere, Category = "HitEffectData")
	TArray<UParticleSystem*>	ParticleSystems;

	UPROPERTY(EditAnywhere, Category = "HitEffectData")
	HitEffectAttachType			Attach = HitEffectAttachType::SOCKET;

	UPROPERTY(EditAnywhere, Category = "HitEffectData",  Meta = (Tooltip = "SkillNotify에 있는 HitEffect정보만 해당됩니다."))
	HitEffectIgnoreType			IgnoreType = HitEffectIgnoreType::NONE;

	UPROPERTY(EditAnywhere, Category = "HitEffectData")
	FString						SocketName;

	// 더이상 사용하지 않을 예정이지만 레거시로 남겨둠
	UPROPERTY(VisibleAnywhere, Category = "HitEffectData")
	float						ScaleFactor = 1.0f;

	UPROPERTY(EditAnywhere, Category = "HitEffectData")
	float						BoundMultipleFactor = 1.0f;

	UPROPERTY(EditAnywhere, Category = "HitEffectData")
	FVector						Offset = FVector::ZeroVector;

	UPROPERTY(EditAnywhere, Category = "HitEffectData", Meta = (EditCondition = "Attach == HitEffectAttachType::SOCKET_RANDOM_BOUNDING", EditConditionHides))
	float						RandomRadius = 10.f;

	UPROPERTY(EditAnywhere, Category = "HitEffectData")
	FRotator					SpawnRotator = FRotator::ZeroRotator;
};
