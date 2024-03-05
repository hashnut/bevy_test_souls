// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GsUISkillWeaponTab.generated.h"


class UImage;
class UTextBlock;
class UGsSwitcherButton;
class UGsUIRedDotBase;

/**
 * 스킬창의 무기탭
 */
UCLASS()
class UGsUISkillWeaponTab : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "GsUIWeaponTab")
	FText _textName;	
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "GsUIWeaponTab")
	FSlateBrush _brushIconOff;
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "GsUIWeaponTab")
	FSlateBrush _brushIconOn;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsSwitcherButton* _sBtnSlot;
	
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UImage* _imgIconOff;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UImage* _imgIconOn;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockOff;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockOn;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UImage* _imgEquipMark;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUIRedDotBase* _uiRedDot;

public:
	virtual void SynchronizeProperties() override;

public:
	void SetEquipMark(bool bIsVisible);

public:
	UGsSwitcherButton* GetSwitcherButton() const { return _sBtnSlot; }
	UGsUIRedDotBase* GetRedDot() const { return _uiRedDot; }
};
