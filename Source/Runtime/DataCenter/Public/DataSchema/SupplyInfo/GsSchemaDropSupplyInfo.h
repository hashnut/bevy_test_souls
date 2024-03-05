/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/DataCenter/Public/DataSchema/SupplyInfo/GsSchemaDropSupplyInfoCreatureIds.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaDropSupplyInfo.generated.h"

USTRUCT(BlueprintType, Meta=(Tooltip="공급처 드랍", AssetSchemaPackagePath="/Game/DataCenter/DataSchema/SupplyInfo/BSGsDropSupplyInfo"))
struct DATACENTER_API FGsSchemaDropSupplyInfo
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Meta=(Tooltip="MapId:FGsSchemaDropSupplyInfoCreatureIds", DisplayName="maps"))
	TMap<int32,FGsSchemaDropSupplyInfoCreatureIds> maps;
public:
	FGsSchemaDropSupplyInfo();
	
protected:
	bool operator== (const FGsSchemaDropSupplyInfo& __Other) const
	{
		return FGsSchemaDropSupplyInfo::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
};
