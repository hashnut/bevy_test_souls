// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/UIContent/HUD/MainMenu/GsUIMainMenuBtnBase.h"
#include "GsUIMainMenuBtnSimple.generated.h"

class UImage;
class UTextBlock;
class UPanelWidget;
class UGsSwitcherButton;

/**
 * HUD 메뉴버튼. New, 레드닷 없음
 */
UCLASS(meta = (DisableNativeTick))
class T1PROJECT_API UGsUIMainMenuBtnSimple : public UGsUIMainMenuBtnBase
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "GsUIMainMenu")
	bool _bUseName = true;
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "GsUIMainMenu")
	FText _textName;
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "GsUIMainMenu")
	FSlateBrush _brushIcon;	

	UPROPERTY(BlueprintReadOnly, Category = "GsUIMainMenu", meta = (BindWidget))
	UImage* _imgIconOn;
	UPROPERTY(BlueprintReadOnly, Category = "GsUIMainMenu", meta = (BindWidget))
	UImage* _imgIconOff;	

	UPROPERTY(BlueprintReadOnly, Category = "GsUIMainMenu", meta = (BindWidget))
	UPanelWidget* _panelRootNameOn;
	UPROPERTY(BlueprintReadOnly, Category = "GsUIMainMenu", meta = (BindWidget))
	UPanelWidget* _panelRootNameOff;

	UPROPERTY(BlueprintReadOnly, Category = "GsUIMainMenu", meta = (BindWidget))
	UTextBlock* _textBlockNameOn;
	UPROPERTY(BlueprintReadOnly, Category = "GsUIMainMenu", meta = (BindWidget))
	UTextBlock* _textBlockNameOff;	

public:
	virtual void SynchronizeProperties() override;

private:
	void SetTextBlockName(OUT UPanelWidget* OutRoot, UTextBlock* OutTextBlock);
	void SetImageIcon(OUT UImage* OutImage);
};
