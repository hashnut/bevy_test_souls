#pragma once

#include "GsNetBase.h"
#include "GsNetBuffer.h"
#include "Management/ScopeGlobal/GsNetManager.h"
#include "Message/GsMessageContents.h"
#include "Message/MessageParam/GsSealMessageParam.h"
#include "Shared/Shared/SharedPackets/PD_Client_World_Bypass.h"
#include "T1Project.h"

/** [패킷] 디버그 로깅용 */
#if !NO_LOGGING
FString LexToString(const PD::CS::PKT_CS_REQ_SEAL_LEVEL_UP_WRITE& InPacket);
FString LexToString(const PD::CS::PKT_CS_REQ_SEAL_SLOT_CHANGE_ALL_WRITE& InPacket);
FString LexToString(const PD::CS::PKT_CS_REQ_SEAL_SLOT_LOCK_SAVE_WRITE& InPacket);
#endif

/** 봉인 패킷 센더 전용 메세지 베이스 */
template<MessageContentSeal InMessageId, typename TPacket>
struct TGsSealMessageParamNetSendBase : public TGsSealMessageParamBase<InMessageId>
{
private:
	TWeakPtr<FGsNetBase> _netBase;
	TSharedPtr<TPacket> _packet;

public:
	template<typename... TParams>
	explicit TGsSealMessageParamNetSendBase(TParams... InParams)
		: _netBase(TGsSealMessageParamNetSendBase::GetActiveNetBase())
		, _packet(MakePacket(InParams...))
	{
	}

	virtual ~TGsSealMessageParamNetSendBase() = default;

	void Send() const final
	{
		if (!_packet.IsValid() || !_netBase.IsValid())
		{
			GSLOG(Error, TEXT("!_packet.IsValid() || !_netBase.IsValid()"));
			return;
		}

#if !NO_LOGGING
		GSLOG(Log, TEXT("%s"), *LexToString(*_packet.Get()));
#endif

		_netBase.Pin()->Send(_packet->mPktWriter.GetPktSize());
	}

private:
	static TWeakPtr<FGsNetBase> GetActiveNetBase()
	{
		TWeakPtr<FGsNetBase> WeakNetBase;
		const FGsNetManager* NetManager = GNet();
		if (nullptr == NetManager)
		{
			GSLOG(Error, TEXT("nullptr == NetManager"));
		}
		else
		{
			TSharedPtr<FGsNetBase> NetBase = NetManager->GetActive();
			if (!NetBase.IsValid())
			{
				GSLOG(Error, TEXT("!NetBase.IsValid()"));
			}
			else
			{
				WeakNetBase = NetBase;
			}
		}
		return WeakNetBase;
	}

	template<typename... TParams>
	TSharedPtr<TPacket> MakePacket(TParams... InParams) const
	{
		TSharedPtr<TPacket> Packet;
		if (_netBase.IsValid())
		{
			TSharedPtr<FGsNetBase> NetBase = _netBase.Pin();
			FGsNetBuffer* NetBuffer = NetBase->GetLocalSendBufferPtr();
			if (nullptr != NetBuffer)
			{
				Packet = MakeShared<TPacket>(NetBuffer->GetBuffer(), NetBuffer->GetSize(), InParams...);
			}
		}
		return MoveTemp(Packet);
	}
};

/** [패킷] 봉인 레벨업 요청 */
using FGsSealMessageParamReqSealLevelUp = TGsSealMessageParamNetSendBase<MessageContentSeal::REQ_SEAL_LEVEL_UP, PD::CS::PKT_CS_REQ_SEAL_LEVEL_UP_WRITE>;

/** [패킷] 봉인 슬롯 전체 변경 요청 */
using FGsSealMessageParamReqSealSlotChangeAll = TGsSealMessageParamNetSendBase<MessageContentSeal::REQ_SEAL_SLOT_CHANGE_ALL, PD::CS::PKT_CS_REQ_SEAL_SLOT_CHANGE_ALL_WRITE>;

/** [패킷] 봉인 슬롯 잠금 상태 변경 요청 */
using FGsSealMessageParamReqSealSlotLockSave = TGsSealMessageParamNetSendBase<MessageContentSeal::REQ_SEAL_SLOT_LOCK_SAVE, PD::CS::PKT_CS_REQ_SEAL_SLOT_LOCK_SAVE_WRITE>;
