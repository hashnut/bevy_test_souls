/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaRewardBoxDataItemExcel.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/NeverCook/Reward/BSGsRewardBoxDataItemExcel"))
struct DATACENTER_API FGsSchemaRewardBoxDataItemExcel
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
	FGsSchemaRewardBoxDataItemExcel();
	
protected:
	bool operator== (const FGsSchemaRewardBoxDataItemExcel& __Other) const
	{
		return FGsSchemaRewardBoxDataItemExcel::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaRewardBoxDataItemExcel::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaRewardBoxDataItemExcel::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaRewardBoxDataItemExcelRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaRewardBoxDataItemExcelRow() : Super()
	{
	}
	
	const FGsSchemaRewardBoxDataItemExcel* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaRewardBoxDataItemExcel* Row = nullptr;
		return Table->FindRow<FGsSchemaRewardBoxDataItemExcel>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaRewardBoxDataItemExcelRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaRewardBoxDataItemExcelRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableRewardBoxDataItemExcel final : public UGsTable
{
	GENERATED_BODY()

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("NeverCook/Reward");
	}
	
};
