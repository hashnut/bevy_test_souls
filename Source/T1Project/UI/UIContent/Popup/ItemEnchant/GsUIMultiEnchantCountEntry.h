// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UI/UIControlLib/Interface/GsEntryInterface.h"
#include "GsUIMultiEnchantCountEntry.generated.h"


DECLARE_MULTICAST_DELEGATE_TwoParams(FOnClickNum, const uint8, const bool);

class UImage;
class UGsButton;
class UTextBlock;
class UWidgetSwitcher;
/**
 * 
 */
UCLASS()
class T1PROJECT_API UGsUIMultiEnchantCountEntry : public UUserWidget
	, public IGsEntryInterface
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UImage* _selectionImg;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _numText;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _numText1;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnOnClick;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidgetSwitcher* _stateSwitcher;  // 0 : block  1 : touch


protected:
	virtual void NativeOnInitialized() override;
	
public:
	virtual void OnShow() override;
	virtual void OnHide() override;

public:
	void SetData(int32 InIndex, bool InIsEnableBtn, bool InIsSelcted);

private:
	bool _isSelected = false;
	bool _stateflag = true;
	uint8 _index = 0;

public:
	FOnClickNum OnClickNum;

public:
	void SetIsSlected(bool InIsSelected) { _isSelected = InIsSelected; }

public:
	bool GetIsSelected() { return _isSelected; }
	uint8 GetCountIndex() { return _index; }
	bool GetState() { return _stateflag; }
	
protected:
	UFUNCTION()
	void OnclickNum();

};
