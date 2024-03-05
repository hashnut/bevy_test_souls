/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/DataCenter/Public/DataSchema/GsSchemaEnums.h"
#include "Runtime/DataCenter/Public/DataSchema/Loading/GsSchemaLoadingScreen.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaLoadingScreenGroup.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/Loading/BSGsLoadingScreenGroup"))
struct DATACENTER_API FGsSchemaLoadingScreenGroup
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="id"))
	int32 id;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="설명"))
	FString desc;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="groupType"))
	EGsLoadingScreenGroupType groupType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="loadingScreenList"))
	TArray<FGsSchemaLoadingScreenRow> loadingScreenList;
public:
	FGsSchemaLoadingScreenGroup();
	
protected:
	bool operator== (const FGsSchemaLoadingScreenGroup& __Other) const
	{
		return FGsSchemaLoadingScreenGroup::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaLoadingScreenGroup::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaLoadingScreenGroup::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaLoadingScreenGroupRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaLoadingScreenGroupRow() : Super()
	{
	}
	
	const FGsSchemaLoadingScreenGroup* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaLoadingScreenGroup* Row = nullptr;
		return Table->FindRow<FGsSchemaLoadingScreenGroup>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaLoadingScreenGroupRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaLoadingScreenGroupRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableLoadingScreenGroup final : public UGsTable
{
	GENERATED_BODY()

protected:
	TMap<int32, FName> _indexById;

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("Loading");
	}
	
	void OnCachedDataTableChanged() final
	{
		TMap<FName, const FGsSchemaLoadingScreenGroup*> RowsMap;
		if (!CreateAllRowsMap<const FGsSchemaLoadingScreenGroup>(RowsMap))
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
	bool FindRowById(const int32& InId, OUT const FGsSchemaLoadingScreenGroup*& OutRow) const
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
		const FGsSchemaLoadingScreenGroup* Temp;
		return !FindRowById(FGsSchemaLoadingScreenGroup().id, Temp);
	}
#endif
};
