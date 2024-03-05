/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/DataCenter/Public/Shared/Client/SharedEnums/SharedBMShopEnum.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaBMShopPopupConditionInfo.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/BMShop/BSGsBMShopPopupConditionInfo"))
struct DATACENTER_API FGsSchemaBMShopPopupConditionInfo
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="id"))
	int32 id;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="conditionType"))
	BMPopupConditionType conditionType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="conditionCount"))
	int64 conditionCount;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="conditionParam1"))
	FString conditionParam1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="conditionParam2"))
	FString conditionParam2;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="conditionParam3"))
	FString conditionParam3;
public:
	FGsSchemaBMShopPopupConditionInfo();
	
protected:
	bool operator== (const FGsSchemaBMShopPopupConditionInfo& __Other) const
	{
		return FGsSchemaBMShopPopupConditionInfo::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaBMShopPopupConditionInfo::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaBMShopPopupConditionInfo::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaBMShopPopupConditionInfoRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaBMShopPopupConditionInfoRow() : Super()
	{
	}
	
	const FGsSchemaBMShopPopupConditionInfo* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaBMShopPopupConditionInfo* Row = nullptr;
		return Table->FindRow<FGsSchemaBMShopPopupConditionInfo>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaBMShopPopupConditionInfoRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaBMShopPopupConditionInfoRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableBMShopPopupConditionInfo final : public UGsTable
{
	GENERATED_BODY()

protected:
	TMap<int32, FName> _indexById;

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("BMShop");
	}
	
	void OnCachedDataTableChanged() final
	{
		TMap<FName, const FGsSchemaBMShopPopupConditionInfo*> RowsMap;
		if (!CreateAllRowsMap<const FGsSchemaBMShopPopupConditionInfo>(RowsMap))
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
	bool FindRowById(const int32& InId, OUT const FGsSchemaBMShopPopupConditionInfo*& OutRow) const
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
		const FGsSchemaBMShopPopupConditionInfo* Temp;
		return !FindRowById(FGsSchemaBMShopPopupConditionInfo().id, Temp);
	}
#endif
};
