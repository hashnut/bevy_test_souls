/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "Shared/Client/SharedEnums/SharedTitleEnum.h"
#include "GsSchemaTitleConditionType.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/Title/BSGsTitleConditionType"))
struct DATACENTER_API FGsSchemaTitleConditionType
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="conditionType"))
	TitleConditionType conditionType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="conditionDataType"))
	TitleConditionDataType conditionDataType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="param1Column"))
	FString param1Column;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="param2Column"))
	FString param2Column;
public:
	FGsSchemaTitleConditionType();
	
protected:
	bool operator== (const FGsSchemaTitleConditionType& __Other) const
	{
		return FGsSchemaTitleConditionType::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaTitleConditionType::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaTitleConditionType::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaTitleConditionTypeRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaTitleConditionTypeRow() : Super()
	{
	}
	
	const FGsSchemaTitleConditionType* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaTitleConditionType* Row = nullptr;
		return Table->FindRow<FGsSchemaTitleConditionType>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaTitleConditionTypeRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaTitleConditionTypeRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableTitleConditionType final : public UGsTable
{
	GENERATED_BODY()

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("Title");
	}
	
};
