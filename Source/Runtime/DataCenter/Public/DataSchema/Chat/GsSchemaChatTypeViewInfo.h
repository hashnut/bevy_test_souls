/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/DataCenter/Public/DataSchema/GsSchemaEnums.h"
#include "Runtime/DataCenter/Public/DataSchema/Chat/GsSchemaChatTabViewInfo.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaChatTypeViewInfo.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/Chat/BSGsChatTypeViewInfo"))
struct DATACENTER_API FGsSchemaChatTypeViewInfo
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="chaType"))
	EGsExChatType chaType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="useChatType"))
	EGsExChatType useChatType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="viewType"))
	EGsChatViewType viewType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="viewTabs"))
	TArray<FGsSchemaChatTabViewInfoRow> viewTabs;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="accountGradeTag"))
	TMap<uint8,FString> accountGradeTag;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="useOtherWorldPrefix"))
	bool useOtherWorldPrefix;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="prefixFmt"))
	FString prefixFmt;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="nameFmt"))
	FString nameFmt;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="textFmt"))
	FString textFmt;
public:
	FGsSchemaChatTypeViewInfo();
	
protected:
	bool operator== (const FGsSchemaChatTypeViewInfo& __Other) const
	{
		return FGsSchemaChatTypeViewInfo::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaChatTypeViewInfo::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaChatTypeViewInfo::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaChatTypeViewInfoRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaChatTypeViewInfoRow() : Super()
	{
	}
	
	const FGsSchemaChatTypeViewInfo* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaChatTypeViewInfo* Row = nullptr;
		return Table->FindRow<FGsSchemaChatTypeViewInfo>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaChatTypeViewInfoRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaChatTypeViewInfoRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableChatTypeViewInfo final : public UGsTable
{
	GENERATED_BODY()

protected:
	TMap<EGsExChatType, FName> _indexById;

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("Chat");
	}
	
	void OnCachedDataTableChanged() final
	{
		TMap<FName, const FGsSchemaChatTypeViewInfo*> RowsMap;
		if (!CreateAllRowsMap<const FGsSchemaChatTypeViewInfo>(RowsMap))
		{
			return;
		}
		_indexById.Empty(RowsMap.Num());
		for (auto Row : RowsMap)
		{
			_indexById.Emplace(Row.Value->chaType, Row.Key);
		}
	}
	
public:
	bool FindRowById(const EGsExChatType& InId, OUT const FGsSchemaChatTypeViewInfo*& OutRow) const
	{
		return FindInIndexMap(_indexById, InId, OutRow);
	}
	
#if WITH_EDITOR
	void RemoveRowById(const EGsExChatType& InId)
	{
		RemoveInIndexMap(_indexById, InId);
	}
	
	bool IsValidAllPrimaryKeys() const final
	{
		const FGsSchemaChatTypeViewInfo* Temp;
		return !FindRowById(FGsSchemaChatTypeViewInfo().chaType, Temp);
	}
#endif
};
