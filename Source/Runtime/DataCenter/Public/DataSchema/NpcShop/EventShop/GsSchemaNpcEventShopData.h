/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/DataCenter/Public/Shared/Client/SharedEnums/SharedCommonEnum.h"
#include "Runtime/DataCenter/Public/Shared/Client/SharedEnums/SharedGuildEnum.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaNpcEventShopData.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/NpcShop/EventShop/BSGsNpcEventShopData"))
struct DATACENTER_API FGsSchemaNpcEventShopData
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="groupId"))
	int32 groupId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="itemId"))
	int32 itemId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="productId"))
	int32 productId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="displayPriority", Tooltip="낮을수록 먼저표시"))
	int32 displayPriority;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="costType"))
	CostType costType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="currencyType"))
	CurrencyType currencyType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="currencyItemId"))
	int32 currencyItemId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="currencyAmount"))
	int32 currencyAmount;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="purchaseLimitLevel"))
	int32 purchaseLimitLevel;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="purchaseLimitGuildLevel"))
	int32 purchaseLimitGuildLevel;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="purchaseLimitGuildMemberGradeType"))
	GuildMemberGradeType purchaseLimitGuildMemberGradeType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="purchaseLimitType"))
	PurchaseLimitBuyerType purchaseLimitType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="purchaseLimitAmount"))
	int32 purchaseLimitAmount;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="purchaseLimitResetType"))
	ResetTimeTicket purchaseLimitResetType;
public:
	FGsSchemaNpcEventShopData();
	
protected:
	bool operator== (const FGsSchemaNpcEventShopData& __Other) const
	{
		return FGsSchemaNpcEventShopData::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaNpcEventShopData::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaNpcEventShopData::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaNpcEventShopDataRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaNpcEventShopDataRow() : Super()
	{
	}
	
	const FGsSchemaNpcEventShopData* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaNpcEventShopData* Row = nullptr;
		return Table->FindRow<FGsSchemaNpcEventShopData>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaNpcEventShopDataRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaNpcEventShopDataRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableNpcEventShopData final : public UGsTable
{
	GENERATED_BODY()

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("NpcShop/EventShop");
	}
	
};
