/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "DataSchema/Item/GsSchemaGuildGiftRewardDataList.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaItemEffectGuildGift.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/Item/BSGsItemEffectGuildGift"))
struct DATACENTER_API FGsSchemaItemEffectGuildGift
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="effectId"))
	int32 effectId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="mailTypeId_Buyer"))
	int32 mailTypeId_Buyer;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="mailTypeId_GuildMembers"))
	int32 mailTypeId_GuildMembers;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="rewardDataList"))
	TArray<FGsSchemaGuildGiftRewardDataList> rewardDataList;
public:
	FGsSchemaItemEffectGuildGift();
	
protected:
	bool operator== (const FGsSchemaItemEffectGuildGift& __Other) const
	{
		return FGsSchemaItemEffectGuildGift::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaItemEffectGuildGift::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaItemEffectGuildGift::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaItemEffectGuildGiftRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaItemEffectGuildGiftRow() : Super()
	{
	}
	
	const FGsSchemaItemEffectGuildGift* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaItemEffectGuildGift* Row = nullptr;
		return Table->FindRow<FGsSchemaItemEffectGuildGift>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaItemEffectGuildGiftRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaItemEffectGuildGiftRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableItemEffectGuildGift final : public UGsTable
{
	GENERATED_BODY()

protected:
	TMap<int32, FName> _indexById;

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("Item");
	}
	
	void OnCachedDataTableChanged() final
	{
		TMap<FName, const FGsSchemaItemEffectGuildGift*> RowsMap;
		if (!CreateAllRowsMap<const FGsSchemaItemEffectGuildGift>(RowsMap))
		{
			return;
		}
		_indexById.Empty(RowsMap.Num());
		for (auto Row : RowsMap)
		{
			_indexById.Emplace(Row.Value->effectId, Row.Key);
		}
	}
	
public:
	bool FindRowById(const int32& InId, OUT const FGsSchemaItemEffectGuildGift*& OutRow) const
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
		const FGsSchemaItemEffectGuildGift* Temp;
		return !FindRowById(FGsSchemaItemEffectGuildGift().effectId, Temp);
	}
#endif
};
