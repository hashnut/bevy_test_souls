#pragma once

#include "CoreMinimal.h"
#include "GsGameFlowBase.h"
#include "Message/GsMessageStage.h"
#include "Message/GsMessageSystem.h"
#include "GsGameFlow.h"

class FGsContentsManagerLobby;
class FGsStageManagerLobby;

/**
 * Lobby Flow.
 * - StageManager: 서버와 동기화 되는 Flow.
 * - ContentsManager: 클라에서 관리를 위해 만드는 Flow.
 */
class T1PROJECT_API FGsGameFlowLobby : public FGsGameFlowBase
{
	// Flow 관련 매니저
	class FGsStageManagerLobby*		_stageManager = nullptr;
	class FGsContentsManagerLobby*	_contentsManager = nullptr;

	bool _isPathComplated = false;	
	
	MsgStageHandleArray		_listStageDelegate;
	MsgSystemHandle			_messageHandle;

	TArray<MsgStageHandle> 	_listStageParamDelegate;

public:
	FGsGameFlowLobby() : FGsGameFlowBase(FGsGameFlow::Mode::LOBBY) {}
	virtual ~FGsGameFlowLobby() = default;

public:	
	virtual void Enter() override;
	virtual void Exit() override;
	virtual void Update(float In_deltaTime) override;

public:
	// 로비는 Reconnection이 없음
	virtual void OnReconnectionEnd() override;
	virtual void OnBackToTitle() override;
	virtual void OnExitGame() override;
	virtual void OnQuitGame() override;
	virtual int32 GetContentStateIndex() const override;
	virtual void OnRestartGame() override;

protected:
	// 메시지 핸들러 등록
	void InitMessageHandler();
	void LobbyFlowMessgeReceiver(const IGsMessageParam* In_data);

protected:
	// MessageStage 메시지 핸들러 함수 정의
	void OnEnterTitle(); // stage
	void OnEnterLogin();
	void OnEnterServerSelect();
	void OnEnterCharacterSelect();
	void OnEnterCharacterCreate();	
	void OnEnterCharacterCreateEmpty();
	void OnEnterNetAddressSelect();	
	void OnEnterIntro();
	void OnEnterOfflinePlayBackGround();
	void OnEnterNetCheckHandShakeCompleted();

	void OnEnterCheckPatchRepairState();
	void OnEnterCharacterCustomizing();
	void OnEnterPatchState();
	void OnEnterCheckPatchState();
	void OnEnterHiveInitState();
	void OnEnterDivergenceState();
	//void OnEnterBannedwrodProcState();
	void OnEnterHiveAuth();

	void OnEnterHiveCheckAuthMaintenance();
	void OnEnterHiveCheckGateWayMaintenance();

	void OnHerculesAuthState();

	void OnEnterPlayableEvent();	
	void OnEnterPostPlayableMovie();
	void OnEnterPrePlayableMovie();

	void OnEnterZpayState();
	void OnLeaveZpayState();

protected:
	void SetIsPathComplated(bool In_isPathComplated) { _isPathComplated = In_isPathComplated; }
	bool GetIsPathComplated() { return _isPathComplated; }

#if RECONNECT_DEBUG
public:
	virtual FString ToString() override;
#endif

public:
	FGsStageManagerLobby* GetStageManagerLobby() { return _stageManager; }
	FGsContentsManagerLobby* GetContentsManagerLobby() { return _contentsManager; }
};
