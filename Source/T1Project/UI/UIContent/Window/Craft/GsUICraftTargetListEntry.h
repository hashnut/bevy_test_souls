#pragma once

#include "Runtime/UMG/Public/Blueprint/IUserObjectListEntry.h"
#include "Runtime/UMG/Public/Blueprint/UserWidget.h"

#include "Item/Craft/GsCraftTarget.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "UI/UIControlLib/Interface/GsEntryInterface.h"

#include "GsUICraftTargetListEntry.generated.h"

class IGsToggleGroupEntry;
class UGsButton;
class UGsCheckBox;
class UGsDynamicIconSlotHelper;
class UGsHorizontalBoxIconSelector;
class UGsToggleButton;
class UGsUIIconBase;
class UGsUIIconItem;
class UTextBlock;
class UWidget;

UCLASS()
class UGsUICraftTargetListItem final : public UObject
{
	GENERATED_BODY()

public:
	CraftId _craftTargetId = INVALID_CRAFT_ID;
	// 로딩 딜레이 개선을 위해, display 시점에 데이터 조회하도록 변경.
	//FText _title;
	FGsCraftTargetObject _craftTargetObject;
	//FText _lockedMessage;
	//bool _favorite = false;
	//bool _isEnoughCostPackage = false;
};

/** 제작 대상 리스트엔트리 */
UCLASS()
class UGsUICraftTargetListEntry final : public UUserWidget, public IUserObjectListEntry
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsToggleButton* _btnMain;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsHorizontalBoxIconSelector* _iconSelector;
	
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _txtTitle;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnLock;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsToggleButton* _btnFavorite;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidget* _wgtDimmed;

	UPROPERTY()
	UGsDynamicIconSlotHelper* _slotHelperIcon;

protected:
	void NativeOnInitialized() final;

	void NativeDestruct() final;

	void NativeOnListItemObjectSet(UObject* InListItemObject) final;

public:
	void Invalidate();

	/************************************************************************/
	/* getter																*/
	/************************************************************************/

public:
	CraftId GetCraftTargetId() const;

	IGsToggleGroupEntry* GetToggleGroupEntry();
	
	/************************************************************************/
	/* event																*/
	/************************************************************************/

private:
	UFUNCTION()
	void OnRefreshIconItem(int32 InIndex, UGsUIIconBase* InIcon);

	void NativeOnItemSelectionChanged(bool bIsSelected) final;

	UFUNCTION()
	void OnClickedLock();

	UFUNCTION()
	void OnClickedFavorite();

	void OnFinishedLongPressEventItemIcon(UGsUIIconItem& InIcon);
};
