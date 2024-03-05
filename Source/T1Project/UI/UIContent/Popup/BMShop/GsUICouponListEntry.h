// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UI/UIControlLib/Interface/GsEntryInterface.h"
#include "GsUICouponListEntry.generated.h"

DECLARE_MULTICAST_DELEGATE_TwoParams(FOnClickCouponBox, uint64, bool);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnExpiredRemainTime, uint64);

class UTextBlock;
class UImage;
class FGsItem;
class UGsButton;
class FGsCoolTimeManager;
class UGsDynamicIconSlotHelper;
class UGsHorizontalBoxIconSelector;
/**
 * 
 */
UCLASS()
class T1PROJECT_API UGsUICouponListEntry : public UUserWidget
	, public IGsEntryInterface
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
	UImage* _selectionImage;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnSelect;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UImage* _remainTimeImage;


private:
	FDateTime _remainDateTime;
	uint64 _couponItemDbid = 0;
	int32 _leftDays = 0;
	bool _isSelected = false;
	bool _isPeriodCoupon = true;
	bool _isRunningRemainTime = false;
	FGsCoolTimeManager* _coolTimeMgr = nullptr;

public:
	FOnClickCouponBox OnClickCoupon;
	FOnExpiredRemainTime OnExpiredRemainTime;


protected:
	virtual void NativeOnInitialized() override;
	virtual void BeginDestroy() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

protected:
	virtual void OnShow() override;
	virtual void OnHide() override;

private:
	void SetRemainTime(const FGsItem* InCouponItem);

public:
	void SetCouponInfo(const uint64 InCouponItemDbid, const bool InIsSelected);
	void SetSelectionImg(const bool InFlag);

public:
	uint64 GetCouponItemDbid() { return _couponItemDbid; }
	const bool IsPeriodCouponItem() { return _isPeriodCoupon; }
	const bool IsRunningRemainTime() { return _isRunningRemainTime; }


protected:
	UFUNCTION()
	void OnClickCouponList();
	UFUNCTION()
	void OnRefreshItemIcon(int32 InIndex, class UGsUIIconBase* InIcon);
	
};
