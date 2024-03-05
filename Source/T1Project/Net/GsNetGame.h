#pragma once
#include "GsNetBase.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "Util/GsTimeUtil.h"
#include "Message/GsMessageSystem.h"
#include "Shared/Shared/SharedPackets/PD_Gateway_Client.h"
#include "../Message/GsMessageSystem.h"

class FGsNetGame : public FGsNetBase, public FGsNetBase::ISessionNotify
{
	using Super = FGsNetBase;	

	// 재연결 상태
	enum ReconnectionState
	{
		RECONNECTION_STATE_POSSIBLE,														// 재연결 가능
		RECONNECTION_STATE_IMPOSSIBLE,														// 재연결 불가능
		RECONNECTION_STATE_CONNECTING,														// 소켓 연결 중
		RECONNECTION_STATE_CONNECTED														// 소켓 연결 완료
	};

	static constexpr double								LIMIT_TIME_SEND_PING = 1.0;			// 핑을 보내는 시간 간격
	double												_sendPingLastTime = 0.0;			// 핑을 보낸 마지막 시간

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// * 핑퐁 프로세스 설명
	//  - 클라이언트는 LIMIT_TIME_SEND_PING 간격으로 _sendPingSeq 를 증가시키면서 핑을 전송합니다.
	//  - 서버는 핑 수신 시, 받은 시퀀스 값을 그대로 담아 퐁을 전송합니다.
	//  - 클라이언트는 퐁 수신 시, 받은 시퀀스 값을 _recvPongSeq 에 기록합니다.
	//
	// * 핑퐁 시퀀스 값을 이용한 연결 끊김 감지 규칙
	//  - 핑 전송 시, _sendPingSeq" 와 _recvPongSeq 격차가 LIMIT_PING_PONG_SEQ_DELTA 보다 크면 '통신 장애',
	//  - 퐁 수신 시, 가지고 있는 "퐁 시퀀스" 값과 비교하여 순서에 맞지 않으면 '통신 장애' -
	//  - 로 간주하여 네트워크 Disconnection 처리를 합니다
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	static constexpr uint32								LIMIT_PING_PONG_SEQ_DELTA = 5;		// 최대 퐁-핑 시퀀스 차이		
	uint32												_sendPingSeq = 0;					// 핑을 보낸 시퀀스
	uint32												_recvPongSeq = 0;					// 퐁을 보낸 시퀀스	

	bool												_isReconnectionStarted = false;		// 재연결 프로세스 시작 여부
	bool												_isSocketReconnecting = false;		// 소켓 재연결 중 여부
	int32												_socketReconnectCount = 0;			// 소켓 재연결 시도 횟수
	FGsSimpleTimer										_socketReconnectTImer;				// 소켓 재연결 타이머
	FGsSimpleTimer										_reqSendReconnectTimer;				// 재연결 요청 타이머

	FGsSimpleTimer										_reqGCAckLoginTimer;

	FGsSimpleTimer										_reqSendSyncWorldTimeTimer;			// 월드 시간 싱크 타이머
	bool												_isHandshakeCompleted = false;
	TArray<TPair<PD::GC::GCPacketId, FDelegateHandle>>	_listGcPacketDelegate;
	MsgSystemHandle										_systemMessageRetryGCAckLogin;
public:
	FGsNetGame();
	virtual ~FGsNetGame() = default;

public:
	virtual void Enter() override;
	virtual void Exit() override;
	virtual void Update(float indeltaTime) override;

	virtual void ReConnect() override;

	virtual void SetRestMode(bool InIsRest) override;
public:
	virtual void Send(int size, bool ignoreSendObserver = false) override;

private:
	void SendGatewayPing();

private:
	void RecvGatewayPongAck(const FGsNet::Packet& inPacket);
	void RecvReconnectAck(const FGsNet::Packet& inPacket);
	void RecvKickedByReason(const FGsNet::Packet &inPacket);	

private:
	void BackToTitle(FText message = FText::GetEmpty());
	void ReconnectSocket();
	void OnReconnectSocketByTimer();
	void OnReqSendReconnectByTimer();
	void OnReqSendSyncWorldTimeByTimer();

	void OnRetryGateWayLogin();
	void OnReqRetrySendGatewayLoginByTimer();

private:
	// 재연결 - 가능 / 불가능 / 진행 중 - 상태를 체크하여 반환
	ReconnectionState GetReconnectionState() const;

public:
	virtual void OnHandshakeCompleted() override;
	virtual void OnKeepAliveCheck() override;
	virtual void OnDisConnected(const FClientSession* inSession) override;
	virtual void OnConnected(const FClientSession* inSession) override;
	virtual void OnPacketReceived(unsigned char*, int) override;
	virtual void OnErrorRecevied(int32 inErrorCode) override;
	virtual void OnExitGame() override;
	virtual void OnAddressQueryRecevied(const FAddressInfoResult&) override;
	virtual uint32 GetPort() override { return _port; }
};