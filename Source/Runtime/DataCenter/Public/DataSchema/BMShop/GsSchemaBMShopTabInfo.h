/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/DataCenter/Public/DataSchema/GsSchemaEnums.h"
#include "Runtime/CoreUObject/Public/UObject/NoExportTypes.h"
#include "Runtime/DataCenter/Public/Shared/Client/SharedEnums/SharedCommonEnum.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaBMShopTabInfo.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/BMShop/BSGsBMShopTabInfo"))
struct DATACENTER_API FGsSchemaBMShopTabInfo
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="id"))
	int32 id;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="parentTabid"))
	int32 parentTabid;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="추가적으로 보여줄 재화타입", DisplayName="extraCurrencyType"))
	CurrencyType extraCurrencyType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="extraCurrencyItemId"))
	int64 extraCurrencyItemId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="displayTabNameText"))
	FText displayTabNameText;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="displayOrder"))
	int32 displayOrder;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="visibilityFlag"))
	bool visibilityFlag;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="isStorageTab"))
	bool isStorageTab;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="동일한 타입 입력금지", DisplayName="specialTabType"))
	EGsBMShopSpecialTab specialTabType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="displayTabType"))
	EGsBMShopDisPlayType displayTabType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="노출 시작 일시", DisplayName="startExposalUtc"))
	FDateTime startExposalUtc;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="노출 종료 일시", DisplayName="endExposalUtc"))
	FDateTime endExposalUtc;
public:
	FGsSchemaBMShopTabInfo();
	
protected:
	bool operator== (const FGsSchemaBMShopTabInfo& __Other) const
	{
		return FGsSchemaBMShopTabInfo::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaBMShopTabInfo::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaBMShopTabInfo::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaBMShopTabInfoRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaBMShopTabInfoRow() : Super()
	{
	}
	
	const FGsSchemaBMShopTabInfo* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaBMShopTabInfo* Row = nullptr;
		return Table->FindRow<FGsSchemaBMShopTabInfo>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaBMShopTabInfoRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaBMShopTabInfoRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableBMShopTabInfo final : public UGsTable
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
		TMap<FName, const FGsSchemaBMShopTabInfo*> RowsMap;
		if (!CreateAllRowsMap<const FGsSchemaBMShopTabInfo>(RowsMap))
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
	bool FindRowById(const int32& InId, OUT const FGsSchemaBMShopTabInfo*& OutRow) const
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
		const FGsSchemaBMShopTabInfo* Temp;
		return !FindRowById(FGsSchemaBMShopTabInfo().id, Temp);
	}
#endif
};
