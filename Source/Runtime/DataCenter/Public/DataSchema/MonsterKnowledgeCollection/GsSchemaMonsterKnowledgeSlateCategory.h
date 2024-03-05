/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/DataCenter/Public/MonsterKnowledgeCollection/EMonsterKnowledgeSlateCategory.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaMonsterKnowledgeSlateCategory.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/MonsterKnowledgeCollection/BSGsMonsterKnowledgeSlateCategory"))
struct DATACENTER_API FGsSchemaMonsterKnowledgeSlateCategory
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="id"))
	int32 id;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="name"))
	FText name;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="type"))
	EMonsterKnowledgeSlateCategory type;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="enable"))
	bool enable;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="orderNum"))
	int32 orderNum;
public:
	FGsSchemaMonsterKnowledgeSlateCategory();
	
protected:
	bool operator== (const FGsSchemaMonsterKnowledgeSlateCategory& __Other) const
	{
		return FGsSchemaMonsterKnowledgeSlateCategory::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaMonsterKnowledgeSlateCategory::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaMonsterKnowledgeSlateCategory::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaMonsterKnowledgeSlateCategoryRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaMonsterKnowledgeSlateCategoryRow() : Super()
	{
	}
	
	const FGsSchemaMonsterKnowledgeSlateCategory* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaMonsterKnowledgeSlateCategory* Row = nullptr;
		return Table->FindRow<FGsSchemaMonsterKnowledgeSlateCategory>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaMonsterKnowledgeSlateCategoryRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaMonsterKnowledgeSlateCategoryRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableMonsterKnowledgeSlateCategory final : public UGsTable
{
	GENERATED_BODY()

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("MonsterKnowledgeCollection");
	}
	
};
