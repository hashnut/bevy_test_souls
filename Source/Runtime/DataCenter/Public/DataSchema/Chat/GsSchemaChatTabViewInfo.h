/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/DataCenter/Public/DataSchema/GsSchemaEnums.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaChatTabViewInfo.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/Chat/BSGsChatTabViewInfo"))
struct DATACENTER_API FGsSchemaChatTabViewInfo
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="tab"))
	EGsChatViewType tab;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="useRoomId"))
	bool useRoomId;
public:
	FGsSchemaChatTabViewInfo();
	
protected:
	bool operator== (const FGsSchemaChatTabViewInfo& __Other) const
	{
		return FGsSchemaChatTabViewInfo::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaChatTabViewInfo::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaChatTabViewInfo::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaChatTabViewInfoRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaChatTabViewInfoRow() : Super()
	{
	}
	
	const FGsSchemaChatTabViewInfo* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaChatTabViewInfo* Row = nullptr;
		return Table->FindRow<FGsSchemaChatTabViewInfo>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaChatTabViewInfoRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaChatTabViewInfoRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableChatTabViewInfo final : public UGsTable
{
	GENERATED_BODY()

protected:
	TMap<EGsChatViewType, FName> _indexById;

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("Chat");
	}
	
	void OnCachedDataTableChanged() final
	{
		TMap<FName, const FGsSchemaChatTabViewInfo*> RowsMap;
		if (!CreateAllRowsMap<const FGsSchemaChatTabViewInfo>(RowsMap))
		{
			return;
		}
		_indexById.Empty(RowsMap.Num());
		for (auto Row : RowsMap)
		{
			_indexById.Emplace(Row.Value->tab, Row.Key);
		}
	}
	
public:
	bool FindRowById(const EGsChatViewType& InId, OUT const FGsSchemaChatTabViewInfo*& OutRow) const
	{
		return FindInIndexMap(_indexById, InId, OutRow);
	}
	
#if WITH_EDITOR
	void RemoveRowById(const EGsChatViewType& InId)
	{
		RemoveInIndexMap(_indexById, InId);
	}
	
	bool IsValidAllPrimaryKeys() const final
	{
		const FGsSchemaChatTabViewInfo* Temp;
		return !FindRowById(FGsSchemaChatTabViewInfo().tab, Temp);
	}
#endif
};
