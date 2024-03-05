/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/DataCenter/Public/DataSchema/GameObject/Prop/GsSchemaPropData.h"
#include "Runtime/DataCenter/Public/Shared/Client/SharedEnums/SharedMapEventEnum.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaSpaceCrackConfigPatternPortal.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/Map/SpaceCrack/BSGsSpaceCrackConfigPatternPortal"))
struct DATACENTER_API FGsSchemaSpaceCrackConfigPatternPortal
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="patternType"))
	SpaceCrackActivatePatternType patternType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="propId"))
	FGsSchemaPropDataRow propId;
public:
	FGsSchemaSpaceCrackConfigPatternPortal();
	
protected:
	bool operator== (const FGsSchemaSpaceCrackConfigPatternPortal& __Other) const
	{
		return FGsSchemaSpaceCrackConfigPatternPortal::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
};
