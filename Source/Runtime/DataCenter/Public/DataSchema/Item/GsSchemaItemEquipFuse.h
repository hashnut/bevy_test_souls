/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/DataCenter/Public/DataSchema/Item/GsSchemaItemCommon.h"
#include "Runtime/DataCenter/Public/Shared/Client/SharedEnums/SharedItemEnum.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaItemEquipFuse.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/Item/BSGsItemEquipFuse"))
struct DATACENTER_API FGsSchemaItemEquipFuse
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="fuseItemCategorySub"))
	ItemCategorySub fuseItemCategorySub;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="fuseItemId"))
	FGsSchemaItemCommonRow fuseItemId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="fuseMinEnchantLevel"))
	int32 fuseMinEnchantLevel;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="fuseResultItemId"))
	FGsSchemaItemCommonRow fuseResultItemId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="fuseCost"))
	int32 fuseCost;
public:
	FGsSchemaItemEquipFuse();
	
protected:
	bool operator== (const FGsSchemaItemEquipFuse& __Other) const
	{
		return FGsSchemaItemEquipFuse::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaItemEquipFuse::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaItemEquipFuse::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaItemEquipFuseRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaItemEquipFuseRow() : Super()
	{
	}
	
	const FGsSchemaItemEquipFuse* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaItemEquipFuse* Row = nullptr;
		return Table->FindRow<FGsSchemaItemEquipFuse>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaItemEquipFuseRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaItemEquipFuseRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableItemEquipFuse final : public UGsTable
{
	GENERATED_BODY()

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("Item");
	}
	
};
