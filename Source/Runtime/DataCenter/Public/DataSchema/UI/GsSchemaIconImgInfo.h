/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/CoreUObject/Public/UObject/NoExportTypes.h"
#include "Runtime/DataCenter/Public/DataSchema/GsSchemaEnums.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaIconImgInfo.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/UI/BSGsIconImgInfo"))
struct DATACENTER_API FGsSchemaIconImgInfo
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="iconType"))
	EGsPurposeIcon iconType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="iconPath"))
	FSoftObjectPath iconPath;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="imageSize"))
	FVector2D imageSize;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="tintColor"))
	FLinearColor tintColor;
public:
	FGsSchemaIconImgInfo();
	
protected:
	bool operator== (const FGsSchemaIconImgInfo& __Other) const
	{
		return FGsSchemaIconImgInfo::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
};
