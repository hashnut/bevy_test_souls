/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/DataCenter/Public/DataSchema/SupplyInfo/GsSchemaDropSupplyInfo.h"
#include "Runtime/DataCenter/Public/DataSchema/SupplyInfo/GsSchemaDropSupplyInfoCreatureIds.h"
#include "Runtime/DataCenter/Public/Unlock/GsUnlockDefine.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaSkillSupplyInfoSet.generated.h"

USTRUCT(BlueprintType, Meta=(Tooltip="스킬 공급처 정보 세트.", AssetSchemaPackagePath="/Game/DataCenter/DataSchema/SupplyInfo/BSGsSkillSupplyInfoSet"))
struct DATACENTER_API FGsSchemaSkillSupplyInfoSet
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Meta=(Tooltip="스킬ID", DisplayName="skillId"))
	int32 skillId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="contentsTypeSet"))
	TSet<EGsUnlockContentsType> contentsTypeSet;
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Meta=(DisplayName="contentsEventMapCreatureSetMap"))
	TMap<int32,FGsSchemaDropSupplyInfo> contentsEventMapCreatureSetMap;
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Meta=(DisplayName="mapCreatureSetMap"))
	TMap<int32,FGsSchemaDropSupplyInfoCreatureIds> mapCreatureSetMap;
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Meta=(DisplayName="craftIdSet"))
	TSet<int32> craftIdSet;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="본 스킬을 획득할 수 있는 모든 아이템ID. 첫번째 아이템을 대표 아이템으로 간주하여 사용함.", DisplayName="itemIds"))
	TArray<int32> itemIds;
public:
	FGsSchemaSkillSupplyInfoSet();
	
protected:
	bool operator== (const FGsSchemaSkillSupplyInfoSet& __Other) const
	{
		return FGsSchemaSkillSupplyInfoSet::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaSkillSupplyInfoSet::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaSkillSupplyInfoSet::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaSkillSupplyInfoSetRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaSkillSupplyInfoSetRow() : Super()
	{
	}
	
	const FGsSchemaSkillSupplyInfoSet* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaSkillSupplyInfoSet* Row = nullptr;
		return Table->FindRow<FGsSchemaSkillSupplyInfoSet>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaSkillSupplyInfoSetRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaSkillSupplyInfoSetRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableSkillSupplyInfoSet final : public UGsTable
{
	GENERATED_BODY()

protected:
	TMap<int32, FName> _indexById;

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("SupplyInfo");
	}
	
	void OnCachedDataTableChanged() final
	{
		TMap<FName, const FGsSchemaSkillSupplyInfoSet*> RowsMap;
		if (!CreateAllRowsMap<const FGsSchemaSkillSupplyInfoSet>(RowsMap))
		{
			return;
		}
		_indexById.Empty(RowsMap.Num());
		for (auto Row : RowsMap)
		{
			_indexById.Emplace(Row.Value->skillId, Row.Key);
		}
	}
	
public:
	bool FindRowById(const int32& InId, OUT const FGsSchemaSkillSupplyInfoSet*& OutRow) const
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
		const FGsSchemaSkillSupplyInfoSet* Temp;
		return !FindRowById(FGsSchemaSkillSupplyInfoSet().skillId, Temp);
	}
#endif
};
