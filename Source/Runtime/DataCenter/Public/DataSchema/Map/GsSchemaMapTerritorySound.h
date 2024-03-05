/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/DataCenter/Public/GameObject/ObjectClass/Data/GsEnvCondition.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaMapTerritorySound.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/Map/BSGsMapTerritorySound"))
struct DATACENTER_API FGsSchemaMapTerritorySound
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="territoryId"))
	int64 territoryId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="fenceId"))
	int64 fenceId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="name"))
	FText name;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="questSoundList"))
	TArray<FGsQuestSound> questSoundList;
public:
	FGsSchemaMapTerritorySound();
	
protected:
	bool operator== (const FGsSchemaMapTerritorySound& __Other) const
	{
		return FGsSchemaMapTerritorySound::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
};
