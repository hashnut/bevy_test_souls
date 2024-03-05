/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaLocationData.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/Location/BSGsLocationData"))
struct DATACENTER_API FGsSchemaLocationData
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="mapId"))
	int32 mapId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="dir"))
	int32 dir;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="posX"))
	float posX;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="posY"))
	float posY;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="posZ"))
	float posZ;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="posRandomRadius"))
	int32 posRandomRadius;
public:
	FGsSchemaLocationData();
	
protected:
	bool operator== (const FGsSchemaLocationData& __Other) const
	{
		return FGsSchemaLocationData::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaLocationData::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaLocationData::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaLocationDataRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaLocationDataRow() : Super()
	{
	}
	
	const FGsSchemaLocationData* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaLocationData* Row = nullptr;
		return Table->FindRow<FGsSchemaLocationData>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaLocationDataRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaLocationDataRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableLocationData final : public UGsTable
{
	GENERATED_BODY()

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("Location");
	}
	
};
