/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "Item/GsStatTypeNameSelector.h"
#include "GsSchemaStatConvertDataExcel.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/NeverCook/Stat/BSGsStatConvertDataExcel"))
struct DATACENTER_API FGsSchemaStatConvertDataExcel
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="baseType"))
	FGsStatTypeNameSelector baseType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="baseValue"))
	int32 baseValue;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="statType1"))
	FGsStatTypeNameSelector statType1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="statValue1"))
	int32 statValue1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="statType2"))
	FGsStatTypeNameSelector statType2;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="statValue2"))
	int32 statValue2;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="statType3"))
	FGsStatTypeNameSelector statType3;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="statValue3"))
	int32 statValue3;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="statType4"))
	FGsStatTypeNameSelector statType4;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="statValue4"))
	int32 statValue4;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="statType5"))
	FGsStatTypeNameSelector statType5;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="statValue5"))
	int32 statValue5;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="statType6"))
	FGsStatTypeNameSelector statType6;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="statValue6"))
	int32 statValue6;
public:
	FGsSchemaStatConvertDataExcel();
	
protected:
	bool operator== (const FGsSchemaStatConvertDataExcel& __Other) const
	{
		return FGsSchemaStatConvertDataExcel::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaStatConvertDataExcel::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaStatConvertDataExcel::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaStatConvertDataExcelRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaStatConvertDataExcelRow() : Super()
	{
	}
	
	const FGsSchemaStatConvertDataExcel* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaStatConvertDataExcel* Row = nullptr;
		return Table->FindRow<FGsSchemaStatConvertDataExcel>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaStatConvertDataExcelRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaStatConvertDataExcelRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableStatConvertDataExcel final : public UGsTable
{
	GENERATED_BODY()

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("NeverCook/Stat");
	}
	
};
