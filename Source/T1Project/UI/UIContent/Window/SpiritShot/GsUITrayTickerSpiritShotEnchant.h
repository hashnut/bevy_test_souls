// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/UIContent/Tray/GsUITrayTickerAnimation.h"
#include "GsUITrayTickerSpiritShotEnchant.generated.h"

class UTextBlock;

/**
 * 
 */
UCLASS()
class T1PROJECT_API UGsUITrayTickerSpiritShotEnchant : public UGsUITrayTickerAnimation
{
	GENERATED_BODY()

protected:
	/** 강화 결과 트레이에 띄워줄 텍스트*/
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _trayTickerText;

	UPROPERTY(BlueprintReadWrite, Transient, meta = (BindWidgetAnim))
	class UWidgetAnimation* _inAnim;

public:
	virtual void NativeOnInitialized() override;
	void SetText(FText&& InText);

	virtual TrayCondition GetTrayCondition() const override { return TrayCondition::TC_Priority; }
};
