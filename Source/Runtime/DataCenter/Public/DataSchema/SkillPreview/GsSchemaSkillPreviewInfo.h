/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "DataSchema/SkillPreview/GsSchemaSkillPreviewCameraModeDataSet.h"
#include "DataSchema/SkillPreview/GsSchemaSkillPreviewTransformSet.h"
#include "GsSchemaSkillPreviewInfo.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/SkillPreview/BSGsSkillPreviewInfo"))
struct DATACENTER_API FGsSchemaSkillPreviewInfo
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="playerCount", Tooltip="스폰되는 플레이어수 (1 기본)"))
	int32 playerCount;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="monsterCount", Tooltip="스폰되는 몬스터수"))
	int32 monsterCount;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="cameraTransform"))
	FGsSchemaSkillPreviewCameraModeDataSetRow cameraTransform;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="gameObjectTransform"))
	FGsSchemaSkillPreviewTransformSetRow gameObjectTransform;
public:
	FGsSchemaSkillPreviewInfo();
	
protected:
	bool operator== (const FGsSchemaSkillPreviewInfo& __Other) const
	{
		return FGsSchemaSkillPreviewInfo::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
};
