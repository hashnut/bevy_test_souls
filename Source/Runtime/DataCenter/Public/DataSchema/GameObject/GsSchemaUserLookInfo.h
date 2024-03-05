/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/DataCenter/Public/DataSchema/GameObject/GsSchemaPartsShapeData.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaUserLookInfo.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/GameObject/BSGsUserLookInfo"))
struct DATACENTER_API FGsSchemaUserLookInfo
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="id"))
	int32 id;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="시스템용", DisplayName="isSystem"))
	bool isSystem;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="PartsHair"))
	FGsSchemaPartsShapeDataRow PartsHair;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="PartsBody"))
	FGsSchemaPartsShapeDataRow PartsBody;
public:
	FGsSchemaUserLookInfo();
	
protected:
	bool operator== (const FGsSchemaUserLookInfo& __Other) const
	{
		return FGsSchemaUserLookInfo::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaUserLookInfo::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaUserLookInfo::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaUserLookInfoRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaUserLookInfoRow() : Super()
	{
	}
	
	const FGsSchemaUserLookInfo* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaUserLookInfo* Row = nullptr;
		return Table->FindRow<FGsSchemaUserLookInfo>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaUserLookInfoRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaUserLookInfoRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableUserLookInfo final : public UGsTable
{
	GENERATED_BODY()

protected:
	TMap<int32, FName> _indexById;

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("GameObject");
	}
	
	void OnCachedDataTableChanged() final
	{
		TMap<FName, const FGsSchemaUserLookInfo*> RowsMap;
		if (!CreateAllRowsMap<const FGsSchemaUserLookInfo>(RowsMap))
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
	bool FindRowById(const int32& InId, OUT const FGsSchemaUserLookInfo*& OutRow) const
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
		const FGsSchemaUserLookInfo* Temp;
		return !FindRowById(FGsSchemaUserLookInfo().id, Temp);
	}
#endif
};
