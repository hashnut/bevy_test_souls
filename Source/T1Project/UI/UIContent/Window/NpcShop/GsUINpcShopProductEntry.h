// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UI/UIControlLib/Interface/GsEntryInterface.h"
#include "Shared/Client/SharedEnums/SharedCreatureEnum.h"
#include "Shared/Client/SharedEnums/SharedCommonEnum.h"
#include "Shared/Client/SharedEnums/SharedGuildEnum.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "UI/UIContent/Define/GsUIContentDefine.h"
#include "UI/UIContent/Define/EGsUIColorType.h"
#include "NpcShop/GsNpcShopEnum.h"


#include "GsUINpcShopProductEntry.generated.h"

class UTextBlock;
class UGsButton;
class UImage;
class UWidgetSwitcher;
class UGsDynamicIconSlotHelper;
class UGsWrapBoxIconSelector;
class UPaperSprite;
class UGsUIIconItem;
struct FGsNpcShopItemInfo;

// OnClick Entry Slot Event - slotIndex , itemTID
DECLARE_MULTICAST_DELEGATE_OneParam(FOnSelectProductItemDelegate, UGsUINpcShopProductEntry*);
/**
 * 
 */
UCLASS()
class T1PROJECT_API UGsUINpcShopProductEntry : public UUserWidget, public IGsEntryInterface
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, Category = "ItemIconSelector", meta = (BindWidget))
	UGsWrapBoxIconSelector* _iconSelector;
	UPROPERTY()
	UGsDynamicIconSlotHelper* _iconSlotHelper;

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnSelectSlot;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _itemNameText;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UImage* _currencyImage;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _itemPriceText;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidgetSwitcher* _slotSelectionSwitcher; // 0 : normal , 1 : selected
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _levelLimitText;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UImage* _dimmedBg;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _renewalTimeText;
	
private:
	bool _isSelected = false;
	uint8 _slotIndex = 0;
	NpcFunctionType _shopNpcFunctionType = NpcFunctionType::NONE;

public:
	FGsNpcShopItemInfo* _shopProduct;

private:
	void SetCurrencyIconImage(const CurrencyType InType);
	void SetCurrencyIconImageByItem(int32 InItemTID);
	void SetAmountTextColor(const EGsUIColorType InColorType);
	void OnClickIconSlot(class UGsUIIconItem& InIcon);
	void CheckState();

public:
	FOnSelectProductItemDelegate OnSelectProductItemDelegate;

public:
	virtual void BeginDestroy() override;

protected:
	// UUserWidget override
	virtual void NativeOnInitialized() override;
	virtual void NativeDestruct() override;

public:
	virtual void OnShow() override;
	virtual void OnHide() override;	

public:
	void SetItemInfo(FGsNpcShopItemInfo* InShopInfo, uint8 InSlotIndex, NpcFunctionType InShopType);
	void SetSlotSelection(const bool InIsSelected);

public:
	int32 GetProductItemTID();
	EventProductId GetProductId();
	uint8 GetSlotIndex();
	FGsNpcShopItemInfo* GetShopProductInfo();
	

public:
	UFUNCTION()
	void OnClickSelect();
	UFUNCTION()
	void OnRefreshIconWrapBox(int32 InIndex, class UGsUIIconBase* InIcon);
	UFUNCTION()
	void OnCreateIconWrapBox(class UGsUIIconBase* InIcon);
	
protected:
	void SetRenewalTimeText(bool InActive, ResetTimeTicket InResetType = ResetTimeTicket::RTT_NONE);
	void SetLimitText(EGsNotAllowedBuyReason InReason);

private:
	FTextKey GetLimitGuildMemberGradeTextKey(GuildMemberGradeType InGradeType);

public:
	void InvalidateItem();

private:
	void OnLongPressIcon(UGsUIIconItem& InIcon);
};
