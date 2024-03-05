// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/UILib/Base/GsUITray.h"
#include "GsUITrayLongPressBar.generated.h"


DECLARE_MULTICAST_DELEGATE_OneParam(FOnFinishedLongPress , bool);

class UProgressBar;
/**
 * ui LongPress class
 */
UCLASS()
class T1PROJECT_API UGsUITrayLongPressBar : public UGsUITray
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget , DisplayName = "maximum 1초 minimum 0초"))
	UProgressBar* _progressBar;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GsUI")
	float _longPressTotalDurationTime = 1.f;

public:
	FOnFinishedLongPress _onFinishedLongPress;

private:	
	bool _isFullGauge = false;
	float _time = 0.f;

	bool _bReserveShow = false;
	FVector2D _absTargetPos;

public:
	void CloseUI();

private:
	void OnFinished();

protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

public:
	// AddToViewport 이전에 세팅할 것
	void SetTargetPosition(const FVector2D& InAbsPos);
};
