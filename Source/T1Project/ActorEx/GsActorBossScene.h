// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ActorEx/GsActorSequence.h"
#include "GameFramework/Pawn.h"
#include "DataSchema/Fairy/GsSchemaFairyEffect.h"
#include "GsActorBossScene.generated.h"

class UInputComponent;
class UCameraComponent;
struct FGsSchemaNpcData;
struct FGsSchemaFieldBossData;
class USceneCaptureComponent2D;
struct FGsSchemaGuildDungeon;
class ACharacter;

UENUM()
enum class EGsBossSceneMode
{
	BossInfo,
	GuildDungeon
};

UCLASS()
class T1PROJECT_API AGsActorBossScene : public AGsActorSequence
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	USceneComponent* _root;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	USceneComponent* _cameraZoomInPos;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	USceneComponent* _cameraZoomOutPos;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	USceneComponent* _cameraRotator;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UChildActorComponent* _childActorComp;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		USceneCaptureComponent2D* _sceneCaptureComp;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		UStaticMeshComponent* _bgDome;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		UStaticMeshComponent* _bgGuilddungoenDome;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GsCamera")
	FVector _curPos{ 0.f, 0.f, 0.f };
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GsCamera")
	bool _isZoomStart = false;;

protected:
	UPROPERTY()
	 ACharacter* _mannequin;
	EGsBossSceneMode _bossSceneMode;

private:
	float _defaultMinDistance;
	float _defaultMaxDistance;
	FVector _defaultDomScale;

	float _currentMinDistance;
	float _currentMaxDistance;

	float _currentMannequinRadius;

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

public:	
	// Sets default values for this actor's properties
	AGsActorBossScene();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void EndPlay(EEndPlayReason::Type Reaseon) override;
	//virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:	
	TWeakObjectPtr<USceneComponent> GetRoot() { return _root; }
	TWeakObjectPtr<USceneComponent> GetCameraZoomOutPosition() { return _cameraZoomOutPos; };
	TWeakObjectPtr<USceneComponent> GetCameraZoomInPosition()	{ return _cameraZoomInPos; };

	void InitAction();
	void EndAction();	
	void SetMannequin(const FGsSchemaFieldBossData* inBossData);
	void SetMannequin(const FGsSchemaGuildDungeon* inBossData);
	void ChangeSubScene(bool start);
	void Clear();
	bool TryLoadBoss(const FGsSchemaNpcData* inNpc);

protected:
	void ZoomIn();
	void ZoomOut();

	void OnTouchPress(ETouchIndex::Type FingerIndex, FVector Location);
	void OnTouchRelease(ETouchIndex::Type FingerIndex, FVector Location);
	void OnTouchMove(ETouchIndex::Type FingerIndex, FVector Location);
	void InvalidZoomPos();

public:

	//Cheat
	void SetZoomValue(float speed, float maxDist = 0.f, float touchMulti = 0.f);
	void SetRotateValue(float touchDistMul = 0.f, float touchLimit = 0.f);
	ACharacter* GetMannequin(){return _mannequin;}
	void SetBossSceneMode(EGsBossSceneMode inBossCeneMode);
};
