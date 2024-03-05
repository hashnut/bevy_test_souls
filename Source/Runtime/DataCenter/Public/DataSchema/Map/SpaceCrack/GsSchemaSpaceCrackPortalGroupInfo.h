/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/DataCenter/Public/Shared/Client/SharedEnums/SharedCreatureEnum.h"
#include "Runtime/DataCenter/Public/DataSchema/GameObject/Prop/GsSchemaPropData.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaSpaceCrackPortalGroupInfo.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/Map/SpaceCrack/BSGsSpaceCrackPortalGroupInfo"))
struct DATACENTER_API FGsSchemaSpaceCrackPortalGroupInfo
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="grade"))
	CreatureRankType grade;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="portalProp"))
	TArray<FGsSchemaPropDataRow> portalProp;
public:
	FGsSchemaSpaceCrackPortalGroupInfo();
	
protected:
	bool operator== (const FGsSchemaSpaceCrackPortalGroupInfo& __Other) const
	{
		return FGsSchemaSpaceCrackPortalGroupInfo::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
};
