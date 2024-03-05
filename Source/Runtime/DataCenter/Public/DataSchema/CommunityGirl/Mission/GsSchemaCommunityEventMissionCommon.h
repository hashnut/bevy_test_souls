/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/DataCenter/Public/DataSchema/CommunityGirl/Mission/GsSchemaCommunityEventMissionCondition.h"
#include "Runtime/DataCenter/Public/DataSchema/Reward/GsSchemaRewardData.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaCommunityEventMissionCommon.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/CommunityGirl/Mission/BSGsCommunityEventMissionCommon"))
struct DATACENTER_API FGsSchemaCommunityEventMissionCommon
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="id"))
	int32 id;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="rewardId"))
	FGsSchemaRewardDataRow rewardId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="eventMissionConditionId"))
	FGsSchemaCommunityEventMissionConditionRow eventMissionConditionId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="eventMissionCount"))
	int32 eventMissionCount;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="eventMissionText"))
	FText eventMissionText;
public:
	FGsSchemaCommunityEventMissionCommon();
	
protected:
	bool operator== (const FGsSchemaCommunityEventMissionCommon& __Other) const
	{
		return FGsSchemaCommunityEventMissionCommon::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaCommunityEventMissionCommon::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaCommunityEventMissionCommon::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaCommunityEventMissionCommonRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaCommunityEventMissionCommonRow() : Super()
	{
	}
	
	const FGsSchemaCommunityEventMissionCommon* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaCommunityEventMissionCommon* Row = nullptr;
		return Table->FindRow<FGsSchemaCommunityEventMissionCommon>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaCommunityEventMissionCommonRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaCommunityEventMissionCommonRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableCommunityEventMissionCommon final : public UGsTable
{
	GENERATED_BODY()

protected:
	TMap<int32, FName> _indexById;

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("CommunityGirl/Mission");
	}
	
	void OnCachedDataTableChanged() final
	{
		TMap<FName, const FGsSchemaCommunityEventMissionCommon*> RowsMap;
		if (!CreateAllRowsMap<const FGsSchemaCommunityEventMissionCommon>(RowsMap))
		{
			return;
		}
		_indexById.Empty(RowsMap.Num());
		for (auto Row : RowsMap)
		{
			_indexById.Emplace(Row.Value->id, Row.Key);
		}
	}
	
public:
	bool FindRowById(const int32& InId, OUT const FGsSchemaCommunityEventMissionCommon*& OutRow) const
	{
		return FindInIndexMap(_indexById, InId, OutRow);
	}
	
#if WITH_EDITOR
	void RemoveRowById(const int32& InId)
	{
		RemoveInIndexMap(_indexById, InId);
	}
	
	bool IsValidAllPrimaryKeys() const final
	{
		const FGsSchemaCommunityEventMissionCommon* Temp;
		return !FindRowById(FGsSchemaCommunityEventMissionCommon().id, Temp);
	}
#endif
};
