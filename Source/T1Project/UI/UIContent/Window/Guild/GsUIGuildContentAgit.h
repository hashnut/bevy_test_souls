// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/UIContent/Window/Guild/GsUIGuildContentBase.h"
#include "UTIL/GsIntervalChecker.h"
#include "GsUIGuildContentAgit.generated.h"

class UTextBlock;
class UPanelWidget;
class UGsUICurrencyButton;

/**
 * 기사단 창의 영지 탭 페이지
 */
UCLASS()
class UGsUIGuildContentAgit : public UGsUIGuildContentBase
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockRentLimit;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockRemainTime;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUICurrencyButton* _currencyBtnRental;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUICurrencyButton* _currencyBtnEnter;

protected:
	time_t _rentalEndDate = 0;
	float _checkRemainSec = 0;
	FGsIntervalChecker _checkRent;

public:
	virtual FString GetTabName() const override { return TEXT("Agit"); }

protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeDestruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

protected:
	virtual void InvalidateAllInternal() override;

protected:
	//virtual void SendRequestPacket() override;
	virtual void Show(bool bIsMyGuild, bool IsStackRecoverFlag = false) override;

public:
	virtual void OnNotifyChangeMemberGrade(const struct FGsGuildMsgParamMemberGrade* InParam) override;
	virtual void OnChangeGuildLevel() override;

public:
	void OnAgitStateChanged();

public:
	void UpdateRentCheckTime();

protected:
	void InvalidateRemainTime(bool bCheckZero);

protected:
	void OnClickBtnRental();
	void OnClickBtnEnter();

	// 연장 가능한 시간인지를 체크함
	bool CanExtensionDate(const time_t InRentalEndDate, int32 InRentalExtensionMinD);
};
