// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ActorEx/GsActorSequence.h"
#include "GameFramework/Pawn.h"
#include "DataSchema/Fairy/GsSchemaFairyEffect.h"
#include "GsActorFairyObject.generated.h"

class UInputComponent;
class UCameraComponent;
class AGsActorFieldFairy;

UCLASS()
class T1PROJECT_API AGsActorFairyObject : public AGsActorSequence
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	USceneComponent* _fairyRoot;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	USceneComponent* _cameraZoomInPos;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	USceneComponent* _cameraZoomOutPos;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* _camera;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	USceneComponent* _cameraRotator;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	AGsActorFieldFairy* _fairy;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GsCamera")
	FVector _curPos{ 0.f, 0.f, 0.f };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GsCamera")
	bool _isZoomStart = false;;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GsCamera")
	FVector _cameraEnchantOffset{ FVector::ZeroVector };

	bool _touchZoomLock{ false };

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GsFairy")
	float _baseFairyHeight{ 0.0f };

	//zoom 처리 변동값
protected:
	enum class Zoom {In, Out};

	Zoom _zoomType{ Zoom::In };
	bool _isZoomEnd{ true };

	float _deltaZoom{ 0.f };
	
	FVector _targetPos;	// zoom End
	FVector _startPos;	// zoom Start

	FQuat _targetRot;
	FQuat _startRot;
	FQuat _curRot;

	FVector _touchPressLocation[2] = { FVector::ZeroVector, FVector::ZeroVector };
	float _touchStartDist{ 0.f }; //Two touch started 되었을 때의 시작 offset

	FVector _preTouchPos{ FVector::ZeroVector };
	FQuat  _initQuat{ FQuat::Identity };

	float _preFairyHeight{ 0.0f };
	
	//Zoom 고정값
protected: 
	FVector _zoomDirection{ FVector::ZeroVector }; //distance 계산을 위한 normalized vector
	FVector _zoomInPos{ FVector::ZeroVector };		// zoom Min position
	FVector _zoomOutPos{ FVector::ZeroVector };	// zoom Max position

	float _zoomTotalDistance{ 0.f };
	FQuat _zoomInRotation{ FQuat::Identity };		// zoom Min rot
	FQuat _zoomOutRotation{ FQuat::Identity };		// zoom Max rot

	float _zoomSpeed{ 0.1f };		//GData()->GetGlobalData()->_fairyZoomSpeed
	float _zoomForce{ 50.f };	//GData()->GetGlobalData()->_fairyZoomDistance
	float _touchZoomFactor{ 1.f };

	float _touchMoveFactor{ 0.5f }; //터치 이동거리 조절값(값이 작아질 수록 움직임이 작아짐)
	float _touchMoveLimit{ 3.0f };	// 터치 이동거리 제한값(min, max가 됨)

private:
	bool _runObject{ false };
	ULocalPlayer* _localPlayer;

	//float _halfFairySize{ 0.0f };
	//float _shiftZPos{ 0.0f };

	FVector zShiftCurrPos;

	float _pitchMin = 0.0f;
	float _pitchMax = 0.0f;

	float _zoomRate = 1.0f;
	float _zoomAddValPC = 0.3f;
	float _zoomMulValMobile = 1.0f;

	FVector _cameraAddOffset{ FVector::ZeroVector };

	bool _isPressed{ false };
private:
	bool	_isLoading = false;
	FString _loadPath;
	
	TFunction<void()> _loadFunction;

public:	
	// Sets default values for this actor's properties
	AGsActorFairyObject();

	UFUNCTION(BlueprintImplementableEvent, Category = "SummonEvent")
	void StartSummon();

	UFUNCTION(BlueprintImplementableEvent, Category = "SummonEvent")
	void EndSummon();

	UFUNCTION(BlueprintImplementableEvent, Category = "ActorSpawn")
	void OnSpawnFairy(float neckHeight);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void EndPlay(EEndPlayReason::Type Reaseon) override;
	//virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:	
	TWeakObjectPtr<USceneComponent> GetFairyRoot() { return _fairyRoot; }
	TWeakObjectPtr<USceneComponent> GetCameraZoomOutPosition() { return _cameraZoomOutPos; };
	TWeakObjectPtr<USceneComponent> GetCameraZoomInPosition()	{ return _cameraZoomInPos; };

	void InitAction();
	void EndAction();
	void SetCameraInitialize();
	void SetFairy(FString path, const TArray<FGsSchemaFairyEffect>& baseEffectData = TArray<FGsSchemaFairyEffect>(), bool isAsync = false);
	void ChangeSubScene(bool start);	

protected:
	void ZoomIn();
	void ZoomOut();

	void OnTouchPress(ETouchIndex::Type FingerIndex, FVector Location);
	void OnTouchRelease(ETouchIndex::Type FingerIndex, FVector Location);
	void OnTouchMove(ETouchIndex::Type FingerIndex, FVector Location);

	void OnMoveRotateYaw(float In_yaw);
	void OnMoveRotatePitch(float In_Pitch);

	void OnFairyLoad(UClass* loadObjectClass, const TArray<FGsSchemaFairyEffect>& baseEffectData);

public:

	//Cheat
	void SetFairyZoomValue(float speed, float maxDist = 0.f, float touchMulti = 0.f);
	void SetFairyRotateValue(float touchDistMul = 0.f, float touchLimit = 0.f);

	void SetZoomOffset(FVector offset);
	void ResetZoomOffset();
};
