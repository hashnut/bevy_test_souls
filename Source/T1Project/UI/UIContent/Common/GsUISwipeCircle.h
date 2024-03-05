// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UI/UIControlLib/Interface/GsEntryInterface.h"
#include "GsUISwipeCircle.generated.h"

class UWidgetSwitcher;
/**
 * 
 */
UCLASS()
class T1PROJECT_API UGsUISwipeCircle : public UUserWidget
	, public IGsEntryInterface
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidgetSwitcher* _switcher; // 0 on , 1 off

	
public:
	virtual void OnShow() override;
	virtual void OnHide() override;

public:
	void SetCircleActive(const bool InIsActive);

};
