// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UI/UIControlLib/Control/GsToggleGroupEntry.h"
#include "GsTabSwitcher.h"
#include "GsUITabSwitcherButton.generated.h"


class UGsSwitcherButton;
class UTextBlock;
/**
 * 
 */
UCLASS()
class T1PROJECT_API UGsUITabSwitcherButton : public UUserWidget, public IGsToggleGroupEntry, public IGsTabSwitcher
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsSwitcherButton* _btnSelect;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GsUI")
	FText _textTabName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GsUI")
	FLinearColor _tabSelectColor;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GsUI")
	FLinearColor _tabBaseColor;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GsUI")
	FLinearColor _tabDisableColor;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GsUIControl")
	int32 _slotIndexSelected = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GsUIControl")
	int32 _slotIndexUnselected = 0;

public:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textSelect;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBase;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textDisable;

public:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

	virtual void SynchronizeProperties() override;

	UFUNCTION(BlueprintCallable)
	void OnButtonClick();

	// IGsToggleGroupEntry
	UFUNCTION(BlueprintCallable)
	virtual void SetIsSelected(bool bInIsSelected) override;

	UFUNCTION(BlueprintCallable)
	virtual bool GetIsSelected() const override;

protected:
	UFUNCTION(BlueprintCallable)
	void SetSwitcherIndex(int32 InIndex);

	UFUNCTION(BlueprintCallable)
	int32 GetSwitcherIndex() const;

public:
	virtual FText GetTabTitle() const override { return _textTabName; }
	virtual FLinearColor GetTabColor() override;

	virtual FLinearColor GetTabBaseColor() const override { return _tabBaseColor; }
	virtual FLinearColor GetTabSelectColor() const override { return _tabSelectColor; }
	virtual FLinearColor GetTabDisabledColor() const override { return _tabDisableColor; }
};
