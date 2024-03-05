/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/DataCenter/Public/DataSchema/Stat/GsSchemaStatInfo.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaNpcBaseStatData.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/Stat/BSGsNpcBaseStatData"))
struct DATACENTER_API FGsSchemaNpcBaseStatData
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="id"))
	int32 id;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="level"))
	int32 level;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="statInfo"))
	TArray<FGsSchemaStatInfo> statInfo;
public:
	FGsSchemaNpcBaseStatData();
	
protected:
	bool operator== (const FGsSchemaNpcBaseStatData& __Other) const
	{
		return FGsSchemaNpcBaseStatData::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaNpcBaseStatData::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaNpcBaseStatData::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaNpcBaseStatDataRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaNpcBaseStatDataRow() : Super()
	{
	}
	
	const FGsSchemaNpcBaseStatData* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaNpcBaseStatData* Row = nullptr;
		return Table->FindRow<FGsSchemaNpcBaseStatData>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaNpcBaseStatDataRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaNpcBaseStatDataRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableNpcBaseStatData final : public UGsTable
{
	GENERATED_BODY()

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("Stat");
	}
	
};
