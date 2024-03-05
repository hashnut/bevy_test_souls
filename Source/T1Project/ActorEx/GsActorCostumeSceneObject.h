// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "GameFramework/Actor.h"
#include "ActorEx/GsActorSequence.h"
#include "GsActorCostumeSceneObject.generated.h"

class UInputComponent;
class UCameraComponent;
class AGsCharacterPlayer;
class FGsCustomizeHandler;

UCLASS()
class T1PROJECT_API AGsActorCostumeSceneObject : public AGsActorSequence
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	USceneComponent* _characterRoot;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	USceneComponent* _cameraZoomInPos;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	USceneComponent* _cameraZoomOutPos;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* _camera;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	USceneComponent* _cameraRotator;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GsCamera")
	FVector _curPos {0.f, 0.f, 0.f};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GsCamera")
	bool _isZoomStart = false;;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GsCharacter")
	AGsCharacterPlayer* _manekin;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GsCharacter")
	float _baseManekinHeight{ 0.0f };

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GsCharacter")
	float _halfManekinHeight{ 88.f };
	
public:	
	// Sets default values for this actor's properties
	AGsActorCostumeSceneObject();

	UFUNCTION(BlueprintImplementableEvent, Category = "SummonEvent")
	void StartSummon();

	UFUNCTION(BlueprintImplementableEvent, Category = "SummonEvent")
	void EndSummon();

	UFUNCTION(BlueprintImplementableEvent, Category = "ActorSpawn")
	void OnSpawnCharacter(float neckHeight);

	UFUNCTION()
	void OnCostumeEffectLoadComplate(AActor* LoadActor);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void EndPlay(EEndPlayReason::Type Reaseon) override;

	void ZoomIn();
	void ZoomOut();

	void OnTouchPress(ETouchIndex::Type FingerIndex, FVector Location);
	void OnTouchRelease(ETouchIndex::Type FingerIndex, FVector Location);
	void OnTouchMove(ETouchIndex::Type FingerIndex, FVector Location);

	void LoadAnimInstance(const FString& ClassPath, AGsCharacterPlayer* manekin);

	void OnMoveRotateYaw(float In_yaw);
	void OnMoveRotatePitch(float In_pitch);

	void SetCameraInitialize();
	void CostumeActorPostCameraModify(float delta);
public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	TWeakObjectPtr<USceneComponent> GetCharacterRoot() { return _characterRoot; }
	TWeakObjectPtr<USceneComponent> GetCameraZoomOutPosition() { return _cameraZoomOutPos; };
	TWeakObjectPtr<USceneComponent> GetCameraZoomInPosition() { return _cameraZoomInPos; };

	void InitAction();
	void EndAction();

	void SetCostume(CostumeId costumeid, bool isHairOn);
	void SetAppearance(CostumeId appearanceId, bool applyHair, bool applyBody);
	void ChangeSubScene(bool start);

protected:
	//zoom 처리 변동값
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

	bool _isPressed{ false };

protected: 
	//Zoom 고정값
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

	FVector _cameraAddOffset{ FVector::ZeroVector };
	bool _isCurPositionInit{ false };

private:
	bool _runObject{ false };

	TSharedPtr<FGsCustomizeHandler> _customizeHandler{ nullptr };

	struct Appearance
	{
		CostumeId costumeId{ INVALID_COSTUME_ID };
		bool isAppliedHair{ false };
		bool isAppliedBody{ false };
	};

	CostumeId  _selectCostumeId{ INVALID_COSTUME_ID };
	Appearance _selectAppearance{ INVALID_COSTUME_ID , false, false };
	bool _isHairOn{ false };
	
	// for camera rotation
	//float _baseManekinHeight{ 0.0f };
	// half + char root Z
	//float _shiftZPos{ 0.0f };

	float _pitchMin = 0.0f;
	float _pitchMax = 0.0f;

	FVector zShiftCurrPos;

	float _zoomRate = 1.0f;
	float _zoomAddValPC = 0.3f;
	float _zoomMulValMobile = 1.0f;
};
