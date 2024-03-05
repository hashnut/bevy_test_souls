/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/CoreUObject/Public/UObject/NoExportTypes.h"
#include "Runtime/DataCenter/Public/Shared/Client/SharedEnums/SharedCreatureEnum.h"
#include "Runtime/DataCenter/Public/DataSchema/GameObject/GsSchemaUserLookInfo.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaCostumeDetail.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/Costume/BSGsCostumeDetail"))
struct DATACENTER_API FGsSchemaCostumeDetail
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="explanation"))
	FText explanation;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="portrait"))
	TMap<CreatureGenderType,FSoftObjectPath> portrait;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="applyCustomizedPelvisSize"))
	bool applyCustomizedPelvisSize;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="flavorText"))
	FText flavorText;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="shapeInfo"))
	FGsSchemaUserLookInfoRow shapeInfo;
public:
	FGsSchemaCostumeDetail();
	
protected:
	bool operator== (const FGsSchemaCostumeDetail& __Other) const
	{
		return FGsSchemaCostumeDetail::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
};
