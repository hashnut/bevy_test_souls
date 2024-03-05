// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

//UE4
#include "Runtime/Core/Public/Templates/SharedPointer.h"
#include "Runtime/Json/Public/Serialization/JsonSerializer.h"
#include "Runtime/Json/Public/Serialization/JsonWriter.h"
#include "Runtime/Core/Public/Templates/SharedPointer.h"

//Territory editor
#include "Actor/Spawn/GsTerritoryActor.h"
#include "Shared/Client/SharedEnums/SharedCreatureEnum.h"
#include "Define/GsTerritorySpawnDefine.h"

#include "GsTerritorySpawnActor.generated.h"

class AGsEditorTerritoryZone;
class UGsTerrEdWidgetComponent;
/**
 *
 */
UCLASS(BlueprintType)
class TERRITORYEDITOR_API AGsTerritorySpawnActor : public AGsTerritoryActor
{
	GENERATED_BODY()	

public:
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Territory actor")
		AGsTerritoryZone* _zone;
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Territory spawn actor", meta = (ShowOnlyInnerProperties))
		FGsTerritorySpawnInfo _spawnInfo;

protected:
	UPROPERTY()
		FString _targetName;

	//select
public:
	FDelegateHandle _selectHandle;

	//Transform 
public:
	UPROPERTY(Transient)
	bool _selected = false;
	UPROPERTY(Transient)
	FVector _preLocation;

public:
	AGsTerritorySpawnActor();
	virtual void OnConstruction(const FTransform& in_trans) override;	
	virtual void Destroyed() override;
	UFUNCTION(BlueprintCallable, Category = "Territory actor")
		virtual void DestroyManually() override;

	//Shape
protected:
	virtual bool TryGetShapePath(int32 in_id, FSoftObjectPath& out_path) { return false; }
	virtual bool SetShape(int32 in_id) { return false; }

public:
	virtual void InvalidShape() {}

	//Undo
protected:
	virtual void ReregisterZone();

	//UniqueID
public:
	//Temp function for revcovery uniuqeID
	virtual bool RecoveryTerritoryUniqueID() override;

	//View
public:
	void CreateWidget(const FTransform& inTransform);
	virtual void InvalidWidgetPosition(const FVector& in_location);	
	virtual FString GetSpawnDataText();
	virtual FVector GetWidgetLocation();

protected:
	void RotateAtOwnPivot();	
	virtual void SelectInternal(UObject* inSelect) override;

public:
	UFUNCTION()
	virtual void ReregisterComponent() override;

public://get set
	UFUNCTION(BlueprintCallable, Category = "Territory actor")
		void SetTerritoryZone(AGsTerritoryZone* in_zone);
	virtual FVector GetRelativeLocation();
	virtual NpcSpawnGroupMemberType GetSpawnGroupMemberType() const {return _spawnInfo.SpawnGroupMemberType;}
	virtual void SetSpawnInfo(const FGsTerritorySpawnInfo& in_spawn);
	virtual void SetTerritoryRelativeLocation(const FVector& in_location);
	virtual CreatureSpawnType GetCreatureSpawnType() const {return CreatureSpawnType::NONE;}
	void SetSpawnLevel(int32 inLevel);
};
