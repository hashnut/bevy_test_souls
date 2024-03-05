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
#include "GsSchemaNpcFunctionData.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/GameObject/Npc/BSGsNpcFunctionData"))
struct DATACENTER_API FGsSchemaNpcFunctionData
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="_npcFunctionType"))
	NpcFunctionType _npcFunctionType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="_isRepresentIcon"))
	bool _isRepresentIcon;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="_slotIconImagePath"))
	FSoftObjectPath _slotIconImagePath;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="_autoMoveIconImagePath"))
	FSoftObjectPath _autoMoveIconImagePath;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="_slotOrderNumber"))
	int32 _slotOrderNumber;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="_slotOrderNumberBattleArena"))
	int32 _slotOrderNumberBattleArena;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="_slotOrderNumberAgit"))
	int32 _slotOrderNumberAgit;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="_slotFunctionName"))
	FText _slotFunctionName;
public:
	FGsSchemaNpcFunctionData();
	
protected:
	bool operator== (const FGsSchemaNpcFunctionData& __Other) const
	{
		return FGsSchemaNpcFunctionData::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaNpcFunctionData::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaNpcFunctionData::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaNpcFunctionDataRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaNpcFunctionDataRow() : Super()
	{
	}
	
	const FGsSchemaNpcFunctionData* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaNpcFunctionData* Row = nullptr;
		return Table->FindRow<FGsSchemaNpcFunctionData>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaNpcFunctionDataRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaNpcFunctionDataRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableNpcFunctionData final : public UGsTable
{
	GENERATED_BODY()

protected:
	TMap<NpcFunctionType, FName> _indexById;

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("GameObject/Npc");
	}
	
	void OnCachedDataTableChanged() final
	{
		TMap<FName, const FGsSchemaNpcFunctionData*> RowsMap;
		if (!CreateAllRowsMap<const FGsSchemaNpcFunctionData>(RowsMap))
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
	bool FindRowById(const NpcFunctionType& InId, OUT const FGsSchemaNpcFunctionData*& OutRow) const
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
		const FGsSchemaNpcFunctionData* Temp;
		return !FindRowById(FGsSchemaNpcFunctionData()._npcFunctionType, Temp);
	}
#endif
};
