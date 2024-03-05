/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/CoreUObject/Public/UObject/NoExportTypes.h"
#include "Runtime/DataCenter/Public/DataSchema/Mail/GsSchemaMailType.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaMonsterKnowledgeSlateStatNode.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/MonsterKnowledgeCollection/BSGsMonsterKnowledgeSlateStatNode"))
struct DATACENTER_API FGsSchemaMonsterKnowledgeSlateStatNode
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="id"))
	int32 id;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="name"))
	FString name;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="passivitySetId"))
	int32 passivitySetId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="openCost"))
	int32 openCost;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="imagePath"))
	FSoftObjectPath imagePath;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="resourceId"))
	int32 resourceId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="prevNodeId"))
	int32 prevNodeId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="mailTypeId"))
	FGsSchemaMailTypeRow mailTypeId;
public:
	FGsSchemaMonsterKnowledgeSlateStatNode();
	
protected:
	bool operator== (const FGsSchemaMonsterKnowledgeSlateStatNode& __Other) const
	{
		return FGsSchemaMonsterKnowledgeSlateStatNode::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaMonsterKnowledgeSlateStatNode::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaMonsterKnowledgeSlateStatNode::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaMonsterKnowledgeSlateStatNodeRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaMonsterKnowledgeSlateStatNodeRow() : Super()
	{
	}
	
	const FGsSchemaMonsterKnowledgeSlateStatNode* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaMonsterKnowledgeSlateStatNode* Row = nullptr;
		return Table->FindRow<FGsSchemaMonsterKnowledgeSlateStatNode>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaMonsterKnowledgeSlateStatNodeRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaMonsterKnowledgeSlateStatNodeRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableMonsterKnowledgeSlateStatNode final : public UGsTable
{
	GENERATED_BODY()

protected:
	TMap<int32, FName> _indexById;

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("MonsterKnowledgeCollection");
	}
	
	void OnCachedDataTableChanged() final
	{
		TMap<FName, const FGsSchemaMonsterKnowledgeSlateStatNode*> RowsMap;
		if (!CreateAllRowsMap<const FGsSchemaMonsterKnowledgeSlateStatNode>(RowsMap))
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
	bool FindRowById(const int32& InId, OUT const FGsSchemaMonsterKnowledgeSlateStatNode*& OutRow) const
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
		const FGsSchemaMonsterKnowledgeSlateStatNode* Temp;
		return !FindRowById(FGsSchemaMonsterKnowledgeSlateStatNode().id, Temp);
	}
#endif
};
