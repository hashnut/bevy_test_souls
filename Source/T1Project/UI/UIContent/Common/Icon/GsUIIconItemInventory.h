// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/UIContent/Common/Icon/GsUIIconItemSimple.h"
#include "Blueprint/UserWidget.h"
#include "Item/GsItem.h"
#include "UTIL/GsTimeUtil.h"
#include "PaperSprite.h"
#include "DataSchema/GsSchemaEnums.h"
#include "GsUIIconItemInventory.generated.h"

class UImage;
class UTextBlock;
class UCanvasPanel;
class UWidgetSwitcher;

/**
 * 인벤토리 아이템 아이콘
 */
UCLASS()
class T1PROJECT_API UGsUIIconItemInventory : public UGsUIIconItemSimple
{
	GENERATED_BODY()

public:
	DECLARE_DELEGATE(FOnButtonReleaseItemSlot);
	DECLARE_DELEGATE_TwoParams(FOnMouseButtonEventItemSlot, const FGeometry&, const FPointerEvent&);

public:
	enum class SlotActionType
	{
		None = 0,
		Equip,
		UnEquip,
		Use,
		DisableUnEquip,

		Max,
	};

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUIIcon", meta = (BindWidget))
	UImage* _itemBG;
	UPROPERTY(BlueprintReadOnly, Category = "GsUIIcon", meta = (BindWidget))
	UImage* _selectImage;
	UPROPERTY(BlueprintReadOnly, Category = "GsUIIcon", meta = (BindWidget))
	UImage* _equipedImage;
	UPROPERTY(BlueprintReadOnly, Category = "GsUIIcon", meta = (BindWidget))
	UImage* _presetImage;
	UPROPERTY(BlueprintReadOnly, Category = "GsUIIcon", meta = (BindWidget))
	UImage* _redDot;
	UPROPERTY(BlueprintReadOnly, Category = "GsUIIcon", meta = (BindWidget))
	UImage* _lockImage;
	UPROPERTY(BlueprintReadOnly, Category = "GsUIIcon", meta = (BindWidget))
	UImage* _skillOptionImage;
	UPROPERTY(BlueprintReadOnly, Category = "GsUIIcon", meta = (BindWidget))
	UTextBlock* _itemNameText;
	UPROPERTY(BlueprintReadOnly, Category = "GsUIIcon", meta = (BindWidget))
	UCanvasPanel* _equipRootPanel;
	UPROPERTY(BlueprintReadOnly, Category = "GsUIIcon", meta = (BindWidget))
	UCanvasPanel* _unequipRootPanel;
	UPROPERTY(BlueprintReadOnly, Category = "GsUIIcon", meta = (BindWidget))
	UCanvasPanel* _useRootPanel;
	UPROPERTY(BlueprintReadOnly, Category = "GsUIIcon", meta = (BindWidget))
	UCanvasPanel* _disableUnEquip;
	UPROPERTY(BlueprintReadOnly, Category = "GsUIIcon", meta = (BindWidget))
	UWidgetSwitcher* _rankImgWidgetSwicher;
	UPROPERTY(BlueprintReadOnly, Category = "GsUIIcon", meta = (BindWidget))
	UImage* _dimmedImg;
	UPROPERTY(BlueprintReadOnly, Category = "GsUIIcon", meta = (BindWidget))
	UCanvasPanel* _vcItemPanel;
	UPROPERTY(BlueprintReadOnly, Category = "GsUIIcon", meta = (BindWidget))
	UTextBlock* _itemExpireTimeText;
	UPROPERTY(BlueprintReadOnly, Category = "GsUIIcon", meta = (BindWidget))
	UImage* _learnedImage;
	UPROPERTY(BlueprintReadOnly, Category = "GsUIIcon", meta = (BindWidget))
	UImage* _collectionImage;
	UPROPERTY(BlueprintReadOnly, Category = "GsUIIcon", meta = (BindWidget))
	class UGsUICoolTimeEffect* _uiCoolTime;
	UPROPERTY(BlueprintReadOnly, Category = "GsUIIcon", meta = (BindWidget))
	UTextBlock* _textBlockRemainTime;
	UPROPERTY(BlueprintReadOnly, Category = "GsUIIcon", meta = (BindWidget))
	UGsUIItemIconSlotEffect* _slotEffectMagicalForge;

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUIIcon", meta = (BindWidget))
	UImage* _autoImage;


private:
	bool _isExpiredTime = false;
	bool _isCircularBg;
	SlotActionType _slotActionType = SlotActionType::None;

private:
	// 특정 상황에서 OnClick() 시에, 각 슬롯에 대한 식별이 필요할때가 있으므로
	// paramValue 값을 기준으로 식별할 수 있도록 추가
	int32 _paramValue = 0;

protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	virtual void BeginDestroy() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

public:
	// player 가 소유하지 않는 아이템일 경우
	virtual void SetNotOwnItem(const uint32 InTID, const ItemAmount InAmount, const int32 InEnchantLevel = 0, const uint8 InEnchantShieldCount = UNCONFIRMED_ENCHANT_SHIELD, const uint8 InEnchantBonusIndex = UNCONFIRMED_ENCHANT_BONUS_INDEX) override;

	// player 가 소유한 아이템일 경우
	virtual void SetOwnedItem(const uint64 InItemDBID , const FindItemStorageType InStorageType = FindItemStorageType::Storage_Inventory) override;

public:
	virtual void SetEmptyIcon() override;

protected:	
	virtual void SetItemGrade(ItemGrade InGrade) override;
	virtual void SetItemSlotEffect(const EGsItemIconSlotEffectType InType) override;
	virtual void SetCooltimeUI(float InRemainSec, float InRate) override;
	virtual void OnClickItemIcon() override;

public:
	void ResetSlotAction();

public:
	void SetItemName(FText InName) const;
	void SetActiveIconBg(bool InActive);
	void SetRedDot(bool InActive);
	void SetCoolTimeBg(bool InIsCircularBg);
	void SetGrade(ItemGrade InGrade);
	void SetSlotEffect(const EGsItemIconSlotEffectType InType);

public:
	void SetSlotSelectionImage(bool InActive);
	void SetSlotEquipImage(bool InActive);
	void SetSlotUnEquipImage(bool InActive);
	void SetSlotUseImage(bool InActive);
	void SetSlotEquipedImage(bool InActive);
	void SetSlotDisableUnEquiped(bool InActive);
	void ResetCoolTime();
	void SetItemNameVisibility(bool InActive);
	void SetLockImage(bool InActive);
	void SetDimmedImage(bool InActive);
	void SetAutoImage(bool InActive);
	void SetVCImage(bool InActive);
	void SetPresetImage(bool InActive);
	void SetSlotActionType(SlotActionType InType);
	void SetPeriodTime(FGsItem* InItem);
	void SetLearnedImage(FGsItem* InItem);
	void SetItemCollectionMarking(const FGsItem* InItem, bool InActive = false);
	virtual void SetSlotEffectMagicalForge(FGsItem* InItem) override;
	void SetCoolTimeVisibility(bool InActive);

public:
	void SetSlotIntParamData(const int32 InParamValue) { _paramValue = InParamValue; };

public:
	const int32 GetSlotIntParamData() { return _paramValue; }
	UGsUIIconItemInventory::SlotActionType GetSlotActionType() { return _slotActionType; }

public:
	bool IsRunningCoolTime() { return _isCoolTime; }
	bool IsDimmed();

private:
	void SetItemIcon();
	void SetSkillOptionImage(FGsItem* InItemData);

	// 아이템 드래그다운으로 오토 사용 이벤트 전달하기 위해 추가
public:
	FOnButtonReleaseItemSlot OnButtonReleaseSlot;
	FOnMouseButtonEventItemSlot OnButtonDownSlot;

public:	
	UFUNCTION()
	void OnButtonRelease();

	void OnButtonDown(const FGeometry& Geometry, const FPointerEvent& MouseEvent);
};
