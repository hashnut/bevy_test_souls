// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GsUITargetInfoHUDBase.generated.h"

class UTextBlock;
class UProgressBar;
class UPanelWidget;
class UNamedSlot;
class UGsButton;
class UGsUIAbnormalityIconList;
class UGsGameObjectBase;
class UGsGameObjectCreature;

/**
 * HUD 상단 타겟의 HP바, 정보
 */
UCLASS(Abstract)
class T1PROJECT_API UGsUITargetInfoHUDBase : public UUserWidget
{
	GENERATED_BODY()

public:
	DECLARE_DELEGATE_OneParam(FOnClickTooltipButton, class UNamedSlot*);

protected:
	// 잔상용 bar의 속도
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "GsHpBar")
	float _shadowBarSpeed = 0.1f;

	// 유저 정보
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockName;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnTooltip;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UNamedSlot* _slotTooltip;	
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnClearTarget;

	// HP정보
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockHpValue;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockShieldValue;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UPanelWidget* _panelHpBarRoot;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UProgressBar* _barHp;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UProgressBar* _barShadow;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UProgressBar* _barShieldFront;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UProgressBar* _barShieldBack;	

	// 상태이상
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUIAbnormalityIconList* _abnormalityIconList;

	//살아있을 수 있는 시간
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textLifeTime;

protected:
	bool _isShowHPBar = false;
	
	int32 _currHp = 0;
	int32 _maxHp = 0;
	int32 _currShield = 0;

	float _targetHpRatio = 1.f;
	float _hpBarAnimRemainTime = 0.f;	
	float _delayInvalidateTime = 0.f;

	time_t _endLifeTime = 0;

public:
	FOnClickTooltipButton OnClickTooltipButton;

protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

public:
	virtual void Reset();
	virtual void SetTarget(UGsGameObjectCreature* InTarget, const FString& InName);

	virtual void ShowAggroTarget(bool InIsBoss) {}

public:	
	void SetRemainLifeTime(time_t endTimeTick);	

	void ShowHPBar(bool In_isShow);
	void SetHP(int32 InCurrent, int32 InMax, bool bInUpdateBar);
	void SetShield(int32 InCurrent, bool bInUpdateBar);	
	void UpdateHpShieldBar();
	
protected:
	void UpdateLifeTime();
	
protected:
	UFUNCTION()
	void OnClickTooltip();
	UFUNCTION()
	void OnClickClearTarget();
};
