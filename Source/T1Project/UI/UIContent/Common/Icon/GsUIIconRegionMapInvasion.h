// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/UIContent/Common/Icon/GsUIIconDefault.h"
#include "GsUIIconRegionMapInvasion.generated.h"

class UTextBlock;
class UWidgetSwitcher;
class UGsButton;
/**
 * 
 */
UCLASS()
class T1PROJECT_API UGsUIIconRegionMapInvasion : public UGsUIIconDefault
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textTime;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidgetSwitcher* _selectActive;
	// 침공전 포탈로 워프 탈수 있게 요청
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnIcon;

private:
	TFunction<void()> _callbackClick;

protected:
	virtual void NativeOnInitialized() override;

public:
	virtual void SetIconImageColor(const FLinearColor& InColor) override;
	virtual void SetTimeText(FText inTextTime);

	virtual void SetActiveSelect(int set = 1);

public:
	UFUNCTION()
	void OnClickButton();

public:
	void SetClickCallback(TFunction<void()> In_callback)
	{
		_callbackClick = In_callback;
	}
};

