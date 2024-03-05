/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/DataCenter/Public/AI/Data/EGsAIContentsType.h"
#include "Runtime/DataCenter/Public/DataSchema/Autoplay/GsSchemaAIActionData.h"
#include "Runtime/DataCenter/Public/DataSchema/Autoplay/GsSchemaAIConditionData.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaAITaskData.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/Autoplay/BSGsAITaskData"))
struct DATACENTER_API FGsSchemaAITaskData
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="ReserveConditions"))
	TArray<FGsSchemaAIConditionData> ReserveConditions;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="ActionConditions"))
	TArray<FGsSchemaAIConditionData> ActionConditions;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="ClearConditions"))
	TArray<FGsSchemaAIConditionData> ClearConditions;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="Action"))
	FGsSchemaAIActionData Action;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="설명", DisplayName="Annotation"))
	FString Annotation;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="ContentsType"))
	EGsAIContentsType ContentsType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="ReserveCount"))
	int32 ReserveCount;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="IsHighPriority"))
	bool IsHighPriority;
public:
	FGsSchemaAITaskData();
	
protected:
	bool operator== (const FGsSchemaAITaskData& __Other) const
	{
		return FGsSchemaAITaskData::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaAITaskData::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaAITaskData::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaAITaskDataRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaAITaskDataRow() : Super()
	{
	}
	
	const FGsSchemaAITaskData* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaAITaskData* Row = nullptr;
		return Table->FindRow<FGsSchemaAITaskData>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaAITaskDataRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaAITaskDataRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableAITaskData final : public UGsTable
{
	GENERATED_BODY()

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("Autoplay");
	}
	
};
