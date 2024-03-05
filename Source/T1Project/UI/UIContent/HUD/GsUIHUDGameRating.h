// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/UILib/Base/GsUIHUD.h"
#include "GsUIHUDGameRating.generated.h"

/**
 * 
 */
UCLASS()
class T1PROJECT_API UGsUIHUDGameRating : public UGsUIHUD
{
	GENERATED_BODY()

protected:
	double _endTime{ 0 };

public:
	virtual void NativeOnInitialized() override;
	virtual bool UpdateManagerTick(float InDeltaTime) override;
	void SetVisibleTime(float sec);
};
