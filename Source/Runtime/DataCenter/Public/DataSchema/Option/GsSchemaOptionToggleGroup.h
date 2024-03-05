/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/DataCenter/Public/Shared/Client/SharedEnums/SharedGsOptionItemEnum.h"
#include "Runtime/DataCenter/Public/DataSchema/Option/GsSchemaOptionToggle.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaOptionToggleGroup.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/Option/BSGsOptionToggleGroup"))
struct DATACENTER_API FGsSchemaOptionToggleGroup
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="optionItemType"))
	EGsOptionItem optionItemType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="nameText"))
	FText nameText;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="toggleList"))
	TArray<FGsSchemaOptionToggle> toggleList;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="isTooltip"))
	bool isTooltip;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="tooltipText"))
	FText tooltipText;
public:
	FGsSchemaOptionToggleGroup();
	
protected:
	bool operator== (const FGsSchemaOptionToggleGroup& __Other) const
	{
		return FGsSchemaOptionToggleGroup::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaOptionToggleGroup::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaOptionToggleGroup::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaOptionToggleGroupRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaOptionToggleGroupRow() : Super()
	{
	}
	
	const FGsSchemaOptionToggleGroup* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaOptionToggleGroup* Row = nullptr;
		return Table->FindRow<FGsSchemaOptionToggleGroup>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaOptionToggleGroupRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaOptionToggleGroupRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableOptionToggleGroup final : public UGsTable
{
	GENERATED_BODY()

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("Option");
	}
	
};
