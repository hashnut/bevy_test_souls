/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "DataSchema/GsSchemaEnums.h"
#include "Runtime/CoreUObject/Public/UObject/NoExportTypes.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaEmoticonResData.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/NpcDialog/BSGsEmoticonResData"))
struct DATACENTER_API FGsSchemaEmoticonResData
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="emoticonType", Tooltip))
	EGsEmoticonType emoticonType;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="emoticonRes", Tooltip))
	FSoftObjectPath emoticonRes;
	
public:
	FGsSchemaEmoticonResData();
	
protected:
	bool operator== (const FGsSchemaEmoticonResData& __Other) const
	{
		return FGsSchemaEmoticonResData::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaEmoticonResData::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaEmoticonResData::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaEmoticonResDataRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaEmoticonResDataRow() : Super()
	{
	}
	
	const FGsSchemaEmoticonResData* GetRow() const
	{
		const FGsSchemaEmoticonResData* Row = nullptr;
		return GetTable()->FindRow<FGsSchemaEmoticonResData>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaEmoticonResDataRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaEmoticonResDataRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableEmoticonResData final : public UGsTable
{
	GENERATED_BODY()

protected:
	TMap<EGsEmoticonType, FName> _indexById;

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("NpcDialog");
	}
	
	void OnCachedDataTableChanged() final
	{
		TMap<FName, const FGsSchemaEmoticonResData*> RowsMap;
		if (!CreateAllRowsMap<const FGsSchemaEmoticonResData>(RowsMap))
		{
			return;
		}
		_indexById.Empty(RowsMap.Num());
		for (auto Row : RowsMap)
		{
			_indexById.Emplace(Row.Value->emoticonType, Row.Key);
		}
	}
	
public:
	bool FindRowById(const EGsEmoticonType& InId, OUT const FGsSchemaEmoticonResData*& OutRow) const
	{
		return FindInIndexMap(_indexById, InId, OutRow);
	}
	
#if WITH_EDITOR
	void RemoveRowById(const EGsEmoticonType& InId)
	{
		RemoveInIndexMap(_indexById, InId);
	}
	
	bool IsValidAllPrimaryKeys() const final
	{
		const FGsSchemaEmoticonResData* Temp;
		return !FindRowById(FGsSchemaEmoticonResData().emoticonType, Temp);
	}
#endif
};
