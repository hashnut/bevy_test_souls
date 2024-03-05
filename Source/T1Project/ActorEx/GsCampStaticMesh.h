// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ActorEx/GsCharacterBase.h"
#include "GsCampStaticMesh.generated.h"


class UStaticMesh;
class UMaterialInterface;
class UStaticMeshComponent;
class AGsCharacterLocalPlayer;

class FGsCampManager;


UCLASS()
class T1PROJECT_API AGsCampStaticMesh : public AGsCharacterBase
{
	GENERATED_BODY()

private:
	const float BUILD_CHECK_ANGLE_MIN = 85.f;
	const float BUILD_CHECK_ANGLE_MAX = 95.f;

protected:
	// 건물 staticmesh 
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CampStaticMesh", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* _campStaticMesh = nullptr;

protected:
	TWeakObjectPtr<AGsCharacterLocalPlayer> _localPlyerPtr;
	class FGsFenceHandler* _fenceHandler;

protected:
	FGsCampManager* _campMgr = nullptr;

private:
	// Test 확인용
	UPROPERTY(EditAnywhere, Category = "GsUI", meta = (AllowPrivateAccess = "true"))
	float _angleMinValue = 0.f;
	UPROPERTY(EditAnywhere, Category = "GsUI", meta = (AllowPrivateAccess = "true"))
	float _angleMaxValue = 10.f;

private:
	FVector _actorBoundsExtends;
	bool _isClientSpawn = true;
	bool _isReadyToCompleteStaticMesh = false;

	
public:	
	// Sets default values for this actor's properties
	AGsCampStaticMesh();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type in_type) override;
	virtual void Tick(float DeltaTime) override;

private:
	bool FindLocalPlayerPtr();

private:
	bool CheckNavMeshRayCast();
	bool IsHitInNav(FVector InTargetStartPos);

public:
	void DestoryClientSpawn();

public:
	float GetCampDiameter();

public:
	void SetCampLocation();
	void SetCampStaticMesh(UStaticMesh* InStaticMesh, UMaterialInterface* InMaterial);
	void SetClientSpawnFlag(bool Inflag) { _isClientSpawn = Inflag; }

public:
	UFUNCTION(BlueprintCallable, Category = "GsCamp")
	uint8 CheckBuilableProc();
	UFUNCTION(BlueprintCallable, Category = "GsCamp")
	bool IsOkBuildCampArea();


};
