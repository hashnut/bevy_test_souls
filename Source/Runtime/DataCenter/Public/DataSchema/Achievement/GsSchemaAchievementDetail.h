/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/CoreUObject/Public/UObject/NoExportTypes.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaAchievementDetail.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/Achievement/BSGsAchievementDetail"))
struct DATACENTER_API FGsSchemaAchievementDetail
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="id"))
	int32 id;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="tabIndex"))
	int32 tabIndex;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="textTitle"))
	FText textTitle;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="textDesc"))
	FText textDesc;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="icon"))
	FSoftObjectPath icon;
public:
	FGsSchemaAchievementDetail();
	
protected:
	bool operator== (const FGsSchemaAchievementDetail& __Other) const
	{
		return FGsSchemaAchievementDetail::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaAchievementDetail::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaAchievementDetail::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaAchievementDetailRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaAchievementDetailRow() : Super()
	{
	}
	
	const FGsSchemaAchievementDetail* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaAchievementDetail* Row = nullptr;
		return Table->FindRow<FGsSchemaAchievementDetail>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaAchievementDetailRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaAchievementDetailRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableAchievementDetail final : public UGsTable
{
	GENERATED_BODY()

protected:
	TMap<int32, FName> _indexById;

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("Achievement");
	}
	
	void OnCachedDataTableChanged() final
	{
		TMap<FName, const FGsSchemaAchievementDetail*> RowsMap;
		if (!CreateAllRowsMap<const FGsSchemaAchievementDetail>(RowsMap))
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
	bool FindRowById(const int32& InId, OUT const FGsSchemaAchievementDetail*& OutRow) const
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
		const FGsSchemaAchievementDetail* Temp;
		return !FindRowById(FGsSchemaAchievementDetail().id, Temp);
	}
#endif
};
