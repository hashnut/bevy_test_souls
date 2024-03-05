/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/DataCenter/Public/Shared/Client/SharedEnums/SharedIffEnum.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaIffFactionData.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/GameObject/Common/BSGsIffFactionData"))
struct DATACENTER_API FGsSchemaIffFactionData
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="iffFactionType"))
	IffFactionType iffFactionType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="enemyFactionTypeList"))
	TArray<IffFactionType> enemyFactionTypeList;
public:
	FGsSchemaIffFactionData();
	
protected:
	bool operator== (const FGsSchemaIffFactionData& __Other) const
	{
		return FGsSchemaIffFactionData::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaIffFactionData::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaIffFactionData::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaIffFactionDataRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaIffFactionDataRow() : Super()
	{
	}
	
	const FGsSchemaIffFactionData* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaIffFactionData* Row = nullptr;
		return Table->FindRow<FGsSchemaIffFactionData>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaIffFactionDataRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaIffFactionDataRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableIffFactionData final : public UGsTable
{
	GENERATED_BODY()

protected:
	TMap<IffFactionType, FName> _indexById;

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("GameObject/Common");
	}
	
	void OnCachedDataTableChanged() final
	{
		TMap<FName, const FGsSchemaIffFactionData*> RowsMap;
		if (!CreateAllRowsMap<const FGsSchemaIffFactionData>(RowsMap))
		{
			return;
		}
		_indexById.Empty(RowsMap.Num());
		for (auto Row : RowsMap)
		{
			_indexById.Emplace(Row.Value->iffFactionType, Row.Key);
		}
	}
	
public:
	bool FindRowById(const IffFactionType& InId, OUT const FGsSchemaIffFactionData*& OutRow) const
	{
		return FindInIndexMap(_indexById, InId, OutRow);
	}
	
#if WITH_EDITOR
	void RemoveRowById(const IffFactionType& InId)
	{
		RemoveInIndexMap(_indexById, InId);
	}
	
	bool IsValidAllPrimaryKeys() const final
	{
		const FGsSchemaIffFactionData* Temp;
		return !FindRowById(FGsSchemaIffFactionData().iffFactionType, Temp);
	}
#endif
};
