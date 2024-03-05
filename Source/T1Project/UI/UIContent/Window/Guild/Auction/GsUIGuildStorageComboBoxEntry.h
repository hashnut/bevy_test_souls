// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UI/UIControlLib/Control/GsToggleGroupEntry.h"
#include "UI/UIControlLib/Interface/GsEntryInterface.h"
#include "GsUIGuildStorageComboBoxEntry.generated.h"

/**
 * 기사단 정산 콤보박스 리스트의 슬롯
 */
UCLASS()
class T1PROJECT_API UGsUIGuildStorageComboBoxEntry : public UUserWidget,
	public IGsToggleGroupEntry,
	public IGsEntryInterface
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UGsSwitcherButton* _btnSlot;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UTextBlock* _textBlockNormal;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UTextBlock* _textBlockSelect;

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
