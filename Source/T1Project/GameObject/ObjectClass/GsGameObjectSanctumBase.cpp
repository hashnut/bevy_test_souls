// Fill out your copyright notice in the Description page of Project Settings.


#include "GameObject/ObjectClass/GsGameObjectSanctumBase.h"
#include "GameObject/Define/GsGameObjectStruct.h"
#include "GameObject/Abnormality/GsAbnormalityHandlerNonPlayer.h"
#include "GameObject/Data/GsGameObjectDataCreature.h"
#include "Components/CapsuleComponent.h"
#include "ActorEx/GsCampBase.h"
#include "DataSchema/Sanctum/GsSchemaSanctumConfig.h"
#include "DataSchema/Sanctum/GsSchemaSanctumData.h"
#include "DataSchema/Sanctum/GsSchemaSanctumNexusData.h"
#include "Management/ScopeGame/GsSanctumManager.h"
#include "UTIL/GsTableUtil.h"
#include "T1Project.h"


void UGsGameObjectSanctumBase::Initialize(const FGsSpawnParam* SpawnParam)
{
	Super::Initialize(SpawnParam);
	SET_FLAG_TYPE(_objectType, UGsGameObjectSanctumBase::GetObjectType());

	_interactionMessageParam._objectType = GetObjectType();
	_interactionMessageParam._targetGameID = GetGameId();

	_spawnReasonType = SPAWN_REASON::NORMAL;
	if (SpawnParam)
	{
		_spawnRot = SpawnParam->_rot;
	}

	CreateSubClass();
}

void UGsGameObjectSanctumBase::OnIFFChanged()
{
	Super::OnIFFChanged();

	SetInteractionActive(_isInteractionActive);
}

TSharedPtr<FGsMovementHandlerBase> UGsGameObjectSanctumBase::CreateMovement()
{
	// 이동 불가 오브젝트라 생성하지 않음
	return nullptr;
}

TSharedPtr<FGsAbnormalityHandlerBase> UGsGameObjectSanctumBase::CreateAbnormality()
{
	return MakeShareable(new FGsAbnormalityHandlerNonPlayer());
}

bool UGsGameObjectSanctumBase::GetIsActiveInteraction()
{
	// 액터 로드 전엔 무조건 false
	if (AActor* actor = GetActor())
	{
		return _isInteractionActive;
	}

	return false;
}

void UGsGameObjectSanctumBase::SetCapsuleAndTracePos(AGsCampBase* InActorCamp, const FVector& InSpawnPos, 
	bool bCapsuleActive, const FGsSchemaCreatureCommonInfo* InCreatureCommonInfo)
{	
	if (nullptr == InActorCamp)
	{
		return;
	}

	// 중력 제거
	InActorCamp->ActiveGravity(false);

	if (InCreatureCommonInfo)
	{
		float radius = (0 < InCreatureCommonInfo->cylinderRadius) ? InCreatureCommonInfo->cylinderRadius : DEFAULT_CAPSULE_RADIUS;
		float height = (0 < InCreatureCommonInfo->cylinderHeight) ? InCreatureCommonInfo->cylinderHeight : DEFAULT_CAPSULE_HEIGHT;

		if (UCapsuleComponent* capsule = InActorCamp->GetCapsuleComponent())
		{
			capsule->SetEnableGravity(false);
			capsule->SetCapsuleSize(radius, height);

			if (false == bCapsuleActive)
			{
				capsule->Deactivate();
			}

			// GameObjectData도 갱신
			if (FGsGameObjectDataCreature* data = GetCastData<FGsGameObjectDataCreature>())
			{
				data->SetScaledCapsuleRadius(capsule->GetScaledCapsuleRadius());
				data->SetScaledCapsuleHalfHeight(capsule->GetScaledCapsuleHalfHeight());
				data->SetApproachDistanceSquared(capsule->GetScaledCapsuleRadius() * capsule->GetScaledCapsuleRadius());
			}
		}
	}	

	FVector location = InActorCamp->GetActorLocation();

	float resultZ = location.Z;
	FHitResult hit;
	FCollisionQueryParams CollisionParams;
	const bool bHit = GetWorld()->LineTraceSingleByChannel(hit, InActorCamp->GetActorLocation(),
		InSpawnPos - FVector(0.f, 0.f, 10000.f), ECC_Visibility, CollisionParams);
	if (bHit)
	{
		resultZ = hit.Location.Z;
	}

	FVector originVec;
	FVector extendVec;
	InActorCamp->GetActorBounds(true, originVec, extendVec);
	InActorCamp->SetNavigationAddOffsetValue(FVector(0.f, 0.f, extendVec.Z));

	FVector tracePos = FVector(location.X, location.Y, resultZ);
	InActorCamp->SetActorLocation(tracePos);
}

int32 UGsGameObjectSanctumBase::GetInteractionRange() const
{
	if (_isInteractionActive)
	{
		if (const FGsSchemaSanctumNexusData* nexusData = FGsSanctumManager::GetTableSanctumNexusDataBySanctumId(GetSanctumId()))
		{
			return nexusData->interactionRange;
		}
	}
	else
	{
		if (const FGsSchemaNpcData* npcData = GetNpcData())
		{
			return npcData->interactionRange;
		}
	}

	return 0;
}
