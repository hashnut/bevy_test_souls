/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/DataCenter/Public/Shared/Client/SharedEnums/SharedAchievementEnum.h"
#include "Runtime/DataCenter/Public/DataSchema/Achievement/GsSchemaAchievementDetail.h"
#include "Runtime/DataCenter/Public/DataSchema/Reward/GsSchemaRewardData.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaAchievementDataExcel.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/NeverCook/Achievement/BSGsAchievementDataExcel"))
struct DATACENTER_API FGsSchemaAchievementDataExcel
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="id"))
	int64 id;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="groupId"))
	int32 groupId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="level"))
	int32 level;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="isTheme"))
	bool isTheme;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="achievementType"))
	AchievementType achievementType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="conditionTableName"))
	FString conditionTableName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="conditionRowName"))
	FName conditionRowName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="completePoint"))
	int64 completePoint;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="rewardId"))
	FGsSchemaRewardDataRow rewardId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="detail"))
	FGsSchemaAchievementDetailRow detail;
public:
	FGsSchemaAchievementDataExcel();
	
protected:
	bool operator== (const FGsSchemaAchievementDataExcel& __Other) const
	{
		return FGsSchemaAchievementDataExcel::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaAchievementDataExcel::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaAchievementDataExcel::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaAchievementDataExcelRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaAchievementDataExcelRow() : Super()
	{
	}
	
	const FGsSchemaAchievementDataExcel* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaAchievementDataExcel* Row = nullptr;
		return Table->FindRow<FGsSchemaAchievementDataExcel>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaAchievementDataExcelRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaAchievementDataExcelRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableAchievementDataExcel final : public UGsTable
{
	GENERATED_BODY()

protected:
	TMap<int64, FName> _indexById;

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("NeverCook/Achievement");
	}
	
	void OnCachedDataTableChanged() final
	{
		TMap<FName, const FGsSchemaAchievementDataExcel*> RowsMap;
		if (!CreateAllRowsMap<const FGsSchemaAchievementDataExcel>(RowsMap))
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
	bool FindRowById(const int64& InId, OUT const FGsSchemaAchievementDataExcel*& OutRow) const
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
		const FGsSchemaAchievementDataExcel* Temp;
		return !FindRowById(FGsSchemaAchievementDataExcel().id, Temp);
	}
#endif
};
