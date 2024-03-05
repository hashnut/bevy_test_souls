/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/DataCenter/Public/DataSchema/Codex/GsSchemaLevelExp.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaCodexMapGroupBalance.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/Codex/BSGsCodexMapGroupBalance"))
struct DATACENTER_API FGsSchemaCodexMapGroupBalance
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="id"))
	int32 id;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="levelList"))
	TArray<FGsSchemaLevelExp> levelList;
public:
	FGsSchemaCodexMapGroupBalance();
	
protected:
	bool operator== (const FGsSchemaCodexMapGroupBalance& __Other) const
	{
		return FGsSchemaCodexMapGroupBalance::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaCodexMapGroupBalance::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaCodexMapGroupBalance::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaCodexMapGroupBalanceRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaCodexMapGroupBalanceRow() : Super()
	{
	}
	
	const FGsSchemaCodexMapGroupBalance* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaCodexMapGroupBalance* Row = nullptr;
		return Table->FindRow<FGsSchemaCodexMapGroupBalance>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaCodexMapGroupBalanceRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaCodexMapGroupBalanceRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableCodexMapGroupBalance final : public UGsTable
{
	GENERATED_BODY()

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("Codex");
	}
	
};
