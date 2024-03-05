#pragma once

#include "GsSystemMessageHandlerInterface.h"
#include "Message/GsMessageSystem.h"

using FGsSystemMessage = TGsMessageDelegateList<MSystem, MessageSystem>;

class UGsUIPopupSystem;
class T1PROJECT_API FGsSystemMessageHandlerGlobal :
	public IGsMessageHandler, FGsSystemMessage
{
private:
	bool _enterGameFadeOutComplate = false;
	bool _lobbyToGameLoadTopoReceve = false;

	bool _authServerConneted = false;
	bool _showNetErrPopup = false;

	// load to po start 되기전 warp 연출이 끝나서 맵 로딩 하면 안됨
	bool _isDirtyLoadTopoStart = false;
	bool _isDirtyWarpStateFinished = false;

	// 플레이어가 워프 요청을 보냈는지 체크 (네트워크 재연결 등 대응)
	bool _isPlayerRequestWarp = false;

	// system 메세지 해제용
	MsgSystemHandleArray _systemMsg;

public:
	FGsSystemMessageHandlerGlobal() = default;
	virtual ~FGsSystemMessageHandlerGlobal();

public:
	virtual void InitializeMessage() override;
	virtual void FinalizeMessage() override;

private://bak1210 : 인게임 진입을 위한 연출(FadeOut) 함수
	void ClearFlagLobbyToGame();
	void CompleteEnterGameFadeOut();
	void RecvLobbyToGameLoadTopo();

private:
	void OnSystemInit();
	void OnSystemShutdown();

protected:	
	void RecvBackToLogin();

	void RecvBackToLobby();
	void ProcessBackToLobby();
	void ProcessBackToTitle();

protected:
	void ShowTrayTickerMsgDeveloping();

protected:
	void OnAuthServerConnected();
	void OnAuthServerDisconected();
	void OnAuthServerConnectFailed();

protected:
	void OnGameServerConnected();
	void OnGameServerDisconected();
	void OnGameServerConectedFailed();
	void OnGameServerReconnectionSuccess();

protected:
	void OnRemoveAllGameObject();

	// LoadTopo 
protected:
	// LoadTopo 시작
	void OnLoadTopoStart(const IGsMessageParam* inParam);
	// LocalPlayer 워프 연출 요청이 들어왔는지 체크
	void OnLoadTopoRequestWarpEffect();
	// LocalPlayer 워프 연출 끝났을 때
	void OnLoadTopoEndWarpEffect(); 
	// LoadTopo로 인한 FadeOut이 끝났을 때
	void OnLoadTopoEndFadeOut(); 
	// LocalPlayer StateSpawn Enter 시
	void OnLoadTopoStartSpawn(); 
	// 서버에 LoadTopoFin 보낸 직후
	void OnAfterLoadTopoFin(); 
	// loadtopo 로비에서 게임진입
	void OnLoadTopoLobbyToGame(); 
	// 워프 연출에 성공하면 true리턴, 아니면 false 리턴
	bool PlayWarpEffect(); 
	// LoadTopo 시 클리어할 것들
	void ClearAllObjectLoadTopo(bool In_isDiffrentMap); 
	// LoadTopo로 인한 FadeOut 오픈
	void ProcessLoadTopoFadeOut(bool bPlayWarpSuccess = true);
	// 게임모드 시작
	void OnGameModeWorldStartPlay();
};