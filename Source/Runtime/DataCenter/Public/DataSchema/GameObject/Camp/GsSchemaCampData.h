/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/DataCenter/Public/DataSchema/GameObject/Camp/GsSchemaConstructCampInfo.h"
#include "Runtime/CoreUObject/Public/UObject/NoExportTypes.h"
#include "Runtime/DataCenter/Public/DataSchema/GameObject/Npc/GsSchemaNpcData.h"
#include "Runtime/DataCenter/Public/DataSchema/GameObject/GsSchemaShapeData.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
class UMaterialInterface;
#include "GsSchemaCampData.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/GameObject/Camp/BSGsCampData"))
struct DATACENTER_API FGsSchemaCampData
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="id"))
	int32 id;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="nameText"))
	FText nameText;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="설치건물 기본정보", DisplayName="constructionInfo"))
	FGsSchemaConstructCampInfo constructionInfo;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="npcDataIdList"))
	TArray<FGsSchemaNpcDataRow> npcDataIdList;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="staticmeshMaterial"))
	UMaterialInterface* staticmeshMaterial;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="defaultHpRate"))
	int32 defaultHpRate;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="constructionTime"))
	int32 constructionTime;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="constructionRecoveryHp"))
	int32 constructionRecoveryHp;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="maxConstructionUnitCount"))
	int32 maxConstructionUnitCount;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="accelerationPerUnit"))
	int32 accelerationPerUnit;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="constructionStepHpRateList"))
	TArray<int32> constructionStepHpRateList;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="campLifeTime"))
	int32 campLifeTime;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="dodgeRate"))
	int32 dodgeRate;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="recoveryHpTickTime"))
	int32 recoveryHpTickTime;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="recoveryHpValue"))
	int32 recoveryHpValue;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="constructionBuffList"))
	TArray<int32> constructionBuffList;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="areaBuffList"))
	TArray<int32> areaBuffList;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="areaBuffRadius"))
	int32 areaBuffRadius;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="completeCampShapeId"))
	TArray<FGsSchemaShapeDataRow> completeCampShapeId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="imagePath"))
	FSoftObjectPath imagePath;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="campStaticMeshPath"))
	FSoftObjectPath campStaticMeshPath;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="정보 팝업 UI의 탭 순서", DisplayName="infoUITabIndex"))
	int32 infoUITabIndex;
public:
	FGsSchemaCampData();
	
protected:
	bool operator== (const FGsSchemaCampData& __Other) const
	{
		return FGsSchemaCampData::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaCampData::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaCampData::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaCampDataRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaCampDataRow() : Super()
	{
	}
	
	const FGsSchemaCampData* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaCampData* Row = nullptr;
		return Table->FindRow<FGsSchemaCampData>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaCampDataRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaCampDataRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableCampData final : public UGsTable
{
	GENERATED_BODY()

protected:
	TMap<int32, FName> _indexById;

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("GameObject/Camp");
	}
	
	void OnCachedDataTableChanged() final
	{
		TMap<FName, const FGsSchemaCampData*> RowsMap;
		if (!CreateAllRowsMap<const FGsSchemaCampData>(RowsMap))
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
	bool FindRowById(const int32& InId, OUT const FGsSchemaCampData*& OutRow) const
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
		const FGsSchemaCampData* Temp;
		return !FindRowById(FGsSchemaCampData().id, Temp);
	}
#endif
};
