// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/UIContent/Popup/Chat/GsUIDropDownMenuChat.h"
#include "UI/UIControlLib/Control/GsToggleGroupEntry.h"
#include "GsTabSwitcher.h"
#include "GsUIChatTabUseSubTab.generated.h"

/**
 * 
 */

class UGsSwitcherButton;
class UTextBlock;

UCLASS()
class T1PROJECT_API UGsUIChatTabUseSubTab : public UGsUIDropDownMenuChat, public IGsToggleGroupEntry, public IGsTabSwitcher
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GsUI")
	FText _textTabName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GsUI")
	FLinearColor _tabSelectColor;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GsUI")
	FLinearColor _tabBaseColor;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GsUI")
	FLinearColor _tabDisableColor;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textSelect;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBase;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textDisable;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GsUIControl")
	bool UseSubTab{ false };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GsUIControl")
	int32 _slotIndexSelected = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GsUIControl")
	int32 _slotIndexUnselected = 0;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GsUIControl", meta = (EditCondition = "UseSubTab == true"))
	//int32 _slotIndexMenuOff = 2;

public:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

protected:
	virtual void OnMenuPopup() override;

	void SetSwitcherIndex(int32 InIndex);
	int32 GetSwitcherIndex() const;

public:
	virtual void SynchronizeProperties() override;

	// IGsToggleGroupEntry
	UFUNCTION(BlueprintCallable)
	virtual void SetIsSelected(bool bInIsSelected) override;

	UFUNCTION(BlueprintCallable)
	virtual bool GetIsSelected() const override;

	virtual void InitializeMenu(int32 Count, int32 init = 0) override;
	virtual void RefreshUI() override;
	
public:
	virtual FText GetTabTitle() const override{ return _textTabName; }
	virtual FLinearColor GetTabColor() override;

	virtual FLinearColor GetTabBaseColor() const override { return _tabBaseColor; }
	virtual FLinearColor GetTabSelectColor() const override { return _tabSelectColor; }
	virtual FLinearColor GetTabDisabledColor() const override { return _tabDisableColor; }

	void SetUseSubTab(bool set) { UseSubTab = set; }
	void SetMenuInitName(FText initName) { _textTabName = initName; }
};
