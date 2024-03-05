/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/CoreUObject/Public/UObject/NoExportTypes.h"
#include "Runtime/DataCenter/Public/Map/GsTerritoryShapeType.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaLineElement.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/Map/Line/BSGsLineElement"))
struct DATACENTER_API FGsSchemaLineElement
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="LineId"))
	int64 LineId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="ShapeType"))
	EGsTerritoryShapeType ShapeType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="isReturn"))
	bool isReturn;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="isRepeat"))
	bool isRepeat;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="repeatDelay"))
	float repeatDelay;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="isSuicide"))
	bool isSuicide;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="Distance"))
	float Distance;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="Radius"))
	float Radius;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="VertextList"))
	TArray<FVector> VertextList;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="Center"))
	FVector Center;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="isReverse"))
	bool isReverse;
public:
	FGsSchemaLineElement();
	
protected:
	bool operator== (const FGsSchemaLineElement& __Other) const
	{
		return FGsSchemaLineElement::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
};
