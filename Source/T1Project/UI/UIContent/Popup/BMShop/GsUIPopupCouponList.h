// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/UILib/Base/GsUIPopup.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "GsUIPopupCouponList.generated.h"

class FGsItem;
class UPanelWidget;
class UScrollBox;
class UGsButton;
class UGsUICouponListEntry;
class UWidgetSwitcher;
class UGsDynamicPanelSlotHelper;
/**
 * 
 */
UCLASS()
class UGsUIPopupCouponList : public UGsUIPopup
{
	GENERATED_BODY()
	
protected:
	// product List
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GsUI")
	TSubclassOf<UUserWidget> _couponListEntryWidgetClass;
	UPROPERTY()
	UGsDynamicPanelSlotHelper* _couponListPanelSlotHelper;

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UPanelWidget* _couponListRootPanel;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnCancel;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnOk;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidgetSwitcher* _listSwitcher; // 0 : itemlist 1 : empty text

private:
	TArray<uint64> _couponItemDBIDList;
	uint64 _selectedItemDBID = 0;
	BMProductId _productID = 0;

protected:
	virtual void NativeOnInitialized() override;
	virtual void BeginDestroy() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

private:
	void OnClickCouponEntry(uint64 InItemDbid, bool InIsSelect);
	void ExpiredRemainTime(uint64 InItemDBID);
	void CheckOkBtnState();
	void SortCouponList(TArray<FGsItem*>& outSortList);

public:
	void SetCouponList(const BMProductId InProductID);

public:
	void RemoveCouponItem(const uint64 InCouponItemDbid);


protected:
	UFUNCTION()
	void OnOk();
	UFUNCTION()
	void OnCancel();
	UFUNCTION()
	void CouponListRefresh(int32 InIndex, UWidget* InEntry);

	void OnInputOk() final;
	void OnInputCancel() final;
};
