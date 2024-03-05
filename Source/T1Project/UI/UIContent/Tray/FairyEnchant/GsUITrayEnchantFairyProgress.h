// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GsUITrayEnchantFairyBase.h"
#include "GsUITrayEnchantFairyProgress.generated.h"

class UGsUIVFX;
//사용안함.... 괜히... -_-
UCLASS()
class T1PROJECT_API UGsUITrayEnchantFairyProgress : public UGsUITrayEnchantFairyBase
{
	GENERATED_BODY()
private:
	float _lockMaxTime = 0.f;
	bool _isUseTimeout{ false };

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUIVFX* _fXEnchant;
protected:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

public:
	class EnchantProgressEffectInitData : public EffectInitData
	{
	public:
		float _lockTime{ 0.f };
		EnchantProgressEffectInitData(float lockTime) : _lockTime(lockTime) {}
	};
	void InitializeData(EffectInitData* initParam) override;

public:
	UFUNCTION(BlueprintCallable)
	void FinishedAnim();

	UFUNCTION()
	void OnClickBlock();
	
};
