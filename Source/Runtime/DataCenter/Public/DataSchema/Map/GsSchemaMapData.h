/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/DataCenter/Public/DataSchema/Map/GsSchemaMapEnterCondition.h"
#include "Runtime/DataCenter/Public/GameObject/Define/EGsArtLevelLoadConditionType.h"
#include "Runtime/DataCenter/Public/GameObject/Define/EGsFootPrintMethod.h"
#include "Runtime/DataCenter/Public/DataSchema/Map/Area/GsSchemaAreaData.h"
#include "Runtime/DataCenter/Public/DataSchema/Loading/GsSchemaLoadingScreenGroup.h"
#include "Runtime/DataCenter/Public/DataSchema/Map/GsSchemaMapBalance.h"
#include "Runtime/DataCenter/Public/DataSchema/Map/GsSchemaMapTerritoryData.h"
#include "Runtime/DataCenter/Public/DataSchema/Sound/GsSchemaSoundResData.h"
#include "Runtime/DataCenter/Public/Shared/Client/SharedEnums/SharedCommonEnum.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
class UParticleSystem;
class UGsPhysicalMaterial;
#include "GsSchemaMapData.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/Map/BSGsMapData"))
struct DATACENTER_API FGsSchemaMapData
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="id"))
	int32 id;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="nameText"))
	FString nameText;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="areaId"))
	FGsSchemaAreaDataRow areaId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="territoryInfo"))
	FGsSchemaMapTerritoryDataRow territoryInfo;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="mapType"))
	MapType mapType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="ContentsType"))
	MapContentsType ContentsType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="MapStringId"))
	FText MapStringId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="bgm"))
	FGsSchemaSoundResDataRow bgm;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="isCrackSpawn"))
	bool isCrackSpawn;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="서버가 보여는 주간 갯수(1~10 사이)", DisplayName="visibleRange"))
	int32 visibleRange;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="footPrintEffect"))
	UParticleSystem* footPrintEffect;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="footPrintMethod"))
	EGsFootPrintMethod footPrintMethod;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="footPrintMaterial"))
	UGsPhysicalMaterial* footPrintMaterial;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="enterCondition"))
	FGsSchemaMapEnterCondition enterCondition;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="isTestLevel"))
	bool isTestLevel;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="artLevelLoadCondition"))
	EGsArtLevelLoadConditionType artLevelLoadCondition;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="대상 맵 로딩시 사용할 그룹", DisplayName="loadingScreenGroup"))
	FGsSchemaLoadingScreenGroupRow loadingScreenGroup;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="balanceId"))
	FGsSchemaMapBalanceRow balanceId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="offlinePlayDeny"))
	bool offlinePlayDeny;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="isUseAutoReturn"))
	bool isUseAutoReturn;
public:
	FGsSchemaMapData();
	
protected:
	bool operator== (const FGsSchemaMapData& __Other) const
	{
		return FGsSchemaMapData::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaMapData::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaMapData::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaMapDataRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaMapDataRow() : Super()
	{
	}
	
	const FGsSchemaMapData* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaMapData* Row = nullptr;
		return Table->FindRow<FGsSchemaMapData>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaMapDataRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaMapDataRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableMapData final : public UGsTable
{
	GENERATED_BODY()

protected:
	TMap<int32, FName> _indexById;

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("Map");
	}
	
	void OnCachedDataTableChanged() final
	{
		TMap<FName, const FGsSchemaMapData*> RowsMap;
		if (!CreateAllRowsMap<const FGsSchemaMapData>(RowsMap))
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
	bool FindRowById(const int32& InId, OUT const FGsSchemaMapData*& OutRow) const
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
		const FGsSchemaMapData* Temp;
		return !FindRowById(FGsSchemaMapData().id, Temp);
	}
#endif
};
