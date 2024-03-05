#pragma once

#include "Classes/GsMessageHandlerOneParam.h"
#include "Classes/GsSingleton.h"
#include "GsMessageParam.h"
#include "Management/GsMessageHolder.h"
#include "Management/GsScopeHolder.h"
#include "Management/ScopeGame/GsSealManager.h"
#include "Message/GsMessageContents.h"
#include "Seal/GsSealSlot.h"
#include "Shared/Client/SharedEnums/SharedItemEnum.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "T1Project.h"

/** 봉인 메세지 베이스 */
template<MessageContentSeal InMessageId>
struct TGsSealMessageParamBase : public IGsMessageParam
{
	static constexpr MessageContentSeal MESSAGE_ID = InMessageId;

protected:
	TGsSealMessageParamBase()
	{
	}

	virtual ~TGsSealMessageParamBase() = default;

public:
	/** 실제 메세지 발생은 이 함수를 호출해야 발생함. */
	virtual void Send() const
	{
		FGsMessageHolder* MessageHolder = GMessage();
		if (nullptr == MessageHolder)
		{
			GSLOG(Error, TEXT("nullptr == MessageHolder"));
			return;
		}

		MessageHolder->GetSeal().SendMessage(MESSAGE_ID, this);
	}
};

/** [명령] Seal 상태가 아니면 상태를 변경하고, Seal 창을 특정 파라미터와 함께 오픈. */
struct FGsSealMessageParamOpenSeal final : public TGsSealMessageParamBase<MessageContentSeal::OPEN_SEAL>
{
	const int32 _sealPackId;

	explicit FGsSealMessageParamOpenSeal(const int32 InSealPackId)
		: _sealPackId(InSealPackId)
	{
	}
	virtual ~FGsSealMessageParamOpenSeal() = default;
};

/** [패킷] 봉인 정보 */
struct SealPacketData
{
	const SealPackId _sealPackId;
	const TArray<SealData> _sealDatas;
	const TArray<SealSlotData> _sealSlotDatas;
	const TArray<SealBonusData> _sealBonusDatas;

	SealPacketData()
	: _sealPackId(INVALID_SEAL_PACK_ID)
	{
	}

	SealPacketData(const SealPackId InSealPackId, const TArray<SealData>& InSealDatas, const TArray<SealSlotData>& InSealSlotDatas, const TArray<SealBonusData>& InSealBonusDatas)
		: _sealPackId(InSealPackId)
		, _sealDatas(InSealDatas)
		, _sealSlotDatas(InSealSlotDatas)
		, _sealBonusDatas(InSealBonusDatas)
	{
#if WITH_EDITOR
		GSLOG(Log, TEXT("FGsSealMessageParamSeal(), _sealDatas.Num():%d, _sealSlotDatas.Num():%d, _sealBonusDatas.Num():%d"), _sealDatas.Num(), _sealSlotDatas.Num(), _sealBonusDatas.Num());
		for (int32 i = 0; i < _sealDatas.Num(); ++i)
		{
			GSLOG(Log, TEXT("_sealDatas[%d]: mSealId:%d, mLevel:%d, mExp:%d"), i, _sealDatas[i].mSealId, _sealDatas[i].mLevel, _sealDatas[i].mExp);
		}

		for (int32 i = 0; i < _sealSlotDatas.Num(); ++i)
		{
			GSLOG(Log, TEXT("_sealSlotDatas[%d]: mSealId:%d, mSealSlotId:%d, mSealSlotColor:%s, mStatType:%s, mStatValue:%d"), i, _sealSlotDatas[i].mSealId, _sealSlotDatas[i].mSealSlotId, *SealSlotColorEnumToString(_sealSlotDatas[i].mSealSlotColor), *StatTypeEnumToString(_sealSlotDatas[i].mStatType), _sealSlotDatas[i].mStatValue);
		}

		for (int32 i = 0; i < _sealBonusDatas.Num(); ++i)
		{
			GSLOG(Log, TEXT("_sealBonusDatas[%d]: mSealSlotColor:%s, mCount:%d"), i, *SealSlotColorEnumToString(_sealBonusDatas[i].mSealSlotColor), _sealBonusDatas[i].mCount);
		}
#endif // WITH_EDITOR
	}
	SealPacketData(const SealPacketData& InSealPacketData)
		: SealPacketData(InSealPacketData._sealPackId, InSealPacketData._sealDatas, InSealPacketData._sealSlotDatas, InSealPacketData._sealBonusDatas)
	{
	}
};

struct FGsSealMessageParamSeal final : public TGsSealMessageParamBase<MessageContentSeal::SEAL>
{
	const TArray<SealPacketData>& _sealPacketData;

	FGsSealMessageParamSeal(const TArray<SealPacketData>& InSealPacketDatas)
	: _sealPacketData(InSealPacketDatas)
	{
		//_sealPacketData.Empty();

		//for (SealPacketData PacketData : InSealPacketDatas)
		//{
		//	_sealPacketData.Emplace(PacketData._sealPackId, PacketData._sealDatas, PacketData._sealSlotDatas, PacketData._sealBonusDatas);
		//}
	}

	virtual ~FGsSealMessageParamSeal() = default;
};

struct SealSlotLock
{
	SealSlotId _sealSlotId;
	bool _isStatLock;
	bool _isColorLock;
};

/** [패킷] 서버에서 봉인 슬롯 잠금 데이터 수신 */
struct FGsSealMessageParamSealSlotLock final : public TGsSealMessageParamBase<MessageContentSeal::SEAL>
{
	TArray<SealSlotLock> _sealSlotLockList;

	FGsSealMessageParamSealSlotLock(const TArray<SealSlotLock>& InSealSlotLockList)
		: _sealSlotLockList(InSealSlotLockList)
	{
	}

	virtual ~FGsSealMessageParamSealSlotLock() = default;
};

/** [패킷] 봉인 레벨업 응답 */
struct FGsSealMessageParamAckSealLevelUp final : public TGsSealMessageParamBase<MessageContentSeal::ACK_SEAL_LEVEL_UP>
{
	const bool _packetNoError;
	const SealPackId _sealPackId;
	const SealData& _sealNode;

	FGsSealMessageParamAckSealLevelUp(const bool InPacketNoError, const SealPackId InSealPackId, const SealData& InSealNode)
		: _packetNoError(InPacketNoError)
		, _sealPackId(InSealPackId)
		, _sealNode(InSealNode)
	{
#if WITH_EDITOR
		GSLOG(Log, TEXT("FGsSealMessageParamSealLevelUp(), _packetNoEr:%d, _sealPackId:%d  _sealNode.mSealId:%d, _sealNode.mLevel:%d, _sealNode.mExp:%d"), _packetNoError, _sealPackId, _sealNode.mSealId, _sealNode.mLevel, _sealNode.mExp);
#endif // WITH_EDITOR
	}

	virtual ~FGsSealMessageParamAckSealLevelUp() = default;
};

/** [패킷] 봉인 슬롯 전체 변경 응답 */
struct FGsSealMessageParamAckSealSlotChangeAll final : public TGsSealMessageParamBase<MessageContentSeal::ACK_SEAL_SLOT_CHANGE_ALL>
{
	const bool _packetNoError;
	const SealPackId _sealPackId;
	const TArray<SealSlotData>& _sealSlotDatas;
	const TArray<SealBonusData>& _sealBonusDatas;

	FGsSealMessageParamAckSealSlotChangeAll(const bool InPacketNoError, const SealPackId InSealPackId, const TArray<SealSlotData>& InSealSlotDatas, const TArray<SealBonusData>& InSealBonusDatas)
		: _packetNoError(InPacketNoError)
		, _sealPackId(InSealPackId)
		, _sealSlotDatas(InSealSlotDatas)
		, _sealBonusDatas(InSealBonusDatas)
	{
#if WITH_EDITOR
		GSLOG(Log, TEXT("FGsSealMessageParamSealSlotChangeAll(), _packetNoEr:%d, SealPackId : %d,  _sealSlotDatas.Num():%d, _sealBonusDatas.Num():%d"), _packetNoError, _sealPackId, _sealSlotDatas.Num(), _sealBonusDatas.Num());

		for (int32 i = 0; i < _sealSlotDatas.Num(); ++i)
		{
			GSLOG(Log, TEXT("_sealSlotDatas[%d]: mSealId:%d, mSealSlotId:%d, mSealSlotColor:%s, mStatType:%s, mStatValue:%d"), i, _sealSlotDatas[i].mSealId, _sealSlotDatas[i].mSealSlotId, *SealSlotColorEnumToString(_sealSlotDatas[i].mSealSlotColor), *StatTypeEnumToString(_sealSlotDatas[i].mStatType), _sealSlotDatas[i].mStatValue);
		}

		for (int32 i = 0; i < _sealBonusDatas.Num(); ++i)
		{
			GSLOG(Log, TEXT("_sealBonusDatas[%d]: mSealSlotColor:%s, mCount:%d"), i, *SealSlotColorEnumToString(_sealBonusDatas[i].mSealSlotColor), _sealBonusDatas[i].mCount);
		}
#endif // WITH_EDITOR
	}

	virtual ~FGsSealMessageParamAckSealSlotChangeAll() = default;
};

/** [패킷] 봉인 슬롯 전체 변경 응답 */
struct FGsSealMessageParamTrySetRerollSlotUnlock final : public TGsSealMessageParamBase<MessageContentSeal::TRY_SEAL_SLOT_REROLL_UNLOCK>
{
	const SealPackId _sealPackId;
	const SealId _sealNodeId;
	const SealSlotId _sealSlotId;
	const EGsSealRerollUnlockTarget _unlockTarget;
	const bool _sealSlotUnlock;

	FGsSealMessageParamTrySetRerollSlotUnlock(const SealPackId InSealPackId, const SealId InSealNodeId, const SealSlotId InSealSlotId, const EGsSealRerollUnlockTarget InUnlockTarget, const bool InSealSlotUnlock)
		: _sealPackId(InSealPackId)
		, _sealNodeId(InSealNodeId)
		, _sealSlotId(InSealSlotId)
		, _unlockTarget(InUnlockTarget)
		, _sealSlotUnlock(InSealSlotUnlock)
	{
		GSLOG(Log, TEXT("FGsSealMessageParamRerollSealSlotUnlock(), _sealPackId:%d, _sealNodeId:%d, _sealSlotId:%d, _unlockTarget:%d, _sealSlotUnlock:%d"), _sealPackId, _sealNodeId, _sealSlotId, _unlockTarget, _sealSlotUnlock ? 1 : 0);
	}

	virtual ~FGsSealMessageParamTrySetRerollSlotUnlock() = default;
};

/** [패킷] 봉인 슬롯 잠금 데이터 저장 응답 */
struct FGsSealMessageParamSlotLockSave final : public TGsSealMessageParamBase<MessageContentSeal::ACK_SEAL_SLOT_LOCK_SAVE>
{
	FGsSealMessageParamSlotLockSave() = default;
	virtual ~FGsSealMessageParamSlotLockSave() = default;
};