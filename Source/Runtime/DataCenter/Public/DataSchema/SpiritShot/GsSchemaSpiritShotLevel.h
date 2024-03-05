/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/DataCenter/Public/DataSchema/SpiritShot/GsSchemaSpiritShotEnchant.h"
#include "Runtime/DataCenter/Public/DataSchema/SpiritShot/GsSchemaSpiritShotRequirement.h"
#include "Runtime/CoreUObject/Public/UObject/NoExportTypes.h"
#include "Runtime/DataCenter/Public/DataSchema/Skill/Passivity/GsSchemaPassivitySet.h"
#include "Runtime/DataCenter/Public/DataSchema/Reward/GsSchemaRewardData.h"
#include "Runtime/DataCenter/Public/DataSchema/SpiritShot/GsSchemaSpiritShotCategory.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaSpiritShotLevel.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/SpiritShot/BSGsSpiritShotLevel"))
struct DATACENTER_API FGsSchemaSpiritShotLevel
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="id"))
	int32 id;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="categoryId"))
	FGsSchemaSpiritShotCategoryRow categoryId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="level"))
	int32 level;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="enchantData"))
	FGsSchemaSpiritShotEnchant enchantData;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="requireGold"))
	int32 requireGold;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="requireItemList"))
	TArray<FGsSchemaSpiritShotRequirement> requireItemList;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="failRewardId"))
	FGsSchemaRewardDataRow failRewardId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="passivityId"))
	FGsSchemaPassivitySetRow passivityId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="stepImagePath"))
	FSoftObjectPath stepImagePath;
public:
	FGsSchemaSpiritShotLevel();
	
protected:
	bool operator== (const FGsSchemaSpiritShotLevel& __Other) const
	{
		return FGsSchemaSpiritShotLevel::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaSpiritShotLevel::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaSpiritShotLevel::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaSpiritShotLevelRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaSpiritShotLevelRow() : Super()
	{
	}
	
	const FGsSchemaSpiritShotLevel* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaSpiritShotLevel* Row = nullptr;
		return Table->FindRow<FGsSchemaSpiritShotLevel>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaSpiritShotLevelRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaSpiritShotLevelRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableSpiritShotLevel final : public UGsTable
{
	GENERATED_BODY()

protected:
	TMap<int32, FName> _indexById;

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("SpiritShot");
	}
	
	void OnCachedDataTableChanged() final
	{
		TMap<FName, const FGsSchemaSpiritShotLevel*> RowsMap;
		if (!CreateAllRowsMap<const FGsSchemaSpiritShotLevel>(RowsMap))
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
	bool FindRowById(const int32& InId, OUT const FGsSchemaSpiritShotLevel*& OutRow) const
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
		const FGsSchemaSpiritShotLevel* Temp;
		return !FindRowById(FGsSchemaSpiritShotLevel().id, Temp);
	}
#endif
};
