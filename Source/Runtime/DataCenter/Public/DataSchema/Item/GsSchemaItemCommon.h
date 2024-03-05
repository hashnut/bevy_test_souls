/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/CoreUObject/Public/UObject/NoExportTypes.h"
#include "Runtime/DataCenter/Public/Shared/Client/SharedEnums/SharedCreatureEnum.h"
#include "Runtime/DataCenter/Public/DataSchema/GsSchemaEnums.h"
#include "Runtime/DataCenter/Public/DataSchema/DemandInfo/GsSchemaDemandInfoConstraint.h"
#include "Runtime/DataCenter/Public/DataSchema/SupplyInfo/GsSchemaSupplyInfoConstraint.h"
#include "Runtime/DataCenter/Public/Shared/Client/SharedEnums/SharedItemEnum.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaItemCommon.generated.h"

USTRUCT(BlueprintType, Meta=(Tooltip="Item_Common Table", AssetSchemaPackagePath="/Game/DataCenter/DataSchema/Item/BSGsItemCommon"))
struct DATACENTER_API FGsSchemaItemCommon
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="id"))
	int32 id;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="name"))
	FText name;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="type"))
	ItemType type;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="detailId"))
	int32 detailId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="categoryMain"))
	ItemCategoryMain categoryMain;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="categorySub"))
	ItemCategorySub categorySub;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="grade"))
	ItemGrade grade;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="minLevel"))
	int32 minLevel;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="maxLevel"))
	int32 maxLevel;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="사용 가능 최소 레벨", ClampMin="0", ClampMax="999", DisplayName="useLevel"))
	int32 useLevel;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="genderType"))
	CreatureGenderType genderType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="maxStackCount"))
	int64 maxStackCount;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="sellPrice"))
	int32 sellPrice;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="storeSellable"))
	bool storeSellable;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="tradable"))
	bool tradable;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="personalTradable"))
	bool personalTradable;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="warehouseStorable"))
	bool warehouseStorable;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="decomposable"))
	bool decomposable;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="icon"))
	FSoftObjectPath icon;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="tooltip"))
	FText tooltip;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="weight"))
	int32 weight;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="acquireType"))
	ItemAcquireType acquireType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="대체 아이템TID 리스트", DisplayName="substituteItemIds"))
	TArray<int32> substituteItemIds;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="expiredTimeId"))
	int32 expiredTimeId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="supplyInfoConstraint"))
	FGsSchemaSupplyInfoConstraintRow supplyInfoConstraint;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="demandInfoConstraint"))
	FGsSchemaDemandInfoConstraintRow demandInfoConstraint;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="귀속 아이템 아이디(삭제예정)", DisplayName="belongingItemId"))
	int32 belongingItemId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="아이템그룹ID. 복구 아이템 정보 등", DisplayName="itemGroupId"))
	int32 itemGroupId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="아이템 상세창 버리기 버튼만 조정함", DisplayName="canDelete"))
	bool canDelete;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="iconSlotEffectType"))
	EGsItemIconSlotEffectType iconSlotEffectType;
public:
	FGsSchemaItemCommon();
	
protected:
	bool operator== (const FGsSchemaItemCommon& __Other) const
	{
		return FGsSchemaItemCommon::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaItemCommon::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaItemCommon::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaItemCommonRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaItemCommonRow() : Super()
	{
	}
	
	const FGsSchemaItemCommon* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaItemCommon* Row = nullptr;
		return Table->FindRow<FGsSchemaItemCommon>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaItemCommonRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaItemCommonRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableItemCommon final : public UGsTable
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
		TMap<FName, const FGsSchemaItemCommon*> RowsMap;
		if (!CreateAllRowsMap<const FGsSchemaItemCommon>(RowsMap))
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
	bool FindRowById(const int32& InId, OUT const FGsSchemaItemCommon*& OutRow) const
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
		const FGsSchemaItemCommon* Temp;
		return !FindRowById(FGsSchemaItemCommon().id, Temp);
	}
#endif
};
