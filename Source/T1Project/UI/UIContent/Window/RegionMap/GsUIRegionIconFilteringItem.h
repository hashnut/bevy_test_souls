#pragma once

#include "CoreMinimal.h"

#include "Map/GsMapIconType.h"
#include "UMG/Public/Blueprint/UserWidget.h"
#include "UI/UIControlLib/Interface/GsEntryInterface.h"
#include "UI/UIControlLib/Control/GsToggleGroupEntry.h"
#include "GsUIRegionIconFilteringItem.generated.h"

/*
* region icon filtering item
*/

class UImage;
class UWidgetSwitcher;

class UGsButton;

UCLASS()
class UGsUIRegionIconFilteringItem : 
	public UUserWidget, 
	public IGsEntryInterface,
	public IGsToggleGroupEntry
{
	GENERATED_BODY()
protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UImage* _imgIconType;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnCheck;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidgetSwitcher* _switcherCheckType;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI")
	FText _textFilteringName;
	bool _bIsSelected = false;
	EGsRegionMapIconFiltering _filteringType;

protected:
	// UUserWidget override
	virtual void NativeOnInitialized() override;

public:
	// IGsToggleGroupEntry override
	virtual void SetIsSelected(bool bInIsSelected) override;
	virtual bool GetIsSelected() const override;

public:

	// click check
	UFUNCTION()
	void OnClickCheck();

	void SetFilteringItemData(EGsRegionMapIconFiltering In_filteringType);

	void OnSelectCheck( bool In_isSelect);
};