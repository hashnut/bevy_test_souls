#pragma once
#include "CoreMinimal.h"
#include "Blueprint/IUserObjectListEntry.h"
#include "UI/UIContent/Common/Icon/GsUIIconItem.h"
#include "GsUIItemIconListViewEntry.generated.h"

class UGsDynamicIconSlotHelper;
struct FGsSchemaRewardItemBagData;
class UGsUIIconBase;

USTRUCT()
struct T1PROJECT_API FGsItemIconData
{
	GENERATED_BODY()

public:
	CurrencyType _currencyType = CurrencyType::NONE;
	int32 _itemId;
	ItemAmount _itemAmount = 0;

public:
	FGsItemIconData() = default;
	FGsItemIconData(int32 inItemId, int32 inCount)
		: _itemId(inItemId), _itemAmount(inCount)
	{
		
	}
	void Init(const int32 inItemId, ItemAmount inItemAmount = 0)
	{
		_itemId = inItemId;
		_itemAmount = inItemAmount;
		_currencyType = CurrencyType::NONE;
	}

	void Init(CurrencyType inCurrencyType, ItemAmount inItemAmount = 0)
	{
		_currencyType = inCurrencyType;		
		_itemAmount = inItemAmount;
		_itemId = 0;
	}
};

UCLASS()
class T1PROJECT_API UGsUIItemIconListViewEntryData : public UObject
{
	GENERATED_BODY()

public:
	TArray<FGsItemIconData> _itemDataArray;
};

UCLASS()
class T1PROJECT_API UGsUIItemIconListViewEntry : public UUserWidget, public IUserObjectListEntry
{
	GENERATED_BODY()

public:
	UPROPERTY()
		class UGsDynamicIconSlotHelper* _iconSlotHelper;
	UPROPERTY(BlueprintReadOnly, Category = "ItemIconSelector", meta = (BindWidget))
		class UGsWrapBoxIconSelector* _iconSelector;

public:
	TArray<FGsItemIconData> _itemDataArray;

public:
	virtual void NativeOnInitialized() override;
	virtual void NativeDestruct() override;
	virtual void NativeOnListItemObjectSet(UObject* inListItemObject) override;

public:
	UFUNCTION()
		virtual void OnCreateIcon(UGsUIIconBase* InIcon);
	UFUNCTION()
		virtual void OnRefreshIcon(int32 InIndex, UGsUIIconBase* InIcon);
		void OnLongPressIcon(UGsUIIconItem& InIcon);
};