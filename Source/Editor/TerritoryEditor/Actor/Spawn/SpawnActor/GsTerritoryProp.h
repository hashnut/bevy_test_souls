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
#include "Components/ChildActorComponent.h"

#include "GsTerritoryProp.generated.h"

/**
 *
 */
class UArrowComponent;
class UStaticMeshComponent;
class UChildActorComponent;

UCLASS(BlueprintType)
class TERRITORYEDITOR_API AGsTerritoryProp : public AGsTerritorySpawnActor
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Territory spawn actor")
		UStaticMeshComponent* _mesh;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Territory spawn actor")
		UChildActorComponent* _childActor;
	UPROPERTY()
		UArrowComponent* _arrowComponent;
		FDelegateHandle _animInitHandle;

public:
	AGsTerritoryProp();
	virtual void OnConstruction(const FTransform& in_transform) override;
	virtual void TickActorInEditor(float inDelta) override;
	virtual void Destroyed() override;

public://Shape
	virtual void InvalidShape() override;
	virtual bool TryGetShapePath(int32 in_id, FSoftObjectPath& out_path) override;
	virtual bool SetShape(int32 in_id) override;	
	bool SetStaticMesh(int32 inId, UStaticMeshComponent* inMesh);
	bool SetChildActor(int32 inId);

	bool TryGetBlueprintPath(int32 inId, FSoftObjectPath& outPath);

public:
	virtual void InvalidWidgetPosition(const FVector& in_location) override;	
	virtual void InvalidWidgetText() override;
	//virtual FVector GetWidgetLocation() override;

	//Get Set
public:	
	virtual const EGsTerritoryObjType GetTerritoryObjType() const override;	
	virtual CreatureSpawnType GetCreatureSpawnType() const override { return CreatureSpawnType::PROP; }
	virtual void SetShapeVisible(bool inShow) override;
};
