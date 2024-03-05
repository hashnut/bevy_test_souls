#include "GsArenaHandler.h"

#include "DataSchema/Arena/GsSchemaArenaData.h"

#include "GameObject/Fence/GsFenceHandler.h"
#include "GameObject/Data/GsGameObjectDataRemotePlayer.h"
#include "GameObject/ObjectClass/GsGameObjectRemotePlayer.h"

#include "Management/ScopeGame/GsArenaManager.h"
#include "Management/ScopeGame/GsUnlockManager.h"
#include "Management/ScopeGame/GsInterServerManager.h"
#include "Management/ScopeGame/GsGameObjectManager.h"
#include "Management/ScopeGlobal/GsUIManager.h"
#include "Management/ScopeGlobal/GsSoundManager.h"
#include "Management/ScopeGlobal/GsLevelManager.h"
#include "Management/ScopeGlobal/GsGameDataManager.h"
#include "Management/GsMessageHolder.h"

#include "Net/GsNetSendServiceWorld.h"

#include "UI/UIContent/Popup/GsUIPopupArenaRequest.h"
#include "UI/UIContent/Popup/GsUIPopupArenaAccept.h"
#include "UI/UIContent/Helper/GsUIHelper.h"
#include "UI/UIContent/Tray/GsUITrayTickerArenaCount.h"

#include "UTIL/GsTimeSyncUtil.h"

#include "Social/GsSocialHelper.h"

#include "Shared/Client/SharedEnums/SharedCommonEnum.h"


void FGsArenaHandler::Initialize()
{
}

void FGsArenaHandler::Finalize()
{
}

void FGsArenaHandler::Set(PD::SC::PKT_SC_ACK_DUEL_INVITE_READ& InPacket)
{
	// 대결을 요청한 것의 응답
	if (CheckPacketError(InPacket.Result()))
	{
		GSArena()->SetArenaOutState(EGsArenaOutState::NONE);
		return;
	}

	// 대결 요청 대기 UI 오픈
	TWeakObjectPtr<UGsUIPopupArenaRequest> popup = Cast<UGsUIPopupArenaRequest>(GUI()->OpenAndGetWidget(TEXT("PopupArenaRequest")));
	if (popup.IsValid())
	{
		popup->InitializeUI(GSArena()->GetTargetInfo());
	}

	GSArena()->SetArenaOutState(EGsArenaOutState::REQUEST);
}

void FGsArenaHandler::Set(PD::SC::PKT_SC_ACK_DUEL_INVITE_CANCEL_READ& InPacket)
{
	// 대결 요청을 취소한 것의 응답
	if (CheckPacketError(InPacket.Result()))
	{

	}
	else
	{
		FText findText;
		FText::FindText(TEXT("ArenaText"), TEXT("ArenaCancelTicker"), findText);
		FGsUIHelper::TraySectionMessageTicker(EGsNotiSection::EXTSYSTEM, findText);
	}

	GSArena()->ResetTargetInfo();
	GMessage()->GetArena().SendMessage(MessageContentArena::CLOSE_ARENA_UI, nullptr);
}

void FGsArenaHandler::Set(PD::SC::PKT_SC_ACK_DUEL_ACCEPT_READ& InPacket)
{
	// 대결 요청을 수락한 것의 응답
	if (CheckPacketError(InPacket.Result()))
	{
		GSArena()->ResetTargetInfo();
		return;
	}

	// 대결 요청을 수락했을 경우
	if (_isClickedAccept)
	{
		GSArena()->SetArenaInState(EGsArenaInState::WAIT);
		GSArena()->SetArenaOutState(EGsArenaOutState::ENTERED);
		GSArena()->SetEnteredArenaData(GSArena()->GetArenaData());

		// 대결 신청이 수락되었습니다. 잠시 후 대결장 안으로 이동합니다.
		FText findText;
		FText::FindText(TEXT("ArenaText"), TEXT("ArenaAccepted"), findText);
		FGsUIHelper::TraySectionMessageTicker(EGsNotiSection::EXTSYSTEM, findText);
		_isClickedAccept = false;
	}
	else
	{
		GSArena()->ResetTargetInfo();
	}

	GMessage()->GetArena().SendMessage(MessageContentArena::CLOSE_ARENA_UI, nullptr);
}

void FGsArenaHandler::Set(PD::SC::PKT_SC_NOTIFY_DUEL_INVITE_CANCEL_READ& InPacket)
{
	// 대결 요청한 유저가 대결 취소한 것의 알림
	// 대결이 취소되었습니다.
	FText findText;
	FText::FindText(TEXT("ArenaText"), TEXT("ArenaCanceledTicker"), findText);
	FGsUIHelper::TraySectionMessageTicker(EGsNotiSection::EXTSYSTEM, findText);

	GSArena()->ResetTargetInfo();

	GMessage()->GetArena().SendMessage(MessageContentArena::CLOSE_ARENA_UI, nullptr);
}

void FGsArenaHandler::Set(PD::SC::PKT_SC_NOTIFY_DUEL_INVITED_READ& InPacket)
{
	// 대결 요청을 받은 것의 알림
	bool isVisible = FGsSocialHelper::IsVisible(InPacket.SocialMediaInfo(), InPacket.UserName(), InPacket.GuildName());
	GSArena()->SetTargetInfo(INVALID_USER_DB_ID, InPacket.Level(), InPacket.UserName(), isVisible);

	// 대결 수락 UI 오픈
	TWeakObjectPtr<UGsUIPopupArenaAccept> popup = Cast<UGsUIPopupArenaAccept>(GUI()->OpenAndGetWidget(TEXT("PopupArenaAccept")));
	if (popup.IsValid())
	{
		popup->InitializeUI(InPacket.Level(), InPacket.UserName(), isVisible);
	}

	GSArena()->SetArenaOutState(EGsArenaOutState::RECEIVE);
}

void FGsArenaHandler::Set(PD::SC::PKT_SC_NOTIFY_DUEL_INVITE_RESULT_READ& InPacket)
{
	if (CheckPacketError(InPacket.Result()))
	{
		GSArena()->ResetTargetInfo();
		GMessage()->GetArena().SendMessage(MessageContentArena::CLOSE_ARENA_UI, nullptr);
		return;
	}

	// 대결 요청 후 상대의 응답 알림
	if (false == InPacket.IsAccept())
	{
		// 상대방이 대결 신청을 거절했습니다.
		FText findText;
		FText::FindText(TEXT("ArenaText"), TEXT("ArenaDeniedTicker"), findText);
		FGsUIHelper::TraySectionMessageTicker(EGsNotiSection::EXTSYSTEM, findText);

		GSArena()->ResetTargetInfo();
	}
	else
	{
		GSArena()->SetArenaInState(EGsArenaInState::WAIT);
		GSArena()->SetArenaOutState(EGsArenaOutState::ENTERED);
		GSArena()->SetEnteredArenaData(GSArena()->GetArenaData());

		// 대결 신청이 수락되었습니다. 잠시 후 대결장 안으로 이동합니다.
		FText findText;
		FText::FindText(TEXT("ArenaText"), TEXT("ArenaAccepted"), findText);
		FGsUIHelper::TraySectionMessageTicker(EGsNotiSection::EXTSYSTEM, findText);
	}

	GMessage()->GetArena().SendMessage(MessageContentArena::CLOSE_ARENA_UI, nullptr);
}

void FGsArenaHandler::Set(PD::SC::PKT_SC_NOTIFY_DUEL_COUNT_START_READ& InPacket)
{
	// 대결 카운트 다운 시작 알림
	GSArena()->SetCountDownStartTime(InPacket.StartedTime());
	GSArena()->SetArenaInState(EGsArenaInState::COUNT_DOWN);
}

void FGsArenaHandler::Set(PD::SC::PKT_SC_NOTIFY_DUEL_START_READ& InPacket)
{
	// 대결 경기 시작 알림
	GSArena()->SetArenaStartTime(InPacket.StartedTime());
	GSArena()->SetArenaInState(EGsArenaInState::ARENA_START);
}

void FGsArenaHandler::Set(PD::SC::PKT_SC_NOTIFY_DUEL_RESULT_READ& InPacket)
{
	// 대결 경기 종료 알림
	GSArena()->SetArenaResult(InPacket.DuelResult());
	GSArena()->SetArenaInState(EGsArenaInState::ARENA_END);
}

void FGsArenaHandler::SetReconnectInfo(PD::SC::PKT_SC_NOTIFY_DUEL_RECONNECT_INFO_READ& InPacket)
{
	time_t startTime = InPacket.StartedTime();

	// 대결 카운트 중
	if (startTime <= 0)
	{
		GSArena()->SetArenaInState(EGsArenaInState::COUNT_DOWN);
	}
	// 대결 중
	else
	{
		GSArena()->SetArenaStartTime(startTime);
		GSArena()->SetArenaInState(EGsArenaInState::ARENA_START);
	}
}

bool FGsArenaHandler::CheckPacketError(int64 InPacketResult)
{
	if (PACKET_RESULT_SUCCESS == InPacketResult)
		return false;

	FText findText;
	switch (InPacketResult)
	{
	case PD::Result::DUEL_ERROR_CANNOT_DUEL_TERRITORY:
		// 대결 신청이 가능한 지역이 아닙니다. 대결장 근처로 이동해주세요.
		FText::FindText(TEXT("ArenaText"), TEXT("NotInArenaReady"), findText);
		break;
	case PD::Result::DUEL_ERROR_CANNOT_DUEL_TERRITORY_OTHER_USER:
		FText::FindText(TEXT("ArenaText"), TEXT("NotInArenaReady"), findText);
		break;
	case PD::Result::DUEL_ERROR_ALEREADY_INVITED_BY_OTHER_USER:
		// 대결이 불가능한 상태입니다.
		FText::FindText(TEXT("ArenaText"), TEXT("NotInArenaReady1"), findText);
		break;
	case PD::Result::DUEL_ERROR_NOT_SAME_MAP:
		FText::FindText(TEXT("ArenaText"), TEXT("NotInArenaReady1"), findText);
		break;
	case PD::Result::DUEL_ERROR_ARENA_ALREADY_PLAYING:
		// 현재 진행중인 대결이 있습니다. 대결 종료 후 다시 시도해주세요.
		FText::FindText(TEXT("ArenaText"), TEXT("ArenaAlreadyOccupied"), findText);
		break;
	case PD::Result::DUEL_ERROR_ALEREADY_INVITE_OTHER_USER:
		FText::FindText(TEXT("ArenaText"), TEXT("NotInArenaReady1"), findText);
		break;
	case PD::Result::DUEL_ERROR_ALEREADY_PLAYING:
		FText::FindText(TEXT("ArenaText"), TEXT("NotInArenaReady1"), findText);
		break;
	case PD::Result::DUEL_ERROR_INVALID_USER:
		FText::FindText(TEXT("ArenaText"), TEXT("NotInArenaReady1"), findText);
		break;
	case PD::Result::DUEL_ERROR_EXPIRED_INVITE_TIME:
	case PD::Result::DUEL_ERROR_NOT_INVITE_USER:
	case PD::Result::DUEL_ERROR_NOT_INVITED:
		break;
	case PD::Result::DUEL_ERROR_UNKNOWN:
	{
		if (findText.IsEmpty())
		{
			FGsUIHelper::PopupNetError(static_cast<PD::Result>(InPacketResult));
			return true;
		}
		break;
	}
	}

	FGsUIHelper::TraySectionMessageTicker(EGsNotiSection::EXTSYSTEM, findText);
	return true;
}

void FGsArenaHandler::SetArenaInState(EGsArenaInState InArenaInState)
{
	UGsUIManager* uiManager = GUI();
	if (nullptr == uiManager)
		return;

	//uiManager->CloseByKeyName(TEXT("TrayTickerArenaCount"));

	FGsMessageHolder* msg = GMessage();
	if (nullptr == msg)
		return;

	FGsArenaManager* arenaManager = GSArena();
	if (nullptr == arenaManager)
		return;

	switch (InArenaInState)
	{
	case EGsArenaInState::NONE:
		break;
	case EGsArenaInState::WAIT:
	{
		uiManager->SetHUDMode(EGsUIHUDMode::NORMAL);
		break;
	}
	case EGsArenaInState::COUNT_DOWN:
	{
		if (const FGsSchemaArenaData* arenaData = arenaManager->GetArenaData())
		{
			int32 count = arenaData->arenaCountSec;
			if (UGsUITrayTickerArenaCount* tray = Cast<UGsUITrayTickerArenaCount>(uiManager->OpenAndGetWidget(TEXT("TrayTickerArenaCount"))))
			{
				tray->SetCount(arenaManager->GetCountDownStartTime(), count);
			}
		}
		break;
	}
	case EGsArenaInState::ARENA_START:
		break;
	case EGsArenaInState::ARENA_END:
	{
		GSArena()->SetArenaStartTime(0);
		FText findText;
		FText::FindText(TEXT("ArenaText"), TEXT("ArenaFinished"), findText);
		FGsUIHelper::TraySectionMessageTicker(EGsNotiSection::EXTSYSTEM, findText);

		DuelResult result = GSArena()->GetArenaResult();
		if (result != DuelResult::LOSE)
		{
			PlayArenaResult(result);
		}
		break;
	}
	}

	msg->GetContentsHud().SendMessage(MessageContentHud::INVALIDATE_ARENA);
}

void FGsArenaHandler::SetArenaOutState(EGsArenaOutState InArenaOutState)
{
	if (InArenaOutState == EGsArenaOutState::NONE)
	{
		GSUnlock()->UnlockContentsByClient(UnlockCategory::DUEL);
	}
	else if (InArenaOutState == EGsArenaOutState::ENTERED)
	{
		GSUnlock()->LockContentsByClient(UnlockCategory::DUEL);
	}
}

void FGsArenaHandler::CancelArenaRequest(bool InShowTicker)
{
	EGsArenaInState inState = GSArena()->GetArenaInState();
	EGsArenaOutState outState = GSArena()->GetArenaOutState();

	// 대결장 밖일 때
	if (inState == EGsArenaInState::NONE)
	{
		// 대결 요청을 받았었으면
		if (outState == EGsArenaOutState::RECEIVE)
		{
			if (TryReqDuelAccept(false) && InShowTicker)
			{
				FText findText;
				FText::FindText(TEXT("ArenaText"), TEXT("ArenaDenyTicker"), findText);
				FGsUIHelper::TraySectionMessageTicker(EGsNotiSection::EXTSYSTEM, findText);
			}
		}
		// 대결 요청을 했었으면
		else if (outState == EGsArenaOutState::REQUEST)
		{
			FGsNetSendServiceWorld::SendReqDuelInviteCancel();
		}

		GSArena()->ResetTargetInfo();
		GMessage()->GetArena().SendMessage(MessageContentArena::CLOSE_ARENA_UI, nullptr);
	}
}

bool FGsArenaHandler::TryReqDuelInvite(UserDBId InUserDBId, Level InLevel, FString InUserName, bool InIsVisible)
{
	EGsArenaOutState arenaOutState = GSArena()->GetArenaOutState();
	switch (arenaOutState)
	{
	case EGsArenaOutState::NONE:
	{
		// 아무런 상태가 아니라면 바로 대결을 요청한다.
		GSArena()->SetArenaOutState(EGsArenaOutState::REQUEST);
		GSArena()->SetTargetInfo(InUserDBId, InLevel, InUserName, InIsVisible);
		FGsNetSendServiceWorld::SendReqDuelInvite(InUserDBId);
		return true;
	}
	case EGsArenaOutState::REQUEST:
	{
		// * 대결 요청 UI가 열렸을 때 컨텍스트 UI를 열면 대결이 취소되므로 이 로직을 타는 일이 없어야한다.
	}
	case EGsArenaOutState::RECEIVE:
	{
		// * 대결 요청 UI가 열렸을 때 컨텍스트 UI를 열면 대결이 취소되므로 이 로직을 타는 일이 없어야한다.
	}
	case EGsArenaOutState::ENTERED:
	{
		// 대결중입니다.
	}
	}

	return false;
}

bool FGsArenaHandler::TryReqDuelAccept(bool InIsAccept)
{
	EGsArenaOutState arenaOutState = GSArena()->GetArenaOutState();
	switch (arenaOutState)
	{
	case EGsArenaOutState::RECEIVE:
		FGsNetSendServiceWorld::SendReqDuelAccept(InIsAccept);
		_isClickedAccept = InIsAccept;
		return true;
	}

	return false;
}

void FGsArenaHandler::PlayArenaResult(DuelResult InResult)
{
	// 진입했던 아레나 정보가 존재하는지 확인
	if (const FGsSchemaArenaData* arenaData = GSArena()->GetEnteredArenaData())
	{
		// UI 오픈 및 사운드 출력
		const FGsSchemaSoundResData* soundRes = nullptr;
		switch (InResult)
		{
		case DuelResult::WIN:
			GUI()->Open(TEXT("TrayTickerWin"));
			soundRes = arenaData->arenaWinSound.GetRow();
			break;
		case DuelResult::NONE:
			GUI()->Open(TEXT("TrayTickerDraw"));
			soundRes = arenaData->arenaWinSound.GetRow();
			break;
		case DuelResult::LOSE:
			GUI()->Open(TEXT("TrayTickerLose"));
			soundRes = arenaData->arenaLoseSound.GetRow();
			break;
		}

		if (soundRes)
		{
			if (UGsSoundPlayer* soundPlayer = GSoundPlayer())
			{
				soundPlayer->PlaySound2D(soundRes);
			}
		}
	}

	GSArena()->SetArenaResult(DuelResult::NONE);
	GSArena()->SetEnteredArenaData(nullptr);
}

void FGsArenaHandler::PlayArenaResultIfLose()
{
	DuelResult result = GSArena()->GetArenaResult();
	if (result == DuelResult::LOSE)
	{
		PlayArenaResult(result);
	}
}

bool FGsArenaHandler::CheckArenaAvailability(bool InShowErrorTicker)
{
	FText findText;
	UGsInterServerManager* interServerManager = GSInterServer();
	if (nullptr == interServerManager)
		return false;

	FGsFenceHandler* fenceHandler = GFence();
	if (nullptr == fenceHandler)
		return false;

	do 
	{
		// 침공전 진행중인가?
		if (GGameData()->GetInvasionDefenseState())
		{
			// 침공전이 진행중일 때는 대결이 불가능합니다.
			FText::FindText(TEXT("ArenaText"), TEXT("ArenaUnableInvade"), findText);
			break;
		}

		// 침공 가능한 맵이고 (침공하거나 침공 가능한 상태)라면 대결이 불가
		int32 mapId = GLevel()->GetCurrentLevelId();
		if (interServerManager->GetInvadeMapData(mapId).IsValid() && (interServerManager->IsInvasionState() || interServerManager->IsValidAirRaidState()))
		{
			// 침공전이 진행중일 때는 대결이 불가능합니다.
			FText::FindText(TEXT("ArenaText"), TEXT("ArenaUnableInvade"), findText);
			break;
		}

		// 대결 준비 구역 안에 있는지 확인
		if ((fenceHandler->IsEnteredTerritoryType(TerritoryType::ARENA) == false) &&
			fenceHandler->IsEnteredTerritoryType(TerritoryType::ARENA_READY))
		{
			// 현재 지역의 아레나 정보가 존재하는지 확인
			if (const FGsSchemaArenaData* arenaData = UGsTableUtil::FindRowById<UGsTableArenaData, FGsSchemaArenaData>(mapId))
			{
				return true;
			}
		}

		// 대결 신청이 가능한 지역이 아닙니다.
		FText::FindText(TEXT("ArenaText"), TEXT("NotInArenaReady"), findText);

	} while (0);
	
	if (InShowErrorTicker)
	{
		FGsUIHelper::TraySectionMessageTicker(EGsNotiSection::EXTSYSTEM, findText);
	}

	return false;
}
