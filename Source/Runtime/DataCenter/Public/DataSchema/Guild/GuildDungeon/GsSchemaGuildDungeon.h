/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/CoreUObject/Public/UObject/NoExportTypes.h"
#include "Runtime/DataCenter/Public/DataSchema/Guild/GuildDungeon/GsSchemaGuildDungeonBuff.h"
#include "Runtime/DataCenter/Public/DataSchema/Map/GsSchemaMapData.h"
#include "Runtime/DataCenter/Public/DataSchema/GameObject/Npc/GsSchemaNpcData.h"
#include "Runtime/DataCenter/Public/DataSchema/Reward/GsSchemaRewardData.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaGuildDungeon.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/Guild/GuildDungeon/BSGsGuildDungeon"))
struct DATACENTER_API FGsSchemaGuildDungeon
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="id"))
	int32 id;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="level"))
	int32 level;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="name"))
	FText name;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="desc"))
	FString desc;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="dungeonBossImg"))
	FSoftObjectPath dungeonBossImg;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="mapId"))
	FGsSchemaMapDataRow mapId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="bossId"))
	FGsSchemaNpcDataRow bossId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="rewardPersonList"))
	TArray<FGsSchemaRewardDataRow> rewardPersonList;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="rewardGuildList"))
	TArray<FGsSchemaRewardDataRow> rewardGuildList;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="rewardJoinList"))
	TArray<FGsSchemaRewardDataRow> rewardJoinList;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="buffList"))
	TArray<FGsSchemaGuildDungeonBuff> buffList;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="cameraRot"))
	FRotator cameraRot;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="cameraMinDist"))
	float cameraMinDist;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="cameraMaxDist"))
	float cameraMaxDist;
public:
	FGsSchemaGuildDungeon();
	
protected:
	bool operator== (const FGsSchemaGuildDungeon& __Other) const
	{
		return FGsSchemaGuildDungeon::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaGuildDungeon::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaGuildDungeon::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaGuildDungeonRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaGuildDungeonRow() : Super()
	{
	}
	
	const FGsSchemaGuildDungeon* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaGuildDungeon* Row = nullptr;
		return Table->FindRow<FGsSchemaGuildDungeon>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaGuildDungeonRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaGuildDungeonRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableGuildDungeon final : public UGsTable
{
	GENERATED_BODY()

protected:
	TMap<int32, FName> _indexById;

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("Guild/GuildDungeon");
	}
	
	void OnCachedDataTableChanged() final
	{
		TMap<FName, const FGsSchemaGuildDungeon*> RowsMap;
		if (!CreateAllRowsMap<const FGsSchemaGuildDungeon>(RowsMap))
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
	bool FindRowById(const int32& InId, OUT const FGsSchemaGuildDungeon*& OutRow) const
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
		const FGsSchemaGuildDungeon* Temp;
		return !FindRowById(FGsSchemaGuildDungeon().id, Temp);
	}
#endif
};
