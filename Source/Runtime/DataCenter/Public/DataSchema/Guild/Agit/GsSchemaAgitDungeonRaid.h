/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/CoreUObject/Public/UObject/NoExportTypes.h"
#include "Runtime/DataCenter/Public/DataSchema/Map/GsSchemaMapData.h"
#include "Runtime/DataCenter/Public/DataSchema/GameObject/Npc/GsSchemaNpcData.h"
#include "Runtime/DataCenter/Public/DataSchema/Reward/GsSchemaRewardData.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaAgitDungeonRaid.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/Guild/Agit/BSGsAgitDungeonRaid"))
struct DATACENTER_API FGsSchemaAgitDungeonRaid
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="id"))
	int32 id;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="기획참고용", DisplayName="desc"))
	FString desc;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="던전 시작 가능 기사단 레벨", DisplayName="requiredGuildLevel"))
	int32 requiredGuildLevel;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="mapId"))
	FGsSchemaMapDataRow mapId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="던전 보스 ID", DisplayName="bossId"))
	FGsSchemaNpcDataRow bossId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="개인 보상_우편 지급", DisplayName="rewardPersonList"))
	TArray<FGsSchemaRewardDataRow> rewardPersonList;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="기사단 보상_기사단 창고 지급", DisplayName="rewardGuildList"))
	TArray<FGsSchemaRewardDataRow> rewardGuildList;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="던전 오픈 비용_기사단 운영비", DisplayName="openCost"))
	int32 openCost;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="bossNickName"))
	FText bossNickName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="bossImg"))
	FSoftObjectPath bossImg;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="portraitImg"))
	FSoftObjectPath portraitImg;
public:
	FGsSchemaAgitDungeonRaid();
	
protected:
	bool operator== (const FGsSchemaAgitDungeonRaid& __Other) const
	{
		return FGsSchemaAgitDungeonRaid::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaAgitDungeonRaid::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaAgitDungeonRaid::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaAgitDungeonRaidRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaAgitDungeonRaidRow() : Super()
	{
	}
	
	const FGsSchemaAgitDungeonRaid* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaAgitDungeonRaid* Row = nullptr;
		return Table->FindRow<FGsSchemaAgitDungeonRaid>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaAgitDungeonRaidRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaAgitDungeonRaidRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableAgitDungeonRaid final : public UGsTable
{
	GENERATED_BODY()

protected:
	TMap<int32, FName> _indexById;

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("Guild/Agit");
	}
	
	void OnCachedDataTableChanged() final
	{
		TMap<FName, const FGsSchemaAgitDungeonRaid*> RowsMap;
		if (!CreateAllRowsMap<const FGsSchemaAgitDungeonRaid>(RowsMap))
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
	bool FindRowById(const int32& InId, OUT const FGsSchemaAgitDungeonRaid*& OutRow) const
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
		const FGsSchemaAgitDungeonRaid* Temp;
		return !FindRowById(FGsSchemaAgitDungeonRaid().id, Temp);
	}
#endif
};
