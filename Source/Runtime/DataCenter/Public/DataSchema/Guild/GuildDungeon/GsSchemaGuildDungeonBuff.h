/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "DataSchema/Skill/Passivity/GsSchemaPassivitySet.h"
#include "GsSchemaGuildDungeonBuff.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/Guild/GuildDungeon/BSGsGuildDungeonBuff"))
struct DATACENTER_API FGsSchemaGuildDungeonBuff
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="memberMin"))
	int32 memberMin;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="memberMax"))
	int32 memberMax;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="buffPassivityId"))
	FGsSchemaPassivitySetRow buffPassivityId;
public:
	FGsSchemaGuildDungeonBuff();
	
protected:
	bool operator== (const FGsSchemaGuildDungeonBuff& __Other) const
	{
		return FGsSchemaGuildDungeonBuff::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
};
