// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "Message/GsMessageContents.h"
#include "Message/GsMessageGameObject.h"
#include "Message/GsMessageUI.h"
#include "Message/GsMessageSystem.h"

struct IGsMessageParam;

/**
 * HUD 스킬 버튼 로직 
 */
class T1PROJECT_API FGsTutorialHandler final
{
public:	
	using MsgHandleTutorial = TPair<MessageContentTutorial, FDelegateHandle>;
	using MsgHandleUnlock = TPair<MessageContentUnlock, FDelegateHandle>;
	using MsgHandleQuest = TPair<MessageContentQuest, FDelegateHandle>;
	using MsgHandleSaveBattery = TPair<MessageContentSaveBattery, FDelegateHandle>;

private:
	MsgSystemHandleArray _msgHandleSystemList;
	MsgSystemHandleArray _msgHandleSystemParamList;
	TArray<MsgHandleTutorial> _msgHandleTutorialList;
	MsgGameObjHandleArray _msgHandleGameObjectList;
	MsgUIHandleArray _msgHandleUIList;
	TArray<MsgHandleUnlock> _msgHandleUnlockList;
	TArray<MsgHandleQuest> _msgHandleQuestList;
	TArray<MsgHandleSaveBattery> _msgHandleSaveBatteryList;

public:
	FGsTutorialHandler() = default;
	~FGsTutorialHandler() {}
	
public:
	void Init();
	void Close();	

private:
	void BindMessages();
	void UnbindMessages();

	// MessageSystem
private:
	// MSystem
	void OnReconnection();
	void OnLoadTopoStart(const IGsMessageParam* inParam);

	// MSystemParam
	void OnCinematicStart(const IGsMessageParam*);
	void OnCinematicEnd(const IGsMessageParam*);

	// MessageContentTutorial
private:
	void OnStartTutorial(const IGsMessageParam*);
	void OnEndTutorial(const IGsMessageParam* InParam);
	void OnForcedEndTutorial(const IGsMessageParam*);
	void OnEndAction(const IGsMessageParam* InParam);
	void OnEventActionTutorialStart(const IGsMessageParam* InParam);
	void OnOpenUI(const IGsMessageParam* InParam);	
	void OnOpenUICheckParam(const IGsMessageParam* InParam);
	void OnEndUIInteraction(const IGsMessageParam* InParam);
	void OnLearnSkill(const IGsMessageParam* InParam);
	void OnSummonButtonState(const IGsMessageParam* InParam);
	void OnFirstTutorial(const IGsMessageParam* InParam);

	// MessageGameObject
private:
	void OnLocalPlayerDead(const IGsMessageParam*);
	void OnLocalSpawnMeComplete(const IGsMessageParam*);

	// MessageUI
private:
	void OnUIReservedQueueEmpty(const IGsMessageParam*);
	void OnStopMovie(const IGsMessageParam*);
	

	// MessageContentUnlock
private:
	void OnContentsUnlock(const IGsMessageParam* InParam);

	// MessageContentQuest
private:
	void OnAcceptQuest(const IGsMessageParam* InParam);
	void OnReadyQuest(const IGsMessageParam* InParam);

	// MessageContentSaveBattery
private:
	void OnSaveBatteryEnter(const IGsMessageParam*);
	void OnSaveBatteryExit(const IGsMessageParam*);
};
