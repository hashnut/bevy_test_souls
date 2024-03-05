/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaOptionOnOff.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/Option/BSGsOptionOnOff"))
struct DATACENTER_API FGsSchemaOptionOnOff
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="nameText"))
	FText nameText;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="OnOff"))
	bool OnOff;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="buttonOnText"))
	FText buttonOnText;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="buttonOffText"))
	FText buttonOffText;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="isTooltip"))
	bool isTooltip;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="tooltipText"))
	FText tooltipText;
public:
	FGsSchemaOptionOnOff();
	
protected:
	bool operator== (const FGsSchemaOptionOnOff& __Other) const
	{
		return FGsSchemaOptionOnOff::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaOptionOnOff::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaOptionOnOff::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaOptionOnOffRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaOptionOnOffRow() : Super()
	{
	}
	
	const FGsSchemaOptionOnOff* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaOptionOnOff* Row = nullptr;
		return Table->FindRow<FGsSchemaOptionOnOff>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaOptionOnOffRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaOptionOnOffRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableOptionOnOff final : public UGsTable
{
	GENERATED_BODY()

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("Option");
	}
	
};
