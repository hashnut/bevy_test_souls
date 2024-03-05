#pragma once
#include "CoreMinimal.h"

#include "UI/UILib/Base/GsUIWindow.h"
#include "UI/UIControlLib/Control/GsToggleGroup.h"

#include "Shared/Client/SharedEnums/SharedItemEnum.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"

#include "GsUIWindowRepair.generated.h"


class UGsButton;
class UScrollBox;
class UGsDynamicPanelSlotHelper;
class UTextBlock;
class FGsItem;
class UGsUIRepairListEntry;
class UWidgetSwitcher;
class UGsUIItemDefaultInfo;
class UGsUIItemDetailInfo;
class UWrapBox;
struct FGsSchemaItemCommon;
/**
 * 합성강화 복구 시스템
 * 장비 복구 Window
 */
UCLASS()
class T1PROJECT_API UGsUIWindowRepair : public UGsUIWindow
{
	GENERATED_BODY()

protected:
	const FTextKey DELIVERY_BOX_TEXT = TEXT("DeliveryBoxText");

	struct RepairItemData
	{
		const FGsSchemaItemCommon* itemTableData = nullptr;
		Level itemLevel = INVALID_LEVEL;
		EnchantBonusIndex enchantBonusIndex = INVALID_ENCHANT_BONUS_INDEX;
		ItemDBId itemDBId;
	};

protected:
	// 상단 영역
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnClose;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnBack;

	// 좌측 카테고리 목록
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UScrollBox* _scrollBoxCategoryTab;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GsUI")
	TSubclassOf<UUserWidget> _subClassOfCategoryTab;

	UPROPERTY()
	UGsDynamicPanelSlotHelper* _slotHelperCategoryTab;

	FGsToggleGroup _toggleGroupCategoryTab;

	// 중간 아이템 슬롯 목록
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UScrollBox* _scrollBoxItem;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWrapBox* _panelItemList;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GsUI")
	TSubclassOf<UUserWidget> _subClassOfItem;

	UPROPERTY()
	UGsDynamicPanelSlotHelper* _slotHelperItem;

	TWeakObjectPtr<UGsUIRepairListEntry> _selectedItemSlot;
	
	// 우측 아이템 정보
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUIItemDefaultInfo* _itemDefaultInfo;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUIItemDetailInfo* _itemDetailInfo;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockNotice;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnRepair;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidgetSwitcher* _switcherInfo;

protected:
	TArray<RepairItemData> _repairableWeaponList;	// 무기 목록
	TMap<ItemCategorySub, TArray<RepairItemData>> _repairableItemListMap;	// 무기 이외 목록
	TArray<ItemCategorySub> _categoryList;

public:
	virtual void BeginDestroy() override;

protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

	virtual bool SyncContentState() override;

protected:
	UFUNCTION()
	void OnRefreshCategoryTab(int32 InIndex, UWidget* InEntry);
	
	UFUNCTION()
	void OnRefreshItem(int32 InIndex, UWidget* InEntry);

	UFUNCTION()
	void OnClickRepair();

	UFUNCTION()
	void OnSelectChangedCategoryTab(int32 InIndex);

	UFUNCTION()
	void OnClickedItemSlot(UUserWidget* InWidget);

protected:
	void InitializeCategoryTab(const TArray<ItemDataCreated>& InItemList);
	void InvalidateInfo();

	void SortItemList(TArray<RepairItemData>& InItemList);
};

