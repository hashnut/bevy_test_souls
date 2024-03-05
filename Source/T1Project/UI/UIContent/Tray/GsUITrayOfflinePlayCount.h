// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/UILib/Base/GsUITray.h"
#include "UI/UILib/Define/GsUIDefine.h"
#include "Message/GsMessageGameObject.h"
#include "GsUITrayOfflinePlayCount.generated.h"


class UImage;
class UGsButton;
class UTextBlock;

/**
 * 
 */
UCLASS()
class T1PROJECT_API UGsUITrayOfflinePlayCount : public UGsUITray
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUIIcon", meta = (BindWidget))
	UTextBlock* _textCount;

	UPROPERTY(BlueprintReadOnly, Category = "GsUIIcon", meta = (BindWidget))
	UGsButton* _buttonCancel;

	UPROPERTY(BlueprintReadOnly, Category = "GsUIIcon", meta = (BindWidget))
	UImage* _blockOutImag;

	UPROPERTY(BlueprintReadOnly, Category = "GsUIIcon", meta = (BindWidget))
	UTextBlock* _noticeExit;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BlackOutAlphaValue")
	float _begineAlphaValue = 0.0f;

	float _elapsedTime = 0.f;

	int32 _delaySecounds = 0;

	FTimerHandle _Timerhandler;

	EGsUIHUDMode _currentHUDMode;

	MsgGameObjHandle _autoStateDelegate;

protected:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

protected:
	void Reset();
	void CancelOfflinePlay();
	void SetTextCount(int32 InDelaySeconds);

protected:
	// 2023/8/17 PKT - AI Status Delegate
	void OnChangedAutoState(const struct IGsMessageParam* InParam = nullptr);

	UFUNCTION()
	void OnCount();

	UFUNCTION()
	void OnClickedScreen();

	void HideHUD();
	void ShowHUD();

public:
	void SetData(int32 InDelaySeconds);
	void UIBlockAndNotice();
};
