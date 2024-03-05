#include "GsObserverHandler.h"

#include "Management/GsMessageHolder.h"
#include "Management/ScopeGlobal/GsUIManager.h"
#include "Management/ScopeGame/GsGameObjectManager.h"
#include "Management/ScopeGame/GsObserverManager.h"
#include "Management/ScopeGame/GsMinimapManager.h"
#include "Management/ScopeGame/GsCameraModeManager.h"
#include "Management/ScopeGame/GsUnlockManager.h"

#include "GameObject/ObjectClass/GsGameObjectLocalPlayer.h"
#include "GameObject/ObjectClass/GsGameObjectBase.h"
#include "GameObject/Camera/GsCameraModeHandler.h"
#include "GameObject/Target/GsTargetHandlerLocalPlayer.h"

#include "ActorEx/GsCharacterLocalPlayer.h"

#include "Message/GsMessageGameObject.h"
#include "Message/GsMessageInput.h"
#include "Message/MessageParam/GsMessageParam.h"
#include "Message/MessageParam/GsGameObjectMessageParam.h"

#include "Map/GsMapIconType.h"
#include "Map/Minimap/GsMinimapFunc.h"

#include "UI/UILib/Define/GsUIDefine.h"
#include "UI/UIContent/Popup/GsUIPopupRevive.h"

#include "Input/GsInputEventMsgBase.h"
#include "Runtime/DataCenter/Public/KeyMapping/GsKeyMappingEnum.h"

#include "GameFramework/PlayerController.h"

#include "Engine/Classes/GameFramework/SpringArmComponent.h"

#include "Management/ScopeGlobal/GsGameFlowManager.h"
#include "GameFlow/GameContents/ContentsGame/GsContentsManagerGame.h"
#include "GameFlow/GsGameFlowGame.h"

// 초기화(최초 한번)
void FGsObserverHandler::Init()
{
	AddMessages();
}
// 정리(마지막 한번)
void FGsObserverHandler::Close()
{
	RemoveMessages();
	// 모드 끝날때 날리지만
	// 혹시 모르니 끝날때 날려보자
	RemoveMessgesObserverMode();
}
// 메시지 등록
void FGsObserverHandler::AddMessages()
{
	if (FGsMessageHolder* msg = GMessage())
	{
		_msgContensObserverDelegates.Emplace(
			msg->GetObserver().AddRaw(MessageContentObserver::START_OBSERVE,
				this, &FGsObserverHandler::OnStartObserve));

		_msgGuildDungeonDelegates.Emplace(
			msg->GetGuildDungeon().AddRaw(MessageContentGuildDungeon::CLEAR, this,
				&FGsObserverHandler::OnGuildDungeonClear));

		_msgSystemNoParamDelegates.Emplace(
			msg->GetSystem().AddRaw(MessageSystem::GAME_SERVER_RECONNECTION_SUCCESS,
				this, &FGsObserverHandler::OnGameServerReconnectionSuccess));
	}
}
// 메시지 해제
void FGsObserverHandler::RemoveMessages()
{
	if (FGsMessageHolder* msg = GMessage())
	{
		if (_msgContensObserverDelegates.Num() != 0)
		{
			for (auto iter : _msgContensObserverDelegates)
			{
				msg->GetObserver().Remove(iter);
			}
			_msgContensObserverDelegates.Empty();
		}

		if (_msgGuildDungeonDelegates.Num() != 0)
		{
			for (auto& iter : _msgGuildDungeonDelegates)
			{
				msg->GetGuildDungeon().Remove(iter);
			}
			_msgGuildDungeonDelegates.Empty();
		}

		if (_msgSystemNoParamDelegates.Num() != 0)
		{
			for (auto iter : _msgSystemNoParamDelegates)
			{
				msg->GetSystem().Remove(iter);
			}
			_msgSystemNoParamDelegates.Empty();
		}
	}
}

// 옵저버 모드 일때만 쓰는 메시지 등록
void FGsObserverHandler::AddMessgesObserverMode()
{
	if (FGsGameFlowManager* mode = GMode())
	{
		if (FGsGameFlowGame* gameFlow = mode->GetGameFlow())
		{
			if (FGsContentsManagerGame* contents = gameFlow->GetContentsManagerGame())
			{
				contents->OnEnterStateEvent().AddRaw(this, &FGsObserverHandler::OnContentsEnterStateEvent);
			}
		}
	}
}
// 옵저버 모드 일때만 쓰는 메시지 해제
void FGsObserverHandler::RemoveMessgesObserverMode()
{
	if (FGsGameFlowManager* mode = GMode())
	{
		if (FGsGameFlowGame* gameFlow = mode->GetGameFlow())
		{
			if (FGsContentsManagerGame* contents = gameFlow->GetContentsManagerGame())
			{
				contents->OnEnterStateEvent().RemoveAll(this);
			}
		}
	}
}

// 관전 시작
void FGsObserverHandler::StartObserve()
{
	// 메시지 등록
	AddMessgesObserverMode();

	TArray<UGsGameObjectBase*> findRemotes =
		GSGameObject()->FindObjectArray(EGsGameObjectType::RemotePlayer);

	if (findRemotes.Num() == 0)
	{
		// 먼가 이상하다 
		return;
	}
	
	// 리모트 obj 레벨 순으로 정렬(리스트 데이터)
	GSObserver()->SetAllRemoteToObserve(findRemotes);

	// 첫번째 gameobject를 타겟으로 변경
	TArray<UGsGameObjectBase*> sortedMembers = GSObserver()->GetPlayGuildDungeonMembers();

	if (sortedMembers.Num() == 0)
	{
		// 이상하다
		return;
	}
	// 제일 레벨 높은사람부터 관전
	UGsGameObjectBase* targetObserve = sortedMembers[0];
	if (targetObserve == nullptr)
	{
		return;
	}

	GSObserver()->SetIsObserveStart(true);

	// camera observer data reset
	GSCameraMode()->LoadObserverCamModeZoomAndPitch();
	
	ChangeTarget(targetObserve, true);

	// ui 세팅
	// 조이스틱 hide
	HideJoystick();

	// 로컬 사망시 인풋 막은거 해제
	//FGsInputEventMsgBase msg;
	//msg.Id = 0;
	FGsInputEventMsgFlag msg(EGsKeyMappingInputFlags::SETTING_OBSERVE, true);
	GMessage()->GetInput().SendMessage(MessageInput::BlockInput, msg);

	// 사용할 수 없는 메뉴 버튼들 비활성화
	GSUnlock()->UnlockContentsByClient(UnlockCategory::GUILD_DUNGEON);
	GSUnlock()->LockContentsByClient(UnlockCategory::GUILD_DUNGEON_WATCHING);

	if (UGsUIManager* uiManager = GUI())
	{
		// 스킬, 퀵슬롯, 퀘스트 hide()
		// HUD 갱신
		uiManager->SetHUDMode(EGsUIHUDMode::NORMAL);

		// 체력 모자른 ui 끄기
		if (uiManager->IsActiveWidget(TEXT("TrayLowHP")))
		{
			// 점등되지 않았다면 return
			uiManager->CloseByKeyName(TEXT("TrayLowHP"));
		}
		// tray observe info 키기
		if (uiManager->IsActiveWidget(TEXT("TrayObserveInfo"))== false)
		{
			uiManager->Open(TEXT("TrayObserveInfo"));
		}
		// tray observe target list 키기
		if (uiManager->IsActiveWidget(TEXT("TrayObservTargetList")) == false)
		{
			uiManager->Open(TEXT("TrayObservTargetList"));
		}

		// 사망으로 Hide된 UI를 켜준다.
		uiManager->ClearHideFlags(EGsUIHideFlags::UI_HIDE_ALL);
	}

	// 타겟 리스트에게 메시지 전송
	GMessage()->GetObserver().SendMessage(MessageContentObserver::CHANGE_OBSERVE_TARGET_LIST, nullptr);

	// 관전 타겟 변경 메시지 전송
	GMessage()->GetObserver().SendMessage(MessageContentObserver::CHANGE_OBSERVE_TARGET, nullptr);
}


// 관전 종료
void FGsObserverHandler::EndObserve()
{
	GSObserver()->SetIsObserveStart(false);
	GSObserver()->SetIsObserveFinished(true);

	// 비활성화 시켰던 메뉴 버튼들 활성화
	GSUnlock()->UnlockContentsByClient(UnlockCategory::GUILD_DUNGEON_WATCHING);

	if (UGsUIManager* uiManager = GUI())
	{
		// tray observe info 끄기
		if (uiManager->IsActiveWidget(TEXT("TrayObserveInfo")) == true)
		{
			uiManager->CloseByKeyName(TEXT("TrayObserveInfo"));
		}
		// tray observe target list 끄기
		if (uiManager->IsActiveWidget(TEXT("TrayObservTargetList")) == true)
		{
			uiManager->CloseByKeyName(TEXT("TrayObservTargetList"));
		}


		// HUD MODE normal로 돌아감
		uiManager->SetHUDMode(EGsUIHUDMode::NORMAL);
		uiManager->ClearHideFlags(EGsUIHideFlags::UI_HIDE_ALL);
		//uiManager->SetHideFlags(EGsUIHideFlags::STATE_DIE);

	}
	UGsGameObjectBase* findLocal =
		GSGameObject()->FindObject(EGsGameObjectType::LocalPlayer);
	// 로컬로 타겟 변경
	ChangeTarget(findLocal, false);

	// dead state block input rollback
	//FGsInputEventMsgBase msg;
	//msg.Id = 1;
	FGsInputEventMsgFlag msg(EGsKeyMappingInputFlags::SETTING_OBSERVE, false);
	GMessage()->GetInput().SendMessage(MessageInput::BlockInput, msg);

	RemoveMessgesObserverMode();
}
// 타겟 변경
void FGsObserverHandler::ChangeTarget(UGsGameObjectBase* In_target, bool In_isStart)
{
	// 이전 타겟
	UGsGameObjectBase* oldTargetObserve = GSObserver()->GetTargetObserve();
	// 타겟 저장
	GSObserver()->SetTargetObserve(In_target);

	// 다음 타겟이 nullptr이면 더 할필요 없음
	if (In_target == nullptr)
	{
		return;
	}

	UGsGameObjectBase* findLocal =
		GSGameObject()->FindObject(EGsGameObjectType::LocalPlayer);

	// 스프링암 이동
	MoveSpringArmToTarget(findLocal, In_target);

	// 카메라 모드
	ChangeCameraModeTarget(findLocal, In_target, In_isStart);
	// 미니맵
	ChangeMinimapLocal(findLocal, In_target, oldTargetObserve,In_isStart);

	// if target is local, clear hud target ui
	if (In_target == findLocal)
	{
		ClearHUDTargetInfo(findLocal);
	}
	else
	{
		// 타겟 ui에 관전 대상 hp 표시
		ChangeHUDTargetInfo(findLocal, In_target);
	}
}
// 스프링암을 이동시킨다
void FGsObserverHandler::MoveSpringArmToTarget(UGsGameObjectBase* In_local, UGsGameObjectBase* In_target)
{
	// 로컬의 spring arm(카메라 상위 컴퍼넌트)을 다른 대상 root component에 붙인다
	if (In_target == nullptr)
	{
		return;
	}


	if (In_local == nullptr)
	{
		return;
	}

	AGsCharacterLocalPlayer* localChar = Cast<AGsCharacterLocalPlayer>(In_local->GetCharacter());

	if (localChar == nullptr)
	{
		return;
	}

	USpringArmComponent* springArm = localChar->GetSpringArm();

	if (springArm == nullptr)
	{
		return;
	}

	AGsCharacterBase* targetChar = In_target->GetCharacter();

	if (nullptr == targetChar)
	{
		return;
	}

	USceneComponent* targetRoot = targetChar->GetRootComponent();

	if (targetRoot == nullptr)
	{
		return;
	}

	springArm->AttachToComponent(targetRoot, FAttachmentTransformRules::KeepRelativeTransform);

	// 회전, 위치 초기화(기본값)
	springArm->SetRelativeRotation(FRotator::ZeroRotator);
	springArm->SetRelativeLocation(FVector::ZeroVector);
}

// 관전 시작 메시지
void FGsObserverHandler::OnStartObserve(const IGsMessageParam* In_param)
{
	StartObserve();
}

// 카메라 모드 타겟을 변경한다
void FGsObserverHandler::ChangeCameraModeTarget(UGsGameObjectBase* In_local, UGsGameObjectBase* In_target, bool In_isStart)
{
	if (In_local == nullptr)
	{
		return;
	}
	UGsGameObjectLocalPlayer* localObj = Cast<UGsGameObjectLocalPlayer>(In_local);
	if (nullptr == localObj)
	{
		return;
	}
	FGsCameraModeHandler* cameraModehandler = localObj->GetCameraModeHandler();
	if (nullptr == cameraModehandler)
	{
		return;
	}

	cameraModehandler->ChangeModeTargetObj(In_target, In_isStart);
}

// 미니맵의 로컬(중심 표시) 변경
void FGsObserverHandler::ChangeMinimapLocal(UGsGameObjectBase* In_local, UGsGameObjectBase* In_newTarget,
	UGsGameObjectBase* In_oldTarget, bool In_isStart)
{
	if (In_local == nullptr || In_newTarget == nullptr)
	{
		return;
	}

	GSMinimap()->SetCenterObject(In_newTarget);

	// 기존꺼를 삭제
	// 1. 최초면 지금 local의 아이콘 삭제
	// 2. 기존에 다른 타겟을 바라보고 있었다면 바라보고 있던 아이콘 삭제

	int64 delGameId = -1;
	if (true == In_isStart)
	{
		// 로컬의 gameid
		delGameId = In_local->GetGameId();
	}
	else
	{
		if (In_oldTarget != nullptr)
		{
			delGameId = In_oldTarget->GetGameId();
		}
	}
	if (delGameId != -1)
	{
		FGsGameObjectMessageParamMapIcon paramIcon(delGameId);
		GMessage()->GetGameObject().SendMessage(MessageGameObject::MINIMAP_DESPAWN_ICON, &paramIcon);
	}
	// 타겟 변경시 
	// 기존 아이콘으로 다시 생성
	if (false == In_isStart &&
		In_oldTarget != nullptr)
	{
		EGsMapIconType findIconType = EGsMapIconType::RemoteTeamRed;
		bool isPartyMember = FGsMinimapFunc::GetPartyIconType(delGameId, findIconType);
		// 파티가 아니면 적군, 아군인지
		if (isPartyMember == false)
		{
			UGsGameObjectLocalPlayer* localObj = Cast<UGsGameObjectLocalPlayer>(In_local);
			if (nullptr == localObj)
			{
				return;
			}

			findIconType = (localObj->IsEnemy(In_oldTarget)) ?
				EGsMapIconType::RemoteTeamRed : EGsMapIconType::RemoteTeamBlue;
		}
		FGsGameObjectMessageParamMapIcon paramIcon(In_oldTarget, findIconType, isPartyMember);
		GMessage()->GetGameObject().SendMessage(MessageGameObject::MINIMAP_SPAWN_ICON, &paramIcon);
	}

	
	// 새로 로컬로 추가
	FGsGameObjectMessageParamMapIcon paramIcon(In_newTarget, EGsMapIconType::LocalPlayer);
	GMessage()->GetGameObject().SendMessage(MessageGameObject::MINIMAP_SPAWN_ICON, &paramIcon);

}

// 조이스틱 보이게
void FGsObserverHandler::ShowJoystick()
{
	UWorld* world = GSGameObject()->GetWorld();
	if (world == nullptr)
	{
		return;
	}

	if (APlayerController* playerController = world->GetFirstPlayerController())
	{
		playerController->CreateTouchInterface();
	}
}
// 조이스틱 안보이게
void FGsObserverHandler::HideJoystick()
{
	UWorld* world = GSGameObject()->GetWorld();
	if (world == nullptr)
	{
		return;
	}
	//  조이스틱 안보이게 처리
	if (APlayerController* playerController = world->GetFirstPlayerController())
	{
		playerController->ActivateTouchInterface(nullptr);
	}
}

// 타겟 변경 클릭
void FGsObserverHandler::OnClickTargetChangeSide(bool In_isRight)
{
	UGsGameObjectBase* nextTarget = GSObserver()->GetSideTarget(In_isRight);

	// 관전 타겟이 없거나 1명일때
	if (nextTarget == nullptr)
	{
		return;
	}

	ChangeTarget(nextTarget, false);

	// 관전 타겟 변경 메시지 전송
	GMessage()->GetObserver().SendMessage(MessageContentObserver::CHANGE_OBSERVE_TARGET, nullptr);
}
// 타겟 변경 클릭(아이템)
void FGsObserverHandler::OnClickTargetChangeItem(UGsGameObjectBase* In_target)
{
	if (In_target == nullptr)
	{
		return;
	}

	// 이전 타겟
	UGsGameObjectBase* nowTargetObserve = GSObserver()->GetTargetObserve();
	// 이미 같은거면 안함
	if (In_target == nowTargetObserve)
	{
		return;
	}

	ChangeTarget(In_target, false);

	// 관전 타겟 변경 메시지 전송
	GMessage()->GetObserver().SendMessage(MessageContentObserver::CHANGE_OBSERVE_TARGET, nullptr);
}

// hud 타겟 정보에 관전 타겟정보 넣기
void FGsObserverHandler::ChangeHUDTargetInfo(UGsGameObjectBase* In_local, UGsGameObjectBase* In_target)
{
	if (In_local == nullptr)
	{
		return;
	}
	UGsGameObjectLocalPlayer* localObj = Cast<UGsGameObjectLocalPlayer>(In_local);
	if (nullptr == localObj)
	{
		return;
	}
	FGsTargetHandlerLocalPlayer* targetHandler = localObj->GetCastTarget<FGsTargetHandlerLocalPlayer>();
	if (nullptr == targetHandler)
	{
		return;
	}

	targetHandler->SetObserveTarget(In_target);
}
// clear hud target ui
void FGsObserverHandler::ClearHUDTargetInfo(UGsGameObjectBase* In_local)
{
	if (In_local == nullptr)
	{
		return;
	}
	UGsGameObjectLocalPlayer* localObj = Cast<UGsGameObjectLocalPlayer>(In_local);
	if (nullptr == localObj)
	{
		return;
	}
	FGsTargetHandlerLocalPlayer* targetHandler = localObj->GetCastTarget<FGsTargetHandlerLocalPlayer>();
	if (nullptr == targetHandler)
	{
		return;
	}
	targetHandler->ClearObserveTarget();
}

// 관전 타겟에서 삭제(디스폰, 사망)
void FGsObserverHandler::RemoveObserveTarget(UGsGameObjectBase* In_target)
{
	if (In_target == nullptr)
	{
		return;
	}

	// 관전 기존 타겟이랑 같으면 타겟 변경 처리
	UGsGameObjectBase* nowTargetObserve = GSObserver()->GetTargetObserve();
	if (nowTargetObserve == In_target)
	{
		UGsGameObjectBase* nextTarget = GSObserver()->GetSideTarget(true);		
		ChangeTarget(nextTarget, false);
	}

	// 리스트에서 삭제
	if (false == GSObserver()->RemoveObserveGameObject(In_target))
	{
		// 바뀐게 없으면 return
		return;
	}

	TArray<UGsGameObjectBase*> remainMembers =
		GSObserver()->GetPlayGuildDungeonMembers();

	// 남은 사람이 없으면
	if (remainMembers.Num() == 0)
	{
		EndObserve();

		if (UGsUIManager* uiManager = GUI())
		{		
			// 부활 팝업 open
			if (false == uiManager->IsActiveWidget(TEXT("PopupRevive")))
			{
				TWeakObjectPtr<UGsUIWidgetBase> widget = uiManager->OpenAndGetWidget(TEXT("PopupRevive"));
				if (widget.IsValid())
				{
					if (UGsUIPopupRevive* popupRevive = Cast<UGsUIPopupRevive>(widget.Get()))
					{
						popupRevive->ShowPopupLastPosition();
						popupRevive->ShowGuildDungeonKickRemainTimeText(true);
					}
				}
			}
			// 체력 모자른 ui 다시 켜기
			if (false == uiManager->IsActiveWidget(TEXT("TrayLowHP")))
			{
				uiManager->Open(TEXT("TrayLowHP"));
			}
		}		
	}
	else
	{
		// ui 갱신(리스트, info)
		// 타겟 리스트에게 메시지 전송
		GMessage()->GetObserver().SendMessage(MessageContentObserver::CHANGE_OBSERVE_TARGET_LIST, nullptr);

		// 관전 타겟 변경 메시지 전송
		GMessage()->GetObserver().SendMessage(MessageContentObserver::CHANGE_OBSERVE_TARGET, nullptr);
	}

	
}

// 관전 타겟에 추가(스폰)
void FGsObserverHandler::AddObserveTarget(UGsGameObjectBase* In_target)
{
	if (In_target == nullptr)
	{
		return;
	}

	// 리스트에 추가
	GSObserver()->AddObserveGameObject(In_target);
	
	// ui 갱신(리스트, info)
	// 타겟 리스트에게 메시지 전송
	GMessage()->GetObserver().SendMessage(MessageContentObserver::CHANGE_OBSERVE_TARGET_LIST, nullptr);

	// 관전 타겟 변경 메시지 전송
	GMessage()->GetObserver().SendMessage(MessageContentObserver::CHANGE_OBSERVE_TARGET, nullptr);
}

// bm shop open
void FGsObserverHandler::OnStateBMShop()
{
	// tray visible hidden으로 바꿈
	SetObserverUIVisibility(false);

}
// hud 복구(bm shop 닫힘)
void FGsObserverHandler::OnStateHud()
{
	// tray visible 원래대로 복구
	SetObserverUIVisibility(true);
}

// observer ui visible 세팅
void FGsObserverHandler::SetObserverUIVisibility(bool In_visible)
{

	ESlateVisibility changeVisible = (In_visible == true) ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Hidden;
	if (UGsUIManager* uiManager = GUI())
	{
		TWeakObjectPtr<UGsUIWidgetBase> widgetObserveInfo = uiManager->GetWidgetByKey(TEXT("TrayObserveInfo"));
		if (widgetObserveInfo.IsValid())
		{
			widgetObserveInfo->SetVisibility(changeVisible);
		}
		TWeakObjectPtr<UGsUIWidgetBase> widgetObserveTargetList = uiManager->GetWidgetByKey(TEXT("TrayObservTargetList"));
		if (widgetObserveTargetList.IsValid())
		{
			widgetObserveTargetList->SetVisibility(changeVisible);
		}
		
	}
}

// guild dungeon clear message
void FGsObserverHandler::OnGuildDungeonClear(const IGsMessageParam* In_param)
{
	// observe end
	EndObserve();
}

void FGsObserverHandler::OnContentsEnterStateEvent(FGsContentsMode::InGame InStateType)
{
	if (InStateType == FGsContentsMode::ContentsHud)
	{
		SetObserverUIVisibility(true);
	}
	else
	{
		SetObserverUIVisibility(false);
	}
}
// 재접속후 ui 끄기 처리
void FGsObserverHandler::OnGameServerReconnectionSuccess()
{
	if (GSObserver()->GetIsObserveStart() == false)
	{
		return;
	}

	if (UGsUIManager* uiManager = GUI())
	{
		// tray observe info 끄기
		if (uiManager->IsActiveWidget(TEXT("TrayObserveInfo")) == true)
		{
			uiManager->CloseByKeyName(TEXT("TrayObserveInfo"));
		}
		// tray observe target list 끄기
		if (uiManager->IsActiveWidget(TEXT("TrayObservTargetList")) == true)
		{
			uiManager->CloseByKeyName(TEXT("TrayObservTargetList"));
		}

		// HUD MODE normal로 돌아감
		uiManager->SetHUDMode(EGsUIHUDMode::NORMAL);
		uiManager->ClearHideFlags(EGsUIHideFlags::UI_HIDE_ALL);
	}

	//https://jira.com2us.com/jira/browse/C2URWQ-4947
	GSObserver()->SetIsObserveStart(false);
	GSObserver()->SetIsObserveFinished(true);

	// 비활성화 시켰던 메뉴 버튼들 활성화
	GSUnlock()->UnlockContentsByClient(UnlockCategory::GUILD_DUNGEON_WATCHING);

	// dead state block input rollback
	//FGsInputEventMsgBase msg;
	//msg.Id = 1;
	FGsInputEventMsgFlag msg(EGsKeyMappingInputFlags::SETTING_OBSERVE, false);
	GMessage()->GetInput().SendMessage(MessageInput::BlockInput, msg);

	RemoveMessgesObserverMode();
}