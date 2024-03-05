// Fill out your copyright notice in the Description page of Project Settings.


#define  BOSS_SCENE true
#define  BOSS_ZOOM_ACTIVE false

#include "GsActorBossScene.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/SceneCaptureComponent2D.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/PlayerController.h"
#include "Data/GsGlobalConstant.h"
#include "Data/GsDataContainManager.h"

#include "Management/ScopeGlobal/GsLevelManager.h"
#include "Management/ScopeGame/GsGameObjectManager.h"
#include "GameObject/ObjectClass/GsGameObjectLocalPlayer.h"
#include "DataSchema/GameObject/Npc/GsSchemaNpcData.h"
#include "DataSchema/Guild/GuildDungeon/GsSchemaGuildDungeon.h"
#include "DataSchema/Map/FieldBoss/GsSchemaFieldBossData.h"
#include "GsCharacterNonPlayer.h"
#include "GsCharacterBase.h"


// Sets default values
AGsActorBossScene::AGsActorBossScene()
	: Super()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootSceneObject"));

	_root = CreateDefaultSubobject<USceneComponent>(TEXT("AttachRoot"));
	_root->SetupAttachment(RootComponent);

	_cameraZoomInPos = CreateDefaultSubobject<USceneComponent>(TEXT("CameraZoomInPos"));
	_cameraZoomInPos->SetupAttachment(RootComponent);

	_cameraZoomOutPos = CreateDefaultSubobject<USceneComponent>(TEXT("CameraZoomOutPos"));
	_cameraZoomOutPos->SetupAttachment(RootComponent);

	_cameraRotator = CreateDefaultSubobject<USceneComponent>(TEXT("CameraRoot"));
	_cameraRotator->SetupAttachment(RootComponent);

	/*_camera = CreateDefaultSubobject<USceneComponent>(TEXT("SceneCamera"));
	_camera->SetupAttachment(_cameraRotator);*/

	_sceneCaptureComp = CreateDefaultSubobject<USceneCaptureComponent2D >(TEXT("SceneCapture"));
	_sceneCaptureComp->SetupAttachment(_cameraRotator);

	_bgDome = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("_bgDome"));
	_bgDome->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AGsActorBossScene::BeginPlay()
{
	Super::BeginPlay();

	//get default distance
	_defaultMinDistance = _cameraZoomInPos->GetRelativeLocation().Size();
	_defaultMaxDistance = _cameraZoomOutPos->GetRelativeLocation().Size();

#if BOSS_SCENE
	GSLOG(Log, TEXT("BossScene - defaultMinDistance : %f\t defaultMaxDistance : % f"), _defaultMinDistance, _defaultMaxDistance);
#endif

	_zoomOutPos = _curPos = _cameraZoomOutPos->GetRelativeLocation();
	_zoomInPos = _cameraZoomInPos->GetRelativeLocation();

#if BOSS_SCENE
	GSLOG(Log, TEXT("BossScene - zoom out pos : %s\t zoom in pos : % s"), *_zoomOutPos.ToString(), *_zoomInPos.ToString());
#endif

	_zoomOutRotation = _curRot = _cameraZoomOutPos->GetRelativeRotation().Quaternion();
	_zoomInRotation = _cameraZoomInPos->GetRelativeRotation().Quaternion();

	if (_sceneCaptureComp)
	{
		_sceneCaptureComp->SetRelativeLocation(_zoomOutPos);
		_sceneCaptureComp->SetRelativeRotation(_zoomOutRotation);
	}

	_zoomDirection = (_zoomInPos - _zoomOutPos).GetSafeNormal();
	_zoomTotalDistance = (_zoomInPos - _zoomOutPos).Size();

	_zoomSpeed = GData()->GetGlobalData()->_fairyZoomSpeed;	//최대 이동 거리를 움직이는 sec
	_zoomForce = GData()->GetGlobalData()->_fairyZoomDistance; //최대 이동 거리
	_touchZoomFactor = GData()->GetGlobalData()->_fairyTouchZoomFactor;

	_touchMoveFactor = GData()->GetGlobalData()->_fairyRotateMoveFactor; //페어리 회전시 터치 이동거리 조절값(값이 작아질 수록 움직임이 작아짐)
	_touchMoveLimit = GData()->GetGlobalData()->_fairyRotateMoveLimit;

	_initQuat = _root->GetRelativeRotation().Quaternion();

	_cameraRotator->SetRelativeLocation(FVector{ _root->GetRelativeLocation().X,  _root->GetRelativeLocation().Y, 0.f });
	_cameraRotator->SetRelativeRotation(_initQuat);

	_defaultDomScale = _bgDome->GetComponentScale();

	FVector2D viewSize;
	GEngine->GameViewport->GetViewportSize(viewSize);
}

void AGsActorBossScene::EndPlay(const EEndPlayReason::Type Reason)
{
	EndAction();
	Clear();

	Super::EndPlay(Reason);
}

// Called every frame
void AGsActorBossScene::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (_isZoomEnd)
	{
		_isZoomStart = false;
		_isZoomEnd = false;
	}

	if (_isZoomStart)
	{
		_deltaZoom += DeltaTime / _zoomSpeed;
		_deltaZoom = FMath::Clamp<float>(_deltaZoom, 0.0f, 1.0f);

		//FieldOfView = FMath::Lerp<float>(90.0f, 60.0f, _deltaZoom); FoV 값 조정은 현재는 없음
		_curPos = FMath::Lerp<FVector>(_startPos, _targetPos, _deltaZoom);
		_curRot = FQuat::Slerp(_startRot, _targetRot, _deltaZoom);

		_sceneCaptureComp->SetRelativeLocation(_curPos);
		_sceneCaptureComp->SetRelativeRotation(_curRot);

		if (_deltaZoom == 1.f)
		{
			_isZoomEnd = true;
		}
	}
}

void AGsActorBossScene::InitAction()
{
	_runObject = true;
	InputComponent = NewObject<UInputComponent>(this);

	APlayerController* controller = GetWorld()->GetFirstPlayerController();
	if (nullptr == controller)
	{
		return;
	}

#if BOSS_ZOOM_ACTIVE
	InputComponent->BindAction(TEXT("ZoomIn"), EInputEvent::IE_Pressed, this, &AGsActorBossScene::ZoomIn);
	InputComponent->BindAction(TEXT("ZoomOut"), EInputEvent::IE_Pressed, this, &AGsActorBossScene::ZoomOut);
#endif

	InputComponent->BindTouch(EInputEvent::IE_Pressed, this, &AGsActorBossScene::OnTouchPress);
	InputComponent->BindTouch(EInputEvent::IE_Released, this, &AGsActorBossScene::OnTouchRelease);
	InputComponent->BindTouch(EInputEvent::IE_Repeat, this, &AGsActorBossScene::OnTouchMove);

	EnableInput(controller);
}

void AGsActorBossScene::EndAction()
{
	if (!_runObject) return;

	if (APlayerController* controller = GetWorld()->GetFirstPlayerController())
	{
		if (InputComponent)
		{
			DisableInput(controller);
		}
	}
	//GLevel()->PopViewTarget(_camera->GetOwner());
	_runObject = false;
}

void AGsActorBossScene::SetMannequin(const FGsSchemaFieldBossData* inBossData)
{
	Clear();

	if (nullptr == inBossData
		|| nullptr == inBossData->bossNpcId.GetRow())
	{
		return;
	}

	const FGsSchemaNpcData* npcData = inBossData->bossNpcId.GetRow();
	if (false == TryLoadBoss(npcData))
	{
		return;
	}

	_mannequin->SetActorRotation(inBossData->cameraRot);

	if (inBossData->cameraMinDist > inBossData->cameraMaxDist)
	{
		_currentMinDistance = _defaultMinDistance;
		_currentMaxDistance = _defaultMaxDistance;
	}
	else
	{
		_currentMinDistance = inBossData->cameraMinDist;
		_currentMaxDistance = inBossData->cameraMaxDist;
	}

	InvalidZoomPos();
}

void AGsActorBossScene::SetMannequin(const FGsSchemaGuildDungeon* inGuildDungeon)
{
	Clear();

	if (nullptr == inGuildDungeon
		|| nullptr == inGuildDungeon->bossId.GetRow())
	{
		return;
	}

	if (false == TryLoadBoss(inGuildDungeon->bossId.GetRow()))
	{
		return;
	}

	_mannequin->SetActorRotation(inGuildDungeon->cameraRot);

	if (inGuildDungeon->cameraMinDist > inGuildDungeon->cameraMaxDist)
	{
		_currentMinDistance = _defaultMinDistance;
		_currentMaxDistance = _defaultMaxDistance;
	}
	else
	{
		_currentMinDistance = inGuildDungeon->cameraMinDist;
		_currentMaxDistance = inGuildDungeon->cameraMaxDist;
	}

	InvalidZoomPos();
}

void AGsActorBossScene::ChangeSubScene(bool start)
{
	if (false == _runObject)
	{
		return;
	}

	APlayerController* controller = GetWorld()->GetFirstPlayerController();
	if (nullptr == controller
		|| nullptr == InputComponent)
	{
		return;
	}

	if (start)
	{
		DisableInput(controller);
	}
	else
	{
#if BOSS_ZOOM_ACTIVE
		InputComponent->BindAction(TEXT("ZoomIn"), EInputEvent::IE_Pressed, this, &AGsActorBossScene::ZoomIn);
		InputComponent->BindAction(TEXT("ZoomOut"), EInputEvent::IE_Pressed, this, &AGsActorBossScene::ZoomOut);
#endif

		InputComponent->BindTouch(EInputEvent::IE_Pressed, this, &AGsActorBossScene::OnTouchPress);
		InputComponent->BindTouch(EInputEvent::IE_Released, this, &AGsActorBossScene::OnTouchRelease);
		InputComponent->BindTouch(EInputEvent::IE_Repeat, this, &AGsActorBossScene::OnTouchMove);

		EnableInput(controller);
	}
}

void AGsActorBossScene::Clear()
{
	if (_mannequin)
	{
		GetWorld()->DestroyActor(_mannequin);
	}
	_mannequin = nullptr;
}

bool AGsActorBossScene::TryLoadBoss(const FGsSchemaNpcData* inNpc)
{
	if (nullptr == inNpc
		|| nullptr == inNpc->shapeId.GetRow())
	{
		GSLOG(Error, TEXT("Npc shape is missing - id :  %d"), inNpc->id);
		return false;
	}

	FSoftObjectPath npcBPPath = inNpc->shapeId.GetRow()->bPClass;
	const FString& path = npcBPPath.ToString() + TEXT("_C");
	UClass* loadClass = LoadObject<UClass>(nullptr, *path);
	if (nullptr == loadClass)
	{
		GSLOG(Error, TEXT("Load blueprint is fail : %s"), *FString::FromInt(inNpc->id));
		return false;
	}

	FVector spawnLocation = GetActorLocation();
	FRotator spawnRotator;
	AGsCharacterNonPlayer* bossNpc = GetWorld()->SpawnActor<AGsCharacterNonPlayer>(loadClass, spawnLocation, spawnRotator);
	_mannequin = bossNpc;
	if (_mannequin)
	{
		_mannequin->AttachToActor(this, FAttachmentTransformRules::KeepRelativeTransform);
		UCharacterMovementComponent* moveComp = Cast<UCharacterMovementComponent>(_mannequin->GetMovementComponent());
		if (moveComp)
		{
			moveComp->GravityScale = 0;
		}

		_mannequin->SetActorRelativeLocation(FVector::ZeroVector);
		UGsAnimInstanceState* animState = Cast<UGsAnimInstanceState>(_mannequin->GetMesh()->GetAnimInstance());
		if (animState)
		{
			animState->ChangeState((uint8)EGsStateBase::Idle);
		}

		FVector origin;
		FVector extention;
		_mannequin->GetActorBounds(false, origin, extention, true);
		_currentMannequinRadius = extention.Size();


		GSLOG(Log, TEXT("BossScene - current mannequin radius : %f"), _currentMannequinRadius);

		bossNpc->ChangeLightingChannels(false, true);
	}

	return true;
}

void AGsActorBossScene::ZoomIn()
{
	if (_isZoomStart && _zoomType == Zoom::In) return;

	_zoomType = Zoom::In;

	FVector targetPos = _curPos + _zoomDirection * _zoomForce;
	targetPos = targetPos.BoundToBox(_zoomInPos, _curPos);

	float rotTargetDelta = (targetPos - _zoomOutPos).Size() / _zoomTotalDistance;
	auto targetRot = FQuat::Slerp(_zoomOutRotation, _zoomInRotation, rotTargetDelta);

	_deltaZoom = 1.f - FMath::Clamp<float>((_curPos - targetPos).Size() / _zoomForce, 0.0f, 1.0f);
	if (_deltaZoom == 1.f) return;

	_startPos = _curPos;
	_targetPos = targetPos;

	_startRot = _curRot;
	_targetRot = targetRot; // _zoomInRotation;

	_isZoomStart = true;
	_isZoomEnd = false;
}

void AGsActorBossScene::ZoomOut()
{
	if (_isZoomStart && _zoomType == Zoom::Out) return;

	_zoomType = Zoom::Out;

	FVector targetPos = _curPos + _zoomDirection * -_zoomForce;
	targetPos = targetPos.BoundToBox(_curPos, _zoomOutPos);

	float rotTargetDelta = (targetPos - _zoomInPos).Size() / _zoomTotalDistance;
	auto targetRot = FQuat::Slerp(_zoomInRotation, _zoomOutRotation, rotTargetDelta);

	_deltaZoom = 1.f - FMath::Clamp<float>((_curPos - targetPos).Size() / _zoomForce, 0.0f, 1.0f);
	if (_deltaZoom == 1.f) return;

	_startPos = _curPos;
	_targetPos = targetPos;

	_startRot = _curRot;
	_targetRot = targetRot;

	_isZoomStart = true;
	_isZoomEnd = false;
}

void AGsActorBossScene::OnTouchPress(ETouchIndex::Type FingerIndex, FVector Location)
{
	if (FingerIndex == ETouchIndex::Type::Touch1)
		_touchPressLocation[0] = Location;
	else if (FingerIndex == ETouchIndex::Type::Touch2)
		_touchPressLocation[1] = Location;

	if (_touchPressLocation[0] != FVector::ZeroVector &&
		_touchPressLocation[1] != FVector::ZeroVector)
	{
		_touchStartDist = (_touchPressLocation[1] - _touchPressLocation[0]).Size2D();
	}
}

void AGsActorBossScene::OnTouchRelease(ETouchIndex::Type FingerIndex, FVector Location)
{
	_preTouchPos = FVector::ZeroVector;

	if (FingerIndex == ETouchIndex::Type::Touch1)
		_touchPressLocation[0] = FVector::ZeroVector;
	else if (FingerIndex == ETouchIndex::Type::Touch2) //release의 Touch2는 press의 touch2 겠지??
		_touchPressLocation[1] = FVector::ZeroVector;

	if (_touchPressLocation[0] == FVector::ZeroVector ||
		_touchPressLocation[1] == FVector::ZeroVector)
	{
		_touchStartDist = 0;
		_isZoomEnd = true;
	}
}

void AGsActorBossScene::OnTouchMove(ETouchIndex::Type FingerIndex, FVector Location)
{
	_preTouchPos = _touchPressLocation[0] == FVector::ZeroVector ? _touchPressLocation[1] : _touchPressLocation[0];

	if (ETouchIndex::Touch1 == FingerIndex)
	{
		_touchPressLocation[0] = Location;
	}
	else if (ETouchIndex::Touch2 == FingerIndex)
	{
		_touchPressLocation[1] = Location;
	}

	if (_touchStartDist <= 0)
	{
		if (_touchPressLocation[0] != FVector::ZeroVector &&
			_touchPressLocation[1] != FVector::ZeroVector)
		{
			_touchStartDist = (_touchPressLocation[1] - _touchPressLocation[0]).Size2D();
		}
		else if (_mannequin)
		{
			FVector dir = Location - _preTouchPos;

			if (dir.Equals(FVector::ZeroVector))
				return;

			_preTouchPos = Location;
			float yaw = FMath::Clamp(dir.X * _touchMoveFactor, -_touchMoveLimit, _touchMoveLimit);

			FRotator rotate = _cameraRotator->GetRelativeRotation();
			rotate.Add(0, yaw, 0);

			_cameraRotator->SetRelativeRotation(rotate.Quaternion());
		}
		return;
	}

	auto touchMoveDist = (_touchPressLocation[1] - _touchPressLocation[0]).Size2D();
	auto touchOffset = touchMoveDist - _touchStartDist;

	if (touchOffset == 0) return;

	_touchStartDist = touchMoveDist;
	_zoomType = touchOffset > 0 ? Zoom::In : Zoom::Out;
	auto zoomForceDist = touchOffset * _touchZoomFactor; //touchOffset minus

	FVector targetPos = _curPos + _zoomDirection * zoomForceDist;

	if (_zoomType == Zoom::In)
		targetPos = targetPos.BoundToBox(_zoomInPos, _curPos);
	else targetPos = targetPos.BoundToBox(_curPos, _zoomOutPos);

	FQuat targetRot{ FQuat::Identity };

	if (_zoomType == Zoom::In)
	{
		float rotTargetDelta = (targetPos - _zoomOutPos).Size() / _zoomTotalDistance;
		targetRot = FQuat::Slerp(_zoomOutRotation, _zoomInRotation, rotTargetDelta);
	}
	else
	{
		float rotTargetDelta = (targetPos - _zoomInPos).Size() / _zoomTotalDistance;
		targetRot = FQuat::Slerp(_zoomInRotation, _zoomOutRotation, rotTargetDelta);
	}

	_deltaZoom = 1.f - FMath::Clamp<float>((_curPos - targetPos).Size() / FMath::Abs(zoomForceDist), 0.0f, 1.0f);
	if (_deltaZoom == 1.f) return;

	_startPos = _curPos;
	_targetPos = targetPos;

	_startRot = _curRot;
	_targetRot = targetRot;

	_isZoomStart = true;
	_isZoomEnd = false;
}

void AGsActorBossScene::InvalidZoomPos()
{
	float factor = _currentMaxDistance / 10 * 2;

	_bgDome->SetWorldScale3D(FVector(factor, factor, factor));

	_zoomInPos = -_currentMinDistance * _zoomDirection;
	_zoomOutPos = _curPos = -_currentMaxDistance * _zoomDirection;

	_zoomOutRotation = _curRot = _cameraZoomOutPos->GetRelativeRotation().Quaternion();
	_zoomInRotation = _cameraZoomInPos->GetRelativeRotation().Quaternion();

	_zoomTotalDistance = (_zoomInPos - _zoomOutPos).Size();

	if (_sceneCaptureComp)
	{
		_sceneCaptureComp->SetRelativeLocation(_zoomOutPos);
		_sceneCaptureComp->SetRelativeRotation(_zoomOutRotation);
	}

	_cameraRotator->SetRelativeLocation(FVector{ _root->GetRelativeLocation().X,  _root->GetRelativeLocation().Y, 0.f });
	_cameraRotator->SetRelativeRotation(_initQuat);

#if BOSS_SCENE
	GSLOG(Log, TEXT("BossScene - invalid currentMinDistance : %f\t currentMaxDistance : % f"), _currentMinDistance, _currentMaxDistance);
	GSLOG(Log, TEXT("BossScene - invalid zoom out pos : %s\t zoom in pos : % s"), *_zoomOutPos.ToString(), *_zoomInPos.ToString());
#endif
}

void AGsActorBossScene::SetZoomValue(float speed, float maxDist, float touchMulti)
{
	_zoomSpeed = speed == 0.f ? GData()->GetGlobalData()->_fairyZoomSpeed : speed;
	_zoomForce = maxDist == 0.f ? GData()->GetGlobalData()->_fairyZoomDistance : maxDist;
	_touchZoomFactor = touchMulti == 0.f ? GData()->GetGlobalData()->_fairyTouchZoomFactor : touchMulti;
}
void AGsActorBossScene::SetRotateValue(float touchDistMul, float touchLimit)
{
	_touchMoveFactor = touchDistMul == 0.f ? GData()->GetGlobalData()->_fairyRotateMoveFactor : touchDistMul;
	_touchMoveLimit = touchLimit == 0.f ? GData()->GetGlobalData()->_fairyRotateMoveLimit : touchLimit;
}

void AGsActorBossScene::SetBossSceneMode(EGsBossSceneMode inBossCeneMode)
{
	switch (inBossCeneMode)
	{
	case EGsBossSceneMode::BossInfo:
		_bgDome->SetHiddenInGame(false);
		_bgGuilddungoenDome->SetHiddenInGame(true);
		break;
	case EGsBossSceneMode::GuildDungeon:
		_bgDome->SetHiddenInGame(true);
		_bgGuilddungoenDome->SetHiddenInGame(false);
		break;
	default:
		break;
	}
}
