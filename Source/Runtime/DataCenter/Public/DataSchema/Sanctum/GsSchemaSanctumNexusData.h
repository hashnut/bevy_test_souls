/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/CoreUObject/Public/UObject/NoExportTypes.h"
#include "Runtime/DataCenter/Public/DataSchema/GameObject/Npc/GsSchemaNpcData.h"
#include "Runtime/DataCenter/Public/DataSchema/Skill/Passivity/GsSchemaPassivitySet.h"
#include "Runtime/DataCenter/Public/DataSchema/GameObject/GsSchemaShapeData.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaSanctumNexusData.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/Sanctum/BSGsSanctumNexusData"))
struct DATACENTER_API FGsSchemaSanctumNexusData
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="성소에 건설될 수호탑Id", DisplayName="id"))
	int32 id;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="기획팀 메모용", DisplayName="descForDesign"))
	FString descForDesign;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="수호탑 이름", DisplayName="nameText"))
	FText nameText;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="레벨별NpcData", DisplayName="npcDataId"))
	TArray<FGsSchemaNpcDataRow> npcDataId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="defaultHpRate"))
	int32 defaultHpRate;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="constructionTime"))
	int32 constructionTime;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="constructionRecoveryHp"))
	int32 constructionRecoveryHp;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="maxConstructionUnitCount"))
	int32 maxConstructionUnitCount;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="accelerationPerUnit"))
	int32 accelerationPerUnit;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="건설 단계 퍼센트", DisplayName="constructionStepHpRateList"))
	TArray<int32> constructionStepHpRateList;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="dodgeRate"))
	int32 dodgeRate;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="Hp회복 주기", DisplayName="recoveryHpTickTime"))
	int32 recoveryHpTickTime;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="Hp 회복 값", DisplayName="recoveryHpValue"))
	int32 recoveryHpValue;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="레벨 별 건설 버프", DisplayName="constructionBuffList"))
	TArray<FGsSchemaPassivitySetRow> constructionBuffList;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="interactionRange"))
	int32 interactionRange;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="completeCampShapeIdList"))
	TArray<FGsSchemaShapeDataRow> completeCampShapeIdList;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="imagePath"))
	FSoftObjectPath imagePath;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="campStaticMeshPath"))
	FSoftObjectPath campStaticMeshPath;
public:
	FGsSchemaSanctumNexusData();
	
protected:
	bool operator== (const FGsSchemaSanctumNexusData& __Other) const
	{
		return FGsSchemaSanctumNexusData::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaSanctumNexusData::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaSanctumNexusData::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaSanctumNexusDataRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaSanctumNexusDataRow() : Super()
	{
	}
	
	const FGsSchemaSanctumNexusData* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaSanctumNexusData* Row = nullptr;
		return Table->FindRow<FGsSchemaSanctumNexusData>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaSanctumNexusDataRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaSanctumNexusDataRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableSanctumNexusData final : public UGsTable
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
		TMap<FName, const FGsSchemaSanctumNexusData*> RowsMap;
		if (!CreateAllRowsMap<const FGsSchemaSanctumNexusData>(RowsMap))
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
	bool FindRowById(const int32& InId, OUT const FGsSchemaSanctumNexusData*& OutRow) const
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
		const FGsSchemaSanctumNexusData* Temp;
		return !FindRowById(FGsSchemaSanctumNexusData().id, Temp);
	}
#endif
};
