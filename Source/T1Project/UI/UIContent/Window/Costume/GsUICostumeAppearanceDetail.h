// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Costume/GsCostumeData.h"
#include "GsUICostumeAppearanceDetail.generated.h"

class UGsSwitcherButton;
class UPanelWidget;
class UWidgetSwitcher;
class UImage;
class UWidget;
class UTextBlock;
/**
 * 
 */
UCLASS()
class T1PROJECT_API UGsUICostumeAppearanceDetail : public UUserWidget
{
	GENERATED_BODY()

public:
	DECLARE_DELEGATE_OneParam(FOnApplyFix, bool);

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UPanelWidget* _costumeInfo;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UPanelWidget* _fixPanel;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsSwitcherButton* _btnNoApplyFix;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidgetSwitcher* _gradeSwitcher;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UImage* _portraitImg;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidget* _bgNotCollected;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI")
	FText _title;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI")
	UTextBlock* _costumeName;

public:
	FOnApplyFix OnNoApplyedFix;

private:
	CostumeParts _panelType{ CostumeParts::NONE };
	TWeakPtr<FGsCostumeData> _data;

protected:
	virtual void NativeOnInitialized();
	virtual void NativeConstruct();

	UFUNCTION(BlueprintCallable, Category = "GsUI")
	void OnClickNoAppliedOn();

public:
	void SetPanelInfo(CostumeParts parts);
	void SetData(TWeakPtr<FGsCostumeData> data);
	void RefreshUI();

	bool GetCostumePartsFixed();

};
