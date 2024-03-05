/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/CoreUObject/Public/UObject/NoExportTypes.h"
#include "Runtime/DataCenter/Public/DataSchema/Item/GsSchemaItemCommon.h"
#include "Runtime/DataCenter/Public/DataSchema/Skill/Passivity/GsSchemaPassivitySet.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaSlateSpecialNode.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/MonsterKnowledgeCollection/BSGsSlateSpecialNode"))
struct DATACENTER_API FGsSchemaSlateSpecialNode
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="id"))
	int32 id;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="probability"))
	int32 probability;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="requireItemId"))
	FGsSchemaItemCommonRow requireItemId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="requireItemAmount"))
	int32 requireItemAmount;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="passivityId"))
	FGsSchemaPassivitySetRow passivityId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="nodeSprite"))
	FSoftObjectPath nodeSprite;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="nodeActiveSprite"))
	FSoftObjectPath nodeActiveSprite;
public:
	FGsSchemaSlateSpecialNode();
	
protected:
	bool operator== (const FGsSchemaSlateSpecialNode& __Other) const
	{
		return FGsSchemaSlateSpecialNode::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaSlateSpecialNode::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaSlateSpecialNode::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaSlateSpecialNodeRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaSlateSpecialNodeRow() : Super()
	{
	}
	
	const FGsSchemaSlateSpecialNode* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaSlateSpecialNode* Row = nullptr;
		return Table->FindRow<FGsSchemaSlateSpecialNode>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaSlateSpecialNodeRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaSlateSpecialNodeRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableSlateSpecialNode final : public UGsTable
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
		TMap<FName, const FGsSchemaSlateSpecialNode*> RowsMap;
		if (!CreateAllRowsMap<const FGsSchemaSlateSpecialNode>(RowsMap))
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
	bool FindRowById(const int32& InId, OUT const FGsSchemaSlateSpecialNode*& OutRow) const
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
		const FGsSchemaSlateSpecialNode* Temp;
		return !FindRowById(FGsSchemaSlateSpecialNode().id, Temp);
	}
#endif
};
