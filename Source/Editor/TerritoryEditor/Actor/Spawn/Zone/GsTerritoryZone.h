// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

//Editor
#include "Runtime/Json/Public/Serialization/JsonSerializer.h"
#include "Runtime/Json/Public/Serialization/JsonWriter.h"
#include "Runtime/Core/Public/Templates/SharedPointer.h"

//T1 Project
#include "T1Project/Editor/Widget/GsEditorWidget.h"

//Territory editor
#include "Actor/Spawn/GsTerritoryActor.h"
#include "Actor/Spawn/Zone/Shape/GsTerritoryZoneShape.h"
#include "Actor/Spawn/SpawnActor/GsTerritoryNpc.h"
#include "Actor/GsTerritoryShapeOwnerInterface.h"
#include "Define/GsTerritoryZoneShapeDefine.h"
#include "Define/GsTerritorySpawnDefine.h"
#include "DataSchema/Map/Spawn/GsSchemaSpawnData.h"
#include "Blutility/Input/GsTerritorySpotInput.h"
#include "GsTerritoryZoneBase.h"
//#include "./GsEditorTerritoryGather.h"

#include "GsTerritoryZone.generated.h"

/**
 *
 */

UCLASS(BlueprintType)
class TERRITORYEDITOR_API AGsTerritoryZone : public AGsTerritoryZoneBase
{
	GENERATED_BODY()

	//Data
public:		
	UPROPERTY(EditInstanceOnly, Category = "Territory zone")
		int32 _groupID;
	UPROPERTY(EditInstanceOnly, Category = "Territory zone")
		NpcSpawnGroupType _spawnGroupType;
	UPROPERTY(EditInstanceOnly, Category = "Territory zone")
		bool _byClient = false;
	UPROPERTY(EditInstanceOnly, Category = "Territory zone")
		bool _byDefault = true;	
	/** 기획자용 섹션 spawn.json에 미포함 */
	UPROPERTY(EditInstanceOnly, Category = "Territory zone")
		int32 _section;
	/** 기획자용 메모 spawn.json에 미포함 */
	UPROPERTY(EditInstanceOnly, Category = "Territory zone")
		FString _note;
	UPROPERTY(EditInstanceOnly, Category = "Territory zone")
		FGsTerritoryLineId _lineId;
	UPROPERTY(EditInstanceOnly, Category = "Territory zone")
		bool _isGroupLineMove = false;	
	UPROPERTY(EditInstanceOnly, Category = "Territory zone")
		TArray<FGsTerritoryCurrentMapSpot> _randomSpotIdList;

public:
	//Territory actors
	UPROPERTY(VisibleInstanceOnly, BlueprintReadWrite, Category = "Territory zone")
		TArray<class AGsTerritorySpawnActor*> _spawnActors;		

public:
	AGsTerritoryZone();
	virtual void OnConstruction(const FTransform& Transform) override;		
	virtual void PostEditChangeProperty(FPropertyChangedEvent& inPropertyChangedEvent) override;
	virtual void Destroyed() override;

public:	
	virtual void DestroyManually() override;

	//Reloacted
protected:
	void KeepTerritoryActorRelatvieLocation(const FTransform& in_trs);	

	//widget
public:	
	virtual void InvalidWidgetPosition(const FVector& in_location) override;	
	virtual void InvalidDestription() override;

	//Add and remove terrain actor
public:
	UFUNCTION(BlueprintCallable, Category = "Territory zone")
		void AddTerritoryActor(AGsTerritorySpawnActor* in_actor);
	UFUNCTION(BlueprintCallable, Category = "Territory zone")
		void RemoveTerritoryActor(AGsTerritorySpawnActor* in_actor);
	UFUNCTION(BlueprintCallable, Category = "Territory zone")
		AGsTerritorySpawnActor* AddTerritorySpawnActor(const FGsTerritorySpawnInfo& in_actor, const FVector& in_worldLocaiton);
	UFUNCTION(BlueprintCallable, Category = "Territory zone")
		bool IsTerritoryActorExist(AGsTerritorySpawnActor* in_actor);

private:
	AActor* CreateTerritoryTemplate(const FGsTerritorySpawnInfo& in_data, const FVector& in_worldLocation);
	bool IsValideKey(const FGsTerritorySpawnInfo& inData) const;

	//UniqueID
public:
	//Temp function for revcovery uniuqeID
	virtual bool RecoveryTerritoryUniqueID() override;

public:
	//Copy child actor, when actor created
	virtual void Copy() override;

	// dat export
public:
	void Export(OUT FGsSchemaSpawnData& outSpawnData);
	static bool TryConvertSpawnActorToSpawnElement(const AGsTerritorySpawnActor* inZoneChild, OUT FGsSchemaSpawnElement& outSpawnElement);
	void Import(const FGsSchemaSpawnData* inSpawnData);
	const AGsTerritorySpawnActor* ConvertSpawnElementToSpawnActor(const FGsSchemaSpawnElement& inElement);

public://get set		
	virtual void CopyZoneInfo(class AGsTerritoryZoneBase* zone) override;
	EGsTerritoryObjType ConverTerritorySpawnType(EGsTerritorySpawnType inSpawn);
	NpcSpawnGroupType GetSpawnGroupType() const {	return _spawnGroupType;}
	void SetAllNpcSpawnLevel(int32 inLevel);
	void SetSection(const int32 inSection);
	void SetNote(const FString& inNote);

	//copy
public:
	virtual bool IsNeedToCopy() override {return true;}
};
