#pragma once

#include "Runtime/CoreUObject/Public/UObject/WeakObjectPtr.h"
#include "Runtime/UMG/Public/Blueprint/UserWidget.h"
#include "UI/UIControlLib/Interface/GsEntryInterface.h"

#include "Item/Collection/GsItemCollectionSlotState.h"
#include "UI/UIContent/Common/Icon/GsUIIconItem.h"
#include "T1Project.h"
#include "GsUIItemCollectionSlot.generated.h"



class FGsItemCollectionState;
class UGsDynamicIconSlotHelper;
class UGsUIIconBase;
class UGsUIIconItem;
class UGsUIIconItemCollectionSlot;
class UGsUIWindowItemCollection;
class UGsWrapBoxIconSelector;
class UTextBlock;
class UGsButton;
class UWidgetSwitcher;
class UGsToggleButton;
class FGsItemCollectionSlotState;

DECLARE_DELEGATE_ThreeParams(FOnClickItemCollectionSlotIcon, const UGsUIIconItem*, const CollectionId, const CollectionConditionId);

UCLASS()
class UGsUIItemCollectionSlot : public UUserWidget, public IGsEntryInterface
{
	GENERATED_BODY()
	

protected:
	// 2021/08/03 PKT - title..
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textTitle;

	// 2021/08/03 PKT - stat..
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textDesc;

	// 2021/08/03 PKT - date..
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textSubDesc;

	// 2021/08/03 PKT - inner Slot Icon
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsWrapBoxIconSelector* _iconSelector;

	UPROPERTY()
	UGsDynamicIconSlotHelper* _slotHelper;

	// 2021/08/03 PKT - lock / unlock switcher
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidgetSwitcher* _switcherState;

	// 2021/08/03 PKT - 진행 사항..
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textStateCount;

	// 2021/08/03 PKT - lock button
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnLock;

	// 2021/08/03 PKT - book mark button
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsToggleButton* _BtnBookmark;

	TWeakObjectPtr<const UGsUIWindowItemCollection> _window;

	const class FGsItemCollectionState* _itemCollection;

public:
	/** 컬렉션의 아이템 아이콘 슬롯 클릭 시 발생 */
	FOnClickItemCollectionSlotIcon OnClickItemCollectionSlotIcon;

protected:
	// UUserWidget override
	virtual void NativeOnInitialized() override;
	virtual void NativeDestruct() override;

	/************************************************************************/
	/* Status                                                               */
	/************************************************************************/	
	virtual void SetCollectionState(const FGsItemCollectionState* InData);
	
public:
	virtual void OnShow() override;
	virtual void OnHide() override;

public:
	void SetData(const UGsUIWindowItemCollection* InWindow, const FGsItemCollectionState* InData);
	const FGsItemCollectionState* GetData() const;

	int32 GetItemIconCount() const;
	//const UGsUIIconItem* GetItemIconByIndex(int32 InIndex) const;

private:
	UFUNCTION(BlueprintCallable, Category = "GsUI")
	void OnCreateIconOwnedItem(UGsUIIconBase* InIcon);

	UFUNCTION(BlueprintCallable, Category = "GsUI")
	void OnRefreshIconOwnedItem(int32 InIndex, UGsUIIconBase* InIcon);

	void OnClickSlotItemIcon(UGsUIIconItem& InIcon);

	UFUNCTION(BlueprintCallable, Category = "GsUI")
	void OnClickBtnLock();

	UFUNCTION(BlueprintCallable, Category = "GsUI")
	void OnClickBtnBookMark();
};