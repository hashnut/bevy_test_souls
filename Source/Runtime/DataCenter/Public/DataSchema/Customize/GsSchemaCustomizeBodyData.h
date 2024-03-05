/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaCustomizeBodyData.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/Customize/BSGsCustomizeBodyData"))
struct DATACENTER_API FGsSchemaCustomizeBodyData
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="Height"))
	int32 Height;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="Head"))
	int32 Head;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="Chest"))
	int32 Chest;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="Waist"))
	int32 Waist;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="Pelvis"))
	int32 Pelvis;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="Legs"))
	int32 Legs;
public:
	FGsSchemaCustomizeBodyData();
	
protected:
	bool operator== (const FGsSchemaCustomizeBodyData& __Other) const
	{
		return FGsSchemaCustomizeBodyData::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaCustomizeBodyData::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaCustomizeBodyData::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaCustomizeBodyDataRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaCustomizeBodyDataRow() : Super()
	{
	}
	
	const FGsSchemaCustomizeBodyData* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaCustomizeBodyData* Row = nullptr;
		return Table->FindRow<FGsSchemaCustomizeBodyData>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaCustomizeBodyDataRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaCustomizeBodyDataRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableCustomizeBodyData final : public UGsTable
{
	GENERATED_BODY()

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("Customize");
	}
	
};
