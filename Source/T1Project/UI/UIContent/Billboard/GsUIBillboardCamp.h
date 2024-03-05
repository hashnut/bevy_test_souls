// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/UILib/Base/GsUIBillboard.h"
#include "GsUIBillboardCamp.generated.h"

class UImage;
class UTextBlock;
class UProgressBar;
class UGsUIGuildIcon;
class UPanelWidget;
/**
 * 
 */
UCLASS()
class T1PROJECT_API UGsUIBillboardCamp : public UGsUIBillboard
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _campNameText;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UProgressBar* _hpProgressBar;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUIGuildIcon* _guildIcon;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockSanctum;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UPanelWidget* _panelRootName;

protected:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

public:
	void SetProgressBarValue(const float InValue);
	void SetCampNameText(FText InText);
	void SetCampEmblemIcon(const uint8 InEmblemId);
	void ShowHPBar(bool In_isShow);

	void SetSanctumNameText(const FText& InText);
	void ShowNamePanel(bool InShow);

public:
	float GetCurProgressBarValue();
	
};
