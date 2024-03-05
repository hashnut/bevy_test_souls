// Fill out your copyright notice in the Description page of Project Settings.


#include "GsActorFairyObject.h"
#include "GsActorFieldFairy.h"

#include "Components/SkeletalMeshComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/PlayerController.h"
#include "Engine/Public/DrawDebugHelpers.h"

#include "Data/GsGlobalConstant.h"
#include "Data/GsDataContainManager.h"

#include "DataSchema/GsSchemaEnums.h"
#include "DataCenter/Public/DataSchema/Camera/GsSchemaContentsCameraData.h"

#include "UTIL/GsTableUtil.h"

#include "Management/ScopeGame/GsGameObjectManager.h"
#include "Management/ScopeGlobal/GsLevelManager.h"
#include "GameObject/ObjectClass/GsGameObjectLocalPlayer.h"
#include "Async.h"

const FName BASE_FAIRY_CENTER = TEXT("Bip001");
const FName BASE_FAIRY_HEIGHT = TEXT("Bip001-Neck");

// Sets default values
AGsActorFairyObject::AGsActorFairyObject()
: Super()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootSceneObject"));

	_fairyRoot = CreateDefaultSubobject<USceneComponent>(TEXT("FaityAttachRoot"));
	_fairyRoot->SetupAttachment(RootComponent);

	_cameraZoomInPos = CreateDefaultSubobject<USceneComponent>(TEXT("CameraZoomInPos"));
	_cameraZoomInPos->SetupAttachment(RootComponent);

	_cameraZoomOutPos = CreateDefaultSubobject<USceneComponent>(TEXT("CameraZoomOutPos"));
	_cameraZoomOutPos->SetupAttachment(RootComponent);

	_cameraRotator = CreateDefaultSubobject<USceneComponent>(TEXT("CameraRoot"));
	_cameraRotator->SetupAttachment(RootComponent);

	_camera = CreateDefaultSubobject<UCameraComponent>(TEXT("FairySceneCamera"));
	_camera->SetupAttachment(_cameraRotator);
}

// Called when the game starts or when spawned
void AGsActorFairyObject::BeginPlay()
{
	Super::BeginPlay();

	if (const FGsSchemaContentsCameraData* findTbl =
		UGsTableUtil::FindRowById<UGsTableContentsCameraData, FGsSchemaContentsCameraData>(EGsContentsCameraType::GameFairyRoom))
	{
		_cameraAddOffset = findTbl->CamearPositionOffset;
		_camera->SetFieldOfView(findTbl->FOV);
	}

	_preFairyHeight = 0.f;

	SetCameraInitialize();
	_curPos = _cameraZoomOutPos->GetRelativeLocation() + _cameraAddOffset;
	_curRot = _cameraZoomOutPos->GetRelativeRotation().Quaternion();

	_camera->SetRelativeLocation(_zoomOutPos);
	_camera->SetRelativeRotation(_zoomOutRotation);

	_zoomSpeed = GData()->GetGlobalData()->_fairyZoomSpeed;	//최대 이동 거리를 움직이는 sec
	_zoomForce = GData()->GetGlobalData()->_fairyZoomDistance; //최대 이동 거리
	_touchZoomFactor = GData()->GetGlobalData()->_fairyTouchZoomFactor;

	_touchMoveFactor = GData()->GetGlobalData()->_fairyRotateMoveFactor; //페어리 회전시 터치 이동거리 조절값(값이 작아질 수록 움직임이 작아짐)
	_touchMoveLimit = GData()->GetGlobalData()->_fairyRotateMoveLimit;

	_initQuat = _fairyRoot->GetRelativeRotation().Quaternion();

	FRotator initRot = _initQuat.Rotator();

	// init pitch value setting
	initRot.Pitch = GData()->GetGlobalData()->_roomContentsInitPitch;

	_initQuat = initRot.Quaternion();

	FVector fairyRootRelativeLocation = _fairyRoot->GetRelativeLocation();
	//_cameraRotator->SetRelativeLocation(FVector{ fairyRootRelativeLocation.X,  fairyRootRelativeLocation.Y, fairyRootRelativeLocation.Z }); // + _halfFairySize
	_cameraRotator->SetRelativeRotation(_initQuat);

	FVector2D viewSize;
	GEngine->GameViewport->GetViewportSize(viewSize);

	_pitchMin = GData()->GetGlobalData()->_roomContentsPitchMin;
	_pitchMax = GData()->GetGlobalData()->_roomContentsPitchMax;	

	_zoomRate = 1.0f;

	_zoomAddValPC = GData()->GetGlobalData()->_roomContentsZoomAddValPC;
	_zoomMulValMobile = GData()->GetGlobalData()->_roomContentsZoomMulValMobile;
}

void AGsActorFairyObject::EndPlay(const EEndPlayReason::Type Reason)
{
	EndAction();
	Super::EndPlay(Reason);
}

// Called every frame
void AGsActorFairyObject::Tick(float DeltaTime)
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
		_curPos = FMath::Lerp<FVector>(_startPos, _targetPos, _deltaZoom); // +_cameraEnchantOffset;
		_curRot = FQuat::Slerp(_startRot, _targetRot, _deltaZoom);

		//zShiftCurrPos = FVector{ _curPos.X, _curPos.Y, _curPos.Z - _shiftZPos };

		_camera->SetRelativeLocation(_curPos);
		_camera->SetRelativeRotation(_curRot);

		if (_deltaZoom == 1.f)
		{
			_isZoomEnd = true;
		}
	}

#ifdef TEST_DRAW
	UWorld* world = GetWorld();
	if (world == nullptr ||
		_cameraRotator == nullptr)
	{
		return;
	}
	FVector camRotatorPos = _cameraRotator->GetComponentLocation();
	DrawDebugSphere(world, camRotatorPos,
		10.0f, 8, FColor::Cyan, false, 1.0);

	FVector fairyRootPos = _fairyRoot->GetComponentLocation();
	DrawDebugSphere(world, fairyRootPos,
		10.0f, 8, FColor::Red, false, 1.0);

#endif
}

void AGsActorFairyObject::InitAction()
{
	_runObject = true;
	_isPressed = false;
	InputComponent = NewObject<UInputComponent>(this);

	if (InputComponent)
	{
		if (APlayerController* controller = GetWorld()->GetFirstPlayerController())
		{
			InputComponent->BindAction(TEXT("ZoomIn"), EInputEvent::IE_Pressed, this, &AGsActorFairyObject::ZoomIn);
			InputComponent->BindAction(TEXT("ZoomOut"), EInputEvent::IE_Pressed, this, &AGsActorFairyObject::ZoomOut);

			InputComponent->BindTouch(EInputEvent::IE_Pressed, this, &AGsActorFairyObject::OnTouchPress);
			InputComponent->BindTouch(EInputEvent::IE_Released, this, &AGsActorFairyObject::OnTouchRelease);
			InputComponent->BindTouch(EInputEvent::IE_Repeat, this, &AGsActorFairyObject::OnTouchMove);

			EnableInput(controller);
			GLevel()->PushViewTargetWithBlend(_camera->GetOwner());
		}

		_runObject = true;
	}
}

void AGsActorFairyObject::EndAction()
{
	if (!_runObject) return;

	if (_fairy)
	{
		_fairy->SetDespawn(true);
		_fairy = nullptr;
	}

	if (APlayerController* controller = GetWorld()->GetFirstPlayerController())
	{
		if (InputComponent)
		{
			DisableInput(controller);
		}
	}

	GLevel()->PopViewTarget(_camera->GetOwner());
	_runObject = false;
	_isLoading = false;
	_loadFunction = nullptr;
}

void AGsActorFairyObject::SetCameraInitialize()
{
	_zoomOutPos = _cameraZoomOutPos->GetRelativeLocation() + _cameraAddOffset;
	_zoomInPos = _cameraZoomInPos->GetRelativeLocation() + _cameraAddOffset;

	_zoomOutRotation = _cameraZoomOutPos->GetRelativeRotation().Quaternion();
	_zoomInRotation = _cameraZoomInPos->GetRelativeRotation().Quaternion();

	_zoomDirection = (_zoomInPos - _zoomOutPos).GetSafeNormal();
	_zoomTotalDistance = (_zoomInPos - _zoomOutPos).Size();

//카메라 로테이션은 미리 돌려두지만 페어리 삭제는 생성될 때 한다 아직 앞의 페어리가 안 만들어졌을 수 있다.
	if (_cameraRotator) _cameraRotator->SetRelativeRotation(_initQuat);
	_camera->SetRelativeRotation(_zoomOutRotation);

	_zoomRate = 1.0f;

	FVector newPos = FVector(_zoomOutPos.X + _cameraEnchantOffset.X,
		_zoomOutPos.Y + _cameraEnchantOffset.Y,
		_zoomOutPos.Z + _cameraEnchantOffset.Z); // - _shiftZPos
	_camera->SetRelativeLocation(newPos);

	_curPos = _zoomOutPos + _cameraEnchantOffset;
	_curRot = _zoomOutRotation;


	//auto zMod = _preFairyHeight > 0.f ? _baseFairyHeight - _preFairyHeight : 0;
	//if (_preFairyHeight <= 0.f)
	//{
	//	_camera->SetRelativeLocation(_zoomOutPos);
	//}
	//_camera->AddRelativeLocation(FVector(0, 0, zMod));
	//_curPos += FVector(0, 0, zMod);


	//_fairyRoot->SetRelativeLocation(FVector(0, 0, _baseFairyHeight));
}

void AGsActorFairyObject::SetFairy(FString path, const TArray<FGsSchemaFairyEffect>& baseEffectData, bool isAsync /*= true*/)
{
	if (_isLoading)
	{
		if(_loadPath == path) return;
	}

	_loadPath = path;

	if (path.IsEmpty())
	{
		_isLoading = false;
		OnFairyLoad(nullptr, baseEffectData);
		return;
	}

	_isLoading = true;

	if (isAsync)
	{
		auto thiz = MakeWeakObjectPtr(this);
		GResource()->LoadAsync(FSoftObjectPath(path), 
			FStreamableDelegateParam::CreateLambda([thiz, path, &baseEffectData](UObject* objFairy) {

				if (thiz.IsValid())
				{
					auto isLoading = thiz->_loadPath == path || thiz->_loadPath.IsEmpty() ? false : true; // 다른 페어리가 로딩중
					if (isLoading == false) //다른 것이 로딩중이면 스폰 하지 않아도 된다.
						thiz->OnFairyLoad(Cast<UClass>(objFairy), baseEffectData);
					thiz->_isLoading = isLoading;
				}
			}));
	}
	else
	{
		if (UClass* loadFairyC = LoadObject<UClass>(nullptr, *path))
		{
			OnFairyLoad(loadFairyC, baseEffectData);
		}
		_isLoading = false;
	}
}

void AGsActorFairyObject::ChangeSubScene(bool start)
{
	if (_runObject)
	{
		if (APlayerController* controller = GetWorld()->GetFirstPlayerController())
		{
			if (InputComponent)
			{
				if (start) DisableInput(controller);
				else
				{
					InputComponent->BindAction(TEXT("ZoomIn"), EInputEvent::IE_Pressed, this, &AGsActorFairyObject::ZoomIn);
					InputComponent->BindAction(TEXT("ZoomOut"), EInputEvent::IE_Pressed, this, &AGsActorFairyObject::ZoomOut);

					InputComponent->BindTouch(EInputEvent::IE_Pressed, this, &AGsActorFairyObject::OnTouchPress);
					InputComponent->BindTouch(EInputEvent::IE_Released, this, &AGsActorFairyObject::OnTouchRelease);
					InputComponent->BindTouch(EInputEvent::IE_Repeat, this, &AGsActorFairyObject::OnTouchMove);

					EnableInput(controller);
					GLevel()->PushViewTarget(_camera->GetOwner());
				}
			}
		}

		if (start) 
			 StartSummon();
		else EndSummon();
	}
}

void AGsActorFairyObject::ZoomIn()
{
	if (_touchZoomLock) return;
	if (_isZoomStart && _zoomType == Zoom::In) return;

	_zoomType = Zoom::In;
#ifdef OLD_COSTUME_CODE
	FVector targetPos = _curPos + _zoomDirection * _zoomForce;
	targetPos = targetPos.BoundToBox(_zoomInPos + _cameraEnchantOffset, _curPos);
#else
	_zoomRate = FMath::Clamp(_zoomRate - _zoomAddValPC, 0.0f, 1.0f);
	FVector targetPos = FMath::Lerp(_zoomInPos + _cameraEnchantOffset, _zoomOutPos+ _cameraEnchantOffset, _zoomRate);
#endif


	float rotTargetDelta = (targetPos - (_zoomOutPos + _cameraEnchantOffset)).Size() / _zoomTotalDistance;
	auto targetRot = FQuat::Slerp(_zoomOutRotation, _zoomInRotation, rotTargetDelta);

	_deltaZoom = 1.f - FMath::Clamp<float>((_curPos - targetPos).Size() / _zoomForce, 0.0f, 1.0f);
	if (_deltaZoom == 1.f) return;

	_startPos  = _curPos;
	_targetPos = targetPos;

	_startRot = _curRot;
	_targetRot = targetRot; // _zoomInRotation;

	_isZoomStart = true;
	_isZoomEnd = false;
}

void AGsActorFairyObject::ZoomOut()
{
	if (_touchZoomLock) return;
	if (_isZoomStart && _zoomType == Zoom::Out) return;

	_zoomType = Zoom::Out;
#ifdef OLD_COSTUME_CODE
	FVector targetPos = _curPos + _zoomDirection * -_zoomForce;
	targetPos = targetPos.BoundToBox(_curPos, _zoomOutPos + _cameraEnchantOffset); 
#else
	_zoomRate = FMath::Clamp(_zoomRate + _zoomAddValPC, 0.0f, 1.0f);
	FVector targetPos = FMath::Lerp(_zoomInPos + _cameraEnchantOffset, _zoomOutPos + _cameraEnchantOffset, _zoomRate);

#endif

	float rotTargetDelta = (targetPos - (_zoomInPos + _cameraEnchantOffset)).Size() / _zoomTotalDistance;
	auto targetRot = FQuat::Slerp(_zoomInRotation, _zoomOutRotation, rotTargetDelta);

	_deltaZoom = 1.f - FMath::Clamp<float>((_curPos - targetPos).Size() / _zoomForce, 0.0f, 1.0f);
	if (_deltaZoom == 1.f) return;

	_startPos  = _curPos;
	_targetPos = targetPos;

	_startRot = _curRot;
	_targetRot = targetRot;

	_isZoomStart = true;
	_isZoomEnd = false;
}

void AGsActorFairyObject::OnTouchPress(ETouchIndex::Type FingerIndex, FVector Location)
{
	if (_touchZoomLock) return;

	if (FingerIndex == ETouchIndex::Type::Touch1)
		_touchPressLocation[0] = Location;
	else if (FingerIndex == ETouchIndex::Type::Touch2)
		_touchPressLocation[1] = Location;

	if (_touchPressLocation[0] != FVector::ZeroVector &&
		_touchPressLocation[1] != FVector::ZeroVector)
	{
		_touchStartDist = (_touchPressLocation[1] - _touchPressLocation[0]).Size2D();
	}

	_isPressed = true;
}

void AGsActorFairyObject::OnTouchRelease(ETouchIndex::Type FingerIndex, FVector Location)
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
		_isPressed = false;
	}
}

void AGsActorFairyObject::OnTouchMove(ETouchIndex::Type FingerIndex, FVector Location)
{
	if (_isPressed == false) return;

	if (_touchZoomLock) return;

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
		else if (_fairy != nullptr)
		{
			FVector dir = Location - _preTouchPos;

			if (dir.Equals(FVector::ZeroVector))
				return;

			_preTouchPos = Location;
			float yaw = FMath::Clamp(dir.X * _touchMoveFactor, -_touchMoveLimit, _touchMoveLimit);
			float pitch = FMath::Clamp(dir.Y * _touchMoveFactor, -_touchMoveLimit, _touchMoveLimit);

			OnMoveRotateYaw(yaw);
			OnMoveRotatePitch(pitch);			
		}
		return;
	}
	
	auto touchMoveDist = (_touchPressLocation[1] - _touchPressLocation[0]).Size2D();
	auto touchOffset = touchMoveDist - _touchStartDist;

	if (touchOffset == 0) return;

	_touchStartDist = touchMoveDist;
	_zoomType = touchOffset > 0 ? Zoom::In : Zoom::Out;
	auto zoomForceDist = touchOffset * _touchZoomFactor; //touchOffset minus
#ifdef OLD_COSTUME_CODE
	FVector targetPos = _curPos + _zoomDirection * zoomForceDist;

	if(_zoomType == Zoom::In)
		 targetPos = targetPos.BoundToBox(_zoomInPos + _cameraEnchantOffset, _curPos);
	else targetPos = targetPos.BoundToBox(_curPos, _zoomOutPos + _cameraEnchantOffset);
#else
	// zoom in +
	// zoom out -
	float zoomRateAddVal = touchOffset * _zoomMulValMobile;
	_zoomRate = FMath::Clamp(_zoomRate - zoomRateAddVal, 0.0f, 1.0f);
	FVector targetPos = FMath::Lerp(_zoomInPos + _cameraEnchantOffset, _zoomOutPos + _cameraEnchantOffset, _zoomRate);
#endif

	FQuat targetRot{ FQuat::Identity };

	if (_zoomType == Zoom::In)
	{
		float rotTargetDelta = (targetPos - (_zoomOutPos + _cameraEnchantOffset)).Size() / _zoomTotalDistance;
		targetRot = FQuat::Slerp(_zoomOutRotation, _zoomInRotation, rotTargetDelta);
	}
	else
	{
		float rotTargetDelta = (targetPos - (_zoomInPos + _cameraEnchantOffset)).Size() / _zoomTotalDistance;
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
void AGsActorFairyObject::OnMoveRotateYaw(float In_yaw)
{
	if (_isPressed == false) return;

	if (_fairy == nullptr)
	{
		return;
	}

	FRotator nowRot = _fairy->GetActorRotation();
	nowRot.Yaw -= In_yaw;

	_fairy->SetActorRotation(nowRot);
}
void AGsActorFairyObject::OnMoveRotatePitch(float In_pitch)
{
	if (_isPressed == false) return;

	if (_cameraRotator == nullptr)
	{
		return;
	}

	FRotator rotate = _cameraRotator->GetRelativeRotation();

	float nextPitchVal = rotate.Pitch + In_pitch;
	float verifiedPitch = FMath::Clamp<float>(nextPitchVal, _pitchMin, _pitchMax);

	rotate.Pitch = verifiedPitch;

	_cameraRotator->SetRelativeRotation(rotate.Quaternion());
}

void AGsActorFairyObject::OnFairyLoad(UClass* loadObjectClass, const TArray<FGsSchemaFairyEffect>& baseEffectData)
{
	if (_fairy)
	{
		if (loadObjectClass == nullptr || _isLoading == false)
		{
			_fairy->SetDespawn(true);
			_fairy = nullptr;
		}
		else
		{
			//desawn이 assync하게 진행되는 동안 loadObjectClass가 안전하지 않음
			auto loadPath = _loadPath;
			TWeakObjectPtr<AGsActorFairyObject> thiz = this;
			_loadFunction = [thiz, baseEffectData, loadPath]() {
				if (thiz.IsValid())
				{
					if (auto fairyObject = thiz.Get())
					{
						fairyObject->_fairy = nullptr;
						fairyObject->_isLoading = true;

						if (auto loadFairyC = LoadObject<UClass>(nullptr, *loadPath))
						{
							fairyObject->OnFairyLoad(loadFairyC, baseEffectData);
						}
						fairyObject->_isLoading = false;
					}
				}
			};

			_fairy->SetDespawn(true, FFairyDespawnAnimEnd::CreateWeakLambda(this, [thiz](){
					if (thiz.IsValid() && thiz.Get()->_loadFunction)
					{
						thiz.Get()->_loadFunction();
						thiz.Get()->_loadFunction = nullptr;
					}
				}
			));
			return;
		}
	}

	if (loadObjectClass == nullptr || _isLoading == false || _runObject == false) return;

	UWorld* world = GetWorld();

	FActorSpawnParameters spawnParam = FActorSpawnParameters();
	spawnParam.SpawnCollisionHandlingOverride =
		ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	_fairy = world->SpawnActor<AGsActorFieldFairy>(loadObjectClass, FVector::ZeroVector, FRotator::ZeroRotator, spawnParam);
	if (_fairy && IsValid(_fairy))
	{
		_preFairyHeight = _baseFairyHeight;

		FVector loc = FVector::ZeroVector;
		//auto center = _fairy->GetMesh()->GetBoneLocation(BASE_FAIRY_CENTER, EBoneSpaces::ComponentSpace);
		auto fairyBasePos = _fairy->GetMesh()->GetBoneLocation(BASE_FAIRY_HEIGHT, EBoneSpaces::ComponentSpace);
		_baseFairyHeight = fairyBasePos.Z;

		_fairy->AttachToComponent(_fairyRoot, FAttachmentTransformRules::KeepRelativeTransform);
		_fairy->SetActorRelativeScale3D(FVector(1.f, 1.f, 1.f));
		_fairy->SetActorRelativeLocation(FVector::ZeroVector);
		_fairy->ChangeLightingChannels(false, true);
		_fairy->SetIsWaiting(true);
		_fairy->SetSelected();

		_fairy->SetBaseEffect(baseEffectData, nullptr, nullptr);

		OnSpawnFairy(_baseFairyHeight);
		SetCameraInitialize();
	}
}

void AGsActorFairyObject::SetFairyZoomValue(float speed, float maxDist, float touchMulti)
{
	_zoomSpeed = speed == 0.f ? GData()->GetGlobalData()->_fairyZoomSpeed : speed;
	_zoomForce = maxDist == 0.f ? GData()->GetGlobalData()->_fairyZoomDistance : maxDist;
	_touchZoomFactor = touchMulti == 0.f ? GData()->GetGlobalData()->_fairyTouchZoomFactor : touchMulti;
}
void AGsActorFairyObject::SetFairyRotateValue(float touchDistMul, float touchLimit)
{
	_touchMoveFactor = touchDistMul == 0.f ? GData()->GetGlobalData()->_fairyRotateMoveFactor : touchDistMul;
	_touchMoveLimit = touchLimit == 0.f ? GData()->GetGlobalData()->_fairyRotateMoveLimit : touchLimit;
}

void AGsActorFairyObject::SetZoomOffset(FVector offset)
{
	FVector targetPos = _zoomOutPos + offset;
	_deltaZoom = 1.f - FMath::Clamp<float>((_curPos - targetPos).Size() / _zoomForce, 0.0f, 1.0f);
	if (_deltaZoom == 1.f) return;

	_cameraEnchantOffset = offset;
	_zoomType = Zoom::Out;
	_touchZoomLock = true;

	if (_fairy)
	{
		_fairy->SetActorRelativeScale3D(FVector(1.f, 1.f, 1.f));
		_fairy->SetActorRelativeLocation(FVector::ZeroVector);

		_fairy->SetActorRotation(FRotator::ZeroRotator);
		_cameraRotator->SetRelativeRotation(_initQuat);
	}

	_startPos  = _curPos;
	_targetPos = targetPos;

	_startRot  = _curRot;
	_targetRot = _zoomOutRotation;

	_isZoomStart = true;
	_isZoomEnd = false;
}

void AGsActorFairyObject::ResetZoomOffset()
{
	_cameraEnchantOffset = FVector::ZeroVector;
	FVector targetPos = _zoomOutPos;

	_deltaZoom = 1.f - FMath::Clamp<float>((_curPos - targetPos).Size() / _zoomForce, 0.0f, 1.0f);
	if (_deltaZoom == 1.f) return;

	_zoomType = Zoom::In;
	_touchZoomLock = false;

	_startPos	= _curPos;
	_targetPos	= _zoomOutPos;

	_startRot	= _curRot;
	_targetRot	= _zoomOutRotation; 

	_isZoomStart = true;
	_isZoomEnd = false;

	_zoomRate = 1.0f;
}