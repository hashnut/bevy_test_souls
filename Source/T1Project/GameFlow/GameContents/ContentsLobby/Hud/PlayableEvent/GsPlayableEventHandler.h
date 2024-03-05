#pragma once

#include "CoreMinimal.h"
#include "Engine/Classes/Engine/EngineTypes.h"

#include "PlayableEvent/GsPlayableEventDefine.h"
#include "Quest/Dialog/SubDialog/GsSubDialog.h"

#include "Message/GsMessageSystem.h"
#include "Message/GsMessageUI.h"

/*
*  플레이어블 이벤트 시스템 핸들러
*/
struct FGsSchemaPlayableEventQuest;
struct IGsMessageParam;

class ULevelStreamingDynamic;
class ULevel;
class UWorld;

class FGsPlayableEventHandler
{
private:	
	// 진행중인가
	bool _isProgress = true;
	// end dirty 들어왔는지
	bool _isEndDirty = false;
	// last cam lerp dirty
	bool _isEndCamLerpDirty = false;
	// 모두 삭제 dity
	bool _isMonsterAllDestroyDirty = false;

	bool _isStartCamMoveDirty = false;

	// 서브대화창
	FGsSubDialog* _questSubDialog;

	// 플레이어블 이벤트용 yaw min 세팅전 백업값
	float _backupPlayableEventViewYawMin = 0.0f;
	// 플레이어블 이벤트용 yaw max 세팅전 백업값
	float _backupPlayableEventViewYawMax = 0.0f;

	ULevelStreamingDynamic* _loadedLevel;

	FTimerHandle _finishDelayTimeHandle;

	FTimerHandle _waitQuestGoalTimeHandle;

	FTimerHandle _trayWaitTimeHandle;

	bool _isLastQuestComplete = false;

	FString _levelPath;

	MsgSystemHandleArray	_listSystemDelegate;
	MsgSystemHandleArray	_listSystemParamDelegate;

	MsgUIHandleArray _msgUIHandlerList;

	bool _isStartGoalSubDialog = false;

	bool _isSequencePlay = false;

	// 로직 함수
public:
	// 초기화(최초 한번)
	void Init();
	// 정리(마지막 한번)
	void Close();

	void Update(float In_deltaTime);

	// 이벤트 시작 호출
	void StartEvent();

	void PostPlayableEventLevel();

	// 이벤트 종료
	void EndEvent();
	// 로컬 스폰
	void SpawnPlayableEventLocal();
	// 로컬 디스폰
	void DespawnPlayableEventLocal();
	// collect ani play bp in level
	void CollectArrangedNPCAniPlayBP();
	
	
	// 퀘스트 데이터 변경
	void ChangeQuestData(int In_index);
	// 다음 퀘스트로 변경
	void ChangeNextQuest();
	// 조이스틱 보이게
	void ShowJoystick();
	// 조이스틱 안보이게
	void HideJoystick();
	// 퀘스트 컴플리트때 하는일 처리
	void ActQuestCompleteJob();
	// 지역명 처음에 출력
	void ShowAreaUI();

	void RecoverPlayableEventCamYawMinMax();
	// clear
	void ClearPlayableEvent();

	void RemoveMessageBind();

	void StartPlayableEventStartCam();

	void OnFinishPlayableCamLerp(bool In_isStartCam);

	void StartPlayableEventEndCam();

public:
	// 킬카운트 처리
	void OnAddKillCount(int In_tblId);

	void OnLevelAddedToWorld(ULevel* Level, UWorld* World);

	void OnFinishGoalTimmerWait();


	// msg 
public:
	void OnExitGame();
	void OnFinishSubDialog(const IGsMessageParam*);
	void OnStopMovie(const IGsMessageParam* In_param);

	// get, set
public:
	// 현재 이동 퀘스트  완료인지
	bool IsCurrentMoveQuestComplete();
	// 플레이어블 이벤트 yaw min, max 백업
	void SetBackupPlayableEventYawMinMax(float In_min, float In_max)
	{
		_backupPlayableEventViewYawMin = In_min;
		_backupPlayableEventViewYawMax = In_max;
	}

	bool GetIsProgress()
	{
		return _isProgress;
	}
	
};