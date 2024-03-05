#include "GsSystemMessageHandlerGlobal.h"

#include "Management/ScopeGlobal/GsNetManager.h"
#include "Management/ScopeGlobal/GsUIManager.h"
#include "Management/ScopeGlobal/GsGameFlowManager.h"
#include "Management/ScopeGlobal/GsLevelManager.h"

#include "Management/ScopeGame/GsCameraModeManager.h"
#include "Management/ScopeGame/GsGameObjectManager.h"
#include "Management/ScopeGame/GsClientSpawnManager.h"
#include "Management/ScopeGame/GsSummonManager.h"
#include "Management/ScopeGame/GsObserverManager.h"
#include "Management/ScopeGame/GsEventProgressManager.h"
#include "Management/ScopeGame/GsInteractionManager.h"

#include "Management/GsMessageHolder.h"
#include "Management/GsScopeHolder.h"

#include "GameObject/GsGlobalGameObjects.h"
#include "GameObject/State/GsGameObjectStateManager.h"
#include "GameObject/ObjectClass/GsGameObjectBase.h"
#include "GameMode/GsGameModeWorld.h"
#include "GameMode/GsGameModeLobby.h"
#include "GameFlow/GsGameFlow.h"

#include "GameFlow/GsGameFlowGame.h"
#include "GameFlow/GameContents/ContentsGame/GsContentsManagerGame.h"
#include "GameFlow/GameContents/ContentsGame/Hud/GsGameStateHud.h"
#include "GameFlow/GameContents/ContentsGame/Hud/Summon/GsSummonHandler.h"
#include "GameFlow/GameContents/GsContentsMode.h"

#include "Data/GsDataContainManager.h"
#include "Data/GsGlobalConstant.h"

#include "Message/MessageParam/GsGameObjectMessageParam.h"

#include "UI/UIContent/Helper/GsUIHelper.h"
#include "UI/UIContent/Tray/GsUITrayFadeEffect.h"

#include "Management/ScopeGlobal/GsOptionManager.h"
#include "Option/GsGameUserSettings.h"

#include "Net/GsNetSendService.h"
#include "Engine/World.h"
#include "Classes/GameFramework/GameModeBase.h"

#include "Movie/GsMoviePostProcess.h"

#include "T1Project.h"
#include "../UI/UIContent/Popup/GsUIPopupSystem.h"
#include "../PopupSystem/GsPopupSystemEnum.h"
#include "../Management/ScopeGlobal/GsPopupSystemManager.h"
#include "../Management/ScopeGlobal/GsSkillManager.h"
#include "Message/GsMessageSystem.h"
#include "Message/MessageParam/GsSystemMessageParam.h"
#include "Sequence/GsSequencePlayer.h"


FGsSystemMessageHandlerGlobal::~FGsSystemMessageHandlerGlobal()
{
}


void FGsSystemMessageHandlerGlobal::InitializeMessage()
{
	// 패킷 바인딩
	
	
	MSystem& system = GMessage()->GetSystem();

	InitializeMessageDelegateList(&system);

	AddMessageDelegate(system.AddRaw(MessageSystem::SYSTEM_INIT,
		this, &FGsSystemMessageHandlerGlobal::OnSystemInit));

	AddMessageDelegate(system.AddRaw(MessageSystem::SYSTEM_SHUTDOWN,
		this, &FGsSystemMessageHandlerGlobal::OnSystemShutdown));

	AddMessageDelegate(system.AddRaw(MessageSystem::ENTER_INGAME,
		this, &FGsSystemMessageHandlerGlobal::OnLoadTopoLobbyToGame));

	AddMessageDelegate(system.AddRaw(MessageSystem::BACKTO_LOGIN,
		this, &FGsSystemMessageHandlerGlobal::RecvBackToLogin));

	AddMessageDelegate(system.AddRaw(MessageSystem::BACKTO_LOBBY,
		this, &FGsSystemMessageHandlerGlobal::RecvBackToLobby));

	AddMessageDelegate(system.AddRaw(MessageSystem::SHOW_DEVELOP_MESSAGE,
		this, &FGsSystemMessageHandlerGlobal::ShowTrayTickerMsgDeveloping));

	// 인증서버 접속 성공
	AddMessageDelegate(system.AddRaw(MessageSystem::AUTH_SERVER_CONNECTION_SUCCESS,
		this, &FGsSystemMessageHandlerGlobal::OnAuthServerConnected));
	
	// 인증서버 접속 실패
	AddMessageDelegate(system.AddRaw(MessageSystem::AUTH_SERVER_CONNECTION_FAIL,
		this, &FGsSystemMessageHandlerGlobal::OnAuthServerConnectFailed));
	// 인증서버 접속 종료
	AddMessageDelegate(system.AddRaw(MessageSystem::AUTH_SERVER_DISCONNECTION,
		this, &FGsSystemMessageHandlerGlobal::OnAuthServerDisconected));

	// 게임서버 접속 성공
	AddMessageDelegate(system.AddRaw(MessageSystem::GAME_SERVER_CONNECTION_SUCCESS,
		this, &FGsSystemMessageHandlerGlobal::OnGameServerConnected));
	// 게임서버 접속 실패
	AddMessageDelegate(system.AddRaw(MessageSystem::GAME_SERVER_CONNECTION_FAIL,
		this, &FGsSystemMessageHandlerGlobal::OnGameServerConectedFailed));
	// 게임서버 접속 종료
	AddMessageDelegate(system.AddRaw(MessageSystem::GAME_SERVER_DISCONNECTION,
		this, &FGsSystemMessageHandlerGlobal::OnGameServerDisconected));

	// 재연결 연결 성공
	AddMessageDelegate(system.AddRaw(MessageSystem::GAME_SERVER_RECONNECTION_SUCCESS,
		this, &FGsSystemMessageHandlerGlobal::OnGameServerReconnectionSuccess));

	/*
	  bak1210 : PVP 컨텐츠 M1에서 사용하던 코드로 실제 사용사례 없음 
	  삭제대기 
	*/
	// 게임오브젝트 객체 전체삭제
	AddMessageDelegate(system.AddRaw(MessageSystem::GAMEOBJECT_REMOVE_ALL,
		this, &FGsSystemMessageHandlerGlobal::OnRemoveAllGameObject));

	// LoadTopo 시작

	FGsMessageHolder* msg = GMessage();
	_systemMsg.Emplace(
		msg->GetSystemParam().AddRaw(MessageSystem::LOAD_TOPO_START, this,
			&FGsSystemMessageHandlerGlobal::OnLoadTopoStart));

	// 만약 워프 연출을 요청했다면
	AddMessageDelegate(system.AddRaw(MessageSystem::LOAD_TOPO_REQUEST_WARP,
		this, &FGsSystemMessageHandlerGlobal::OnLoadTopoRequestWarpEffect));

	// 워프연출이 끝났을 때
	AddMessageDelegate(system.AddRaw(MessageSystem::LOAD_TOPO_END_WARP_EFFECT,
		this, &FGsSystemMessageHandlerGlobal::OnLoadTopoEndWarpEffect));

	// 스폰 시작 시
	AddMessageDelegate(system.AddRaw(MessageSystem::LOAD_TOPO_START_SPAWN,
		this, &FGsSystemMessageHandlerGlobal::OnLoadTopoStartSpawn));

	// 맵 전환 완료 패킷 보낸 직후
	AddMessageDelegate(system.AddRaw(MessageSystem::AFTER_LOAD_TOPO_FIN,
		this, &FGsSystemMessageHandlerGlobal::OnAfterLoadTopoFin));

	// 게임모드에서 StartPlayEvent
	AddMessageDelegate(system.AddRaw(MessageSystem::GAMEMODE_WORLD_START_PLAY,
		this, &FGsSystemMessageHandlerGlobal::OnGameModeWorldStartPlay));
	
}

void FGsSystemMessageHandlerGlobal::FinalizeMessage()
{
	FinalizeMessageDelegateList();

	if (FGsMessageHolder* msg = GMessage())
	{
		if (_systemMsg.Num() != 0)
		{
			for (auto iter : _systemMsg)
			{
				msg->GetSystemParam().Remove(iter);
			}
			_systemMsg.Empty();
		}
	}
}

void FGsSystemMessageHandlerGlobal::OnSystemInit()
{
	UGsGameObjectManager::SystemInit();

	// FGsInitSmokeSignal().Send();
}

void FGsSystemMessageHandlerGlobal::OnSystemShutdown()
{
	UGsGameObjectManager::SystemShutdown();
}

//로비 진입연출관련 플레그를 초기화한다.
void FGsSystemMessageHandlerGlobal::ClearFlagLobbyToGame()
{
	_enterGameFadeOutComplate = _lobbyToGameLoadTopoReceve = false;

	_isPlayerRequestWarp = false;
}

void FGsSystemMessageHandlerGlobal::CompleteEnterGameFadeOut()
{
	_enterGameFadeOutComplate = true;
	if (_enterGameFadeOutComplate && _lobbyToGameLoadTopoReceve)
	{
		OnLoadTopoEndFadeOut();
	}
}

void FGsSystemMessageHandlerGlobal::RecvLobbyToGameLoadTopo()
{
	_lobbyToGameLoadTopoReceve = true;
	if (_enterGameFadeOutComplate && _lobbyToGameLoadTopoReceve)
	{
		OnLoadTopoEndFadeOut();
	}
}

void FGsSystemMessageHandlerGlobal::RecvBackToLogin()
{
	GNet()->SetReturnToStageType(static_cast<int32>(MessageSystem::BACKTO_LOGIN));

	ProcessBackToTitle();
}

void FGsSystemMessageHandlerGlobal::RecvBackToLobby()
{
	// AGsGameModeLobby::StartPlay() 에서 캐릭터 선택으로 분기하기 위함
	GNet()->SetReturnToStageType(static_cast<int32>(MessageSystem::BACKTO_LOBBY));
	
	GUI()->SetHideFlags(EGsUIHideFlags::STATE_MOVE_EFFECT);

	// [B1] | ejrrb10 | 로비로 이동할 때는 플레이어의 워프 출력 여부를 꺼 주어야 한다
	if (UGsGlobalGameObjects* globalGameObj = GGamObjects())
	{
		if (UGsGameObjectLocalPlayer* local = globalGameObj->GetLocalPlayer())
		{
			local->SetIsWarpEffectOn(false);
		}
	}

	ProcessBackToLobby();
}

void FGsSystemMessageHandlerGlobal::ProcessBackToTitle()
{
	if (UGsSkillManager* skillManager = GSkill())
	{
		skillManager->ClearData();
	}

	if (UGsGlobalGameObjects* globalGameObj = GGamObjects())
	{
		globalGameObj->RemoveLocalPlayer();
	}

	// 미니맵 아이콘 모두 삭제
	// 이건 게임 오브젝트에서 호출하면 얘도 한다
	GMessage()->GetGameObject().SendMessage(MessageGameObject::MINIMAP_DESPAWN_ALL_ICON, nullptr);

	if (UGsGameObjectManager* objManager = GSGameObject())
	{
		objManager->DespawnAllObject();
	}

	if (UGsLevelManager* levelManager = GLevel())
	{
		levelManager->BackToLauncher();
	}

	GScope()->BeginDestroyGameModeWorld();
}

void FGsSystemMessageHandlerGlobal::ProcessBackToLobby()
{
	// FadeOut 연출 시작
	TWeakObjectPtr<UGsUIWidgetBase> widget = GUI()->OpenAndGetWidget(TEXT("TrayFadeEffect"));
	if (false == widget.IsValid())
	{
		return;
	}

	UGsUITrayFadeEffect* fadeEffect = Cast<UGsUITrayFadeEffect>(widget.Get());
	if (nullptr == fadeEffect)
	{
		return;
	}
	
	// tray movie 파란 포털 동영상에서
	// 검은 바탕 tray loading으로 바뀌어서 검은색으로 변경
	FLinearColor fadeColor = GData()->GetGlobalData()->_warpTrayFadeEffectColor;
	fadeEffect->SetFadeAnimation(UGsUITrayFadeEffect::EGsFadeAnimationType::FadeOut, fadeColor, 0.2f);
	fadeEffect->SetAnimationFinishedCallback([this]()
		{
			if (UGsGlobalGameObjects* globalGameObj = GGamObjects())
			{
				if (UGsGameObjectLocalPlayer* local = globalGameObj->GetLocalPlayer())
				{
					local->SetIsWarpEffectOn(true);
				}
				globalGameObj->RemoveLocalPlayer();
			}

			// 미니맵 아이콘 모두 삭제
			// 이건 게임 오브젝트에서 호출하면 얘도 한다
			GMessage()->GetGameObject().SendMessage(MessageGameObject::MINIMAP_DESPAWN_ALL_ICON, nullptr);

			if (UGsGameObjectManager* objManager = GSGameObject())
			{
				objManager->DespawnAllObject();
			}
			if (UGsLevelManager* levelManager = GLevel())
			{
				levelManager->BackToLobby();
			}

			//AI 매니저는 Game 매니저에 있다. Skill Manager는 Global Manager에 있다.
			//AI 매니저 Finalize 호출 전에 Skill Manager 클리어 해줘야한다.
			if (UGsSkillManager* skillManager = GSkill())
			{
				skillManager->ClearData();
			}

			GScope()->BeginDestroyGameModeWorld();
		});
}

void FGsSystemMessageHandlerGlobal::ShowTrayTickerMsgDeveloping()
{
	// TEXT: <img id="IconAlert"></>  업데이트 준비중입니다.
	FText text;
	FText::FindText(TEXT("UICommonText"), TEXT("PrepareToUpdate"), text);
	FGsUIHelper::TrayMessageTicker(text);
}

void FGsSystemMessageHandlerGlobal::OnAuthServerConnected()
{
	GSLOG(Warning, TEXT("OnAuthServerConnected"));
	FGsUIHelper::HideBlockUI();
	_showNetErrPopup = false;
	_authServerConneted = true;	
}

void FGsSystemMessageHandlerGlobal::OnAuthServerDisconected()
{
	FGsUIHelper::HideBlockUI();
	GSLOG(Warning, TEXT("OnAuthServerDisconected"));

	FText errorText;
	FText::FindText(TEXT("LauncherNetText"), TEXT("AuthServerDisconected"), errorText);
	_authServerConneted = false;
		
	GPopupSystem()->PopupSystemMsg(errorText, []()
		{
			GMessage()->GetSystem().SendMessage(MessageSystem::RESTART_GAME);
		}, EPopupType::BackToTitle);
}

void FGsSystemMessageHandlerGlobal::OnAuthServerConnectFailed()
{
	FGsUIHelper::HideBlockUI();
	GSLOG(Warning, TEXT("OnAuthServerConnectFailed"));

	FText errorText;
	FText::FindText(TEXT("LauncherNetText"), TEXT("AuthServerConnectFailed"), errorText);

	GPopupSystem()->PopupSystemMsg(errorText, []()
		{
			GMessage()->GetSystem().SendMessage(MessageSystem::RESTART_GAME);
		}, EPopupType::BackToTitle);
}

void FGsSystemMessageHandlerGlobal::OnGameServerConnected()
{
	GSLOG(Warning, TEXT("OnGameServerConnected"));
}

void FGsSystemMessageHandlerGlobal::OnGameServerDisconected()
{
	FGsUIHelper::HideBlockUI();
}

void FGsSystemMessageHandlerGlobal::OnGameServerConectedFailed()
{
	GSLOG(Warning, TEXT("OnGameServerConectedFailed"));
}

void FGsSystemMessageHandlerGlobal::OnGameServerReconnectionSuccess()
{		
#if !REVIVE_RECONNECT_TEST
	// Hide 되었던 UI는 다 켜준다. 
	// 필요 시 각자의 State 혹은 Stage에서 꺼줄 것.(예: 결과창 상태라 Hide되어야 할 때)
	if (UGsUIManager* uiManager = GUI())
	{
		uiManager->ClearHideFlags(EGsUIHideFlags::UI_HIDE_ALL);
		uiManager->ResetUIByReconnection();
	}
#endif

	// FlowManager에 메시지 전송
	GMessage()->GetSystem().SendMessage(MessageSystem::RECONNECT_END);
}

// 시스템적으로 모든 객체를 지워야 되는 상황에 활용한다.
void FGsSystemMessageHandlerGlobal::OnRemoveAllGameObject()
{
	// 미니맵 아이콘 모두 삭제
	GMessage()->GetGameObject().SendMessage(MessageGameObject::MINIMAP_DESPAWN_OBJ_ALL_ICON, nullptr);
}

void FGsSystemMessageHandlerGlobal::OnLoadTopoStart(const IGsMessageParam* inParam)
{	
	GSLOG(Error, TEXT("OnLoadTopoStart start"));
	// 로비->인게임 시에는 Warp, Fade 연출을 사용하지 않음(맵 전환 과정에서 지저분하게 보임)
	// 로비->인게임 시에는 GameFlow가 인게임 판정나므로 FGsGameFlow::Mode::LOBBY가 아닌 GameMode로 검사
	if (UWorld* world = GLevel()->GetWorld())
	{
		if (AGameModeBase* gameMode = world->GetAuthGameMode())
		{
			if (gameMode->IsA<AGsGameModeLobby>())
			{
				//bak1210 
				RecvLobbyToGameLoadTopo();
				return;
			}
		}
	}

	// 워프전 모든 stack된 ui 닫기(페어리, 뽑기 연출, window 등)
	GUI()->CloseAllStack();
	// Tray제외한 각종UI Hide 처리
	GUI()->SetHideFlags(EGsUIHideFlags::STATE_MOVE_EFFECT);

	GSkill()->LoadTopoStart();

	// 소환 연출중이면 닫기

	// on warp 시점(서버 social action 패킷)
	// 에서 먼저 처리함 -> 재연결시에는 저기 안들어옴
	// 다시 여기서도 처리
	FGsSummonHandler* summonHandler = UGsSummonManager::GetSummonHandler();
	if (summonHandler == nullptr)
	{
		return;
	}
	summonHandler->FinishSummonForced();
	//if (summonHandler->GetIsSummonPlay() == true)
	//{
	//	summonHandler->EndSummon();		
	//}


	//
	//// R3
	//// COMPOSE -> SUMMON ROOM -> RECONNECT -> REMAIN TRAY SUMMON
	//// https://jira.com2us.com/jira/browse/C2URWQ-1113
	//if (true == GUI()->IsActiveWidget("TraySummon"))
	//{
	//	GUI()->CloseByKeyName("TraySummon", true);
	//}

	GSEventProgress()->ClearAllData();

	// 대화 인터랙션 타겟 npc도착후 서버 send 전에 워프류 아이템 사용 체크하는값이
	// 클리어 안되는 경우가 있어서 여기서 한번 클리어한다
	// 그래도 문제가 생기면 워프류 아이템 send 시 실패패킷에서 클리어 해야함
	// https://jira.com2us.com/jira/browse/C2URWQ-5584
	if (FGsInteractionManager* interactionManager = GSInteraction())
	{
		interactionManager->ClearCurrentConsumeCategory();
	}

	// 시네마틱 플레이 중이면?
	if (true == UGsSequencePlayer::IsSequencePlaying())
	{
		// 시네마틱 플레이 중이라면 종료시킴
		// https://jira.com2us.com/jira/browse/C2URWQ-5739
		FGsMessageHolder* msgHolder = GMessage();
		if (msgHolder != nullptr)
		{
			msgHolder->GetSystemParam().SendMessage(MessageSystem::SEQUENCE_PLAYER_STOP_ONLY_PLAYER, nullptr);
		}
	}

	// 동일 맵, 동일 위치일 경우, 워프 연출을 사용하지 않음
	bool bPlayWarpSuccess = true;

	// [B1] | ejrrb10 | 동일 맵, 동일 위치와 관계 없이 연출 시작 위치를 SC_ACK_SOCIAL_ACTION 패킷 부로 옮김
	// TODO : 아래 PlayWarpEffect 는 주석처리해야함
	//if (EGsLoadTopoEffectType::SAME_MAP_SAME_POS != GLevel()->GetLoadTopoEffectType())
	//{
	//	bPlayWarpSuccess = PlayWarpEffect();
	//}

	UGsGameObjectManager* gameObjectManager = GSGameObject();
	if (gameObjectManager == nullptr)
	{
		bPlayWarpSuccess = false;
	}

	if (UGsGameObjectBase* local = gameObjectManager->FindObject(EGsGameObjectType::LocalPlayer))
	{
		if (FGsGameObjectStateManager* fsm = local->GetBaseFSM())
		{
			// 내 캐릭터가 죽어있는 경우 체크
			if (true == fsm->IsState(EGsStateBase::Dead))
			{
				bPlayWarpSuccess = false;
			}
		}
	}

	// 워프 연출이 없거나 실패할 경우엔 바로 Fade 처리 돌입
	if (false == bPlayWarpSuccess)
	{
		GSLOG(Log, TEXT("false == bPlayWarpSuccess"));

		// 카메라 현재 위치 저장
		// 크래쉬 널체크 추가
		// https://console.firebase.google.com/u/0/project/raon-65a0e/crashlytics/app/android:com.Gamevil.WorldofZenonia/issues/fb7054c0087fb6bb97462bf95c207ff9?time=last-seven-days&sessionEventKey=60997db8029500012dd5a0b5fdcc88e7_1539178747285539009
		// 시공의 틈새 컨텐츠에서는 백업하면 안된다
		// 관전 모드 저장 안함 or observer finish(warp before)
		if (GLevel()->IsSpaceCrackMapContents() == false &&
			GSObserver()->GetIsObserveStart() == false &&
			GSObserver()->GetIsObserveFinished() == false)
		{
			if (FGsCameraModeManager* camModeManager = GSCameraMode())
			{
				camModeManager->SaveBackUpTransform();
			}
		}

		ProcessLoadTopoFadeOut(bPlayWarpSuccess);
	}
	else if (false == _isPlayerRequestWarp)
	{
		GSLOG(Log, TEXT("false == _isPlayerRequestWarp"));

		// [B1] | ejrrb10 | 네트워크 재연결 시, 재접 처리를 해 주어야 함 (testDisconnect)
		// 플레이어가 직접적으로 워프를 요청하지 않은 경우 (카메라 처리가 없는 경우)
		ProcessLoadTopoFadeOut(false);
	}
	else
	{
		GSLOG(Log, TEXT("false == bPlayWarpSuccess && false == bPlayWarpSuccess"));

		if (_isDirtyWarpStateFinished == true)
		{
			_isDirtyWarpStateFinished = false;
			OnLoadTopoEndFadeOut();
		}
		else
		{
			if (_isDirtyLoadTopoStart == false)
			{
				_isDirtyLoadTopoStart = true;
			}
		}
	}

	// [B1] | himichani| 여기서 바로 캐릭터 삭제 하면 연출 구간 확보가 안됨 
	//OnLoadTopoEndFadeOut();

	_isPlayerRequestWarp = false;

	GSLOG(Error, TEXT("OnLoadTopoStart end"));
}

void FGsSystemMessageHandlerGlobal::OnLoadTopoRequestWarpEffect()
{
	_isPlayerRequestWarp = true;
}

void FGsSystemMessageHandlerGlobal::OnLoadTopoEndWarpEffect()
{
	GScope()->AddPostUpdateFunc(EGsPostUpdateFuncType::PROCESS_LOAD_TO_PO_FADE_OUT,
		[this]()
	{
		ProcessLoadTopoFadeOut();
	});
}

void FGsSystemMessageHandlerGlobal::OnLoadTopoEndFadeOut()
{
	GSLOG(Error, TEXT("OnLoadTopoEndFadeOut"));

	UGsLevelManager* levelMgr = GLevel();
	if (nullptr == levelMgr)
	{
		return;
	}

	if (UWorld* world = levelMgr->GetWorld())
	{
		if (AGameModeBase* gameMode = world->GetAuthGameMode())
		{
			if (!gameMode->IsA<AGsGameModeLobby>())
			{
				bool isDiffrentMapGoing = levelMgr->IsWarpDataDifferentMap();
				ClearAllObjectLoadTopo(isDiffrentMapGoing);
			}
		}
	}
	
	levelMgr->LoadLevelByWarp();
}

void FGsSystemMessageHandlerGlobal::OnLoadTopoStartSpawn()
{
	GMessage()->GetGameObject().SendMessage(MessageGameObject::LOCAL_SPAWN_END_WARP, nullptr);

	// FadeIn 연출 시작
	TWeakObjectPtr<UGsUIWidgetBase> widget = GUI()->OpenAndGetWidget(TEXT("TrayFadeEffect"));
	if (widget.IsValid())
	{
		if (UGsUITrayFadeEffect* fadeEffect = Cast<UGsUITrayFadeEffect>(widget.Get()))
		{
			// tray movie 파란 포털 동영상에서
			// 검은 바탕 tray loading으로 바뀌어서 검은색으로 변경
			FLinearColor fadeColor =
				GData()->GetGlobalData()->_warpTrayFadeEffectColor;
			fadeEffect->SetFadeAnimation(UGsUITrayFadeEffect::EGsFadeAnimationType::FadeIn, fadeColor);
			fadeEffect->SetAnimationFinishedCallback([fadeEffect]()
			{
				// Fade UI 닫기
				fadeEffect->Close();
				GMessage()->GetGameObject().SendMessage(MessageGameObject::LOCAL_SPAWN_FADE_IN_END, nullptr);
			});
		}
		else
		{
#if !UE_BUILD_SHIPPING
			FString errorMsg = FString::Printf(TEXT("[OnLoadTopoStartSpawn] No Casted TrayFadeEffect \n"));
			GEngine->AddOnScreenDebugMessage(42, 5.f, FColor::Red, errorMsg);
#endif
		}
	}
	else
	{
#if !UE_BUILD_SHIPPING
		FString errorMsg = FString::Printf(TEXT("[OnLoadTopoStartSpawn] No TrayFadeEffect \n"));
		GEngine->AddOnScreenDebugMessage(42, 5.f, FColor::Red, errorMsg);
#endif
	}

	// HIDE 풀어주기. 사실 플레이어가 스폰되며 UI_HIDE_ALL로 클리어해서 다 풀려있을테지만 안전을 위해 삽입
	GUI()->ClearHideFlags(EGsUIHideFlags::STATE_MOVE_EFFECT);
}

void FGsSystemMessageHandlerGlobal::OnAfterLoadTopoFin()
{

}

bool FGsSystemMessageHandlerGlobal::PlayWarpEffect()
{
	UGsGameObjectManager* gameObjectManager = GSGameObject();
	if (gameObjectManager == nullptr)
	{
		return false;
	}

	if (UGsGameObjectBase* local = gameObjectManager->FindObject(EGsGameObjectType::LocalPlayer))
	{
		if (FGsGameObjectStateManager* fsm = local->GetBaseFSM())
		{
			// 내 캐릭터가 살아있을 경우에만 워프 연출
			if (false == fsm->IsState(EGsStateBase::Dead))
			{
				if (UGsGameObjectPlayer* localPlayer = Cast<UGsGameObjectPlayer>(local))
				{
					localPlayer->OnWarp();
				}

				return true;
			}
		}
	}

	// 모든 예외 상황에 사용안함
	return false;
}

void FGsSystemMessageHandlerGlobal::ClearAllObjectLoadTopo(bool In_isDiffrentMap)
{
	// 미니맵 아이콘 모두 삭제
	// 이건 게임 오브젝트에서 호출하면 얘도 한다

	// 다른맵이면 모든 아이콘을 지우고
	// 같은 맵이면 obj icon(서버 내려주는 데이터로 생성되는 아이콘)만 지운다
	MessageGameObject sendMessage = (In_isDiffrentMap == true) ?
		MessageGameObject::MINIMAP_DESPAWN_ALL_ICON :
		MessageGameObject::MINIMAP_DESPAWN_OBJ_ALL_ICON;
	GMessage()->GetGameObject().SendMessage(sendMessage, nullptr);
	
	if (GSGameObject())
	{
		GSGameObject()->DespawnAllObject(false);
	}

	if (GSClientSpawn())
	{
		GSClientSpawn()->DespawnAll();
	}
}

void FGsSystemMessageHandlerGlobal::ProcessLoadTopoFadeOut(bool bPlayWarpSuccess)
{
	UGsUIManager* uiMgr = GUI();
	if (nullptr == uiMgr)
	{
		OnLoadTopoEndFadeOut();
		return;
	}

	// 암전 시, 기존 피격 HUD 효과가 있으면 꺼 준다
	if (uiMgr->IsActiveWidget(TEXT("TrayLowHP")))
	{
		uiMgr->CloseByKeyName(TEXT("TrayLowHP"));
	}

	// FadeOut 연출 시작
	TWeakObjectPtr<UGsUIWidgetBase> widget = uiMgr->OpenAndGetWidget(TEXT("TrayFadeEffect"));
	if (widget.IsValid())
	{
		if (UGsUITrayFadeEffect* fadeEffect = Cast<UGsUITrayFadeEffect>(widget.Get()))
		{
			// tray movie 파란 포털 동영상에서
			// 검은 바탕 tray loading으로 바뀌어서 검은색으로 변경
			FLinearColor fadeColor = GData()->GetGlobalData()->_warpTrayFadeEffectColor;

			// [B1] | ejrrb10
			// 현재 로컬 플레이어의 Warp 관련 SkillCommon 의 totalRuntime 에 맞게 페이드 아웃 지연을 걸어준다
			// 페이드 아웃 시간이 있어서 약간의 offset 을 걸어준다
			//float fadeOutDelay = 0.f;
			//if (UGsGameObjectManager* mgr = GSGameObject())
			//{
			//	if (UGsGameObjectBase* local = mgr->FindObject(EGsGameObjectType::LocalPlayer))
			//	{
			//		const FGsSchemaSkillCommon* commonData = GSkill()->GetPlayerCommonActionData(local, CommonActionType::TELEPORT);
			//		if (nullptr != commonData)
			//		{
			//			fadeOutDelay = commonData->totalRuntime;
			//		}
			//		else
			//		{
			//			GSLOG(Error, TEXT("Montage for Player's Warp Animation is not set!"));
			//		}
			//	}
			//}
			//float fadeoutTime = 0.2f;

			//// 모종의 이유로 워프 연출이 어색할 경우(죽음 등) 바로 페이드 아웃
			//if (false == bPlayWarpSuccess)
			//{
			//	fadeEffect->SetFadeAnimation(UGsUITrayFadeEffect::EGsFadeAnimationType::FadeOut, fadeColor, fadeoutTime);
			//}
			//else
			//{
			//	fadeEffect->SetFadeAnimationWithDelay(UGsUITrayFadeEffect::EGsFadeAnimationType::FadeOut, fadeColor, fadeoutTime, fadeOutDelay - fadeoutTime * 3);
			//}

			// [B1] | ejrrb10 | 연출과 맵 로딩을 아예 분리해야 한다. 
			// 맵이 전부 로딩되었다는 것을 서버에게 알려준 후에 서버가 SpawnMe 를 내려 줘야 한다.
			// 맵 로딩이 끝나고 나서야 PKT_CG_ACK_LOAD_TOPO_FIN_WRITE 을 보내주도록 수정했다.

			// [B1] | himichani| 연출이 끝나기도 전에 캐릭터 삭제해서
			// WARP STATE exit 되는 구간에서 LOAD_TOPO_END_WARP_EFFECT 메시지 보내서(이전에 zoom 끝날때 보내던 방식)
			// 처리로 다시 변경
			fadeEffect->SetFadeAnimation(UGsUITrayFadeEffect::EGsFadeAnimationType::FadeOut, fadeColor, 0.2f);
			fadeEffect->SetAnimationFinishedCallback([this, bPlayWarpSuccess]()
				{
					// dead state direct level load
					if (bPlayWarpSuccess == false)
					{
						OnLoadTopoEndFadeOut();
					}
					else
					{
						if (_isDirtyLoadTopoStart == true)
						{
							_isDirtyLoadTopoStart = false;
							OnLoadTopoEndFadeOut();
						}
						else
						{
							_isDirtyWarpStateFinished = true;
						}
					}
				});

		}
	}
	else
	{
		GSLOG(Error, TEXT("[ProcessLoadTopoFadeOut] TrayFadeEffect is invalid"));
		OnLoadTopoEndFadeOut();
	}
}

void FGsSystemMessageHandlerGlobal::OnGameModeWorldStartPlay()
{
	FBoxSphereBounds Levelbound;
	if (GLevel()->GetLevelBounds(Levelbound))
	{
		GSGameObject()->InitSphereTree(Levelbound.GetSphere().Center, Levelbound.GetSphere().W);
	}
}

// loadtopo 로비에서 게임진입
void FGsSystemMessageHandlerGlobal::OnLoadTopoLobbyToGame()
{
	ClearFlagLobbyToGame();
	TWeakObjectPtr<UGsUIWidgetBase> widget = GUI()->OpenAndGetWidget(TEXT("TrayFadeEffect"));
	if (widget.IsValid())
	{
		if (UGsUITrayFadeEffect* fadeEffect = Cast<UGsUITrayFadeEffect>(widget.Get()))
		{
			// tray movie 파란 포털 동영상에서
			// 검은 바탕 tray loading으로 바뀌어서 검은색으로 변경
			FLinearColor fadeColor =
				GData()->GetGlobalData()->_warpTrayFadeEffectColor;
			fadeEffect->SetFadeAnimation(UGsUITrayFadeEffect::EGsFadeAnimationType::FadeOut, fadeColor, 1.0f);
			fadeEffect->SetAnimationFinishedCallback([this](){
				CompleteEnterGameFadeOut();
				});
		}
	}
}