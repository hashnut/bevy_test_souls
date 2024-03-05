#pragma once

#include "Classes/GsMessageHandlerOneParam.h"
#include "GsMessageParam.h"
#include "Management/GsMessageHolder.h"
#include "Management/ScopeGlobal/GsNetManager.h"
#include "Message/GsMessageContents.h"
#include "Net/GSNet.h"
#include "Net/GsNetBase.h"
#include "Net/GsNetBuffer.h"
#include "Shared/Client/SharedEnums/SharedItemEnum.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "Shared/Shared/SharedPackets/PD_Client_World_Bypass.h"
#include "T1Project.h"


/** 천마석 메시지 베이스 */
template<MessageContentSpiritShot InMessageId>
struct TGsSpiritShotParamBase : public IGsMessageParam
{
	static constexpr MessageContentSpiritShot MESSAGE_ID = InMessageId;

protected:
	TGsSpiritShotParamBase()
	{
	}

	virtual ~TGsSpiritShotParamBase()
	{
	}

public:
	/** 실제 메시지 발생은 이 함수를 호출해야 발생함 */
	virtual void Send() const
	{
		FGsMessageHolder* messageHolder = GMessage();
		if (nullptr == messageHolder)
		{
			GSLOG(Error, TEXT("nullptr == messageHolder"));
			return;
		}

		messageHolder->GetSpiritShot().SendMessage(MESSAGE_ID, this);
	}
};

/** 천마석 패킷 센더 전용 메시지 베이스 */
template<MessageContentSpiritShot InMessageId, typename TPacket>
struct TGsSpiritShotMessageParamNetSendBase : public TGsSpiritShotParamBase<InMessageId>
{
private:
	TWeakPtr<FGsNetBase> _netBase;
	TSharedPtr<TPacket> _packet;

public:
	template<typename... TParams>
	explicit TGsSpiritShotMessageParamNetSendBase(TParams... InParams)
		: _netBase(TGsSpiritShotMessageParamNetSendBase::GetActiveNetBase())
		, _packet(MakePacket(InParams...))
	{
	}
	virtual ~TGsSpiritShotMessageParamNetSendBase() = default;

	void Send() const final
	{
		if (!_packet.IsValid() || !_netBase.IsValid())
		{
			GSLOG(Error, TEXT("!_packet.IsValid() || !_netBase.IsValid()"));
			return;
		}

		_netBase.Pin()->Send(_packet->mPktWriter.GetPktSize());
	}

private:
	static TWeakPtr<FGsNetBase> GetActiveNetBase()
	{
		TWeakPtr<FGsNetBase> weakNetBase;
		const FGsNetManager* netManager = GNet();
		if (nullptr == netManager)
		{
			GSLOG(Error, TEXT("nullptr == netManager"));
		}
		else
		{
			TSharedPtr<FGsNetBase> netBase = netManager->GetActive();
			if (!netBase.IsValid())
			{
				GSLOG(Error, TEXT("!netBase.IsValid()"));
			}
			else
			{
				weakNetBase = netBase;
			}
		}

		return weakNetBase;
	}

	template<typename... TParams>
	TSharedPtr<TPacket> MakePacket(TParams... InParams) const
	{
		TSharedPtr<TPacket> packet;
		if (_netBase.IsValid())
		{
			TSharedPtr<FGsNetBase> netBase = _netBase.Pin();
			FGsNetBuffer* netBuffer = netBase->GetLocalSendBufferPtr();
			if (nullptr != netBuffer)
			{
				packet = MakeShared<TPacket>(netBuffer->GetBuffer(), netBuffer->GetSize(), InParams...);
			}
		}
		return MoveTemp(packet);
	}
};

/** [명령] SpiritShot 상태가 아니면 상태를 변경하고, SpiritShot 창을 특정 파라미터와 함께 오픈 */
struct FGsSpiritShotMessageParamOpen final : public TGsSpiritShotParamBase<MessageContentSpiritShot::OPEN_SPIRIT_SHOT>
{
	const int32 _spiritShotCategoryId;

	explicit FGsSpiritShotMessageParamOpen(const int32 InSpiritShotCategoryId)
		: _spiritShotCategoryId(InSpiritShotCategoryId)
	{
	}
	virtual ~FGsSpiritShotMessageParamOpen() = default;
};

/** [패킷] 천마석 정보 */
struct FGsSpiritShotMessageParamInfo final : public TGsSpiritShotParamBase<MessageContentSpiritShot::SPIRIT_SHOT_ENCHANT_INFO>
{
	const TArray<SpiritShotLevelId> _spiritShotLevelIdList;

	explicit FGsSpiritShotMessageParamInfo(const TArray<SpiritShotLevelId>& InSpiritShotLevelIdList)
		: _spiritShotLevelIdList(InSpiritShotLevelIdList)
	{
#if WITH_EDITOR
		for (int32 i = 0; i < _spiritShotLevelIdList.Num(); ++i)
		{
			GSLOG(Log, TEXT("_spiritShotLevelIdList[%d]: %d"), i, _spiritShotLevelIdList[i]);
		}
#endif // WITH_EDITOR
	}
	virtual ~FGsSpiritShotMessageParamInfo() = default;
};

/** [패킷] 천마석 강화 요청 */
using FGsSpiritShotMessageParamReqEnchant = TGsSpiritShotMessageParamNetSendBase<MessageContentSpiritShot::REQ_SPIRIT_SHOT_ENCHANT, PD::CS::PKT_CS_REQ_SPIRIT_SHOT_ENCHNT_WRITE>;

/** [패킷] 천마석 강화 응답 */
struct FGsSpiritShotMessageParamAckEnchant final : public TGsSpiritShotParamBase<MessageContentSpiritShot::ACK_SPIRIT_SHOT_ENCHANT>
{
	const bool _packetNoError;
	SpiritShotCategoryId _spiritShotCategoryId;
	SpiritShotLevelId _spiritShotLevelId;
	TArray<ItemIdAmountPair> _itemIdAmountPairList;

	FGsSpiritShotMessageParamAckEnchant(const bool InPacketNoError, const SpiritShotCategoryId InSpiritShotCategoryId, const SpiritShotLevelId InSpiritShotLevelId, const TArray<ItemIdAmountPair>& InItemIdAmountPairList)
		: _packetNoError(InPacketNoError)
		, _spiritShotCategoryId(InSpiritShotCategoryId)
		, _spiritShotLevelId(InSpiritShotLevelId)
		, _itemIdAmountPairList(InItemIdAmountPairList)
	{
#if WITH_EDITOR
		GSLOG(Log, TEXT("FGsSpiritShotMessageParamAckEnchant(), _packetNoError:%d, _spiritShotCategoryId:%d, _spiritShotLevelId:%d"), _packetNoError, _spiritShotCategoryId, _spiritShotLevelId);

		for (int32 i = 0; i < _itemIdAmountPairList.Num(); ++i)
		{
			GSLOG(Log, TEXT("_itemIdAmountPairList[%d]: ItemId:%d, Amount:%d"), i, _itemIdAmountPairList[i].mItemId, _itemIdAmountPairList[i].mItemAmount);
		}
#endif // WITH_EDITOR
	}

	virtual ~FGsSpiritShotMessageParamAckEnchant() = default;
};

/** [연출] 천마석 연출 완료 알림 */
struct FGsSpiritShotMessageParamEnchantEffectFinished final : public TGsSpiritShotParamBase<MessageContentSpiritShot::SPIRIT_SHOT_ENCHANT_EFFECT_FINISED>
{
	explicit FGsSpiritShotMessageParamEnchantEffectFinished() = default;
	virtual ~FGsSpiritShotMessageParamEnchantEffectFinished() = default;
};