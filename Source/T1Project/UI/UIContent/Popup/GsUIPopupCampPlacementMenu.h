// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/UILib/Base/GsUIPopup.h"
#include "GsUIPopupCampPlacementMenu.generated.h"


class UGsButton;
class UGsSwitcherButton;
/**
 * 
 */
UCLASS()
class T1PROJECT_API UGsUIPopupCampPlacementMenu : public UGsUIPopup
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsSwitcherButton* _btnOk;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnCancel;

protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

protected:
	virtual void OnInputOk() override;
	virtual void OnInputCancel() override;
	
protected:
	UFUNCTION(BlueprintCallable, Category = "GsUI")
	void OnClickOk();
	UFUNCTION(BlueprintCallable, Category = "GsUI")
	void OnClickCancel();
};
