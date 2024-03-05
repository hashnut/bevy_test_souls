#pragma once

#include "GsNet.h"
#include "../Classes/GsState.h"
#include "../Classes/GsMessageHandler.h"
#include "ClientSession.h"
#include "IPAddress.h"
#include "GsNetBuffer.h"

#include "../Message/GsMessageSystem.h"



struct FTimerHandle;
using namespace CNetLib;
//------------------------------------------------------------------------------
// 네트워킹을 하기위한 최소단위
//------------------------------------------------------------------------------
class FGsNetBase : public TGsState<FGsNet::Mode>
{
protected:
	FString						_sessionName = TEXT("NoName");
	TUniquePtr<FGsNetBuffer>	_localSendBuffer;
	TSharedPtr<FInternetAddr>	_addressInfo;
protected:
	TUniquePtr<FClientSession>	_session;
	//FTimerHandle				_checkMaintenanceTimer;			// 점검 체크 타이머

private:
	// 서버로 패킷을 전송하는것을 로깅하는 구간(원인파악용)
	bool						_observerSending = false;
	// 패킷 블러킹
	bool						_sendblock = false;

protected:
	bool						_isReconnectable = false;

protected:
	MsgSystemHandleArray		_msgHandleSystemList;

protected:
	uint32						_port = 7700;

public:
	FGsNetBase() 
		: TGsState<FGsNet::Mode>(FGsNet::Mode::MAX) {}
	explicit FGsNetBase(FGsNet::Mode inMode) 
		: TGsState<FGsNet::Mode>(inMode) {}

	virtual ~FGsNetBase() = default;

public:
	virtual void Enter() override;
	virtual void Exit() override;
	virtual void Update(float indeltaTime) override;

public:
	void InitConnection();
	void SetPort(unsigned short port);
	virtual void ReConnect() {};	
	virtual void OnExitGame() {};	

public:
	FGsNetBuffer* GetLocalSendBufferPtr() const { return _localSendBuffer.Get(); }

public:	
	//bak1210 : SetObserveSend 설정된 구간에서 Send항목을 로깅한다. 단 ignoreSendObserver 활성화시 로깅무시
	virtual void Send(int size, bool ignoreSendObserver = false);
	void SetObserveSend(bool inFlag) { _observerSending = inFlag; }
	void SetReconnectAble(bool inVelue) { _isReconnectable = inVelue; }
	bool GetReconnectAble() const { return _isReconnectable; }

public:
	/* bak1210 다음과 같은 인터페이스를 가진 클래스를 호출시 넣어줍니다.
	   다음과 같이 Template으로 구성한 이유 : => 델리게이트는 호출하는 자신의 주소와 타입의 함수주소를 인자로 받습니다.*/
	class ISessionNotify
	{
	public:
		virtual void OnHandshakeCompleted() {};
		virtual void OnKeepAliveCheck() {};
		virtual void OnDisConnected(const FClientSession* inSession) {};
		virtual void OnConnected(const FClientSession* inSession) {};
		virtual void OnPacketReceived(unsigned char*, int) {};
		virtual void OnErrorRecevied(int32 inErrorCode) {};
		virtual void OnAddressQueryRecevied(const FAddressInfoResult&) {};
		virtual uint32 GetPort() { return 7700; }
	};

protected:
	template <typename TSessionNotifyType>
	bool Connect(TSessionNotifyType* target, bool reconnect = false)
	{
		if(nullptr == _addressInfo)
			return false;

		if (nullptr == _session)
			return false;

		if (false == _session->Initialize(_sessionName))
			return false;

		// 확실하게 Clear
		_session->ClearDelegate();
		_session->Disconnect(false);

		// 핸드쉐이크 완료콜백 설정
		_session->HandShakeDelegate.AddRaw(target, &TSessionNotifyType::OnHandshakeCompleted);

		// 접속 확인용 ping-pong 함수 설정
		_session->ConnectionCheckDelegate.AddRaw(target, &TSessionNotifyType::OnKeepAliveCheck);

		// 세션 접속 완료 콜백
		_session->ConnectionCompleteDelegate.AddRaw(target, &TSessionNotifyType::OnConnected);

		// 세션 접속 종료 콜백
		_session->DisconnectionComplateDelegate.AddRaw(target, &TSessionNotifyType::OnDisConnected);

		// 오류메시지 전달 콜백
		_session->ErrorMessageDelegate.AddRaw(target, &TSessionNotifyType::OnErrorRecevied);

		// 포트번호 입력
		_addressInfo->SetPort(target->GetPort());
			
		// 접속 처리
		if (false == _session->Connect(_addressInfo, reconnect))
			return false;

		// 패킷 핸들러 추가
		_session->PackectHandlerDelegate.AddRaw(target, &TSessionNotifyType::OnPacketReceived);

		return true;
	}


public:
	void Disconnect(bool inNotify = false);

	virtual void SetRestMode(bool InIsRest) {}

public:
	bool IsHandshakeCompleted() const { return _session->IsHandshakeCompleted(); }
	void TestErrorMessageFunc(int32 inErrCode);	//외부에서 메시지가 제대로 동작하는지 확인하기 위한 용처
	void SetSessionName(const FString& name) { _sessionName = name; }
	uint64 GetDId() const;
	bool IsVaildSessionCondition() const;

	template <typename TSessionNotifyType>
	bool GetAddressQuery(TSessionNotifyType* target, const FString& HostStr)
	{
		if (nullptr == _session)
		{
			return false;
		}
		_session->AddressQueryResultDelegate.Clear();

		// 주소 쿼리 전달 콜백
		_session->AddressQueryResultDelegate.AddRaw(target, &TSessionNotifyType::OnAddressQueryRecevied);

		//bak1210 : 안드로이드 에서 적용안됨
		// LogSockets: Warning: GetAddressInfo failed to resolve host with error SE_EPFNOSUPPORT [9]
		//return _session->RunAddressAsyncQuery(HostStr);
		return _session->RunAddressQuery(HostStr);
	}

public:
	void SetPacketSendBlock(bool inSend);
	bool IsBlockingSend();
//protected:
//	// 점검 상태 확인 요청
//	void OnCheckMaintenance();
//	// 점검 상태 체크를 위한 타이머		
//	void StartCheckMaintenanceTimer();
//	void StopCheckMaintenanceTimer();
//
//	// 하이브 CheckMaintenance 요청 실패/성공
//	void HiveCheckMaintenanceResultFailed();
//	void HiveCheckMaintenanceResultSucceeded();
};