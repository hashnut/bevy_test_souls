// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Shared/Client/SharedEnums/SharedCreatureEnum.h"
#include "GsActorUserProfileSceneObject.generated.h"


class UCameraComponent;
class AGsCharacterPlayer;
class AGsActorFieldFairy;
class FGsCustomizeHandler;
struct FGsSchemaUserData;
UCLASS()
class T1PROJECT_API AGsActorUserProfileSceneObject : public AActor
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	USceneComponent* _characterRoot;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	USceneComponent* _fairyRoot;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	USceneComponent* _cameraPos;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	USceneComponent* _cameraRotator;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* _camera;

protected:
	AGsCharacterPlayer* _character;
	AGsActorFieldFairy* _fairy;

	TSharedPtr<FGsCustomizeHandler> _customizeHandler{ nullptr };
	
	bool _runObject{ false };

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GsCamera")
		FVector _curPos {
		0.f, 0.f, 0.f
	};

	FVector _touchPressLocation[2] = { FVector::ZeroVector, FVector::ZeroVector };

	//zoom 처리 변동값
protected:
	FVector _targetPos;	// zoom End
	FVector _startPos;	// zoom Start

	FQuat _targetRot;
	FQuat _startRot;
	FQuat _curRot;

	float _touchStartDist{ 0.f }; //Two touch started 되었을 때의 시작 offset

	FVector _preTouchPos{ FVector::ZeroVector };
	FQuat  _initQuat{ FQuat::Identity };

protected:
	float _touchMoveFactor{ 0.5f }; //터치 이동거리 조절값(값이 작아질 수록 움직임이 작아짐)
	float _touchMoveLimit{ 3.0f };	// 터치 이동거리 제한값(min, max가 됨)
	
public:	
	// Sets default values for this actor's properties
	AGsActorUserProfileSceneObject();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void EndPlay(EEndPlayReason::Type Reason) override;

protected:
	void OnTouchPress(ETouchIndex::Type FingerIndex, FVector Location);
	void OnTouchRelease(ETouchIndex::Type FingerIndex, FVector Location);
	void OnTouchMove(ETouchIndex::Type FingerIndex, FVector Location);

	void LoadAnimInstance(const FString& ClassPath, AGsCharacterPlayer* manekin);

public:
	void InitAction();
	void EndAction();

	void SetCharacter();
	void SetFairy();

	void PlayCharacterAnim();

protected:
	void DestroyCharacter();
	void DestroyFairy();
};
