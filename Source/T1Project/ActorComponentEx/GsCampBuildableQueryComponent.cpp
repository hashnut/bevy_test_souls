// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorComponentEx/GsCampBuildableQueryComponent.h"

#include "Management/ScopeGame/GsGameObjectManager.h"
#include "Management/ScopeGlobal/GsLevelManager.h"
#include "GameObject/Fence/GsFenceHandler.h"
#include "GameObject/ObjectClass/GsGameObjectLocalPlayer.h"
#include "UTIL/GsLevelUtil.h"
#include "SphereTree/GsSphereTree.h"

#include "DrawDebugHelpers.h"
#include "GameObject/Fence/GsFence.h"


class FGsFenceConverter : public FGsSphereInterface
{
	TSharedPtr<FGsFence> _fence;

public:
	FGsFenceConverter(TSharedPtr<FGsFence> inData)
	{
		if (inData.IsValid() && inData->IsValid())
		{
			_fence = inData;
		}		
	}
	virtual ~FGsFenceConverter() {}

	virtual int GetVertexCount(void) const override
	{
		return (_fence.IsValid()) ? _fence->_fence->vertexList.Num() : 0;
	}

	virtual bool GetVertex(int index, FVector& OUT vect) const override
	{
		if (false == _fence.IsValid()) return false;

		if (GetVertexCount() <= index) return false;

		vect = _fence->_fence->vertexList[index];
		return true;
	}

	bool IsVaild()
	{
		return _fence.IsValid();
	}

	FVector GetCenter()
	{
		return _fence->_fence->Center;
	}

	float GetRadius()
	{
		return _fence->_fence->Radius;
	}
};

class FGsBuildAbleQueryCallback : public FGsSphereNodeCallback
{
	bool m_Result = true;
public:
	virtual ~FGsBuildAbleQueryCallback() {}
public:
	void Reset()
	{
		m_Result = true;
	}

	bool GetQueryResult()
	{
		return m_Result;
	}
public:
	virtual void RangeTestCallback(const FVector& searchpos, float distance, const FGsSphereNode* sphere, GsViewState state) override
	{
		if (state != GsViewState::VS_OUTSIDE)
		{
			m_Result = false;
		}
	};
};


static AActor* BPClass(UWorld* World, UClass* Class, const FVector& SpawnLocation, const FRotator& SpawnRotation, FName Name = NAME_None)
{
	if (nullptr == World)
		return nullptr;

	if (nullptr == Class)
		return nullptr;

	FActorSpawnParameters spawnParam = FActorSpawnParameters();
	spawnParam.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	if (Name != NAME_None)
	{
		spawnParam.Name = Name;
	}

	return World->SpawnActor(Class, &SpawnLocation, &SpawnRotation, spawnParam);
}


// Sets default values for this component's properties
UGsCampBuildableQueryComponent::UGsCampBuildableQueryComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

}


// Called when the game starts
void UGsCampBuildableQueryComponent::BeginPlay()
{
	Super::BeginPlay();

	if (nullptr != GSGameObject())
	{
		m_pQuery = MakeShareable(new FGsBuildAbleQueryCallback());

		int32 levelKey = GLevel()->GetCurrentLevelId();
		const TArray<FGsSchemaSpotInfo>& zoneArray = GLevel()->GetMapSpot(levelKey);
		const TArray<FGsSchemaSpotInfo>& spotArray = GLevel()->GetZone(levelKey);

		auto handler = GSGameObject()->GetSphereHandler();

		for (const FGsSchemaSpotInfo& zoneInfo : zoneArray)
		{
			FVector location = zoneInfo.pos;
			UGsLevelUtil::TryGetClosestNavmeshLocation(location, GetWorld(), zoneInfo.pos);
			handler->AddSphere(location, zoneInfo.radius);
		}

		for (const FGsSchemaSpotInfo& spotInfo : spotArray)
		{
			FVector location = spotInfo.pos;
			UGsLevelUtil::TryGetClosestNavmeshLocation(location, GetWorld(), spotInfo.pos);
			handler->AddSphere(location, spotInfo.radius);
		}

		if (FGsFenceHandler* fenceHandler = GFence())
		{
			TArray<TSharedPtr<FGsFence>> fenceList;
			if (false == fenceHandler->TryGetAllFence(fenceList, TerritoryType::TOWN))
			{
				return;
			}

			for (TSharedPtr<FGsFence> fence : fenceList)
			{
				FGsFenceConverter conv(fence);
				handler->AddSphere(conv.GetCenter(), conv.GetRadius());
			}
		}

		handler->SpawnDebug(_DebugRender);
	}


	if (_activeCheck)
	{
		UClass* bpClass = nullptr;
		FString BPPath = BluePrint.ToString();
		if (BPPath.Len() != 0)
		{
			BPPath += TEXT("_C");
			bpClass = LoadObject<UClass>(nullptr, *BPPath);
		}
		MeshActor = BPClass(GetWorld(), bpClass, GetOwner()->GetActorLocation(), GetOwner()->GetActorRotation() /*, name*/);
		if (nullptr != MeshActor)
		{
			StaticMeshCompnent = Cast<UStaticMeshComponent>(MeshActor->GetComponentByClass(UStaticMeshComponent::StaticClass()));
			check(StaticMeshCompnent.IsValid());
		}
	}
	
}


void UGsCampBuildableQueryComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}

// Called every frame
void UGsCampBuildableQueryComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (nullptr != GSGameObject() && MeshActor.IsValid())
	{
		FVector Origin, Extends;
		MeshActor->GetActorBounds(false, Origin, Extends);

		float R = (Extends.X + Extends.Y) * 0.5f;
		FVector Dir = GetOwner()->GetActorForwardVector();
		FVector Location = GetOwner()->GetActorLocation() + (Dir * (_DistanceGap * R)) + FVector(0, 0, _HeightGap);

		MeshActor->SetActorLocation(Location);

		bool bEnable = CheckCampBuildAble(Location, R);

		FVector vectorParam = FVector(bEnable ? FLinearColor::Green : FLinearColor::Red);
	
		StaticMeshCompnent->SetVectorParameterValueOnMaterials(TEXT("Color"), vectorParam);
	}
}

bool UGsCampBuildableQueryComponent::CheckCampBuildAble(const FVector& inPos, float inRadius)
{
	auto sphereHandler = GSGameObject()->GetSphereHandler();

	FVector NavPos;
	UGsLevelUtil::TryGetClosestNavmeshLocation(NavPos, GetWorld(), inPos);

	m_pQuery->Reset();
	sphereHandler->RangeTest(NavPos, inRadius, m_pQuery);
	bool bResult = m_pQuery->GetQueryResult();
	return bResult;
}

