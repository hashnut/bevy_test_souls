// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Message/GsMessageContents.h"
#include "Message/GsMessageGameObject.h"

#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "Message/GsMessageSystem.h"
#include "Quest/Dialog/SubDialog/GsSubDialog.h"
#include "Quest/Dialog/NpcDialog/GsNpcDialog.h"

#include "EventProgress/GsEventProgressInterface.h"
#include "EventProgress/GsEventProgressDefine.h"
#include "Runtime/DataCenter/Public/Quest/GsQuestEnum.h"
#include "Camera/Define/GsCameraGameDefine.h"

struct IGsMessageParam;
struct FGsSchemaSubDialogData;
struct FGsSchemaNpcDialogData;
struct FGsSchemaCameraDialogViewData;
class UGsGameObjectBase;

DECLARE_DELEGATE(FFinishDelegate);
DECLARE_DELEGATE_TwoParams(FNpcDialogEndDelegate, bool, int64);

/**
 * 
 */
class T1PROJECT_API FGsQuestActionManagement : public IGsEventProgressInterface
{
private:
	MsgGameObjHandleArray								_listMsgHandle;
	MsgSystemHandleArray								_listSystemMsgHandle;
	TArray<TPair<MessageContentEvent, FDelegateHandle>>	_listMsgHandleEvent;
	TArray<TPair<MessageContentQuest, FDelegateHandle>> _messageContentQuestList;

	// system 메세지 해제용
	MsgSystemHandleArray _systemMsg;
private:
	QuestId			_questId					= 0;
	QuestId			_nextQuestId				= 0;	
	int32			_subDialogId				= 0;
	int64			_gameId						= 0;
	int32			_preEventActionGroupId		= 0;
	int32			_postEventActionGroupId		= 0;
	QuestEventType	_questEventType				= QuestEventType::EVENT_STROY_BEGIN;
	EventActionIndex _preEventActionIndex		= INVALID_EVENT_ACTION_INDEX;
	EventActionIndex _postEventActionIndex		= INVALID_EVENT_ACTION_INDEX;
	bool			_isEventReStart				= false;

	int				_backupEventActionDialogId		= 0;
	int				_backupEventActionTargetNpcId	= 0;

	// npc대화
	int32						_npcDialogId			= 0;		
	bool						_isCameraWalking		= true;
	FVector						_LocalPlayerPosBackup	= FVector::ZeroVector;
	UGsGameObjectBase*			_npcDialogIdTarget		= nullptr;					// npc대화창 대상 npc	
	TUniquePtr<FGsNpcDialog>	_questNpcDialog;									// npc대화창	
	FNpcDialogEndDelegate		_delegateNpcDialog;

	const FGsSchemaNpcDialogData* _npcDialogData;	
	const FGsSchemaCameraDialogViewData* _npcDialogCameraViewType = nullptr;	// npc대화창 default CameraViewType		

	// 서브대화창
	TUniquePtr<FGsSubDialog>	_questSubDialog;

public:
	// 이벤트 시작
	virtual void StartEvent(EGsEventProgressType In_startType, EGsEventProgressType In_preType) override;

public:
	void Initialize();
	void Finalize();
	// 로비로 넘어갈때 호출되는 함수
	void Clear();

protected:
	void RegisterMessages();
	void UnregisterMessages();

public:
	void ClearData();

	// 액션 시작 [챕터 오픈 연출, npc 대화, 서브 대화..]
	void OnQuestEventBeginStart(const struct IGsMessageParam* inData);
	void OnQuestEventClearStart(const struct IGsMessageParam* inData);

	// 액션 끝
	void EndQuestAction();
	// 액선 중지
	void StopQuestAction();
	// 액션 중인가?
	bool IsActioning();
	// 액션 델리게이트 삭제
	void ClearQuestAction(QuestId InQuestId);

	// npc 다이얼로그는 액션리스트 제외
	void OpenNpcDialog(QuestId inQuestId, int64 inGameId, const FGsSchemaNpcDialogData* inDialogData, FNpcDialogEndDelegate const& inDelegate);
	void OpenEventDialog(int In_targetNpcTblId, int In_npcDialogId);

protected:
	// 새 챕터가 시작 될 때 연출 티커
	void PalyChapterStartTicker();
	void FinishChapterStartTicker();

	// 서브대화창
	void OpenSubDialog();
	void CloseSubDialog();

	// npc대화창	
	void CameraLerpEndPlayNpcDialog(const IGsMessageParam* In_data);
	void FadeInEndPlayNpcDialog();
	void PlayNpcDialog(EGsCamearDialogEndCallBackType inCamearDialogEndCallBackType);
	void EndNpcDialog();
	void CloseNpcDialog(const IGsMessageParam* In_data);
	void NpcDialogOpneUI();

	//EventAction
	void OnEventAction();

	// EventAction PreEventAction
	void StartPreEventAction();
	void FinishedPreEventAction();

	// EventAction PostEventAction
	void StartPostEventAction();
	void FinishedPostEventAction();

	void FinishedEventAction(const IGsMessageParam& In_data);

	// user die
	void OnLocalPlayerDie(const IGsMessageParam*);
	// user Hit
	void OnHitLocal(const IGsMessageParam*);

	void EventActionAllClearImmediate(const IGsMessageParam& Param);
	// Reconnection
	void OnReconnection();

	// 워프전 처리
	void OnLocalWarp(const IGsMessageParam* inParam);

public:
	static void SetFairyHiddenInGame(bool inValue);

private:
	void SaveLocalPlayerLocation();
	bool IsLocationSame();

public:
	// 치트용 (UI팀)
	void OpenSubDialog(int32 inDialogId);
	void OpenNpcDialog(int32 inDialogId, float inVelocity);
	void OpenNpcDialog(int32 inDialogId, int32 npcTblId);
	//void PlayNpcDialog(int32 inDialogId, int32 npcTblId);
	void PalyChapterStartTicker(QuestId inQuestId);
	const FGsSchemaNpcDialogData* GetNpcDialogData(int32 InRowKey);
	const FGsSchemaSubDialogData* GetSubDialogData(int32 InRowKey);

	void OnCloseSubDialog();
	void OnCloseNpcDialog();
};
