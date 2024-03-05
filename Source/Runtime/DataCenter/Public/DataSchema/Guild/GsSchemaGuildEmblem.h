/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/CoreUObject/Public/UObject/NoExportTypes.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaGuildEmblem.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/Guild/BSGsGuildEmblem"))
struct DATACENTER_API FGsSchemaGuildEmblem
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="id", Tooltip))
	int32 id;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Meta=(Tooltip="기획참고용"))
	FString note;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Meta=(Tooltip="개방길드레벨"))
	int32 requireLevel;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Meta=(Tooltip="큰아이콘"))
	FSoftObjectPath iconPathBig;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Meta=(Tooltip="작은아이콘"))
	FSoftObjectPath iconPathSmall;
	
public:
	FGsSchemaGuildEmblem();
	
protected:
	bool operator== (const FGsSchemaGuildEmblem& __Other) const
	{
		return FGsSchemaGuildEmblem::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaGuildEmblem::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaGuildEmblem::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaGuildEmblemRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaGuildEmblemRow() : Super()
	{
	}
	
	const FGsSchemaGuildEmblem* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaGuildEmblem* Row = nullptr;
		return Table->FindRow<FGsSchemaGuildEmblem>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaGuildEmblemRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaGuildEmblemRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableGuildEmblem final : public UGsTable
{
	GENERATED_BODY()

protected:
	TMap<int32, FName> _indexById;

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("Guild");
	}
	
	void OnCachedDataTableChanged() final
	{
		TMap<FName, const FGsSchemaGuildEmblem*> RowsMap;
		if (!CreateAllRowsMap<const FGsSchemaGuildEmblem>(RowsMap))
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
	bool FindRowById(const int32& InId, OUT const FGsSchemaGuildEmblem*& OutRow) const
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
		const FGsSchemaGuildEmblem* Temp;
		return !FindRowById(FGsSchemaGuildEmblem().id, Temp);
	}
#endif
};
