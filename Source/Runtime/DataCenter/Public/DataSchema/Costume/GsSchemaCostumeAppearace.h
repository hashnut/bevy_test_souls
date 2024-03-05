/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaCostumeAppearace.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/Costume/BSGsCostumeAppearace"))
struct DATACENTER_API FGsSchemaCostumeAppearace
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="fullSet 로만 변경 가능한가", DisplayName="isSetAppearance"))
	bool isSetAppearance;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(EditCondition="isSetAppearance == false", DisplayName="hairPart"))
	bool hairPart;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(EditCondition="isSetAppearance == false", DisplayName="bodyPart"))
	bool bodyPart;
public:
	FGsSchemaCostumeAppearace();
	
protected:
	bool operator== (const FGsSchemaCostumeAppearace& __Other) const
	{
		return FGsSchemaCostumeAppearace::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
};
