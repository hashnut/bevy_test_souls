// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GsUIVFX.generated.h"

/**
 * UIParticle위젯 사용 편의를 위해 만들었음
 */
UCLASS()
class T1PROJECT_API UGsUIVFX : public UUserWidget
{
	GENERATED_BODY()

public:
	DECLARE_DELEGATE(FGsOnVFXAnimationFinished);

public:
	// 시작 즉시 자동시작
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GsUI")
	bool _bAutoStart = false;

	// 애니메이션 재생에 필요한 변수들
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "GsUIAnimation")
	float _startAtTime = 0.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "GsUIAnimation")
	int32 _numLoopsToPlay = 1; // 0: 루핑
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "GsUIAnimation")
	uint8 _playMode = 0; // EUMGSequencePlayMode::Forward;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "GsUIAnimation")
	float _playbackSpeed = 1.0f;

private:
	bool _bDynamicCreate = false;
	bool _bReserveShow = false;
	bool _bAdjustToCenter = false;
	FVector2D _dynamicTargetPos;

protected:
	TArray<class UWidgetAnimation*> _animationList;

public:
	FGsOnVFXAnimationFinished OnVFXAnimationFinished;

public:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime);

protected:
	virtual void OnAnimationFinished_Implementation(const UWidgetAnimation* Animation) override;

protected:
	void InitAnimationList();

public:
	float GetAnimEndTime();

protected:
	UFUNCTION(BlueprintNativeEvent, Category = "GsUI", meta = (DisplayName = "OnShowVFX"))
	void OnShowVFXEvent();
	virtual void OnShowVFXEvent_Implementation();
	
	UFUNCTION(BlueprintNativeEvent, Category = "GsUI", meta = (DisplayName = "OnHideVFX"))
	void OnHideVFXEvent();
	virtual void OnHideVFXEvent_Implementation();

public:
	UFUNCTION(BlueprintCallable, Category = "GsUI", meta = (DisplayName = "ShowVFX"))
	void ShowVFX();

	UFUNCTION(BlueprintCallable, Category = "GsUI", meta = (DisplayName = "HideVFX"))
	void HideVFX();

	void SetDynamicCreate(bool InDynamic, const FVector2D& InPos, bool InAdjustToCenter);
};
