/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/DataCenter/Public/Shared/Client/SharedEnums/SharedItemEnum.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaDropProbGroupExcel.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/Drop/BSGsDropProbGroupExcel"))
struct DATACENTER_API FGsSchemaDropProbGroupExcel
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="id"))
	int32 id;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="categoryMain"))
	ItemCategoryMain categoryMain;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="categorySub"))
	ItemCategorySub categorySub;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="grade"))
	ItemGrade grade;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="tier"))
	int32 tier;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="prob"))
	int32 prob;
public:
	FGsSchemaDropProbGroupExcel();
	
protected:
	bool operator== (const FGsSchemaDropProbGroupExcel& __Other) const
	{
		return FGsSchemaDropProbGroupExcel::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaDropProbGroupExcel::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaDropProbGroupExcel::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaDropProbGroupExcelRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaDropProbGroupExcelRow() : Super()
	{
	}
	
	const FGsSchemaDropProbGroupExcel* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaDropProbGroupExcel* Row = nullptr;
		return Table->FindRow<FGsSchemaDropProbGroupExcel>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaDropProbGroupExcelRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaDropProbGroupExcelRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableDropProbGroupExcel final : public UGsTable
{
	GENERATED_BODY()

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("Drop");
	}
	
};
