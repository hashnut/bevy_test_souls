/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaInvadeBattleLevel.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/InterServer/BSGsInvadeBattleLevel"))
struct DATACENTER_API FGsSchemaInvadeBattleLevel
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="battleLevel"))
	int32 battleLevel;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="LevelName"))
	FText LevelName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="requireItemCount"))
	int32 requireItemCount;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="pkDeathDecreaseRate"))
	int32 pkDeathDecreaseRate;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="teleportDecreaseRate"))
	int32 teleportDecreaseRate;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="DropRate"))
	int32 DropRate;
public:
	FGsSchemaInvadeBattleLevel();
	
protected:
	bool operator== (const FGsSchemaInvadeBattleLevel& __Other) const
	{
		return FGsSchemaInvadeBattleLevel::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaInvadeBattleLevel::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaInvadeBattleLevel::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaInvadeBattleLevelRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaInvadeBattleLevelRow() : Super()
	{
	}
	
	const FGsSchemaInvadeBattleLevel* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaInvadeBattleLevel* Row = nullptr;
		return Table->FindRow<FGsSchemaInvadeBattleLevel>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaInvadeBattleLevelRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaInvadeBattleLevelRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableInvadeBattleLevel final : public UGsTable
{
	GENERATED_BODY()

protected:
	TMap<int32, FName> _indexById;

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("InterServer");
	}
	
	void OnCachedDataTableChanged() final
	{
		TMap<FName, const FGsSchemaInvadeBattleLevel*> RowsMap;
		if (!CreateAllRowsMap<const FGsSchemaInvadeBattleLevel>(RowsMap))
		{
			return;
		}
		_indexById.Empty(RowsMap.Num());
		for (auto Row : RowsMap)
		{
			_indexById.Emplace(Row.Value->battleLevel, Row.Key);
		}
	}
	
public:
	bool FindRowById(const int32& InId, OUT const FGsSchemaInvadeBattleLevel*& OutRow) const
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
		const FGsSchemaInvadeBattleLevel* Temp;
		return !FindRowById(FGsSchemaInvadeBattleLevel().battleLevel, Temp);
	}
#endif
};
