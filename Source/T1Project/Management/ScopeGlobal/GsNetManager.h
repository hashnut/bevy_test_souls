#pragma once

#include "Classes/GsState.h"
#include "Classes/GsStateMng.h"
#include "Classes/GsManager.h"

#include "Net/GsNetBase.h"
#include "Net/GsNetTimeSynchronizer.h"
#include "Net/GsNetCoolTimeSend.h"

#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "DataSchema/GsSchemaNetAddress.h"

#include "Management/GsScopeGlobal.h"
#include "Management/GsScopeHolder.h"
#include "Management/ScopeLobby/GsLobbyDataManager.h"
//#include "Net/NetErrorHandler/GsNetErrorHandler.h"

#include "Message/GsMessageNet.h"

#include "Shared/Client/SharedEnums/PD_ResultEnum.h"

UENUM()
enum class EGsConnectType : uint8
{
	GoToConnect,		// 고투 
	BackToConnect,		// 빽투 
	ReConnect,			// 리커넥션 

	Max,
};

UENUM()
enum class EGsConnectStep : uint8
{
	Title,				// 타이틀
	Login,				// 로그인 
	ServerSelect,		// 서버선택
	CharacterSelect,	// 캐릭터선택 

	Max,
};


class FGsNetErrorHandler;
class IGsNetMessageHandler;
//------------------------------------------------------------------
class FGsNetAllocator : TGsSharedMapAllocator<FGsNet::Mode, FGsNetBase>
{
public:
	FGsNetAllocator(){}
	virtual ~FGsNetAllocator() {}
	virtual FGsNetBase* Alloc(FGsNet::Mode inMode) override;
};

class FGsNetAutoLogin;
//------------------------------------------------------------------
class T1PROJECT_API FGsNetManager :
	public TGsStateMng<FGsNetBase, FGsNetAllocator>,
	public IGsManager
{
	using _Super = TGsStateMng<FGsNetBase, FGsNetAllocator>;

public:
	//----------------------------------
	// 서버 선택 데이터
	//----------------------------------
	struct FGsServerSelectData
	{
		AccountDBId _accoutId;
		AuthTicket	_authTickect;
		WorldId		_worldId;
		FString		_serverName;
		FString		_clientIpAddress;
	};

private:
	MProtocalAuth				_protocalAuth;
	MResultAuth					_resultAuth;

	MProtocalGateway			_protocalGateway;
	MResultGateway				_resultGateway;

	MProtocalWorld				_protocalWorld;
	MResultWorld				_resultWorld;

private:
	FGsNetCoolTimeSend			_coolTimeHandler;

private:
	TStateType					_nextType = FGsNet::Mode::MAX;	// 치환 원하는 타입
	bool						_dirty = false;

private:
	FString						_serverOwnerName;		// Contents/Table/Net/NetAddress 테이블 참조(유저이름)
	FGsServerSelectData			_serverSelectData;
	UserDBId					_selectUserDBId;		// 게임접속 선택 유저
	uint64						_vID;
	AuthTicket					_reconnectTicket = INVALID_RECONNECT_TICKET;
	FString						_loginId;
	FString						_accountName;					// 최초 생성 캐릭터 이름
	int							_serverSelectIndex = 0;
	uint16						_planetWorldId = 0;
	uint16						_lastPlanetWorldId = 0;			// 마지막 접속 서버
	uint16						_bestPlanetWorldId = 0;			// 추천 서버
	uint16						_beforePatchPlanetWorldId = 0;	// 패치 전 서버
	//uint16					_selectPlanetWorldId = 0;		// 최종적으로 선택되어 보여질 서버 
	FString						_selectPlanetWorldName;

private:
	FGsNetTimeSynchronizer		_netTimeSync;

private:
	//bool						_isAutoLogin;
	int32						_returnToStageType = -1;

private:
	FGsNetAutoLogin*			_autoLogin = nullptr;

private:
	TArray<IGsNetMessageHandler*> _messageHandlers;		// 네트워크 패킷 핸들러 모음(인증,게이트웨이, 게임(오브젝트...)
	TSharedPtr<FGsNetErrorHandler> _errorHandler;

private:
	bool						_isLoginWating = false;
	FTimerHandle				_loginWatingTimerHandle;

private:
	FString						_serverRevision;

private:
	TArray<ServerElem>			_serverList;

private:
	bool						_registerDevice = false;	// 기기등록 여부

private:
	uint16						_advReservationPlanetWorldId = 0; // 사전예약 PlanetWorldId
	FString						_advReservationUserName;		  // 사전예약 Name

public:
	FGsNetManager() 
	{
		/*_protocalAuth(TEXT("MProtocalAuth"))
			_resultAuth(TEXT("MResultAuth")),
			_protocalGateway(TEXT("MProtocalGateway")),
			_resultGateway(TEXT("MResultGateway")),
			_protocalWorld(TEXT("MProtocalWorld")),
			_resultWorld(TEXT("MResultWorld"))*/
	}
	virtual ~FGsNetManager() = default;

public:
	//IGsManager
	virtual void Initialize() override;
	virtual void Finalize() override;
	virtual void Update(float inTick) override;

public:
	virtual void ChangeState(TStateType/*FGsNet::Mode*/ inState) override;

private:
	void InitializeMessage();

public:
	void RegisterMessage();
	void LoginStart();

protected:
	void AuthServerQueryIpFail();
	void AuthServerQuerySuccess();
	void GameServerQueryIpFail();
	void GameServerQuerySuccess();

public:
	// 활성화된 네트워크 전송자를 전달 + 유효하지 않을때는 nullptr을 전달함으로 Vaild 체크는 필수입니다.
	TSharedPtr<FGsNetBase> GetActive() const;

public:
	void SetServerUserName(const FString& inAccountName) { _serverOwnerName = inAccountName; }
	void SetServerIndex(int inIndex) { _serverSelectIndex = inIndex; }
	void SetAccountId(AccountDBId inId);
	void SetVId(const uint64 inId) { _vID = inId; }
	void SetSelectUserDBId(UserDBId userDBId) { _selectUserDBId = userDBId; }
	void InitTimeSync(const int64& inFrequncy, const uint64& inTickCount,
		const time_t& inTimeNow, float inTimeDilation, const time_t& inUTCOffsetSecond);
	void SetTimeNow(const time_t& inTimeNow, const time_t& inUTCOffsetSecond);

	void SetTicket(AuthTicket inTicket) { _serverSelectData._authTickect = inTicket; }
	//void SetAutoLogin(bool inVal);
	void SetWorldId(WorldId inWorldId);
	void SetClientIpAddreass(const FString& inAddress) { _serverSelectData._clientIpAddress = inAddress; }

	void SetReconnectTicket(AuthTicket inTicket) { _reconnectTicket = inTicket; }
	void SetReturnToStageType(int32 inType) { _returnToStageType = inType; }
		
	void ClearPacketSendBlock();

	void SetLoginId(const FString& inLoginId) { _loginId = inLoginId; }	
	//void SetPlanetWorldId(const uint16& inPlanetWorldId) { _planetWorldId = inPlanetWorldId; }
	void SetPlanetWorldId(const uint16& inPlanetWorldId);
	void SetBeforePatchPlanetWorldId(const uint16& inPlanetWorldId) { _beforePatchPlanetWorldId = inPlanetWorldId; }
	void ResetBeforePatchPlanetWorldId() { _beforePatchPlanetWorldId = 0; }

	void SetLoginWaitingStart(int inWaitingCount);
	void SetLoginWaitingStop();
	void SetLoginWaitingUserStop();

	void SetServerRevisionInfo(const FString& inRevision) { _serverRevision = inRevision; }

	void SetBestPlanetWorldId(int inBestPlanetWorldId) { _bestPlanetWorldId = inBestPlanetWorldId; }
	void SetLastPlanetWorldId(int inLastPlanetWorldId) { _lastPlanetWorldId = inLastPlanetWorldId; }
	//void SetSelectPlanetWorldId(int inSelectPlanetWorldId) { _selectPlanetWorldId = inSelectPlanetWorldId; }
	void SetAccountName(const FString& inAccountName) { _accountName = inAccountName; }


private:
	const FGsSchemaNetAddress*	GetNetAddressTableRow() const;

public:
	uint64						GetNetMilliSecond();
	double						GetNetTickMultiply() const;
	void						PrintServerTime();

public:
	void						OnExitGame();

public:
	MProtocalAuth&				GetProtocalAuth() { return _protocalAuth; }
	MResultAuth&				GetResultAuth() { return _resultAuth; }

public:
	MProtocalGateway&			GetProtocalGateway() { return _protocalGateway; }
	MResultGateway&				GetResultGateway() { return _resultGateway; }

public:
	MProtocalWorld&				GetProtocalWorld() { return _protocalWorld; } 
	MResultWorld&				GetResultWorld() { return _resultWorld; }

public:
	FGsNetCoolTimeSend&			GetCoolTime() {	return _coolTimeHandler; }		// 패킷 전송시 쿨타임제어를 위해

public:
	const FGsServerSelectData&	GetServerSelectData() const { return _serverSelectData; }
	UserDBId					GetSelectUserDBId() const { return _selectUserDBId; }

	const uint64				GetVId() const { return _vID; }
	const FString&				GetLoginId() const { return _loginId; }
	const WorldId				GetWorldId() const { return _serverSelectData._worldId; }
	const FString&				GetServerName() const { return _serverSelectData._serverName; }
	const FString&				GetClientIpAddress() const { return _serverSelectData._clientIpAddress; }

	AuthTicket					GetReconnectTicket() const { return _reconnectTicket; }
	AccountDBId					GetAccoutDBId() const { return _serverSelectData._accoutId; }
	//bool						GetIsAutoLogin() const		{ return _isAutoLogin; }
	int32						GetReturnToStageType() const { return _returnToStageType; }
	FGsNetAutoLogin*			GetAutoLogin() { return _autoLogin; }
	const FGsNetTimeSynchronizer& GetTimeSync() const { return _netTimeSync; }
	uint16						GetPlanetWorldId() const { return _planetWorldId; }
	uint16						GetBeforePatchPlanetWorldId() const { return _beforePatchPlanetWorldId; }

	const TArray<ServerElem>&	GetSeverList() { return _serverList; }

	const FString&				GetServerRevisionInfo() const { return _serverRevision; }
	//uint16						GetSelectPlanetWorldId() const { return _selectPlanetWorldId; }

	const FString&				GetAccountName() const { return _accountName; }
	const FString&				GetSelectPlanetWorldName() const { return _selectPlanetWorldName; }

	bool						GetIsRegisterDevice() const { return _registerDevice; }

public:
	bool						IsLobbyHandshakeCompleted();

public:
	bool						SetServerList(PD::AC::PKT_AC_ACK_LOGIN_READ* Packet);
	bool						SetServerList(PD::AC::PKT_AC_ACK_SELECT_SERVER_READ* inPacket);
	void						SetPreLogin(PD::AC::PKT_AC_ACK_PRE_LOGIN_READ* inPacket);
	bool						TryGetServerName(WorldId inWorldId, OUT FString& outServerName);
	//bool						TryGetZpayPreLoginServerName(WorldId inWorldId, FString InServerAppendixName, OUT FString& outServerName);
	bool						GetPlanetWorldIdName(PlanetWorldId inPlanetWorldId, OUT FString& outServerName);
	bool						TryGetHiveAnalyticsServerId(WorldId inWorldId, OUT FString& outServerId);

public:
	FGsNet::Mode				GetCurrentNetType() const;

 public:
	 bool						IsBlockingSend();

public:
	uint16						GetAdvReservationPlanetWorldId() const { return _advReservationPlanetWorldId; }
	const FString&				GetAdvReservationUserName() const { return _advReservationUserName; }
	void						SetAdvReservationUserName(IN const FString& inAdvReservationUserName) { _advReservationUserName = inAdvReservationUserName; }

protected:
	bool						IsVaildServerId(uint16 inServerId);

public:
	void						SetRestMode(bool InIsRest);

	TWeakPtr<FGsNetErrorHandler> GetErrorHandler() { return _errorHandler; }
};

#define GNet()	UGsScopeHolder::GetGlobalManagerFType<FGsNetManager>(UGsScopeGlobal::EManagerType::Net)
#define GPacketErrorHandler() GNet()->GetErrorHandler().Pin()

