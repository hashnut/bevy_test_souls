/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/CoreUObject/Public/UObject/NoExportTypes.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaEmoji.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/SocialEmotion/BSGsEmoji"))
struct DATACENTER_API FGsSchemaEmoji
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="socialId"))
	int64 socialId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="name"))
	FString name;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(ShowAllItems, DisplayName="emoticon"))
	FSoftObjectPath emoticon;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="order"))
	int32 order;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="isBaseType"))
	bool isBaseType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="desc"))
	FString desc;
public:
	FGsSchemaEmoji();
	
protected:
	bool operator== (const FGsSchemaEmoji& __Other) const
	{
		return FGsSchemaEmoji::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaEmoji::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaEmoji::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaEmojiRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaEmojiRow() : Super()
	{
	}
	
	const FGsSchemaEmoji* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaEmoji* Row = nullptr;
		return Table->FindRow<FGsSchemaEmoji>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaEmojiRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaEmojiRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableEmoji final : public UGsTable
{
	GENERATED_BODY()

protected:
	TMap<int64, FName> _indexById;

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("SocialEmotion");
	}
	
	void OnCachedDataTableChanged() final
	{
		TMap<FName, const FGsSchemaEmoji*> RowsMap;
		if (!CreateAllRowsMap<const FGsSchemaEmoji>(RowsMap))
		{
			return;
		}
		_indexById.Empty(RowsMap.Num());
		for (auto Row : RowsMap)
		{
			_indexById.Emplace(Row.Value->socialId, Row.Key);
		}
	}
	
public:
	bool FindRowById(const int64& InId, OUT const FGsSchemaEmoji*& OutRow) const
	{
		return FindInIndexMap(_indexById, InId, OutRow);
	}
	
#if WITH_EDITOR
	void RemoveRowById(const int64& InId)
	{
		RemoveInIndexMap(_indexById, InId);
	}
	
	bool IsValidAllPrimaryKeys() const final
	{
		const FGsSchemaEmoji* Temp;
		return !FindRowById(FGsSchemaEmoji().socialId, Temp);
	}
#endif
};
