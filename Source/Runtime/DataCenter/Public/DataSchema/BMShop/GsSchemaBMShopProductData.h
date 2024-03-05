/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/CoreUObject/Public/UObject/NoExportTypes.h"
#include "Runtime/DataCenter/Public/Shared/Client/SharedEnums/SharedCommonEnum.h"
#include "Runtime/DataCenter/Public/Shared/Client/SharedEnums/SharedBMShopEnum.h"
#include "Runtime/DataCenter/Public/GameObject/Define/EBMShopHighlightType.h"
#include "Runtime/DataCenter/Public/GameObject/Define/EBMShopProductDetailPopupType.h"
#include "Runtime/DataCenter/Public/GameObject/Define/EBMShopProductTagType.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaBMShopProductData.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/BMShop/BSGsBMShopProductData"))
struct DATACENTER_API FGsSchemaBMShopProductData
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="id"))
	int32 id;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="itemId"))
	int32 itemId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="purchaseCostType"))
	CostType purchaseCostType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="purchaseCurrencyItemId"))
	int64 purchaseCurrencyItemId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="purchaseCurrencyType"))
	CurrencyType purchaseCurrencyType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="purchaseCurrencyPrice"))
	int64 purchaseCurrencyPrice;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="rebateCurrencyType"))
	CurrencyType rebateCurrencyType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="rebateCurrencyAmount"))
	int32 rebateCurrencyAmount;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="saleStartTime"))
	FString saleStartTime;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="saleEndTime"))
	FString saleEndTime;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="해당 입력 시간보다 적게 남았으면 남은시간 출력", DisplayName="displaySaleEndCountdownTimeValue"))
	int32 displaySaleEndCountdownTimeValue;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="rewardIdList"))
	TArray<int32> rewardIdList;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="pcLevelLimit"))
	int32 pcLevelLimit;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="quantityLimit"))
	int32 quantityLimit;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="bmShopLimitBuyerType"))
	BMPurchaseLimitBuyerType bmShopLimitBuyerType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="resetTimeTicketType"))
	ResetTimeTicket resetTimeTicketType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="googleMarketPID"))
	FString googleMarketPID;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="appStoreMarketPID"))
	FString appStoreMarketPID;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="pcPlatformPID"))
	FString pcPlatformPID;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="titleText"))
	FText titleText;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="visibilityFlag"))
	bool visibilityFlag;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="desc"))
	FText desc;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="parentProductId"))
	int32 parentProductId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="bmShopParentTabId"))
	int32 bmShopParentTabId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="bmShopSubTabId"))
	int32 bmShopSubTabId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="displayOrder"))
	int32 displayOrder;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="detailPopupType"))
	EBMShopProductDetailPopupType detailPopupType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="tagType"))
	EBMShopProductTagType tagType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="highlightType"))
	EBMShopHighlightType highlightType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="guildLevelLimit"))
	int32 guildLevelLimit;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="only use Client", DisplayName="isGuildGiftProduct"))
	bool isGuildGiftProduct;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="externalDesc"))
	FText externalDesc;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="bannerDisplayOrder"))
	int32 bannerDisplayOrder;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="bannerImgPath"))
	FString bannerImgPath;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="isBulkSale"))
	bool isBulkSale;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="stepGroupID"))
	int32 stepGroupID;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="stepLevel"))
	int32 stepLevel;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="stepBuyLimitCount"))
	int32 stepBuyLimitCount;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="popupConditionId"))
	int32 popupConditionId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="popupSaleDurationTime"))
	int32 popupSaleDurationTime;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="popuphiveBannerId"))
	int32 popuphiveBannerId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="discountCouponConditionGroupId"))
	TArray<int32> discountCouponConditionGroupId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="icon"))
	FSoftObjectPath icon;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="isOOAP"))
	bool isOOAP;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="immediatelyUse"))
	bool immediatelyUse;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="resetAfterServerMigrate"))
	bool resetAfterServerMigrate;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="구매 제한 별도 초기화 기준 (RTT 초기화와는 별도)", DisplayName="resetConditionType"))
	BMResetConditionType resetConditionType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="TRUE = 다중 구매 프로세스 진입 상품. FALSE = 기본 구매 프로세스 진입 상품.", DisplayName="isMultiplePurchase"))
	bool isMultiplePurchase;
public:
	FGsSchemaBMShopProductData();
	
protected:
	bool operator== (const FGsSchemaBMShopProductData& __Other) const
	{
		return FGsSchemaBMShopProductData::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaBMShopProductData::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaBMShopProductData::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaBMShopProductDataRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaBMShopProductDataRow() : Super()
	{
	}
	
	const FGsSchemaBMShopProductData* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaBMShopProductData* Row = nullptr;
		return Table->FindRow<FGsSchemaBMShopProductData>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaBMShopProductDataRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaBMShopProductDataRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableBMShopProductData final : public UGsTable
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
		TMap<FName, const FGsSchemaBMShopProductData*> RowsMap;
		if (!CreateAllRowsMap<const FGsSchemaBMShopProductData>(RowsMap))
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
	bool FindRowById(const int32& InId, OUT const FGsSchemaBMShopProductData*& OutRow) const
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
		const FGsSchemaBMShopProductData* Temp;
		return !FindRowById(FGsSchemaBMShopProductData().id, Temp);
	}
#endif
};
