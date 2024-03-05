// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Classes/GsManager.h"

#include "Management/GsScopeHolder.h"

#include "Message/GsMessageGameObject.h"
#include "Message/GsMessageSystem.h"
#include "Message/GsMessageInvasion.h"

#include "EventProgress/GsEventProgressInterface.h"

#include "Shared/Shared/SharedInclude/SharedTypes.h"


struct FGsSchemaPersonalTradingConfig;

namespace PD { namespace SC { 
	struct PKT_SC_ACK_PERSONAL_TRADING_TRY_READ;
	struct PKT_SC_INFORM_PERSONAL_TRADING_READ;
} }

/**
 * 개인 거래 시스템 매니저
 */

enum class PersonalTradingState
{
	NONE,		// 아무것도 아닌 상태
	WAIT,		// 대기중인 상태
	TRADING,	// 거래중인 상태
	LOCK,		// 잠금된 상태
	CONFIRM,	// 최종 확인한 상태
};

enum class PersonalTradingDirtyUI
{
	NONE,
	REMOTE,
	REMOTE_ITEM,
	LOCAL,
	LOCAL_ITEM,
	ALL,
};

enum class PersonalTradingCancelType
{
	NONE,
	CLICK,
	INVADE,
	TIME_OVER,
};

class T1PROJECT_API FGsPersonalTradingManager final : public IGsManager, public IGsEventProgressInterface
{
protected:
	// 거래 요청자 : 나 or 상대
	int64 _ownerGameId;

	// 거래 상대 Game Id
	int64 _remoteGameId;
	// 거래 상대 이름
	FString _remoteUserName;

	// 개인 거래 상태
	PersonalTradingState _remoteState = PersonalTradingState::NONE;
	PersonalTradingState _localState = PersonalTradingState::NONE;

	// 거래 상대가 올린 아이템 정보
	TArray<ItemDataEquip> _remoteItemEquipDataList;
	TArray<ItemData> _remoteItemDataList;
	TArray<ItemDBId> _remoteItemDBIdList;
	// 거래 상대가 올린 재화 정보 (다이아)
	Gem _remoteDiaAmount;

	// 내가 올린 아이템 DBID, Amount
	TArray<TPair<ItemDBId, ItemAmount>> _localItemDataList;
	// 요청중인 아이템 DBID
	TArray<TPair<ItemDBId, ItemAmount>> _localItemDataWaitList;
	// 내가 올린 재화 정보 (다이아)
	Gem _localDiaAmount;
	// 요청중인 재화 정보 (다이아)
	Gem _localDiaAmountWait;

	bool _packetSend = false;
	PersonalTradingCancelType _cancelType = PersonalTradingCancelType::NONE;

	// Config
	const FGsSchemaPersonalTradingConfig* _personalTradingConfig = nullptr;

	MsgGameObjHandleArray _msgGameObjectHandlerList;
	MsgSystemHandleArray _msgSystemHandlerList;
	TArray<TPair<MessageInvasion, FDelegateHandle>> _msgInvasionHandlerList;

public:
	virtual void Initialize() override;
	virtual void Finalize() override;

	void Reset(bool InIsUpdate = true);

public:
	void ClearPersonalTrading();

public:
	// Config
	Level GetTradeOpenLevel();
	int32 GetApproveProgressTimeSec();
	int32 GetDailyTradeNumberMax();
	CurrencyDeltaPair GetTradeChargeData();		// 기본 거래 수수료
	int32 GetSlotCountMax();

	void ResetTimeContents(ResetTimeTicket InType);

	// Target Getter
	int64 GetRemoteGameId();
	FString& GetRemoteUserName();
	PersonalTradingState GetRemotePersonalTradingState();
	ItemDBId GetRemoteItemDBId(int32 InIndex);
	const ItemDataEquip* GetRemoteItemDataEquip(ItemDBId InItemDBId);
	const ItemData* GetRemoteItemData(ItemDBId InItemDBId);
	Gem GetRemoteDiaAmount();

	// Local Getter
	PersonalTradingState GetLocalPersonalTradingState();
	TArray<TPair<ItemDBId, ItemAmount>> GetLocalItemDataList();
	int32 GetLocalItemDataWaitListCount() { return _localItemDataWaitList.Num(); }
	Gem GetLocalDiaAmount();
	int32 GetLocalRemainDailyTradeNumber();
	bool IsRequestingItem(ItemDBId InItemDBId);
	bool IsRequestingDia();
	bool IsSelectedItem(ItemDBId InItemDBId);

	int64 GetOwnerGameId() { return _ownerGameId; }
	bool IsRemoteTrader(int64 InGameId);
	bool IsTradeFromMe();
	CurrencyDeltaPair GetMyTradeChargeData();	// 현재 거래의 나의 수수료
	bool IsTradeOpen();
	bool IsTradeSet();

	// 기타
	void SetCancelType(PersonalTradingCancelType InType) { _cancelType = InType; }

protected:
	void SetRemoteUserInfo(int64 InGameId, const FString& InUserName);
	void SetRemotePersonalTradingState(PersonalTradingState InState);

	void SetLocalPersonalTradingState(PersonalTradingState InState);

	bool CanOpenTrade();

	// 패킷 송신 처리
public:
	void TryReqPersonalTradingTry(int64 InTargetGameId, const FString InTargetUserName); // 상대에게 거래 신청 (거래전)
	void TryReqPersonalTradingAccept();		// 거래 승락
	bool TryReqPersonalTradingCancel();		// 거래 취소
	void TryReqPersonalTradingLock();		// 거래 잠금 요청
	void TryReqPersonalTradingLockCancel();	// 거래 잠금 해제 요청
	void TryReqPersonalTradingConfirm();	// 거래 확인 요청
	void TryReqPersonalTradingConfirmCancel();	// 거래 확인 해제 요청
	bool TryReqPersonalTradingAbort();		// 거래 중지 요청

	// 아이템 정보 갱신 요청 (추가/변경/제거)
	void ReqLocalItemUpdate(ItemDBId InItemDBId, ItemAmount InAmount);
	// 재화 정보 갱신 요청
	void ReqLocalDiaAmount(Gem InAmount);

	// 패킷 수신 처리
public:
	void AckPersonalTradingTry(PD::SC::PKT_SC_ACK_PERSONAL_TRADING_TRY_READ& InPacket);	// 거래 신청 Ack
	void AckPersonalTradingAccept(int32 InResult);	// 거래 승락 Ack
	void AckPersonalTradingCancel();	// 거래 취소 Ack
	void AckPersonalTradingLock();		// 거래 잠금/해제 요청 Ack
	void AckPersonalTradingConfirm();	// 거래 확인/해제 요청 Ack
	void AckPersonalTradingAbort();		// 거래 중지 Ack
	void AckLocalItemUpdate(bool InIsSuccess);	// 아이템 정보 갱신 Ack
	void AckLocalDiaAmount(bool InIsSuccess);	// 재화 정보 갱신 Ack

	void NotifyPersonalTrading(PD::SC::PKT_SC_INFORM_PERSONAL_TRADING_READ& InPacket);
	// 상대 아이템 정보 갱신 알림
	void NotifyPersonalTradingItemUpdate(int64 InTargetGameId, const ItemData& InItemData, ItemAmount InAmount);
	// 상대 장비 아이템 정보 갱신 알림
	void NotifyPersonalTradingItemEquipUpdate(int64 InTargetGameId, const ItemDataEquip& InItemData);
	// 상대 등록된 아이템 제거 알림
	void NotifyPersonalTradingItemRemove(int64 InTargetGameId, ItemDBId InItemDBId);
	// 상대 재화 정보 갱신 알림
	void NotifyPersonalTradingDiaUpdate(int64 InTargetGameId, Gem InAmount);

protected:
	// 개인거래 관련 패킷을 보내고나서 받기 전까지 취소 외의 패킷 전송을 막는다.
	bool IsAlreadyPacketSend() { return _packetSend; }
	void SetIsPacketSend(bool InIsSend) { _packetSend = InIsSend; }

protected:
	void OnLocalPlayerDie(const struct IGsMessageParam* InParam);
	void OnLocalSpawnMeComplete(const struct IGsMessageParam* InParam);
	void OnBeforeGameToLobby();
	void OnReconnectEnd();
	void OnInvasionStart(const struct IGsMessageParam* InParam);

public:
	void ProcessNetError(int32 InResult);
	FText GetErrorText(int32 InResult);
	void ShowTicker(const FTextKey& InNamespace, const FTextKey& InKey);

	// 이벤트 진행 우선순위
public:
	virtual void StartEvent(EGsEventProgressType In_startType, EGsEventProgressType In_preType) override;
	virtual void FinishEvent(EGsEventProgressType In_finishType, EGsEventProgressType In_nextType) override;
	virtual void OnCanceledStartEvent() override;

};

#define GSPersonalTrading() UGsScopeHolder::GetGameManagerFType<FGsPersonalTradingManager>(UGsScopeGame::EManagerType::PersonalTrading)