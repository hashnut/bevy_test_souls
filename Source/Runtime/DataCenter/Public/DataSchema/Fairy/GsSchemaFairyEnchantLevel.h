/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaFairyEnchantLevel.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/Fairy/BSGsFairyEnchantLevel"))
struct DATACENTER_API FGsSchemaFairyEnchantLevel
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Meta=(DisplayName="current level", Tooltip="현재 레벨을 기준"))
	uint8 level;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="costGold", Tooltip))
	int32 costGold;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int32 materialItemCount;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Meta=(DisplayName="level up rate"))
	int32 levelIncreasedProb;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Meta=(DisplayName="level maintain rate"))
	int32 levelMaintainedProb;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Meta=(DisplayName="level down rate"))
	int32 levelDecreasedProb;
	
public:
	FGsSchemaFairyEnchantLevel();
	
protected:
	bool operator== (const FGsSchemaFairyEnchantLevel& __Other) const
	{
		return FGsSchemaFairyEnchantLevel::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
};
