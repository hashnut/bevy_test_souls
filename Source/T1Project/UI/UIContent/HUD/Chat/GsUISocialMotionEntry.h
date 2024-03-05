// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/UIContent/Common/Icon/GsUIIconDefault.h"
#include "GsUISocialMotionEntry.generated.h"


class UGsButton;
class UGsUICoolTimeEffectProgressBar;
struct FSocialEmotionMotion;

DECLARE_DELEGATE_OneParam(FOnClickMotionItem, TWeakPtr<FSocialEmotionMotion>);

/**
 * 
 */
UCLASS()
class T1PROJECT_API UGsUISocialMotionEntry : public UGsUIIconDefault
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnMotion;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUICoolTimeEffectProgressBar* _coolTimeBar;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidget* _lock;

protected:
	TWeakPtr<FSocialEmotionMotion> _motionData;

private:
	float _coolSec{ 0.f };
	float _coolTimeEndSec{ 0.f };

public:
	FOnClickMotionItem OnClickSlot;

protected:
	UFUNCTION(BlueprintCallable, Category = "GsUI")
	void OnSelected();

	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
public:
	void SetMotionData(TWeakPtr<FSocialEmotionMotion> data);
	void RefreshUI();
};
