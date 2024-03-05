/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "DataSchema/Guild/GsSchemaGuildStoreExtend.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaGuildStore.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/Guild/BSGsGuildStore"))
struct DATACENTER_API FGsSchemaGuildStore
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="maxAcquiredItemHistoryCount"))
	int32 maxAcquiredItemHistoryCount;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="maxDistributedItemHistoryCount"))
	int32 maxDistributedItemHistoryCount;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="mailTypeId"))
	int32 mailTypeId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="safeCapacityDefault"))
	int32 safeCapacityDefault;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="safeStorageTimeMax"))
	int32 safeStorageTimeMax;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="extendSlotCount"))
	int32 extendSlotCount;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="extendList"))
	TArray<FGsSchemaGuildStoreExtend> extendList;
public:
	FGsSchemaGuildStore();
	
protected:
	bool operator== (const FGsSchemaGuildStore& __Other) const
	{
		return FGsSchemaGuildStore::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
};
