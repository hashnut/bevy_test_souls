/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/DataCenter/Public/DataSchema/Codex/SchemaOnly/GsSchemaCodexCategoryBalanceLevelData.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaCodexCategoryBalance.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/Codex/BSGsCodexCategoryBalance"))
struct DATACENTER_API FGsSchemaCodexCategoryBalance
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="id"))
	int32 id;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="levelList"))
	TArray<FGsSchemaCodexCategoryBalanceLevelData> levelList;
public:
	FGsSchemaCodexCategoryBalance();
	
protected:
	bool operator== (const FGsSchemaCodexCategoryBalance& __Other) const
	{
		return FGsSchemaCodexCategoryBalance::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaCodexCategoryBalance::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaCodexCategoryBalance::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaCodexCategoryBalanceRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaCodexCategoryBalanceRow() : Super()
	{
	}
	
	const FGsSchemaCodexCategoryBalance* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaCodexCategoryBalance* Row = nullptr;
		return Table->FindRow<FGsSchemaCodexCategoryBalance>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaCodexCategoryBalanceRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaCodexCategoryBalanceRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableCodexCategoryBalance final : public UGsTable
{
	GENERATED_BODY()

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("Codex");
	}
	
};
