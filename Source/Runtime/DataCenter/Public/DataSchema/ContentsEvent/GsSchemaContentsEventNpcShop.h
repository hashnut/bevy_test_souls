/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaContentsEventNpcShop.generated.h"

USTRUCT(BlueprintType, Meta=(Tooltip="컨텐츠이벤트 - NPC상점. 이벤트에 사용할 NPC On/Off 기능. 지정된 npc id 를 모든 맵에서 검색하여 On/Off 를 수행함", AssetSchemaPackagePath="/Game/DataCenter/DataSchema/ContentsEvent/BSGSContentsEventNpcShop"))
struct DATACENTER_API FGsSchemaContentsEventNpcShop
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="id"))
	int32 id;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="NpcEventShopSchedulerData 테이블의 groupId", DisplayName="npcEventShopSchedulerDataGroupId"))
	int32 npcEventShopSchedulerDataGroupId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="시작 Cron", DisplayName="startCron"))
	FString startCron;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="유지 시간", DisplayName="durationMinutes"))
	int32 durationMinutes;
public:
	FGsSchemaContentsEventNpcShop();
	
protected:
	bool operator== (const FGsSchemaContentsEventNpcShop& __Other) const
	{
		return FGsSchemaContentsEventNpcShop::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaContentsEventNpcShop::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaContentsEventNpcShop::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaContentsEventNpcShopRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaContentsEventNpcShopRow() : Super()
	{
	}
	
	const FGsSchemaContentsEventNpcShop* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaContentsEventNpcShop* Row = nullptr;
		return Table->FindRow<FGsSchemaContentsEventNpcShop>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaContentsEventNpcShopRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaContentsEventNpcShopRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableContentsEventNpcShop final : public UGsTable
{
	GENERATED_BODY()

protected:
	TMap<int32, FName> _indexById;

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("ContentsEvent");
	}
	
	void OnCachedDataTableChanged() final
	{
		TMap<FName, const FGsSchemaContentsEventNpcShop*> RowsMap;
		if (!CreateAllRowsMap<const FGsSchemaContentsEventNpcShop>(RowsMap))
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
	bool FindRowById(const int32& InId, OUT const FGsSchemaContentsEventNpcShop*& OutRow) const
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
		const FGsSchemaContentsEventNpcShop* Temp;
		return !FindRowById(FGsSchemaContentsEventNpcShop().id, Temp);
	}
#endif
};
