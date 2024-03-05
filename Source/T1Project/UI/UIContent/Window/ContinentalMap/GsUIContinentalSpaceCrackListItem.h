#pragma once

#include "CoreMinimal.h"

#include "UI/UIControlLib/Control/GsToggleGroupEntry.h"
#include "UI/UIControlLib/Interface/GsEntryInterface.h"

#include "UMG/Public/Blueprint/UserWidget.h"

#include "GsUIContinentalSpaceCrackListItem.generated.h"

class UWidgetSwitcher;
class UGsButton;

class UImage;

UCLASS()
class UGsUIContinentalSpaceCrackListItem :
	public UUserWidget,
	public IGsToggleGroupEntry,
	public IGsEntryInterface
{
	GENERATED_BODY()
protected:
	// 0: no deco, 1: deco
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidgetSwitcher* _switcherDecoCheckType;

	// 0: off, 1: on
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidgetSwitcher* _switcherActiveType;
	// hide: not select, show: select
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UImage* _imgSelect;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnItem;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	FText _textMapName;

	bool _bIsSelected = false;

	// virtual func
protected:
	// UUserWidget override
	virtual void NativeOnInitialized() override;

public:
	// IGsToggleGroupEntry override
	virtual void SetIsSelected(bool bInIsSelected) override;
	virtual bool GetIsSelected() const override;

public:
	// set data 
	void SetContinentalSpaceCrackListItemData(bool In_isDeco, const FText& In_name, bool In_isSpawned);
	
	// 클릭 버튼
	UFUNCTION()
	void OnClickButton();
};