// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GsContentsGameBase.h"
#include "Message/GsMessageInput.h"

/**
 * 
 */
class FGsInputEventMsgBase;

class T1PROJECT_API FGsGameStateKeyBoardMapping : public FGsContentsGameBase
{
private:
	TWeakObjectPtr<class UGsUIWindowKeyBoardMapping>	_windowUI;
	TPair<MessageInput, FDelegateHandle> _delegatePressQuickExecute;

public:
	FGsGameStateKeyBoardMapping();
	virtual ~FGsGameStateKeyBoardMapping();

public:
	virtual void Enter() override;
	virtual void Exit() override;
	virtual void Update(float In_deltaTime) override;

	virtual void Init() override;
	virtual void Close() override;

	void OnPressQuickExecute(const FGsInputEventMsgBase&);

protected:
	virtual bool IsChangeSoundMixer() const override { return false; }
};
