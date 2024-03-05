/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaRewardBoxDataBMExcel.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/NeverCook/Reward/BSGsRewardBoxDataBMExcel"))
struct DATACENTER_API FGsSchemaRewardBoxDataBMExcel
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="id"))
	int64 id;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="itemBagList"))
	int64 itemBagList;
public:
	FGsSchemaRewardBoxDataBMExcel();
	
protected:
	bool operator== (const FGsSchemaRewardBoxDataBMExcel& __Other) const
	{
		return FGsSchemaRewardBoxDataBMExcel::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaRewardBoxDataBMExcel::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaRewardBoxDataBMExcel::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaRewardBoxDataBMExcelRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaRewardBoxDataBMExcelRow() : Super()
	{
	}
	
	const FGsSchemaRewardBoxDataBMExcel* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaRewardBoxDataBMExcel* Row = nullptr;
		return Table->FindRow<FGsSchemaRewardBoxDataBMExcel>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaRewardBoxDataBMExcelRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaRewardBoxDataBMExcelRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableRewardBoxDataBMExcel final : public UGsTable
{
	GENERATED_BODY()

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("NeverCook/Reward");
	}
	
};
