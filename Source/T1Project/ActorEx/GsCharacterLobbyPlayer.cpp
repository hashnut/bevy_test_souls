// Fill out your copyright notice in the Description page of Project Settings.


#include "GsCharacterLobbyPlayer.h"

#include "Data/GsDataContainManager.h"
#include "Data/GsGlobalConstant.h"

#include "DataSchema/Camera/GsSchemaCustomizeCameraData.h"
#include "DataSchema/Camera/GsSchemaContentsCameraData.h"

#include "Camera/Lobby/GsCameraHandlerLobby.h"
#include "Camera/LookAt/GsCameraLookAtHandlerLobby.h"

#include "Message/GsMessageSystem.h"
#include "GsWeaponActor.h"

#include "Management/ScopeLobby/GsLobbyCharacterManager.h"

#include "Engine/Engine.h"

#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/PlayerController.h"

#include "Components/SkeletalMeshComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/InputComponent.h"
#include "Engine/Classes/Camera/CameraComponent.h"

#include "Materials/Material.h"
#include "Materials/MaterialInterface.h"

// Called when the game starts or when spawned
void AGsCharacterLobbyPlayer::BeginPlay()
{
	Super::BeginPlay();

	// 카메라 줌 강도 값 저장하기
	_touchZoomFactorMobile = GData()->GetGlobalData()->LobbyCamZoomPowerMobile;
	_touchZoomFactorPC = GData()->GetGlobalData()->LobbyCamZoomPowerPC;
	_pitchRotMul = GData()->GetGlobalData()->InputPitchRotMul;

	_touchMoveFactorMobile = GData()->GetGlobalData()->LobbyCamMoveFactorMobile;
	_touchMoveFactorPC = GData()->GetGlobalData()->LobbyCamMoveFactorPC;

	if (GEngine && GEngine->GameViewport)
	{
		FVector2D viewPortVec;
		GEngine->GameViewport->GetViewportSize(viewPortVec);
		if (viewPortVec != FVector2D::ZeroVector)
		{
			_rotPitchAspectRate = viewPortVec.Y / viewPortVec.X;
		}
	}

	
	if (nullptr == _cameraHandler)
	{
		_cameraHandler = new FGsCameraHandlerLobby();
	}
	_cameraHandler->Initialize(this);
	

	if (nullptr == Controller)
		return;

	APlayerController* pc = CastChecked<APlayerController>(Controller);
	if (nullptr != pc->PlayerCameraManager)
	{
		pc->PlayerCameraManager->ViewPitchMin = GData()->GetGlobalData()->PitchRotMin;
		pc->PlayerCameraManager->ViewPitchMax = GData()->GetGlobalData()->PitchRotMax;
	}

	

	// Weapon 관련 PrimitiveComponent가 동적 생성으로 변경됨에 따라 
	// Lobby쪽 PrimitiveComponent 관련 설정에 활성화 하는 방향으로 논의됨
	SetupPrimitives(true);
}

// Called to bind functionality to input
void AGsCharacterLobbyPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Touch Common
	PlayerInputComponent->BindTouch(EInputEvent::IE_Pressed, this, &AGsCharacterLobbyPlayer::OnTouchPress);
	PlayerInputComponent->BindTouch(EInputEvent::IE_Released, this, &AGsCharacterLobbyPlayer::OnTouchRelease);
	PlayerInputComponent->BindTouch(EInputEvent::IE_Repeat, this, &AGsCharacterLobbyPlayer::OnTouchMove);

	// Camera
	PlayerInputComponent->BindAction(TEXT("ZoomIn"), EInputEvent::IE_Pressed, this, &AGsCharacterLobbyPlayer::OnZoomIn);
	PlayerInputComponent->BindAction(TEXT("ZoomOut"), EInputEvent::IE_Pressed, this, &AGsCharacterLobbyPlayer::OnZoomOut);
}

void AGsCharacterLobbyPlayer::OnTouchPress(ETouchIndex::Type FingerIndex, FVector Location)
{
	_bTouchPressed = true;
	_prevTouchLocation = Location;

	
	// 들어온걸로 저장
	if (ETouchIndex::Touch1 == FingerIndex)
	{
		_prevTouchLoc1 = Location;
	}
	else if (ETouchIndex::Touch2 == FingerIndex)
	{
		_prevTouchLoc2 = Location;
	}

	UpdateZoomMode();

	if (_onTouchPressDelegate.IsBound())
	{
		_onTouchPressDelegate.Broadcast(Location);
	}
}

void AGsCharacterLobbyPlayer::OnTouchRelease(ETouchIndex::Type FingerIndex, FVector Location)
{

	_bTouchPressed = false;
	_prevTouchLocation = FVector::ZeroVector;

	// 땐거만 초기화
	if (ETouchIndex::Touch1 == FingerIndex)
	{
		_prevTouchLoc1 = FVector::ZeroVector;
	}
	else if (ETouchIndex::Touch2 == FingerIndex)
	{
		_prevTouchLoc2 = FVector::ZeroVector;
	}

	UpdateZoomMode();

	// 이전 거리는 무조건 초기화
	_prevDist = 0.0f;
}

void AGsCharacterLobbyPlayer::OnTouchMove(ETouchIndex::Type FingerIndex, FVector Location)
{
	if (false == _bTouchPressed)
	{
		return;
	}

#ifdef SHOW_LOG
	//GSLOG(Error, TEXT("OnTouchMove yaw: %f, pitch: %f"), yaw, pitch);
#endif

	//GSLOG(Warning, TEXT("OnTouchMove"));

	// 터치 줌인, 아웃 처리
	if (ETouchIndex::Touch1 == FingerIndex)
	{
		_prevTouchLoc1 = Location;
	}
	else if (ETouchIndex::Touch2 == FingerIndex)
	{
		_prevTouchLoc2 = Location;
	}

	UpdateZoomMode();

	// 줌모드 아닐때만 회전 처리
	if (_isZoomMode == false)
	{
		FVector dir = Location - _prevTouchLocation;
		_prevTouchLocation = Location;

		float moveFactor = GetTouchMoveFactor();

		/*float yaw = FMath::Clamp(dir.X * moveFactor, GetTouchMoveLimitMin(), GetTouchMoveLimitMax());
		float pitch = FMath::Clamp(dir.Y * moveFactor, GetTouchMoveLimitMin(), GetTouchMoveLimitMax());*/

		float yaw = dir.X * moveFactor;
		float pitch = dir.Y * moveFactor;

		pitch *= (_rotPitchAspectRate * _pitchRotMul);
		// pitch 값은 apect rate 에서 
		// y/x 값이 되어야 함(즉 비율만큼 크기)
		OnMoveRotateYaw(yaw);
		OnMoveRotatePitch(pitch);

		// 둘중 하나라도 값이 zero면
		// 둘다 닿지 않았음...
		// 나감

		return;
	}


	// 1,2 거리 값
	FVector distLoc = _prevTouchLoc1 - _prevTouchLoc2;

	// 새로운 dist 거리
	// SizeSquare2D 쓰면 값이 너무 커짐...
	float newDist = distLoc.Size2D();
	// 이전 터치가 없었으면(이번이 두손가락 동시 첫터치면)
	// 나감
	if (_prevDist == 0.0f)
	{
		// 백업
		_prevDist = newDist;
		return;
	}

	// 이전과 차이(손가락 사이 거리 변화값)
	// 양수면 줌인
	// 음수면 줌아웃
	float diffDist = newDist - _prevDist;

	// 백업
	_prevDist = newDist;

	// 줌 강도
	float zoomPower = FMath::Abs(diffDist) * _touchZoomFactorMobile;

	// 바뀐게 없음
	// 나감
	if (diffDist == 0.0f)
	{
		return;
	}
	// 줌인
	else if (diffDist > 0.0f)
	{
		ZoomIn(zoomPower);
	}
	// 줌아웃
	else
	{		
		ZoomOut(zoomPower);
	}
}
void AGsCharacterLobbyPlayer::UpdateZoomMode()
{
	// 두개 동시에 입력이 있어야 줌모드임...
	if (_prevTouchLoc1 != FVector::ZeroVector
		&& _prevTouchLoc2 != FVector::ZeroVector)
	{
		_isZoomMode = true;
	}
	else
	{
		_isZoomMode = false;
	}
}

void AGsCharacterLobbyPlayer::OnMoveRotateYaw(float Value)
{
	if (Value == 0.0f)
	{
		return;
	}

	FRotator nowRot = GetActorRotation();
	nowRot.Yaw -= Value;

	SetActorRotation(nowRot);
}

void AGsCharacterLobbyPlayer::OnMoveRotatePitch(float Value)
{	
	AddControllerPitchInput(Value);
}

void AGsCharacterLobbyPlayer::OnZoomIn()
{
	float zoomPower = GetZoomPower();
	ZoomIn(zoomPower);
}
void AGsCharacterLobbyPlayer::OnZoomOut()
{
	float zoomPower = GetZoomPower();
	ZoomOut(zoomPower);
}
// 줌 강도 가져오기
float AGsCharacterLobbyPlayer::GetZoomPower()
{
	return _touchZoomFactorPC;
}
void AGsCharacterLobbyPlayer::ZoomIn(float In_val)
{
	if (nullptr != _cameraHandler)
	{
		_cameraHandler->OnZoomChange(-In_val);
	}
}
void AGsCharacterLobbyPlayer::ZoomOut(float In_val)
{
	if (nullptr != _cameraHandler)
	{
		_cameraHandler->OnZoomChange(In_val);
	}
}

void AGsCharacterLobbyPlayer::Tick(float In_delta)
{
	Super::Tick(In_delta);

	if (nullptr != _cameraHandler)
	{
		_cameraHandler->Update(In_delta);
	}
}

void AGsCharacterLobbyPlayer::EndPlay(const EEndPlayReason::Type in_type)
{
	Super::EndPlay(in_type);

	if (nullptr != _cameraHandler)
	{
		_cameraHandler->Finalize();
		delete _cameraHandler;
		_cameraHandler = nullptr;
	}

	_onTouchPressDelegate.Clear();
}

// 스프링 암 다른데 붙인다(키 조절 카메라 움직임 이슈)
void AGsCharacterLobbyPlayer::AttachSpringArmTarget(AActor* In_target)
{
	if(nullptr == In_target )
	{ 
		return;
	}

	GetSpringArm()->AttachToComponent(In_target->GetRootComponent(), 
		FAttachmentTransformRules::KeepRelativeTransform);
}


void AGsCharacterLobbyPlayer::InitState(float In_zoomRate)
{
	// 카메라 캐릭터에 붙을걸로 교체
	APlayerController* playerController = Cast<APlayerController>(GetController());
	if (nullptr != playerController)
	{
		playerController->PlayerCameraManager->SetViewTarget(this);
	}
	else
	{
		return;
	}

	// 줌 초기화
	if (_cameraHandler != nullptr)
	{
		_cameraHandler->InitZoom(In_zoomRate);
	}

}

void AGsCharacterLobbyPlayer::InitWidgetComponent()
{
	// 위젯을 생성하지 않기 위해 오버라이딩. Super 함수 호출하지 말 것
}

class UStaticMeshComponent* AGsCharacterLobbyPlayer::GetWeaponMesh()
{
	if (AGsWeaponActor* weaponActor = GetWeapon())
	{
		return weaponActor->GetStaticMeshComponent();
	}
	
	return nullptr;
}

class UStaticMeshComponent* AGsCharacterLobbyPlayer::GetWeaponSubMesh()
{
	if (AGsWeaponActor* weaponActor = GetWeaponSub())
	{
		return weaponActor->GetStaticMeshComponent();
	}

	return nullptr;
}

class UMaterial* AGsCharacterLobbyPlayer::GetWeaponMaterial()
{
	if (UStaticMeshComponent* mesh = GetWeaponMesh())
	{
		if (UMaterialInterface* materialInterface = mesh->GetMaterial(0))
		{
			return materialInterface->GetMaterial();
		}
	}

	return nullptr;
}

class UMaterial* AGsCharacterLobbyPlayer::GetWeaponSubMaterial()
{
	if (UStaticMeshComponent* mesh = GetWeaponSubMesh())
	{
		if (UMaterialInterface* materialInterface = mesh->GetMaterial(0))
		{
			return materialInterface->GetMaterial();
		}
	}

	return nullptr;
}
float AGsCharacterLobbyPlayer::GetTouchMoveFactor()
{
	float moveFactor = 0.0f;
	// PC라면
#if WITH_EDITOR || PLATFORM_WINDOWS	
	moveFactor = _touchMoveFactorPC;
#else
	moveFactor = _touchMoveFactorMobile;
#endif
	return moveFactor;
}