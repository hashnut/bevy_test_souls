/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/DataCenter/Public/DataSchema/Fairy/GsSchemaCeilingTrial.h"
#include "Runtime/DataCenter/Public/Shared/Client/SharedEnums/SharedCostumeEnum.h"
#include "Runtime/DataCenter/Public/DataSchema/Costume/GsSchemaCostumeProbability.h"
#include "Runtime/DataCenter/Public/DataSchema/Item/GsSchemaItemCommon.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaCostumeCeiling.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/Costume/BSGsCostumeCeiling"))
struct DATACENTER_API FGsSchemaCostumeCeiling
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="grade"))
	CostumeGrade grade;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="trialData"))
	TArray<FGsSchemaCeilingTrial> trialData;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="needItemId"))
	FGsSchemaItemCommonRow needItemId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="maxCount"))
	int32 maxCount;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="successProbabilityId"))
	FGsSchemaCostumeProbabilityRow successProbabilityId;
public:
	FGsSchemaCostumeCeiling();
	
protected:
	bool operator== (const FGsSchemaCostumeCeiling& __Other) const
	{
		return FGsSchemaCostumeCeiling::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaCostumeCeiling::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaCostumeCeiling::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaCostumeCeilingRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaCostumeCeilingRow() : Super()
	{
	}
	
	const FGsSchemaCostumeCeiling* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaCostumeCeiling* Row = nullptr;
		return Table->FindRow<FGsSchemaCostumeCeiling>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaCostumeCeilingRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaCostumeCeilingRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableCostumeCeiling final : public UGsTable
{
	GENERATED_BODY()

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("Costume");
	}
	
};
