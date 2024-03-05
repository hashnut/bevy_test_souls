/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/DataCenter/Public/DataSchema/Fairy/GsSchemaCeilingTrial.h"
#include "Runtime/DataCenter/Public/Shared/Client/SharedEnums/SharedFairyEnum.h"
#include "Runtime/DataCenter/Public/DataSchema/Fairy/GsSchemaFairyProbability.h"
#include "Runtime/DataCenter/Public/DataSchema/Item/GsSchemaItemCommon.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaFairyCeiling.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/Fairy/BSGsFairyCeiling"))
struct DATACENTER_API FGsSchemaFairyCeiling
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="grade"))
	FairyGrade grade;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="trialData"))
	TArray<FGsSchemaCeilingTrial> trialData;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="needItemId"))
	FGsSchemaItemCommonRow needItemId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="maxCount"))
	int32 maxCount;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="successProbabilityId"))
	FGsSchemaFairyProbabilityRow successProbabilityId;
public:
	FGsSchemaFairyCeiling();
	
protected:
	bool operator== (const FGsSchemaFairyCeiling& __Other) const
	{
		return FGsSchemaFairyCeiling::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaFairyCeiling::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaFairyCeiling::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaFairyCeilingRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaFairyCeilingRow() : Super()
	{
	}
	
	const FGsSchemaFairyCeiling* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaFairyCeiling* Row = nullptr;
		return Table->FindRow<FGsSchemaFairyCeiling>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaFairyCeilingRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaFairyCeilingRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableFairyCeiling final : public UGsTable
{
	GENERATED_BODY()

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("Fairy");
	}
	
};
