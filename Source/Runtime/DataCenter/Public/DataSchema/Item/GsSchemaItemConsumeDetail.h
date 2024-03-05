/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "Shared/Client/SharedEnums/SharedItemEnum.h"
#include "GsSchemaItemConsumeDetail.generated.h"

USTRUCT(BlueprintType, Meta=(Tooltip="ItemConsumeDetail Table", AssetSchemaPackagePath="/Game/DataCenter/DataSchema/Item/BSGsItemConsumeDetail"))
struct DATACENTER_API FGsSchemaItemConsumeDetail
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="id"))
	int32 id;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="consumeDecrement"))
	int32 consumeDecrement;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="cooldownTime"))
	float cooldownTime;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="cooldownGroupId"))
	int32 cooldownGroupId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="effectType"))
	ItemEffectType effectType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="effectIDList"))
	TArray<int32> effectIDList;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="tooltipDurationTime"))
	int32 tooltipDurationTime;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="isAllowedAutoUse", Tooltip="자동사용 가능유무"))
	bool isAllowedAutoUse;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="isAllowedAutoSell"))
	bool isAllowedAutoSell;
public:
	FGsSchemaItemConsumeDetail();
	
protected:
	bool operator== (const FGsSchemaItemConsumeDetail& __Other) const
	{
		return FGsSchemaItemConsumeDetail::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaItemConsumeDetail::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaItemConsumeDetail::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaItemConsumeDetailRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaItemConsumeDetailRow() : Super()
	{
	}
	
	const FGsSchemaItemConsumeDetail* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaItemConsumeDetail* Row = nullptr;
		return Table->FindRow<FGsSchemaItemConsumeDetail>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaItemConsumeDetailRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaItemConsumeDetailRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableItemConsumeDetail final : public UGsTable
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
		TMap<FName, const FGsSchemaItemConsumeDetail*> RowsMap;
		if (!CreateAllRowsMap<const FGsSchemaItemConsumeDetail>(RowsMap))
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
	bool FindRowById(const int32& InId, OUT const FGsSchemaItemConsumeDetail*& OutRow) const
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
		const FGsSchemaItemConsumeDetail* Temp;
		return !FindRowById(FGsSchemaItemConsumeDetail().id, Temp);
	}
#endif
};
