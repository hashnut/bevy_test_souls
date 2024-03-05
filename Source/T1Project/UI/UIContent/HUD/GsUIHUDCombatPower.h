// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/UILib/Base/GsUIHUD.h"
#include "Message/GsMessageContentHud.h"
#include "GsUIHUDCombatPower.generated.h"


/**
 * HUD 로컬플레이어 카오틱 상태 표시
 */

UCLASS(meta = (DisableNativeTick))
class T1PROJECT_API UGsUIHUDCombatPower : public UGsUIHUD
{
	GENERATED_BODY()

public:	
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
		class UTextBlock* _combatPowerTextBlock;

private:
	MsgHudHandleArray _hudMessageHandlerArray;

public:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

private:
	void BindMessage();
	void UnbindMessage();
	void OnInvalidCombatPower();
	void InitCombatPower();
};
