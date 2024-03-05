// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/UILib/Base/GsUIPopup.h"
#include "Shared/Client/SharedEnums/SharedCommonEnum.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "GsUIPopupItemDetailMain.generated.h"

class FGsItem;
class UGsUIPopupItemDetail;

DECLARE_MULTICAST_DELEGATE(FOnCloseItemDetailMainDelegate);
/**
 * 아이템 상세창 메인 wbp class
 */
UCLASS()
class T1PROJECT_API UGsUIPopupItemDetailMain : public UGsUIPopup
{
	GENERATED_BODY()

public:
	enum CenterPosType : uint8
	{
		CenterPos = 0,
		LeftPos ,
		RightPos ,
		GuildStoragePos,
	};

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUIPopupItemDetail* _itemDetailWbp;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidget* _centerAlignSpacer;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidget* _centerAlignSpacerLeft;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidget* _sizeBoxGuildStorageList;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidget* _bgImg;


	/*
	* UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UImage* _imgBlackBG;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UNamedSlot* _inventoryModeRoot;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UNamedSlot* _infoModeRoot;
	*/

public:
	FOnCloseItemDetailMainDelegate OnCloseItemDetailMainDelegate;

private:
	bool _keepSelectionFlag = false;
	uint64 _itemDbid = 0;
	TSharedPtr<FGsItem> _notOwnedItemPtr;


protected:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

protected:
	virtual void OnInputCancel() override;

public:
	virtual EGsUIHideFlags GetHideFlagType() const override { return EGsUIHideFlags::UI_HIDE_ITEM_DETAIL; }

public:
	void SetItemData(const FGsItem* InItem, bool InHideButtons, CenterPosType InCenterPosType, bool InHideBG, bool InHideButtonsBottom = false, bool InUseLock = true, bool InUseItemCollectionMarking = false);
	void SetItemData(const uint32 InItemTID, bool InHideButtons, CenterPosType InCenterPosType, bool InHideBG);
	void SetItemDataItemCollectionConditionItemOnly(const FGsItem* InItem, bool InShowBtn, bool InEnchant, bool InChanged, bool InRegister, bool InIsBtnRegisterType);
	void SetItemCollectionRegisterInfoText(const FText& InText);

public:
	void SetCurrencyData(const CurrencyType InCurrencyType, const Currency InAmount, bool InHideButtons, const CenterPosType InCenterPosType, bool InHideBG);

public:
	void SetCenterPos(CenterPosType InCenterPosType);
	void SetbgImg(bool InHideBG);
	void SetKeepSelectionFlag(bool InFlag) { _keepSelectionFlag = InFlag; }

public:
	void UpdateItemData(const uint64 InItemDBID);
	void RemoveItemData(const uint64 InItemDBID);

private:
	const FGsItem* FindItemData();

private:
	void CloseItemDetailUI();


};
