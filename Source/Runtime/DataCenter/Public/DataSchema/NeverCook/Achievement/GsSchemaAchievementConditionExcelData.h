/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/DataCenter/Public/Shared/Client/SharedEnums/SharedAchievementEnum.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaAchievementConditionExcelData.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/NeverCook/Achievement/BSGsAchievementConditionExcelData"))
struct DATACENTER_API FGsSchemaAchievementConditionExcelData
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="conditionType"))
	AchievementType conditionType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="param1"))
	FString param1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="param2"))
	FString param2;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="param3"))
	FString param3;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="param4"))
	FString param4;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="param5"))
	FString param5;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="param6"))
	FString param6;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="param7"))
	FString param7;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="param8"))
	FString param8;
public:
	FGsSchemaAchievementConditionExcelData();
	
protected:
	bool operator== (const FGsSchemaAchievementConditionExcelData& __Other) const
	{
		return FGsSchemaAchievementConditionExcelData::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaAchievementConditionExcelData::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaAchievementConditionExcelData::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaAchievementConditionExcelDataRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaAchievementConditionExcelDataRow() : Super()
	{
	}
	
	const FGsSchemaAchievementConditionExcelData* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaAchievementConditionExcelData* Row = nullptr;
		return Table->FindRow<FGsSchemaAchievementConditionExcelData>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaAchievementConditionExcelDataRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaAchievementConditionExcelDataRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableAchievementConditionExcelData final : public UGsTable
{
	GENERATED_BODY()

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("NeverCook/Achievement");
	}
	
};
