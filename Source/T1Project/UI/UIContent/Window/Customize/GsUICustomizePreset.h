// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/UIContent/Window/Customize/GsUICustomizeBase.h"
#include "UI/UIContent/Window/Customize/GsCustomizeUIData.h"
#include "UI/UIControlLib/Control/GsToggleGroup.h"
#include "GsUICustomizePreset.generated.h"

class UPanelWidget;
class UGsButton;
class UGsDynamicPanelSlotHelper;

/**
 * UGsUICustomizingPreset
 * : Ä¿½ºÅÍ¸¶ÀÌÂ¡ Preset ÅÇ
 */
UCLASS()
class T1PROJECT_API UGsUICustomizePreset : public UGsUICustomizeBase
{
	GENERATED_BODY()

public:
	DECLARE_DELEGATE_OneParam(FOnCustomizeSetPreset, int32 /*InIndex*/);
	DECLARE_DELEGATE(FOnCustomizeRandomPreset);

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GsUI")
	TSubclassOf<class UUserWidget> _presetEntryWidgetClass;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UPanelWidget* _panelPreset;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnRandom;

protected:
	UPROPERTY()
	UGsDynamicPanelSlotHelper* _slotHelperPreset;

protected:
	FGsToggleGroup _toggleGroupPreset;

public:
	FOnCustomizeSetPreset OnSetPreset;
	FOnCustomizeRandomPreset OnClickRandom;

public:
	// UObject override
	virtual void BeginDestroy() override;

protected:
	// UUserWidget override
	virtual void NativeOnInitialized() override;

public:
	// UGsUICustomizingBase override
	virtual void Show() override;

protected:
	UFUNCTION()
	void OnRefreshEntryPreset(int32 InIndex, class UWidget* InEntry);

	UFUNCTION()
	void OnClickPreset(int32 InIndex, bool bIsSelected);

	UFUNCTION()
	void OnClickRandomButton();

protected:
	void SetPreset(int32 InIndex);
};
