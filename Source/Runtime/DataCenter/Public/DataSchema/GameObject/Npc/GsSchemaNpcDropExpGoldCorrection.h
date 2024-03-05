/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaNpcDropExpGoldCorrection.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/GameObject/Npc/BSGsNpcDropExpGoldCorrection"))
struct DATACENTER_API FGsSchemaNpcDropExpGoldCorrection
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="id"))
	int32 id;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="expPlusCorrectionValue"))
	int32 expPlusCorrectionValue;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="goldPlusCorrectionValue"))
	int32 goldPlusCorrectionValue;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="expMultiplyCorrectionValue"))
	int32 expMultiplyCorrectionValue;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="goldMultiplyCorrectionValue"))
	int32 goldMultiplyCorrectionValue;
public:
	FGsSchemaNpcDropExpGoldCorrection();
	
protected:
	bool operator== (const FGsSchemaNpcDropExpGoldCorrection& __Other) const
	{
		return FGsSchemaNpcDropExpGoldCorrection::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaNpcDropExpGoldCorrection::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaNpcDropExpGoldCorrection::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaNpcDropExpGoldCorrectionRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaNpcDropExpGoldCorrectionRow() : Super()
	{
	}
	
	const FGsSchemaNpcDropExpGoldCorrection* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaNpcDropExpGoldCorrection* Row = nullptr;
		return Table->FindRow<FGsSchemaNpcDropExpGoldCorrection>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaNpcDropExpGoldCorrectionRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaNpcDropExpGoldCorrectionRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableNpcDropExpGoldCorrection final : public UGsTable
{
	GENERATED_BODY()

protected:
	TMap<int32, FName> _indexById;

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("GameObject/Npc");
	}
	
	void OnCachedDataTableChanged() final
	{
		TMap<FName, const FGsSchemaNpcDropExpGoldCorrection*> RowsMap;
		if (!CreateAllRowsMap<const FGsSchemaNpcDropExpGoldCorrection>(RowsMap))
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
	bool FindRowById(const int32& InId, OUT const FGsSchemaNpcDropExpGoldCorrection*& OutRow) const
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
		const FGsSchemaNpcDropExpGoldCorrection* Temp;
		return !FindRowById(FGsSchemaNpcDropExpGoldCorrection().id, Temp);
	}
#endif
};
