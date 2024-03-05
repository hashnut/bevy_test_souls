// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UI/UIControlLib/Interface/GsEntryInterface.h"
#include "UI/UIControlLib/Control/GsToggleGroupEntry.h"
#include "GsUICheckBoxCommon.generated.h"

class UGsSwitcherButton;
class UTextBlock;

/**
 * WBP_CheckBoxCommon
 */
UCLASS()
class T1PROJECT_API UGsUICheckBoxCommon : 
	public UUserWidget, 
	public IGsEntryInterface, 
	public IGsToggleGroupEntry
{
	GENERATED_BODY()

public:
	DECLARE_DELEGATE_TwoParams(FOnClickCheckBox, bool /*bIsChecked*/, int32 /*InIndexInGroup*/);

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsSwitcherButton* _sBtnSlot;

public: 
	FOnClickCheckBox _onClickSlot;

public:
	virtual void BeginDestroy() override;

protected:
	virtual void NativeOnInitialized() override;
	
public:
	virtual void SetIsSelected(bool bInIsSelected) override;
	virtual bool GetIsSelected() const override;
	
public:
	FOnClickCheckBox& OnClickCheckBox() { return _onClickSlot; }
	
protected:
	UFUNCTION()
	void OnClickSlot();
};
