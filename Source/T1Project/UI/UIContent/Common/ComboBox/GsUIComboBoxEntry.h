// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UI/UIControlLib/Control/GsToggleGroupEntry.h"
#include "UI/UIControlLib/Interface/GsEntryInterface.h"
#include "GsUIComboBoxEntry.generated.h"

/**
 * ÄÞº¸¹Ú½º ¸®½ºÆ®ÀÇ ½½·Ô
 */
UCLASS()
class T1PROJECT_API UGsUIComboBoxEntry : public UUserWidget,
	public IGsToggleGroupEntry,
	public IGsEntryInterface
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UGsButton* _btnSlot;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UImage* _imgSelected;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UTextBlock* _textBlockName;

protected:
	bool _bIsSelected = false;

protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeDestruct() override;

public:
	// IGsToggleGroupEntry override
	virtual void SetIsSelected(bool bInIsSelected) override;
	virtual bool GetIsSelected() const override;

public:
	void SetNameText(const FText& InText);

protected:
	UFUNCTION()
	void OnClickSlot();
};
