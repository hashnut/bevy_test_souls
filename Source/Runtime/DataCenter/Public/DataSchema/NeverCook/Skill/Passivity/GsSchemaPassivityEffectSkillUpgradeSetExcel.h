/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaPassivityEffectSkillUpgradeSetExcel.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/NeverCook/Skill/Passivity/BSGsPassivityEffectSkillUpgradeSetExcel"))
struct DATACENTER_API FGsSchemaPassivityEffectSkillUpgradeSetExcel
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="id"))
	int32 id;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="currentStep"))
	int32 currentStep;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="skillUpgradeId"))
	FName skillUpgradeId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="essentialPassivityId"))
	int32 essentialPassivityId;
public:
	FGsSchemaPassivityEffectSkillUpgradeSetExcel();
	
protected:
	bool operator== (const FGsSchemaPassivityEffectSkillUpgradeSetExcel& __Other) const
	{
		return FGsSchemaPassivityEffectSkillUpgradeSetExcel::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaPassivityEffectSkillUpgradeSetExcel::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaPassivityEffectSkillUpgradeSetExcel::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaPassivityEffectSkillUpgradeSetExcelRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaPassivityEffectSkillUpgradeSetExcelRow() : Super()
	{
	}
	
	const FGsSchemaPassivityEffectSkillUpgradeSetExcel* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaPassivityEffectSkillUpgradeSetExcel* Row = nullptr;
		return Table->FindRow<FGsSchemaPassivityEffectSkillUpgradeSetExcel>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaPassivityEffectSkillUpgradeSetExcelRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaPassivityEffectSkillUpgradeSetExcelRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTablePassivityEffectSkillUpgradeSetExcel final : public UGsTable
{
	GENERATED_BODY()

protected:
	TMap<int32, FName> _indexById;

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("NeverCook/Skill/Passivity");
	}
	
	void OnCachedDataTableChanged() final
	{
		TMap<FName, const FGsSchemaPassivityEffectSkillUpgradeSetExcel*> RowsMap;
		if (!CreateAllRowsMap<const FGsSchemaPassivityEffectSkillUpgradeSetExcel>(RowsMap))
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
	bool FindRowById(const int32& InId, OUT const FGsSchemaPassivityEffectSkillUpgradeSetExcel*& OutRow) const
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
		const FGsSchemaPassivityEffectSkillUpgradeSetExcel* Temp;
		return !FindRowById(FGsSchemaPassivityEffectSkillUpgradeSetExcel().id, Temp);
	}
#endif
};
