/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/DataCenter/Public/DataSchema/QuestBoard/GsSchemaRefreshCurrencyInfoList.h"
#include "Runtime/DataCenter/Public/Shared/Client/SharedEnums/SharedQuestEnum.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaVillageQuestBoardConfig.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/QuestBoard/BSGsVillageQuestBoardConfig"))
struct DATACENTER_API FGsSchemaVillageQuestBoardConfig
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="id"))
	int32 id;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="questBoardType"))
	QuestBoardType questBoardType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="DailyQuestAcceptLimit"))
	int32 DailyQuestAcceptLimit;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="DailyQuestRefreshLimit"))
	int32 DailyQuestRefreshLimit;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="DailyQuestNumber"))
	int32 DailyQuestNumber;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="RefreshCurrencyInfoList"))
	TArray<FGsSchemaRefreshCurrencyInfoList> RefreshCurrencyInfoList;
public:
	FGsSchemaVillageQuestBoardConfig();
	
protected:
	bool operator== (const FGsSchemaVillageQuestBoardConfig& __Other) const
	{
		return FGsSchemaVillageQuestBoardConfig::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaVillageQuestBoardConfig::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaVillageQuestBoardConfig::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaVillageQuestBoardConfigRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaVillageQuestBoardConfigRow() : Super()
	{
	}
	
	const FGsSchemaVillageQuestBoardConfig* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaVillageQuestBoardConfig* Row = nullptr;
		return Table->FindRow<FGsSchemaVillageQuestBoardConfig>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaVillageQuestBoardConfigRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaVillageQuestBoardConfigRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableVillageQuestBoardConfig final : public UGsTable
{
	GENERATED_BODY()

protected:
	TMap<int32, FName> _indexById;

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("QuestBoard");
	}
	
	void OnCachedDataTableChanged() final
	{
		TMap<FName, const FGsSchemaVillageQuestBoardConfig*> RowsMap;
		if (!CreateAllRowsMap<const FGsSchemaVillageQuestBoardConfig>(RowsMap))
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
	bool FindRowById(const int32& InId, OUT const FGsSchemaVillageQuestBoardConfig*& OutRow) const
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
		const FGsSchemaVillageQuestBoardConfig* Temp;
		return !FindRowById(FGsSchemaVillageQuestBoardConfig().id, Temp);
	}
#endif
};
