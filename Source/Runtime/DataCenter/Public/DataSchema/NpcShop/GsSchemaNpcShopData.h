/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/DataCenter/Public/Shared/Client/SharedEnums/SharedCommonEnum.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaNpcShopData.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/NpcShop/BSGsNpcShopData"))
struct DATACENTER_API FGsSchemaNpcShopData
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="desc"))
	FString desc;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="groupID"))
	int32 groupID;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="itemID"))
	int32 itemID;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="costType"))
	CostType costType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="currencyType"))
	CurrencyType currencyType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="currencyItemId"))
	int32 currencyItemId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="currencyAmount"))
	int32 currencyAmount;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="pcLimitLevel"))
	int32 pcLimitLevel;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="출력 우선순위", DisplayName="disPlayPriority"))
	int32 disPlayPriority;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="tempKeyID", Tooltip="만지지마셈"))
	int32 tempKeyID;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="autoPurchaseMaxCount"))
	int32 autoPurchaseMaxCount;
public:
	FGsSchemaNpcShopData();
	
protected:
	bool operator== (const FGsSchemaNpcShopData& __Other) const
	{
		return FGsSchemaNpcShopData::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaNpcShopData::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaNpcShopData::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaNpcShopDataRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaNpcShopDataRow() : Super()
	{
	}
	
	const FGsSchemaNpcShopData* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaNpcShopData* Row = nullptr;
		return Table->FindRow<FGsSchemaNpcShopData>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaNpcShopDataRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaNpcShopDataRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableNpcShopData final : public UGsTable
{
	GENERATED_BODY()

protected:
	TMap<int32, FName> _indexById;

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("NpcShop");
	}
	
	void OnCachedDataTableChanged() final
	{
		TMap<FName, const FGsSchemaNpcShopData*> RowsMap;
		if (!CreateAllRowsMap<const FGsSchemaNpcShopData>(RowsMap))
		{
			return;
		}
		_indexById.Empty(RowsMap.Num());
		for (auto Row : RowsMap)
		{
			_indexById.Emplace(Row.Value->tempKeyID, Row.Key);
		}
	}
	
public:
	bool FindRowById(const int32& InId, OUT const FGsSchemaNpcShopData*& OutRow) const
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
		const FGsSchemaNpcShopData* Temp;
		return !FindRowById(FGsSchemaNpcShopData().tempKeyID, Temp);
	}
#endif
};
