// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "T1Project/UI/UIContent/Define/GsUIContentDefine.h"
#include "UI/UIContent/Define/EGsUIColorType.h"
#include "GsUIQuickEnchantGroup.generated.h"

class UPanelWidget;
class UTextBlock;
/**
 * 
 */
UCLASS()
class T1PROJECT_API UGsUIQuickEnchantGroup : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _enchantNumText;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _enchantWarnText;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UPanelWidget* _warnTextRootPanel;

protected:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

public:
	void SetEnchantGoalNumText(const FText InText, const bool InIsUseWarningColor);
	void SetEnchantWarnText(const FText InWarnText, const bool InIsUseWarningColor);
	void SetWarnTextColor(const EGsUIColorType InColorType);
	void SetEnchantWarnTextVisibility(const bool InState);
	void SetEnchantGoalNumTextColor(const EGsUIColorType InColorType);
	
};
