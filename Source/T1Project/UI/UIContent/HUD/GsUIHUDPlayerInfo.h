// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/UILib/Base/GsUIHUD.h"
#include "Message/GsMessageContentHud.h"
#include "Message/GsMessageGameObject.h"
#include "GsUIHUDPlayerInfo.generated.h"

class UImage;
class UTextBlock;
class UPanelWidget;
class UGsButton;
class UGsUIRedDotBase;
class UProgressBar;
class FGsStatInfo;
struct IGsMessageParam;

/**
 * HUD 하단 유저정보
 */
UCLASS(meta = (DisableNativeTick))
class T1PROJECT_API UGsUIHUDPlayerInfo : public UGsUIHUD
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "GsUI")
	float _hpBarSpeed = 0.2f;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UPanelWidget* _panelRootBar;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UProgressBar* _barHp;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UProgressBar* _barMp;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UProgressBar* _barShieldFront;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UProgressBar* _barShieldBack;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _hpValueText;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _mpValueText;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _shieldValueText;

	// 레벨 표시
	UPROPERTY(BlueprintReadOnly, Category = "GsUI")
	FText _textLevel;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _detailInfoBtn;

	//red dot
protected:
	//stat distribution 
	UPROPERTY(BlueprintReadOnly, Category = "Reddot", meta = (BindWidget))
	UGsUIRedDotBase* _statDistributionRedDot;

protected:
	// 실제 입력받은 값
	float _targetHpRatio = 1.f;
	float _hpBarAnimRemainTime = 0.f;
	FText _textValueFormat;
	float _delayInvalidateTime = 0.f;

	int32 _currHp = 0;
	int32 _maxHp = 0;
	int32 _currShield = 0;

private:
	MsgHUDRedDotHandleArray _redDotMessageHandlerArray;
	MsgGameObjHandleArray _msgHandlerGameObjectList;

protected:
	// UUserWidget override
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	//virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;	
	
	// IGsInvalidateUIInterface override
	virtual void InvalidateAllInternal() override;

public:
	virtual bool UpdateManagerTick(float InDeltaTime) override;

private:
	void BindMessages();
	void UnbindMessages();

public:
	void OnStatDistributionRedDot(bool inActive);
	
	void OnAddAbnormality(const IGsMessageParam* InParam);
	void OnRemoveAbnormality(const IGsMessageParam* InParam);

private:
	void InvalidStatDistributionRedDot();

public:
	void SetStat(const FGsStatInfo& InStatInfo);

protected:
	void SetHP(int32 InCurrent, int32 InMax, bool bInUpdateBar);
	void SetMP(int32 InCurrent, int32 InMax);
	void SetShield(int32 InCurrent, bool bInUpdateBar);
	void UpdateHpShieldBar();

	// 상태이상 컨퓨즈(내 HP/MP 정보 가림)
	void SetAbnormalityConfuse(bool bIsConfuse);

	// 절전 모드용으로 설정
public:
	void SetSaveBatteryMode();

public:
	UFUNCTION()
	void OnClickPlayerDetailInfo();
};
