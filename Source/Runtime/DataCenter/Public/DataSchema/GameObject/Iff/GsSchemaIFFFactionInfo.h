/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/DataCenter/Public/Shared/Client/SharedEnums/SharedIffEnum.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaIFFFactionInfo.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/GameObject/Iff/BSGsIFFFactionInfo"))
struct DATACENTER_API FGsSchemaIFFFactionInfo
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="적용될 대상의 IFF상태 타입 목록", DisplayName="possibleTypeList"))
	TArray<IffFactionType> possibleTypeList;
public:
	FGsSchemaIFFFactionInfo();
	
protected:
	bool operator== (const FGsSchemaIFFFactionInfo& __Other) const
	{
		return FGsSchemaIFFFactionInfo::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
};
