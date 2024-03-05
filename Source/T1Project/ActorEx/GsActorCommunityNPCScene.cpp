// Fill out your copyright notice in the Description page of Project Settings.

#include "GsActorCommunityNPCScene.h"
#include "Animation/GsCommunityGirlAnimInstanceState.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/SceneCaptureComponent2D.h"
#include "Camera/CameraComponent.h"
#include "GsCharacterLocalPlayer.h"
#include "GsCharacterNonPlayer.h"
#include "ControllerEx/GsPlayerController.h"
#include "ControllerEx/GsSimplePlayerController.h"
#include "Message/GsMessageContents.h"
#include "Message/MessageParam/GsUIMessageParam.h"
#include "Management/GsMessageHolder.h"
#include "Management/ScopeGlobal/GsLevelManager.h"
#include "Management/ScopeGlobal/GsPathManager.h"
#include "Private/KismetTraceUtils.h"
#include "BPFunction/UI/GsBlueprintFunctionLibraryUI.h"
#include "Public/Blueprint/WidgetLayoutLibrary.h"
#include "UMG/Public/Blueprint/SlateBlueprintLibrary.h"

// Sets default values
AGsActorCommunityNPCScene::AGsActorCommunityNPCScene()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootSceneObject"));

	_characterRoot = CreateDefaultSubobject<USceneComponent>(TEXT("CharacterRoot"));
	_characterRoot->SetupAttachment(RootComponent);

	_mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
	if (_mesh)
	{
		_mesh->SetupAttachment(_characterRoot);

		_meshFace = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("MeshFace"));
		_meshFace->SetupAttachment(_mesh);
		_meshFace->SetMasterPoseComponent(_mesh);
	}

	_cameraRotator = CreateDefaultSubobject<USceneComponent>(TEXT("CameraRoot"));
	_cameraRotator->SetupAttachment(RootComponent);

	_camera = CreateDefaultSubobject<UCameraComponent>(TEXT("SceneCamera"));
	_camera->SetupAttachment(_cameraRotator);
	//_captureCamera = CreateDefaultSubobject<USceneCaptureComponent2D>(TEXT("SceneCaptureCamera"));
	//_captureCamera->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AGsActorCommunityNPCScene::BeginPlay()
{
	Super::BeginPlay();	

	if (auto animState = Cast<UGsCommunityGirlAnimInstanceState>(_mesh->GetAnimInstance()))
	{
		animState->_cbStartAction = [](EGsStateBase state) {

			FGsUIMsgParamInt param; param._data = static_cast<int32>(state);
			GMessage()->GetCommunity().SendMessage(MessageContentCommunity::CHANGE_ACTION, &param);
		};
	}
}

// Called every frame
void AGsActorCommunityNPCScene::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AGsActorCommunityNPCScene::InitAction()
{
	if (auto animState = Cast<UGsCommunityGirlAnimInstanceState>(_mesh->GetAnimInstance()))
	{
		animState->ChangeState(StaticCast<int>(EGsStateBase::Spawn));
	}

	InputComponent = NewObject<UInputComponent>(this);
	if (InputComponent)
	{
		if (APlayerController* controller = GetWorld()->GetFirstPlayerController())
		{
			InputComponent->BindTouch(EInputEvent::IE_Pressed, this, &AGsActorCommunityNPCScene::OnTouchPress);
			InputComponent->BindTouch(EInputEvent::IE_Released, this, &AGsActorCommunityNPCScene::OnTouchRelease);

			EnableInput(controller);
			GLevel()->PushViewTargetWithBlend(_camera->GetOwner());
		}
	}
}

void AGsActorCommunityNPCScene::EndAction()
{
	if (APlayerController* controller = GetWorld()->GetFirstPlayerController())
	{
		if (InputComponent)
		{
			DisableInput(controller);
		}
	}

	GLevel()->PopViewTarget(_camera->GetOwner());
}

void AGsActorCommunityNPCScene::ActionSelectTab()
{
	if (auto animState = Cast<UGsCommunityGirlAnimInstanceState>(_mesh->GetAnimInstance()))
	{
		animState->SelectTabAction();
	}
}

void AGsActorCommunityNPCScene::ActionTouch(const FVector2D& pos) 
{
	if (auto animState = Cast<UGsCommunityGirlAnimInstanceState>(_mesh->GetAnimInstance()))
	{
		animState->TouchAction();

		if (_touchVFXPath.IsValid())
		{
			UClass* vfxClass = GPath()->GetClassByObjectPath(_touchVFXPath);
			if (vfxClass)
			{
				FVector2D absPos;
				USlateBlueprintLibrary::ScreenToWidgetAbsolute(GetWorld(), pos, absPos);

				FGeometry viewportGeometry = UWidgetLayoutLibrary::GetViewportWidgetGeometry(GetWorld());
				FVector2D localPos = viewportGeometry.AbsoluteToLocal(absPos);
				
				UGsBlueprintFunctionLibraryUI::AddVFX(vfxClass, localPos, _touchVFXZOrder);
			}
		}
	}
}

void AGsActorCommunityNPCScene::LoadAnimInstance(const FString& ClassPath)
{
	if (auto animState = _mesh->GetAnimInstance())
	{
		TSoftClassPtr<UObject> classPtr = TSoftClassPtr<UObject>(ClassPath);
		UClass* bpClass = classPtr.Get();
		if (nullptr == bpClass)
		{
			bpClass = classPtr.LoadSynchronous();
		}

		if (bpClass)
		{
			// AnimInstance Class ¼³Á¤
			_mesh->SetAnimInstanceClass(bpClass);
		}
	}
}

void AGsActorCommunityNPCScene::OnTouchPress(ETouchIndex::Type FingerIndex, FVector Location)
{
	if (FingerIndex == ETouchIndex::Type::Touch1)
	{
		_isTouchStart = false;
		if (APlayerController* controller = GetWorld()->GetFirstPlayerController())
		{
			FVector2D screenPos = FVector2D(Location);

			FHitResult HitResult;
			for (auto channel : { ECC_GameTraceChannel11,
								  ECC_GameTraceChannel12,
								  ECC_GameTraceChannel13,
								  ECC_GameTraceChannel14, })
			{
				if (controller->GetHitResultAtScreenPosition(screenPos, channel, false, HitResult))
				{
					_isTouchStart = true;
					break;
				}
			}
		}
	}
}

void AGsActorCommunityNPCScene::OnTouchRelease(ETouchIndex::Type FingerIndex, FVector Location)
{
	if (FingerIndex == ETouchIndex::Type::Touch1 && _isTouchStart)
	{
		if (APlayerController* controller = GetWorld()->GetFirstPlayerController())
		{
			FVector2D screenPos = FVector2D(Location);

			bool isHit = false;
			FHitResult HitResult;
			for (auto channel : { ECC_GameTraceChannel11,
								  ECC_GameTraceChannel12,
								  ECC_GameTraceChannel13,
								  ECC_GameTraceChannel14, })
			{
				isHit = controller->GetHitResultAtScreenPosition(screenPos, channel, false, HitResult);
				if (isHit) break;
			}
			if (isHit)
			{
				ActionTouch(screenPos);
			}
		}
		_isTouchStart = false;
	}
}