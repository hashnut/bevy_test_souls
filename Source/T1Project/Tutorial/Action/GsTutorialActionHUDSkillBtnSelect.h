// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Tutorial/Action/GsTutorialActionBase.h"

class UGsUISkillSlotSetHUD;


/**
 * HUD 스킬버튼 선택
 */
class T1PROJECT_API FGsTutorialActionHUDSkillBtnSelect final : public FGsTutorialActionBase
{
public:
	using Super = FGsTutorialActionBase;

private:	
	TWeakObjectPtr<UGsUISkillSlotSetHUD> _skillSlotSetUI;

public:
	FGsTutorialActionHUDSkillBtnSelect() : Super()
	{
	}
	virtual ~FGsTutorialActionHUDSkillBtnSelect() = default;

public:
	virtual bool OnStart() override;
	virtual void OnEnd(bool bInSendMessage = true) override;
};
