// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/UILib/Base/GsUIPopup.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "GsUIPopupConfirmDiscount.generated.h"

DECLARE_MULTICAST_DELEGATE_ThreeParams(FOnClickOkCoupon, const TArray<BMProductIdAmountPair>&, bool, uint64);

class UImage;
class FGsItem;
class UTextBlock;
class UGsButton;
class FGsCoolTimeManager;
class UGsDynamicIconSlotHelper;
class UGsHorizontalBoxIconSelector;
/**
 * 
 */
UCLASS()
class T1PROJECT_API UGsUIPopupConfirmDiscount : public UGsUIPopup
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsHorizontalBoxIconSelector* _iconSelector;
	UPROPERTY()
	UGsDynamicIconSlotHelper* _iconSlotHelper;

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _itemNameText;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _remainTimeText;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnOk;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnCancel;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UImage* _remainTimeImage;

private:
	FDateTime _remainDateTime;
	uint64 _couponItemDbid = 0;
	int32 _leftDays = 0;
	bool _isRunningRemainTime = false;
	TArray<BMProductIdAmountPair> _reservePairList;
	bool _reserveBulkState = false;
	FGsCoolTimeManager* _coolTimeMgr = nullptr;

public:
	FOnClickOkCoupon OnClickOkCoupon;


protected:
	virtual void NativeOnInitialized() override;
	virtual void BeginDestroy() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

	
private:
	void SetRemainTime(const FGsItem* InCouponItem);

public:
	void SetCouponInfo(const uint64 InCouponItemDbid, TArray<BMProductIdAmountPair> InPairList, bool InIsBulkSale);


protected:
	UFUNCTION()
	void OnClickOk();
	UFUNCTION()
	void OnClickCancel();
	UFUNCTION()
	void OnRefreshItemIcon(int32 InIndex, class UGsUIIconBase* InIcon);
};
