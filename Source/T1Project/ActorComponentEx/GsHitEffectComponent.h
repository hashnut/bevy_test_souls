// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Shared/Client/SharedEnums/SharedCreatureEnum.h"
#include "GsHitEffectComponent.generated.h"

/**
* HitEffect처리용 컴퍼넌트
*/

class UGsHitEffectData;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class T1PROJECT_API UGsHitEffectComponent : public UActorComponent
{
	GENERATED_BODY()
	
private:
	bool				_enable = false;
	
	FVector				_effectScale = (FVector)(1.0f);
	USceneComponent*	_attachTarget = nullptr;
	UGsHitEffectData*	_data = nullptr;
	const struct FGsSchemaSoundResData* _soundData = nullptr;

	// 오버라이드할 값들 저장
	float _overrideEffectScale = 0.f;
	float _modelRadius = 0.f;	

public:	
	// Sets default values for this component's properties
	UGsHitEffectComponent();

public:
	void OverrideData(UGsHitEffectData* inData, const struct FGsSchemaSoundResData* inSoundData, 
		float inOverrideEffectSizeRate, float inOverrideModelRadius);

	// UGsHitEffectData* inData 정보가 있을경우 이미 저장된 _data를 참조하지 않고 inData를 참조
	void OnHitEffect(const FVector& attackerLocation, UGsHitEffectData* inData = nullptr, bool inAttackerHeightFixed = false);
	void OnHitSound(const struct FGsSchemaSoundResData* inSoundData, bool inIsCritical = false,
		CreatureWeaponType inWeaponType = CreatureWeaponType::NONE);

private:
	bool	ValidateData(UGsHitEffectData* inData);
	bool	FindAttachTarget();
	float	ExtrectModelRadius();

	void	GetPlayList(TArray<UParticleSystem*>& outList, UGsHitEffectData* inData) const;
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;	
};
