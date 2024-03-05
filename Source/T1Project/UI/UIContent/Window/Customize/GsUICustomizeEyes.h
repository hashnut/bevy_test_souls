// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/UIContent/Window/Customize/GsUICustomizeBase.h"
#include "UI/UIControlLib/Control/GsToggleGroup.h"
#include "GsUICustomizeEyes.generated.h"

class UPanelWidget;
class UGsDynamicPanelSlotHelper;

/**
 * UGsUICustomizingEyes
 * : 
 */
UCLASS()
class T1PROJECT_API UGsUICustomizeEyes : public UGsUICustomizeBase
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GsUI")
	TSubclassOf<class UUserWidget> _colorEntryWidgetClass;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UPanelWidget* _panelPupilColor;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UPanelWidget* _panelBrowColor;

	UPROPERTY()
	UGsDynamicPanelSlotHelper* _slotHelperPupilColor;

	UPROPERTY()
	UGsDynamicPanelSlotHelper* _slotHelperBrowColor;

protected:
	FGsToggleGroup _toggleGroupPupilColor;
	FGsToggleGroup _toggleGroupBrowColor;

	TArray<FColor> _pupilColorList;
	TArray<FColor> _browColorList;

public:
	// UObject override
	virtual void BeginDestroy() override;

protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeDestruct() override;

public:
	// UGsUICustomizingBase override
	virtual void Show() override;

protected:
	UFUNCTION()
	void OnRefreshPupilColor(int32 InIndex, class UWidget* InEntry);

	UFUNCTION()
	void OnRefreshBrowColor(int32 InIndex, class UWidget* InEntry);

	UFUNCTION()
	void OnSelectPupilColor(int32 InIndex);

	UFUNCTION()
	void OnSelectBrowColor(int32 InIndex);
};
