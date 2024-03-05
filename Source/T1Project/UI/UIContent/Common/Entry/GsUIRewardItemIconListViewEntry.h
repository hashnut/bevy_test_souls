#pragma once
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Blueprint/IUserObjectListEntry.h"
#include "GsUIItemIconListViewEntry.h"
#include "DataSchema/Reward/GsSchemaRewardItemBagData.h"
#include "UI/UIContent/Common/Icon/GsUIIconItem.h"
#include "UObject/Object.h"
#include "Shared/Client/SharedEnums/SharedCommonEnum.h"
#include "GsUIRewardItemIconListViewEntry.generated.h"

class UGsUIIconBase;
struct FGsSchemaRewardItemBagData;
class UGsDynamicIconSlotHelper;
class UGsWrapBoxIconSelector;

USTRUCT()
struct T1PROJECT_API FGsUIRewardIconData
{
	GENERATED_BODY()

public:
	const FGsSchemaRewardItemBagData* _rewardItem = nullptr;	
	CurrencyType _currencyType = CurrencyType::NONE;
	int64 _minAmount;
	int64 _maxAmount;

public:
	void Init(const FGsSchemaRewardItemBagData* inReward, ItemAmount inItemAmount = 0)
	{
		_rewardItem = inReward;
		_currencyType = (inReward) ? inReward->currencyType : CurrencyType::NONE;
		_minAmount = inItemAmount;
		_maxAmount = inItemAmount;
	}

	void Init(const FGsSchemaRewardItemBagData* inReward, ItemAmount inMin, ItemAmount inMax)
	{
		_rewardItem = inReward;		
		_currencyType = (inReward) ? inReward->currencyType : CurrencyType::NONE;
		_minAmount = inMin;
		_maxAmount = inMax;
	}

	void Init(const CurrencyType inCurrencyType, ItemAmount inAmount)
	{
		_currencyType = inCurrencyType;
		_minAmount = inAmount;
		_maxAmount = inAmount;
	}

	void Init(const CurrencyType inCurrencyType, ItemAmount inMin, ItemAmount inMax)
	{
		_currencyType = inCurrencyType;
		_minAmount = inMin;
		_maxAmount = inMax;
	}
};

UCLASS()
class T1PROJECT_API UGsUIRewardIconListViewEntryData : public UObject
{
	GENERATED_BODY()

public:
	TArray<FGsUIRewardIconData> _rewardItemDataArray;
};

UCLASS()
class T1PROJECT_API UGsUIRewardItemIconListViewEntry : public UUserWidget, public IUserObjectListEntry
{
	GENERATED_BODY()

public:
	UPROPERTY()
		class UGsDynamicIconSlotHelper* _iconSlotHelper;
	UPROPERTY(BlueprintReadOnly, Category = "ItemIconSelector", meta = (BindWidget))
		class UGsWrapBoxIconSelector* _iconSelector;

private:
	TArray<FGsUIRewardIconData> _rewardItemDataArray;		


public:
	virtual void NativeOnInitialized() override;
	virtual void NativeOnListItemObjectSet(UObject* inListItemObject) override;
	virtual void NativeDestruct() override;

public:
	UFUNCTION()
		void OnCreateIcon(class UGsUIIconBase* InIcon);
	UFUNCTION()
		void OnRefreshIcon(int32 InIndex, class UGsUIIconBase* InIcon);
	void OnLongPressIcon(UGsUIIconItem& InIcon);
};

