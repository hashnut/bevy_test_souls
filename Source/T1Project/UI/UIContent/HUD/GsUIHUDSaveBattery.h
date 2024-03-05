// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "UI/UILib/Base/GsUIHUD.h"

#include "Message/GsMessageContents.h"

#include "GsUIHUDSaveBattery.generated.h"


class UGsSwitcherButton;
/**
 * HUD 좌하단 절전모드 버튼
 */
UCLASS(meta = (DisableNativeTick))
class T1PROJECT_API UGsUIHUDSaveBattery : public UGsUIHUD
{
	GENERATED_BODY()

public:
	using MsgHandleUnlock = TPair<MessageContentUnlock, FDelegateHandle>;

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsSwitcherButton* _btnSaveBattery;

	TArray<MsgHandleUnlock> _unlockDelegateList;

public:
	virtual void BeginDestroy() override;

protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

public:
	UFUNCTION()
	void OnClickSaveBattery();

protected:
	void OnContentsUnlock(const struct IGsMessageParam* InParam);
};
