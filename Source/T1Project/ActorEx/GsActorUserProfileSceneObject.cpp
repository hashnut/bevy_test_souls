// Fill out your copyright notice in the Description page of Project Settings.

#include "GsActorUserProfileSceneObject.h"
#include "ActorEx/GsActorFieldFairy.h"
#include "ActorEx/GsCharacterPlayer.h"
#include "ActorEx/GsWeaponActor.h"
#include "Animation/GsCostumeAnimInstanceState.h"
#include "Camera/CameraComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Data/GsDataContainManager.h"
#include "Data/GsGlobalConstant.h"
#include "DataSchema/Costume/GsSchemaCostumeResPath.h"
#include "DataSchema/User/GsSchemaUserData.h"
#include "DataSchema/Item/GsSchemaItemEquipDetail.h"
#include "Fairy/GsFairyData.h"
#include "GameObject/Customize/GsCustomizeHandler.h"
#include "GameObject/Customize/GsCustomizeFunc.h"
#include "GameFramework/PlayerController.h"
#include "Item/GsItemManager.h"
#include "Management/ScopeGame/GsGameObjectManager.h"
#include "Management/ScopeGame/GsUserProfileManager.h"
#include "Management/ScopeGame/GsFairyManager.h"
#include "Management/ScopeGlobal/GsLevelManager.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"


AGsActorUserProfileSceneObject::AGsActorUserProfileSceneObject()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootSceneObject"));

	_characterRoot = CreateDefaultSubobject<USceneComponent>(TEXT("CharacterAttachRoot"));
	_characterRoot->SetupAttachment(RootComponent);

	_fairyRoot = CreateDefaultSubobject<USceneComponent>(TEXT("FaityAttachRoot"));
	_fairyRoot->SetupAttachment(RootComponent);

	_cameraPos = CreateDefaultSubobject<USceneComponent>(TEXT("CameraPos"));
	_cameraPos->SetupAttachment(RootComponent);

	_cameraRotator = CreateDefaultSubobject<USceneComponent>(TEXT("CameraRoot"));
	_cameraRotator->SetupAttachment(RootComponent);

	_camera = CreateDefaultSubobject<UCameraComponent>(TEXT("FairySceneCamera"));
	_camera->SetupAttachment(_cameraRotator);
}

void AGsActorUserProfileSceneObject::BeginPlay()
{
	Super::BeginPlay();

	_touchMoveFactor = GData()->GetGlobalData()->_fairyRotateMoveFactor;
	_touchMoveLimit = GData()->GetGlobalData()->_fairyRotateMoveLimit;

	_camera->SetRelativeLocation(_cameraPos->GetRelativeLocation());
	_camera->SetRelativeRotation(_cameraPos->GetRelativeRotation().Quaternion());

	_initQuat = _characterRoot->GetRelativeRotation().Quaternion();

	_cameraRotator->SetRelativeRotation(_initQuat);
}

void AGsActorUserProfileSceneObject::EndPlay(EEndPlayReason::Type Reason)
{
	EndAction();
	Super::EndPlay(Reason);
}

void AGsActorUserProfileSceneObject::OnTouchPress(ETouchIndex::Type FingerIndex, FVector Location)
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

void AGsActorUserProfileSceneObject::OnTouchRelease(ETouchIndex::Type FingerIndex, FVector Location)
{
	_preTouchPos = FVector::ZeroVector;

	if (FingerIndex == ETouchIndex::Type::Touch1)
		_touchPressLocation[0] = FVector::ZeroVector;
	else if (FingerIndex == ETouchIndex::Type::Touch2)
		_touchPressLocation[1] = FVector::ZeroVector;

	if (_touchPressLocation[0] == FVector::ZeroVector ||
		_touchPressLocation[1] == FVector::ZeroVector)
	{
		_touchStartDist = 0;
	}
}

void AGsActorUserProfileSceneObject::OnTouchMove(ETouchIndex::Type FingerIndex, FVector Location)
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
		else if (_character)
		{
			FVector dir = Location - _preTouchPos;

			if (dir.Equals(FVector::ZeroVector))
				return;

			_preTouchPos = Location;
			float yaw = FMath::Clamp(dir.X * _touchMoveFactor, -_touchMoveLimit, _touchMoveLimit);

			/*FRotator rotate = _cameraRotator->GetRelativeRotation();
			rotate.Add(0, yaw, 0);

			_cameraRotator->SetRelativeRotation(rotate.Quaternion());*/
			FRotator rotate = _characterRoot->GetRelativeRotation();
			rotate.Add(0, -yaw, 0);

			_characterRoot->SetRelativeRotation(rotate.Quaternion());
		}
		return;
	}

	auto touchMoveDist = (_touchPressLocation[1] - _touchPressLocation[0]).Size2D();
	auto touchOffset = touchMoveDist - _touchStartDist;

	if (touchOffset == 0) return;

	_touchStartDist = touchMoveDist;
}

void AGsActorUserProfileSceneObject::LoadAnimInstance(const FString& ClassPath, AGsCharacterPlayer* manekin)
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

void AGsActorUserProfileSceneObject::InitAction()
{
	SetCharacter();
	SetFairy();

	InputComponent = NewObject<UInputComponent>(this);
	if (InputComponent)
	{
		if (APlayerController* controller = GetWorld()->GetFirstPlayerController())
		{
			InputComponent->BindTouch(EInputEvent::IE_Pressed, this, &AGsActorUserProfileSceneObject::OnTouchPress);
			InputComponent->BindTouch(EInputEvent::IE_Released, this, &AGsActorUserProfileSceneObject::OnTouchRelease);
			InputComponent->BindTouch(EInputEvent::IE_Repeat, this, &AGsActorUserProfileSceneObject::OnTouchMove);

			EnableInput(controller);
			GLevel()->PushViewTargetWithBlend(_camera->GetOwner());
		}

		_runObject = true;
	}
}

void AGsActorUserProfileSceneObject::EndAction()
{
	if (!_runObject) return;

	DestroyCharacter();
	DestroyFairy();

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

void AGsActorUserProfileSceneObject::SetCharacter()
{
	DestroyCharacter();

	CreatureGenderType genderType = GSUserProfile()->GetGenderType();
	const UserLookInfo* userLookInfo = GSUserProfile()->GetUserLookInfo();
	const CustomizeFaceData* faceData = GSUserProfile()->GetCustomizeFaceData();
	const CustomizeBodyData* bodyData = GSUserProfile()->GetCustomizeBodyData();

	if (nullptr == userLookInfo || nullptr == faceData || nullptr == bodyData)
		return;

	const FGsSchemaCostumeResPath* costumePlayer = UGsTableUtil::FindRowById<UGsTableCostumeResPath, FGsSchemaCostumeResPath>(genderType);
	if (nullptr == costumePlayer)
		return;

	FString path = costumePlayer->objectBPPath.ToString() + TEXT("_C");
	UClass* loadCharacterC = LoadObject<UClass>(nullptr, *path);
	if (nullptr == loadCharacterC)
		return;

	// 캐릭터 스폰
	FActorSpawnParameters spawnParam = FActorSpawnParameters();
	spawnParam.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	AGsCharacterPlayer* manekin = GetWorld()->SpawnActor<AGsCharacterPlayer>(loadCharacterC, FVector::ZeroVector, FRotator::ZeroRotator, spawnParam);

	// 캐릭터 외형 설정
	_customizeHandler = MakeShared<FGsCustomizeHandler>();
	_customizeHandler->SetActor(manekin);
	_customizeHandler->SetGenderType(genderType);
	_customizeHandler->SetCustomizeDataBonScale(*faceData, *bodyData);
	_customizeHandler->SetCostume(userLookInfo->mTransformCostumeId, userLookInfo->mCostumeId, userLookInfo->mViewHelmet, true, true);

	FGsCustomizeFunc::AttachWeapon(_customizeHandler.Get(), userLookInfo, (int32)genderType, false);

	/*int32 weaponModelPath = 0;
	CreatureWeaponType weaponType = CreatureWeaponType::HAND;
	Level weaponLevel = userLookInfo->mWeaponLevel;

	const FGsSchemaItemEquipDetail* weapon = UGsItemManager::GetEquipItemTableDataByTID(userLookInfo->mWeapon);
	if (nullptr != weapon)
	{
		weaponType = weapon->weaponType;
		weaponModelPath = weapon->modelPath;
	}
	_customizeHandler->SetWeaponChange(weaponModelPath, weaponType, weaponLevel);

	const FGsSchemaUserData* userData = GetUserData(genderType);
	if (nullptr != userData)
	{
		_customizeHandler->ChangeWeaponSocket(userData, weaponType, false);
	}*/

	if (AGsWeaponActor* weaponActor = manekin->GetWeapon())
	{
		weaponActor->ChangeLightingChannels(false, true);
	}
	if (AGsWeaponActor* subWeaponActor = manekin->GetWeaponSub())
	{
		subWeaponActor->ChangeLightingChannels(false, true);
	}

	// Transform 설정
	FVector loc = FVector::ZeroVector;
	loc.Z = manekin->GetDefaultHalfHeight();
	_characterRoot->SetRelativeRotation(_initQuat);
	manekin->AttachToComponent(_characterRoot, FAttachmentTransformRules::KeepRelativeTransform);
	manekin->SetActorRelativeLocation(loc);
	manekin->SetActorRelativeRotation(FRotator::ZeroRotator);
	manekin->SetActorHiddenInGame(false);

	if (auto body = manekin->GetPartsSkeletalComponentBODY())
	{
		body->SetHiddenInGame(false);
	}

	_character = manekin;

	PlayCharacterAnim();
}

void AGsActorUserProfileSceneObject::SetFairy()
{
	DestroyFairy();

	const UserLookInfo* userLookInfo = GSUserProfile()->GetUserLookInfo();
	if (nullptr == userLookInfo)
		return;

	auto fairyData = GSFairy()->GetFairyData(userLookInfo->mFairy).Pin();
	if (false == fairyData.IsValid())
		return;

	FString path = fairyData->GetFairySpawnObjectPath().ToString() + TEXT("_C");
	UClass* loadFairyC = LoadObject<UClass>(nullptr, *path);
	if (nullptr == loadFairyC)
		return;

	FActorSpawnParameters spawnParam = FActorSpawnParameters();
	spawnParam.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	_fairy = GetWorld()->SpawnActor<AGsActorFieldFairy>(loadFairyC, FVector::ZeroVector, FRotator::ZeroRotator, spawnParam);

	if (_fairy && IsValid(_fairy))
	{
		_fairy->AttachToComponent(_fairyRoot, FAttachmentTransformRules::KeepRelativeTransform);
		_fairy->SetActorRelativeScale3D(FVector(1.f, 1.f, 1.f));
		_fairy->SetActorRelativeLocation(FVector::ZeroVector);
		_fairy->ChangeLightingChannels(false, true);
		_fairy->SetIsWaiting(true);
		_fairy->SetSelected();

		_fairy->SetBaseEffect(fairyData->GetFairyBaseEffect(), nullptr, nullptr);
	}
}

void AGsActorUserProfileSceneObject::PlayCharacterAnim()
{
	/*if (auto animState = Cast<UGsCostumeAnimInstanceState>(_character->GetAnim()))
	{
		animState->ChangeCostumeState();
	}*/
}

void AGsActorUserProfileSceneObject::DestroyCharacter()
{
	if (_character)
	{
		if (auto world = _character->GetWorld())
			world->DestroyActor(_character);

		_character = nullptr;
	}
}

void AGsActorUserProfileSceneObject::DestroyFairy()
{
	if (_fairy)
	{
		_fairy->SetDespawn(true);
		_fairy = nullptr;
	}
}


