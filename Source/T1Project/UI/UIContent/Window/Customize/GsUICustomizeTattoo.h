// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/UIContent/Window/Customize/GsUICustomizeBase.h"
#include "UI/UIControlLib/Control/GsToggleGroup.h"
#include "GsUICustomizeTattoo.generated.h"

class UPanelWidget;
class UScrollBox;
class UGsDynamicPanelSlotHelper;

/**
 * UGsUICustomizingTattoo
 * : 
 */
UCLASS()
class T1PROJECT_API UGsUICustomizeTattoo : public UGsUICustomizeBase
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GsUI")
	TSubclassOf<class UUserWidget> _tattooEntryWidgetClass;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UScrollBox* _scroll;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UPanelWidget* _panelTattoo;

	UPROPERTY()
	UGsDynamicPanelSlotHelper* _slotHelperTattoo;

protected:
	FGsToggleGroup _toggleGroupTattoo;
	const TArray<struct FGsSchemaCustomizeSlotData>* _tattooList;

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
	void OnRefreshTattoo(int32 InIndex, class UWidget* InEntry);

	UFUNCTION()
	void OnSelectTattoo(int32 InIndex);
};
