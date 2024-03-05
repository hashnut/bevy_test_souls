// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/UILib/Base/GsUIHUD.h"
#include "GenericPlatform/GenericPlatformMisc.h"
#include "GsUIHUDSystemInfo.generated.h"

/**
 * HUD화면 하단 경험치바, 와이파이, 배터리, 시간
 */
UCLASS(meta = (DisableNativeTick))
class T1PROJECT_API UGsUIHUDSystemInfo : public UGsUIHUD
{
	GENERATED_BODY()

protected:
	// 레벨 표시
	UPROPERTY(BlueprintReadOnly, Category = "GsUI")
	FText _textLevel;

	// 경험치 표시
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UProgressBar* _barExp;
	
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UTextBlock* _textBlockExp;

	// 시간표시. 위젯 블루프린트에서 변수 바인딩.
	UPROPERTY(BlueprintReadOnly, Category = "GsUI")
	FText _textTime;

	// 배터리 바
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UProgressBar* _barBattery;

	// 배터리 아이콘
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UImage* _imgBattery;

	// 충전중 아이콘
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UImage* _imgBatteryCharge;

	// lowBattery일때 출력할 컬러
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "GsUI", meta = (BindWidget))
	FLinearColor _colorLowBattery;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UWidgetSwitcher* _switcherNetwork;

	const float _invalidateInterval = 2.0f; // 2 초마다 갱신(시간, 배터리 등)
	const int _lowBatteryMax = 25; // 25 미만 다른 색으로 표시

	float _tempInterval = 0.0f;

protected:
	// UUserWidget override.
	virtual void NativeConstruct() override;

	// IGsInvalidateUIInterface override.
	virtual void InvalidateAllInternal() override;  // 정보받으면 갱신항목

public:
	virtual bool UpdateManagerTick(float InDeltaTime) override;

public:
	// 정보를 받을때 마다 갱신
	void InvalidateExp();

protected:
	// 주기적으로 갱신
	void InvalidateSystemInfo();

	// 테스트 편의를 위해 블루프린트 접근 허용
	UFUNCTION(BlueprintCallable, Category = "GsUI")
	void SetExpUI(float InRate);

	UFUNCTION(BlueprintCallable, Category = "GsUI")
	void SetBatteryUI(int InBatteryLevel);

	UFUNCTION(BlueprintCallable, Category = "GsUI")
	void SetBatteryChargeState(bool InBatteryCharge);

	UFUNCTION(BlueprintCallable, Category = "GsUI")
	void SetNetworkTypeUI(uint8 InType);

	void SetNetworkTypeUIInternal(ENetworkConnectionType InNetworkType);

	// 배터리 레벨. 0 ~ 100 값을 반환. PC환경에서는 무조건 100을 반환. 
	int GetBatteryLevel() const;

	// 네트워크 타입. PC환경에서는 무조건 WIFI로 반환.
	ENetworkConnectionType GetNetworkConnectionType() const;
};