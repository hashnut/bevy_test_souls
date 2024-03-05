// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/UILib/Base/GsUIHUD.h"
#include "GsUIHUDPickUp.generated.h"


/**
 * HUD 줍기 버튼 
 */
UCLASS(meta = (DisableNativeTick))
class T1PROJECT_API UGsUIHUDPickUp : public UGsUIHUD
{
	GENERATED_BODY()
	
protected:	
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UGsButton* _btnPickUp;
	
protected:
	virtual void NativeOnInitialized() override;

public:
	UFUNCTION()
	void OnClickPickUp();
};
