// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/UIContent/Tray/GsUITrayTickerAnimation.h"
#include "GsUITrayTickerInvadeTwoText.generated.h"

/**
 * 
 */
UENUM()
enum class EGsInvadeTwoType : uint8
{
	CARRIAGE_SUVIVE,
	CARRIAGE_DESTROYED,
	VICTORY,
	DEFEATE,
};

UCLASS()
class T1PROJECT_API UGsUITrayTickerInvadeTwoText : public UGsUITrayTickerAnimation
{
	GENERATED_BODY()	

public:
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
		class UTextBlock* _contextTextBlock1;
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
		class UTextBlock* _contextTextBlock2;
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
		class UWidgetSwitcher* _iconSwitcher;
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
		class UImage* _iconImage;
	UPROPERTY(BlueprintReadWrite, Transient, meta = (BindWidgetAnim))
		class UWidgetAnimation* _inAnim;

public:
	virtual void NativeOnInitialized() override;
	void SetInvadeTwoText(const FText& inContext1, const FText& inContext2, const FSoftObjectPath& inIconPath);

public:
	virtual TrayCondition GetTrayCondition() const override { return TrayCondition::TC_Priority; }
};
