// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GsItemConsumable.h"

struct FGsSchemaRewardData;
struct FGsSchemaRewardItemBagData;


struct FGsCurrencyItemIconData
{
	CostType _costType;
	CurrencyType _currencyType;
	ItemId _itemId;
	Level _itemLevel;
	int32 _amountMin;
	int32 _amountMax;
};


class T1PROJECT_API FGsItemConsumableBox : public FGsItemConsumable
{
	using Super = FGsItemConsumable;

public:
	explicit FGsItemConsumableBox(const int32 In_ItemDetailID);
	FGsItemConsumableBox() = default;
	virtual ~FGsItemConsumableBox() = default;

public:
	virtual FText GetUIContentsText() const override;

private:
	static void SetRewardDescString(RewardId rewardId, OUT FString& OutString);
	static void SetRewardDescString(const FGsSchemaRewardData* rewardData, OUT FString& OutString);
	static void SetRewardDataString(const FGsSchemaRewardData* rewardData, OUT FString& OutString);
	static void SetRewardBoxDescString(RewardType rewardType, OUT FString& OutString);
	static void SetRewardBoxDataString(TArray<const FGsSchemaRewardItemBagData*>& itemBagDataList, OUT FString& OutString);
	static void SetRewardBoxDataToCurrencyItemIconData(TArray<const FGsSchemaRewardItemBagData*>& itemBagDataList, OUT TArray<FGsCurrencyItemIconData>& currencyItemIconDataList);

private:
	static FText GetItemNameFromItemId(int32 InItemId);

private:
	static void SortBoxItems(OUT TArray<const FGsSchemaRewardItemBagData*>& OutItemBagDataList);

public:
	static FText GetRewardDataTextFromId(RewardId InId);
	static TArray<TPair<FString, TArray<FGsCurrencyItemIconData>>> GetRewardDescItemsPairList(RewardId InRewardId);
};