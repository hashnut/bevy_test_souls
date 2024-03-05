// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/UILib/Base/GsUITray.h"
#include "GsUITrayRepairWait.generated.h"

class UGsButton;
/**
 * 장비 복구 대기 Tray UI
 */
UCLASS()
class T1PROJECT_API UGsUITrayRepairWait : public UGsUITray
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnConfirm;

private:
	TFunction<void()> _confirmCallback;
	float _currentTime = 0.f;
	float _lockMaxTime = 0.f;

protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeDestruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

private:
	bool IsPossibleOnClick() const;

public:
	void SetData(float InBtnLockTime, TFunction<void()> InConfirmCallback);

private:
	UFUNCTION()
	void OnClickConfirm();

};
