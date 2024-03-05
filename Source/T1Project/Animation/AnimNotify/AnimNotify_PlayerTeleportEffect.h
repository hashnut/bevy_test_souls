// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotify/AnimNotify_PlayParticleParam.h"
#include "AnimNotify_PlayerTeleportEffect.generated.h"

enum class ETeleportEffectType : uint8;

/**
 * 텔레포트 시 출력할 플레이어의 파티클 이펙트
 * 페어리 등급에 따라 이펙트 출력이 달라집니다.
 */
UCLASS()
class T1PROJECT_API UAnimNotify_PlayerTeleportEffect : public UAnimNotify_PlayParticleParam
{
	GENERATED_BODY()

private:
	// 출발, 도착 등의 타입 설정
	UPROPERTY(EditAnywhere, Category = "TeleportEffect")
	ETeleportEffectType _playerTeleportEffect;
	
public:
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;
};
