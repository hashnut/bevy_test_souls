/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaNpcDialogActorSpawnInfo.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/NpcDialog/BSGsNpcDialogActorSpawnInfo"))
struct DATACENTER_API FGsSchemaNpcDialogActorSpawnInfo
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="spawnRotate", Tooltip))
	float spawnRotate;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="spawnRange", Tooltip))
	float spawnRange;
	
public:
	FGsSchemaNpcDialogActorSpawnInfo();
	
protected:
	bool operator== (const FGsSchemaNpcDialogActorSpawnInfo& __Other) const
	{
		return FGsSchemaNpcDialogActorSpawnInfo::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
};
