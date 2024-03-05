// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/UILib/Base/GsUIPopup.h"
#include "UI/UIContent/Define/GsUIContentDefine.h"
#include "GsUIPopupItemDetailCompare.generated.h"


//class UWidget;
class UImage;
class FGsItem;
class UGsUIPopupItemDetail;
/**
 * 아이템 비교창
 */
UCLASS()
class T1PROJECT_API UGsUIPopupItemDetailCompare : public UGsUIPopup
{
	GENERATED_BODY()
	

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUIPopupItemDetail* _mainItemDetail;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUIPopupItemDetail* _subItemDetail;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidget* _centerAlignSpacer;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UImage* _bgImg;


private:
	uint64 _mainItemDbid = 0;
	uint64 _subItemDbid = 0;
	TSharedPtr<FGsItem> _notOwnedItemPtr;

protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

protected:
	virtual void OnInputOk() override;
	virtual void OnInputCancel() override;

public:
	void SetItemData(FGsItem* InTargetItem, uint64 InOwnedItemDbid, bool InHideButtons, bool InCenterPopup, bool InHideBG, bool InHideButtonsBottom = false, bool InUseLock = true);
	void RemoveItemData(const uint64 InItemDBID);

public:
	void SetCenterPos(bool InCenterPopup);
	void SetbgImg(bool InHideBG);

private:
	void CloseMainDetailUI();
	void CloseSubDetailUI();
	void OnClickCompareBtn();
	void OnCompareNextSlotItem(const EGsEquipSlotType InTargetSlotType, const bool InHideBtnFlag, const bool InHideButtonsBottom, const bool InUseLock);
	void OnChangeMainItemEnchantCount(int32 InCount);
	void OnChangeSubItemEnchantCount(int32 InCount);

private:
	const FGsItem* FindMainItemData();
	const FGsItem* FindSubItemData();

public:
	void UpdateItemData(const uint64 InItemDBID, bool InSkipUpdateDetailInfo = false);

protected:
	UFUNCTION()
	FEventReply OnPressBGImg(FGeometry InGeometry, const FPointerEvent& InMouseEvent);

};
