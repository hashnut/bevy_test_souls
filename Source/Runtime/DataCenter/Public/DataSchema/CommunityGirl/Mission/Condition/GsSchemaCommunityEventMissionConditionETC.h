/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaCommunityEventMissionConditionETC.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/CommunityGirl/Mission/Condition/BSGsCommunityEventMissionConditionETC"))
struct DATACENTER_API FGsSchemaCommunityEventMissionConditionETC
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="param"))
	int64 param;
public:
	FGsSchemaCommunityEventMissionConditionETC();
	
protected:
	bool operator== (const FGsSchemaCommunityEventMissionConditionETC& __Other) const
	{
		return FGsSchemaCommunityEventMissionConditionETC::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaCommunityEventMissionConditionETC::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaCommunityEventMissionConditionETC::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaCommunityEventMissionConditionETCRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaCommunityEventMissionConditionETCRow() : Super()
	{
	}
	
	const FGsSchemaCommunityEventMissionConditionETC* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaCommunityEventMissionConditionETC* Row = nullptr;
		return Table->FindRow<FGsSchemaCommunityEventMissionConditionETC>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaCommunityEventMissionConditionETCRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaCommunityEventMissionConditionETCRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableCommunityEventMissionConditionETC final : public UGsTable
{
	GENERATED_BODY()

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("CommunityGirl/Mission/Condition");
	}
	
};
