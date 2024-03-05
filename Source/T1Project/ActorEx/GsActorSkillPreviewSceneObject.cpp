// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorEx/GsActorSkillPreviewSceneObject.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/PlayerController.h"
#include "Components/SceneComponent.h"
#include "Management/ScopeGlobal/GsLevelManager.h"

// Sets default values
AGsActorSkillPreviewSceneObject::AGsActorSkillPreviewSceneObject()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SkillPreviewSceneRoot"));

	_dummyOwner = CreateDefaultSubobject<USceneComponent>(TEXT("DummyOwnerPlayer"));
	_dummyOwner->SetupAttachment(RootComponent);

	_dummyPartyPlayer_Left = CreateDefaultSubobject<USceneComponent>(TEXT("DummyPartyPlayer_Left"));
	_dummyPartyPlayer_Left->SetupAttachment(RootComponent);
	_dummyPartyPlayer_Right = CreateDefaultSubobject<USceneComponent>(TEXT("DummyPartyPlayer_Right"));
	_dummyPartyPlayer_Right->SetupAttachment(RootComponent);

	_dummyEnemyPlayer_Center = CreateDefaultSubobject<USceneComponent>(TEXT("DummyEnemyPlayer_Center"));
	_dummyEnemyPlayer_Center->SetupAttachment(RootComponent);
	_dummyEnemyPlayer_Left = CreateDefaultSubobject<USceneComponent>(TEXT("DummyEnemyPlayer_Left"));
	_dummyEnemyPlayer_Left->SetupAttachment(RootComponent);
	_dummyEnemyPlayer_Right = CreateDefaultSubobject<USceneComponent>(TEXT("DummyEnemyPlayer_Right"));
	_dummyEnemyPlayer_Right->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AGsActorSkillPreviewSceneObject::BeginPlay()
{
	Super::BeginPlay();

	// 카메라 설정
	if (nullptr == _camera)
	{
		_camera = FindComponentByClass<UCameraComponent>();
	}
	
	if (APlayerController* controller = GetWorld()->GetFirstPlayerController())
	{
		GLevel()->PushViewTarget(_camera->GetOwner());
	}
}

void AGsActorSkillPreviewSceneObject::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	GLevel()->PopViewTarget(_camera->GetOwner());

	Super::EndPlay(EndPlayReason);
}

// Called every frame
void AGsActorSkillPreviewSceneObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AGsActorSkillPreviewSceneObject::SetFieldOfView_Camera(float Fov)
{
	_camera->SetFieldOfView(Fov);
}

void AGsActorSkillPreviewSceneObject::SetTransform_Camera(const FTransform& Tm)
{
	_camera->SetRelativeTransform(Tm);
}

void AGsActorSkillPreviewSceneObject::SetTransform_Owner(const FTransform& Tm)
{
	_dummyOwner->SetRelativeTransform(Tm);
}

void AGsActorSkillPreviewSceneObject::SetTransform_PartyPlayer_Left(const FTransform& Tm)
{
	_dummyPartyPlayer_Left->SetRelativeTransform(Tm);
}

void AGsActorSkillPreviewSceneObject::SetTransform_PartyPlayer_Right(const FTransform& Tm)
{
	_dummyPartyPlayer_Right->SetRelativeTransform(Tm);
}

void AGsActorSkillPreviewSceneObject::SetTransform_EnemyPlayer_Center(const FTransform& Tm)
{
	_dummyEnemyPlayer_Center->SetRelativeTransform(Tm);
}

void AGsActorSkillPreviewSceneObject::SetTransform_EnemyPlayer_Left(const FTransform& Tm)
{
	_dummyEnemyPlayer_Left->SetRelativeTransform(Tm);
}

void AGsActorSkillPreviewSceneObject::SetTransform_EnemyPlayer_Right(const FTransform& Tm)
{
	_dummyEnemyPlayer_Right->SetRelativeTransform(Tm);
}

