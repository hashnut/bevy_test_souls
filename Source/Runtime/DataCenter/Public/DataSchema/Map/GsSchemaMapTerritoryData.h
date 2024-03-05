/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/DataCenter/Public/DataSchema/Map/GsSchemaMapSpotInfo.h"
#include "Runtime/DataCenter/Public/DataSchema/Map/GsSchemaMapTerritoryEnv.h"
#include "Runtime/DataCenter/Public/DataSchema/Map/GsSchemaMapTerritoryInfo.h"
#include "Runtime/DataCenter/Public/DataSchema/Map/GsSchemaMapTerritorySound.h"
#include "Runtime/DataCenter/Public/DataSchema/Map/GsSchemaMapTerritorySpaceCrack.h"
#include "Runtime/DataCenter/Public/DataSchema/Map/GsSchemaMapWarpInfo.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaMapTerritoryData.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/Map/BSGsMapTerritoryData"))
struct DATACENTER_API FGsSchemaMapTerritoryData
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="spawnList"))
	TArray<FGsSchemaMapSpotInfo> spawnList;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="부활 데이터는 다른 레벨을 참고하기 때문에 수동 조작 및 저장해야합니다", DisplayName="resurrectList"))
	TArray<FGsSchemaMapSpotInfo> resurrectList;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="teleportList"))
	TArray<FGsSchemaMapSpotInfo> teleportList;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="warpList"))
	TArray<FGsSchemaMapWarpInfo> warpList;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="territoryList"))
	TArray<FGsSchemaMapTerritoryInfo> territoryList;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="ReturnToTown", DisplayName="RTT"))
	FGsSchemaMapSpotInfo RTT;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="RTTList"))
	TArray<FGsSchemaMapSpotInfo> RTTList;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="spaceCrackZoneList"))
	TArray<FGsSchemaMapTerritorySpaceCrack> spaceCrackZoneList;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="envTerritoryList"))
	TArray<FGsSchemaMapTerritoryEnv> envTerritoryList;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="soundTerritoryList"))
	TArray<FGsSchemaMapTerritorySound> soundTerritoryList;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="noCampZoneList"))
	TArray<FGsSchemaMapTerritoryInfo> noCampZoneList;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="isObstacleMap"))
	bool isObstacleMap;
public:
	FGsSchemaMapTerritoryData();
	
protected:
	bool operator== (const FGsSchemaMapTerritoryData& __Other) const
	{
		return FGsSchemaMapTerritoryData::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaMapTerritoryData::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaMapTerritoryData::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaMapTerritoryDataRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaMapTerritoryDataRow() : Super()
	{
	}
	
	const FGsSchemaMapTerritoryData* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaMapTerritoryData* Row = nullptr;
		return Table->FindRow<FGsSchemaMapTerritoryData>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaMapTerritoryDataRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaMapTerritoryDataRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableMapTerritoryData final : public UGsTable
{
	GENERATED_BODY()

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("Map");
	}
	
};
