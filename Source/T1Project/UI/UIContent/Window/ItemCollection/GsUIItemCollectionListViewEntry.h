#pragma once

/**
* file		GsUIItemCollectionListViewEntry.h
* @brief	Item Collection List View Entry
* @author	PKT
* @date		2022/01/04
**/

#include "CoreMinimal.h"
#include "Blueprint/IUserObjectListEntry.h"
#include "Item/Collection/GsItemCollectionBase.h"
#include "Item/Collection/EItemCollectionEnumData.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "UI/UIContent/Common/Icon/GsUIIconItem.h"
#include "Runtime/UMG/Public/Blueprint/IUserObjectListEntry.h"
#include "Runtime/UMG/Public/Blueprint/UserWidget.h"
#include "GsUIItemCollectionListViewEntry.generated.h"


/************************************************************************/
/* View List Entry Data                                                 */
/************************************************************************/
class FGsItemCollectionBase;

DECLARE_DELEGATE_ThreeParams(FOnClickedItemCollectionIcon, const UGsUIIconItem*, const CollectionId, const CollectionConditionId);

UCLASS()
class T1PROJECT_API UGsItemIconCollectionListViewEntryData : public UObject
{
	GENERATED_BODY()

public:
	const FGsItemCollectionBase* _collectionData = nullptr;

	FOnClickedItemCollectionIcon OnClickedItemIcon;
};

DECLARE_DELEGATE_TwoParams(FOnClickedCollectionEvent, UUserWidget*, uint32);
DECLARE_DELEGATE_ThreeParams(FOnLongPressdeIcon, UGsUIIconItem*, UUserWidget*, CollectionConditionId);

UCLASS()
class T1PROJECT_API UGsItemCollectionViewEntryData : public UObject
{
	GENERATED_BODY()
	/**
	 * Show Info
	 */
protected:
	const class FGsItemCollectionField* _fieldData = nullptr;
	bool _isSelected = false;
	CollectionConditionId _seletecdConditionId = INVALID_COLLECTION_CONDITION_ID;

public:
	void Initialze(const class FGsItemCollectionField* InData);
	void SetSelectedCondition(CollectionConditionId InCollectionConditionId);
	void SetSelectedField(bool InSelected);

	const class FGsItemCollectionField* GetData() const { return _fieldData; }
	bool IsSelectedCondition(CollectionConditionId InCollectionConditionId) const {
		return InCollectionConditionId == _seletecdConditionId;
	}
	bool IsSelected() const {	return _isSelected;	}

	FOnClickedCollectionEvent OnCliekedField;
	FOnClickedCollectionEvent OnClickedCondition;
	FOnLongPressdeIcon OnLongPressedIcon;
};



/************************************************************************/
/* View List UI Entry                                                   */
/************************************************************************/
class UImage;
class UTextBlock;
class UGsUIIconItem;
class UGsButton;
class UWidgetSwitcher;
class UGsToggleButton;
class UGsWrapBoxIconSelector;
class UGsDynamicIconSlotHelper;


UCLASS()
class UGsUIItemCollectionListViewEntry : public UUserWidget, public IUserObjectListEntry
{
	GENERATED_BODY()

	/************************************************************************/
	/* Blue Print Bind                                                      */
	/************************************************************************/
protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidgetSwitcher* _switcherBackGround;
	
	// 2021/09/29 PKT - Title
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textTitle;

	// 2023/11/16 PKT - Stat 표기
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textStat;

	// 2021/08/03 PKT - 유효 기간( 만료 기간 )
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textDate;

	/**
	 * Item Icon
	 */
	 // 2021/08/03 PKT - inner Slot Icon
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsWrapBoxIconSelector* _iconSelector;

	UPROPERTY()
	UGsDynamicIconSlotHelper* _slotHelper;

	// 2021/08/03 PKT - 등록한(완료) 갯수
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textCompleteProgressCount;

	// 2021/09/29 PKT - Active / InActive Switcher
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidgetSwitcher* _switcherActive;

	// 2021/09/29 PKT - 비활성화 상태일때는 "잠금"표시가 되는데 이 잠금 표시를 인터렉션 할 수 있다.
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnInActive;

	// 2021/08/03 PKT - book mark button
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsToggleButton* _BtnBookmark;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UImage* _selectedFieldImage;
	
	TMap<CollectionConditionId, bool> _registeredEffectFlags;
public:
	virtual void BeginDestroy() override;	

protected:
	virtual void NativeOnInitialized() override;	
	virtual void NativeDestruct() override;
	virtual void NativeOnListItemObjectSet(UObject* inListItemObject) override;

public:
	virtual void Refresh();

private:
	UFUNCTION()
	void OnRefreshItemConditionIcon(int32 InIndex, UGsUIIconBase* InIcon);
	
	UFUNCTION(BlueprintCallable, Category = "GsUI")
	void OnClickBtnLock();

	UFUNCTION(BlueprintCallable, Category = "GsUI")
	void OnClickBtnBookMark();

	void OnClickSlotItemIcon(UGsUIIconItem& InIcon);
	void OnLongPressedItemIcon(UGsUIIconItem& InIcon);
};