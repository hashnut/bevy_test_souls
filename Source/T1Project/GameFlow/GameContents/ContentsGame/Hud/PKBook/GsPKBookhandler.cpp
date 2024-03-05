
#include "GsPKBookhandler.h"
#include "PKBook/GsPKBookData.h"

#include "Management/GsMessageHolder.h"
#include "Management/ScopeGlobal/GsUIManager.h"
#include "Management/ScopeGame/GsChatManager.h"
#include "Management/ScopeGame/GsPKBookManager.h"
#include "Management/ScopeGame/GsMinimapManager.h"
#include "Management/ScopeGame/GsGameObjectManager.h"
#include "Management/ScopeGame/GsEventProgressManager.h"

#include "EventProgress/GsEventProgressDefine.h"

#include "UTIL/GsText.h"

#include "Option/GsAlertHelper.h"
#include "Option/GsServerOption.h"

#include "Map/Minimap/MapIcon/GsMinimapIconHandler.h"

#include "GameObject/ObjectClass/GsGameObjectNonPlayer.h"

#include "PKBook/GsPKBookHelper.h"
#include "Message/GsMessageGameObject.h"
#include "Message/MessageParam/GsMessageParam.h"

#include "PKBook/GsPKBookStruct.h"
#include "DataSchema/PKBook/GsSchemaPKBookConfig.h"

#include "../UI/UILib/Base/GsUIWidgetBase.h"
#include "../UI/UIContent/Tray/PKBook/GsUITrayMockeryTicker.h"
#include "../UI/UIContent/Tray/PKBook/GsUITrayPKGuildNotifyTicker.h"

#include "../Net/GsNetSendService.h"
#include "../Net/GsNetSendServiceWorld.h"
#include "T1Project.h"





void FGsMockeryReserve::Done()
{
	_attacker = FText::GetEmpty();
	_Dead = FText::GetEmpty();
	_customeMessage = FText::GetEmpty();

	GUI()->CloseByKeyName(TEXT("TrayMockeryTicker"));

	GSEventProgress()->OnReqFinishEvent(EGsEventProgressType::UI_TRAY_PKMOCKERY_TICKER);
}

// 이벤트 시작
void FGsMockeryReserve::StartEvent(EGsEventProgressType In_startType, EGsEventProgressType In_preType)
{
	// 2022/09/22 PKT - "{0}님이 {1}님을 처치하였습니다."
	FText format;	
	FGsPKBookHelper::FindText(TEXT("Popup_Try_Tease_Message"), format);	
	FText message = FText::Format(format, _attacker, _Dead);

	TWeakObjectPtr<UGsUIWidgetBase> widget = GUI()->OpenAndGetWidget(TEXT("TrayMockeryTicker"));
	TWeakObjectPtr <UGsUITrayMockeryTicker> tray = (widget.IsValid()) ? Cast<UGsUITrayMockeryTicker>(widget.Get()) : nullptr;

	if (true == tray.IsValid())
	{
		// 2023/1/18 PKT - Set...
		tray->SetData(message, _customeMessage, [this]() {Done(); });
	}
};

void FGsMockeryReserve::FinishEvent(EGsEventProgressType In_finishType, EGsEventProgressType In_nextType)
{
	if (EGsEventProgressType::UI_TRAY_PKMOCKERY_TICKER == In_finishType)
	{
		Done();
	}	
};

void FGsMockeryReserve::PushData(const FText& InAttacker, const FText& InDead, const FText& InCustomeMessage)
{
	Done();

	_attacker = InAttacker;
	_Dead = InDead;
	_customeMessage = InCustomeMessage;

	GSEventProgress()->OnReqStartEvent(EGsEventProgressType::UI_TRAY_PKMOCKERY_TICKER, this);
}





void FGsPKInfoGuildNotifyReserve::Done()
{
	_killerGuilEmblemdId = INVALID_GUILD_EMBLEM_ID;
	_deadGuildEmblemdId = INVALID_GUILD_EMBLEM_ID;
	_killerName = FText::GetEmpty();
	_deadName = FText::GetEmpty();

	GUI()->CloseByKeyName(TEXT("TrayPKInfoGuildNotify"));

	GSEventProgress()->OnReqFinishEvent(EGsEventProgressType::UI_TRAY_NOTIFY_PK_GUILD_TICKER);
}

// 이벤트 시작
void FGsPKInfoGuildNotifyReserve::StartEvent(EGsEventProgressType In_startType, EGsEventProgressType In_preType)
{
	/**
	 * 채팅 메세지
	 */
	FText format;
	// 2022/09/22 PKT - "{0}님이 {1}님을 처치하였습니다."
	FGsPKBookHelper::FindText(TEXT("Popup_Try_Tease_Message"), format);

	FText message = FText::Format(format, _killerName, _deadName);

	UGsChatManager* chatManager = GSChat();
	if (chatManager && false == message.IsEmpty())
	{
		chatManager->OnSystemChat(message.ToString(), TEXT("SYSTEM"));
	}

	/**
	 * 티커 메세지
	 */
	TWeakObjectPtr<UGsUIWidgetBase> widget = GUI()->OpenAndGetWidget(TEXT("TrayPKInfoGuildNotify"));
	TWeakObjectPtr <UGsUITrayPKGuildNotifyTicker> tray = (widget.IsValid()) ? Cast<UGsUITrayPKGuildNotifyTicker>(widget.Get()) : nullptr;

	if (true == tray.IsValid())
	{
		// 2023/1/18 PKT - Set...
		tray->SetData(_killerGuilEmblemdId, _deadGuildEmblemdId, _killerName, _deadName, [this]() {Done(); });
	}	
};

void FGsPKInfoGuildNotifyReserve::FinishEvent(EGsEventProgressType In_finishType, EGsEventProgressType In_nextType)
{
	if (EGsEventProgressType::UI_TRAY_NOTIFY_PK_GUILD_TICKER == In_finishType)
	{
		Done();
	}
};

void FGsPKInfoGuildNotifyReserve::PushData(GuildEmblemId InKillerGuilEmblemdId, const FText& InKillerName, GuildEmblemId InDeadGuildEmblemdId, const FText& InDeadName)
{
	Done();

	_killerGuilEmblemdId = InKillerGuilEmblemdId;
	_deadGuildEmblemdId = InDeadGuildEmblemdId;
	_killerName = InKillerName;
	_deadName = InDeadName;

	GSEventProgress()->OnReqStartEvent(EGsEventProgressType::UI_TRAY_NOTIFY_PK_GUILD_TICKER, this);
}







void FGsPKBookHandler::Init()
{
	FGsMessageHolder* MessageManager = GMessage();
	if (nullptr == MessageManager)
	{
		GSLOG(Error, TEXT("nullptr == MessageManager"));
		return;
	}

	_listPKBookDelegate.Emplace(MessageManager->GetPKBook().AddRaw(
		MessageContentPKBook::ON_CLOSE_TARGET_DECTION_ALERT, this, &FGsPKBookHandler::OnCloseTargetDectionAlert));

	_listGameObjectDelegate.Emplace(MessageManager->GetGameObject().AddRaw(
		MessageGameObject::LOCAL_SPAWN_ME_COMPLETE, this, &FGsPKBookHandler::OnLocalPlayerSpawnComplete));

	_listGameObjectDelegate.Emplace(MessageManager->GetGameObject().AddRaw(
		MessageGameObject::SPAW_REMOTE_PLAYER, this, &FGsPKBookHandler::OnSpawnRemotePlayer));

	_listGameObjectDelegate.Emplace(MessageManager->GetGameObject().AddRaw(
		MessageGameObject::DESPAW_REMOTE_PLAYER, this, &FGsPKBookHandler::OnDeSpawnRemotePlyaer));


	_elapsedTime = 0.f;
	_isDoneAlert = true;
}

void FGsPKBookHandler::Close()
{
	_elapsedTime = 0.f;
	_isDoneAlert = true;

	FGsMessageHolder* MessageManager = GMessage();
	if (nullptr == MessageManager)
	{
		GSLOG(Error, TEXT("nullptr == MessageManager"));
		return;
	}

	for (const MsgPKBookHandle& ItemDelegate : _listPKBookDelegate)
	{
		MessageManager->GetPKBook().Remove(ItemDelegate);
	}
	_listPKBookDelegate.Empty();

	for (const MsgGameObjHandle& ItemDelegate : _listGameObjectDelegate)
	{
		MessageManager->GetGameObject().Remove(ItemDelegate);
	}
	_listGameObjectDelegate.Empty();
}

void FGsPKBookHandler::Update(float InDeltaTime)
{
	if (false == _isDoneAlert)
	{
		_elapsedTime -= InDeltaTime;

		if (0.f >= _elapsedTime)
		{	// 2022/10/05 PKT - Reset Interval
			_isDoneAlert = true;
			_elapsedTime = 0.f;
		}
	}	
}

void FGsPKBookHandler::OnCloseTargetDectionAlert(const struct IGsMessageParam* InParam)
{
	if (false == _isDoneAlert)
	{	// 2022/09/30 PKT - 아직 알람을 알릴 준비가 되어 있지 않았다.
		return;
	}

	FGsServerOption* serverOption = GServerOption();
	if (nullptr == serverOption)
	{
		GSLOG(Error, TEXT("nullptr == serverOption"));
		return;
	}

	bool isOnAlreat = false;
	
	const MonitoringTargetNotification& PKBookOption = serverOption->GetPKBookOption();
	if (true == PKBookOption.mSoundOn)
	{	// 2022/10/05 PKT - 사운드 알림 옵션이 켜져 있다면..
		FSoftObjectPath softPath = FSoftObjectPath(TEXT("None"));

		FGsPKBookManager* PKBookManager = GsPKBook();
		if (PKBookManager && PKBookManager->GetConfig())
		{
			softPath = PKBookManager->GetConfig()->soundResource;
		}		

		FGsAlertHelper::PlayAlertEffect(EGsAlertType::RingerAlert, softPath);

		// 2022/10/05 PKT - 알림을 켰다.
		isOnAlreat = true;
	}

	if (true == PKBookOption.mVibrationOn)
	{	// 2022/10/05 PKT - 진동 알림 옵션이 켜져 있다면.
		FGsAlertHelper::PlayAlertEffect(EGsAlertType::VibrateAlert);

		// 2022/10/05 PKT - 알림을 켰다.
		isOnAlreat = true;
	}

	if (true == isOnAlreat)
	{
		// 2022/10/05 PKT - 알림 이벤트가 실행 되었다면..
		_elapsedTime = PKBookOption.mNotificationCycleSecond;
		_isDoneAlert = false;	
	}
}

void FGsPKBookHandler::OnLocalPlayerSpawnComplete(const IGsMessageParam* InParam)
{
	// 2022/09/29 PKT - 미니맵에 존재하는 경계 대상 목록을 모두 삭제 한다.
	UGsMinimapIconHandler* miniMapIconHandler = (nullptr != GSMinimap()) ? GSMinimap()->GetIconHandler() : nullptr;
	if (nullptr == miniMapIconHandler)
	{
		GSLOG(Error, TEXT("nullptr == minimapManager"));
		return;
	}
	
	miniMapIconHandler->ClearPKDetectionTargetIcon();

	_elapsedTime = 0.f;
	// 2022/09/30 PKT - 알림을 할 준비가 되어 있다.
	_isDoneAlert = false;
}

// 2022/09/29 PKT - User Spawn
void FGsPKBookHandler::OnSpawnRemotePlayer(const IGsMessageParam* InParam)
{
	const FGsPrimitivePairUInt64* param = StaticCast<const FGsPrimitivePairUInt64*>(InParam);
	if (nullptr == param)
	{
		GSLOG(Error, TEXT("nullptr == param"));
		return;
	}

	// 2022/09/30 PKT - DBId / GameId
	UserDBId userDBId = StaticCast<UserDBId>(param->_first);
	int64  gameId = StaticCast<int64>(param->_second);

	AddDetectionTarget(userDBId, gameId);
}

// 2022/09/29 PKT - User DeSpawn
void FGsPKBookHandler::OnDeSpawnRemotePlyaer(const IGsMessageParam* InParam)
{
	const FGsPrimitiveUInt64* param = StaticCast<const FGsPrimitiveUInt64*>(InParam);
	if (nullptr == param)
	{
		GSLOG(Error, TEXT("nullptr == param"));
		return;
	}

	ClearDetectionTarget(param->_data);
}

void FGsPKBookHandler::SendChatMessagePKBookResult(PKBookResult InResult)
{
	FGsPKBookManager* PKBookManager = GsPKBook();
	if (nullptr == PKBookManager)
	{
		GSLOG(Error, TEXT("nullptr == PKBookManager"));
		return;
	}

	if (false == PKBookManager->IsPKMessageActive())
	{	// 2022/09/29 PKT - 메세지 보내기 옵션을 꺼놨음.
		return;
	}

	FText sendMessage = FText::GetEmpty();

	if (PKBookResult::KILL == InResult)
	{	// 2022/09/29 PKT - 내가 누군가를 처치 하였다면..
		sendMessage = PKBookManager->GetTeasePKMessage();
		if (sendMessage.IsEmpty())
		{	// 2022/09/29 PKT - 설정해 놓은 메세지가 없다면 기본 메세지를 보낸댜.
			FGsPKBookHelper::FindText(TEXT("Message_Kill_Default"), sendMessage);
		}
	}
	else if (PKBookResult::DIE == InResult)
	{	// 2022/09/29 PKT - 내가 누군가에게 사망 하였다면..
		sendMessage = PKBookManager->GetRevengePKMessage();
		if (sendMessage.IsEmpty())
		{	// 2022/09/29 PKT - 설정해 놓은 메세지가 없다면 기본 메세지를 보낸댜.
			FGsPKBookHelper::FindText(TEXT("Message_Die_Default"), sendMessage);
		}
	}

	if (false == sendMessage.IsEmpty())
	{
		FGsNetSendService::SendReqChat(ChatType::NORMAL, sendMessage.ToString());
	}	
}

void FGsPKBookHandler::PKBookTeaseActionMessage(const FText& InAttacker, const FText& InDead, const FText& InCustomeMessage)
{
	_reserveMockery.PushData(InAttacker, InDead, InCustomeMessage);	
}

void FGsPKBookHandler::DetectionTargetLogOnMessage(UserDBId InUserId)
{
	FGsPKBookManager* PKBookManager = GsPKBook();
	if (nullptr == PKBookManager)
	{
		GSLOG(Error, TEXT("nullptr == PKBookManager"));
		return;
	}

	const FGsPKDetectionTargetData* detectionTarget = PKBookManager->FindDetectionTargetDataById(InUserId);
	if (nullptr == detectionTarget)
	{
		GSLOG(Error, TEXT("not find Detection Target [%lu]"), InUserId);
		return;
	}
	
	// 2022/09/28 PKT - {0}님이 접속하였습니다.
	FText format;
	FGsPKBookHelper::FindText(TEXT("Message_Notify_DetectorTaarget_LogOn"), format);

	FString userNamePrefix = FGsTextUtil::CombineUserAndPrefix(detectionTarget->_targetName, detectionTarget->_worldId);

	FText message = FText::Format(format, { FText::FromString(userNamePrefix)});

	UGsChatManager* chatManager = GSChat();
	if (chatManager && false == message.IsEmpty())
	{
		chatManager->OnSystemChat(message.ToString(), TEXT("SYSTEM"));
	}
}

void FGsPKBookHandler::AddDetectionTarget(UserDBId InUserDBId, int64 InGameId)
{
	FGsPKBookManager* PKBookManager = GsPKBook();
	if (nullptr == PKBookManager)
	{
		GSLOG(Error, TEXT("nullptr == PKBookManager"));
		return;
	}

	if (nullptr == PKBookManager->FindDetectionTargetDataById(InUserDBId))
	{	// 2022/09/29 PKT - 경계 대상이 아님.
		return;
	}

	// 2022/09/30 PKT - Effect Config Data
	const FGsSchemaPKBookConfig* PKBookConfig = PKBookManager->GetConfig();
	if (nullptr == PKBookConfig)
	{
		GSLOG(Error, TEXT("nullptr == PKBookConfig"));
		return;
	}

	UGsMinimapIconHandler* miniMapIconHandler = (nullptr != GSMinimap()) ? GSMinimap()->GetIconHandler() : nullptr;
	if (nullptr == miniMapIconHandler)
	{
		GSLOG(Error, TEXT("nullptr == minimapManager"));
		return;
	}

	miniMapIconHandler->AddPKDetectionTargetIcon(InUserDBId, InGameId, PKBookConfig->detectionEffectInfoSet);
}

void FGsPKBookHandler::ClearDetectionTarget(int64 InGameId)
{
	UGsMinimapIconHandler* miniMapIconHandler = (nullptr != GSMinimap()) ? GSMinimap()->GetIconHandler() : nullptr;
	if (nullptr == miniMapIconHandler)
	{
		GSLOG(Error, TEXT("nullptr == minimapManager"));
		return;
	}

	miniMapIconHandler->DelPKDetectionTargetIcon(InGameId);
}

bool FGsPKBookHandler::SavePKSettingMessage(bool InIsActive, const FText& InTeaseMessage, const FText& InRevengeMessage)
{
	FGsPKBookManager* PKBookManager = GsPKBook();
	if (nullptr == PKBookManager)
	{
		GSLOG(Error, TEXT("nullptr == PKBookManager"));
		return false;
	}

	bool isDiff = true;

	do 
	{
		if (InIsActive != PKBookManager->IsPKMessageActive())
		{	// 2022/10/06 PKT - 다름.
			break;
		}
			
		if (false == InTeaseMessage.EqualTo(PKBookManager->GetTeasePKMessage(), ETextComparisonLevel::Default))
		{	// 2022/10/06 PKT - 다름.
			break;
		}

		if (false == InRevengeMessage.EqualTo(PKBookManager->GetRevengePKMessage(), ETextComparisonLevel::Default))
		{	// 2022/10/06 PKT - 다름.
			break;
		}

		isDiff = false;

	} while (0);

	bool isInBanWord = false;

	if (true == isDiff)
	{
		TArray<FString> checkTexts = { InTeaseMessage.ToString(), InRevengeMessage.ToString() };
		
		FGsTextUtil::IsBanWordInWordList(checkTexts, BannedWordCheckUsage::sentence, [&isInBanWord](bool success, FString inText, FString outText)
			{
				// 2022/10/06 PKT - 한개라도 실패 했다면 금칙어가 포함되어 있는 것.
				isInBanWord |= (false == success);
			}
		);

		if (false == isInBanWord)
		{	// 2022/10/06 PKT - 서버에 저장
			FGsNetSendServiceWorld::Send_ReqPKBookSavePKMessage(
				PKBookManager->IsShowLogInDetectionTargetOnly(), InIsActive, InTeaseMessage, InRevengeMessage
			);
		}
		else
		{	// 2022/10/06 PKT - 금칙어 포함 되었다는 티커 출력
			FText message;
			FGsPKBookHelper::FindText(TEXT("Warning_Message_IncludeBanWord"), message);
			FGsUIHelper::TraySectionMessageTicker(EGsNotiSection::EXTSYSTEM, message);
		}
	}

	return (isInBanWord == false);
}

bool FGsPKBookHandler::SavedMockerMessage(bool InIsUsed, const FText& InMessage)
{
	bool isInBanWord = false;

	TArray<FString> checkTexts = { InMessage.ToString() };

	FGsTextUtil::IsBanWordInWordList(checkTexts, BannedWordCheckUsage::sentence, [&isInBanWord](bool success, FString inText, FString outText)
		{
			// 2022/10/06 PKT - 한개라도 실패 했다면 금칙어가 포함되어 있는 것.
			isInBanWord |= (false == success);
		}
	);

	if (isInBanWord)
	{
		// 2022/10/06 PKT - 금칙어 포함 되었다는 티커 출력
		FText message;
		FGsPKBookHelper::FindText(TEXT("Warning_Message_IncludeBanWord"), message);
		FGsUIHelper::TraySectionMessageTicker(EGsNotiSection::EXTSYSTEM, message);

		return false;
	}

	bool isUseMockeryMessage = GsPKBook()->IsUseMockeryMessage();
	const FText& mockeryMessage = GsPKBook()->GetMockeryMessage();

	bool isSaved = true;

	do 
	{
		if (isUseMockeryMessage != InIsUsed)
		{
			break;
		}

		if (false == InMessage.EqualTo(mockeryMessage))
		{
			break;
		}

		isSaved = false;

	} while (0);


	if (isSaved)
	{
		// 2023/1/18 PKT - 저장
		FGsNetSendServiceWorld::Send_ReqSavedMockeryMessage(InIsUsed, InMessage);
	}

	return true;
}

void FGsPKBookHandler::PKGuildNotify(GuildEmblemId InKillerGuilEmblemdId, const FText& InKillerName, GuildEmblemId InDeadGuildEmblemdId, const FText& InDeadName)
{
	_reserveNofifyPKGuild.PushData(InKillerGuilEmblemdId, InKillerName, InDeadGuildEmblemdId, InDeadName);
}