// Fill out your copyright notice in the Description page of Project Settings.

#include "GsPersonalTradingManager.h"

#include "DataSchema/PersonalTrading/GsSchemaPersonalTradingConfig.h"

#include "Shared/Shared/SharedPackets/PD_World_Client_Bypass.h"
#include "Shared/Shared/SharedInclude/SharedDefine.h"
#include "Shared/Client/SharedEnums/PD_ResultEnum.h"

#include "Net/GsNetSendServiceWorld.h"

#include "Management/ScopeGlobal/GsGameDataManager.h"
#include "Management/ScopeGlobal/GsUIManager.h"
#include "Management/ScopeGame/GsCoolTimeManager.h"
#include "Management/ScopeGame/GsGameObjectManager.h"
#include "Management/ScopeGame/GsInventoryManager.h"
#include "Management/ScopeGame/GsEventProgressManager.h"
#include "Management/ScopeGame/GsUnlockManager.h"

#include "Management/GsMessageHolder.h"
#include "Message/GsMessageContentHud.h"

#include "UI/UIContent/Helper/GsUIHelper.h"
#include "UI/UIContent/Popup/GsUIPopupChat.h"
#include "UI/UIContent/Popup/GsUIPopupTrade.h"

#include "GameObject/Movement/GsMovementHandlerLocalPlayer.h"
#include "GameObject/Fence/GsFenceHandler.h"

#include "ControllerEx/GsPlayerController.h"

#include "T1Project.h"

#include "Item/GsItemManager.h"

#include "Currency/GsCurrencyHelper.h"



void FGsPersonalTradingManager::Initialize()
{
	if (const UGsTablePersonalTradingConfig* table = Cast<UGsTablePersonalTradingConfig>(FGsSchemaPersonalTradingConfig::GetStaticTable()))
	{
		TArray<const FGsSchemaPersonalTradingConfig*> rowList;
		if (table->GetAllRows(rowList))
		{
			if (rowList.Num() > 0)
			{
				_personalTradingConfig = rowList[0];
			}
		}
	}

	Reset(false);

	FGsMessageHolder* msg = GMessage();

	_msgGameObjectHandlerList.Empty();
	_msgGameObjectHandlerList.Emplace(msg->GetGameObject().AddRaw(MessageGameObject::LOCAL_DIE,
		this, &FGsPersonalTradingManager::OnLocalPlayerDie));
	_msgGameObjectHandlerList.Emplace(msg->GetGameObject().AddRaw(MessageGameObject::LOCAL_SPAWN_ME_COMPLETE,
		this, &FGsPersonalTradingManager::OnLocalSpawnMeComplete));

	_msgSystemHandlerList.Empty();
	_msgSystemHandlerList.Emplace(msg->GetSystem().AddRaw(MessageSystem::BEFORE_GAME_TO_LOBBY,
		this, &FGsPersonalTradingManager::OnBeforeGameToLobby));
	_msgSystemHandlerList.Emplace(msg->GetSystem().AddRaw(MessageSystem::RECONNECT_END,
		this, &FGsPersonalTradingManager::OnReconnectEnd));

	_msgInvasionHandlerList.Empty();
	_msgInvasionHandlerList.Add(msg->GetInvasion().AddRaw(MessageInvasion::INVAION_START,
		this, &FGsPersonalTradingManager::OnInvasionStart));
}

void FGsPersonalTradingManager::Finalize()
{
	_personalTradingConfig = nullptr;

	Reset(false);

	FGsMessageHolder* msg = GMessage();

	for (MsgGameObjHandle& iter : _msgGameObjectHandlerList)
	{
		msg->GetGameObject().Remove(iter);
	}
	_msgGameObjectHandlerList.Empty();

	for (MsgSystemHandle& iter : _msgSystemHandlerList)
	{
		msg->GetSystem().Remove(iter);
	}
	_msgSystemHandlerList.Empty();

	for (TPair<MessageInvasion, FDelegateHandle>& iter : _msgInvasionHandlerList)
	{
		msg->GetInvasion().Remove(iter);
	}
	_msgInvasionHandlerList.Empty();
}

void FGsPersonalTradingManager::Reset(bool InIsUpdate)
{
	_ownerGameId = 0;
	_remoteGameId = 0;
	_remoteUserName.Empty();

	_remoteState = PersonalTradingState::NONE;
	_localState = PersonalTradingState::NONE;

	_remoteItemDataList.Empty();
	_remoteItemEquipDataList.Empty();
	_remoteItemDBIdList.Empty();
	_remoteDiaAmount = 0;

	_localItemDataList.Empty();
	_localItemDataWaitList.Empty();
	_localDiaAmount = 0;
	_localDiaAmountWait = 0;

	_packetSend = false;

	if (InIsUpdate)
	{
		GSEventProgress()->OnReqFinishEvent(EGsEventProgressType::PERSONAL_TRADING);
		GCoolTime()->SetCoolTime(EGsCoolTime::PersonalTrading, 0, 0, 0);
		GMessage()->GetContentsHud().SendMessage(MessageContentHud::INVALIDATE_PERSONAL_TRADING);
		GMessage()->GetPersonalTrading().SendMessage(MessageContentPersoncalTrading::CLOSE);
	}
}

void FGsPersonalTradingManager::AckPersonalTradingTry(PD::SC::PKT_SC_ACK_PERSONAL_TRADING_TRY_READ& InPacket)
{
	ShowTicker(TEXT("TradeText"), TEXT("Notice_RequestTrade"));

	_ownerGameId = GGameData()->GetUserData()->mGameId;
	SetRemotePersonalTradingState(PersonalTradingState::WAIT);
	GCoolTime()->SetCoolTime(EGsCoolTime::PersonalTrading, 0, 0, GetApproveProgressTimeSec());
	GMessage()->GetContentsHud().SendMessage(MessageContentHud::INVALIDATE_PERSONAL_TRADING);
}

void FGsPersonalTradingManager::AckPersonalTradingAccept(int32 InResult)
{
	SetIsPacketSend(false);

	if (PACKET_RESULT_SUCCESS == InResult)
	{

	}
	else
	{
#if WITH_EDITOR
		FString msg = FString::Format(TEXT("Code({0} / {1}"), { InResult, PD::ResultEnumToString((PD::Result)InResult) });
		GSLOG(Error, TEXT("[PersonalTrading] %s"), *msg);
#endif

		FText errorText = GetErrorText(InResult);
		FGsUIHelper::TraySectionMessageTicker(EGsNotiSection::EXTSYSTEM, errorText);

		TryReqPersonalTradingCancel();
		_cancelType = PersonalTradingCancelType::NONE;
	}
}

void FGsPersonalTradingManager::AckPersonalTradingCancel()
{
	if (_cancelType == PersonalTradingCancelType::CLICK)
	{
		ShowTicker(TEXT("TradeText"), TEXT("Notice_CancelTrade"));
	}
	else if (_cancelType == PersonalTradingCancelType::INVADE)
	{
		ShowTicker(TEXT("TradeText"), TEXT("Notice_CanNotTrade_Invade"));
	}
	else if (_cancelType == PersonalTradingCancelType::TIME_OVER)
	{
		ShowTicker(TEXT("TradeText"), TEXT("Notice_OverTime"));
	}
	_cancelType = PersonalTradingCancelType::NONE;
	Reset();
}

void FGsPersonalTradingManager::AckPersonalTradingLock()
{
	SetIsPacketSend(false);
	if (_localState == PersonalTradingState::LOCK)
	{
		SetLocalPersonalTradingState(PersonalTradingState::TRADING);

		if (_remoteState == PersonalTradingState::CONFIRM)
		{
			SetRemotePersonalTradingState(PersonalTradingState::LOCK);
			GMessage()->GetPersonalTrading().SendMessage(MessageContentPersoncalTrading::INVALIDATE_TARGET_STATE);
		}
	}
	else if (_localState == PersonalTradingState::TRADING)
	{
		SetLocalPersonalTradingState(PersonalTradingState::LOCK);
	}
	GMessage()->GetPersonalTrading().SendMessage(MessageContentPersoncalTrading::INVALIDATE_LOCAL_STATE);
}

void FGsPersonalTradingManager::AckPersonalTradingConfirm()
{
	SetIsPacketSend(false);
	// LOCK <-> CONFIRM 상태 변경
	if (_localState == PersonalTradingState::LOCK)
	{
		SetLocalPersonalTradingState(PersonalTradingState::CONFIRM);
	}
	else if (_localState == PersonalTradingState::CONFIRM)
	{
		SetLocalPersonalTradingState(PersonalTradingState::LOCK);
	}

	GMessage()->GetPersonalTrading().SendMessage(MessageContentPersoncalTrading::INVALIDATE_LOCAL_STATE);
}

void FGsPersonalTradingManager::AckPersonalTradingAbort()
{
	if (_cancelType == PersonalTradingCancelType::CLICK)
	{
		ShowTicker(TEXT("TradeText"), TEXT("Notice_CancelTrade"));
		_cancelType = PersonalTradingCancelType::NONE;
	}
	Reset();
}

void FGsPersonalTradingManager::ClearPersonalTrading()
{
	switch (GetLocalPersonalTradingState())
	{
	case PersonalTradingState::WAIT:
	{
		TryReqPersonalTradingCancel();
		break;
	}
	case PersonalTradingState::TRADING:
	case PersonalTradingState::LOCK:
	case PersonalTradingState::CONFIRM:
	{
		TryReqPersonalTradingAbort();
		break;
	}
	default:
		break;
	}
}

Level FGsPersonalTradingManager::GetTradeOpenLevel()
{
	if (_personalTradingConfig)
	{
		return _personalTradingConfig->tradeOpenLevel;
	}
	return 0;
}

int32 FGsPersonalTradingManager::GetApproveProgressTimeSec()
{
	if (_personalTradingConfig)
	{
		return _personalTradingConfig->approveProgressTimeSec;
	}
	return 0;
}

int32 FGsPersonalTradingManager::GetDailyTradeNumberMax()
{
	if (_personalTradingConfig)
	{
		return _personalTradingConfig->dailyTradeNumber;
	}
	return 0;
}

CurrencyDeltaPair FGsPersonalTradingManager::GetTradeChargeData()
{
	if (_personalTradingConfig)
	{
		return CurrencyDeltaPair(_personalTradingConfig->tradeCharge.currencyType, _personalTradingConfig->tradeCharge.currencyDelta);
	}
	return CurrencyDeltaPair(CurrencyType::DIA, 0);
}

int32 FGsPersonalTradingManager::GetSlotCountMax()
{
	if (_personalTradingConfig)
	{
		return _personalTradingConfig->slotCnt;
	}
	return 0;
}

void FGsPersonalTradingManager::ResetTimeContents(ResetTimeTicket InType)
{
	if (InType == ResetTimeTicket::RTT_DAILY)
	{
		GGameData()->SetDailyTradingCount(GetDailyTradeNumberMax());
	}
}

int64 FGsPersonalTradingManager::GetRemoteGameId()
{
	return _remoteGameId;
}

FString& FGsPersonalTradingManager::GetRemoteUserName()
{
	return _remoteUserName;
}

PersonalTradingState FGsPersonalTradingManager::GetRemotePersonalTradingState()
{
	return _remoteState;
}

ItemDBId FGsPersonalTradingManager::GetRemoteItemDBId(int32 InIndex)
{
	return (_remoteItemDBIdList.IsValidIndex(InIndex) ? _remoteItemDBIdList[InIndex] : 0);
}

const ItemDataEquip* FGsPersonalTradingManager::GetRemoteItemDataEquip(ItemDBId InItemDBId)
{
	for (int32 i = 0;i < _remoteItemEquipDataList.Num();++i)
	{
		if (_remoteItemEquipDataList[i].mItemDBId == InItemDBId)
		{
			return &_remoteItemEquipDataList[i];
		}
	}
	return nullptr;
}

const ItemData* FGsPersonalTradingManager::GetRemoteItemData(ItemDBId InItemDBId)
{
	for (int32 i = 0;i < _remoteItemDataList.Num();++i)
	{
		if (_remoteItemDataList[i].mItemDBId == InItemDBId)
		{
			return &_remoteItemDataList[i];
		}
	}
	return nullptr;
}

Gem FGsPersonalTradingManager::GetRemoteDiaAmount()
{
	return _remoteDiaAmount;
}

void FGsPersonalTradingManager::SetRemoteUserInfo(int64 InGameId, const FString& InUserName)
{
	_remoteGameId = InGameId;
	_remoteUserName = InUserName;
}

void FGsPersonalTradingManager::SetRemotePersonalTradingState(PersonalTradingState InState)
{
	_remoteState = InState;
}

PersonalTradingState FGsPersonalTradingManager::GetLocalPersonalTradingState()
{
	return _localState;
}

TArray<TPair<ItemDBId, ItemAmount>> FGsPersonalTradingManager::GetLocalItemDataList()
{
	return _localItemDataList;
}

Gem FGsPersonalTradingManager::GetLocalDiaAmount()
{
	return _localDiaAmount;
}

int32 FGsPersonalTradingManager::GetLocalRemainDailyTradeNumber()
{
	return GGameData()->GetUserData()->_dailyTradingCount;
}

bool FGsPersonalTradingManager::IsRequestingItem(ItemDBId InItemDBId)
{
	for (TPair<ItemDBId, ItemAmount> itemDataWait : _localItemDataWaitList)
	{
		if (itemDataWait.Key == InItemDBId)
		{
			return true;
		}
	}
	return false;
}

bool FGsPersonalTradingManager::IsRequestingDia()
{
	return (_localDiaAmountWait > 0);
}

bool FGsPersonalTradingManager::IsSelectedItem(ItemDBId InItemDBId)
{
	for (const TPair<ItemDBId, ItemAmount>& itemData : _localItemDataList)
	{
		if (itemData.Key == InItemDBId)
			return true;
	}
	return false;
}

void FGsPersonalTradingManager::SetLocalPersonalTradingState(PersonalTradingState InState)
{
	_localState = InState;
}

void FGsPersonalTradingManager::ReqLocalItemUpdate(ItemDBId InItemDBId, ItemAmount InAmount)
{
	_localItemDataWaitList.Emplace(InItemDBId, InAmount);
	FGsNetSendServiceWorld::SendReqPersonalTradingPutItem(InItemDBId, InAmount);
}

void FGsPersonalTradingManager::AckLocalItemUpdate(bool InIsSuccess)
{
	if (InIsSuccess)
	{
		if (_localItemDataWaitList.Num() > 0)
		{
			TPair<ItemDBId, ItemAmount> itemDataWait = _localItemDataWaitList[0];
			_localItemDataWaitList.RemoveAt(0);

			if (itemDataWait.Value == 0)
			{
				for (int32 i = 0;i < _localItemDataList.Num();++i)
				{
					if (itemDataWait.Key == _localItemDataList[i].Key)
					{
						_localItemDataList.RemoveAt(i);
						break;
					}
				}
			}
			else
			{
				bool isExistSameItem = false;
				for (TPair<ItemDBId, ItemAmount>& itemData : _localItemDataList)
				{
					if (itemDataWait.Key == itemData.Key)
					{
						itemData.Value = itemDataWait.Value;
						isExistSameItem = true;
						break;
					}
				}
				if (false == isExistSameItem)
				{
					_localItemDataList.Emplace(itemDataWait.Key, itemDataWait.Value);
				}
			}
		}
		GMessage()->GetPersonalTrading().SendMessage(MessageContentPersoncalTrading::INVALIDATE_LOCAL_ITEM);
	}
	else
	{
		if (_localItemDataWaitList.Num() > 0)
		{
			_localItemDataWaitList.RemoveAt(0);
		}
	}
}

void FGsPersonalTradingManager::ReqLocalDiaAmount(Gem InAmount)
{
	if (IsAlreadyPacketSend())
		return;

	_localDiaAmountWait = InAmount;
	FGsNetSendServiceWorld::SendReqPersonalTradingPutDia(InAmount);
	SetIsPacketSend(true);
}

void FGsPersonalTradingManager::AckLocalDiaAmount(bool InIsSuccess)
{
	SetIsPacketSend(false);

	if (InIsSuccess)
	{
		_localDiaAmount = _localDiaAmountWait;
		_localDiaAmountWait = 0;
		GMessage()->GetPersonalTrading().SendMessage(MessageContentPersoncalTrading::INVALIDATE_LOCAL_DIA);
	}
	else
	{
		_localDiaAmountWait = 0;
	}
}

bool FGsPersonalTradingManager::IsRemoteTrader(int64 InGameId)
{
	return (InGameId == _remoteGameId);
}

bool FGsPersonalTradingManager::IsTradeFromMe()
{
	return (GGameData()->GetUserData()->mGameId == _ownerGameId);
}

CurrencyDeltaPair FGsPersonalTradingManager::GetMyTradeChargeData()
{
	return GetTradeChargeData();
}

bool FGsPersonalTradingManager::CanOpenTrade()
{
	if (GUI()->IsActiveWidget(TEXT("NpcDialog")))
		return false;

	return true;
}

void FGsPersonalTradingManager::ProcessNetError(int32 InResult)
{
#if WITH_EDITOR
	FString msg = FString::Format(TEXT("Code({0} / {1}"), { InResult, PD::ResultEnumToString((PD::Result)InResult) });
	GSLOG(Error, TEXT("[PersonalTrading] %s"), *msg);
#endif

	_cancelType = PersonalTradingCancelType::NONE;

	FText errorText = GetErrorText(InResult);
	if (false == errorText.IsEmpty())
	{
		FGsUIHelper::TraySectionMessageTicker(EGsNotiSection::EXTSYSTEM, errorText);
	}

	switch (InResult)
	{
	case PD::Result::PERSONAL_TRADING_INVALID_USER_STATUS:
	case PD::Result::PERSONAL_TRADING_NOT_FOUND_USER:
	case PD::Result::PERSONAL_TRADING_IMPOSSIBLE_USER:
	case PD::Result::PERSONAL_TRADING_ERROR_INVALID_RANGE:
	case PD::Result::PERSONAL_TRADING_INVALID_OTHER_USER_STATUS:
	case PD::Result::PERSONAL_TRADING_ERROR_OHTER_TRADING:
	case PD::Result::PERSONAL_TRADING_ERROR_TRAGET_USER_OHTER_TRADING:
	case PD::Result::PERSONAL_TRADING_ERROR_SAME_POS:
	case PD::Result::PERSONAL_TRADING_ERROR_TARGET_LEVEL:
	case PD::Result::PERSONAL_TRADING_ERROR_CANNOT_USABLE_GM:
	case PD::Result::PERSONAL_TRADING_ERROR_NOT_ENOUGH_INVENTORY_SLOT:
	case PD::Result::PERSONAL_TRADING_ERROR_TARGET_NOT_ENOUGH_INVENTORY_SLOT:
	case PD::Result::PERSONAL_TRADING_ERROR_TIME_OUT:
	case PD::Result::PERSONAL_TRADING_ERROR_TRY_COUNT:
	case PD::Result::PERSONAL_TRADING_ERROR_TRAGET_TRY_COUNT:
	case PD::Result::PERSONAL_TRADING_ERROR_USABLE_IN_SAFE_ZONE:
	case PD::Result::PERSONAL_TRADING_ERROR_OTHER_USABLE_IN_SAFE_ZONE:
	case PD::Result::PERSONAL_TRADING_ERROR_LEVEL:
	case PD::Result::PERSONAL_TRADING_ERROR_TARGET_WEIGHT_FULL:
	case PD::Result::PERSONAL_TRADING_ERROR_WEIGHT_FULL:
	case PD::Result::PERSONAL_TRADING_ERROR_TARGET_NOT_ENOUGH_DIA:
	case PD::Result::PERSONAL_TRADING_ERROR_NO_DEVICE_REGISTERED:
	{
		Reset();
		break;
	}
	case PD::Result::PERSONAL_TRADING_ERROR_TRY_CONFIRM:
	case PD::Result::PERSONAL_TRADING_ALREADY_TRADING:
	case PD::Result::PERSONAL_TRADING_DONT_ACTION:
	case PD::Result::PERSONAL_TRADING_ERROR_NOT_TRADING:
	case PD::Result::PERSONAL_TRADING_ERROR_INVALID_ITEM:
	case PD::Result::PERSONAL_TRADING_ERROR_ITEM_ADD:
	case PD::Result::PERSONAL_TRADING_ERROR_NOT_ENOUGH_DIA:
	case PD::Result::PERSONAL_TRADING_ERROR_NOT_DIA_FULLY:
	case PD::Result::PERSONAL_TRADING_ERROR_UNKNOWN:
	{
		break;
	}
	default:
		FGsUIHelper::PopupNetError(static_cast<PD::Result>(InResult));
		break;
	}
}

void FGsPersonalTradingManager::OnLocalPlayerDie(const struct IGsMessageParam* InParam)
{
	ClearPersonalTrading();
	Reset();
}

void FGsPersonalTradingManager::OnLocalSpawnMeComplete(const struct IGsMessageParam* InParam)
{
	ClearPersonalTrading();
	Reset();
}

void FGsPersonalTradingManager::OnBeforeGameToLobby()
{
	ClearPersonalTrading();
	Reset();
}

void FGsPersonalTradingManager::OnReconnectEnd()
{
	ClearPersonalTrading();
	Reset();
}

void FGsPersonalTradingManager::OnInvasionStart(const struct IGsMessageParam* InParam)
{
	// 침공 시작 시 거래 요청을 취소시킨다.
	if (IsTradeFromMe())
	{
		if (TryReqPersonalTradingCancel())
		{
			SetCancelType(PersonalTradingCancelType::INVADE);
		}
	}
}

void FGsPersonalTradingManager::ShowTicker(const FTextKey& InNamespace, const FTextKey& InKey)
{
	FText findText;
	FText::FindText(InNamespace, InKey, findText);
	FGsUIHelper::TraySectionMessageTicker(EGsNotiSection::EXTSYSTEM, findText);
}

bool FGsPersonalTradingManager::IsTradeOpen()
{
	return (_localState == PersonalTradingState::TRADING ||
			_localState == PersonalTradingState::LOCK ||
			_localState == PersonalTradingState::CONFIRM);
}

void FGsPersonalTradingManager::StartEvent(EGsEventProgressType In_startType, EGsEventProgressType In_preType)
{
	if (CanOpenTrade())
	{
		if (UGsUIManager* uiManager = GUI())
		{
			uiManager->CloseAllStack();
		}

		if (UGsGameObjectManager* mgr = GSGameObject())
		{
			if (UGsGameObjectBase* local = mgr->FindObject(EGsGameObjectType::LocalPlayer))
			{
				if (FGsMovementHandlerLocalPlayer* moveHandler = local->CastGameObject<UGsGameObjectLocalPlayer>()->GetCastMovement<FGsMovementHandlerLocalPlayer>())
				{
					moveHandler->SpectatorEnd();
				}
			}

			if (UGsGameObjectBase* target = mgr->FindObject(EGsGameObjectType::Creature, _remoteGameId))
			{
				FGsMovementHandlerBase* movementHandler = target->GetMovementHandler();
				if (false == target->IsObjectType(EGsGameObjectType::LocalPlayer))
				{
					FGsMovementHandlerRemote* rmoteMovementHandler =
						FGsClassUtil::FCheckStaticCast<FGsMovementHandlerRemote>(movementHandler);
					rmoteMovementHandler->ReceveNetMotionStopReason(MotionStopReason::NORMAL);
					rmoteMovementHandler->Stop(true);
				}
			}
		}

		SetRemotePersonalTradingState(PersonalTradingState::TRADING);
		_remoteItemDataList.Empty();
		_remoteItemEquipDataList.Empty();
		_remoteItemDBIdList.Empty();
		_remoteDiaAmount = 0;

		SetLocalPersonalTradingState(PersonalTradingState::TRADING);
		_localItemDataList.Empty();
		_localItemDataWaitList.Empty();
		_localDiaAmount = 0;
		_localDiaAmountWait = 0;

		GCoolTime()->SetCoolTime(EGsCoolTime::PersonalTrading, 0, 0, 0);
		GMessage()->GetContents().SendMessage(MessageContents::CLOSE_QUICK_MENU);
		GMessage()->GetContentsHud().SendMessage(MessageContentHud::INVALIDATE_PERSONAL_TRADING);
		if (UGsInventoryManager* inventoryMgr = GSInventory())
		{
			inventoryMgr->CloseInventoryPopup(false,false);
		}

		// Open 개인거래 팝업 : 거래UI, 인벤토리, 채팅
		TWeakObjectPtr<UGsUIWidgetBase> widgetChat = GUI()->OpenAndGetWidget(TEXT("PopupChat"));
		if (widgetChat.IsValid())
		{
			TWeakObjectPtr<UGsUIPopupChat> popupChat = Cast<UGsUIPopupChat>(widgetChat);
			if (popupChat.IsValid())
			{
				popupChat->SetTradeMode();
				UGsUIPopupChat::PopupInitData initData{ EGsChatViewType::WHISPER, EGsExChatType::WHISPER, _remoteUserName };
				popupChat->InitializeData(&initData);
			}
		}

		TWeakObjectPtr<UGsUIWidgetBase> widgetTrade = GUI()->OpenAndGetWidget(TEXT("PopupTrade"));
		if (widgetTrade.IsValid())
		{
			TWeakObjectPtr<UGsUIPopupTrade> popupTrade = Cast<UGsUIPopupTrade>(widgetTrade);
			if (popupTrade.IsValid())
			{

			}
		}
	}
	else
	{
		// 개인 거래가 열려야 하지만 열 수 없는 상태가 되었다.
		TryReqPersonalTradingAbort();
	}
}

void FGsPersonalTradingManager::FinishEvent(EGsEventProgressType In_finishType, EGsEventProgressType In_nextType)
{
	
}

void FGsPersonalTradingManager::OnCanceledStartEvent()
{
	// 개인 거래가 열려야 하지만 열 수 없는 상태가 되었다.
	TryReqPersonalTradingAbort();
}

bool FGsPersonalTradingManager::IsTradeSet()
{
	return (_localState != PersonalTradingState::NONE || _remoteState != PersonalTradingState::NONE);
}

void FGsPersonalTradingManager::TryReqPersonalTradingTry(int64 InTargetGameId, const FString InTargetUserName)
{
	FGsFenceHandler* fenceHandler = GFence();
	if (nullptr == fenceHandler)
		return;

	FGsGameDataManager* dataManager = GGameData();
	if (nullptr == dataManager)
		return;

	UGsItemManager* itemManager = GItem();
	if (nullptr == itemManager)
		return;

	FGsUnlockManager* unlockManager = GSUnlock();
	if (nullptr == unlockManager)
		return;

	if (false == unlockManager->IsContentsUnlock(EGsUnlockContentsType::PERSONAL_TRADING, true))
	{
		// 컨텐츠가 잠겨있어서 시도할 수 없다.
		return;
	}

	if (GetLocalPersonalTradingState() != PersonalTradingState::NONE)
	{
		// 이미 거래 상태를 가지고 있다.
		return;
	}
	if (false == fenceHandler->IsPlayerInTownFence())
	{
		// 마을이 아니다.
		ShowTicker(TEXT("TradeText"), TEXT("Notice_CanNotTrade_Place"));
		return;
	}
	if (false == fenceHandler->IsPlayerInNonPVPFence())
	{
		// 플레이어가 PVP 지역에 있다.
		ShowTicker(TEXT("TradeText"), TEXT("Notice_CanNotTrade_Place"));
		return;
	}
	if (dataManager->GetInvasionDefenseState())
	{
		// 침공전이 진행중이다.
		ShowTicker(TEXT("TradeText"), TEXT("Notice_CanNotTrade_Invade"));
		return;
	}
	if (dataManager->GetUserData()->mLevel < GetTradeOpenLevel())
	{
		// 레벨 부족
		FText findText;
		FText::FindText(TEXT("TradeText"), TEXT("Notice_CanNotTrade_Level"), findText);
		FText errorText = FText::Format(findText, GetTradeOpenLevel());
		FGsUIHelper::TraySectionMessageTicker(EGsNotiSection::EXTSYSTEM, errorText);
		return;
	}
	if (dataManager->GetUserData()->_dailyTradingCount <= 0)
	{
		// 일일 거래 가능 횟수 부족
		ShowTicker(TEXT("TradeText"), TEXT("Notice_CanNotTrade_DailyTradeNumber"));
		return;
	}
	CurrencyDeltaPair tradeCharge = GetTradeChargeData();
	if (false == FGsCurrencyHelper::CheckCurrency(tradeCharge.mCurrencyType, tradeCharge.mCurrencyDelta))
	{
		// 거래 수수료 부족
		ShowTicker(TEXT("TradeText"), TEXT("Notice_NotEnoughCharge"));
		return;
	}
	if (itemManager->IsInvenSlotFull(GetSlotCountMax()))
	{
		// 인벤토리 공간 부족
		ShowTicker(TEXT("TradeText"), TEXT("Notice_OverInventory"));
		return;
	}

	SetRemoteUserInfo(InTargetGameId, InTargetUserName);
	SetLocalPersonalTradingState(PersonalTradingState::WAIT);
	FGsNetSendServiceWorld::SendReqPersonalTradingTry(InTargetGameId);
}

void FGsPersonalTradingManager::TryReqPersonalTradingAccept()
{
	if (false == IsAlreadyPacketSend())
	{
		if (FGsUnlockManager* unlockManager = GSUnlock())
		{
			if (false == unlockManager->IsContentsUnlock(EGsUnlockContentsType::PERSONAL_TRADING, true))
			{	// 갑자기 요청을 받은 상태에서 컨텐츠가 잠겼을 상황
				if (TryReqPersonalTradingCancel())
				{	// 언락 티커가 노출되었기 때문에 별도의 거래 취소 티커를 띄우지 않는다.
					SetCancelType(PersonalTradingCancelType::NONE);
				}
				return;
			}
		}

		FGsNetSendServiceWorld::SendReqPersonalTradingAccept(GetOwnerGameId());
		SetIsPacketSend(true);
	}
}

bool FGsPersonalTradingManager::TryReqPersonalTradingCancel()
{
	if (_localState != PersonalTradingState::WAIT)
		return false;

	FGsNetSendServiceWorld::SendReqPersonalTradingCancel(GetOwnerGameId());
	return true;
}

void FGsPersonalTradingManager::TryReqPersonalTradingLock()
{
	if (IsAlreadyPacketSend())
		return;

	if (GetLocalPersonalTradingState() != PersonalTradingState::TRADING)
		return;


	FGsNetSendServiceWorld::SendReqPersonalTradingLock(true);
	SetIsPacketSend(true);
}

void FGsPersonalTradingManager::TryReqPersonalTradingLockCancel()
{
	if (IsAlreadyPacketSend())
		return;

	if (GetLocalPersonalTradingState() != PersonalTradingState::LOCK)
		return;

	FGsNetSendServiceWorld::SendReqPersonalTradingLock(false);
	SetIsPacketSend(true);
}

void FGsPersonalTradingManager::TryReqPersonalTradingConfirm()
{
	if (IsAlreadyPacketSend())
		return;

	bool isLocalReady = (GetLocalPersonalTradingState() == PersonalTradingState::LOCK);
	bool isTargetReady = (GetRemotePersonalTradingState() == PersonalTradingState::LOCK || GetRemotePersonalTradingState() == PersonalTradingState::CONFIRM);


	// 2023/7/19 PKT - https://jira.com2us.com/jira/browse/CHR-18259
	//				개인 거래 간 거래 불가능 한 규칙 적용
	if (0 >= (_remoteItemDBIdList.Num() + _localItemDataList.Num()))
	{	
		ShowTicker(TEXT("TradeText"), TEXT("Notice_CanNotTrade_NoItem"));
		return;
	}

	if (isLocalReady && isTargetReady)
	{
		
		FGsNetSendServiceWorld::SendReqPersonalTradingConfirm(true);
		SetIsPacketSend(true);
	}
}

void FGsPersonalTradingManager::TryReqPersonalTradingConfirmCancel()
{
	if (IsAlreadyPacketSend())
		return;

	FGsNetSendServiceWorld::SendReqPersonalTradingConfirm(false);
	SetIsPacketSend(true);
}

bool FGsPersonalTradingManager::TryReqPersonalTradingAbort()
{
	FGsNetSendServiceWorld::SendReqPersonalTradingAbort();
	return true;
}

void FGsPersonalTradingManager::NotifyPersonalTrading(PD::SC::PKT_SC_INFORM_PERSONAL_TRADING_READ& InPacket)
{
	switch (InPacket.PersonalTradingNoticeType())
	{
	case PersonalTradingNoticeType::ACCEPTED:	// 거래 수락됨
	{
		GSEventProgress()->OnReqStartEvent(EGsEventProgressType::PERSONAL_TRADING, this);
		break;
	}
	case PersonalTradingNoticeType::REJECT:		// 거래 거절
	{
		if (_localState != PersonalTradingState::NONE)
		{
			ShowTicker(TEXT("TradeText"), TEXT("Notice_CancelTrade_Opponent"));
			Reset();
		}
		break;
	}
	case PersonalTradingNoticeType::REQUEST:	// 요청
	{
		// 거래 요청을 받았습니다.
		ShowTicker(TEXT("TradeText"), TEXT("Notice_ReceiveTrade"));

		_ownerGameId = InPacket.TradingGameId();
		SetRemoteUserInfo(InPacket.TradingGameId(), InPacket.TradingUserName());
		SetRemotePersonalTradingState(PersonalTradingState::WAIT);
		SetLocalPersonalTradingState(PersonalTradingState::WAIT);
		GCoolTime()->SetCoolTime(EGsCoolTime::PersonalTrading, 0, 0, GetApproveProgressTimeSec());
		GMessage()->GetContentsHud().SendMessage(MessageContentHud::INVALIDATE_PERSONAL_TRADING);
		break;
	}
	case PersonalTradingNoticeType::ABORT:		// 중지
	{
		ShowTicker(TEXT("TradeText"), TEXT("Notice_CancelTrade_Opponent"));
		Reset();
		break;
	}
	case PersonalTradingNoticeType::ABORT_INVALID_STATUS:	// 중지: 유효하지 않은
	{
		if (_localState != PersonalTradingState::NONE)
		{
			Reset();
			// 개인 거래가 불가능한 상태입니다.
			ShowTicker(TEXT("TradeText"), TEXT("Notice_CanNotTrade_State"));
		}
		break;
	}
	case PersonalTradingNoticeType::LOCK:		// 잠금
	{
		SetRemotePersonalTradingState(PersonalTradingState::LOCK);
		GMessage()->GetPersonalTrading().SendMessage(MessageContentPersoncalTrading::INVALIDATE_TARGET_STATE);
		break;
	}
	case PersonalTradingNoticeType::UNLOCK:		// 해제
	{
		SetRemotePersonalTradingState(PersonalTradingState::TRADING);
		GMessage()->GetPersonalTrading().SendMessage(MessageContentPersoncalTrading::INVALIDATE_TARGET_STATE);
		if (GetLocalPersonalTradingState() == PersonalTradingState::CONFIRM)
		{
			SetLocalPersonalTradingState(PersonalTradingState::LOCK);
			GMessage()->GetPersonalTrading().SendMessage(MessageContentPersoncalTrading::INVALIDATE_LOCAL_STATE);
		}
		break;
	}
	case PersonalTradingNoticeType::CONFIRM:	// 거래 최종 확인(수락)
	{
		SetRemotePersonalTradingState(PersonalTradingState::CONFIRM);
		GMessage()->GetPersonalTrading().SendMessage(MessageContentPersoncalTrading::INVALIDATE_TARGET_STATE);
		break;
	}
	case PersonalTradingNoticeType::SUCCESS:	// 거래 성공
	{
		ShowTicker(TEXT("TradeText"), TEXT("Notice_SuccesTrade"));
		GGameData()->SetDailyTradingCount(GGameData()->GetUserData()->_dailyTradingCount - 1);
		Reset();
		break;
	}
	case PersonalTradingNoticeType::TIME_OUT:	// 거래 요청 시간 만료
	{
		ShowTicker(TEXT("TradeText"), TEXT("Notice_OverTime"));
		Reset();
	}
	default:
		break;
	}
}

void FGsPersonalTradingManager::NotifyPersonalTradingItemUpdate(int64 InTargetGameId, const ItemData& InItemData, ItemAmount InAmount)
{
	if (IsRemoteTrader(InTargetGameId))
	{
		ItemData newItemData(InItemData);
		newItemData.mItemAmount = InAmount;

		bool isExistSameItem = false;
		for (ItemData& itemData : _remoteItemDataList)
		{
			if (itemData.mItemDBId == newItemData.mItemDBId)
			{
				itemData = newItemData;
				isExistSameItem = true;
			}
		}
		if (false == isExistSameItem)
		{
			_remoteItemDBIdList.Emplace(newItemData.mItemDBId);
			_remoteItemDataList.Emplace(newItemData);
		}
		GMessage()->GetPersonalTrading().SendMessage(MessageContentPersoncalTrading::INVALIDATE_TARGET_ITEM);
	}
	else
	{
		// Error
	}
}

void FGsPersonalTradingManager::NotifyPersonalTradingItemEquipUpdate(int64 InTargetGameId, const ItemDataEquip& InItemData)
{
	if (IsRemoteTrader(InTargetGameId))
	{
		bool isExistSameItem = false;
		for (ItemDataEquip& itemDataEquip : _remoteItemEquipDataList)
		{
			if (itemDataEquip.mItemDBId == InItemData.mItemDBId)
			{
				itemDataEquip = InItemData;
				isExistSameItem = true;
			}
		}
		if (false == isExistSameItem)
		{
			_remoteItemDBIdList.Emplace(InItemData.mItemDBId);
			_remoteItemEquipDataList.Emplace(InItemData);
		}
		GMessage()->GetPersonalTrading().SendMessage(MessageContentPersoncalTrading::INVALIDATE_TARGET_ITEM);
	}
	else
	{
		// Error
	}
}

void FGsPersonalTradingManager::NotifyPersonalTradingItemRemove(int64 InTargetGameId, ItemDBId InItemDBId)
{
	if (IsRemoteTrader(InTargetGameId))
	{
		for (int32 i = 0;i < _remoteItemDataList.Num();++i)
		{
			if (_remoteItemDataList[i].mItemDBId == InItemDBId)
			{
				_remoteItemDataList.RemoveAt(i);
				break;
			}
		}
		for (int32 i = 0;i < _remoteItemEquipDataList.Num();++i)
		{
			if (_remoteItemEquipDataList[i].mItemDBId == InItemDBId)
			{
				_remoteItemEquipDataList.RemoveAt(i);
				break;
			}
		}
		for (int32 i = 0;i < _remoteItemDBIdList.Num();++i)
		{
			if (_remoteItemDBIdList[i] == InItemDBId)
			{
				_remoteItemDBIdList.RemoveAt(i);
				break;
			}
		}
		GMessage()->GetPersonalTrading().SendMessage(MessageContentPersoncalTrading::INVALIDATE_TARGET_ITEM);
	}
	else
	{
		// Error
	}
}

void FGsPersonalTradingManager::NotifyPersonalTradingDiaUpdate(int64 InTargetGameId, Gem InAmount)
{
	if (IsRemoteTrader(InTargetGameId))
	{
		_remoteDiaAmount = InAmount;

		GMessage()->GetPersonalTrading().SendMessage(MessageContentPersoncalTrading::INVALIDATE_TARGET_DIA);
	}
	else
	{
		// Error
	}
}

FText FGsPersonalTradingManager::GetErrorText(int32 InResult)
{
	FText findText;
	switch (InResult)
	{
	case PD::Result::PERSONAL_TRADING_INVALID_USER_STATUS:
	case PD::Result::PERSONAL_TRADING_NOT_FOUND_USER:
	case PD::Result::PERSONAL_TRADING_IMPOSSIBLE_USER:
	case PD::Result::PERSONAL_TRADING_ERROR_INVALID_RANGE:
	case PD::Result::PERSONAL_TRADING_INVALID_OTHER_USER_STATUS:
	case PD::Result::PERSONAL_TRADING_ERROR_OHTER_TRADING:
	case PD::Result::PERSONAL_TRADING_ERROR_TRAGET_USER_OHTER_TRADING:
	case PD::Result::PERSONAL_TRADING_ERROR_SAME_POS:
	case PD::Result::PERSONAL_TRADING_ERROR_TARGET_LEVEL:
	case PD::Result::PERSONAL_TRADING_ERROR_CANNOT_USABLE_GM:
	{
		// 개인 거래가 불가능한 상태입니다.
		FText::FindText(TEXT("TradeText"), TEXT("Notice_CanNotTrade_State"), findText);
		break;
	}
	case PD::Result::PERSONAL_TRADING_ERROR_TRY_CONFIRM:
	{
		// 잠금 상태가 아닙니다.
		FText::FindText(TEXT("TradeText"), TEXT("Notice_Unlock"), findText);
		break;
	}
	case PD::Result::PERSONAL_TRADING_ERROR_NOT_ENOUGH_INVENTORY_SLOT:
	{
		// 가방 공간이 부족합니다.
		FText::FindText(TEXT("TradeText"), TEXT("Notice_OverInventory"), findText);
		break;
	}
	case PD::Result::PERSONAL_TRADING_ERROR_TARGET_NOT_ENOUGH_INVENTORY_SLOT:
	{
		// 상대방의 가방 공간이 부족합니다.
		FText::FindText(TEXT("TradeText"), TEXT("Notice_OverInventory_Opponent"), findText);
		break;
	}
	case PD::Result::PERSONAL_TRADING_ERROR_TIME_OUT:
	{
		// 요청 시간을 초과하였습니다.
		FText::FindText(TEXT("TradeText"), TEXT("Notice_OverTime"), findText);
		break;
	}
	case PD::Result::PERSONAL_TRADING_ERROR_TRY_COUNT:
	{
		// 일일 거래 가능 횟수를 초과하였습니다
		FText::FindText(TEXT("TradeText"), TEXT("Notice_CanNotTrade_DailyTradeNumber"), findText);
		break;
	}
	case PD::Result::PERSONAL_TRADING_ERROR_TRAGET_TRY_COUNT:
	{
		// 상대방이 거래 횟수를 초과하였습니다.
		FText::FindText(TEXT("TradeText"), TEXT("Notice_CanNotTrade_DailyTradeNumber_Opponent"), findText);
		break;
	}
	case PD::Result::PERSONAL_TRADING_ERROR_USABLE_IN_SAFE_ZONE:
	{
		// 거래가 불가능한 장소입니다.
		FText::FindText(TEXT("TradeText"), TEXT("Notice_CanNotTrade_Place"), findText);
		break;
	}
	case PD::Result::PERSONAL_TRADING_ERROR_OTHER_USABLE_IN_SAFE_ZONE:
	{
		// 상대방이 거래 불가능한 장소에 있습니다.
		FText::FindText(TEXT("TradeText"), TEXT("Notice_NotSafePlace_Opponet"), findText);
		break;
	}
	case PD::Result::PERSONAL_TRADING_ERROR_LEVEL:
	{
		// 레벨 부족
		FText::FindText(TEXT("TradeText"), TEXT("Notice_CanNotTrade_Level"), findText);
		findText = FText::Format(findText, GetTradeOpenLevel());
		break;
	}
	case PD::Result::PERSONAL_TRADING_ERROR_INVALID_ITEM:
	{
		// 개인 거래 불가능한 아이템입니다.
		FText::FindText(TEXT("TradeText"), TEXT("Notice_NotPersonalTradable"), findText);
		break;
	}
	case PD::Result::PERSONAL_TRADING_ERROR_TARGET_WEIGHT_FULL:
	{
		// 상대의 인벤토리 무게가 초과하여 개인 거래가 불가합니다.
		FText::FindText(TEXT("NetText"), TEXT("PERSONAL_TRADING_ERROR_TARGET_WEIGHT_FULL"), findText);
		break;
	}
	case PD::Result::PERSONAL_TRADING_ERROR_WEIGHT_FULL:
	{
		// 인벤토리 무게가 초과하여 개인 거래가 불가합니다.
		FText::FindText(TEXT("NetText"), TEXT("PERSONAL_TRADING_ERROR_WEIGHT_FULL"), findText);
		break;
	}
	case PD::Result::PERSONAL_TRADING_ERROR_TARGET_NOT_ENOUGH_DIA:
	{
		// 상대방이 거래 수수료가 부족합니다.
		FText::FindText(TEXT("TradeText"), TEXT("Notice_NotEnoughCharge_Opponent"), findText);
		break;
	}
	case PD::Result::PERSONAL_TRADING_ERROR_NO_DEVICE_REGISTERED:
	{
		// 상대방이 기기 등록을 하지 않아 개인 거래가 불가능합니다.
		FText::FindText(TEXT("TradeText"), TEXT("Notice_CanNotTrade_DeviceRegist_Opponent"), findText);
		break;
	}
	case PD::Result::PERSONAL_TRADING_ALREADY_TRADING:
	case PD::Result::PERSONAL_TRADING_DONT_ACTION:
	case PD::Result::PERSONAL_TRADING_ERROR_NOT_TRADING:
	case PD::Result::PERSONAL_TRADING_ERROR_ITEM_ADD:
	case PD::Result::PERSONAL_TRADING_ERROR_NOT_ENOUGH_DIA:
	case PD::Result::PERSONAL_TRADING_ERROR_NOT_DIA_FULLY:
	case PD::Result::PERSONAL_TRADING_ERROR_UNKNOWN:
	{
		break;
	}
	default:
		break;
	}
	return findText;
}

