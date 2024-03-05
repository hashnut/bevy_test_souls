// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/UIContent/Common/Icon/GsUIIconBase.h"
#include "UI/UIControlLib/Interface/GsNameTooltipInterface.h"
#include "UI/UIContent/Common/Item/GsUIItemIconSlotEffect.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "Shared/Client/SharedEnums/SharedCommonEnum.h"
#include "Shared/Client/SharedEnums/SharedItemEnum.h"
#include "Shared/Client/SharedEnums/SharedCommonEnum.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "DataSchema/GsSchemaEnums.h"
#include "GsUIIconItem.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FOnClickItemIcon, class UGsUIIconItem&);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnFinishedLongPressEvent, class UGsUIIconItem&);

class FGsItem;
/**
 * 아이템/재화 아이콘 기본 클래스
 */
UCLASS(meta = (DisableNativeTick))
class T1PROJECT_API UGsUIIconItem : public UGsUIIconBase, public IGsNameTooltipInterface
{
	GENERATED_BODY()

public:
	enum FindItemStorageType
	{
		Storage_All = 0,
		Storage_Inventory,
		Storage_Depot ,
		Storage_Shop,

		StorageMax,
	};

	// 어떤방식으로 아이템 상세창을 출력할지 정함
	enum UseDisplayDetailWndType : uint8
	{
		NotUseDetailWndType = 0,	// 상세창 출력 미사용
		LongPressType,				// 롱프레스를 통한 상세창 사용
		OnClickType,				// 단순 터치로 인한 상세창 사용
	};


protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUIIcon", meta = (BindWidget))
	class UTextBlock* _itemCountText;
	UPROPERTY(BlueprintReadOnly, Category = "GsUIIcon", meta = (BindWidget))
	class UGsButton* _itemSlotButton;
	UPROPERTY(BlueprintReadOnly, Category = "GsUIIcon", meta = (BindWidget))
	class UTextBlock* _enchantNum;

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUIIcon")
	UGsUIItemIconSlotEffect* _slotEffectWidget = nullptr;


protected:
	TSharedPtr<FGsItem> _notOwnItem;
	uint64 _tableId = 0; // ItemId, 재화일 경우 CurrencyType
	uint64 _itemDBID = INVALID_ITEM_DB_ID;
	CurrencyType _currencyType;
	int64 _amount;
	FindItemStorageType _eFindItemStorageType = FindItemStorageType::Storage_Inventory;
	UseDisplayDetailWndType _eDisplayDetailWndType = UseDisplayDetailWndType::LongPressType;

private:
	bool _nameToolTipFlag = false;
	bool _isHoverMode = false;

private:	
	FSoftObjectPath _lastSoftObjectPath;

private:
	FDelegateHandle _longPressDelegate;

public:
	FOnClickItemIcon OnClickSlot;
	FOnFinishedLongPressEvent OnFinishedLongPressEvent;

protected:
	// UUserWidget override
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	virtual void NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent);
	virtual void NativeOnMouseLeave(const FPointerEvent& InMouseEvent);

protected:
	// UGsUIIconBase override
	virtual void SetEmptyIcon() override;

public:
	virtual void SetIconImage(const FSoftObjectPath& InPath) override;

protected:
	// 자손에서 구현
	virtual void SetItemGrade(ItemGrade InGrade) {}
	virtual void SetItemSlotEffect(const EGsItemIconSlotEffectType InType) {}

public:
	// player 가 소유하지 않는 아이템일 경우
	virtual void SetNotOwnItem(const uint32 InTID, const ItemAmount InAmount, const int32 InEnchantLevel = 0 , const uint8 InEnchantShieldCount = UNCONFIRMED_ENCHANT_SHIELD, const uint8 InEnchantBonusIndex = UNCONFIRMED_ENCHANT_BONUS_INDEX);

	// player 가 소유한 아이템일 경우
	virtual void SetOwnedItem(const uint64 InItemDBID , const FindItemStorageType InStorageType = FindItemStorageType::Storage_Inventory);

	// Sprite 세팅 코드(Asyc로드 시 사용 통일성을 위해 함수로 따로 뺌)
	virtual void SetIconImageSprite(class UPaperSprite* InSprite) {}

	virtual void SetSlotEffectMagicalForge(FGsItem* InItem) {}

public:
	void SetItemNameToolTipFlag(bool InFlag) { _nameToolTipFlag = InFlag; }
	void SetItemNameToolTipHoverMode(bool InIsHover) { _isHoverMode = InIsHover; }
	void SetOnChangeDisplayDetailWndType(UseDisplayDetailWndType InUseType);
	void SetOnClickLockSec(float InSec);

public:
	// 소유하지 않은 아이템중, 확정 아이템 출력에 필요한 정보
	void SetEquipmentInfo(FGsItem* InItemData, const uint8 InEnchantShieldNum, const uint8 InBonusStatIndex);

public:
	virtual FGsItem* GetItemIconData() const;

public:
	// 재화일 경우
	virtual void SetCurrency(const CurrencyType InCurrencyType, const Currency InMinAmount, const Currency InMaxAmount = MIN_ITEM_AMOUNT);
	CurrencyType GetCurrencyType() const;
	Currency GetAmount() const;
	const struct FGsSchemaCurrencyData* GetCurrencyTable() const;

public:
	// Exp일 경우
	virtual void SetExpPercent(double InExpPercent);

public:
	//공통
	virtual void SetItemCount(int64 InCount);
	virtual void SetItemCountEX(int64 InCount, bool InForcedShowText = false);
	virtual void SetItemCountMinMax(int64 InMinValue, int64 InMaxValue);
	virtual void SetAllOwnedItemCountDisplay();	// 보유 아이템 수량 표시. 백만 이상은 99.9만+ 로 표기
	virtual void SetOwnedItemCountByItemTid();	// 보유 아이템 수량 표시. 백만 이상은 99.9만+ 로 표기
	virtual void SetItemEnchantLevel(int32 InLevel);

	CostType GetCostType() const;
	uint64 GetTableId() const { return _tableId; }
	FSoftObjectPath GetLastSoftObjectPath() const { return _lastSoftObjectPath; }
	ItemDBId GetDBID() const { return _itemDBID; }

	/** 수량 텍스트의 가시성 설정 */
	void SetItemCountTextVisibility(ESlateVisibility InVisibility);

	/** 강화 수치 텍스트의 가시성 설정 */
	void SetEnchantNumTextVisibility(ESlateVisibility InVisibility);


private:
	void SetEnchantTextColor();
	void OnFinishedLongPress();
	void OnFinishedItemNameToolTip();

public:
	virtual void OpenNameTooltip() override;
	//virtual void CloseNameTooltip() override;

protected:
	UFUNCTION()
	virtual void OnClickItemIcon();

public:
	static uint8 GetGradeNumByGradeType(ItemGrade InItemGrade);
	// 컬랙션 정보 +0, +1 표시 할지
	static bool DefaultItemDetailOpener(UGsUIIconItem& InIcon, const bool InShowBg, bool In_isShowCollectionShow = false);
};
