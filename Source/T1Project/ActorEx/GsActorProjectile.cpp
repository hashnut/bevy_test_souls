// Fill out your copyright notice in the Description page of Project Settings.


#include "GsActorProjectile.h"
#include "Engine/World.h"
#include "Engine/Public/TimerManager.h"
#include "Engine/Classes/Particles/ParticleSystemComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "ActorComponentEx/GsSkillGuideEffectComponent.h"
#include "Runtime/Engine/Classes/Components/SphereComponent.h"

// 확인 검증이 끝나면 삭제
#include "T1Project.h"
#include "T1Project/GameMode/GsGameModeBase.h"
#include "Sound/GsSchemaSoundResData.h"
#include "Sound/GsSoundPlayer.h"
#include "Management/ScopeGlobal/GsSoundManager.h"

// Sets default values
AGsActorProjectile::AGsActorProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	_projectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));

	//_sphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));

	_modelRoot = CreateDefaultSubobject<USceneComponent>(TEXT("ModelRoot"));
	_modelRoot->SetupAttachment(RootComponent);

	// GarbageCollection 관련 설정
	// 활성화시키면, Actor Clustering에 병합되며 레벨 언로드시 모두 파괴된다
	bCanBeInCluster = true;
}

//[Todo]
//현재는 임시 프로퍼티값 설정
//추후 타입에 맞는 데이터 구조체를 드리븐 받아 설정하든 에디터에서 설정하든 정해야될듯
void AGsActorProjectile::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	// 중력 무시함
	_projectileMovement->ProjectileGravityScale = 0.f;
	// 초기 속도값 설정 없음
	_projectileMovement->InitialSpeed = 0.f;

	InitializeGuideData();
}

void AGsActorProjectile::InitializeGuideData()
{
	// 예시선 설정 확인
	const FGsSchemaSkillCollision* collistionSet = _guideData.overrideCollisionInfo.GetRow();
	if (nullptr == collistionSet)
	{
		return;
	}

	if (collistionSet->type != SkillCollisionType::NONE)
	{
		// guideEffect 컴퍼넌트 생성
		_guideEffectComponent = NewObject<UGsSkillGuideEffectComponent>(this, TEXT("GuideEffect"));
		_guideEffectComponent->SetupAttachment(RootComponent);
		_guideEffectComponent->RegisterComponent();

		// 모델 리스트 찾기
		_modelRoot->GetChildrenComponents(true, _listMesh);
		for (USceneComponent* el : _listMesh)
		{
			// 파티클 시스템 찾기
			if (el->IsA(UParticleSystemComponent::StaticClass()))
			{
				UParticleSystemComponent* particleComponent = Cast<UParticleSystemComponent>(el);
				particleComponent->DeactivateImmediate();
			}
			// 일반 메시일 경우 hide
			else
			{
				el->SetVisibility(false);
			}
		}

		// 타이머 처리
		GetWorld()->GetTimerManager().SetTimer(_delayTimer,
			this, &AGsActorProjectile::OnGuideEffectTimer,
			FMath::Max(_guideData.duration, 0.1f), false);
	}
}

void AGsActorProjectile::PlayParticle(FGsProjectileParticleInfo& ParticleInfo)
{
	ParticleInfo._playParticleComponent = UGameplayStatics::SpawnEmitterAttached(ParticleInfo._particle, _modelRoot, NAME_None,
		ParticleInfo._transform.GetLocation(), ParticleInfo._transform.Rotator(), ParticleInfo._transform.GetScale3D(),
		EAttachLocation::KeepRelativeOffset, true, EPSCPoolMethod::AutoRelease, true );

	if (false == FMath::IsNearlyZero(ParticleInfo._despawnDelay))
	{
		if (ParticleInfo._timer.IsValid())
		{
			GetWorld()->GetTimerManager().ClearTimer(ParticleInfo._timer);
		}

		// 디스폰 딜레이 타이머 설정	
		GetWorld()->GetTimerManager().SetTimer(ParticleInfo._timer, FTimerDelegate::CreateWeakLambda(this, [&ParticleInfo, this]() {
			DeactiveParticle(ParticleInfo);
			}), ParticleInfo._despawnDelay, false);
	}
}

void AGsActorProjectile::PlayGuideEffect()
{
	if (_guideEffectComponent)
	{
		if (const FGsSchemaSkillCollision* collistionSet = _guideData.overrideCollisionInfo.GetRow())
		{
			_guideEffectComponent->Set(*collistionSet, _guideData.effectPath, _guideData.duration);
		}
	}
}

void AGsActorProjectile::DeactiveParticle(FGsProjectileParticleInfo& ParticleInfo)
{
	if (ParticleInfo._playParticleComponent)
	{
		ParticleInfo._playParticleComponent->DeactivateImmediate();
	}
}

// Called when the game starts or when spawned
void AGsActorProjectile::BeginPlay()
{
	Super::BeginPlay();

	_isNetDespawn = false;

	// 사용자 설정 파티클 List 처리
	for (FGsProjectileParticleInfo& el : _effectData._particlePlayInfo)
	{
		if (el._delay > 0.f)
		{
			// 딜레이 타이머 설정			
			GetWorld()->GetTimerManager().SetTimer(el._timer, FTimerDelegate::CreateWeakLambda(this, [&el, this]() {
				PlayParticle(el);
			}), el._delay, false);
		}
		else
		{
			PlayParticle(el);
		}
	}

	const FGsSchemaSoundResData* schemaSoundResData = _effectData._soundRes.GetRow();

	// 예시선 정보가 없을 경우에는 기본 사운드 출력 있을경우는 예시선 사운드 출력
	if (const FGsSchemaSkillCollision* collistionSet = _guideData.overrideCollisionInfo.GetRow())
	{
		if (collistionSet->type != SkillCollisionType::NONE)
		{
			schemaSoundResData = _effectData._skillGuideSoundRes.GetRow();
		}
	}

	if (schemaSoundResData != nullptr)
	{
		if (UGsSoundPlayer* soundPlayer = GSoundPlayer())
		{
			soundPlayer->PlaySoundTarget(schemaSoundResData, GetRootComponent());
		}
	}

	// 예시선 이펙트 출력
	PlayGuideEffect();
}

void AGsActorProjectile::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (_delayTimer.IsValid())
	{
		GetWorld()->GetTimerManager().ClearTimer(_delayTimer);
	}

	for (FGsProjectileParticleInfo& el : _effectData._particlePlayInfo)
	{
		if (el._timer.IsValid())
		{
			GetWorld()->GetTimerManager().ClearTimer(el._timer);
		}
		if (el._playParticleComponent)
		{
			el._playParticleComponent->DeactivateImmediate();
		}
	}
	_effectData._particlePlayInfo.Empty();

	Super::EndPlay(EndPlayReason);
}

void AGsActorProjectile::FinishDestroy()
{	
	// 개발 확인용 서버 에서 패킷을 받지 못하고 Actor만 자체 소멸된경우
	if (UWorld* world = GetWorld())
	{
		if (AGsGameModeBase* gameMode = world->GetAuthGameMode<AGsGameModeBase>())
		{
			if (gameMode->GetGameModeType() == EGsGameModeType::World && false == _isNetDespawn)
			{
				GSLOG(Error, TEXT("Projectile Actor Despawn to Client, UGsGameObjectProjectile not delete"));
			}
		}
	}

	OnActorDelete.Broadcast(this);

	Super::FinishDestroy();
}

// Called every frame
void AGsActorProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AGsActorProjectile::PlayHitEffect()
{
	if (_effectData._hitParticle.Num() > 0)
	{
		FVector loc = GetActorLocation();
		for (UParticleSystem* psc : _effectData._hitParticle)
		{
			UGameplayStatics::SpawnEmitterAtLocation(
				GetWorld(), psc,
				loc, FRotator::ZeroRotator,	FVector::OneVector,
				true, EPSCPoolMethod::AutoRelease
			);
		}
	}

	// 사운드 재생
	const FGsSchemaSoundResData* schemaSoundResData = _effectData._hitSoundRes.GetRow();
	if (schemaSoundResData != nullptr)
	{
		if (UGsSoundPlayer* soundPlayer = GSoundPlayer())
		{
			soundPlayer->PlaySoundTarget(schemaSoundResData, GetRootComponent());
		}
	}
}

void AGsActorProjectile::SetProjectileSpeed(float Speed)
{	
	// 리미트 속도 맞춤
	_projectileMovement->MaxSpeed = Speed;
	// velocity 설정
	_projectileMovement->SetVelocityInLocalSpace(FVector::ForwardVector * Speed);
}

void AGsActorProjectile::OnGuideEffectTimer()
{
	for (USceneComponent* el : _listMesh)
	{
		if (el->IsA(UParticleSystemComponent::StaticClass()))
		{
			UParticleSystemComponent* particleComponent = Cast<UParticleSystemComponent>(el);
			particleComponent->Activate(true);
		}
		else
		{
			el->SetVisibility(true);
		}

		GetWorld()->GetTimerManager().ClearTimer(_delayTimer);
	}

	// 사운드 출력
	const FGsSchemaSoundResData* schemaSoundResData = _effectData._soundRes.GetRow();
	if (schemaSoundResData != nullptr)
	{
		if (UGsSoundPlayer* soundPlayer = GSoundPlayer())
		{
			soundPlayer->PlaySoundTarget(schemaSoundResData, GetRootComponent());
		}
	}
}