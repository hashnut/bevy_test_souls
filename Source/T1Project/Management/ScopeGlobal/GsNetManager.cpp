#include "GsNetManager.h"

#include "Net/GsNetLobby.h"
#include "Net/GsNetGame.h"
#include "Net/GsNetSendService.h"
#include "Net/GsNetAutoLogin.h"

#include "Shared/Shared/SharedPackets/PD_Auth_Client.h"

#include "Net/MessageHandler/GsNetMessageHandlerGameObject.h"
#include "Net/MessageHandler/GsNetMessageHandlerItem.h"
#include "Net/MessageHandler/GsNetMessageHandlerAuth.h"
#include "Net/MessageHandler/GsNetMessageHandlerGateway.h"
#include "Net/MessageHandler/GsNetMessageHandlerGatewayGuild.h"
#include "Net/MessageHandler/GsNetMessageHandlerGatewayMarket.h"
#include "Net/MessageHandler/GsNetMessageHandlerWorld.h"
#include "Net/MessageHandler/GsNetMessageHandlerQuest.h"
#include "Net/MessageHandler/GsNetMessageHandlerEvent.h"
#include "Net/MessageHandler/GsNetMessageHandlerContentsEvent.h"
#include "Net/MessageHandler/GsNetMessageHandlerFairy.h"
#include "Net/MessageHandler/GsNetMessageHandlerChat.h"
#include "Net/MessageHandler/GsNetMessageHandlerGuild.h"
#include "Net/MessageHandler/GsNetMessageHandlerBuddy.h"
#include "Net/MessageHandler/GsNetMessageHandlerCostume.h"
#include "Net/MessageHandler/GsNetMessageHandlerItemCollection.h"
#include "Net/MessageHandler/GsNetMessageHandlerSpaceCrack.h"
#include "Net/MessageHandler/GsNetMessageHandlerMarket.h"
#include "Net/MessageHandler/GsNetMessageHandlerCraft.h"
#include "Net/MessageHandler/GsNetMessageHandlerParty.h"
#include "Net/MessageHandler/GsNetMessageHandlerMultiLevelRank.h"
#include "Net/MessageHandler/GsNetMessageHandlerCommunity.h"
#include "Net/MessageHandler/GsNetMessageHandlerBMShop.h"
#include "Net/MessageHandler/GsNetMessageHandlerBoss.h"
#include "Net/MessageHandler/GsNetMessageHandlerAchievement.h"
#include "Net/MessageHandler/GsNetMessageHandlerMonsterKnowledge.h"
#include "Net/MessageHandler/GsNetMessageHandlerTitle.h"
#include "Net/MessageHandler/GsNetMessageHandlerPKBook.h"
#include "Net/MessageHandler/GsNetMessageHandlerSeal.h"
#include "Net/MessageHandler/GsNetMessageHandlerBattlePass.h"
#include "Net/MessageHandler/GsNetMessageHandlerSpiritShot.h"
#include "Net/MessageHandler/GsNetMessageHandlerBattleArena.h"

#include "Data/GsDataContainManager.h"

#include "Management/GsMessageHolder.h"

#include "GsState.h"

#include "GsDivergenceManager.h"

#include "Management/ScopeGlobal/GsBMShopManager.h"
#include "Management/ScopeLobby/GsLobbyDataManager.h"

#include "StringConv.h"
#include "UI/UIContent/Helper/GsUIHelper.h"

#include "GsUIManager.h"
#include "GsLevelManager.h"
#include "TimerManager.h"
#include "UI/UIContent/Popup/GsUIPopupLoginWaiting.h"

#include "Message/MessageParam/GsStageMessageParam.h"
#include "GsHiveManager.h"
#include "Net/NetErrorHandler/GsNetErrorHandler.h"

//------------------------------------------------------------------

//------------------------------------------------------------------
FGsNetBase* FGsNetAllocator::Alloc(FGsNet::Mode inMode)
{
	if (FGsNet::Mode::LOBBY == inMode)
	{
		return new FGsNetLobby();
	}
	else if (FGsNet::Mode::GAME == inMode)
	{
		return new FGsNetGame();
	}
	else if (FGsNet::Mode::RECONNECT == inMode)
	{
		return nullptr;
	}
	return nullptr;
}
//------------------------------------------------------------------

//------------------------------------------------------------------

#define LOCTEXT_NAMESPACE "FGsNetManager" 

void FGsNetManager::Initialize()
{
	_errorHandler = MakeShared<FGsNetErrorHandler>();
	_errorHandler->Initialize(this);

	_protocalWorld.SetErrorHandler(_errorHandler);
	_protocalGateway.SetErrorHandler(_errorHandler);

	//핸들러들을 등록 및 초기화한다.
	InitializeMessage();

	const FGsNet::Mode allMode [] = { FGsNet::Mode::LOBBY, FGsNet::Mode::GAME };
	for (const FGsNet::Mode& e : allMode)
	{
		/*auto inst = */MakeInstance(e);
	}

	_autoLogin = new FGsNetAutoLogin();	
	if (nullptr != _autoLogin)
	{
		_autoLogin->OnInitialize();
	}

	_Super::InitState();
}

void FGsNetManager::Finalize()
{
	_serverOwnerName = nullptr;
	_Super::RemoveAll();

	if (nullptr != _autoLogin)
	{
		delete _autoLogin;
		_autoLogin = nullptr;
	}

	if (_errorHandler.IsValid())
	{
		_errorHandler->Finalize();
		_errorHandler.Reset();
	}

	for (IGsNetMessageHandler* handler : _messageHandlers)
	{
		handler->FinalizeMessage();
		delete handler;
	}

	if (UWorld* world = GLevel()->GetWorld())
	{
		world->GetTimerManager().ClearTimer(_loginWatingTimerHandle);
	}

	_messageHandlers.Empty();
	_serverList.Empty();
}

void FGsNetManager::Update(float inTick)
{
	if (true == _dirty)
	{
		_Super::ChangeState(_nextType);
		_dirty = false;
	}

	_coolTimeHandler.Update(inTick);

	if (auto state = CurrentState.Pin())
	{
		state->Update(inTick);
	}
}

//TStateType == FGsNet::Mode
void FGsNetManager::ChangeState(TStateType inState)
{
	if (_nextType != inState)
	{
		_nextType = inState;
		_dirty = true;
	}
}

//핸들러들을 등록 및 초기화한다.
void FGsNetManager::InitializeMessage()
{
	_messageHandlers.Add(new FGsNetMessageHandlerItem());
	_messageHandlers.Add(new FGsNetMessageHandlerGameObject());	
	_messageHandlers.Add(new FGsNetMessageHandlerAuth());
	_messageHandlers.Add(new FGsNetMessageHandlerGateway());
	_messageHandlers.Add(new FGsNetMessageHandlerGatewayGuild());
	_messageHandlers.Add(new FGsNetMessageHandlerGatewayMarket());
	_messageHandlers.Add(new FGsNetMessageHandlerQuest());
	_messageHandlers.Add(new FGsNetMessageHandlerEvent());
	_messageHandlers.Add(new FGsNetMessageHandlerContentsEvent());
	_messageHandlers.Add(new FGsNetMessageHandlerWorld());
	_messageHandlers.Add(new FGsNetMessageHandlerFairy());
	_messageHandlers.Add(new FGsNetMessageHandlerChat());
	_messageHandlers.Add(new FGsNetMessageHandlerGuild());	
	_messageHandlers.Add(new FGsNetMessageHandlerBuddy());
	_messageHandlers.Add(new FGsNetMessageHandlerCostume());
	_messageHandlers.Add(new FGsNetMessageHandlerItemCollection());
	_messageHandlers.Add(new FGsNetMessageHandlerSpaceCrack());
	_messageHandlers.Add(new FGsNetMessageHandlerMarket());
	_messageHandlers.Add(new FGsNetMessageHandlerCraft());
	_messageHandlers.Add(new FGsNetMessageHandlerParty());
	_messageHandlers.Add(new FGsNetMessageHandlerMultiLevelRank());
	_messageHandlers.Add(new FGsNetMessageHandlerCommunity());
	_messageHandlers.Add(new FGsNetMessageHandlerBMShop());
	_messageHandlers.Add(new FGsNetMessageHandlerBoss());
	_messageHandlers.Add(new FGsNetMessageHandlerAchievement());
	_messageHandlers.Add(new FGsNetMessageHandlerMonsterKnowledge());
	_messageHandlers.Add(new FGsNetMessageHandlerTitle());
	_messageHandlers.Add(new FGsNetMessageHandlerPKBook());
	_messageHandlers.Add(new FGsNetMessageHandlerSeal());
	_messageHandlers.Add(new FGsNetMessageHandlerBattlePass());
	_messageHandlers.Add(new FGsNetMessageHandlerSpiritShot());
	_messageHandlers.Add(new FGsNetMessageHandlerBattleArena());

	for (IGsNetMessageHandler* handler : _messageHandlers)
	{
		handler->InitializeMessage(this);
	}
}

void FGsNetManager::RegisterMessage()
{
	// 메시지 등록한다.

	GMessage()->GetSystem().AddRaw(
		MessageSystem::AUTH_SERVER_CONNECTION_START, this, &FGsNetManager::LoginStart
	);

	GMessage()->GetSystem().AddRaw(
		MessageSystem::AUTH_SERVER_QUERY_IP_FAIL, this, &FGsNetManager::AuthServerQueryIpFail
	);
	GMessage()->GetSystem().AddRaw(
		MessageSystem::AUTH_SERVER_QUERY_IP_SUCCESS, this, &FGsNetManager::AuthServerQuerySuccess
	);

	GMessage()->GetSystem().AddRaw(
		MessageSystem::GAME_SERVER_QUERY_IP_FAIL, this, &FGsNetManager::GameServerQueryIpFail
	);
	GMessage()->GetSystem().AddRaw(
		MessageSystem::GAME_SERVER_QUERY_IP_SUCCESS, this, &FGsNetManager::GameServerQuerySuccess
	);
}

void FGsNetManager::LoginStart()
{
	GSLOG(Log, TEXT("NetManager::LoginStart"));

	FGsUIHelper::ShowBlockUI();

	TSharedPtr<FGsNetBase> netLobby = Find(FGsNet::Mode::LOBBY);
	if (netLobby.IsValid())
	{
		FString address;
		int port = 0;
		
#if WITH_EDITOR
		if (const FGsSchemaNetAddress* netAddress = GetNetAddressTableRow())
		{
			address = netAddress->Address;
			port = netAddress->Port;
		}
		else
		{
			GSLOG(Error, TEXT("chemaNetAddress Nullptr"));
		}
#else
		const TArray<FString>& authAgentList = GDivergence()->GetAuthAgentList();
		if (0 < authAgentList.Num())
		{
			address = authAgentList[_serverSelectIndex];
			port = FCString::Atoi(*GDivergence()->GetAuthAgentPort());
		}
		else
		{
			GSLOG(Error, TEXT("Divergence AuthAgentList Num = 0"));

			FGsLobbyFlowMsgParam param(MessageStage::BACKTO_DIVERGENCE);
			GMessage()->GetStageParam().SendMessage(MessageStage::LOBBY_STATE_UPDATE_NOTIFY, &param);
		}		
#endif		
		//*도메인 방법 :
		//wz - tgateway01.qpyou.cn 로 한국에서 184.85.234.127 이 얻어지면 성공,
		//다른 IP가 얻어지거나 문제가 있다면 말씀해주시기 부탁드립니다.
		//FClientSession::RunAsyncQuery(TEXT("184.85.234.127"));
	
		//OnAuthServerConnected 접속이 성공하면 ShowBlockUI(false); 가 호출됨		
		netLobby->InitConnection();
		netLobby->SetPort(port);
		if (true == netLobby->GetAddressQuery<FGsNetLobby>(static_cast<FGsNetLobby*>(netLobby.Get()), address))
		{
			FGsUIHelper::HideBlockUI();
			if (auto state = GetCurrentState().Pin())
			{
				if (state->GetType() == FGsNet::Mode::LOBBY)
				{
					if (netLobby->IsVaildSessionCondition())
					{
						return;
					}
					else
					{
						netLobby->ReConnect();
					}
				}
			}

			return;
		}
	}	
}

void FGsNetManager::AuthServerQueryIpFail()
{
	GSLOG(Error, TEXT("FGsNetManager::AuthServerQueryIpFail"));
	FGsUIHelper::HideBlockUI();
}

void FGsNetManager::AuthServerQuerySuccess()
{
	GSLOG(Error, TEXT("FGsNetManager::AuthServerQuerySuccess"));

	TSharedPtr<FGsNetBase> netLobby = Find(FGsNet::Mode::LOBBY);
	if (netLobby.IsValid())
	{
		ChangeState(FGsNet::Mode::LOBBY);
	}
}

void FGsNetManager::GameServerQueryIpFail()
{
	GSLOG(Error, TEXT("FGsNetManager::GameServerQueryIpFail"));
	FGsUIHelper::TrayDimmed(false);
	FGsUIHelper::HideBlockUI();
}

void FGsNetManager::GameServerQuerySuccess()
{
	GSLOG(Error, TEXT("FGsNetManager::GameServerQuerySuccess"));
	TSharedPtr<FGsNetBase> netGame = Find(FGsNet::Mode::GAME);
	if (netGame.IsValid())
	{
		ChangeState(FGsNet::Mode::GAME);
	}
}

TSharedPtr<FGsNetBase> FGsNetManager::GetActive() const
{
	if(TSharedPtr<FGsNetBase> active = GetCurrentState().Pin())
	{
		if (active->GetType() == FGsNet::Mode::LOBBY)
		{
			GSLOG(Log, TEXT("\t FGsNetManager::GetActive GetType = LOBBY"));
		}
		/*else if (active->GetType() == FGsNet::Mode::GAME)
		{
			GSLOG(Log, TEXT("\t FGsNetManager::GetActive GetType = GAME"));
		}*/
		else if (active->GetType() != FGsNet::Mode::GAME)
		{
			GSLOG(Log, TEXT("\t FGsNetManager::GetActive GetType = NOTING "));
		}

		if (active->IsVaildSessionCondition())
		{
			bool isBlockingSend = active->IsBlockingSend();

			return  isBlockingSend ? nullptr : active;
		}
		GSLOG(Error, TEXT("\t FGsNetManager::GetActive is IsVaildSessionCondition = false "));
	}	
	return nullptr;
}

bool FGsNetManager::IsBlockingSend()
{
	TSharedPtr<FGsNetBase> netBase = GetActive();
	if (netBase.IsValid())
	{
		return false;
	}

	return true;
}

void FGsNetManager::SetAccountId(AccountDBId inId)
{
	_serverSelectData._accoutId = inId;
}

void FGsNetManager::InitTimeSync(const int64& inFrequncy, const uint64& inTickCount, const time_t& inTimeNow, 
	float inTimeDilation, const time_t& inUTCOffsetSecond)
{
	_netTimeSync.Initialize(inFrequncy, inTickCount, inTimeNow, inTimeDilation, inUTCOffsetSecond);
}

void FGsNetManager::SetTimeNow(const time_t& inTimeNow, const time_t& inUTCOffsetSecond)
{
	_netTimeSync.SetNowTime(inTimeNow, inUTCOffsetSecond);
}

void FGsNetManager::SetWorldId(WorldId inWorldId)
{
	_serverSelectData._worldId = inWorldId; 

	if (UGsLobbyDataManager* LobbyDataManager = LSLobbyData())
	{
		FString strServerName;
		//LobbyDataManager->GetServerName(inWorldId, strServerName);
		_serverSelectData._serverName = strServerName;
	}
}

void FGsNetManager::ClearPacketSendBlock()
{
	if(TSharedPtr<FGsNetBase> active = GetCurrentState().Pin())
	{
		active->SetPacketSendBlock(false);
	}
}

const FGsSchemaNetAddress* FGsNetManager::GetNetAddressTableRow() const
{
	if (const UGsTable* table = FGsSchemaNetAddress::GetStaticTable())
	{
		TArray<const FGsSchemaNetAddress*> allRow;

		if (false != table->GetAllRows(allRow))
		{
			if (_serverSelectIndex < allRow.Num())
			{
				return allRow[_serverSelectIndex];
			}
		}
	}

	FString msg = FString::Format(TEXT("[NetAddress] Fail to find row. keyIndex: {0} "), { _serverSelectIndex });
	FGsUIHelper::PopupNetError(FText::FromString(msg));
#if WITH_EDITOR
	GSLOG(Error, TEXT("%s"), *msg);
#endif //WITH_EDITOR
	
	return nullptr;
}

uint64 FGsNetManager::GetNetMilliSecond()
{
	return _netTimeSync.GetServerTickMillisecond();
}

double FGsNetManager::GetNetTickMultiply() const
{
	return _netTimeSync.GetTickMultiply();
}

void FGsNetManager::PrintServerTime()
{
	_netTimeSync.PrintServerTime();
}

void FGsNetManager::OnExitGame()
{
	TSharedPtr<FGsNetBase> netBase = GetActive();
	if (netBase.IsValid())
	{
		netBase->OnExitGame();
	}

	_coolTimeHandler.Clear();
}

bool FGsNetManager::IsLobbyHandshakeCompleted()
{
	TSharedPtr<FGsNetBase> netBase = GetActive();
	if (netBase.IsValid())
	{
		FGsNetLobby* netLobby = static_cast<FGsNetLobby*>(netBase.Get());
		if (netLobby)
		{
			return netLobby->IsVaildSessionCondition();
		}
	}

	return false;
}

// 프리 로그인
void FGsNetManager::SetPreLogin(PD::AC::PKT_AC_ACK_PRE_LOGIN_READ* inPacket)
{
	if (nullptr == inPacket)
	{
		return;
	}

	//if (FGsBMShopManager* bmShopMgr = GBMShop())
	//{
	//	bmShopMgr->SetIsCanUseZpay(inPacket->CanZPay());
	//}
	_registerDevice = inPacket->RegisterDevice();	// 기기등록 여부	

	GSLOG(Log, TEXT("GsNetManager::SetPreLogin registerDevice : %d"), _registerDevice);

	if (false == _registerDevice)
	{
		FText findText;
		FText::FindText(TEXT("LauncherText"), TEXT("Device_Registration_Alert"), findText);

		FGsUIHelper::TrayTickerLauncher(findText);
	}	

	FGsPrimitiveUInt8 param(static_cast<uint8>(_registerDevice));
	GMessage()->GetSystemParam().SendMessage(MessageSystem::REGISTER_DEVIECE_RESULT_RECEVIED_SERVER, &param);
}

bool FGsNetManager::IsVaildServerId(uint16 inServerId)
{
	if (inServerId == 0)
	{
		return false;
	}

	ServerElem* find = _serverList.FindByPredicate([&](const ServerElem& serverInfo)->bool {
		return inServerId == serverInfo.mPlanetWorldId;
		});

	return (nullptr == find)? false : true;
}

bool FGsNetManager::SetServerList(PD::AC::PKT_AC_ACK_LOGIN_READ* inPacket)
{
	GSLOG(Log, TEXT("GsNetManager::SetServerList ACK_LOGIN"));
	if (nullptr == inPacket)
	{
		GSLOG(Log, TEXT("GsNetManager::SetServerList ACK_LOGIN, nullptr == inPacket"));
		return false;
	}

	// 사전예약 PlanetWorldId
	_advReservationPlanetWorldId = inPacket->AdvReservationPlanetWorldId();

	if (0 >= inPacket->GetServerListCount())
	{
		// 활성화 된 서버가 없다.

		GSLOG(Log, TEXT("GsNetManager::SetServerList ACK_LOGIN, 0 >= inPacket->GetServerListCount()"));
		return false;
	}

	SetAccountId(inPacket->AccountId());
	SetBestPlanetWorldId(inPacket->BestPlanetWorldId());
	SetLastPlanetWorldId(inPacket->LastPlanetWorldId());
	SetAccountName(inPacket->AccountName());

	_serverList.Empty();
	using serverListIter = PD::AC::PKT_AC_ACK_LOGIN_READ::ServerListIterator;
	for (serverListIter it = inPacket->GetFirstServerListIterator();
		it != inPacket->GetLastServerListIterator(); ++it)
	{
		_serverList.Emplace(
			it->PlanetWorldId(), it->OrderNum(), it->UserCount(), it->Congestion(), it->CharCreatable(), it->Recommended(),
			it->NewOpened(), it->HiveAnalyticsServerId(), it->AppendixName()
		);
	}

	_serverList.Sort([](const ServerElem& A, const ServerElem& B)
		{
			return (A.mOrderNum < B.mOrderNum) ? true : false;
		});
	
	/// 다운로드 전 선택된 서버Id
	if (IsVaildServerId(_beforePatchPlanetWorldId))
	{
		SetPlanetWorldId(_beforePatchPlanetWorldId);

#if	!UE_BUILD_SHIPPING
		FString beforePatchPlanetWorldName;
		GetPlanetWorldIdName(_beforePatchPlanetWorldId, beforePatchPlanetWorldName);
		const FString beforePatchPlanetWorldInfoString = FString::Printf(
			TEXT("[Net] beforePatch PlanetWorldId : %d, PlanetWorldName : %s"), _planetWorldId, *beforePatchPlanetWorldName);

		GSLOG(Log, TEXT("%s"), *beforePatchPlanetWorldInfoString);
#endif
		_beforePatchPlanetWorldId = 0;

		return true;	
	}

	if (IsVaildServerId(_lastPlanetWorldId))
	{
		SetPlanetWorldId(_lastPlanetWorldId);
	}
	else
	{
		if (IsVaildServerId(_bestPlanetWorldId))
		{
			SetPlanetWorldId(_bestPlanetWorldId);
		}
		else
		{
			if (0 < _serverList.Num())
			{
				SetPlanetWorldId(_serverList[0].mPlanetWorldId);
			}
		}		
	}

#if	!UE_BUILD_SHIPPING
	FString selectPlanetWorldName = GetSelectPlanetWorldName();
	const FString planetWorldInfoString = FString::Printf(
		TEXT("[Net] PlanetWorldId : %d, PlanetWorldName : %s"), _planetWorldId, *selectPlanetWorldName);

	GSLOG(Log, TEXT("%s"), *planetWorldInfoString);
#endif

	return true;
}

bool FGsNetManager::SetServerList(PD::AC::PKT_AC_ACK_SELECT_SERVER_READ* inPacket)
{
	if (nullptr == inPacket)
	{
		return false;
	}

	if (0 >= inPacket->GetUpdateServerListCount())
	{
		// 활성화 된 서버가 없다.
		return false;
	}

	//_serverList.Empty();
	using UpdateServerListIterator = PD::AC::PKT_AC_ACK_SELECT_SERVER_READ::UpdateServerListIterator;
	for (UpdateServerListIterator it = inPacket->GetFirstUpdateServerListIterator();
		it != inPacket->GetLastUpdateServerListIterator(); ++it)
	{
		ServerElem* find = _serverList.FindByPredicate([&](const ServerElem& serverInfo)->bool {
			return it->PlanetWorldId() == serverInfo.mPlanetWorldId;
			});

		if (nullptr != find)
		{
			find->mCongestion = it->Congestion();
			find->mCharCreatable = it->CharCreatable();
			find->mRecommended = it->Recommended();
			find->mNewOpened = it->NewOpened();
		}
	}

	return true;
}
/*
bool FGsNetManager::TryGetZpayPreLoginServerName(WorldId inWorldId, FString InServerAppendixName, OUT FString& outServerName)
{
	if (0 >= inWorldId || InServerAppendixName.IsEmpty())
		return false;

	//GSLOG(Error, TEXT("[TryGetZpayPreLoginServerName]"));

	FString worldName;
	if (const FGsSchemaWorldData* schemaWorldData = UGsTableUtil::FindRowById<UGsTableWorldData, FGsSchemaWorldData>(inWorldId))
	{
		worldName = schemaWorldData->worldName.ToString();
	}
	else
	{
		worldName = FString::FromInt(inWorldId);
	}

	const FString& appendixName = InServerAppendixName;
	if (0 < appendixName.Len())
	{
		outServerName = FString::Printf(TEXT("[%s] %s"), *appendixName, *worldName);
	}
	else
	{
		outServerName = worldName;
	}

	return true;
}
*/
bool FGsNetManager::TryGetServerName(WorldId inWorldId, OUT FString& outServerName)
{
	const ServerElem* find = _serverList.FindByPredicate([&](const ServerElem& serverInfo)->bool {
		return inWorldId == PlanetWorldId(serverInfo.mPlanetWorldId).st.worldId;
		});

	if (!find)
		return false;

	outServerName = find->mAppendixName;

	return true;
}

bool FGsNetManager::GetPlanetWorldIdName(PlanetWorldId inPlanetWorldId, OUT FString& outServerName)
{
	return TryGetServerName(inPlanetWorldId.st.worldId, outServerName);
}

bool FGsNetManager::TryGetHiveAnalyticsServerId(uint16 inPlanetWorldId, OUT FString& outServerId)
{
	const ServerElem* find = _serverList.FindByPredicate([&](const ServerElem& serverInfo)->bool {
			return inPlanetWorldId == serverInfo.mPlanetWorldId;
		});

	if (find)
	{
		outServerId = find->mHiveAnalyticsServerId;
	}

	return (find != nullptr);
}

void FGsNetManager::SetLoginWaitingStart(int inWaitingCount)
{
	TWeakObjectPtr<UGsUIWidgetBase> widget = GUI()->GetWidgetByKey(TEXT("PopupLoginWaiting"));
	if (!widget.IsValid())
	{
		if (UGsLobbyDataManager* lobbyDataMgr = LSLobbyData())
		{
			if (const UGsLobbyConstant* lobbyConst = lobbyDataMgr->GetLobbyConstantData())
			{
				widget = GUI()->OpenAndGetWidgetByPath(
					lobbyConst->_pathPopupLoginWaiting.Id, lobbyConst->_pathPopupLoginWaiting.Path);
			}
		}
	}

	if (widget.IsValid())
	{
		UGsUIPopupLoginWaiting* popup = Cast<UGsUIPopupLoginWaiting>(widget.Get());
		if (popup)
		{			
			popup->SetData(inWaitingCount, [this]()
				{
					SetLoginWaitingUserStop();
				});
		}
	}

	if (UWorld* world = GLevel()->GetWorld())
	{
		// 존재하고 있는 타이머가 이미 있다면 제거
		if (_loginWatingTimerHandle.IsValid())
		{
			world->GetTimerManager().ClearTimer(_loginWatingTimerHandle);
		}

		world->GetTimerManager().SetTimer(_loginWatingTimerHandle, [this]()
			{
				FGsNetSendService::SendGatewayReqLoginWaitingInfo();
				_isLoginWating = false;
			}, 10.0f, false);
	}

	_isLoginWating = true;
}

void FGsNetManager::SetLoginWaitingStop()
{
	if (GUI()->IsActiveWidget(TEXT("PopupLoginWaiting")))
	{
		TWeakObjectPtr<UGsUIWidgetBase> widget = GUI()->GetWidgetByKey(TEXT("PopupLoginWaiting"));
		if (widget.IsValid())
		{
			widget->Close(true);
		}
	}

	if (UWorld* world = GLevel()->GetWorld())
	{
		// 존재하고 있는 타이머가 이미 있다면 제거
		if (_loginWatingTimerHandle.IsValid())
		{
			world->GetTimerManager().PauseTimer(_loginWatingTimerHandle);
			world->GetTimerManager().ClearTimer(_loginWatingTimerHandle);
		}
	}
	_isLoginWating = false;
}

void FGsNetManager::SetLoginWaitingUserStop()
{
	SetLoginWaitingStop();
	FGsNetSendService::SendGatewayReqLoginWaitingCancel();
}

void FGsNetManager::SetPlanetWorldId(const uint16& inPlanetWorldId)
{
	_planetWorldId = inPlanetWorldId;

	uint16 id = _planetWorldId;
	int index = _serverList.IndexOfByPredicate([id](const ServerElem& data)
		{
			return data.mPlanetWorldId == id;
		});

	FString serverName;
	if (0 <= index && index < _serverList.Num())
	{			
		GetPlanetWorldIdName(_serverList[index].mPlanetWorldId, serverName);
		_selectPlanetWorldName = serverName;
	}

	if (0 > index && 0 < _serverList.Num())
	{	
		GetPlanetWorldIdName(_serverList[0].mPlanetWorldId, serverName);
		_selectPlanetWorldName = serverName;
	}

	//bak1210 
	// 안명달 구두협약 : 선택하고 클라먼저 하이브쪽에 서버선택
	//https://jira.com2us.com/wiki/pages/viewpage.action?pageId=304440737
	FString hiveAnalyticsServerId;
	if (TryGetHiveAnalyticsServerId(inPlanetWorldId, hiveAnalyticsServerId))
	{
		GHive()->SetServerIdByUserSelectServer(hiveAnalyticsServerId);
	}
	//-------------------------------------------------------------------------
}

FGsNet::Mode FGsNetManager::GetCurrentNetType() const
{
	if(TSharedPtr<FGsNetBase> currState = GetCurrentState().Pin())
	{
		return currState->GetType();
	}

	return FGsNet::Mode::MAX;
}

void FGsNetManager::SetRestMode(bool InIsRest)
{
	if (TSharedPtr<FGsNetBase> currState = GetCurrentState().Pin())
	{
		currState->SetRestMode(InIsRest);
	}
}

#undef LOCTEXT_NAMESPACE 
