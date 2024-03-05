// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

//UE4
#include "Runtime/Json/Public/Serialization/JsonSerializer.h"
#include "Runtime/Json/Public/Serialization/JsonWriter.h"

//T1 Project 
#include "Editor/GsReregisterComponentInterface.h"

//Territory editor
#include "Define/GsTerritoryZoneShapeDefine.h"
#include "Define/GsTerritorySpawnDefine.h"
#include "Util/GsTerritoryEditorUtil.h"
#include "GsTerritoryUniqueIDManager.h"
#include "Shared/Client/SharedEnums/SharedCreatureEnum.h"
#include "T1Project/ActorComponentEx/WidgetComponentEx.h"

#include "GsTerritoryActor.generated.h"

const float DEFAULT_TERRITORY_WIDGET_HEIGHT = 200.0f;
const FString INVALID_TERRITORY_ACTOR = TEXT("잘못된 액터입니다. 삭제해주세요.");

class UGsTerrEdWidgetComponent;

DECLARE_MULTICAST_DELEGATE_OneParam(FTerritoryEditorActorConstruct, AActor*);
DECLARE_MULTICAST_DELEGATE_OneParam(FTerrEdActorPostEditPropertyChange, AActor*);

/**
 * placed in level for spawn data
 */
UCLASS(BlueprintType)
class TERRITORYEDITOR_API AGsTerritoryActor : public AActor, public IGsReregisterComponentInterface
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleInstanceOnly, BlueprintReadWrite, Category = "Territory")
		int64 _uniqueID = -1;	
	UPROPERTY(VisibleInstanceOnly, BlueprintReadWrite, Category = "Territory")
		int64 _instanceID = -1;
	UPROPERTY(VisibleInstanceOnly, BlueprintReadWrite, Category = "Territory")
		bool _isValid = true;	
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Territory")
		FString _description = "Description";

public:
	UPROPERTY(Transient)
		bool _isCanDelete = false;
	
public:
	UPROPERTY(EditDefaultsOnly)
		FColor _descWidgetFontColor = FColor::Black;
	UPROPERTY(EditDefaultsOnly)
		FColor _descWidgetBGColor = FColor::Blue;

private:
	FString _invalidReason;
	UPROPERTY()
	bool _canSpawnOnNavmesh = true;

public:
	FTerritoryEditorActorConstruct  _onTerritoryActorConstruct;
	FTerrEdActorPostEditPropertyChange _onTerrEdActorPostEditPropertyChange;

public:
	FDelegateHandle _selectHandle;

public:
	//Widget
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Terrain zone widget")
		float _widgetHeight = DEFAULT_TERRITORY_WIDGET_HEIGHT;
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Terrain zone widget")
		FVector2D _widgetScreenSize = FVector2D(400, 150);	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Terrain zone widget")
		class UGsTerrEdWidgetComponent* _widgetComp;

public:
	AGsTerritoryActor();
	virtual void OnConstruction(const FTransform& Transform) override;
	virtual void BeginDestroy() override;
	virtual void Destroyed() override;
	virtual void ReregisterComponent() override;
	virtual void BeginPlay() override;	
	virtual bool ShouldTickIfViewportsOnly() const override;
	virtual void TickActor(float in_delta, ELevelTick in_type, FActorTickFunction& in_tickFuc) override;
	virtual void TickActorInEditor(float in_delta);
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent);
	virtual void PostDuplicate(bool bDuplicateForPIE) override;
	virtual void PostDuplicate(EDuplicateMode::Type inDuplicateMode) override;

public:
	bool IsCreatedInNonServerLevel();

	//UniqueKey
public:
	//Never call anytime, must be called at create first time
	virtual bool RecoveryTerritoryUniqueID();
	virtual bool InvalidTerritoryUniqueID(FString& outInvalidReason);
	virtual void InitializeTerritoryUniqueID() { _uniqueID = INDEX_NONE; }

public:
	static bool TryGenerateTerritoryUniqueID(AActor* inOwner, EGsTerritoryObjType inType, int64& outId, int64& outInstanceID);
	static bool UnregisterTerritoryUniqueID(AActor* inOwner, EGsTerritoryObjType inType, int64& inInstanceID);

public:
	virtual bool CanSetUniqueID() { return INDEX_NONE == _uniqueID; }
	virtual void SetTerritoryUniqueID(int64& inUniqueID, int64& inInstanceID)
	{
		_uniqueID = inUniqueID;
		_instanceID = inInstanceID;		
	}

	UFUNCTION(BlueprintCallable, Category = "Territory actor")
		void PrintTerritoryUniqueID();
	virtual bool CheckEditableTerritoryUniqueID();

public:
	virtual bool IsNeedToCopy(){return false;}
	virtual void Copy() {}
	virtual void DestroyManually();

public:
	virtual void InvalidShape();

	//Widget
public:
	virtual void CreateWidget(const FTransform& in_transform);
	virtual void InvalidWidgetPosition(const FVector& in_location);
	virtual void InvalidWidgetText();	
	virtual void InvalidDestription();
	void InvalidWidgetColor();	
	class UGsTerrEdWidgetComponent* GetWidgetComp() { return _widgetComp; }
	virtual void SetWidgetVisibility(bool inVisible);

public://editor	
	virtual bool IsCanDeleteSelectedActor(FText& out_reasone) const;

public:
	void OnSelected(class UObject* in_select);
	virtual void SelectInternal(class UObject* inSelect);

public:
	virtual void SetTerritoryUniqueIDForce(int64 inUniqueId);
	virtual int64 GetTerritoryUniqueID() { return _uniqueID; }
	virtual int64 GetTerritoryUniqueID() const { return _uniqueID; }
	int64 GetTerritoryInstanceId(){return _instanceID;}	
	virtual const EGsTerritoryObjType GetTerritoryObjType() const { return EGsTerritoryObjType::Max; }
	virtual FString GetLevelName();
	void SetValid(bool isIsValid);
	bool IsValid(){return _isValid;}
	void SetInvalidReason(const FString& inReason){_invalidReason = inReason;}

public:
	FString ToString();

	//Navmesh
public:
	void SetCanSpawnOnNavmesh(bool inSpawn) { _canSpawnOnNavmesh = inSpawn;}
	virtual bool IsNeedToCheckOnNavmesh() {return true;}
	void SetTerritoryTitleText(FString inTitle);
	void SetTerritoryTitleColor(FColor inColor);
	void SetTerritoryDescriptionText(FString inDescription);
	void SetTerritoryDescriptionColor(FColor inColor);
	virtual void SetShapeVisible(bool inShow) {}
};
