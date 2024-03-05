// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GsTerritorySpawnActor.h"

//UE4
#include "Runtime/Json/Public/Serialization/JsonSerializer.h"
#include "Runtime/Json/Public/Serialization/JsonWriter.h"
#include "Runtime/Core/Public/Templates/SharedPointer.h"

//Territory editor
#include "Define/GsTerritoryZoneShapeDefine.h"
#include "Define/GsTerritorySpawnDefine.h"
#include "Shared/Client/SharedEnums/SharedCreatureEnum.h"

#include "GsTerritoryNpc.generated.h"

/**
 *
 */
class UArrowComponent;
class USkeletalMeshComponent;

UCLASS(BlueprintType)
class TERRITORYEDITOR_API AGsTerritoryNpc : public AGsTerritorySpawnActor
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Territory spawn actor")
		UChildActorComponent* _npcChildActor;
	UPROPERTY()
		UArrowComponent* _arrowComponent;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Territory spawn actor")
		UStaticMeshComponent* _sightRadiusComponent;	

public:
	FDelegateHandle _animInitHandle;	

private:
	FVector _cameraLocation;
	bool _isIn;

public:
	AGsTerritoryNpc();
	virtual void OnConstruction(const FTransform& in_transform) override;
	virtual void BeginDestroy() override;
	virtual void PostEditChangeProperty(FPropertyChangedEvent& inPropertyChangedEvent) override;
	virtual void Destroyed() override;

public://Shape
	virtual void InvalidShape() override;
	virtual bool SetShape(int32 in_id) override;
	virtual void SelectInternal(UObject* inSelect) override;
	void InvalidSightRadius();

public:
	virtual void InvalidWidgetPosition(const FVector& in_location) override;
	virtual void InvalidWidgetText() override;
	virtual FVector GetWidgetLocation() override;
	virtual void TickActorInEditor(float inDelta) override;
	virtual FString GetSpawnDataText() override;	

	//Get Set
public:
	virtual const EGsTerritoryObjType GetTerritoryObjType() const override;
	virtual CreatureSpawnType GetCreatureSpawnType() const override { return CreatureSpawnType::NPC; }
	virtual void SetShapeVisible(bool inShow) override;
	float GetSightRadius();
	void SetSightRidusVisible(bool inVisible);
};
