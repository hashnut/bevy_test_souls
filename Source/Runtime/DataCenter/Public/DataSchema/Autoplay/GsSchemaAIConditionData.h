/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "DataSchema/GsSchemaEnums.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaAIConditionData.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/Autoplay/BSGsAIConditionData"))
struct DATACENTER_API FGsSchemaAIConditionData
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	EGsAIConditionType ConditionType;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool FunctionCompareData;
	
public:
	FGsSchemaAIConditionData();
	
protected:
	bool operator== (const FGsSchemaAIConditionData& __Other) const
	{
		return FGsSchemaAIConditionData::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaAIConditionData::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaAIConditionData::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaAIConditionDataRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaAIConditionDataRow() : Super()
	{
	}
	
	const FGsSchemaAIConditionData* GetRow() const
	{
		const FGsSchemaAIConditionData* Row = nullptr;
		return GetTable()->FindRow<FGsSchemaAIConditionData>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaAIConditionDataRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaAIConditionDataRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableAIConditionData final : public UGsTable
{
	GENERATED_BODY()

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("Autoplay");
	}
	
};
