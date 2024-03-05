// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GsUITrayEnchantFairyBase.h"
#include "GsUITrayEnchantFairyWait.generated.h"

class UGsButton;
/**
 * 
 */
UCLASS()
class T1PROJECT_API UGsUITrayEnchantFairyWait : public UGsUITrayEnchantFairyBase
{
	GENERATED_BODY()

private:
	float _lockMaxTime = 0.f;
	bool _useButton{ false };

protected:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

public:
	class EnchantWaitEffectInitData : public EffectInitData
	{
	public:
		float _lockTime{ 0.f };
		bool _useButton{ false };
		EnchantWaitEffectInitData(float lockTime, bool useButton = false) : _lockTime(lockTime), _useButton(useButton){}
	};
	void InitializeData(EffectInitData* initParam) override;

private:
	UFUNCTION()
	void OnClickConfirm();
	
};
