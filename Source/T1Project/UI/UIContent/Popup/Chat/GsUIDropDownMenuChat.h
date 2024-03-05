// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/UIContent/Common/Menu/GsUIDropDownMenuCommon.h"
#include "UI/UIContent/Common/Menu/GsUIDropDownMenuEntry.h"
#include "GsUIDropDownMenuChat.generated.h"

class UWidgetSwitcher;
/**
 * 
 */
UCLASS()
class T1PROJECT_API UGsUIDropDownMenuChat : public UGsUIDropDownMenuCommon
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidgetSwitcher* _mainBtnSwitcher;

public:
	void SetMenuPanelActive(bool set);
};
