// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/UIContent/Window/Customize/GsUICustomizeBase.h"
#include "UI/UIControlLib/Control/GsToggleGroup.h"
#include "GsUICustomizeHair.generated.h"

class UPanelWidget;
class UGsDynamicPanelSlotHelper;

/**
 * UGsUICustomizingHair
 * : 
 */
UCLASS()
class T1PROJECT_API UGsUICustomizeHair : public UGsUICustomizeBase
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GsUI")
	TSubclassOf<class UUserWidget> _hairEntryWidgetClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GsUI")
	TSubclassOf<class UUserWidget> _colorEntryWidgetClass;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UPanelWidget* _panelHair;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UPanelWidget* _panelHairColor;

	UPROPERTY()
	UGsDynamicPanelSlotHelper* _slotHelperHair;

	UPROPERTY()
	UGsDynamicPanelSlotHelper* _slotHelperHairColor;

protected:
	FGsToggleGroup _toggleGroupHair;
	FGsToggleGroup _toggleGroupHairColor;

	const TArray<struct FGsSchemaCustomizeSlotData>* _hairList;
	TArray<FColor> _hairColorList;

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
	void OnRefreshHair(int32 InIndex, class UWidget* InEntry);

	UFUNCTION()
	void OnRefreshHairColor(int32 InIndex, class UWidget* InEntry);

	UFUNCTION()
	void OnSelectHair(int32 InIndex);

	UFUNCTION()
	void OnSelectHairColor(int32 InIndex);
};
