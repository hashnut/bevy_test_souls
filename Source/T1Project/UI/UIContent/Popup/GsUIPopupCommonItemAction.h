// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/UILib/Base/GsUIPopup.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "Shared/Client/SharedEnums/SharedCommonEnum.h"
#include "Styling/SlateColor.h"
#include "GsUIPopupCommonItemAction.generated.h"

class UGsWrapBoxIconSelector;
class UGsDynamicIconSlotHelper;
class UTextBlock;
class URichTextBlock;
class UImage;
class UGsUIIconItemInventory;
class UPaperSprite;
class UGsUIIconBase;
class UButton;
class UCanvasPanel;
class FGsItem;
class UWidgetSwitcher;

/**
 * 아이템 아이콘 / 아이템 이름(대표 설명) / 부가 설명 / 재화로
 * 구성되는 YesNo 팝업
 */
UCLASS()
class T1PROJECT_API UGsUIPopupCommonItemAction : public UGsUIPopup
{
	GENERATED_BODY()

public:
	enum class EButtonStyle
	{
		Decidable,		// 2021/10/29 PKT - Yes & No
		Notice,			// 2021/10/29 PKT - Yes
	};
	
protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsWrapBoxIconSelector* _iconSelector;

	UPROPERTY()
	UGsDynamicIconSlotHelper* _slotHelper;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _itemNameText;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	URichTextBlock* _contentDescText;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UCanvasPanel* _currencySlot;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UImage* _currencyImage;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _currencyAmountText;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _yesButtonText;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _noButtonText;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UButton* _btnYes;
	
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UButton* _btnNo;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UButton* _btnNotice;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidgetSwitcher* _switcherButtonStyle;

	// T: Yes, F: No
	TFunction<void(bool)> _buttonCallback;

protected:
	TArray<FGsItem*> _itemList;
	TArray<ItemDBIdAmountPair> _itemDbidAmountPairList;
	FSlateColor _itemTextColor;
	bool _useLongPress = false;

public:
	virtual void BeginDestroy() override;

protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

public:
	void SetCurrencyData(CurrencyType InCurrencyType, Currency InCurrency);
	void SetCurrencyItemData(ItemId InCurrencyItemId, ItemAmount InAmount);
	bool IsEnoughCurrencyAmount(ItemAmount InTargetAmount, ItemAmount InCurrencyAmount);
	void SetDescription(const FText& InContent);
	void SetItem(FGsItem* InItem, ItemAmount InAmount = 1, bool bUseItemColor = false, bool bUseLongPress = false);
	// 2023/10/5 PKT - Enchant Level 포함( +n 아이템 명 )
	void SetItemEx(FGsItem* InItem, ItemAmount InAmount = 1, bool bUseItemColor = false, bool bUseLongPress = false);
	void SetItemList(const TArray<FGsItem*>& InItemList, bool bUseLongPress = false);
	void SetItemList(const TArray<ItemDBIdAmountPair>& InDbidAmountPairList, bool bUseLongPress = false);
	void SetCallBack(TFunction<void(bool)> InCallback);

	void SetButtonStyle(EButtonStyle InStyle);

protected:
	UGsUIIconItemInventory* GetIcon();
	UPaperSprite* GetIconImage(CurrencyType InType);
	UPaperSprite* GetIconImage(int32 InItemId);

public:
	void SetIconVisibility(bool InActive);
	void SetNameVisibility(bool InActive);
	void SetDescVisibility(bool InActive);
	void SetCurrencyVisibility(bool InActive);

public:
	void ClickNo();

protected:
	UFUNCTION(BlueprintCallable, Category = "GsUI")
	void OnClickYes();

	UFUNCTION(BlueprintCallable, Category = "GsUI")
	void OnClickNo();

	// 아이콘 생성 시
	UFUNCTION()
	void OnCreateIcon(UGsUIIconBase* InIcon);

	// 아이콘 갱신 시
	UFUNCTION()
	void OnRefreshIcon(int32 InIndex, UGsUIIconBase* InIcon);

	void OnLongPressIcon(class UGsUIIconItem& InIcon);

	// 2022/10/20 PKT - KeyBoard Mapping
protected:
	virtual void OnInputCancel() override;
};
