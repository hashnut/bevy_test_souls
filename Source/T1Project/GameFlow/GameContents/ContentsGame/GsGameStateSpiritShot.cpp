#include "GsGameStateSpiritShot.h"

#include "GameFlow/GameContents/GsContentsMode.h"
#include "Item/GsItemManager.h"
#include "Management/GsMessageHolder.h"
#include "Management/ScopeGame/GsSpiritShotManager.h"
#include "Management/ScopeGame/GsUnlockManager.h"
#include "Management/ScopeGlobal/GsGameFlowManager.h"
#include "Message/GsMessageContents.h"
#include "Message/MessageParam/GsItemMessageParam.h"
#include "Message/MessageParam/GsMessageParam.h"
#include "Message/MessageParam/GsSpiritShotMessageParam.h"
#include "T1Project.h"
#include "UI/UIContent/Window/GsUIWindowSpiritShot.h"
#include "Unlock/GsUnlockDefine.h"


FGsGameStateSpiritShot::FGsGameStateSpiritShot()
	: Super(FGsContentsMode::InGame::ContentsSpiritShot)
{

}

void FGsGameStateSpiritShot::Init()
{
	Super::Init();

	FGsMessageHolder* messageHolder = GMessage();
	if (nullptr == messageHolder)
	{
		GSLOG(Error, TEXT("nullptr == messageHolder"));
		return;
	}
	
	// 상태 진입에 대한 메시지는 게임 초기화 단계에서부터 필요하다. 나머지는 상태 진입 후에 바인딩해도 충분함
	_openDelegate = messageHolder->GetSpiritShot().AddRaw(FGsSpiritShotMessageParamOpen::MESSAGE_ID, this, &FGsGameStateSpiritShot::Open);

	// 언락 메시지는 윈도우가 열리기 전부터 유효해야 한다
	_unlockDelegate = messageHolder->GetUnlock().AddRaw(MessageContentUnlock::NOTIFY_CONTENTS_UNLOCK, this, &FGsGameStateSpiritShot::OnContentsUnlock);
}

void FGsGameStateSpiritShot::Close()
{
	FGsMessageHolder* messageHolder = GMessage();
	if (nullptr == messageHolder)
	{
		GSLOG(Error, TEXT("nullptr == messageHolder"));
		return;
	}

	messageHolder->GetSpiritShot().Remove(_openDelegate);
	messageHolder->GetUnlock().Remove(_unlockDelegate);

	Super::Close();
}

void FGsGameStateSpiritShot::Enter()
{
	Super::Enter();

	_window = UGsUIWindowSpiritShot::OpenWindow(this);

	// 윈도우 상단의 재화 표시줄
	SetNavigationBar(CurrencyType::NONE);

	RegisterMessages();


	if (!_window.IsValid())
	{
		GSLOG(Warning, TEXT("!_window.IsValid()"));
		return;
	}

	// 히스토리가 없을 경우, 파라미터가 없이 state 에 진입했다는 것을 의미
	// 디폴트 데이터를 히스토리에 입력
	if (!HasHistory())
	{
		FGsSpiritShotManager* spiritShotMgr = GSSpiritShot();
		if (nullptr == spiritShotMgr)
		{
			GSLOG(Error, TEXT("nullptr == spiritShotMgr"));
			return;
		}
		PushHistory(UGsUIWindowSpiritShot::Parameters(spiritShotMgr->GetDefaultCategoryId()));
	}
	// 히스토리가 있을 경우, 초기화 데이터가 있는 상태로 state 에 진입했다는 것을 의미
	// 뒤로가기 등으로 스택에서부터 윈도우가 복구되는 경우, 현재 네비상태 저장하지 않음
	else
	{
	}

	UGsUIWindowSpiritShot::Parameters lastParameters;
	if (PopHistory(lastParameters))
	{
		_window->SetParameters(MoveTemp(lastParameters));
	}
}

void FGsGameStateSpiritShot::Exit()
{
	// 메시지 해지
	UnregisterMessages();

	Super::Exit();
}

bool FGsGameStateSpiritShot::CanEnter(bool InShowUnableMessage) const
{
	FGsUnlockManager* unlockMgr = GSUnlock();
	if (nullptr == unlockMgr)
	{
		GSLOG(Error, TEXT("nullptr == unlockMgr"));
		return false;
	}

	if (unlockMgr->IsContentsUnlock(EGsUnlockContentsType::SPIRIT_SHOT))
	{
		return true;
	}

	if (InShowUnableMessage)
	{
		unlockMgr->ShowLockTicker(EGsUnlockContentsType::SPIRIT_SHOT);
	}

	return false;
}

bool FGsGameStateSpiritShot::HasHistory() const
{
	return 0 < _historyStack.Num();
}

void FGsGameStateSpiritShot::PushHistory(UGsUIWindowSpiritShot::Parameters InParameters)
{
	if (_historyStack.Num() > 0)
	{
		// Top 과 동일하면 스킵
		if (_historyStack.Top() == InParameters)
		{
			return;
		}
	}

	_historyStack.Push(MoveTemp(InParameters));
}

bool FGsGameStateSpiritShot::PopHistory(OUT UGsUIWindowSpiritShot::Parameters& OutParameters)
{
	if (HasHistory())
	{
		OutParameters = _historyStack.Pop(false);
		return true;
	}

	return false;
}

void FGsGameStateSpiritShot::ClearHistory()
{
	_historyStack.Empty();
}

void FGsGameStateSpiritShot::RegisterMessages()
{
	FGsMessageHolder* messageHolder = GMessage();
	if (nullptr == messageHolder)
	{
		GSLOG(Error, TEXT("nullptr == messageHolder"));
		return;
	}
	
	// 상태에 진입한 후에야, 필요한 메시지들을 바인딩해 줄 것이다.
	MSpiritShot& spiritShotMessageHandler = messageHolder->GetSpiritShot();
	
	// 천마석 시스템 관련
	_spiritShotDelegates.Empty();
	_spiritShotDelegates.Emplace(spiritShotMessageHandler.AddRaw(FGsSpiritShotMessageParamInfo::MESSAGE_ID, this, &FGsGameStateSpiritShot::OnSpiritShotInfo));
	_spiritShotDelegates.Emplace(spiritShotMessageHandler.AddRaw(FGsSpiritShotMessageParamAckEnchant::MESSAGE_ID, this, &FGsGameStateSpiritShot::OnSpiritShotAckEnchant));

	// 천마석 연출 관련
	_spiritShotDelegates.Emplace(spiritShotMessageHandler.AddRaw(FGsSpiritShotMessageParamEnchantEffectFinished::MESSAGE_ID, this, &FGsGameStateSpiritShot::OnSpiritShotEnchantEffectFinished));

	// 재화 보유 수치 변동
	_userInfoDelegates.Empty();
	_userInfoDelegates.Emplace(messageHolder->GetUserBasicInfo().AddRaw(MessageUserInfo::CURRENCY, this, &FGsGameStateSpiritShot::OnUserCurrency));

	// 아이템 보유 수치 변동
	_itemActionDelegates.Empty();
	_itemActionDelegates.Emplace(messageHolder->GetItem().AddRaw(MessageItem::ADDITEMLIST, this, &FGsGameStateSpiritShot::OnAddItemList));
	_itemActionDelegates.Emplace(messageHolder->GetItem().AddRaw(MessageItem::UPDATEITEM, this, &FGsGameStateSpiritShot::OnUpdateItem));
	_itemActionDelegates.Emplace(messageHolder->GetItem().AddRaw(MessageItem::REMOVEITEM, this, &FGsGameStateSpiritShot::OnRemoveItem));
}

void FGsGameStateSpiritShot::UnregisterMessages()
{
	FGsMessageHolder* messageHolder = GMessage();
	if (nullptr == messageHolder)
	{
		GSLOG(Error, TEXT("nullptr == messageHolder"));
		return;
	}

	MSpiritShot& spiritShotMessageHandler = messageHolder->GetSpiritShot();
	for (const TPair<MessageContentSpiritShot, FDelegateHandle>& delegatePair : _spiritShotDelegates)
	{
		spiritShotMessageHandler.Remove(delegatePair);
	}
	_spiritShotDelegates.Empty();

	MUserBasicInfo& userBasicInfoMessageHandler = messageHolder->GetUserBasicInfo();
	for (const TPair<MessageUserInfo, FDelegateHandle>& delegatePair : _userInfoDelegates)
	{
		userBasicInfoMessageHandler.Remove(delegatePair);
	}
	_userInfoDelegates.Empty();

	MItem& itemMessageHandler = messageHolder->GetItem();
	for (const TPair<MessageItem, FDelegateHandle>& delegatePair : _itemActionDelegates)
	{
		itemMessageHandler.Remove(delegatePair);
	}
	_itemActionDelegates.Empty();
}

void FGsGameStateSpiritShot::CurrencyChanged(const CurrencyType InCurrencyType)
{
	if (CurrencyType::NONE == InCurrencyType || !_window.IsValid())
	{
		return;
	}

	_window->OnChangeCurrency(InCurrencyType);
}

void FGsGameStateSpiritShot::ItemChanged(const ItemId InItemId)
{
	if (INVALID_ITEM_ID == InItemId || !_window.IsValid())
	{
		return;
	}

	_window->OnChangeItem(InItemId);
}

void FGsGameStateSpiritShot::Open(const IGsMessageParam* InParam)
{
	if (!CanEnter(true))
	{
		return;
	}

	
	const FGsSpiritShotMessageParamOpen* param = InParam->Cast< const FGsSpiritShotMessageParamOpen>();
	if (nullptr == param)
	{
		GSLOG(Error, TEXT("nullptr == param"));
		return;
	}

	FGsGameFlowManager* gameFlowManager = GMode();
	if (nullptr == gameFlowManager)
	{
		GSLOG(Error, TEXT("nullptr == gameFlowManager"));
		return;
	}

	// 현재 Flow 에서, State 가 SpiritShot 가 아니라면 State 변경
	if (gameFlowManager->GetCurrentContentStateIndex() != FGsContentsMode::InGame::ContentsSpiritShot)
	{
		FGsMessageHolder* messageHolder = GMessage();
		if (nullptr == messageHolder)
		{
			GSLOG(Error, TEXT("nullptr == messageHolder"));
			return;
		}

		messageHolder->GetContents().SendMessage(MessageContents::ACTIVE_WINDOW_SPIRIT_SHOT);
	}
	// State 가 천마석이면, Parameters 업데이트
	else
	{
		if (_window.IsValid())
		{
			_window->SetParameters(UGsUIWindowSpiritShot::Parameters(param->_spiritShotCategoryId));
		}
	}
}

void FGsGameStateSpiritShot::OnSpiritShotInfo(const IGsMessageParam* InParam)
{
	// 해당 경우는 인게임 -> 로비 진입 시 정보 전달이므로, 따로 처리할 것은 없어 보임
}

void FGsGameStateSpiritShot::OnSpiritShotAckEnchant(const IGsMessageParam* InParam)
{
	if (!_window.IsValid())
	{
		return;
	}

	const FGsSpiritShotMessageParamAckEnchant* param = InParam->Cast<const FGsSpiritShotMessageParamAckEnchant>();
	if (nullptr == param)
	{
		GSLOG(Error, TEXT("nullptr == param"));
		return;
	}

	if (!param->_packetNoError)
	{
		GSLOG(Error, TEXT("!param->_packetNoError"));
	}

	_window->OnSpiritShotAckEnchant(param->_packetNoError, param->_spiritShotLevelId, param->_itemIdAmountPairList);
}

void FGsGameStateSpiritShot::OnSpiritShotEnchantEffectFinished(const IGsMessageParam* InParam)
{
	if (!_window.IsValid())
	{
		return;
	}

	_window->OnFinishedEnchantEffect();
}

void FGsGameStateSpiritShot::OnUserCurrency(uint64 InCurrncyType)
{
	CurrencyChanged(StaticCast<CurrencyType>(InCurrncyType));
}

void FGsGameStateSpiritShot::OnAddItemList(FGsItemMessageParamBase& InParam)
{
	const FGsAddItemList* param = StaticCast<const FGsAddItemList*>(&InParam);
	check(nullptr != param);

	UGsItemManager* itemManager = GItem();
	if (nullptr == itemManager)
	{
		GSLOG(Error, TEXT("nullptr == itemManager"));
		return;
	}

	TSet<ItemId> itemIdSet;
	for (const ItemDBId currentItemDBId : param->_itemDbidList)
	{
		const ItemId currentItemId = itemManager->GetTIDByDBID(currentItemDBId);
		if (INVALID_ITEM_ID == currentItemId)
		{
			GSLOG(Error, TEXT("INVALID_ITEM_ID == currentItemId, currentItemDBId: %lld"), currentItemDBId);
			continue;
		}

		itemIdSet.Add(currentItemId);
	}

	for (const ItemId CurrentItemId : itemIdSet)
	{
		ItemChanged(CurrentItemId);
	}
}

void FGsGameStateSpiritShot::OnUpdateItem(FGsItemMessageParamBase& InParam)
{
	const FGsUpdateItem* param = StaticCast<const FGsUpdateItem*>(&InParam);
	check(nullptr != param);

	UGsItemManager* itemManager = GItem();
	if (nullptr == itemManager)
	{
		GSLOG(Error, TEXT("nullptr == itemManager"));
		return;
	}

	const ItemId currentItemId = itemManager->GetTIDByDBID(param->_itemDBID);
	if (INVALID_ITEM_ID == currentItemId)
	{
		GSLOG(Error, TEXT("INVALID_ITEM_ID == currentItemId, param->_itemDBID:%lld"), param->_itemDBID);
		return;
	}

	ItemChanged(currentItemId);
}

void FGsGameStateSpiritShot::OnRemoveItem(FGsItemMessageParamBase& InParam)
{
	const FGsRemoveItem* param = StaticCast<const FGsRemoveItem*>(&InParam);
	check(nullptr != param);

	ItemChanged(param->_itemTID);
}

void FGsGameStateSpiritShot::OnContentsUnlock(const IGsMessageParam* InParam)
{
	const FGsPrimitivePairInt32* param = StaticCast<const FGsPrimitivePairInt32*>(InParam);
	if (nullptr == param)
	{
		GSLOG(Error, TEXT("nullptr == param"));
		return;
	}

	FGsSpiritShotManager* spiritShotMgr = GSSpiritShot();
	if (nullptr == spiritShotMgr)
	{
		GSLOG(Error, TEXT("nullptr == spiritShotMgr"));
		return;
	}

	// 매니저를 통해 데이터 갱신
	bool isChanged = spiritShotMgr->OnUnlockCategory(param->_second);

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
