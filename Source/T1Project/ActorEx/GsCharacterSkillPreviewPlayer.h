// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ActorEx/GsCharacterPlayer.h"
#include "GsCharacterSkillPreviewPlayer.generated.h"

/**
 * 
 */
UCLASS()
class T1PROJECT_API AGsCharacterSkillPreviewPlayer : public AGsCharacterPlayer
{
	GENERATED_BODY()

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UGsSkillCollisionComponent* _skillCollision = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UGsSkillCollisioHandlerComponent* _skillCollisionHandle = nullptr;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UGsSkillGuideEffectComponent* _skillGuideEffect = nullptr;

public:
	AGsCharacterSkillPreviewPlayer();

public:
	FORCEINLINE class UGsSkillCollisionComponent* GetSkillCollisionComponennt() const { return _skillCollision; }
};
