/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/DataCenter/Public/DataSchema/Achievement/GsSchemaAchievementCondition.h"
#include "Runtime/DataCenter/Public/DataSchema/Achievement/GsSchemaAchievementDetail.h"
#include "Runtime/DataCenter/Public/DataSchema/Reward/GsSchemaRewardData.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaAchievementCommon.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/Achievement/BSGsAchievementCommon"))
struct DATACENTER_API FGsSchemaAchievementCommon
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="id"))
	int64 id;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="groupId"))
	int64 groupId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="level"))
	uint8 level;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="completePoint"))
	int64 completePoint;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="rewardId"))
	FGsSchemaRewardDataRow rewardId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="achievementCondition"))
	FGsSchemaAchievementConditionRow achievementCondition;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="detail"))
	FGsSchemaAchievementDetailRow detail;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="isTheme"))
	bool isTheme;
public:
	FGsSchemaAchievementCommon();
	
protected:
	bool operator== (const FGsSchemaAchievementCommon& __Other) const
	{
		return FGsSchemaAchievementCommon::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaAchievementCommon::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaAchievementCommon::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaAchievementCommonRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaAchievementCommonRow() : Super()
	{
	}
	
	const FGsSchemaAchievementCommon* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaAchievementCommon* Row = nullptr;
		return Table->FindRow<FGsSchemaAchievementCommon>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaAchievementCommonRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaAchievementCommonRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableAchievementCommon final : public UGsTable
{
	GENERATED_BODY()

protected:
	TMap<int64, FName> _indexById;

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("Achievement");
	}
	
	void OnCachedDataTableChanged() final
	{
		TMap<FName, const FGsSchemaAchievementCommon*> RowsMap;
		if (!CreateAllRowsMap<const FGsSchemaAchievementCommon>(RowsMap))
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
	bool FindRowById(const int64& InId, OUT const FGsSchemaAchievementCommon*& OutRow) const
	{
		return FindInIndexMap(_indexById, InId, OutRow);
	}
	
#if WITH_EDITOR
	void RemoveRowById(const int64& InId)
	{
		RemoveInIndexMap(_indexById, InId);
	}
	
	bool IsValidAllPrimaryKeys() const final
	{
		const FGsSchemaAchievementCommon* Temp;
		return !FindRowById(FGsSchemaAchievementCommon().id, Temp);
	}
#endif
};
