/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "DataSchema/Quest/GsSchemaQuest.h"
#include "GsSchemaMapEnterConditionSpotInfo.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/Map/BSGsMapEnterConditionSpotInfo"))
struct DATACENTER_API FGsSchemaMapEnterConditionSpotInfo
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="spotIndex", Tooltip="spot index not id"))
	int32 spotIndex;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="questId"))
	FGsSchemaQuestRow questId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="level"))
	int32 level;
public:
	FGsSchemaMapEnterConditionSpotInfo();
	
protected:
	bool operator== (const FGsSchemaMapEnterConditionSpotInfo& __Other) const
	{
		return FGsSchemaMapEnterConditionSpotInfo::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaMapEnterConditionSpotInfo::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaMapEnterConditionSpotInfo::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaMapEnterConditionSpotInfoRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaMapEnterConditionSpotInfoRow() : Super()
	{
	}
	
	const FGsSchemaMapEnterConditionSpotInfo* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaMapEnterConditionSpotInfo* Row = nullptr;
		return Table->FindRow<FGsSchemaMapEnterConditionSpotInfo>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaMapEnterConditionSpotInfoRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaMapEnterConditionSpotInfoRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableMapEnterConditionSpotInfo final : public UGsTable
{
	GENERATED_BODY()

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("Map");
	}
	
};
