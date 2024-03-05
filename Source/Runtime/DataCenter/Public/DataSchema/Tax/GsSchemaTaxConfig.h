/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/DataCenter/Public/Shared/Client/SharedEnums/SharedCreatureEnum.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaTaxConfig.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/Tax/BSGsTaxConfig"))
struct DATACENTER_API FGsSchemaTaxConfig
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="골드 시스템 세율_만분율", DisplayName="goldSystemTaxRate"))
	int32 goldSystemTaxRate;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="다이아 시스템 세율_만분율", DisplayName="diaSystemTaxRate"))
	int32 diaSystemTaxRate;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="골드 기본 세율_만분율", DisplayName="goldDefaultTaxRate"))
	int32 goldDefaultTaxRate;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="다이아 기본 세율_만분율", DisplayName="diaDefaultTaxRate"))
	int32 diaDefaultTaxRate;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="taxNpcList"))
	TArray<NpcFunctionType> taxNpcList;
public:
	FGsSchemaTaxConfig();
	
protected:
	bool operator== (const FGsSchemaTaxConfig& __Other) const
	{
		return FGsSchemaTaxConfig::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaTaxConfig::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaTaxConfig::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaTaxConfigRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaTaxConfigRow() : Super()
	{
	}
	
	const FGsSchemaTaxConfig* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaTaxConfig* Row = nullptr;
		return Table->FindRow<FGsSchemaTaxConfig>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaTaxConfigRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaTaxConfigRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableTaxConfig final : public UGsTable
{
	GENERATED_BODY()

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("Tax");
	}
	
};
