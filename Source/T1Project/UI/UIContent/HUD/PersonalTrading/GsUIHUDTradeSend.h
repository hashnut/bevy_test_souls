// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "UI/UILib/Base/GsUIHUD.h"

#include "Message/GsMessageContentHud.h"

#include "GsUIHUDTradeSend.generated.h"


/**
 * HUD 개인 거래 요청 대기 (개설자)
 */

class UGsButton;
class UTextBlock;
class UProgressBar;
UCLASS()
class T1PROJECT_API UGsUIHUDTradeSend : public UGsUIHUD
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnCancel;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockUserName;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UProgressBar* _progressBarSend;

protected:
	bool _isOpened;
	int32 _approveProgressTimeSec;

	MsgHudHandleArray _msgHudHandlerList;

protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

public:
	void SetTradeInfo(const FString& InUserName);

	void Show();
	void Hide();
	virtual void SetHUDMode(EGsUIHUDMode InMode) override;

protected:
	UFUNCTION()
	void OnClickCancel();

	void InvalidateUI();
};
