// Fill out your copyright notice in the Description page of Project Settings.


#include "GsActorCostumeSceneObject.h"

#include "Components/SkeletalMeshComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/PlayerController.h"
#include "Engine/Classes/Components/CapsuleComponent.h"
#include "Engine/Public/DrawDebugHelpers.h"

#include "Data/GsGlobalConstant.h"
#include "Data/GsDataContainManager.h"

#include "Item/GsSchemaItemEquipDetail.h"
#include "Item/GsItemManager.h"

#include "Management/ScopeGame/GsGameObjectManager.h"
#include "Management/ScopeGlobal/GsLevelManager.h"
#include "GameObject/ObjectClass/GsGameObjectLocalPlayer.h"
#include "Gameobject/Data/GsGameObjectDataLocalPlayer.h"

#include "DataSchema/GsSchemaEnums.h"
#include "DataCenter/Public/DataSchema/Camera/GsSchemaContentsCameraData.h"
#include "DataSchema/Customize/GsSchemaCustomizeColorPalette.h"
#include "DataSchema/GameObject/GsSchemaPartsShapeInfo.h"
#include "DataSchema/User/GsSchemaUserData.h"
#include "DataSchema/Costume/GsSchemaCostumeResPath.h"

#include "GsCharacterLocalPlayer.h"
#include "ControllerEx/GsPlayerController.h"
#include "ControllerEx/GsSimplePlayerController.h"
#include "GsSpawn.h"
#include "GameObject/Parts/GsPartsHandlerBase.h"
#include "GameObject/Customize/GsCustomizeHandler.h"
#include "GameObject/Customize/GsCustomizeFunc.h"
#include "Costume/GsCostumeData.h"
#include "Animation/GsCostumeAnimInstanceState.h"
#include "ActorEx/GsWeaponActor.h"

#include "UTIL/GsTableUtil.h"

// Sets default values
AGsActorCostumeSceneObject::AGsActorCostumeSceneObject()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootSceneObject"));

	_characterRoot = CreateDefaultSubobject<USceneComponent>(TEXT("CharacterAttachRoot"));
	_characterRoot->SetupAttachment(RootComponent);

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
void AGsActorCostumeSceneObject::BeginPlay()
{
	Super::BeginPlay();

	if (const FGsSchemaContentsCameraData* findTbl =
		UGsTableUtil::FindRowById<UGsTableContentsCameraData, FGsSchemaContentsCameraData>(EGsContentsCameraType::GameCostumeRoom))
	{
		_cameraAddOffset = findTbl->CamearPositionOffset;
		_camera->SetFieldOfView(findTbl->FOV);
	}

	_isCurPositionInit = false;
	SetCameraInitialize();

	_curPos = _cameraZoomOutPos->GetRelativeLocation() +_cameraAddOffset;
	_curRot = _cameraZoomOutPos->GetRelativeRotation().Quaternion();

	_camera->SetRelativeLocation(_zoomOutPos);
	_camera->SetRelativeRotation(_zoomOutRotation);

	_zoomSpeed = GData()->GetGlobalData()->_fairyZoomSpeed;	//최대 이동 거리를 움직이는 sec
	_zoomForce = GData()->GetGlobalData()->_fairyZoomDistance; //최대 이동 거리
	_touchZoomFactor = GData()->GetGlobalData()->_fairyTouchZoomFactor;

	_touchMoveFactor = GData()->GetGlobalData()->_fairyRotateMoveFactor; //페어리 회전시 터치 이동거리 조절값(값이 작아질 수록 움직임이 작아짐)
	_touchMoveLimit = GData()->GetGlobalData()->_fairyRotateMoveLimit;

	_initQuat = _characterRoot->GetRelativeRotation().Quaternion();

	FRotator initRot = _initQuat.Rotator();

	// init pitch value setting
	initRot.Pitch = GData()->GetGlobalData()->_roomContentsInitPitch;

	_initQuat = initRot.Quaternion();

	FVector charRootRelativeLocation = _characterRoot->GetRelativeLocation();
	_cameraRotator->SetRelativeRotation(_initQuat);

	_pitchMin = GData()->GetGlobalData()->_roomContentsPitchMin;
	_pitchMax = GData()->GetGlobalData()->_roomContentsPitchMax;

	_zoomRate = 1.0f;

	_zoomAddValPC = GData()->GetGlobalData()->_roomContentsZoomAddValPC;
	_zoomMulValMobile = GData()->GetGlobalData()->_roomContentsZoomMulValMobile;
}

void AGsActorCostumeSceneObject::EndPlay(const EEndPlayReason::Type Reason)
{	
	EndAction();
	Super::EndPlay(Reason);
}

// Called every frame
void AGsActorCostumeSceneObject::Tick(float DeltaTime)
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

	FVector charRootPos = _characterRoot->GetComponentLocation();
	DrawDebugSphere(world, charRootPos,
		10.0f, 8, FColor::Red, false, 1.0);

#endif
}

const FName BASE_CHARATER_CENTER = TEXT("Bip001");
const FName BASE_CHARACTER_HEIGHT = TEXT("Bip001-Neck");

void AGsActorCostumeSceneObject::InitAction()
{
	_runObject = true;
	_isPressed = false;
	InputComponent = NewObject<UInputComponent>(this);

	if (UGsGameObjectLocalPlayer* player = Cast<UGsGameObjectLocalPlayer>(GSGameObject()->FindObject(EGsGameObjectType::LocalPlayer)))
	{
		UWorld* world = player->GetActor()->GetWorld();
		AGsCharacterLocalPlayer* playerActor = Cast<AGsCharacterLocalPlayer>(player->GetActor());
		FGsGameObjectDataLocalPlayer* LocalData = player->GetCastData<FGsGameObjectDataLocalPlayer>();
		if (LocalData && playerActor)
		{
			if (const FGsSchemaCostumeResPath* costumePlayer = UGsTableUtil::FindRowById<UGsTableCostumeResPath, FGsSchemaCostumeResPath>(
				LocalData->GetCreatureGenderType()))
			{
				FString path = (*costumePlayer).objectBPPath.ToString() + TEXT("_C");
				if (UClass* loadCharacterC = LoadObject<UClass>(nullptr, *path))
				{	
					FActorSpawnParameters spawnParam = FActorSpawnParameters();
					spawnParam.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

					auto manekin = world->SpawnActor<AGsCharacterPlayer>(loadCharacterC, FVector::ZeroVector, FRotator::ZeroRotator, spawnParam);

					//FVector loc = FVector::ZeroVector;
					_halfManekinHeight = playerActor->GetDefaultHalfHeight();
					//loc.Z += _halfManekinHeight;
					manekin->AttachToComponent(_characterRoot, FAttachmentTransformRules::KeepRelativeTransform);
					manekin->SetActorRelativeRotation(FRotator::ZeroRotator);
					manekin->SetActorRelativeLocation(FVector::ZeroVector);
					manekin->SetActorHiddenInGame(false);

					if (auto body = manekin->GetPartsSkeletalComponentBODY())
						body->SetHiddenInGame(false);

					_customizeHandler = MakeShared<FGsCustomizeHandler>();
					_customizeHandler->InitializeActor(manekin);

					manekin->OnCostumeEffectLoadComplate.AddDynamic(this, &AGsActorCostumeSceneObject::OnCostumeEffectLoadComplate);

					if (auto lookInfo = LocalData->GetUserLookInfo())
					{
						_selectAppearance.costumeId = lookInfo->mTransformCostumeId;
						_selectAppearance.isAppliedHair = lookInfo->mTransformCostumeId != INVALID_COSTUME_ID; // && (lookInfo->mTransformCostumeType == HAIR || lookInfo->mTransformCostumeType == FULLSET);
						_selectAppearance.isAppliedBody = lookInfo->mTransformCostumeId != INVALID_COSTUME_ID; // && (lookInfo->mTransformCostumeType == BODY || lookInfo->mTransformCostumeType == FULLSET);

						_selectCostumeId = lookInfo->mCostumeId;
						_isHairOn = lookInfo->mViewHelmet;
						FGsCustomizeFunc::SetCustomizeByLocalData(_customizeHandler.Get(), LocalData, _selectCostumeId, _isHairOn, true, true, true);
					}
					if (AGsWeaponActor* weaponActor = manekin->GetWeapon())
					{
						weaponActor->ChangeLightingChannels(false, true);
					}
					if (AGsWeaponActor* subWeaponActor = manekin->GetWeaponSub())
					{
						subWeaponActor->ChangeLightingChannels(false, true);
					}					

					_manekin = manekin;

					auto maneingBasePos = _manekin->GetMesh()->GetBoneLocation(BASE_CHARACTER_HEIGHT, EBoneSpaces::ComponentSpace);
					_baseManekinHeight = maneingBasePos.Z;

					OnSpawnCharacter(_baseManekinHeight);
					SetCameraInitialize();

					_isCurPositionInit = false;
					CostumeActorPostCameraModify(_baseManekinHeight);
					
				}
			}
		}
	}

	if (InputComponent)
	{
		if (APlayerController* controller = GetWorld()->GetFirstPlayerController())
		{
			InputComponent->BindAction(TEXT("ZoomIn"), EInputEvent::IE_Pressed, this, &AGsActorCostumeSceneObject::ZoomIn);
			InputComponent->BindAction(TEXT("ZoomOut"), EInputEvent::IE_Pressed, this, &AGsActorCostumeSceneObject::ZoomOut);

			InputComponent->BindTouch(EInputEvent::IE_Pressed, this, &AGsActorCostumeSceneObject::OnTouchPress);
			InputComponent->BindTouch(EInputEvent::IE_Released, this, &AGsActorCostumeSceneObject::OnTouchRelease);
			InputComponent->BindTouch(EInputEvent::IE_Repeat, this, &AGsActorCostumeSceneObject::OnTouchMove);

			EnableInput(controller);
			GLevel()->PushViewTargetWithBlend(_camera->GetOwner());
		}

		_runObject = true;
	}
}

void AGsActorCostumeSceneObject::EndAction()
{
	if (!_runObject) return;

	if (_manekin)
	{
		if (auto world = _manekin->GetWorld())
			world->DestroyActor(_manekin);

		_manekin = nullptr;
	}

	if (APlayerController* controller = GetWorld()->GetFirstPlayerController())
	{
		if (InputComponent)
		{
			DisableInput(controller);
		}
	}

	GLevel()->PopViewTarget(_camera->GetOwner());

	_customizeHandler.Reset();
	_runObject = false;
}

void AGsActorCostumeSceneObject::SetCameraInitialize()
{
	_characterRoot->SetRelativeLocation(FVector::ZeroVector);

	_zoomOutPos = _cameraZoomOutPos->GetRelativeLocation() +_cameraAddOffset;
	_zoomInPos = _cameraZoomInPos->GetRelativeLocation() +_cameraAddOffset;

	_zoomOutRotation = _cameraZoomOutPos->GetRelativeRotation().Quaternion();
	_zoomInRotation = _cameraZoomInPos->GetRelativeRotation().Quaternion();

	_zoomDirection = (_zoomInPos - _zoomOutPos).GetSafeNormal();
	_zoomTotalDistance = (_zoomInPos - _zoomOutPos).Size();
}

void AGsActorCostumeSceneObject::CostumeActorPostCameraModify(float delta)
{
	if (_isCurPositionInit == false)
	{
		_curPos = _zoomOutPos;
		_camera->SetRelativeLocation(_curPos);
		_characterRoot->AddRelativeLocation(FVector(0, 0, delta));

		//FRotator Rotator = UKismetMathLibrary::FindLookAtRotation(_zoomOutPos, _characterRoot->GetRelativeLocation());
		//_camera->SetRelativeRotation(Rotator);
		//_cameraZoomOutPos->SetRelativeRotation(Rotator);

		_isCurPositionInit = true;
	}
}

void AGsActorCostumeSceneObject::ZoomIn()
{
	if (_isZoomStart && _zoomType == Zoom::In) return;

	_zoomType = Zoom::In;

#ifdef OLD_COSTUME_CODE
	FVector targetPos = _curPos + _zoomDirection * _zoomForce;
	targetPos = targetPos.BoundToBox(_zoomInPos, _curPos);
#else
	_zoomRate = FMath::Clamp(_zoomRate - _zoomAddValPC, 0.0f, 1.0f);
	FVector targetPos = FMath::Lerp(_zoomInPos, _zoomOutPos, _zoomRate);
#endif

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

void AGsActorCostumeSceneObject::ZoomOut()
{
	if (_isZoomStart && _zoomType == Zoom::Out) return;

	_zoomType = Zoom::Out;
#ifdef OLD_COSTUME_CODE
	FVector targetPos = _curPos + _zoomDirection * -_zoomForce;
	targetPos = targetPos.BoundToBox(_curPos, _zoomOutPos);
#else
	_zoomRate = FMath::Clamp(_zoomRate + _zoomAddValPC, 0.0f, 1.0f);
	FVector targetPos = FMath::Lerp(_zoomInPos, _zoomOutPos, _zoomRate);
#endif

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

void AGsActorCostumeSceneObject::OnTouchPress(ETouchIndex::Type FingerIndex, FVector Location)
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

	_isPressed = true;
}

void AGsActorCostumeSceneObject::OnTouchRelease(ETouchIndex::Type FingerIndex, FVector Location)
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

void AGsActorCostumeSceneObject::OnTouchMove(ETouchIndex::Type FingerIndex, FVector Location)
{
	if (_isPressed == false) return;
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
		else if (_manekin != nullptr)
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

	if (_zoomType == Zoom::In)
		targetPos = targetPos.BoundToBox(_zoomInPos, _curPos);
	else targetPos = targetPos.BoundToBox(_curPos, _zoomOutPos);
#else
	// zoom in +
	// zoom out -
	float zoomRateAddVal = touchOffset * _zoomMulValMobile;
	_zoomRate = FMath::Clamp(_zoomRate - zoomRateAddVal, 0.0f, 1.0f);
	FVector targetPos = FMath::Lerp(_zoomInPos, _zoomOutPos, _zoomRate);
#endif

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
void AGsActorCostumeSceneObject::OnMoveRotateYaw(float In_yaw)
{
	if (_isPressed == false) return;

	if (_manekin == nullptr)
	{
		return;
	}

	FRotator nowRot = _manekin->GetActorRotation();
	nowRot.Yaw -= In_yaw;

	_manekin->SetActorRotation(nowRot);
}
void AGsActorCostumeSceneObject::OnMoveRotatePitch(float In_pitch)
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
//_customizeHandler는 원래 캐릭터가 가지고 있는 정보를 가질 수 없으므로, 코스츔 전체 핸들링 할 수 없다. 차후 매니저를 통해 적용.
void AGsActorCostumeSceneObject::SetCostume(CostumeId costumeId, bool isHairOn) //customiseHandler
{
	if (_selectCostumeId == costumeId && _isHairOn == isHairOn) return;

	_isHairOn = isHairOn;
	_selectCostumeId = costumeId;
	_customizeHandler->SetCostume(INVALID_COSTUME_ID, costumeId, isHairOn);
	if (_manekin)
	{
		if (auto animState = Cast<UGsCostumeAnimInstanceState>(_manekin->GetAnim()))
		{
			animState->ChangeCostumeState();
		}

		_manekin->SetActorRelativeRotation(FRotator::ZeroRotator);
	}

	// init cam rotation 
	// rotator: pitch rot
	_cameraRotator->SetRelativeRotation(_initQuat);
	// cam: zoom rot
	_camera->SetRelativeRotation(_zoomOutRotation);
	// init zoom
	_zoomRate = 1.0f;
	
	FVector newPos = FVector(_zoomOutPos.X, _zoomOutPos.Y, _zoomOutPos.Z ); //- _shiftZPos
	_camera->SetRelativeLocation(newPos);

	_curPos = _zoomOutPos;
	_curRot = _zoomOutRotation;
}

void AGsActorCostumeSceneObject::SetAppearance(CostumeId appearanceId, bool applyHair, bool applyBody)
{
	if (appearanceId == INVALID_COSTUME_ID && _isHairOn == false) //초기화
	{
		_selectAppearance.isAppliedHair = !applyHair;
		_customizeHandler->SetAppearance(appearanceId, _selectAppearance.isAppliedHair, applyBody);
	}

	if (appearanceId == _selectAppearance.costumeId  && 
		applyHair == _selectAppearance.isAppliedHair && 
		applyBody == _selectAppearance.isAppliedBody) return;

	_selectAppearance.costumeId = appearanceId;
	_selectAppearance.isAppliedHair = applyHair;
	_selectAppearance.isAppliedBody = applyBody;
	_customizeHandler->SetAppearance(appearanceId, applyHair, applyBody);
	if (_manekin)
	{
		if (auto animState = Cast<UGsCostumeAnimInstanceState>(_manekin->GetAnim()))
		{
			animState->ChangeCostumeState();
		}

		_manekin->SetActorRelativeRotation(FRotator::ZeroRotator);
	}
}

void AGsActorCostumeSceneObject::LoadAnimInstance(const FString& ClassPath, AGsCharacterPlayer* manekin)
{
	if (manekin)
	{
		TSoftClassPtr<UObject> classPtr = TSoftClassPtr<UObject>(ClassPath);
		UClass* bpClass = classPtr.Get();
		if (nullptr == bpClass)
		{
			bpClass = classPtr.LoadSynchronous();
		}

		if (bpClass)
		{
			// AnimInstance Class 설정
			USkeletalMeshComponent* meshComponent = manekin->GetMesh();
			meshComponent->SetAnimInstanceClass(bpClass);
		}
	}
}

void AGsActorCostumeSceneObject::OnCostumeEffectLoadComplate(AActor* LoadActor)
{
	// 날개 채널 반영
	if (LoadActor)
	{
		TArray<UMeshComponent*> findMeshComponents;
		LoadActor->GetComponents<UMeshComponent>(findMeshComponents);
		for (UMeshComponent* el : findMeshComponents)
		{
			el->ChangeLightingChannels(false, true, false);
		}
	}
}

void AGsActorCostumeSceneObject::ChangeSubScene(bool start)
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
					InputComponent->BindAction(TEXT("ZoomIn"), EInputEvent::IE_Pressed, this, &AGsActorCostumeSceneObject::ZoomIn);
					InputComponent->BindAction(TEXT("ZoomOut"), EInputEvent::IE_Pressed, this, &AGsActorCostumeSceneObject::ZoomOut);

					InputComponent->BindTouch(EInputEvent::IE_Pressed, this, &AGsActorCostumeSceneObject::OnTouchPress);
					InputComponent->BindTouch(EInputEvent::IE_Released, this, &AGsActorCostumeSceneObject::OnTouchRelease);
					InputComponent->BindTouch(EInputEvent::IE_Repeat, this, &AGsActorCostumeSceneObject::OnTouchMove);

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

//if (AGsPlayerController* playerController =  player->GetActor()->GetPlayerController())
//{
//	playerController->SetIgnoreMoveInput(true);
//	playerController->SetIgnoreLookInput(true);
//}
//
//auto manekin = Cast<AGsCharacterLocalPlayer>(GsSpawn::Clone(player->GetActor(), RootComponent->GetRelativeLocation(), FRotator::ZeroRotator));
//
//FActorSpawnParameters spawnParam = FActorSpawnParameters();
//spawnParam.ObjectFlags |= RF_Transient;
//if (auto playerController = world->SpawnActor<APlayerController>(APlayerController::StaticClass(), spawnParam))
//{
//	manekin->PossessedBy(playerController); //소유를 바꿔준다. 클론은 현재 controller를 공유중. null은 오류가 나고, UnPossessed 하면 player가 control을 잃어버림
//	manekin->UnPossessed(); //input은 마네킹이 아닌 현재 오브젝트가 처리하므로 바로 UnPossessed 한다
//
//	world->DestroyActor(playerController);
//}
//if (const FGsSchemaCostumeAnimPath* costumeAnim = UGsTableUtil::FindRowById<UGsTableCostumeAnimPath, FGsSchemaCostumeAnimPath>(
//	LocalData->GetCreatureGenderType()))
//{
//	FString path = (*costumeAnim).animPath.ToString() + TEXT("_C");
//	LoadAnimInstance(path, manekin);
//}