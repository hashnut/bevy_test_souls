// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/UILib/Base/GsUIHUD.h"
#include "Classes/GsPool.h"
#include "GsUIHUDGainEffect.generated.h"

class UGsUIGainEffectSlot;

/**
 * EXP, Gold 획득 연출
 */
UCLASS(meta = (DisableNativeTick))
class T1PROJECT_API UGsUIHUDGainEffect : public UGsUIHUD
{
	GENERATED_BODY()

public:
	struct FGsGainEffectDelayData
	{
	public:
		uint64 _value = 0;
		float _remainSec = 0.f;
	};

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUIGainEffectSlot* _slot0;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUIGainEffectSlot* _slot1;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUIGainEffectSlot* _slot2;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUIGainEffectSlot* _slot3;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUIGainEffectSlot* _slot4;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "GsUIGainEffect")
	float _upHeight = -30.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "GsUIGainEffect")
	float _randomPosX = 200.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "GsUIGainEffect")
	float _firstSlotScale = 1.f;

protected:
	TGsPoolDefault<FGsGainEffectDelayData> _poolDelayData;

	TArray<FGsGainEffectDelayData*> _delayList;
	TArray<FGsGainEffectDelayData*> _removeDelayList;

protected:
	TArray<TWeakObjectPtr<UGsUIGainEffectSlot>> _activeList; // 순서중요. Queue 처럼 동작하도록 처리
	TArray<TWeakObjectPtr<UGsUIGainEffectSlot>> _inactiveList;
	TArray<TWeakObjectPtr<UGsUIGainEffectSlot>> _removeList;

	FNumberFormattingOptions _textFormatOptionExp;

	FDelegateHandle _tickHandle;

public:
	void BeginDestroy() override;

protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

protected:
	virtual void SetHUDMode(EGsUIHUDMode InMode) override;

protected:
	void UpdateTick(float InDeltaTime);
	void ResetSlots();

public:	
	void SetExp(uint64 InValue);
	void SetGold(uint64 InValue);
	void SetMonsterBookPoint(uint64 InValue);

protected:
	void SetValue(uint64 InValue, bool bUseGrouping);
};
