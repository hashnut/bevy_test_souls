/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/DataCenter/Public/DataSchema/Option/GsSchemaOptionCheck.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaOptionCheckGroup.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/Option/BSGsOptionCheckGroup"))
struct DATACENTER_API FGsSchemaOptionCheckGroup
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="nameText"))
	FText nameText;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="isTooltip"))
	bool isTooltip;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="tooltipText"))
	FText tooltipText;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="checkList"))
	TArray<FGsSchemaOptionCheck> checkList;
public:
	FGsSchemaOptionCheckGroup();
	
protected:
	bool operator== (const FGsSchemaOptionCheckGroup& __Other) const
	{
		return FGsSchemaOptionCheckGroup::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaOptionCheckGroup::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaOptionCheckGroup::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaOptionCheckGroupRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaOptionCheckGroupRow() : Super()
	{
	}
	
	const FGsSchemaOptionCheckGroup* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaOptionCheckGroup* Row = nullptr;
		return Table->FindRow<FGsSchemaOptionCheckGroup>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaOptionCheckGroupRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaOptionCheckGroupRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableOptionCheckGroup final : public UGsTable
{
	GENERATED_BODY()

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("Option");
	}
	
};
