/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/DataCenter/Public/DataSchema/Map/GsSchemaMapData.h"
#include "Runtime/DataCenter/Public/DataSchema/GameObject/Npc/GsSchemaNpcData.h"
#include "Runtime/DataCenter/Public/DataSchema/Skill/Passivity/GsSchemaPassivitySet.h"
#include "Runtime/DataCenter/Public/DataSchema/Sanctum/GsSchemaSanctumNexusData.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaSanctumData.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/Sanctum/BSGsSanctumData"))
struct DATACENTER_API FGsSchemaSanctumData
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="성소Id", DisplayName="sanctumAreaId"))
	int32 sanctumAreaId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="기획팀 메모용", DisplayName="descForDesign"))
	FString descForDesign;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="성소 이름", DisplayName="nameText"))
	FText nameText;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="설치될 수호탑Id", DisplayName="sanctumNexusId"))
	FGsSchemaSanctumNexusDataRow sanctumNexusId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="점령에 필요한 수호탑 레벨", DisplayName="needNexusLevel"))
	int32 needNexusLevel;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="성소가 배치된 MapId", DisplayName="sanctumMapId"))
	FGsSchemaMapDataRow sanctumMapId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="스폰 위치 정보", DisplayName="preSpawnGroupId"))
	int32 preSpawnGroupId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="스폰될 수호탑의 NpcId", DisplayName="interactionSpawnNpcId"))
	FGsSchemaNpcDataRow interactionSpawnNpcId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="점령한 기사단에 제공할 PassivityId", DisplayName="rewardPassivityId"))
	FGsSchemaPassivitySetRow rewardPassivityId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="activeInBattle"))
	bool activeInBattle;
public:
	FGsSchemaSanctumData();
	
protected:
	bool operator== (const FGsSchemaSanctumData& __Other) const
	{
		return FGsSchemaSanctumData::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaSanctumData::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaSanctumData::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaSanctumDataRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaSanctumDataRow() : Super()
	{
	}
	
	const FGsSchemaSanctumData* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaSanctumData* Row = nullptr;
		return Table->FindRow<FGsSchemaSanctumData>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaSanctumDataRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaSanctumDataRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableSanctumData final : public UGsTable
{
	GENERATED_BODY()

protected:
	TMap<int32, FName> _indexById;

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("Sanctum");
	}
	
	void OnCachedDataTableChanged() final
	{
		TMap<FName, const FGsSchemaSanctumData*> RowsMap;
		if (!CreateAllRowsMap<const FGsSchemaSanctumData>(RowsMap))
		{
			return;
		}
		_indexById.Empty(RowsMap.Num());
		for (auto Row : RowsMap)
		{
			_indexById.Emplace(Row.Value->sanctumAreaId, Row.Key);
		}
	}
	
public:
	bool FindRowById(const int32& InId, OUT const FGsSchemaSanctumData*& OutRow) const
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
		const FGsSchemaSanctumData* Temp;
		return !FindRowById(FGsSchemaSanctumData().sanctumAreaId, Temp);
	}
#endif
};
