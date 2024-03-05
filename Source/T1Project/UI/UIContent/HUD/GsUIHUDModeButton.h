// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/UILib/Base/GsUIHUD.h"
#include "UI/UILib/Define/GsUIDefine.h"
#include "GsUIHUDModeButton.generated.h"

/**
 * HUD 카메라버튼 영역
 */
UCLASS(meta = (DisableNativeTick))
class T1PROJECT_API UGsUIHUDModeButton : public UGsUIHUD
{
	GENERATED_BODY()

protected:
	// Index: EGsUIHUDMode
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UWidgetSwitcher* _switchMode;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UGsButton* _btnMode;

protected:
	// UUserWidget override
	virtual void NativeOnInitialized() override;	

public:
	virtual void SetHUDMode(EGsUIHUDMode InMode) override;

protected:
	UFUNCTION()
	void OnClickMode();
};
