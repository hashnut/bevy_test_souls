/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/CoreUObject/Public/UObject/NoExportTypes.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "Shared/Client/SharedEnums/SharedCreatureEnum.h"
#include "GsSchemaTerritoryNpc.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/Map/Spawn/BSGsTerritoryNpc"))
struct DATACENTER_API FGsSchemaTerritoryNpc
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Meta=(Tooltip="Territory unique id"))
	int64 SpawnId;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="memberType", Tooltip))
	NpcSpawnGroupMemberType memberType;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Meta=(Tooltip="Npc Id, reference npc table"))
	int32 CreatureId;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Meta=(Tooltip="How many spawn count at the same time"))
	int32 SpawnCount;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="RandomPos", Tooltip))
	bool RandomPos;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="Pos", Tooltip))
	FVector Pos;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="Dir", Tooltip))
	int32 Dir;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Meta=(Tooltip="Milliseconds"))
	int32 RespawnInterval;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int64 ClientGameId;
	
public:
	FGsSchemaTerritoryNpc();
	
protected:
	bool operator== (const FGsSchemaTerritoryNpc& __Other) const
	{
		return FGsSchemaTerritoryNpc::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
};
