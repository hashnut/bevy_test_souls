/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/CoreUObject/Public/UObject/NoExportTypes.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaRootmotionInfo.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/Skill/RootMotion/BSGsRootmotionInfo"))
struct DATACENTER_API FGsSchemaRootmotionInfo
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="Offset", Tooltip="경과 시간(Sec)"))
	float Offset;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="Pos", Tooltip="변화된 위치"))
	FVector Pos;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="Angle", Tooltip="변화된 이동 각도"))
	float Angle;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="ObjectAngle", Tooltip="변화된 개체의 각도"))
	float ObjectAngle;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="Distance"))
	float Distance;
public:
	FGsSchemaRootmotionInfo();
	
protected:
	bool operator== (const FGsSchemaRootmotionInfo& __Other) const
	{
		return FGsSchemaRootmotionInfo::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
};
