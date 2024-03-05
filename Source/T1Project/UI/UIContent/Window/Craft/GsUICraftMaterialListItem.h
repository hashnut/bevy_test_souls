#pragma once

#include "Runtime/UMG/Public/Blueprint/UserWidget.h"

#include "Shared/Client/SharedEnums/SharedCommonEnum.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "UI/UIControlLib/Interface/GsEntryInterface.h"

#include "GsUICraftMaterialListItem.generated.h"

class IGsToggleGroupEntry;
class UGsButton;
class UGsDynamicIconSlotHelper;
class UGsHorizontalBoxIconSelector;
class UGsToggleButton;
class UGsUIIconBase;
class UGsUIIconItem;
class UImage;
class UTextBlock;
class UWidget;

UCLASS()
class UGsUICraftMaterialListItem : public UUserWidget, public IGsEntryInterface
{
	
	GENERATED_BODY()

public:
	DECLARE_DELEGATE_OneParam(FGsOnClickCraftMaterialListItem, const int32);

	struct Parameters
	{
		int32 _index;
		CostType _costType;
		CurrencyType _currencyType;
		ItemId _itemId;
		Level _itemLevel;
		uint64 _requireCount;
		uint64 _ownedCount;
		bool _isEnoughCostElement;
		bool _isEnoughCostElementOnlyFirstCandidate;
		bool _hasNavigateToCraft;
		bool _hasCandidate;
		FGsOnClickCraftMaterialListItem _onClickedCraft;
		FGsOnClickCraftMaterialListItem _onClickedCandidate;
	};

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsToggleButton* _tglMain;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsHorizontalBoxIconSelector* _iconSelector;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _txtTitle;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _txtSubTitle;
	
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidget* _wgtCraftIcon;
	
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UImage* _wgtCandidateIcon;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidget* _wgtDimmed;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidget* _selectedOveray;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnOverayCraft;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnOverayCandidate;

	UPROPERTY()
	UGsDynamicIconSlotHelper* _slotHelperIcon;

	Parameters _parameters;

protected:
	void NativeOnInitialized() override;

	void NativeDestruct() override;

	/************************************************************************/
	/* setter																*/
	/************************************************************************/
	
public:
	void SetParameters(Parameters InParameters);

	/************************************************************************/
	/* getter																*/
	/************************************************************************/

public:
	IGsToggleGroupEntry* GetToggleGroupEntry() const;

	/************************************************************************/
	/* event																*/
	/************************************************************************/

private:
	UFUNCTION()
	void OnRefreshIconItem(int32 InIndex, UGsUIIconBase* InIcon);

	void OnFinishedLongPressEventItemIcon(UGsUIIconItem& InIcon);
	
	UFUNCTION()
	void OnToggleButtonSelectedMain(bool InSelected);

	UFUNCTION()
	void OnClickedCraft();

	UFUNCTION()
	void OnClickedShowCandidate();

};
