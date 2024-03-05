/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/CoreUObject/Public/UObject/NoExportTypes.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaCustomizeSlotData.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/Customize/BSGsCustomizeSlotData"))
struct DATACENTER_API FGsSchemaCustomizeSlotData
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int32 targetId;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="iconPath", Tooltip))
	FSoftObjectPath iconPath;
	
public:
	FGsSchemaCustomizeSlotData();
	
protected:
	bool operator== (const FGsSchemaCustomizeSlotData& __Other) const
	{
		return FGsSchemaCustomizeSlotData::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaCustomizeSlotData::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaCustomizeSlotData::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaCustomizeSlotDataRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaCustomizeSlotDataRow() : Super()
	{
	}
	
	const FGsSchemaCustomizeSlotData* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaCustomizeSlotData* Row = nullptr;
		return Table->FindRow<FGsSchemaCustomizeSlotData>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaCustomizeSlotDataRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaCustomizeSlotDataRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableCustomizeSlotData final : public UGsTable
{
	GENERATED_BODY()

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("Customize");
	}
	
};
