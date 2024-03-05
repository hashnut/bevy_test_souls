/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "Shared/Client/SharedEnums/SharedFairyEnum.h"
#include "GsSchemaTitleConditionDataFairy.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/Title/BSGsTitleConditionDataFairy"))
struct DATACENTER_API FGsSchemaTitleConditionDataFairy
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="mFairyId"))
	int32 mFairyId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="mFairyGrade"))
	FairyGrade mFairyGrade;
public:
	FGsSchemaTitleConditionDataFairy();
	
protected:
	bool operator== (const FGsSchemaTitleConditionDataFairy& __Other) const
	{
		return FGsSchemaTitleConditionDataFairy::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaTitleConditionDataFairy::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaTitleConditionDataFairy::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaTitleConditionDataFairyRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaTitleConditionDataFairyRow() : Super()
	{
	}
	
	const FGsSchemaTitleConditionDataFairy* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaTitleConditionDataFairy* Row = nullptr;
		return Table->FindRow<FGsSchemaTitleConditionDataFairy>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaTitleConditionDataFairyRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaTitleConditionDataFairyRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableTitleConditionDataFairy final : public UGsTable
{
	GENERATED_BODY()

protected:
	TMap<int32, FName> _indexById;

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("Title");
	}
	
	void OnCachedDataTableChanged() final
	{
		TMap<FName, const FGsSchemaTitleConditionDataFairy*> RowsMap;
		if (!CreateAllRowsMap<const FGsSchemaTitleConditionDataFairy>(RowsMap))
		{
			return;
		}
		_indexById.Empty(RowsMap.Num());
		for (auto Row : RowsMap)
		{
			_indexById.Emplace(Row.Value->mFairyId, Row.Key);
		}
	}
	
public:
	bool FindRowById(const int32& InId, OUT const FGsSchemaTitleConditionDataFairy*& OutRow) const
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
		const FGsSchemaTitleConditionDataFairy* Temp;
		return !FindRowById(FGsSchemaTitleConditionDataFairy().mFairyId, Temp);
	}
#endif
};
