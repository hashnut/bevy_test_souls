// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/UIContent/Window/Customize/GsUICustomizeBase.h"
#include "UI/UIContent/Window/Customize/GsCustomizeUIData.h"
#include "UI/UIControlLib/Control/GsToggleGroup.h"
#include "GsUICustomizeFace.generated.h"

class UPanelWidget;
class UGsButton;
class UGsDynamicPanelSlotHelper;

/**
 * UGsUICustomizingFace
 * : Ä¿½ºÅÍ¸¶ÀÌÂ¡ FaceÅÇ
 */
UCLASS()
class T1PROJECT_API UGsUICustomizeFace : public UGsUICustomizeBase
{
	GENERATED_BODY()

public:
	DECLARE_DELEGATE(FOnClickSlot)

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GsUI")
	TSubclassOf<class UUserWidget> _faceEntryWidgetClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GsUI")
	TSubclassOf<class UUserWidget> _colorEntryWidgetClass;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UPanelWidget* _panelFace;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UPanelWidget* _panelSkinColor;

	UPROPERTY()
	UGsDynamicPanelSlotHelper* _slotHelperFace;
	
	UPROPERTY()
	UGsDynamicPanelSlotHelper* _slotHelperSkinColor;

public:
	FOnClickSlot OnClickSlot;

protected:
	FGsToggleGroup _toggleGroupFace;
	FGsToggleGroup _toggleGroupSkinColor;

	const TArray<struct FGsSchemaCustomizeSlotData>* _faceList;
	TArray<FColor> _skinColorList;

public:
	// UObject override
	virtual void BeginDestroy() override;

protected:
	// UUserWidget override
	virtual void NativeOnInitialized() override;
	virtual void NativeDestruct() override;

public:
	// UGsUICustomizingBase override
	virtual void Show() override;

protected:
	UFUNCTION()
	void OnRefreshFace(int32 InIndex, UWidget* InEntry);
	UFUNCTION()
	void OnRefreshSkinColor(int32 InIndex, UWidget* InEntry);

	UFUNCTION()
	void OnSelectFace(int32 InIndex);
	UFUNCTION()
	void OnSelectSkinColor(int32 InIndex);

	UFUNCTION()
	void OnClickSlotFace(int32 InIndex, bool bIsSelected);
	UFUNCTION()
	void OnClickSlotSkinColor(int32 InIndex, bool bIsSelected);
};
