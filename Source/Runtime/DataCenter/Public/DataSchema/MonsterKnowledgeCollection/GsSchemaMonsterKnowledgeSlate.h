/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/DataCenter/Public/DataSchema/MonsterKnowledgeCollection/SchemaOnly/GsSchemaMonsterKnowledgeSpecialNode.h"
#include "Runtime/CoreUObject/Public/UObject/NoExportTypes.h"
#include "Runtime/DataCenter/Public/MonsterKnowledgeCollection/EMonsterKnowledgeSlateCategory.h"
#include "Runtime/DataCenter/Public/DataSchema/MonsterKnowledgeCollection/GsSchemaMonsterKnowledgeSlateStatNode.h"
#include "Runtime/DataCenter/Public/Shared/Client/SharedEnums/SharedMonsterknowledgeEnum.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaMonsterKnowledgeSlate.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/MonsterKnowledgeCollection/BSGsMonsterKnowledgeSlate"))
struct DATACENTER_API FGsSchemaMonsterKnowledgeSlate
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="id"))
	int32 id;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="name"))
	FText name;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="grade"))
	MonsterKnowledgeSlateGrade grade;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="linkNodeList"))
	TArray<FGsSchemaMonsterKnowledgeSlateStatNodeRow> linkNodeList;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="beginNode"))
	FGsSchemaMonsterKnowledgeSlateStatNodeRow beginNode;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="categoryType"))
	EMonsterKnowledgeSlateCategory categoryType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="resourcePath"))
	FSoftObjectPath resourcePath;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="specialNodeList"))
	TArray<FGsSchemaMonsterKnowledgeSpecialNode> specialNodeList;
public:
	FGsSchemaMonsterKnowledgeSlate();
	
protected:
	bool operator== (const FGsSchemaMonsterKnowledgeSlate& __Other) const
	{
		return FGsSchemaMonsterKnowledgeSlate::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaMonsterKnowledgeSlate::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaMonsterKnowledgeSlate::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaMonsterKnowledgeSlateRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaMonsterKnowledgeSlateRow() : Super()
	{
	}
	
	const FGsSchemaMonsterKnowledgeSlate* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaMonsterKnowledgeSlate* Row = nullptr;
		return Table->FindRow<FGsSchemaMonsterKnowledgeSlate>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaMonsterKnowledgeSlateRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaMonsterKnowledgeSlateRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableMonsterKnowledgeSlate final : public UGsTable
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
		TMap<FName, const FGsSchemaMonsterKnowledgeSlate*> RowsMap;
		if (!CreateAllRowsMap<const FGsSchemaMonsterKnowledgeSlate>(RowsMap))
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
	bool FindRowById(const int32& InId, OUT const FGsSchemaMonsterKnowledgeSlate*& OutRow) const
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
		const FGsSchemaMonsterKnowledgeSlate* Temp;
		return !FindRowById(FGsSchemaMonsterKnowledgeSlate().id, Temp);
	}
#endif
};
