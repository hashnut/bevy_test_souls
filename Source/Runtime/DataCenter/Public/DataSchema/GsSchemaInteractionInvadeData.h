/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/CoreUObject/Public/UObject/NoExportTypes.h"
#include "Runtime/DataCenter/Public/Shared/Client/SharedEnums/SharedCreatureEnum.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaInteractionInvadeData.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/BSGsInteractionInvadeData"))
struct DATACENTER_API FGsSchemaInteractionInvadeData
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="_npcFunctionType"))
	NpcFunctionType _npcFunctionType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="_iconImagePath"))
	TArray<FSoftObjectPath> _iconImagePath;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="_iconColor"))
	FLinearColor _iconColor;
public:
	FGsSchemaInteractionInvadeData();
	
protected:
	bool operator== (const FGsSchemaInteractionInvadeData& __Other) const
	{
		return FGsSchemaInteractionInvadeData::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaInteractionInvadeData::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaInteractionInvadeData::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaInteractionInvadeDataRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaInteractionInvadeDataRow() : Super()
	{
	}
	
	const FGsSchemaInteractionInvadeData* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaInteractionInvadeData* Row = nullptr;
		return Table->FindRow<FGsSchemaInteractionInvadeData>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaInteractionInvadeDataRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaInteractionInvadeDataRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableInteractionInvadeData final : public UGsTable
{
	GENERATED_BODY()

protected:
	TMap<NpcFunctionType, FName> _indexById;

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("");
	}
	
	void OnCachedDataTableChanged() final
	{
		TMap<FName, const FGsSchemaInteractionInvadeData*> RowsMap;
		if (!CreateAllRowsMap<const FGsSchemaInteractionInvadeData>(RowsMap))
		{
			return;
		}
		_indexById.Empty(RowsMap.Num());
		for (auto Row : RowsMap)
		{
			_indexById.Emplace(Row.Value->_npcFunctionType, Row.Key);
		}
	}
	
public:
	bool FindRowById(const NpcFunctionType& InId, OUT const FGsSchemaInteractionInvadeData*& OutRow) const
	{
		return FindInIndexMap(_indexById, InId, OutRow);
	}
	
#if WITH_EDITOR
	void RemoveRowById(const NpcFunctionType& InId)
	{
		RemoveInIndexMap(_indexById, InId);
	}
	
	bool IsValidAllPrimaryKeys() const final
	{
		const FGsSchemaInteractionInvadeData* Temp;
		return !FindRowById(FGsSchemaInteractionInvadeData()._npcFunctionType, Temp);
	}
#endif
};
