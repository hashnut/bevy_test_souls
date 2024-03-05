/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "Shared/Client/SharedEnums/SharedCommonEnum.h"
#include "GsSchemaSkillUpCostSetExcel.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/NeverCook/Skill/Cost/BSGsSkillUpCostSetExcel"))
struct DATACENTER_API FGsSchemaSkillUpCostSetExcel
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="id"))
	int32 id;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="desc"))
	FText desc;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="costType"))
	CostType costType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="currencyType1"))
	CurrencyType currencyType1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="currencyAmount1"))
	int32 currencyAmount1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="currencyType2"))
	CurrencyType currencyType2;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="currencyAmount2"))
	int32 currencyAmount2;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="currencyType3"))
	CurrencyType currencyType3;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="currencyAmount3"))
	int32 currencyAmount3;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="currencyType4"))
	CurrencyType currencyType4;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="currencyAmount4"))
	int32 currencyAmount4;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="currencyType5"))
	CurrencyType currencyType5;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="currencyAmount5"))
	int32 currencyAmount5;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="itemId1"))
	int32 itemId1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="itemCount1"))
	int32 itemCount1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="itemId2"))
	int32 itemId2;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="itemCount2"))
	int32 itemCount2;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="itemId3"))
	int32 itemId3;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="itemCount3"))
	int32 itemCount3;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="itemId4"))
	int32 itemId4;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="itemCount4"))
	int32 itemCount4;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="itemId5"))
	int32 itemId5;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="itemCount5"))
	int32 itemCount5;
public:
	FGsSchemaSkillUpCostSetExcel();
	
protected:
	bool operator== (const FGsSchemaSkillUpCostSetExcel& __Other) const
	{
		return FGsSchemaSkillUpCostSetExcel::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaSkillUpCostSetExcel::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaSkillUpCostSetExcel::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaSkillUpCostSetExcelRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaSkillUpCostSetExcelRow() : Super()
	{
	}
	
	const FGsSchemaSkillUpCostSetExcel* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaSkillUpCostSetExcel* Row = nullptr;
		return Table->FindRow<FGsSchemaSkillUpCostSetExcel>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaSkillUpCostSetExcelRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaSkillUpCostSetExcelRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableSkillUpCostSetExcel final : public UGsTable
{
	GENERATED_BODY()

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("NeverCook/Skill/Cost");
	}
	
};
