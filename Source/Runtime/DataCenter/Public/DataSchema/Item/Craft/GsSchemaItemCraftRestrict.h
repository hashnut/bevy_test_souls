/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/DataCenter/Public/Shared/Client/SharedEnums/SharedItemEnum.h"
#include "Runtime/DataCenter/Public/Shared/Client/SharedEnums/SharedCommonEnum.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaItemCraftRestrict.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/Item/Craft/BSGsItemCraftRestrict"))
struct DATACENTER_API FGsSchemaItemCraftRestrict
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="itemCraftRecipeId"))
	int32 itemCraftRecipeId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="displayBeginUtc"))
	FString displayBeginUtc;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="displayEndUtc"))
	FString displayEndUtc;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="limitCountResetTimeTicket"))
	ResetTimeTicket limitCountResetTimeTicket;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="limitCount1"))
	ItemCraftRestrict limitCount1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="limitCount1Amount"))
	int32 limitCount1Amount;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="limitCount2"))
	ItemCraftRestrict limitCount2;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="limitCount2Amount"))
	int32 limitCount2Amount;
public:
	FGsSchemaItemCraftRestrict();
	
protected:
	bool operator== (const FGsSchemaItemCraftRestrict& __Other) const
	{
		return FGsSchemaItemCraftRestrict::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaItemCraftRestrict::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaItemCraftRestrict::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaItemCraftRestrictRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaItemCraftRestrictRow() : Super()
	{
	}
	
	const FGsSchemaItemCraftRestrict* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaItemCraftRestrict* Row = nullptr;
		return Table->FindRow<FGsSchemaItemCraftRestrict>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaItemCraftRestrictRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaItemCraftRestrictRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableItemCraftRestrict final : public UGsTable
{
	GENERATED_BODY()

protected:
	TMap<int32, FName> _indexById;

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("Item/Craft");
	}
	
	void OnCachedDataTableChanged() final
	{
		TMap<FName, const FGsSchemaItemCraftRestrict*> RowsMap;
		if (!CreateAllRowsMap<const FGsSchemaItemCraftRestrict>(RowsMap))
		{
			return;
		}
		_indexById.Empty(RowsMap.Num());
		for (auto Row : RowsMap)
		{
			_indexById.Emplace(Row.Value->itemCraftRecipeId, Row.Key);
		}
	}
	
public:
	bool FindRowById(const int32& InId, OUT const FGsSchemaItemCraftRestrict*& OutRow) const
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
		const FGsSchemaItemCraftRestrict* Temp;
		return !FindRowById(FGsSchemaItemCraftRestrict().itemCraftRecipeId, Temp);
	}
#endif
};
