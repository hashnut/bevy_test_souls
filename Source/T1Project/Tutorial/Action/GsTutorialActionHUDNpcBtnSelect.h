// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Tutorial/Action/GsTutorialActionBase.h"

class UGsUINPCFunctionSlotSetHUD;

/**
 * HUD Npc버튼 선택
 */
class T1PROJECT_API FGsTutorialActionHUDNpcBtnSelect final : public FGsTutorialActionBase
{
public:
	using Super = FGsTutorialActionBase;

private:	
	TWeakObjectPtr<UGsUINPCFunctionSlotSetHUD> _npcSlotSetUI;

public:
	FGsTutorialActionHUDNpcBtnSelect() : Super()
	{
	}
	virtual ~FGsTutorialActionHUDNpcBtnSelect() = default;

public:
	virtual bool OnStart() override;
	virtual void OnEnd(bool bInSendMessage = true) override;
};
