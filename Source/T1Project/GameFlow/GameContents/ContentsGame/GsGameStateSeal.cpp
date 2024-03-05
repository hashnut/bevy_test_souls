#include "GsGameStateSeal.h"

#include "GameFlow/GameContents/GsContentsMode.h"
#include "Item/GsItemManager.h"
#include "Management/GsMessageHolder.h"
#include "Management/ScopeGame/GsSealManager.h"
#include "Management/ScopeGame/GsUnlockManager.h"
#include "Management/ScopeGlobal/GsGameFlowManager.h"
#include "Message/GsMessageContents.h"
#include "Message/MessageParam/GsItemMessageParam.h"
#include "Message/MessageParam/GsMessageParam.h"
#include "Message/MessageParam/GsSealMessageParam.h"
#include "T1Project.h"
#include "UI/UIContent/Window/GsUIWindowSeal.h"
#include "Unlock/GsUnlockDefine.h"
#include "../Management/GsScopeGame.h"

FGsGameStateSeal::FGsGameStateSeal()
	: Super(FGsContentsMode::InGame::ContentsSeal)
{
}

void FGsGameStateSeal::Init()
{
	Super::Init();

	FGsMessageHolder* MessageHolder = GMessage();
	if (nullptr != MessageHolder)
	{
		_openDelegate = MessageHolder->GetSeal().AddRaw(FGsSealMessageParamOpenSeal::MESSAGE_ID, this, &FGsGameStateSeal::Open);
	
		_unlockDelegate = MessageHolder->GetUnlock().AddRaw(MessageContentUnlock::NOTIFY_CONTENTS_UNLOCK, this, &FGsGameStateSeal::OnContentsUnlock);
	}
	else
	{
		GSLOG(Error, TEXT("nullptr == MessageHolder"));
	}

}

void FGsGameStateSeal::Close()
{
	FGsMessageHolder* MessageHolder = GMessage();
	if (nullptr != MessageHolder)
	{
		MessageHolder->GetSeal().Remove(_openDelegate);
		MessageHolder->GetUnlock().Remove(_unlockDelegate);
	}

	Super::Close();
}

void FGsGameStateSeal::Enter()
{
	Super::Enter();

	_window = UGsUIWindowSeal::OpenWindow(this);

	SetNavigationBar(CurrencyType::NONE);

	RegisterMessages();

	if (_window.IsValid())
	{
		if (!HasHistory())
		{
			// 히스토리가 없다면, 아무런 파라미터 없이 본 state 로 진입한 경우.
			// 초기화에 필요한 아무런 데이터가 없는 상태이므로, 디폴트 데이터를 히스토리에 입력해 줌.
			PushHistory(UGsUIWindowSeal::Parameters(FGsSealManager::GetDefaultSealPackId()));
		}
		else
		{
			// 히스토리가 있다면,
			// * OpenCraftWith() 등을 통해 초기화 데이터가 있는 상태로 진입한 경우. 현재 네비상태를 저장하지 않음.
			// * 뒤로가기 등으로 스택에서부터 윈도우가 복구되는 경우. 현재 네비상태를 저장하지 않음.
		}

		UGsUIWindowSeal::Parameters LastParameters;
		if (PopHistory(LastParameters))
		{
			_window->SetParameters(LastParameters, true, false);
		}
	}
	else
	{
		GSLOG(Warning, TEXT("NOT _window.IsValid()"));
	}
}

void FGsGameStateSeal::Exit()
{
	// 메세지 해지
	UnregisterMessages();

	Super::Exit();
}

bool FGsGameStateSeal::CanEnter(bool InShowUnableMessage /*= false*/) const
{
	if (GSUnlock()->IsContentsUnlock(EGsUnlockContentsType::SEAL))
	{
		return true;
	}

	if (InShowUnableMessage)
	{
		GSUnlock()->ShowLockTicker(EGsUnlockContentsType::SEAL);
	}

	return false;
}

bool FGsGameStateSeal::HasHistory() const
{
	return 0 < _historyStack.Num();
}

void FGsGameStateSeal::PushHistory(UGsUIWindowSeal::Parameters InParameters)
{
	if (0 < _historyStack.Num())
	{
		if (_historyStack.Top() == InParameters)
		{
			// Top 과 동일하면 스킵
			return;
		}
	}

	_historyStack.Push(MoveTemp(InParameters));
}

bool FGsGameStateSeal::PopHistory(OUT UGsUIWindowSeal::Parameters& OutHistory)
{
	if (HasHistory())
	{
		OutHistory = _historyStack.Pop(false);
		return true;
	}

	return false;
}

void FGsGameStateSeal::ClearHistory()
{
	_historyStack.Empty();
}

void FGsGameStateSeal::RegisterMessages()
{
	FGsMessageHolder* MessageHolder = GMessage();
	if (nullptr == MessageHolder)
	{
		GSLOG(Error, TEXT("nullptr == MessageHolder"));
		return;
	}

	MSeal& SealMessageHandler = MessageHolder->GetSeal();

	_sealDelegates.Empty();
	_sealDelegates.Emplace(SealMessageHandler.AddRaw(FGsSealMessageParamSeal::MESSAGE_ID, this, &FGsGameStateSeal::OnSeal));
	_sealDelegates.Emplace(SealMessageHandler.AddRaw(FGsSealMessageParamAckSealLevelUp::MESSAGE_ID, this, &FGsGameStateSeal::OnSealLevelUp));
	_sealDelegates.Emplace(SealMessageHandler.AddRaw(FGsSealMessageParamAckSealSlotChangeAll::MESSAGE_ID, this, &FGsGameStateSeal::OnSealSlotChangeAll));
	_sealDelegates.Emplace(SealMessageHandler.AddRaw(FGsSealMessageParamSlotLockSave::MESSAGE_ID, this, &FGsGameStateSeal::OnSealSlotLockSave));

	_userInfoDelegates.Empty();
	_userInfoDelegates.Emplace(MessageHolder->GetUserBasicInfo().AddRaw(MessageUserInfo::CURRENCY, this, &FGsGameStateSeal::OnUser_Currency));

	_itemActionDelegates.Empty();
	_itemActionDelegates.Emplace(MessageHolder->GetItem().AddRaw(MessageItem::ADDITEMLIST, this, &FGsGameStateSeal::OnAddItemList));
	_itemActionDelegates.Emplace(MessageHolder->GetItem().AddRaw(MessageItem::UPDATEITEM, this, &FGsGameStateSeal::OnUpdateItem));
	_itemActionDelegates.Emplace(MessageHolder->GetItem().AddRaw(MessageItem::REMOVEITEM, this, &FGsGameStateSeal::OnRemoveItem));
}

void FGsGameStateSeal::UnregisterMessages()
{
	FGsMessageHolder* MessageHolder = GMessage();
	if (nullptr == MessageHolder)
	{
		GSLOG(Error, TEXT("nullptr == MessageHolder"));
		return;
	}

	MSeal& SealMessageHandler = MessageHolder->GetSeal();
	for (const TPair<MessageContentSeal, FDelegateHandle>& DelegatePair : _sealDelegates)
	{
		SealMessageHandler.Remove(DelegatePair);
	}
	_sealDelegates.Empty();

	MUserBasicInfo& UserBasicInfoMessageHandler = MessageHolder->GetUserBasicInfo();
	for (const TPair<MessageUserInfo, FDelegateHandle>& DelegatePair : _userInfoDelegates)
	{
		UserBasicInfoMessageHandler.Remove(DelegatePair);
	}
	_userInfoDelegates.Empty();

	MItem& ItemMessageHandler = MessageHolder->GetItem();
	for (const TPair<MessageItem, FDelegateHandle>& DelegatePair : _itemActionDelegates)
	{
		ItemMessageHandler.Remove(DelegatePair);
	}
	_itemActionDelegates.Empty();
}

void FGsGameStateSeal::CurrencyChanged(const CurrencyType InCurrencyType)
{
	if (CurrencyType::NONE == InCurrencyType || !_window.IsValid())
	{
		return;
	}

	_window->OnChangeCurrency(InCurrencyType);
}

void FGsGameStateSeal::ItemChanged(const ItemId InItemId)
{
	if (INVALID_ITEM_ID == InItemId || !_window.IsValid())
	{
		return;
	}

	_window->OnChangeItem(InItemId);
}

void FGsGameStateSeal::Open(const IGsMessageParam* InParam)
{
	if (!CanEnter(true))
	{
		return;
	}

	const FGsSealMessageParamOpenSeal* Param = InParam->Cast<const FGsSealMessageParamOpenSeal>();
	if (nullptr == Param)
	{
		GSLOG(Error, TEXT("nullptr == Param"));
		return;
	}

	FGsGameFlowManager* GameFlowManager = GMode();
	if (nullptr == GameFlowManager)
	{
		GSLOG(Error, TEXT("nullptr == GameFlowManager"));
		return;
	}

	if (GameFlowManager->GetCurrentContentStateIndex() != FGsContentsMode::InGame::ContentsSeal)
	{
		FGsMessageHolder* MessageHolder = GMessage();
		if (nullptr == MessageHolder)
		{
			GSLOG(Error, TEXT("nullptr == MessageHolder"));
			return;
		}

		MessageHolder->GetContents().SendMessage(MessageContents::ACTIVE_WINDOW_SEAL);
	}
	else
	{
		if (_window.IsValid())
		{
			_window->SetParameters(UGsUIWindowSeal::Parameters(Param->_sealPackId));
		}
	}
}

void FGsGameStateSeal::OnSeal(const IGsMessageParam* InParam)
{
	if (!_window.IsValid())
	{
		return;
	}

	const FGsSealMessageParamSeal* Param = InParam->Cast<const FGsSealMessageParamSeal>();
	if (nullptr == Param)
	{
		GSLOG(Error, TEXT("nullptr == Param"));
		return;
	}

	//_window->SealTargetUpdated();
}

void FGsGameStateSeal::OnSealLevelUp(const IGsMessageParam* InParam)
{
	if (!_window.IsValid())
	{
		return;
	}

	const FGsSealMessageParamAckSealLevelUp* Param = InParam->Cast<const FGsSealMessageParamAckSealLevelUp>();
	if (nullptr == Param)
	{
		GSLOG(Error, TEXT("nullptr == Param"));
		return;
	}

	if (!Param->_packetNoError)
	{
		return;
	}

	SealPackId CurrentSealPackId = INVALID_SEAL_PACK_ID;
	const FGsSealManager* SealManager = GSSeal();
	if (nullptr != SealManager)
	{
		//CurrentSealPackId = SealManager->GetDefaultSealPackId();
		CurrentSealPackId = Param->_sealPackId;
	}

	_window->OnSealLevelUp(CurrentSealPackId, Param->_sealNode.mSealId);
}

void FGsGameStateSeal::OnSealSlotChangeAll(const IGsMessageParam* InParam)
{
	if (!_window.IsValid())
	{
		return;
	}

	const FGsSealMessageParamAckSealSlotChangeAll* Param = InParam->Cast<const FGsSealMessageParamAckSealSlotChangeAll>();
	if (nullptr == Param)
	{
		GSLOG(Error, TEXT("nullptr == Param"));
		return;
	}

	if (!Param->_packetNoError)
	{
		return;
	}

	SealId CurrentSealNodeId = INVALID_SEAL_ID;
	if (!Param->_sealSlotDatas.IsValidIndex(0))
	{
		GSLOG(Error, TEXT("!Param->_sealSlotDatas.IsValidIndex(0)"));
	}
	else
	{
		SealPackId CurrentSealPackId = INVALID_SEAL_PACK_ID;
		const FGsSealManager* SealManager = GSSeal();
		if (nullptr != SealManager)
		{
			//CurrentSealPackId = SealManager->GetDefaultSealPackId();
			CurrentSealPackId = Param->_sealPackId;
		}

		CurrentSealNodeId = Param->_sealSlotDatas[0].mSealId;
		_window->OnSealSlotChangeAll(CurrentSealPackId, CurrentSealNodeId);
	}
}

void FGsGameStateSeal::OnSealSlotLockSave(const IGsMessageParam* InParam)
{
	if (!_window.IsValid())
	{
		return;
	}

	const FGsSealMessageParamSlotLockSave* Param = InParam->Cast<const FGsSealMessageParamSlotLockSave>();
	if (nullptr == Param)
	{
		GSLOG(Error, TEXT("nullptr == Param"));
		return;
	}

		_window->OnSealSlotLockSave();
}

void FGsGameStateSeal::OnUser_Currency(uint64 InCurrncyType)
{
	CurrencyChanged(StaticCast<CurrencyType>(InCurrncyType));
}

void FGsGameStateSeal::OnAddItemList(FGsItemMessageParamBase& InParam)
{
	const FGsAddItemList* Param = StaticCast<const FGsAddItemList*>(&InParam);
	check(nullptr != Param);

	UGsItemManager* ItemManager = GItem();
	if (nullptr == ItemManager)
	{
		GSLOG(Error, TEXT("nullptr == ItemManager"));
		return;
	}

	TSet<ItemId> ItemIdSet;
	for (const ItemDBId CurrentItemDBId : Param->_itemDbidList)
	{
		const ItemId CurrentItemId = ItemManager->GetTIDByDBID(CurrentItemDBId);
		if (INVALID_ITEM_ID == CurrentItemId)
		{
			GSLOG(Error, TEXT("INVALID_ITEM_ID == CurrentItemId, CurrentItemDBId: %lld"), CurrentItemDBId);
			continue;
		}

		ItemIdSet.Add(CurrentItemId);
	}

	for (const ItemId CurrentItemId : ItemIdSet)
	{
		ItemChanged(CurrentItemId);
	}
}

void FGsGameStateSeal::OnUpdateItem(FGsItemMessageParamBase& InParam)
{
	const FGsUpdateItem* Param = StaticCast<const FGsUpdateItem*>(&InParam);
	check(nullptr != Param);

	UGsItemManager* ItemManager = GItem();
	if (nullptr == ItemManager)
	{
		GSLOG(Error, TEXT("nullptr == ItemManager"));
		return;
	}

	const ItemId CurrentItemId = ItemManager->GetTIDByDBID(Param->_itemDBID);
	if (INVALID_ITEM_ID == CurrentItemId)
	{
		GSLOG(Error, TEXT("INVALID_ITEM_ID == CurrentItemId, Param->_itemDBID:%lld"), Param->_itemDBID);
		return;
	}

	ItemChanged(CurrentItemId);
}

void FGsGameStateSeal::OnRemoveItem(FGsItemMessageParamBase& InParam)
{
	const FGsRemoveItem* Param = StaticCast<const FGsRemoveItem*>(&InParam);
	check(nullptr != Param);

	ItemChanged(Param->_itemTID);
}

void FGsGameStateSeal::OnContentsUnlock(const IGsMessageParam* InParam)
{
	const FGsPrimitivePairInt32* param = StaticCast<const FGsPrimitivePairInt32*>(InParam);
	if (nullptr == param)
	{
		GSLOG(Error, TEXT("nullptr == param"));
		return;
	}

	FGsSealManager* SealManager = GSSeal();
	if (nullptr == SealManager)
	{
		GSLOG(Error, TEXT("nullptr == SealManager"));
		return;
	}

	// 매니저를 통해 데이터 갱신
	bool isChanged = SealManager->OnUnlockCategory(param->_second);

	if (!_window.IsValid())
	{
		return;
	}

	// 언락된 것이 있다면, 윈도우도 갱신
	if (isChanged)
	{
		_window->OnUnlockCategory();
	}
}
