#include "GsGameStateMonsterCollection.h"
#include "GsContentsGameBase.h"
#include "GameFlow/GameContents/GsContentsMode.h"
#include "Management/GsMessageHolder.h"
#include "Management/ScopeGlobal/GsGameDataManager.h"
#include "Management/ScopeGlobal/GsUIManager.h"
#include "Management/ScopeGame/GsCodexManager.h"
#include "Message/MessageParam/GsMonsterKnowledgeCollection.h"
#include "MonsterKnowledgeCollection/EMonsterKnowledgeEnumData.h"
#include "UI/UILib/Base/GsUIWidgetBase.h"
#include "Message/MessageParam/GsItemMessageParam.h"
#include "Item/GsItemManager.h"
#include "Net/GsNetSendServiceWorld.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "Message/GsMessageItem.h"
#include "Classes/GsMessageHandlerOneParam.h"

FGsGameStateMonsterCollection::FGsGameStateMonsterCollection()
	: FGsContentsGameBase(FGsContentsMode::InGame::ContentsMonsterCollection)
{
	// 스폰 시, 대륙 탐사 쪽의 컬렉션 정보 변동
	_gameObjectDelegate = GMessage()->GetGameObject().AddRaw(
		MessageGameObject::LOCAL_SPAWN_ME_COMPLETE, this, &FGsGameStateMonsterCollection::OnLocalPlayerSpawnComplete);
}

FGsGameStateMonsterCollection::~FGsGameStateMonsterCollection()
{
	if (GMessage())
		GMessage()->GetGameObject().Remove(_gameObjectDelegate);
}

void FGsGameStateMonsterCollection::ItemChanged(const ItemId InItemId)
{
	if (INVALID_ITEM_ID == InItemId || !_window.IsValid())
	{
		return;
	}

	_window->OnChangeItem(InItemId);
}

void FGsGameStateMonsterCollection::Enter()
{
	FGsContentsGameBase::Enter();

	if (UGsUIManager* UiManager = GUI())
	{
		TWeakObjectPtr<UGsUIWidgetBase> Widget = UiManager->OpenAndGetWidget(TEXT("WindowMonsterCollection"));
		if (Widget.IsValid())
		{
			_window = Cast<UGsUIWindowMonsterCollection>(Widget);
		}
	}

	// 몬스터 지식 포인트 보이게 수정
	SetNavigationBar(CurrencyType::MONSTER_BOOK_POINT);

	RegisterMessages();

	// 지역 일람 진입 시, 지식 관련 데이터를 얻기 위해 패킷을 전송. 카테고리 ID 는 필요 없으니 첫 요소를 전송
	// UGsUIWindowMonsterCollection::NativeConstruct() 가 먼저 열려서 해당 부분에서 호출하도록...
	// 사실 이건 어색한 동작이다. State 에서 Enter 체크를 하고 Window 가 열리는 게 논리적으로 맞기 때문
	//FGsNetSendServiceWorld::Send_Codex(GsCodex()->GetCategoryIdWithIndex(0));
}

void FGsGameStateMonsterCollection::Exit()
{
	// 주의: Exit 시 Window 닫기를 하지 않음(UI Stack과 State연계로직 때문. OnBack, CloseAll등의 로직에 의해 닫음)
	_window = nullptr;

	// 메세지 해지
	UnregisterMessages();

	// HUD 관련 레드닷 갱신 요청	
	GMessage()->GetHudRedDot().SendMessage(MessageContentHUDRedDot::MONSTER_COLLECTION, true);

	FGsContentsGameBase::Exit();
}

void FGsGameStateMonsterCollection::RegisterMessages()
{
	MMonsterKnowledgeCollection& MessageUiHolder = GMessage()->GetMonsterKnowledgeCollection();


	_messageUiDelegates.Emplace(MessageUiHolder.AddRaw(MessageContentMonsterKnowledge::INVALIDATE_BOOK
		, this
		, &FGsGameStateMonsterCollection::OnInvalidateBook));

	_messageUiDelegates.Emplace(MessageUiHolder.AddRaw(MessageContentMonsterKnowledge::INVALIDATE_BOOK_SET
		, this
		, &FGsGameStateMonsterCollection::OnInvalidateBookSet));

	_messageUiDelegates.Emplace(MessageUiHolder.AddRaw(MessageContentMonsterKnowledge::INVALIDATE_SLATE
		, this
		, &FGsGameStateMonsterCollection::OnInvalidateSlate));

	_messageUiDelegates.Emplace(MessageUiHolder.AddRaw(MessageContentMonsterKnowledge::SHOTCUT_SLATE
		, this
		, &FGsGameStateMonsterCollection::OnGoSlate));


	_messageUiDelegates.Emplace(MessageUiHolder.AddRaw(MessageContentMonsterKnowledge::INVALIDATE_CODEX
		, this
		, &FGsGameStateMonsterCollection::OnInvalidateCodex));

	_messageUiDelegates.Emplace(MessageUiHolder.AddRaw(MessageContentMonsterKnowledge::INVALIDATE_KNOWLEDGE_CONVERT
		, this
		, &FGsGameStateMonsterCollection::OnInvalidateKnowledgeConvert));
	
	_messageUiDelegates.Emplace(MessageUiHolder.AddRaw(MessageContentMonsterKnowledge::INVALIDATE_CATEGORY_LEVEL_UP
		, this
		, &FGsGameStateMonsterCollection::OnInvalidateCategoryLevelUp));

	_messageUiDelegates.Emplace(MessageUiHolder.AddRaw(MessageContentMonsterKnowledge::INVALIDATE_CODEX_MAP_GROUP_EXP_UPDATE
		, this
		, &FGsGameStateMonsterCollection::OnInvalidateKnowledgeAmount));

	_messageUiDelegates.Emplace(MessageUiHolder.AddRaw(MessageContentMonsterKnowledge::INVALIDATE_CODEX_MAP_GROUP_GEM
		, this
		, &FGsGameStateMonsterCollection::OnInvalidateKnowledgeAmount));


	_messageUiDelegates.Emplace(MessageUiHolder.AddRaw(MessageContentMonsterKnowledge::INAVLIDATE_SLATE_CURRENCY
		, this
		, &FGsGameStateMonsterCollection::OnUpdateCurrency));
	//MessageContentMonsterKnowledge::INAVLIDATE_SLATE_CURRENCY

	//MUserBasicInfo& basicInfo = GMessage()->GetUserBasicInfo();

	//_userInfoHandlerList.Emplace(basicInfo.AddRaw(MessageUserInfo::CURRENCY
	//	, this
	//	, &FGsGameStateMonsterCollection::OnUpdateCurrency));

	// 아이템 보유 수치 변동
	_itemActionDelegates.Empty();
	_itemActionDelegates.Emplace(GMessage()->GetItem().AddRaw(MessageItem::ADDITEMLIST, this, &FGsGameStateMonsterCollection::OnAddItemList));
	_itemActionDelegates.Emplace(GMessage()->GetItem().AddRaw(MessageItem::UPDATEITEM, this, &FGsGameStateMonsterCollection::OnUpdateItem));
	_itemActionDelegates.Emplace(GMessage()->GetItem().AddRaw(MessageItem::REMOVEITEM, this, &FGsGameStateMonsterCollection::OnRemoveItem));
}

void FGsGameStateMonsterCollection::UnregisterMessages()
{
	if (!GMessage())
		return;

	MUserBasicInfo& basicInfo = GMessage()->GetUserBasicInfo();
	for (TPair<MessageUserInfo, FDelegateHandle>& msgHandler : _userInfoHandlerList)
	{
		basicInfo.Remove(msgHandler);
	}
	_userInfoHandlerList.Empty();

	MMonsterKnowledgeCollection& MessageUiHolder = GMessage()->GetMonsterKnowledgeCollection();
	for (const TPair<MessageContentMonsterKnowledge, FDelegateHandle>& DelegatePair : _messageUiDelegates)
	{
		MessageUiHolder.Remove(DelegatePair);
	}

	MItem& ItemHandlers = GMessage()->GetItem();
	for (const TPair<MessageItem, FDelegateHandle>& DelegatePair : _itemActionDelegates)
	{
		ItemHandlers.Remove(DelegatePair);
	}
	_itemActionDelegates.Empty();
}

void FGsGameStateMonsterCollection::OnInvalidateBook(const IGsMessageParam* InParam)
{
	const FGsPrimitiveInt32* param = StaticCast<const FGsPrimitiveInt32*>(InParam);
	if (nullptr == param)
	{
		return;
	}

	if (_window.IsValid() && _window->IsActive())
	{
		_window->InvalidateBook(StaticCast<uint32>(param->_data));
	}
}

void FGsGameStateMonsterCollection::OnInvalidateBookSet(const IGsMessageParam* InParam)
{
	const FGsMessageMonsterCollectionBookResultSet* param = StaticCast<const FGsMessageMonsterCollectionBookResultSet*>(InParam);
	if (nullptr == param)
	{
		return;
	}

	if (_window.IsValid() && _window->IsActive())
	{
		_window->InvalidateBook(param->_data);
	}
}

void FGsGameStateMonsterCollection::OnInvalidateSlate(const IGsMessageParam* InParam)
{
	const FGsPrimitiveInt32* param = StaticCast<const FGsPrimitiveInt32*>(InParam);
	if (nullptr == param)
	{
		return;
	}

	if (_window.IsValid() && _window->IsActive())
	{
		_window->InvalidateSlate(StaticCast<MonsterKnowledgeSlateId>(param->_data));
	}
}

void FGsGameStateMonsterCollection::OnInvalidateCodex(const IGsMessageParam* InParam)
{
	if (_window.IsValid() && _window->IsActive())
	{
		_window->InvalidateCodex();
	}
}

void FGsGameStateMonsterCollection::OnInvalidateKnowledgeConvert(const IGsMessageParam* InParam)
{
	if (_window.IsValid() && _window->IsActive())
	{
		_window->InvalidateKnowledgeTab();
	}
}

void FGsGameStateMonsterCollection::OnInvalidateCategoryLevelUp(const IGsMessageParam* InParam)
{
	const FGsMessageCodexCategoryLevelUp* param = StaticCast<const FGsMessageCodexCategoryLevelUp*>(InParam);
	if (nullptr == param)
		return;

	if (_window.IsValid() && _window->IsActive())
	{
		_window->InvalidateCategoryLevelUp(param->_codexCategoryId, param->_codexCategoryLevel, param->_mapGroupLevelUpCount);
	}
}

void FGsGameStateMonsterCollection::OnInvalidateKnowledgeAmount(const IGsMessageParam* InParam)
{
	if (_window.IsValid() && _window->IsActive())
	{
		// 지식 획득 시, 맵 그룹 리스트는 실시간 갱신을 하지 않고, 대신 총 지식 획득 경험치만 갱신해 주자.
		_window->InvalidateMapGroupList();
		_window->InvalidateKnowledgeAmount();
	}
}

void FGsGameStateMonsterCollection::OnGoSlate(const IGsMessageParam* InParam)
{
	const FGsPrimitiveUInt32* param = StaticCast<const FGsPrimitiveUInt32*>(InParam);
	if (nullptr == param)
	{
		return;
	}

	if (_window.IsValid() && _window->IsActive())
	{
		_window->GoSelectedSlate(param->_data);
	}
}

void FGsGameStateMonsterCollection::OnUpdateCurrency(const IGsMessageParam* InParam)
{
	if (_window.IsValid() && _window->IsActive())
	{
		_window->InvalidateSlateCurrency();
	}
}

void FGsGameStateMonsterCollection::OnAddItemList(FGsItemMessageParamBase& InParam)
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

void FGsGameStateMonsterCollection::OnUpdateItem(FGsItemMessageParamBase& InParam)
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

void FGsGameStateMonsterCollection::OnRemoveItem(FGsItemMessageParamBase& InParam)
{
	const FGsRemoveItem* param = StaticCast<const FGsRemoveItem*>(&InParam);
	check(nullptr != param);

	ItemChanged(param->_itemTID);
}

void FGsGameStateMonsterCollection::OnLocalPlayerSpawnComplete(const IGsMessageParam* InParam)
{
	if (GsCodex())
		GsCodex()->InvalidateExplorePassivityCollectionData();
}
