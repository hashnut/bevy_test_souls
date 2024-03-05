/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/DataCenter/Public/DataSchema/GsSchemaEnums.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaNetErrorDataExcel.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/NeverCook/BSGsNetErrorDataExcel"))
struct DATACENTER_API FGsSchemaNetErrorDataExcel
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="result"))
	FString result;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="text"))
	FText text;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="exText"))
	FText exText;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="uiType"))
	EGsNetErrorProcUIType uiType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="customUIKey"))
	FString customUIKey;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="option"))
	int32 option;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="isDefault"))
	bool isDefault;
public:
	FGsSchemaNetErrorDataExcel();
	
protected:
	bool operator== (const FGsSchemaNetErrorDataExcel& __Other) const
	{
		return FGsSchemaNetErrorDataExcel::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaNetErrorDataExcel::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaNetErrorDataExcel::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaNetErrorDataExcelRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaNetErrorDataExcelRow() : Super()
	{
	}
	
	const FGsSchemaNetErrorDataExcel* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaNetErrorDataExcel* Row = nullptr;
		return Table->FindRow<FGsSchemaNetErrorDataExcel>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaNetErrorDataExcelRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaNetErrorDataExcelRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableNetErrorDataExcel final : public UGsTable
{
	GENERATED_BODY()

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("NeverCook");
	}
	
};
