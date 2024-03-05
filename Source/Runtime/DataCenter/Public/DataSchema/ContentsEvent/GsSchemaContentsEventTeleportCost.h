/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/DataCenter/Public/Shared/Client/SharedEnums/SharedEventEnum.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaContentsEventTeleportCost.generated.h"

USTRUCT(BlueprintType, Meta=(Tooltip="컨텐츠이벤트 - 텔레포트 비용 할인. 퀘스트 텔레포트 포함(메인,서브,반복,기사단). 골드 이외의 비용을 받는 경우는 적용 대상이 아니다", AssetSchemaPackagePath="/Game/DataCenter/DataSchema/ContentsEvent/BSGSContentsEventTeleportCost"))
struct DATACENTER_API FGsSchemaContentsEventTeleportCost
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="id"))
	int32 id;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="goldDiscountValue 보다 낮은 비용은 적용 대상이 아니다", DisplayName="goldDiscountType"))
	ContentsEventTeleportCost goldDiscountType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="value / rate 형 둘다 사용 (rate형일땐 10,000분율로적용)", DisplayName="goldDiscountValue"))
	int32 goldDiscountValue;
public:
	FGsSchemaContentsEventTeleportCost();
	
protected:
	bool operator== (const FGsSchemaContentsEventTeleportCost& __Other) const
	{
		return FGsSchemaContentsEventTeleportCost::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaContentsEventTeleportCost::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaContentsEventTeleportCost::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaContentsEventTeleportCostRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaContentsEventTeleportCostRow() : Super()
	{
	}
	
	const FGsSchemaContentsEventTeleportCost* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaContentsEventTeleportCost* Row = nullptr;
		return Table->FindRow<FGsSchemaContentsEventTeleportCost>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaContentsEventTeleportCostRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaContentsEventTeleportCostRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableContentsEventTeleportCost final : public UGsTable
{
	GENERATED_BODY()

protected:
	TMap<int32, FName> _indexById;

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("ContentsEvent");
	}
	
	void OnCachedDataTableChanged() final
	{
		TMap<FName, const FGsSchemaContentsEventTeleportCost*> RowsMap;
		if (!CreateAllRowsMap<const FGsSchemaContentsEventTeleportCost>(RowsMap))
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
	bool FindRowById(const int32& InId, OUT const FGsSchemaContentsEventTeleportCost*& OutRow) const
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
		const FGsSchemaContentsEventTeleportCost* Temp;
		return !FindRowById(FGsSchemaContentsEventTeleportCost().id, Temp);
	}
#endif
};
