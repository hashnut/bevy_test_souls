/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/DataCenter/Public/Shared/Client/SharedEnums/SharedCreatureEnum.h"
#include "Runtime/DataCenter/Public/DataSchema/Item/GsSchemaItemCommon.h"
#include "Runtime/DataCenter/Public/DataSchema/GameObject/Npc/GsSchemaNpcData.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaWorldMapMonsterElementData.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/Map/WorldMap/BSGsWorldMapMonsterElementData"))
struct DATACENTER_API FGsSchemaWorldMapMonsterElementData
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="id"))
	FGsSchemaNpcDataRow id;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="mapId"))
	int32 mapId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="level"))
	int32 level;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="isOffensive"))
	bool isOffensive;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="rank"))
	CreatureRankType rank;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="equipItemList"))
	TArray<FGsSchemaItemCommonRow> equipItemList;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="etcItemList"))
	TArray<FGsSchemaItemCommonRow> etcItemList;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="invasionAttackEquipItemList"))
	TArray<FGsSchemaItemCommonRow> invasionAttackEquipItemList;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="invasionAttackEtcItemList"))
	TArray<FGsSchemaItemCommonRow> invasionAttackEtcItemList;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="invasionDefenceEquipItemList"))
	TArray<FGsSchemaItemCommonRow> invasionDefenceEquipItemList;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="invasionDefenceEtcItemList"))
	TArray<FGsSchemaItemCommonRow> invasionDefenceEtcItemList;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="isInvasion"))
	bool isInvasion;
public:
	FGsSchemaWorldMapMonsterElementData();
	
protected:
	bool operator== (const FGsSchemaWorldMapMonsterElementData& __Other) const
	{
		return FGsSchemaWorldMapMonsterElementData::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaWorldMapMonsterElementData::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaWorldMapMonsterElementData::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaWorldMapMonsterElementDataRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaWorldMapMonsterElementDataRow() : Super()
	{
	}
	
	const FGsSchemaWorldMapMonsterElementData* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaWorldMapMonsterElementData* Row = nullptr;
		return Table->FindRow<FGsSchemaWorldMapMonsterElementData>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaWorldMapMonsterElementDataRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaWorldMapMonsterElementDataRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableWorldMapMonsterElementData final : public UGsTable
{
	GENERATED_BODY()

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("Map/WorldMap");
	}
	
};
