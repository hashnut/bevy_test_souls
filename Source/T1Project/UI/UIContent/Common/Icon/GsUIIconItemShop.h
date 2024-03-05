// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/UIContent/Common/Icon/GsUIIconItem.h"
#include "Message/GsMessageGameObject.h"
#include "Components/ScaleBox.h"
#include "DataSchema/GsSchemaEnums.h"
#include "GsUIIconItemShop.generated.h"

class UCanvasPanel;
class UGsGrayscaleImage;
class UImage;
class UScaleBox;
class UWidgetSwitcher;
struct FGsNpcShopItemInfo;

/**
 * 상점 아이템 아이콘
 */
UCLASS()
class T1PROJECT_API UGsUIIconItemShop : public UGsUIIconItem
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUIIcon", meta = (BindWidget))
	UImage* _itemBG;
	UPROPERTY(BlueprintReadOnly, Category = "GsUIIcon", meta = (BindWidget))
	UImage* _dimmedImg;
	UPROPERTY(BlueprintReadOnly, Category = "GsUIIcon", meta = (BindWidget))
	UImage* _limitLock;
	UPROPERTY(BlueprintReadOnly, Category = "GsUIIcon", meta = (BindWidget))
	UImage* _skillOptionImage;
	UPROPERTY(BlueprintReadOnly, Category = "GsUIIcon", meta = (BindWidget))
	class UScaleBox* _limitText;
	UPROPERTY(BlueprintReadOnly, Category = "GsUIIcon", meta = (BindWidget))
	class UWidgetSwitcher* _rankImgWidgetSwicher;
	UPROPERTY(BlueprintReadOnly, Category = "GsUIIcon", meta = (BindWidget))
	class UCanvasPanel* _vcItemPanel;
	UPROPERTY(BlueprintReadOnly, Category = "GsUIIcon", meta = (BindWidget))
	class UGsGrayscaleImage* _iconImage;
	UPROPERTY(BlueprintReadOnly, Category = "GsUIIcon", meta = (BindWidget))
	class UCanvasPanel* _ownedTextPanel;
	UPROPERTY(BlueprintReadOnly, Category = "GsUIIcon", meta = (BindWidget))
	UImage* _collectionImage;

protected:
	FGsNpcShopItemInfo* _npcShopItemInfo;

private:
	// 특정 상황에서 OnClick() 시에, 각 슬롯에 대한 식별이 필요할때가 있으므로
	// paramValue 값을 기준으로 식별할 수 있도록 추가
	int32 _paramValue = 0;

protected:
	// UUserWidget override
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

public:
	// UGsUIIconBase override
	virtual void SetEmptyIcon() override;
	virtual void SetImageGrayscale(const bool InIsGray) override;

public:
	// UGsUIIconItem override
	virtual void SetNotOwnItem(const uint32 InTID, const ItemAmount InAmount, const int32 InEnchantLevel = 0, const uint8 InEnchantShieldCount = UNCONFIRMED_ENCHANT_SHIELD, const uint8 InEnchantBonusIndex = UNCONFIRMED_ENCHANT_BONUS_INDEX) override;
	virtual void SetOwnedItem(const uint64 InItemDBID, const FindItemStorageType InStorageType = FindItemStorageType::Storage_Inventory) override;
	virtual void SetIconImageSprite(class UPaperSprite* InSprite) override;
	virtual void SetItemGrade(ItemGrade InGrade) override;
	virtual void SetItemSlotEffect(const EGsItemIconSlotEffectType InType) override;

public:
	void SetItemIcon();
	// 상점 아이콘에 필요한 추가 정보를 설정한다.
	void SetItemShopInfo(FGsNpcShopItemInfo* InNpcShopItemInfo);
	// 아이콘의 상태를 갱신한다. ( 매진, 잠금 표시 등 )
	void InvalidateItem();

public:
	void SetActiveIconBg(bool InActive);

public:
	void SetDimmedImage(bool InActive);
	void SetLimitLock(bool InActive);
	void SetLimitText(bool InActive);
	void SetGradeImage(bool InActive);
	void SetVCImage(bool InActive);
	void SetOwnedText(bool InActive);
	void SetItemCollectionMarking(const FGsItem* InItem, bool InActive = false);

private:
	void SetSkillOptionImage(FGsItem* InItemData);

public:
	void SetSlotIntParamData(const int32 InParamValue) { _paramValue = InParamValue; };

public:
	const int32 GetSlotIntParamData() { return _paramValue; };
};