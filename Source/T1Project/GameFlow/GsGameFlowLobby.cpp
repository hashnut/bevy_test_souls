#include "GsGameFlowLobby.h"

#include "Stage/GsStageMode.h"
#include "Stage/StageLobby/GsStageManagerLobby.h"

#include "T1Project.h"

#include "Net/GsNetBase.h"

#include "GameFlow/GameContents/GsContentsMode.h"
#include "GameFlow/GameContents/ContentsLobby/GsContentsManagerLobby.h"

#include "Management/GsMessageHolder.h"
#include "Management/ScopeLobby/GsLobbyCharacterManager.h"
#include "Management/ScopeGlobal/GsNetManager.h"

#include "Management/ScopeGlobal/GsUIManager.h"
#include "Message/GsMessageStage.h"
#include "Management/ScopeGlobal/GsLevelManager.h"
#include "Management/ScopeGlobal/GsOfflinePlayManager.h"
#include "Management/ScopeGlobal/GsPopupSystemManager.h"
#include "Management/ScopeGlobal/GsPatchManager.h"
#include "Management/ScopeGlobal/GsHiveManager.h"
#include "Management/ScopeGlobal/GsOptionManager.h"

#include "Message/MessageParam/GsStageMessageParam.h"
#include "Message/MessageParam/GsMessageParam.h"
#include "Net/GsNetSendService.h"
#include "UI/UIContent/Helper/GsUIHelper.h"

#include "Management/ScopeGlobal/GsSoundManager.h"


void FGsGameFlowLobby::Enter()
{
	if (nullptr == _stageManager)
	{
		_stageManager = new FGsStageManagerLobby();
	}
	_stageManager->InitState();

	if (nullptr == _contentsManager)
	{
		_contentsManager = new FGsContentsManagerLobby();
	}
	_contentsManager->InitState();

	// DelegateHandle을 Exit시 지우므로, 인게임에서 로비로 재 진입 시 등록이 필요하다. 
	InitMessageHandler();
}

void FGsGameFlowLobby::Exit()
{
	FGsMessageHolder* msg = GMessage();
	
	MStage& stageMsgHandler = msg->GetStage();
	for (MsgStageHandle& elemStage : _listStageDelegate)
	{
		stageMsgHandler.Remove(elemStage);
	}

	MStageParam& msgStage = msg->GetStageParam();
	for (MsgStageHandle& msgStageParamHandle : _listStageParamDelegate)
	{
		msgStage.Remove(msgStageParamHandle);
	}

	_listStageDelegate.Empty();
	_listStageParamDelegate.Empty();

	if (_stageManager)
	{
		_stageManager->RemoveAll();
		delete _stageManager;
		_stageManager = nullptr;
	}

	if (_contentsManager)
	{
		_contentsManager->RemoveAll();
		delete _contentsManager;
		_contentsManager = nullptr;
	}

	UGsLobbyCharacterManager* characterMgr = LSLobbyCharacter();
	if (characterMgr)
	{
		characterMgr->RemoveAll();
	}

	if (UGsUIManager* uiManager = GUI())
	{
		uiManager->RemoveWhenFlowChanged();
	}

	GPopupSystem()->RemoveAll();
}

void FGsGameFlowLobby::Update(float In_deltaTime)
{
	if (_stageManager)
	{
		_stageManager->Update(In_deltaTime);	
	}

	if (_contentsManager)
	{
		_contentsManager->Update(In_deltaTime);
	}
}

void FGsGameFlowLobby::OnReconnectionEnd()
{
	//만약 재연결 요청이 왔는데 상태가 없다면 유저 리스트를 보낼 것
	if (false == _stageManager->GetCurrentState().IsValid()
		&& false == _contentsManager->GetCurrentState().IsValid())
	{		
		if (FGsNetManager* netMgr = GNet())
		{
			//패킷 보내는 도중 블락이 될 경우가 있기 때문에 블락 해제 후
			//로비로 가는 첫번째 패킷인 UserList 요청
			//만약 이 방법도 문제가 있다면 Restart_game으로 대체할 것 
			netMgr->ClearPacketSendBlock();
			FGsNetSendService::SendGatewayReqUserList();
			//GMessage()->GetSystem().SendMessage(MessageSystem::RESTART_GAME);
		}		
	}	
}

void FGsGameFlowLobby::InitMessageHandler()
{
	_listStageDelegate.Empty();

	FGsMessageHolder* msg = GMessage();
	auto& stageMsgHandler = msg->GetStage();

	// 런처
	_listStageDelegate.Emplace(stageMsgHandler.AddRaw(MessageStage::ENTER_NETADDRESSSELECT_STAGE,
		this, &FGsGameFlowLobby::OnEnterNetAddressSelect));

	// 로비
	// 캐릭터 선택 스테이지로 이동
	_listStageDelegate.Emplace(stageMsgHandler.AddRaw(MessageStage::ENTER_CHARACTERSELECT_STAGE,
		this, &FGsGameFlowLobby::OnEnterCharacterSelect));

	// 캐릭터 생성 스테이지로 이동
	_listStageDelegate.Emplace(stageMsgHandler.AddRaw(MessageStage::ENTER_CHARACTERCREATE_STAGE,
		this, &FGsGameFlowLobby::OnEnterCharacterCreate));
	
	// 2021/08/25 PKT - IOS 전용 - 오프라인 플레이 백그라운드 이동
	_listStageDelegate.Emplace(stageMsgHandler.AddRaw(MessageStage::ENTER_OFFLINE_PLAY_BACKGROUND,
		this, &FGsGameFlowLobby::OnEnterOfflinePlayBackGround));

	_listStageDelegate.Emplace(stageMsgHandler.AddRaw(MessageStage::ENTER_PLAYABLE_EVENT,
		this, &FGsGameFlowLobby::OnEnterPlayableEvent));

	_listStageDelegate.Emplace(stageMsgHandler.AddRaw(MessageStage::ENTER_PRE_PLAYABLE_MOVIE_STATE,
		this, &FGsGameFlowLobby::OnEnterPrePlayableMovie));

	_listStageDelegate.Emplace(stageMsgHandler.AddRaw(MessageStage::ENTER_POST_PLAYABLE_MOVIE_STATE,
		this, &FGsGameFlowLobby::OnEnterPostPlayableMovie));
	
	
	////////////////////////////////////////////////////////////////////////////////////////////////
	// State 이동
	////////////////////////////////////////////////////////////////////////////////////////////////

	// 로비
	// 커스터마이징 컨텐츠로 진입
	_listStageDelegate.Emplace(stageMsgHandler.AddRaw(MessageStage::ENTER_CHARACTER_CUSTOMIZING,
		this, &FGsGameFlowLobby::OnEnterCharacterCustomizing));

	_listStageDelegate.Emplace(stageMsgHandler.AddRaw(MessageStage::ENTER_CHARACTER_EMPTY_STATE,
		this, &FGsGameFlowLobby::OnEnterCharacterCreateEmpty));

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////
	MStageParam& msgStage = msg->GetStageParam();
	_listStageParamDelegate.Emplace(msgStage.AddRaw(MessageStage::LOBBY_STATE_UPDATE_NOTIFY,
		this, &FGsGameFlowLobby::LobbyFlowMessgeReceiver));
}

void FGsGameFlowLobby::OnEnterCheckPatchRepairState()
{
	if (nullptr == _stageManager)
		return;

	_stageManager->ChangeState(FGsStageMode::PATCH);
	_contentsManager->ChangeState(FGsContentsMode::REPAIR_CHECK);
}

void FGsGameFlowLobby::OnEnterPatchState()
{	
	if (nullptr == _stageManager || nullptr == _contentsManager)
		return;

	_stageManager->ChangeState(FGsStageMode::PATCH);
	_contentsManager->ChangeState(FGsContentsMode::PATCH);
}

void FGsGameFlowLobby::OnEnterCheckPatchState()
{
	if (nullptr == _stageManager || nullptr == _contentsManager)
		return;

	_stageManager->ChangeState(FGsStageMode::TITLE);
	_contentsManager->ChangeState(FGsContentsMode::PATCH_CHECK);
}

void FGsGameFlowLobby::OnEnterHiveInitState()
{
	if (nullptr == _stageManager || nullptr == _contentsManager)
		return;

	_stageManager->ChangeState(FGsStageMode::TITLE);
	_contentsManager->ChangeState(FGsContentsMode::HIVE_INIT);
}

void FGsGameFlowLobby::OnHerculesAuthState()
{
	if (nullptr == _stageManager || nullptr == _contentsManager)
		return;

	_stageManager->ChangeState(FGsStageMode::TITLE);
	_contentsManager->ChangeState(FGsContentsMode::HERCULES_AUTH);
}

void FGsGameFlowLobby::OnEnterDivergenceState()
{
	if (nullptr == _stageManager || nullptr == _contentsManager)
		return;

	_stageManager->ChangeState(FGsStageMode::TITLE);
	_contentsManager->ChangeState(FGsContentsMode::DIVERGENCE);
}

//void FGsGameFlowLobby::OnEnterBannedwrodProcState()
//{
//	_contentsManager->ChangeState(FGsContentsMode::BANNEDWORD_PROC);
//}

void FGsGameFlowLobby::OnEnterHiveAuth()
{
	if (nullptr == _stageManager || nullptr == _contentsManager)
		return;

	if (GHive()->GetIsDeleteAccountSuccessed())
	{
		FText findText;
		FText::FindText(TEXT("LauncherText"), TEXT("Popup_Delete_Result"), findText);
		FGsUIHelper::PopupSystemMsg(findText, [this]()
			{
				GHive()->SetIsDeleteAccountSuccess(false);
				_stageManager->ChangeState(FGsStageMode::TITLE);
				_contentsManager->ChangeState(FGsContentsMode::HIVE_AUTH);
			});
	}
	else
	{
		_stageManager->ChangeState(FGsStageMode::TITLE);
		_contentsManager->ChangeState(FGsContentsMode::HIVE_AUTH);
	}
}

void FGsGameFlowLobby::OnEnterHiveCheckAuthMaintenance()
{
	if (nullptr == _stageManager || nullptr == _contentsManager)
		return;

	_stageManager->ChangeState(FGsStageMode::TITLE);
	_contentsManager->ChangeState(FGsContentsMode::HIVECHECK_AUTH_MAINTENANCE);
}

void FGsGameFlowLobby::OnEnterHiveCheckGateWayMaintenance()
{
	if (nullptr == _stageManager || nullptr == _contentsManager)
		return;

	_stageManager->ChangeState(FGsStageMode::TITLE);
	_contentsManager->ChangeState(FGsContentsMode::HIVECHECK_GATEWAY_MAINTENANCE);
}

void FGsGameFlowLobby::OnEnterTitle()
{
	if (nullptr == _stageManager || nullptr == _contentsManager)
		return;

	_stageManager->ChangeState(FGsStageMode::TITLE);
	_contentsManager->ChangeState(FGsContentsMode::TITLE);
}

void FGsGameFlowLobby::OnEnterIntro()
{
	if (nullptr == _stageManager || nullptr == _contentsManager)
		return;

	_stageManager->ChangeState(FGsStageMode::INTRO);
	_contentsManager->ChangeState(FGsContentsMode::INTRO);
}

void FGsGameFlowLobby::OnEnterOfflinePlayBackGround()
{
	if (nullptr == _stageManager || nullptr == _contentsManager)
		return;

	_contentsManager->ChangeState(FGsContentsMode::OFFLINE_PLAY_BACKGROUND);
}

void FGsGameFlowLobby::OnEnterLogin()
{	
	if (nullptr == _stageManager || nullptr == _contentsManager)
		return;

	_contentsManager->ChangeState(FGsContentsMode::LOGIN);
}

void FGsGameFlowLobby::OnEnterServerSelect()
{
	if (nullptr == _stageManager || nullptr == _contentsManager)
		return;

	_contentsManager->ChangeState(FGsContentsMode::SERVER_SELCET);
}

void FGsGameFlowLobby::OnEnterNetAddressSelect()
{
	if (nullptr == _stageManager || nullptr == _contentsManager)
		return;

	_stageManager->ChangeState(FGsStageMode::TITLE);
	_contentsManager->ChangeState(FGsContentsMode::NETADDRESS_SELECT);
}

void FGsGameFlowLobby::OnEnterNetCheckHandShakeCompleted()
{
	if (nullptr == _stageManager || nullptr == _contentsManager)
		return;

	_stageManager->ChangeState(FGsStageMode::TITLE);
	_contentsManager->ChangeState(FGsContentsMode::NETCHECK_HANDSHAKECOMPLETED);
}

void FGsGameFlowLobby::OnEnterZpayState()
{
	if (nullptr == _stageManager || nullptr == _contentsManager)
		return;

	_stageManager->ChangeState(FGsStageMode::TITLE);
	_contentsManager->ChangeState(FGsContentsMode::Z_PAY);
}

void FGsGameFlowLobby::OnLeaveZpayState()
{

}

#if RECONNECT_DEBUG
FString FGsGameFlowLobby::ToString()
{
	if (nullptr == _stageManager || nullptr == _contentsManager)
		return;

	if (_stageManager->GetCurrentState().IsValid()
		&& _contentsManager->GetCurrentState().IsValid())
	{
		return FString::Printf(TEXT("FGsGameFlowLobby - FGsStageMode : %d\tFGsContentsMode : %d")
			, (int32)_stageManager->GetCurrentState().Pin()->GetType()
			, (int32)_contentsManager->GetCurrentState().Pin()->GetType());
	}

	return TEXT("FGsGameFlowLobby - no state");
}
#endif

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void FGsGameFlowLobby::OnEnterCharacterSelect()
{
	if (nullptr == _stageManager || nullptr == _contentsManager)
		return;

	_stageManager->ChangeState(FGsStageMode::CHARACTER_SELECT);
	_contentsManager->ChangeState(FGsContentsMode::CHARACTER_SELECT);
}

void FGsGameFlowLobby::OnEnterCharacterCreate()
{
	if (nullptr == _stageManager || nullptr == _contentsManager)
		return;

	_stageManager->ChangeState(FGsStageMode::CHARACTER_CREATE);
	_contentsManager->ChangeState(FGsContentsMode::CHARACTER_CREATE);
}

void FGsGameFlowLobby::OnEnterCharacterCreateEmpty()
{
	if (nullptr == _stageManager || nullptr == _contentsManager)
		return;

	_stageManager->ChangeState(FGsStageMode::CHARACTER_CREATE);
	_contentsManager->ChangeState(FGsContentsMode::CHARACTER_CREATE_EMPTY_STATE);
}

void FGsGameFlowLobby::OnEnterCharacterCustomizing()
{
	if (nullptr == _stageManager || nullptr == _contentsManager)
		return;

	_stageManager->ChangeState(FGsStageMode::CHARACTER_CUSTOMIZING);
	_contentsManager->ChangeState(FGsContentsMode::CHARACTER_CUSTOMIZING);
}

void FGsGameFlowLobby::OnEnterPlayableEvent()
{
	if (nullptr == _stageManager || nullptr == _contentsManager)
		return;

	_stageManager->ChangeState(FGsStageMode::PLAYABLE_EVENT);
	_contentsManager->ChangeState(FGsContentsMode::HUD);
}

void FGsGameFlowLobby::OnEnterPostPlayableMovie()
{
	if (nullptr == _stageManager || nullptr == _contentsManager)
		return;

	_stageManager->ChangeState(FGsStageMode::CHARACTER_SELECT);
	_contentsManager->ChangeState(FGsContentsMode::POST_PLAYABLE_MOVIE);
}

void FGsGameFlowLobby::OnEnterPrePlayableMovie()
{
	if (nullptr == _stageManager || nullptr == _contentsManager)
		return;

	_stageManager->ChangeState(FGsStageMode::CHARACTER_SELECT);
	_contentsManager->ChangeState(FGsContentsMode::PRE_PLAYABLE_MOVIE);
}


void FGsGameFlowLobby::OnBackToTitle()
{
	GSLOG(Log, TEXT("FGsGameFlowLobby::OnBackToTitle()"));	

	// UI 터치 풀기
	FGsUIHelper::ForcedHideBlockUI(EGsHideBlockUIFactor::ON_BACK_TO_TITLE);
	FGsUIHelper::TrayDimmed(false);
	// 타이틀로 돌아왔으면 재 로그인
	GNet()->OnExitGame();
	// 타이틀로 돌아왔음 패치 체크 여부 초기화
	SetIsPathComplated(false);
	//if (GPatch()->IsComplated())
		//FGsUIHelper::TrayDimmed(false);

	if (UGsLevelManager* level = GLevel())
	{
		level->LoadLauncher();
	}
	
	OnEnterNetCheckHandShakeCompleted();
	GSound()->SetSoundLobbyMaster();
}

void FGsGameFlowLobby::OnExitGame()
{
	GSLOG(Log, TEXT("FGsGameFlowLobby::OnExitGame()"));

	// UI 터치 풀기
	FGsUIHelper::ForcedHideBlockUI(EGsHideBlockUIFactor::ON_EXIT_GAME);
	FGsUIHelper::TrayDimmed(false);
	GUI()->CloseAllStack();
	// 타이틀로 돌아왔으면 재 로그인
	GNet()->OnExitGame();
	// 타이틀로 돌아왔음 패치 체크 여부 초기화
	SetIsPathComplated(false);
	//if (GPatch()->IsComplated())
		//FGsUIHelper::TrayDimmed(false);	

	if (UGsLevelManager* level = GLevel())
	{
		if (false == level->LoadLauncher())
		{
#if FLOW_DUBUG_TEST
			OnEnterDivergenceState();
#else
#if WITH_EDITOR
			OnEnterNetAddressSelect();
#else			
			OnEnterDivergenceState();
#endif
#endif
		}
	}

	GSound()->SetSoundLobbyMaster();
}

void FGsGameFlowLobby::OnQuitGame()
{
	FGsStageMode::Lobby stageMode = _stageManager->GetCurrentStageMode();
	FGsContentsMode::InLobby contentsMode = _contentsManager->GetCurrentContentsMode();

	GSLOG(Log, TEXT("FGsGameFlowLobby::OnQuitGame(), stageMode: %d, contentsMode: %d"), 
		static_cast<int32>(stageMode), static_cast<int32>(contentsMode));
	
	if (FGsStageMode::Lobby::INTRO == stageMode)
	{
		// 인트로에서는 아무것도 할 수 없게 수정
		return;
	}
	// https://jira.com2us.com/jira/browse/C2URWQ-1921
	// 천마대전 1, 2 영상에 back key 눌렀을때 아무 처리 안하게 처리
	if (contentsMode == FGsContentsMode::InLobby::PRE_PLAYABLE_MOVIE ||
		contentsMode == FGsContentsMode::InLobby::POST_PLAYABLE_MOVIE)
	{
		return;
	}


	// 타이틀이 아닌 곳에서는 기존 로직 타도록 수정
	if (FGsStageMode::Lobby::TITLE != stageMode)
	{
		OnExitGame();
		return;
	}

	// 타이틀 메뉴에서도 제외할 항목들 정의. 일단 별도 창을 띄우는 Z_PAY 제외
	
	if (FGsContentsMode::InLobby::Z_PAY == contentsMode)
	{
		OnExitGame();
		return;
	}

	FGsUIHelper::PopupQuitGame();
}

void FGsGameFlowLobby::OnRestartGame()
{
	GSLOG(Log, TEXT("FGsGameFlowLobby::OnRestartGame()"));
	OnExitGame();
}

int32 FGsGameFlowLobby::GetContentStateIndex() const
{
	return static_cast<int32>(_contentsManager->GetCurrentContentsMode());
}

void FGsGameFlowLobby::LobbyFlowMessgeReceiver(const IGsMessageParam* In_data)
{
	const FGsLobbyFlowMsgParam* param = In_data->Cast<const FGsLobbyFlowMsgParam>();
	if (nullptr == param)
		return;

	GSLOG(Log, TEXT("FGsGameFlowLobby::LobbyFlowMessgeReceiver() FGsLobbyFlowMsgParam : %s"), *GetEnumToString(
		MessageStage, param->_messageType));
	
	switch (param->_messageType)
	{
	case MessageStage::COMPLETED_MODELAUNCHER:
	{
		// 런처모드로 게임 실행이 끝났을때
		OnEnterIntro();
	}
	break;
	case MessageStage::FNINSHED_INTRO_EFFECT:
	case MessageStage::ERROR_OFFLINE_PLAY_BACKGROUND:
	{
		// 인트로 연출이 끝났을때 (컴투스~)

		//2021/08/26 PKT - 오프라인 플레이로 인한 종료 상태로 들어갈지에 대한 결정을 최 우선으로 한다.
		FGsOfflinePlayManager* offlinePlayManager = GSOfflinePlay();
		if (nullptr != offlinePlayManager && true == offlinePlayManager->InOnFlag())
		{	// 2021/08/25 PKT - 오프라인 플레이로 인한 타이틀 화면 이동이라면 오프라인 백그라운드 스테이지로 이동
			OnEnterOfflinePlayBackGround();
		}
		else
		{
#if FLOW_DUBUG_TEST
			OnEnterDivergenceState();
#else
#if WITH_EDITOR
			OnEnterNetAddressSelect();
#else
			OnEnterDivergenceState();
#endif
#endif
		}
	}
	break;
	case MessageStage::COMPLETED_BACKTO_LAUNCHER:
		{
			// 캐릭터 선택이나 게임에서 다시 BACK 하고 모드 런처가 시작됐을 때
			OnEnterNetCheckHandShakeCompleted();
		}
	break;
	case MessageStage::SUCCEEDED_HIVE_DIVERGENCE:
	case MessageStage::RETRY_HIVE_INIT:
	case MessageStage::RETRY_HIVE_AUTH_SINGNIN:
		{
			// 하이브 Divergence 요청 성공 시,
			// 하이브 Auth 실패, 무응답 시,
			OnEnterHiveInitState();
		}
		break;
	case MessageStage::SUCCEEDED_HIVE_INIT:
		{
			// 허큘리스 Auth
			OnHerculesAuthState();
		}
	break;
	case MessageStage::SUCCEEDED_HIVE_AUTH:
		{			
			// AUTH 로그인 전 점검팝업 체크
			OnEnterHiveCheckAuthMaintenance();
		}
		break;
	case MessageStage::SUCCEEDED_CHECK_AUTH_MAINTENANCE:
		{
			// net 연결 체크
			OnEnterNetCheckHandShakeCompleted();
		}
		break;
	case MessageStage::FAILED_NETCHECK_HANDSHAKE:
	case MessageStage::FAILED_LOGIN:
	case MessageStage::FAILED_PATCH:
	case MessageStage::REQUEST_PATHC_REJECT:
	case MessageStage::FAILED_CHECK_MAINTENANCE:
	case MessageStage::CHECK_PATHC_FAILED:
	case MessageStage::FAILED_REPAIR_CHECK:
		{
			// 네트워크 연결 실패 시
			// 로그인 실패 시
			// 패치 체크 실패 시
			// 다운로드 거절 시
			// 패치 복구 실패 시
			OnBackToTitle();
		}			
		break;	
	case MessageStage::BACKTO_DIVERGENCE:
		{			
			OnRestartGame();
		}
		break;
	case MessageStage::SUCCEEDED_CHECK_GATEWAY_MAINTENANCE:
		{
			TSharedPtr<FGsNetBase> netBase = GNet()->GetActive();
			if (netBase.IsValid())
			{
				//OnEnterTitle();
				FGsNetSendService::SendAuthReqSelectServer(GNet()->GetPlanetWorldId());
			}
			else
			{
				// 패치 받고 다시 서버로그인
				OnEnterNetCheckHandShakeCompleted();
			}
		}
		break;
	case MessageStage::COMPLETED_NETCHECK_HANDSHAKE:
		{
			// 서버선택 성공
			GMessage()->GetSystem().SendMessage(MessageSystem::AUTH_SERVER_CONNECTION_SUCCESS);

			// 로그인 윈도우 창 오픈 (에디터에선 vid를 입력한다.)
			OnEnterLogin();
		}
		break;
	case MessageStage::SELECTED_NETADDRESS:
		{
			// net 연결 체크
			OnEnterNetCheckHandShakeCompleted();
		}
		break;

	case MessageStage::SUCCEEDED_LOGIN:
		{
			OnEnterTitle();
			if (true == GetIsPathComplated())
			{
				FGsNetSendService::SendAuthReqSelectServer(GNet()->GetPlanetWorldId());				
			}
		}
		break;
	case MessageStage::COMPLETED_PATCH:
		{
			SetIsPathComplated(true);

#if FLOW_DUBUG_TEST
			//OnEnterHiveCheckMaintenance();
			// 게이트웨이 붙기 전 점검팝업 체크
			OnEnterHiveCheckGateWayMaintenance();
#else
#if WITH_EDITOR
			OnEnterTitle();
			FGsNetSendService::SendAuthReqSelectServer(GNet()->GetPlanetWorldId());
#else
			//OnEnterHiveCheckMaintenance();
			// 게이트웨이 붙기 전 점검팝업 체크
			OnEnterHiveCheckGateWayMaintenance();
#endif
#endif
		}
		break;
	case MessageStage::REQUEST_PATCH_REPAIR:
		{
			OnEnterCheckPatchRepairState();
		}
		break;
	case MessageStage::REQUEST_PATCH_START:
		{
			// 패치가 있으니 패치 스테이트로 변경
			OnEnterPatchState();
		}
		break;
	case MessageStage::CHECK_PATHC_COMPLETE:
		{
			// 패치 받을게 없다.
			// 점검팝업 체크			
			//OnEnterHiveCheckMaintenance();
			// 게이트웨이 붙기 전 점검팝업 체크
			OnEnterHiveCheckGateWayMaintenance();
		}
		break;
	case MessageStage::REQUEST_TITLE_CLICK_EVENT:
		{
#if FLOW_DUBUG_TEST
			OnEnterCheckPatchState();
#else
#if WITH_EDITOR
			FGsNetSendService::SendAuthReqSelectServer(GNet()->GetPlanetWorldId());
#else
			OnEnterCheckPatchState();
#endif
#endif
		}
		break;
	case MessageStage::SUCCEEDED_HIVE_LOGOUT:
		{
			OnEnterHiveAuth();
		}
		break;
	case MessageStage::SUCCEEDED_HERCULES_AUTH:
	case MessageStage::FAILED_HERCULES_AUTH:
		{
			OnEnterHiveAuth();
		}
	break;

	case MessageStage::ENTER_Z_PAY:
		{
			OnEnterZpayState();
		}
	break;
	case MessageStage::LEAVE_Z_PAY:
		{
			OnLeaveZpayState();
		}
	break;

	default:
		break;
	}
}