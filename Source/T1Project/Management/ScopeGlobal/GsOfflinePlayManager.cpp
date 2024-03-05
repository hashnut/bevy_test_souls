

#include "Management/ScopeGlobal/GsOfflinePlayManager.h"
#include "Management/GsScopeHolder.h"
#include "Management/GsMessageHolder.h"
#include "Management/GsScopeGame.h"
#include "Management/ScopeGlobal/GsUIManager.h"
#include "Management/ScopeGame/GsAIManager.h"
#include "Management/ScopeGlobal/GsNetManager.h"
#include "Management/ScopeGame/GsGameObjectManager.h"
#include "Management/ScopeGlobal/GsLevelManager.h"
#include "Management/ScopeGlobal/GsGameFlowManager.h"
#include "Management/ScopeGlobal/GsGameDataManager.h"
#include "../ScopeGame/GsUnlockManager.h"
#include "../ScopeGame/GsEventProgressManager.h"

#include "DataSchema/Map/GsSchemaMapData.h"

#include "GameFlow/GameContents/GsContentsMode.h"
#include "GameFlow/GsGameFlowGame.h"
#include "GameFlow/Stage/GsStageMode.h"
#include "GameFlow/Stage/StageGame/GsStageManagerGame.h"
#include "GameFlow/GameContents/ContentsGame/GsContentsManagerGame.h"
#include "GameFlow/GameContents/ContentsGame/GsGameStateDungeon.h"

#include "Message/GsMessageGameObject.h"
#include "Message/MessageParam/GsMessageParam.h"
#include "Message/MessageParam/GsUIMessageParam.h"

#include "EventProgress/GsEventProgressDefine.h"

#include "UI/UILib/Define/GsUIDefine.h"
#include "UI/UIContent/Popup/GsUIPopupOKTwoMessage.h"
#include "UI/UIContent/Tray/GsUITrayOfflinePlayCount.h"
#include "Net/GsNetSendServiceWorld.h"
#include "Runtime/Core/Public/Misc/EnumClassFlags.h"
#include "Net/GsNetSendService.h"
#include "UTIL/GsTimeSyncUtil.h"
#include "Input/GsInputEventMsgBase.h"
#include "GameObject/ObjectClass/GsGameObjectBase.h"
#include "GameObject/ObjectClass/GsGameObjectLocalPlayer.h"
#include "T1Project/T1Project.h"
#include "Shared/Client/SharedEnums/PD_ResultEnum.h"

#include "T1Project.h"
#include "../UI/UILib/Base/GsUIWidgetBase.h"
#include "../UI/UIContent/Tray/GsUITrayOfflinePlayResults.h"



void FGsOfflinePlayManager::Initialize()
{
	/**
	 * 시간 초기화
	 */
	_freeRemainingSecounds = 0;
	_paidRemainingSecounds = 0;
	_isBuyTime = false;

	FlagReset();

	BindMessage();
}

void FGsOfflinePlayManager::Finalize()
{
	UnBindMessage();

	_freeRemainingSecounds = 0;
	_paidRemainingSecounds = 0;
	_isBuyTime = false;
}

void FGsOfflinePlayManager::BindMessage()
{
	FGsMessageHolder* msgHolder = GMessage();

	_unlockContentsDelegateList.Emplace(msgHolder->GetUnlock().AddRaw(MessageContentUnlock::NOTIFY_CONTENTS_UNLOCK, this, &FGsOfflinePlayManager::OnOfflinePlayContetnsUnlock));
	//_unlockContentsDelegateList.Emplace(msgHolder->GetUnlock().AddRaw(MessageContentUnlock::INVALIDATE_ALL_UNLOCK, this, &FGsOfflinePlayManager::OnInvalidateAllContetnsUnlock));
	
	_gameobjectDelegate = msgHolder->GetGameObject().AddRaw(MessageGameObject::AI_MODE_ACTIVE,this, &FGsOfflinePlayManager::OnAIModeActive);
}

void FGsOfflinePlayManager::UnBindMessage()
{
	FGsMessageHolder* msgHolder = GMessage();

	for (TPair<MessageContentUnlock, FDelegateHandle>& pair : _unlockContentsDelegateList)
	{
		msgHolder->GetUnlock().Remove(pair);
	}
	_unlockContentsDelegateList.Empty();

	msgHolder->GetGameObject().Remove(_gameobjectDelegate);
}

void FGsOfflinePlayManager::StartEvent(EGsEventProgressType In_startType, EGsEventProgressType In_preType)
{
	bool isForceAutoState = _resultData._isPrevAutoState;
	auto CloseEvent = [isForceAutoState]()
	{
		GSOfflinePlay()->FinishEvent(EGsEventProgressType::NONE, EGsEventProgressType::NONE);
		// 2023/4/7 PKT - Offline Mode 확인 버튼 눌렀을 경우 서버에게 알람.
		FGsNetSendServiceWorld::SendOfflinePlayResultConfirm();

		if (isForceAutoState)
		{
			GSAI()->SetActiveAI(true);
		}
	};

	// 2021/06/21 PKT - 이곳에서 직접 Tray오픈
	UGsUIManager* uiManager = GUI();
	if (nullptr != uiManager)
	{
		TWeakObjectPtr<UGsUIWidgetBase> widget = uiManager->OpenAndGetWidget(TEXT("TrayOfflinePlayResults"));
		TWeakObjectPtr <UGsUITrayOfflinePlayResults> tray = (widget.IsValid()) ? Cast<UGsUITrayOfflinePlayResults>(widget.Get()) : nullptr;

		if (true == tray.IsValid())
		{
			// 2021/06/21 PKT - set...
			tray->SetData(_resultData, CloseEvent);
		}
	}	
}

void FGsOfflinePlayManager::FinishEvent(EGsEventProgressType In_finishType, EGsEventProgressType In_nextType)
{
	// 이벤트 진행 순서에 전송(다음 이벤트 진행)
	GSEventProgress()->OnReqFinishEvent(EGsEventProgressType::UI_TRAY_OFFLINEPLAY_RESULT);
}

void FGsOfflinePlayManager::UnavailablePlay(FailedType InType /* = FailedType::IsCommon */)
{
	if (FailedType::IsNotFailed == InType)
	{	// 2022/06/29 PKT - 실패하지 않았는데 오는 경우는 에러 메세지 처리.
		GSLOG(Error, TEXT("FailedType::IsNotFailed == InType"));
		return;
	}

	// 2021/06/29 PKT - 오프라인 실패 팝업
	TWeakObjectPtr<UGsUIWidgetBase> popupBase = GUI()->OpenAndGetWidget(TEXT("PopupOKTwoMessage"));
	TWeakObjectPtr<UGsUIPopupOKTwoMessage> popup = (popupBase.IsValid()) ? Cast<UGsUIPopupOKTwoMessage>(popupBase.Get()) : nullptr;
	if (nullptr != popup)
	{
		FText textFirstMessage;	
		FText::FindText(TEXT("OfflinePlayText"), TEXT("OfflinePlayPopupFirstMessage"), textFirstMessage);

		FText textDescMessage;	
		if (FailedType::IsImpossibleArea == InType)
		{	// 2022/06/29 PKT - 실패 사유가 지역일때..
			FText::FindText(TEXT("OfflinePlayText"), TEXT("OfflinePlayPopupDescMessage2"), textDescMessage);
		}
		else if (FailedType::IsNotEnoughTime == InType)
		{	// 2022/06/29 PKT - 실패 사유가 시간 부족 일때
			FText::FindText(TEXT("OfflinePlayText"), TEXT("OfflinePlayRunOutofTime"), textDescMessage);
		}
		else if (FailedType::IsNotAutoPlay == InType)
		{	// 2023/1/16 PKT - 자동 사냥 중이지 않을 때.
			FText::FindText(TEXT("OfflinePlayText"), TEXT("OfflinePlayAutoFalseAlert"), textDescMessage);
		}
		else
		{	// 2022/06/29 PKT - 그외 모든 곳에 통용되는 메세지
			FText::FindText(TEXT("OfflinePlayText"), TEXT("OfflinePlayPopupDescMessage"), textDescMessage);
		}


		popup->SetData(textFirstMessage, textDescMessage);
	}
}

void FGsOfflinePlayManager::NotifyRemainingSeconds(time_t InFresRemainingSeconds
												, time_t InPaidRemainingSeconds
												, bool InIsBuyTime /*= false*/)
{
	_isBuyTime = InIsBuyTime;

	_freeRemainingSecounds = InFresRemainingSeconds;
	_paidRemainingSecounds = InPaidRemainingSeconds;
	
	/*추후 갱신을 위해 여기서 메지를 뿌려 줘야 할 지도 모름. 윈도우에서 시간 결제가 바로 가능하다면...*/
	GMessage()->GetOfflinePlay().SendMessage(MessageContentOfflinePlay::INVALIDATE_AVAILABLE_TIME, nullptr);
}

void FGsOfflinePlayManager::NotifyCancel()
{
	// 2022/10/18 PKT - 공용 메세지로 에러 메세지 출력(사유를 모름.)
	UnavailablePlay();
}

void FGsOfflinePlayManager::AckReadyState(int32 InResult, int32 InDelaySeconds)
{
	if (true == IsNotPlayResult(InResult))
	{
		return;
	}	

	/*여기에서 Tray 띄어주자.*/
	UGsUIManager* uiManager = GUI();
	if (nullptr == uiManager)
	{
		return;
	}

	TWeakObjectPtr<UGsUIWidgetBase> widget = uiManager->OpenAndGetWidget(TEXT("TrayOfflinePlayCount"));
	TWeakObjectPtr<UGsUITrayOfflinePlayCount> tray = (widget.IsValid()) ? Cast<UGsUITrayOfflinePlayCount>(widget.Get()) : nullptr;
	if (true == tray.IsValid())
	{
		tray->SetData(InDelaySeconds);
	}
}

void FGsOfflinePlayManager::AckPlayStart(int32 InResult)
{
	if (true == IsNotPlayResult(InResult))
	{
		return;
	}

	// 2021/08/25 PKT - 오프라인 플레이 모드 On
	// 2021/08/25 PKT - IOS는 App에서 종료시킬수 없기 때문에 Title 화면으로 넘어 간다.
	_IsOnFlag = true;
	// 2021/08/25 PKT - DisConnection
	GMessage()->GetSystem().SendMessage(MessageSystem::GAME_SERVER_DISCONNECTION);
	// 2021/08/25 PKT - 'OnExitGame'함수를 호출하지 않으면 재연결 로그인 발생. 에러 메세지 뜸
	GNet()->OnExitGame();

#if PLATFORM_IOS

	if (UGsUIManager* uiManager = GUI())
	{
		TWeakObjectPtr<UGsUIWidgetBase> widget = uiManager->OpenAndGetWidget(TEXT("TrayOfflinePlayCount"));
		TWeakObjectPtr<UGsUITrayOfflinePlayCount> tray = (widget.IsValid()) ? Cast<UGsUITrayOfflinePlayCount>(widget.Get()) : nullptr;
		if (true == tray.IsValid())
		{
			tray->UIBlockAndNotice();
		}

		return;
	}

#else
	TWeakObjectPtr<UGsScopeHolder> scopeHolder = Cast<UGsScopeHolder>(GScope());
	TWeakObjectPtr<UGsScopeGame> scopeGame = (true == scopeHolder.IsValid())
		? Cast<UGsScopeGame>(scopeHolder->GetGameScope()) : nullptr;
	if (scopeGame.IsValid())
	{
		scopeGame->QuitGame();
	}
#endif
}

void FGsOfflinePlayManager::AckLocalPlayerSpectatorMode(bool InEnable)
{
	UGsGameObjectBase* object = GSGameObject()->FindObject(EGsGameObjectType::LocalPlayer);
	UGsGameObjectLocalPlayer* localPlayer = (nullptr != object) ? Cast<UGsGameObjectLocalPlayer>(object) : nullptr;
	if (nullptr == localPlayer)
	{
		GSLOG(Error, TEXT("nullptr == localPlayer"));
		return;
	}

	// 2021/10/21 PKT - LocalPlayer 관전자 모드로 세팅.
	if (true == InEnable)
	{	// 2021/11/04 PKT - 이미 관전자 모드라면 따로 처리 할 것 없음.
		if (UGsGameObjectLocalPlayer::EMovementType::Spectator == localPlayer->GetMovementType())
		{
			return;
		}

		// 2021/10/21 PKT - 오프라인 플레이 관전자 모드 시작.( 개발용 )
		GSAI()->SetActiveAI(false);
	}
	else   // 2021/11/04 PKT - 관전자 모드를 끈다.
	{	// 2021/11/04 PKT - 이미 관전자 모드가 아니라면 따로 처리 할 것 없음.
		if (UGsGameObjectLocalPlayer::EMovementType::LocalPlayer == localPlayer->GetMovementType())
		{
			return;
		}
	}

	// 2021/10/21 PKT - UI hidden
	if (UGsUIManager* uiManager = GUI())
	{
		uiManager->SetHUDMode(InEnable ? EGsUIHUDMode::SPECTATOR : EGsUIHUDMode::NORMAL);
	}

	localPlayer->SwitchSpectatorMode(InEnable);	
}

void FGsOfflinePlayManager::NotifyOfflineResult(const FGsOfflinePlayResultData& InData)
{
	_resultData = InData;

	GSEventProgress()->OnReqStartEvent(EGsEventProgressType::UI_TRAY_OFFLINEPLAY_RESULT, this);
}

void FGsOfflinePlayManager::Request_ReadyState()
{
	FailedType failedType = FailedType::IsNotFailed;

	do 
	{
		/*
		* 1. 잔여 시간 체크
		* 2. 오프라인 가능 지역 체크
		*    침공전은 UnLockContetns에서 제어..
		*/
		if (0 >=(_freeRemainingSecounds + _paidRemainingSecounds))
		{	// 2022/06/29 PKT - 남은 시간 체크
			failedType = FailedType::IsNotEnoughTime;
			break;
		}

		const FGsSchemaMapData* schemaMapData = GLevel()->GetCurrentMapInfo();
		if (nullptr == schemaMapData)
		{
			GSLOG(Error, TEXT("nullptr == schemaMapData"));
			return;
		}

		if (true == schemaMapData->offlinePlayDeny)
		{	// 2022/10/17 PKT - Offline Play 불가 지역
			failedType = FailedType::IsImpossibleArea;
			break;
		}
		
		if (FGsGameFlowGame* gameFlow = GMode()->GetGameFlow())
		{
			if (FGsStageManagerGame* stage = gameFlow->GetStageManagerGame())
			{
				if (FGsStageMode::TOWN == stage->GetCurrentStageMode())
				{	// 2022/10/17 PKT - Offline Play 불가 지역
					failedType = FailedType::IsImpossibleArea;
					break;
				}
			}
		}

		// 2023/1/16 PKT - 로컬 플레이어가 자동 사냥 중인지를 체크
		if (nullptr == GSAI() || false == GSAI()->IsAIOn())
		{	// 2022/06/29 PKT - AI 상태일때만 가능.
			failedType = FailedType::IsNotAutoPlay;
			break;
		}

		// 2022/10/17 PKT - Offline Play 시작
		failedType = FailedType::IsNotFailed;

	} while (0);

	if (failedType == FailedType::IsNotFailed)
	{
		FGsNetSendServiceWorld::SendReqOfflinePlayReadyState();
	}
	else
	{
		UnavailablePlay(failedType);
	}
}

time_t FGsOfflinePlayManager::FreeRemainingSecounds() const
{
	return _freeRemainingSecounds;
}

time_t FGsOfflinePlayManager::PaidRemainingSecounds() const
{
	return _paidRemainingSecounds;
}

bool FGsOfflinePlayManager::IsBuyTime() const
{
	return _isBuyTime;
}

bool FGsOfflinePlayManager::InOnFlag() const
{
	return _IsOnFlag;
}

void FGsOfflinePlayManager::FlagReset()
{
	_IsOnFlag = false;
}

bool FGsOfflinePlayManager::IsNotPlayResult(int32 InResult)
{
	if (PD::Result::RESULT_SUCCESS == InResult)
	{
		return false;
	}

	// 2021/06/29 PKT - 오프라인 플레이 이용에 실패( 팝업과 초기화 진행 )
	FailedType type = FailedType::IsCommon;
	if (PD::Result::OFF_LINE_PLAY_AREA == InResult)
	{	// 2022/10/18 PKT - 지역 체크 에러
		type = FailedType::IsImpossibleArea;
	}
	else if (PD::Result::OFF_LINE_PLAY_EXPIRY_TIME == InResult)
	{	// 2022/10/18 PKT - 시간 만료
		type = FailedType::IsNotEnoughTime;
	}

	// 2022/10/18 PKT - Open Error Reason Popup
	UnavailablePlay(type);

	return true;
}

void FGsOfflinePlayManager::OnOfflinePlayContetnsUnlock(const IGsMessageParam* InParm)
{
	if (nullptr == InParm)
	{
		return;
	}

	const FGsPrimitivePairInt32* param = InParm->Cast<const FGsPrimitivePairInt32>();
	if (nullptr == param || EGsUnlockContentsType::OFFLINE_PLAY != static_cast<EGsUnlockContentsType>(param->_first))
	{
		return;
	}

	if (GNet()->IsBlockingSend())
	{
		return;
	}

	if (false == GNet()->GetActive().IsValid() || FGsNet::Mode::GAME != GNet()->GetCurrentNetType())
	{
		return;
	}

	if (GNet()->GetActive()->IsBlockingSend())
	{
		return;
	}

	FGsNetSendService::SendCombatAutoPlay(GSAI()->IsAIOn());
}

//void FGsOfflinePlayManager::OnInvalidateAllContetnsUnlock(const struct IGsMessageParam*)
//{
//	if (GNet()->IsBlockingSend())
//	{
//		return;
//	}
//
//	if (false == GNet()->GetActive().IsValid() || FGsNet::Mode::GAME != GNet()->GetCurrentNetType())
//	{
//		return;
//	}
//
//	if (GNet()->GetActive()->IsBlockingSend())
//	{
//		return;
//	}
//
//	if (false == GSUnlock()->IsContentsUnlock(EGsUnlockContentsType::OFFLINE_PLAY))
//	{
//		return;
//	}
//
//	FGsNetSendService::SendCombatAutoPlay(GSAI()->IsAIOn());
//}

void FGsOfflinePlayManager::OnAIModeActive(const IGsMessageParam* InParm)
{
	if (nullptr == InParm)
	{
		return;
	}
		
	const FGsPrimitiveInt32* param = InParm->Cast<const FGsPrimitiveInt32>();
	if (nullptr == param)
	{
		return;
	}

	if (GNet()->IsBlockingSend())
	{
		return;
	}

	if (false == GNet()->GetActive().IsValid() || FGsNet::Mode::GAME != GNet()->GetCurrentNetType())
	{
		return;
	}

	if (GNet()->GetActive()->IsBlockingSend())
	{
		return;
	}

	if (false == GSUnlock()->IsContentsUnlock(EGsUnlockContentsType::OFFLINE_PLAY))
	{
		return;
	}

	FGsNetSendService::SendCombatAutoPlay(GSAI()->IsAIOn());	
}