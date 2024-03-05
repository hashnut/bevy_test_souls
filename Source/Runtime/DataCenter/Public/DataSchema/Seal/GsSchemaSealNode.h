/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaSealNode.generated.h"

USTRUCT(BlueprintType, Meta=(Tooltip="봉인노드", AssetSchemaPackagePath="/Game/DataCenter/DataSchema/Seal/BSGsSealNode"))
struct DATACENTER_API FGsSchemaSealNode
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="id"))
	int32 id;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="봉인팩ID", DisplayName="sealPackId"))
	int32 sealPackId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="이름", DisplayName="sealNodeName"))
	FText sealNodeName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="레벨테이블ID", DisplayName="sealNodeLevelId"))
	int32 sealNodeLevelId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="보유한 봉인슬롯을 오픈할 레벨", DisplayName="openSealSlotOpenLevel"))
	int32 openSealSlotOpenLevel;
public:
	FGsSchemaSealNode();
	
protected:
	bool operator== (const FGsSchemaSealNode& __Other) const
	{
		return FGsSchemaSealNode::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaSealNode::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaSealNode::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaSealNodeRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaSealNodeRow() : Super()
	{
	}
	
	const FGsSchemaSealNode* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaSealNode* Row = nullptr;
		return Table->FindRow<FGsSchemaSealNode>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaSealNodeRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaSealNodeRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableSealNode final : public UGsTable
{
	GENERATED_BODY()

protected:
	TMap<int32, FName> _indexById;

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("Seal");
	}
	
	void OnCachedDataTableChanged() final
	{
		TMap<FName, const FGsSchemaSealNode*> RowsMap;
		if (!CreateAllRowsMap<const FGsSchemaSealNode>(RowsMap))
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
	bool FindRowById(const int32& InId, OUT const FGsSchemaSealNode*& OutRow) const
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
		const FGsSchemaSealNode* Temp;
		return !FindRowById(FGsSchemaSealNode().id, Temp);
	}
#endif
};
