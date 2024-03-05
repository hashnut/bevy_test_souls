/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "DataSchema/Skill/GsSchemaSkillEnchantInfo.h"
#include "Shared/Client/SharedEnums/SharedItemEnum.h"
#include "GsSchemaSkillEnchantSet.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/Skill/BSGsSkillEnchantSet"))
struct DATACENTER_API FGsSchemaSkillEnchantSet
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="grade"))
	ItemGrade grade;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="개발자용노트"))
	FString note;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="enchantInfoList"))
	TArray<FGsSchemaSkillEnchantInfo> enchantInfoList;
public:
	FGsSchemaSkillEnchantSet();
	
protected:
	bool operator== (const FGsSchemaSkillEnchantSet& __Other) const
	{
		return FGsSchemaSkillEnchantSet::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaSkillEnchantSet::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaSkillEnchantSet::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaSkillEnchantSetRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaSkillEnchantSetRow() : Super()
	{
	}
	
	const FGsSchemaSkillEnchantSet* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaSkillEnchantSet* Row = nullptr;
		return Table->FindRow<FGsSchemaSkillEnchantSet>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaSkillEnchantSetRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaSkillEnchantSetRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableSkillEnchantSet final : public UGsTable
{
	GENERATED_BODY()

protected:
	TMap<ItemGrade, FName> _indexById;

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("Skill");
	}
	
	void OnCachedDataTableChanged() final
	{
		TMap<FName, const FGsSchemaSkillEnchantSet*> RowsMap;
		if (!CreateAllRowsMap<const FGsSchemaSkillEnchantSet>(RowsMap))
		{
			return;
		}
		_indexById.Empty(RowsMap.Num());
		for (auto Row : RowsMap)
		{
			_indexById.Emplace(Row.Value->grade, Row.Key);
		}
	}
	
public:
	bool FindRowById(const ItemGrade& InId, OUT const FGsSchemaSkillEnchantSet*& OutRow) const
	{
		return FindInIndexMap(_indexById, InId, OutRow);
	}
	
#if WITH_EDITOR
	void RemoveRowById(const ItemGrade& InId)
	{
		RemoveInIndexMap(_indexById, InId);
	}
	
	bool IsValidAllPrimaryKeys() const final
	{
		const FGsSchemaSkillEnchantSet* Temp;
		return !FindRowById(FGsSchemaSkillEnchantSet().grade, Temp);
	}
#endif
};
