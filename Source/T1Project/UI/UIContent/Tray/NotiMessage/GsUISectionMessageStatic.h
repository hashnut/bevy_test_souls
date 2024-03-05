// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/UIContent/Tray/NotiMessage/GsUISectionMessageBase.h"
#include "GsUISectionMessageStatic.generated.h"

using FGsTaskTrayAct = UGsUISectionMessageBase::FGsTaskTrayAct;
class UWidgetAnimation;
/**
 * fadeIn -> output message -> fadeOut  
 * fixSize. MultiLine EditBox output
 */
UCLASS()
class T1PROJECT_API UGsUISectionMessageStatic : public UGsUISectionMessageBase
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", Meta = (BindWidgetAnim, AllowPrivateAccess = true), Transient)
	UWidgetAnimation* _animationFade;

	FLinearColor _backBGColor;

protected:
	virtual void OnInitWork(TWeakPtr<FGsTickerMessageDataEx> data) override;

public:
	UFUNCTION(BlueprintImplementableEvent, Category = "GsUI")
	void StartAnimation(EGsTickerSectionAnimationType InAnimationType);

protected:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

protected:
	FLinearColor GetBackColor(NoticeType type);
};
