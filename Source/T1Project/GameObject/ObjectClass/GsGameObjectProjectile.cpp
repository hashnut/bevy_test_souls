// Fill out your copyright notice in the Description page of Project Settings.

#include "GsGameObjectProjectile.h"

#include "ActorEx/GsActorProjectile.h"

#include "DataSchema/GameObject/Projectile/GsSchemaProjectileSet.h"
#include "Management/ScopeGlobal/GsSoundManager.h"
#include "Management/ScopeGame/GsGameObjectManager.h"

#include "GameObject/Movement/GsMovementHandlerProjectile.h"
#include "GameObject/HUD/GsScreenFilterLocalPlayer.h"

#include "UTIL/GsTableUtil.h"
#include "UTIL/GsLevelUtil.h"
#include "DrawDebugHelpers.h"

EGsGameObjectType	UGsGameObjectProjectile::GetObjectType() const  { return EGsGameObjectType::Projectile; }
AActor*				UGsGameObjectProjectile::GetActor() const		{ return GetProjectile(); }
void				UGsGameObjectProjectile::ClearActor()			{ _actor = nullptr; }
AGsActorProjectile* UGsGameObjectProjectile::GetProjectile() const	{ return _actor; }
//FActorDeleteSignature* UGsGameObjectProjectile::GetDeleteSignature() const 
//{ 
//	return GetProjectile() ? &GetProjectile()->OnActorDelete : nullptr; 
//}

UGsGameObjectProjectile::~UGsGameObjectProjectile()
{
}

void UGsGameObjectProjectile::Initialize(const FGsSpawnParam* SpawnParam)
{
	Super::Initialize(SpawnParam);

    SET_FLAG_TYPE(_objectType, UGsGameObjectProjectile::GetObjectType());

	//  데이터 얻기
	_projectileData = UGsTableUtil::FindRowById<UGsTableProjectileSet, FGsSchemaProjectileSet>(_gameObjectData->GetTableId());

	_spawnPos = SpawnParam->_pos;

	//bak1210 : final 에서호출
	CreateSubClass();
}


void UGsGameObjectProjectile::ActorDespawn()
{
	// 트리거 타입 확인
	if (_triggerEvent.IsValid())
	{
		_triggerEvent->Deactive();
		_triggerEvent = nullptr;
	}

	// 모든 SubClass들을 정리하고 난후 _actor 포인터 소멸
	Super::ActorDespawn();
}

void UGsGameObjectProjectile::InitializeActor(AActor* Owner)
{
	//액터 저장
	_actor = Cast<AGsActorProjectile>(Owner);

	Super::InitializeActor(Owner);

	// 스폰시 액터가 없을수 있음..
	SetCasterActor();

	// 스케일 설정
	_actor->SetActorScale3D(FVector(_projectileData->scale));

	// 프로젝타일은 무조건 소멸 시간 설정을 하자
	// 서버 응답에 의해 소멸될것이므로 확인용으로 서버에서의 소멸타이밍 보다 크게 설정
	_actor->SetLifeSpan(FMath::Max(0.1f, _projectileData->lifeTime) * 2.f);

	// 스피드 설정
	_actor->SetProjectileSpeed(_projectileData->speed);	

	// 스폰 위치 조정
	FHitResult hit;
	if (UGsLevelUtil::TrySweepToLand(hit, GetWorld(), _spawnPos, 100.f, 100.f))
	{
		_actor->SetActorLocation(hit.ImpactPoint);

#if SPAWN_DEBUG
		//Npc spawn 보정 위치 
		FString log = FString::Printf(TEXT("Projectile spawn - label : %s\tlocation : %s"), *_actor->GetActorLabel(), *hit.ImpactPoint.ToString());
		DrawDebugSphere(UObject::GetWorld(), hit.ImpactPoint, 50.0f, 8, FColor::Blue, false, 15);
		GSLOG(Log, TEXT("%s"), *log);
#endif
	}
 	

	// 충돌 범위 설정
	//if (USphereComponent* collider = _actor->GetSphereComponent())
	//{		
	//	collider->SetSphereRadius(_projectileData->collisionInfo.radius);
	//}

	// 트리거 타입 확인
	if (_projectileData->screenFilter.screenFilterUse)
	{
		if (_projectileData->collisionInfoList.Num() > 0)
		{
			_triggerEvent = MakeShareable(new FGsTriggerEventSkillCollision());
			_triggerEvent->Active(this, EGsGameObjectType::LocalPlayer);
		}
	}
}

void UGsGameObjectProjectile::CreateSubClass()
{
	Super::CreateSubClass();

	// 이동형 발사체의 경우 FGsMovementHandlerProjectile 생성
	if (_projectileData->moveType == ProjectileMoveType::MOVE_TARGET)
	{
		AppendInitialize(CreateMovement());
		FGsMovementHandlerBase* movehandler = GetMovementHandler();
		movehandler->InitSpeed(_projectileData->speed, _projectileData->speed);
	}
}

TSharedPtr<FGsMovementHandlerBase> UGsGameObjectProjectile::CreateMovement()
{
	return MakeShareable(new FGsMovementHandlerProjectile());
}

void UGsGameObjectProjectile::SetCasterActor(UGsGameObjectBase* Caster)
{
	if (nullptr == GetActor())
	{
		return;
	}

	if (nullptr == Caster)
	{
		Caster = GSGameObject()->FindObject(EGsGameObjectType::Creature, _casterGameId);
	}

	if (nullptr != Caster)
	{
		AActor* projectileActor = GetActor();
		if (AActor* casterActor = Caster->GetActor())
		{
			projectileActor->SetOwner(casterActor);

			// 캐스터는 충돌 대상에서 제외 시킴
			if (UPrimitiveComponent* primitive = Cast<UPrimitiveComponent>(projectileActor->GetRootComponent()))
			{
				primitive->MoveIgnoreActors.AddUnique(Caster->GetActor());
			}

			// 캐스터가 설정된다면 회전 정보를 캐스터 회전 정보로 맞춘다
			projectileActor->SetActorRotation(casterActor->GetActorRotation());
		}
	}
}

void UGsGameObjectProjectile::SetCaster(UGsGameObjectBase* Caster)
{
	if (Caster)
	{
		_casterGameId = Caster->GetGameId();

		SetCasterActor(Caster);

		// 프로젝타일의 IFF타입은 Caster것을 그대로 사용
		GetData()->SetIffFaction(Caster->GetData()->GetIff());
	}
}

void UGsGameObjectProjectile::NetSkillDamagedAck(PD::SC::PKT_SC_SKILL_DAMAGED_READ* Packet)
{
	// 이펙트 연출
	if (_actor)
	{
		_actor->PlayHitEffect();
	}
}

void UGsGameObjectProjectile::ComponentHit(UPrimitiveComponent* HitComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (OtherActor)
	{
		check(GSGameObject());
		UGsGameObjectBase* hitObj = GSGameObject()->FindObject(HitComponent->GetOwner(), EGsGameObjectType::Projectile);
		if (nullptr != hitObj)
		{
			// 피격 알림
			if (UGsGameObjectBase * findObj = GSGameObject()->FindObject(OtherActor, EGsGameObjectType::Creature))
			{
				UGsGameObjectProjectile* projectile = Cast<UGsGameObjectProjectile>(hitObj);
				if (nullptr != projectile)
				{
					//const FGsProjectileData* data = projectile->GetProjectileData();
					//check(data);

					//findObj->OnHit(hitObj, data->Reaction , 1.f);
				}
			}

			// 소멸 이펙트 처리
			//_actor->PlayExpiredEffect();

			// 충돌 객체 성향에 따라 Destory여부 설정하기
			// 현재는 무조건 삭제
			//GSGameObject()->DespawnObject(hitObj);
		}
	}
}

const int64 UGsGameObjectProjectile::GetDmgValue()
{
	//if (nullptr != ProjectileData)
	//{
	//	return ProjectileData->Stat.AttackDmg;
	//}

	return 0;
}

int64 UGsGameObjectProjectile::GetCasterId() const
{
	return _casterGameId;
}