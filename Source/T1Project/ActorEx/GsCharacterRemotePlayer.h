// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ActorEx/GsCharacterPlayer.h"
#include "GameObject/Interaction/GsActorInteractionInterface.h"
#include "GsCharacterRemotePlayer.generated.h"

/**
 * 
 */
class UGsUROComponent;

// IGsActorInteractionInterface: 셀렉트 타겟팅 용
UCLASS()
class T1PROJECT_API AGsCharacterRemotePlayer final : public AGsCharacterPlayer, public IGsActorInteractionInterface
{
	GENERATED_BODY()

	//URO
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Character base", meta = (AllowPrivateAccess = "true"))
		UGsUROComponent* _uroComponent;

private:
	// 인터랙션 클릭 콜백
	TFunction<void(bool)> _callbackInteractionClick;
	
public:
	// Sets default values for this character's properties
	AGsCharacterRemotePlayer();	

protected:
	virtual void BeginPlay() override;

public:
	// IGsActorInteractionInterface
	// 인터랙션 클릭
	virtual void ClickInteraction() override;

	// 인터랙션 클릭 콜백 세팅
	virtual void SetInteractionClickCallback(TFunction<void(bool)> In_callback) override;
};
