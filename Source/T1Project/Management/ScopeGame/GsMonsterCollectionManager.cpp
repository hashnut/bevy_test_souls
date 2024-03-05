#include "GsMonsterCollectionManager.h"
#include "GsUnlockManager.h"
#include "GsDepotManager.h"
#include "../Data/GsDataContainManager.h"
#include "Management/ScopeGlobal/GsGameDataManager.h"
#include "../Item/GsItemManager.h"
#include "../Currency/GsCostPackage.h"
#include "../MonsterKnowledgeCollection/EMonsterKnowledgeEnumData.h"
#include "../MonsterKnowledgeCollection/GsMonsterKnowledgeSlateNode.h"
#include "../Dungeon/GsDungeonMessageHolder.h"
#include "../GsMessageHolder.h"
#include "../ScopeGlobal/GsSoundManager.h"
#include "../ScopeGame/GsCodexManager.h"
#include "../Message/GsMessageGameObject.h"
#include "../Message/GsMessageContents.h"
#include "../Message/MessageParam/GsMessageParam.h"
#include "../Message/MessageParam/GsItemMessageParam.h"
#include "../Message/MessageParam/GsItemContentsMessageParam.h"
#include "../Message/MessageParam/GsMonsterKnowledgeCollection.h"
#include "../Sound/GsSoundPlayer.h"
#include "../UI/UIContent/Tray/NotiMessage/GsUISectionMessageDefine.h"
#include "../UI/UIContent/Helper/GsUIHelper.h"

#include "GsTable.h"
#include "GsTableManager.h"
#include "DataSchema/Sound/GsSchemaSoundResData.h"
#include "DataSchema/GameObject/Npc/GsSchemaNpcData.h"
#include "DataSchema/Item/GsSchemaItemIngredientDetail.h"
#include "DataSchema/Item/GsSchemaItemEffectGainMonsterKnowledgeExp.h"
#include "DataSchema/Item/GsSchemaItemEffectUnlockMonsterKnowledgeSlate.h"
#include "DataSchema/MonsterKnowledgeCollection/GsSchemaMonsterKnowledgeSlate.h"
#include "DataSchema/MonsterKnowledgeCollection/GsSchemaMonsterKnowledgeSlateCategory.h"
#include "../Data/GsGlobalConstant.h"
#include "Shared/Client/SharedEnums/SharedItemEnum.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "../Net/GsNetSendServiceWorld.h"
#include "Shared/Client/SharedEnums/SharedPassivityEnum.h"
#include "T1Project.h"



using WindowMessageHolder = FGsDungeonMessageHolder;
#define WINDOW_MESSAGE_LOCK {	WindowMessageHolder holer(_messageHolder);
#define WINDOW_MESSAGE_UNLOCK }


void FGsMonsterCollectionManager::Initialize()
{
	WINDOW_MESSAGE_LOCK

	_contentsStatAllSet.Empty();

	TMap<MonsterKnowledgeSlateId, TSet<ItemId>> costSlateOpenItemSet;
	BuildCostItem(_costResetItem, _costBookExpItemSet, _amountIncreaseBookExpItemSet, costSlateOpenItemSet);
	BuildBookSet(_bookHolder);
	BuildSlate(costSlateOpenItemSet, _slateHolder);

	_passivityCollectionData.Set(PassivityOwnerType::MONSTER_BOOK, MessageGameObject::PASSIVITY_UPDATE_COLLECTION);
	_slateCategoryDataSet.Empty();

	for (auto& item : _bookHolder)
	{
		if (false == item._OnBookDelegate.IsBound())
		{
			item._OnBookDelegate.BindRaw(this, &FGsMonsterCollectionManager::OnStateChangedBook);
		}
	}

	for (auto& item : _slateHolder)
	{
		item.Initialized();

		if (false == item.OnDelegateSlateDelegate.IsBound())
		{
			item.OnDelegateSlateDelegate.BindRaw(this, &FGsMonsterCollectionManager::OnStateChangedSlate);
		}

		if (false == item.OnDelegateSpecialNodeCurrency.IsBound())
		{
			item.OnDelegateSpecialNodeCurrency.BindRaw(this, &FGsMonsterCollectionManager::OnChangedSpecialNodeCurrency);
		}
	}

	RegisterMessages();

	WINDOW_MESSAGE_UNLOCK
}

void FGsMonsterCollectionManager::Finalize()
{
	WINDOW_MESSAGE_LOCK

	UnegisterMessages();

	_contentsStatAllSet.Empty();

	_amountIncreaseBookExpItemSet.Empty();
	_costBookExpItemSet.Empty();

	for (auto& item : _slateHolder)
	{
		item.Finalized();
	}

	_bookHolder.Empty();
	_slateHolder.Empty();
	_slateCategoryDataSet.Empty();

	_passivityCollectionData.Clear(false);

	WINDOW_MESSAGE_UNLOCK
}

bool FGsMonsterCollectionManager::CheckRedDot(bool bInUpdate /*= true*/)
{
	if (bInUpdate)
	{
		bool isRedDot = false;

		if (GGameData() && GGameData()->IsCodexRedDot())
			isRedDot = true;

		isRedDot |= CheckRedDot(EMonsterCollectionMainTab::Codex);
		isRedDot |= CheckRedDot(EMonsterCollectionMainTab::Slate);

		bool isPrevRedDot = IsRedDot();

		SetRedDot(isRedDot);

		if (IsRedDot() != isPrevRedDot)
		{	// 2021/08/13 PKT - RedDot의 변경을 HUD에 알린다.
			GMessage()->GetHudRedDot().SendMessage(MessageContentHUDRedDot::MONSTER_COLLECTION, false);
		}
	}
	
	return IsRedDot();
}

bool FGsMonsterCollectionManager::CheckRedDot(MonsterKnowledgeBookGrade InGrade) const
{
	bool isRedDot = false;

	//if (MonsterKnowledgeBookGrade::MAX == InGrade)
	//{
	//	isRedDot = CheckRedDot(EMonsterCollectionMainTab::Codex);
	//}
	//else
	//{
	//	for (auto item : _redDotBookSet)
	//	{
	//		if (_bookHolder.FindByPredicate([item, InGrade](const FGsMonsterKnowledgeBook& InData)
	//			{
	//				return (InData.Id() == item && InData.BookGrade() == InGrade);
	//			}
	//		))
	//		{
	//			isRedDot = true;
	//			break;
	//		}
	//	}
	//}

	return isRedDot;
}

bool FGsMonsterCollectionManager::CheckRedDot(EMonsterKnowledgeSlateCategory InCategory) const
{
	bool isRedDot = false;

	if (EMonsterKnowledgeSlateCategory::MAX == InCategory)
	{
		isRedDot = CheckRedDot(EMonsterCollectionMainTab::Slate);
	}
	else
	{
		for (auto item : _redDotSlateSet)
		{
			if (_slateHolder.FindByPredicate([item, InCategory](const FGsMonsterKnowledgeSlate& InData)
				{
					return (InData.Id() == item && InData.CategoryType() == InCategory);
				}
			))
			{
				isRedDot = true;
				break;
			}
		}
	}

	return isRedDot;
}

bool FGsMonsterCollectionManager::CheckRedDot(EMonsterCollectionMainTab InPage) const
{
	bool isRedDot = false;

	switch (InPage)
	{
	case EMonsterCollectionMainTab::Codex:
	{
		if (!GsCodex())
			return isRedDot;

		isRedDot = GsCodex()->CheckRedDot();

		break;
	}
	case EMonsterCollectionMainTab::Slate:
		isRedDot = (0 != _redDotSlateSet.Num());
		break;
	}

	return isRedDot;
}

void FGsMonsterCollectionManager::RegisterMessages()
{
	FGsMessageHolder* MessageManager = GMessage();
	if (nullptr == MessageManager)
	{
		GSLOG(Error, TEXT("nullptr == MessageManager"));
		return;
	}
	
	_listMonsterKnowledgeDelegates.Emplace(MessageManager->GetMonsterKnowledgeCollection().AddRaw(MessageContentMonsterKnowledge::USE_ITEM_SLATE
		, this
		, &FGsMonsterCollectionManager::OnSlateUseActiveItem));
	
	_listMonsterKnowledgeDelegates.Emplace(MessageManager->GetMonsterKnowledgeCollection().AddRaw(MessageContentMonsterKnowledge::USE_ITEM_SLATE_RESET
		, this
		, &FGsMonsterCollectionManager::OnSlateUseResetItem));

	_listUserInfoDelegates.Emplace(MessageManager->GetUserBasicInfo().AddRaw(MessageUserInfo::CURRENCY
		, this
		, &FGsMonsterCollectionManager::OnUpdateCurrency));

	// 2021/08/02 PKT - inventory Item - New ..
	_listItemActionDelegates.Emplace(MessageManager->GetItem().AddRaw(MessageItem::ADDITEMLIST
		, this
		, &FGsMonsterCollectionManager::OnInventoryAddItem));

	// 2021/08/02 PKT - inventory Item - Update ..
	_listItemActionDelegates.Emplace(MessageManager->GetItem().AddRaw(MessageItem::UPDATEITEM
		, this
		, &FGsMonsterCollectionManager::OnInventoryUpdateItem));
	// 2021/08/02 PKT - inventory Item - Remove ..
	_listItemActionDelegates.Emplace(MessageManager->GetItem().AddRaw(MessageItem::REMOVEITEM
		, this
		, &FGsMonsterCollectionManager::OnInventoryRemoveItem));

	// 2021/08/02 PKT - Depot Item - New ..
	_listDepotItemActionDelegates.Emplace(MessageManager->GetItemContents().AddRaw(MessageContentItem::DEPOT_ADD_ITEM
		, this
		, &FGsMonsterCollectionManager::OnAddToDepot));
	// 2021/08/02 PKT - Depot Item - Update ..
	_listDepotItemActionDelegates.Emplace(MessageManager->GetItemContents().AddRaw(MessageContentItem::DEPOT_INVALIDATE
		, this
		, &FGsMonsterCollectionManager::OnChangeToDepot));
	// 2021/08/02 PKT - Depot Item - Remove ..
	_listDepotItemActionDelegates.Emplace(MessageManager->GetItemContents().AddRaw(MessageContentItem::DEPOT_REMOVE_ITEM
		, this
		, &FGsMonsterCollectionManager::OnRemoveToDepot));

	//_listGameObjectDelegate.Emplace(MessageManager->GetGameObject().AddRaw(
	//	MessageGameObject::LOCAL_SPAWN_ME_COMPLETE, this, &FGsMonsterKnowledgeCollectionManager::OnLocalPlayerSpawnComplete));
}

void FGsMonsterCollectionManager::UnegisterMessages()
{
	FGsMessageHolder* MessageManager = GMessage();
	if (nullptr == MessageManager)
	{
		GSLOG(Error, TEXT("nullptr == MessageManager"));
		return;
	}

	for (const TPair<MessageContentMonsterKnowledge, FDelegateHandle>& ItemDelegate : _listMonsterKnowledgeDelegates)
	{
		MessageManager->GetMonsterKnowledgeCollection().Remove(ItemDelegate);
	}
	_listMonsterKnowledgeDelegates.Empty();

	for (const TPair<MessageUserInfo, FDelegateHandle>& ItemDelegate : _listUserInfoDelegates)
	{
		MessageManager->GetUserBasicInfo().Remove(ItemDelegate);
	}
	_listUserInfoDelegates.Empty();

	for (const TPair<MessageItem, FDelegateHandle>& ItemDelegate : _listItemActionDelegates)
	{
		MessageManager->GetItem().Remove(ItemDelegate);
	}
	_listItemActionDelegates.Empty();

	for (const TPair<MessageContentItem, FDelegateHandle>& ItemDelegate : _listDepotItemActionDelegates)
	{
		MessageManager->GetItemContents().Remove(ItemDelegate);
	}
	_listDepotItemActionDelegates.Empty();

	for (const MsgGameObjHandle& ItemDelegate : _listGameObjectDelegate)
	{
		MessageManager->GetGameObject().Remove(ItemDelegate);
	}
	_listGameObjectDelegate.Empty();
}

void FGsMonsterCollectionManager::OnSlateUseActiveItem(const IGsMessageParam* InParam)
{
	const FGsPrimitiveInt32* param = StaticCast<const FGsPrimitiveInt32*>(InParam);
	if (nullptr == param)
	{
		GSLOG(Error, TEXT("nullptr == param"));
		return;
	}

	if (false == GSUnlock()->IsContentsUnlock(EGsUnlockContentsType::MONSTER_KNOWLEDGE_COLLECTION, true))
	{
		return;
	}

	for (const auto& item : _slateHolder)
	{
		if (item.IsCostItem(param->_data))
		{
			if (EMonsterKnowledgeSlateState::UnLock == item.State())
			{	
				// 2023/1/31 PKT - 이미 활성화 되어 있는 상태
				FText ticker;
				FText::FindText(TEXT("MonsterKnowledgeText"), TEXT("ticker_AlreadyActivatedSlate"), ticker);
				FGsUIHelper::TraySectionMessageTicker(EGsNotiSection::EXTSYSTEM, ticker);

				break;
			}
			else
			{
				// 2023/1/31 PKT - 도감 Slate 바로가기
				const FGsPrimitiveUInt32 message(item.Id());
				SendMessageToWindow(MessageContentMonsterKnowledge::SHOTCUT_SLATE, &message);

				break;
			}
		}
	}
}

void FGsMonsterCollectionManager::OnSlateUseResetItem(const IGsMessageParam* InParam)
{
	const FGsPrimitiveUInt64* message = StaticCast<const FGsPrimitiveUInt64*>(InParam);
	if (nullptr == message)
	{
		GSLOG(Error, TEXT("nullptr == message"));
		return;
	}

	if (false == GSUnlock()->IsContentsUnlock(EGsUnlockContentsType::MONSTER_KNOWLEDGE_COLLECTION, true))
	{
		return;
	}

	bool isUseable = false;

	for (const auto& item : _slateHolder)
	{
		// 2023/1/31 PKT - 초기화 아이템을  사용하려면 하나라도 Node가 열려 있어야 한다.
		for (int32 idx = 0; idx < item.NodeMaxCount(); ++idx)
		{
			if (const auto* node = item.NodeAt(idx))
			{
				if (EMonsterStoneSlateNodeState::Complete == node->State())
				{
					isUseable = true;
					break;
				}
			}
		}
	}

	if (false == isUseable)
	{
		FText ticker;
		FText::FindText(TEXT("MonsterKnowledgeText"), TEXT("NotUseableResetItem"), ticker);
		FGsUIHelper::TraySectionMessageTicker(EGsNotiSection::EXTSYSTEM, ticker);
	}
	else
	{
		// 2023/1/31 PKT - 서버로 보냄
		FGsNetSendServiceWorld::Send_MonsaterKnowledgeSlateReset(message->_data);
	}
}

void FGsMonsterCollectionManager::OnUpdateCurrency(uint64 InData)
{
	// 2023/11/10 PKT - 몬스터 도감이 리뉴얼 중 Manager가 분리 되었음.(Codex.. 때문에 이곳에서는 석판 관련 변경만 체크 함. )
	const CurrencyType TargetCurrencyType = static_cast<CurrencyType>(InData);

	if (CurrencyType::MONSTER_BOOK_POINT == TargetCurrencyType)
	{
		// 2023/11/10 PKT - Window 갱신
		SendMessageToWindow(MessageContentMonsterKnowledge::INAVLIDATE_SLATE_CURRENCY, nullptr);
	}
}

void FGsMonsterCollectionManager::OnInventoryAddItem(FGsItemMessageParamBase& InParam)
{
	FGsAddItemList* Param = static_cast<FGsAddItemList*>(&InParam);
	if (nullptr == Param)
	{
		return;
	}

	if (GItem())
	{
		for (int32 idx = 0; idx < Param->_itemDbidList.Num(); ++idx)
		{
			TWeakPtr<FGsItem> item = GItem()->FindItem(Param->_itemDbidList[idx]);
			if (false == item.IsValid())
			{
				GSLOG(Error, TEXT("nullptr == findData, Param->_itemDBID : %lld"), Param->_itemDbidList[idx]);
				return;
			}

			for (auto& slate : _slateHolder)
			{
				slate.CheckedCostItem(item.Pin()->GetTID());

				slate.CheckedSpecialNodeCurrency(item.Pin()->GetTID());;
			}
		}
	}
}

void FGsMonsterCollectionManager::OnInventoryUpdateItem(FGsItemMessageParamBase& InParam)
{
	const FGsUpdateItem* Param = StaticCast<const FGsUpdateItem*>(&InParam);
	check(Param);
	
	if (GItem())
	{
		TWeakPtr<FGsItem> item = GItem()->FindItem(Param->_itemDBID);
		if (false == item.IsValid())
		{
			GSLOG(Error, TEXT("nullptr == Item, Param->_itemDBID:%lld"), Param->_itemDBID);
			return;
		}

		for (auto& slate : _slateHolder)
		{
			slate.CheckedCostItem(item.Pin()->GetTID());

			slate.CheckedSpecialNodeCurrency(item.Pin()->GetTID());
		}
	}
}

void FGsMonsterCollectionManager::OnInventoryRemoveItem(FGsItemMessageParamBase& InParam)
{
	const FGsRemoveItem* Param = StaticCast<const FGsRemoveItem*>(&InParam);
	check(Param);

	for (auto& slate : _slateHolder)
	{
		slate.CheckedCostItem(Param->_itemTID);

		slate.CheckedSpecialNodeCurrency(Param->_itemTID);
	}
}

void FGsMonsterCollectionManager::OnAddToDepot(const IGsMessageParam* InParam)
{
	const FGsAddDepotItem* Param = StaticCast<const FGsAddDepotItem*>(InParam);
	if (nullptr == Param)
	{
		GSLOG(Error, TEXT("nullptr == Param"));
		return;
	}

	FGsDepotManager* depotManager = GSDepot();
	if (depotManager)
	{
		TWeakPtr<const FGsItem> findData = depotManager->FindDepotItemByDBID(Param->_itemDBID);
		if (false == findData.IsValid())
		{
			GSLOG(Error, TEXT("nullptr == findData, Param->_itemDBID:%lld"), Param->_itemDBID);
			return;
		}

		for (auto& slate : _slateHolder)
		{
			slate.CheckedCostItem(findData.Pin().Get()->GetTID());

			slate.CheckedSpecialNodeCurrency(findData.Pin().Get()->GetTID());
		}
	}
}

void FGsMonsterCollectionManager::OnChangeToDepot(const IGsMessageParam* InParam)
{
	const FGsUpdateDepotItem* Param = StaticCast<const FGsUpdateDepotItem*>(InParam);
	if (nullptr == Param)
	{
		GSLOG(Error, TEXT("nullptr == Param"));
		return;
	}

	FGsDepotManager* depotManager = GSDepot();
	if (depotManager)
	{
		TWeakPtr<const FGsItem> findData = depotManager->FindDepotItemByDBID(Param->_itemDBID);
		if (false == findData.IsValid())
		{
			GSLOG(Error, TEXT("nullptr == findData, Param->_itemDBID:%lld"), Param->_itemDBID);
			return;
		}

		for (auto& slate : _slateHolder)
		{
			slate.CheckedCostItem(findData.Pin()->GetTID());

			slate.CheckedSpecialNodeCurrency(findData.Pin()->GetTID());
		}
	}
}

void FGsMonsterCollectionManager::OnRemoveToDepot(const IGsMessageParam* InParam)
{
	const FGsRemoveDepotItem* Param = StaticCast<const FGsRemoveDepotItem*>(InParam);
	if (nullptr == Param)
	{
		GSLOG(Error, TEXT("nullptr == Param"));
		return;
	}

	for (auto& slate : _slateHolder)
	{
		slate.CheckedCostItem(Param->_itemTID);

		slate.CheckedSpecialNodeCurrency(Param->_itemTID);
	}
}

void FGsMonsterCollectionManager::OnStateChangedBook(const FGsMonsterKnowledgeBook* InData)
{
	if (nullptr == InData)
	{
		GSLOG(Error, TEXT("nullptr == InData"));
		return;
	}

	if (EMonsterKnowledgeState::LevelUpPossible == InData->State())
	{
		if (false == _redDotBookSet.Contains(InData->Id()))
		{
			_redDotBookSet.Emplace(InData->Id());
		}		
	}
	else
	{
		if (_redDotBookSet.Contains(InData->Id()))
		{
			_redDotBookSet.Remove(InData->Id());
		}
	}	

	const FGsPrimitiveInt32 param(InData->Id());
	SendMessageToWindow(MessageContentMonsterKnowledge::INVALIDATE_BOOK, &param);
}

void FGsMonsterCollectionManager::OnStateChangedSlate(const FGsMonsterKnowledgeSlate* InData)
{
	if (nullptr == InData)
	{
		GSLOG(Error, TEXT("nullptr == InData"));
		return;
	}

	if (EMonsterKnowledgeSlateState::Lock == InData->State() && 0 < InData->HaveCandinateCostAmount())
	{
		if (false == _redDotSlateSet.Contains(InData->Id()))
		{
			_redDotSlateSet.Emplace(InData->Id());
		}
	}
	else
	{
		if (_redDotSlateSet.Contains(InData->Id()))
		{
			_redDotSlateSet.Remove(InData->Id());
		}
	}

	const FGsPrimitiveInt32 param(InData->Id());
	SendMessageToWindow(MessageContentMonsterKnowledge::INVALIDATE_SLATE, &param);
}

void FGsMonsterCollectionManager::OnChangedSpecialNodeCurrency(const FGsMonsterKnowledgeSlate* InSlate, const FGsSlateSpecialNode* InSpecialNode)
{
	if (nullptr == InSlate || nullptr == InSpecialNode)
	{
		return;
	}

	const FGsPrimitiveInt32 param(InSpecialNode->GetId());
	SendMessageToWindow(MessageContentMonsterKnowledge::INAVLIDATE_SLATE_CURRENCY, &param);
}

void FGsMonsterCollectionManager::SendMessageToWindow(MessageContentMonsterKnowledge InMessageContents, const struct IGsMessageParam* InParam)
{
	CheckRedDot();

	const static int32 IS_SEND_MESSAGE_UNLOCK = 0;
	if (IS_SEND_MESSAGE_UNLOCK != _messageHolder)
	{	// 2022/10/11 PKT - Message Hold 상태
		return;
	}
	
	GMessage()->GetMonsterKnowledgeCollection().SendMessage(InMessageContents, InParam);
}

void FGsMonsterCollectionManager::Notify_BookDataSet(const TArray<const MonsterKnowledge*>& InDataSet, bool InIsCompleted)
{
	WINDOW_MESSAGE_LOCK

	for (auto& item : _bookHolder)
	{
		const auto& data = InDataSet.FindByPredicate([item](const MonsterKnowledge* InData)
			{
				return (item.Id() == InData->mId);
			}
		);

		if (data && (*data))
		{
			item.Set((*data)->mLevel, (*data)->mExp);
		}
	}

	WINDOW_MESSAGE_UNLOCK
}

void FGsMonsterCollectionManager::Notify_BookData(uint32 InId, MonsterKnowledgeExp InExp)
{
	for (auto& item : _bookHolder)
	{
		if (item.Id() == InId)
		{
			item.AddExp(InExp);

			FText desc;
			FText::FindText(TEXT("MonsterKnowledgeText"), TEXT("ticker_GetKnowledge"), desc);
			FText ticker = FText::FromString(FString::Format(*desc.ToString(), { item.NPCData()->nameText.ToString() }));
			FGsUIHelper::TraySectionMessageTicker(EGsNotiSection::EXTSYSTEM, ticker);

			break;
		}
	}
}

void FGsMonsterCollectionManager::Ack_BookData(const FGsMonsterCollectionBook& InData)
{
	for (auto& item : _bookHolder)
	{
		if (item.Id() == InData.id)
		{
			bool isLevelUp = (item.Level() < InData.level);
			item.Set(InData.level, InData.exp);
			
			if (isLevelUp)
			{	// 2023/5/26 PKT - 도감 몬스터의 레벨업 시 사운드 1회 출력
				UGsGlobalConstant* globalConstant = GData()->GetGlobalData();
				if (UGsSoundPlayer* soundPlayer = GSoundPlayer())
				{
					soundPlayer->PlaySound2D(globalConstant->_effectSoundMonsterCollectionLevelUp.GetRow());
				}
			}

			break;
		}
	}
}

void FGsMonsterCollectionManager::Ack_BookDataSet(const TArray<FGsMonsterCollectionBook>& InData)
{
	TArray<FGsMonsterCollectionBookResult> bookResultSet;

	bool isEffectSoundPlay = false;

	WINDOW_MESSAGE_LOCK

	for (const auto& item : InData)
	{
		FGsMonsterKnowledgeBook* findData = _bookHolder.FindByPredicate([item](const FGsMonsterKnowledgeBook& InData)
				{
					return InData.Id() == item.id;
				}
			);

		if (findData)
		{
			bool isLevelUp = (findData->Level() < item.level);
			findData->Set(item.level, item.exp);

			isEffectSoundPlay |= isLevelUp;

			FGsMonsterCollectionBookResult bookResult;
			bookResult.id = item.id;
			bookResult.rewardPoint = item.rewardPoint;
			bookResult.isLevelUp = isLevelUp;

			bookResultSet.Emplace(bookResult);
		}
	}
	WINDOW_MESSAGE_UNLOCK

	if (isEffectSoundPlay)
	{	// 2023/5/26 PKT - 도감 몬스터의 레벨업 시 사운드 1회 출력
		UGsGlobalConstant* globalConstant = GData()->GetGlobalData();
		if (UGsSoundPlayer* soundPlayer = GSoundPlayer())
		{
			soundPlayer->PlaySound2D(globalConstant->_effectSoundMonsterCollectionMultiLevelUp.GetRow());
		}
	}

	FGsMessageMonsterCollectionBookResultSet message(&bookResultSet);
	SendMessageToWindow(MessageContentMonsterKnowledge::INVALIDATE_BOOK_SET, &message);
}

void FGsMonsterCollectionManager::Notify_ActiveSlate(const TArray<MonsterKnowledgeSlateId>& InActiveSlateIdSet, const TArray<MonsterKnowledgeSlateSlotId>& InActiveNodeIdSet, const TArray<SlateSpecialNodeId>& InSpecialNodeIdList)
{
	WINDOW_MESSAGE_LOCK

	_passivityCollectionData.Clear(false);

	for (auto& item : _slateHolder)
	{
		if (InActiveSlateIdSet.Contains(item.Id()))
		{
			item.ActiveSlate(true);

			for (auto& special : InSpecialNodeIdList)
			{
				item.ActiveSpecialNode(special);
			}
		}

		item.ActiveNodeSet(InActiveNodeIdSet);

		// 2023/1/31 PKT - Add Passivity
		_passivityCollectionData.Add(item.ActivePassivtySet(), false);

		const int32 maxCount = item.GetSlateSpecialNodeCount();
		for (int32 idx = 0; idx < maxCount; ++idx)
		{
			if (const FGsSlateSpecialNode* specialNodeData = item.FindSpecialNodeByIndex(idx))
			{
				if (EMonsterStoneSlateNodeState::Complete == specialNodeData->GetState())
				{
					_passivityCollectionData.Add(specialNodeData->GetPassivityId(), false);
				}
			}
		}		
	}

	// TODO(ejrrb10) : 특수 노드 관련 작업
	//InSpecialNodeIdList


	WINDOW_MESSAGE_UNLOCK
}

void FGsMonsterCollectionManager::Ack_OpenSlate(MonsterKnowledgeSlateId InSlateId)
{
	int32 findIndex = _slateHolder.IndexOfByPredicate([InSlateId](const FGsMonsterKnowledgeSlate& InData)
		{
			return (InSlateId == InData.Id());
		}
	);

	if (INDEX_NONE >= findIndex)
	{
		GSLOG(Error, TEXT("INDEX_NONE >= findIndex : [%d]"), InSlateId);
		return;
	}

	_slateHolder[findIndex].ActiveSlate(true);

	// 2023/5/26 PKT - 석판 해금 시 사운드 1회 출력
	UGsGlobalConstant* globalConstant = GData()->GetGlobalData();
	if (UGsSoundPlayer* soundPlayer = GSoundPlayer())
	{
		soundPlayer->PlaySound2D(globalConstant->_effectSoundMonsterCollectionSlateUnlock.GetRow());
	}
}

void FGsMonsterCollectionManager::Ack_SlateNodeSet(MonsterKnowledgeSlateId InSlateId, const TArray<MonsterKnowledgeSlateSlotId>& InActiveNodeIdSet, const TArray<SlateSpecialNodeId>& InSpecialNodeIdList)
{
	Ack_SlateNodeSet(InSlateId, InActiveNodeIdSet);

	// TODO(ejrrb10) : 특수 노드 관련 정보 응답 처리
	int32 findIndex = _slateHolder.IndexOfByPredicate([InSlateId](const FGsMonsterKnowledgeSlate& InData)
		{
			return (InSlateId == InData.Id());
		}
	);

	if (INDEX_NONE >= findIndex)
	{
		GSLOG(Error, TEXT("INDEX_NONE >= findIndex : [%d]"), InSlateId);
		return;
	}

	// InSpecialNodeIdList 에는 완료된 노드만 담겨서 온다
	for (auto specialNodeId : InSpecialNodeIdList)
	{
		const FGsSlateSpecialNode* node = _slateHolder[findIndex].FindSpecialNodeByNodeId(specialNodeId);

		_passivityCollectionData.Add(node->GetPassivityId());

		_slateHolder[findIndex].ActiveSpecialNode(specialNodeId);
	}
}

void FGsMonsterCollectionManager::Ack_SlateNodeSet(MonsterKnowledgeSlateId InSlateId, const TArray<MonsterKnowledgeSlateSlotId>& InActiveNodeIdSet)
{
	int32 findIndex = _slateHolder.IndexOfByPredicate([InSlateId](const FGsMonsterKnowledgeSlate& InData)
		{
			return (InSlateId == InData.Id());
		}
	);

	if (INDEX_NONE >= findIndex)
	{
		GSLOG(Error, TEXT("INDEX_NONE >= findIndex : [%d]"), InSlateId);
		return;
	}

	WINDOW_MESSAGE_LOCK

		for (auto item : InActiveNodeIdSet)
		{
			const FGsMonsterKnowledgeSlateNode* node = _slateHolder[findIndex].FindNodeById(item);
			if (EMonsterStoneSlateNodeState::Normal == node->State())
			{
				_passivityCollectionData.Add(node->Passivity());

			}

			_slateHolder[findIndex].ActiveNode(item);
		}

	WINDOW_MESSAGE_UNLOCK

	const FGsMessageMonsterCollectionSlate message(_slateHolder[findIndex].Id());
	SendMessageToWindow(MessageContentMonsterKnowledge::INVALIDATE_SLATE, &message);

	// 2023/5/26 PKT - 노드 해금 시 사운드 1회 출력
	UGsGlobalConstant* globalConstant = GData()->GetGlobalData();
	if (UGsSoundPlayer* soundPlayer = GSoundPlayer())
	{
		soundPlayer->PlaySound2D(globalConstant->_effectSoundMonsterCollectionSlotActive.GetRow());
	}
}

void FGsMonsterCollectionManager::Ack_SlatActiveNodeReset()
{
	WINDOW_MESSAGE_LOCK

	TArray<MonsterKnowledgeSlateSlotId> empty;
	for (auto& item : _slateHolder)
	{
		item.ActiveNodeSet(empty);
	}
	WINDOW_MESSAGE_UNLOCK
	
	_passivityCollectionData.Clear();

	for (auto& item : _slateHolder)
	{
		if (EMonsterKnowledgeSlateState::UnLock == item.State())
		{
			for (int32 idx = 0; idx < item.GetSlateSpecialNodeCount(); ++idx)
			{
				if (const FGsSlateSpecialNode* findSpecialNode = item.FindSpecialNodeByIndex(idx))
				{
					if (EMonsterStoneSlateNodeState::Complete == findSpecialNode->GetState())
					{
						_passivityCollectionData.Add(findSpecialNode->GetPassivityId());
					}
				}
			}			
		}
	}

	FText ticker;
	FText::FindText(TEXT("MonsterKnowledgeText"), TEXT("ticker_SlateResetSuccess"), ticker);
	FGsUIHelper::TraySectionMessageTicker(EGsNotiSection::EXTSYSTEM, ticker);

	const FGsMessageMonsterCollectionSlate message(INVALID_MONSTER_KNOWLEDGE_SLATE_ID);
	SendMessageToWindow(MessageContentMonsterKnowledge::INVALIDATE_SLATE, &message);
}

void FGsMonsterCollectionManager::AckActivateSlateSpecialNode(bool InSuccess, MonsterKnowledgeSlateId InSlateId, SlateSpecialNodeId InSpecialNodeId)
{
	// ejrrb10 | 특수 노드 관련 정보 응답 처리	
	FText textResult;
	if (InSuccess)
	{
		FText::FindText(TEXT("CodexUIText"), TEXT("UI_Slate_Ticker_OpenSuccess"), textResult);
		FGsUIHelper::Play2DSound(TEXT("UI_MB_SpecialNodeTry_Success"));
	}
	else
	{
		FText::FindText(TEXT("CodexUIText"), TEXT("UI_Slate_Ticker_OpenFail"), textResult);
		FGsUIHelper::Play2DSound(TEXT("UI_MB_SpecialNodeTry_Fail"));
	}
	// 2023/11/9 PKT - 실패에 대한 처리.. 티커
	FGsUIHelper::TrayTickerSpiritShotEnchant(textResult, InSuccess);

	// 특수 노드 개방 성공/실패 연출
	{
		const FGsMessageSpecialNodeResult message(InSuccess);
		SendMessageToWindow(MessageContentMonsterKnowledge::SPECIAL_NODE_EFFECT, &message);
	}

	if (InSuccess)
	{
		for (FGsMonsterKnowledgeSlate& item : _slateHolder)
		{
			if (item.Id() == InSlateId)
			{
				item.ActiveSpecialNode(InSpecialNodeId);

				if (const FGsSlateSpecialNode* specialNode = item.FindSpecialNodeByNodeId(InSpecialNodeId))
				{
					if (EMonsterStoneSlateNodeState::Complete == specialNode->GetState())
					{
						_passivityCollectionData.Add(specialNode->GetPassivityId());
					}
				}

				break;
			}
		}

		const FGsMessageMonsterCollectionSlate message(INVALID_MONSTER_KNOWLEDGE_SLATE_ID);
		SendMessageToWindow(MessageContentMonsterKnowledge::INVALIDATE_SLATE, &message);
	}
}

const void* FGsMonsterCollectionManager::BookGradeDetailInfo(MonsterKnowledgeBookGrade InGrade) const
{
	//const UGsTableMonsterKnowledgeGradeDetailInfo* detailInfoTable 
	//	= Cast<UGsTableMonsterKnowledgeGradeDetailInfo>(FGsSchemaMonsterKnowledgeGradeDetailInfo::GetStaticTable());
	//if (nullptr == detailInfoTable)
	//{
	//	GSLOG(Error, TEXT("nullptr == detailInfoTable"));
	//	return nullptr;
	//}

	//TArray<const FGsSchemaMonsterKnowledgeGradeDetailInfo*> detailInfoSet;
	//if (false == detailInfoTable->GetAllRows(detailInfoSet))
	//{
	//	GSLOG(Error, TEXT("false == detailInfoTable->GetAllRows(detailInfoSet)"));
	//	return nullptr;
	//}

	//for (const auto& item : detailInfoSet)
	//{
	//	if (item->grade == InGrade)
	//	{
	//		return item;
	//	}
	//}
	return nullptr;
}

int32 FGsMonsterCollectionManager::BuildBookViewSet(MonsterKnowledgeBookGrade InBookGrade)
{
	TArray<const FGsMonsterKnowledgeBook*> frontEntrySet;
	TArray<const FGsMonsterKnowledgeBook*> rearEntrySet;

	//for (const auto& item : _bookHolder)
	//{
	//	if (MonsterKnowledgeBookGrade::NONE == item.BookGrade())
	//	{
	//		continue;
	//	}

	//	if (MonsterKnowledgeBookGrade::MAX != InBookGrade && InBookGrade != item.BookGrade())
	//	{
	//		continue;
	//	}

	//	if (EMonsterKnowledgeState::LevelUpPossible == item.State())
	//	{
	//		frontEntrySet.Emplace(&item);
	//	}
	//	else
	//	{
	//		rearEntrySet.Emplace(&item);
	//	}
	//}

	//const int32 maxCount = frontEntrySet.Num() + frontEntrySet.Num();
	//_bookViewSet.Empty(maxCount);
	//_bookViewSet.Append(frontEntrySet);
	//_bookViewSet.Append(rearEntrySet);

	return _bookViewSet.Num();
}

const TArray<const FGsMonsterKnowledgeBook*>& FGsMonsterCollectionManager::BookViewSet() const
{
	return _bookViewSet;
}

const FGsMonsterKnowledgeBook* FGsMonsterCollectionManager::BookDataById(uint32 InId) const
{
	return _bookHolder.FindByPredicate([InId](const FGsMonsterKnowledgeBook& InData)
		{
			return InData.Id() == InId;
		}
	);
}

const TArray<FGsCostPackage>& FGsMonsterCollectionManager::BookExpCostSet() const
{
	return _costBookExpItemSet;
}

MonsterKnowledgeExp FGsMonsterCollectionManager::AmountIncreasedUsedExpItem(ItemId InItemId) const
{
	return _amountIncreaseBookExpItemSet.Contains(InItemId) ? _amountIncreaseBookExpItemSet[InItemId] : 0;
}

int32 FGsMonsterCollectionManager::BuildSlateCategory() const
{
	if (0 >= _slateCategoryDataSet.Num())
	{
		do
		{
			const UGsTableMonsterKnowledgeSlateCategory* categoryTable
				= Cast<UGsTableMonsterKnowledgeSlateCategory>(FGsSchemaMonsterKnowledgeSlateCategory::GetStaticTable());
			if (nullptr == categoryTable)
			{
				GSLOG(Error, TEXT("nullptr == categoryTable"));
				break;
			}

			TArray<const FGsSchemaMonsterKnowledgeSlateCategory*> categorySet;
			if (false == categoryTable->GetAllRows(categorySet))
			{
				GSLOG(Error, TEXT("false == categoryTable->GetAllRows(categorySet)"));
				break;
			}

			categorySet.Sort([](const FGsSchemaMonsterKnowledgeSlateCategory& lhr, const FGsSchemaMonsterKnowledgeSlateCategory& rhr)
				{
					return lhr.orderNum < rhr.orderNum;
				}
			);

			for (const FGsSchemaMonsterKnowledgeSlateCategory* item : categorySet)
			{
				if (item->enable)
				{
					_slateCategoryDataSet.Emplace(TPair<EMonsterKnowledgeSlateCategory, FText>(item->type, item->name));
				}
			}
		} while (0);
	}
	return _slateCategoryDataSet.Num();
}

const TPair<EMonsterKnowledgeSlateCategory, FText>* FGsMonsterCollectionManager::SlateCategoryData(int32 InIndex) const
{
	return _slateCategoryDataSet.IsValidIndex(InIndex) ? &(_slateCategoryDataSet[InIndex]) : nullptr;
}

int32 FGsMonsterCollectionManager::BuildSlateViewSet(EMonsterKnowledgeSlateCategory InCategory, const TArray<StatType>& InFilterStatSet)
{
	_slateViewSet.Empty();
	for (const auto& item : _slateHolder)
	{
		if (EMonsterKnowledgeSlateCategory::MAX != InCategory && InCategory != item.CategoryType())
		{
			continue;
		}

		bool isSkip = false;

		if (InFilterStatSet.Num())
		{
			isSkip = true;
			
			for (const auto& filter : InFilterStatSet)
			{
				if (item.IsInclusionStat(filter))
				{
					isSkip = false;
					break;
				}
			}
		}		

		if (false == isSkip)
		{
			_slateViewSet.Emplace(&item);
		}
	}

	return _slateViewSet.Num();
}

const TArray<const FGsMonsterKnowledgeSlate*>& FGsMonsterCollectionManager::SlateViewSet() const
{
	return _slateViewSet;
}

int32 FGsMonsterCollectionManager::SlateMaxCount() const
{
	return _slateHolder.Num();
}

const FGsMonsterKnowledgeSlate* FGsMonsterCollectionManager::SlateDataAt(int32 InIndex) const
{
	return _slateHolder.IsValidIndex(InIndex) ? &(_slateHolder[InIndex]) : nullptr;
}

bool FGsMonsterCollectionManager::IsHaveCostSlateNodeAllReset() const
{
	return _costResetItem.PlayerHasEnough();
}

ItemId FGsMonsterCollectionManager::ResetCostId() const
{
	ItemId outItemId = INVALID_ITEM_ID;
	const FGsCostElement* element = _costResetItem.GetFirstCostElement();
	if (element)
	{
		outItemId = element->GetItemId();
	}
	return outItemId;
}

const FGsPassivityCollectionData* FGsMonsterCollectionManager::PassivityCollectionData() const
{
	return &_passivityCollectionData;
}

TSet<StatType>& FGsMonsterCollectionManager::ContentsStatAll() const
{
	if (0 >= _contentsStatAllSet.Num())
	{	// 2023/2/10 PKT - Slate
		for (const auto& item : _slateHolder)
		{	// 2023/2/10 PKT - Node
			int32 maxCount = item.NodeMaxCount();
			for (int32 idx = 0; idx < maxCount; ++idx)
			{	
				const auto& nodeData = item.NodeAt(idx);
				if (nodeData)
				{	// 2023/2/10 PKT - Stat Set
					const auto& statDataSet = nodeData->StatSet();
					for (const auto& statData : statDataSet)
					{
						_contentsStatAllSet.Emplace(statData.Key);
					}					
				}
			}
		}
	}

	return _contentsStatAllSet;
}

void FGsMonsterCollectionManager::BuildCostItem(
	FGsCostPackage& OutResetItem 
	, TArray<FGsCostPackage>& OutBookExpItemSet 
	, TMap<ItemId, MonsterKnowledgeExp>& OutAmountIncreaseBookExpItemSet
	, TMap<MonsterKnowledgeSlateId, TSet<ItemId>>& OutSlateOpenItemSet) const
{
	OutBookExpItemSet.Empty();
	OutSlateOpenItemSet.Empty();
	OutAmountIncreaseBookExpItemSet.Empty();
	
	const UGsTable* table = FGsSchemaItemCommon::GetStaticTable();

	const TMap<FName, uint8*>* RowMap = nullptr;
	if (nullptr == table || false == table->GetRowMap(RowMap))
	{
		GSLOG(Error, TEXT("nullptr == table || false == table->GetRowMap(RowMap)"));
		return;
	}

	for (auto& RowKvIt : (*RowMap))
	{
		const FGsSchemaItemCommon* schemaItemdata = reinterpret_cast<const FGsSchemaItemCommon*>(RowKvIt.Value);
		if (nullptr == schemaItemdata)
		{
			GSLOG(Error, TEXT("nullptr == schemaItemdata"));
			continue;
		}

		
		if (ItemCategoryMain::MONSTER_KNOWLEDGE == schemaItemdata->categoryMain)
		{	// 2023/1/31 PKT - 몬스터 북 경험치 아이템
			if (ItemCategorySub::MONSTER_KNOWLEDGE_GAIN_EXP == schemaItemdata->categorySub)
			{
				FGsCostPackage cost;
				const static Currency DEFAULT_COST_AMOUNT = 1;
				cost.AddCost(schemaItemdata->id, DEFAULT_COST_AMOUNT);				
				OutBookExpItemSet.Emplace(MoveTemp(cost));

				const UGsTableItemIngredientDetail* detailTable = Cast<UGsTableItemIngredientDetail>(FGsSchemaItemIngredientDetail::GetStaticTable());
				const FGsSchemaItemIngredientDetail* schemaDetailData = nullptr;
				if (nullptr == detailTable || false == detailTable->FindRowById(schemaItemdata->detailId, schemaDetailData))
				{
					GSLOG(Error, TEXT("nullptr == detailTable || false == detailTable->FindRowById(schemaItemdata->detailId, schemaDetailData) : [%d]"), schemaItemdata->detailId);
					continue;
				}

				const UGsTableItemEffectGainMonsterKnowledgeExp* gainExpTable = Cast<UGsTableItemEffectGainMonsterKnowledgeExp>(FGsSchemaItemEffectGainMonsterKnowledgeExp::GetStaticTable());
				if (nullptr == gainExpTable)
				{
					GSLOG(Error, TEXT("nullptr == gainExpTable"));
					continue;
				}

				for (int32 effecID : schemaDetailData->effectIDList)
				{
					const FGsSchemaItemEffectGainMonsterKnowledgeExp* schemaGainExp = nullptr;
					if (false == gainExpTable->FindRowById(effecID, schemaGainExp))
					{
						continue;
					}

					OutAmountIncreaseBookExpItemSet.FindOrAdd(schemaItemdata->id, StaticCast<MonsterKnowledgeExp>(schemaGainExp->gainAmount));
					break;
				}
			}
		}
		else if (ItemCategoryMain::MONSTER_KNOWLEDGE_SLATE == schemaItemdata->categoryMain)
		{// 2023/1/31 PKT - 지식 석판 오픈 아이템
			if (ItemCategorySub::MONSTER_KNOWLEDGE_SLATE_UNLOCK_ATTACK == schemaItemdata->categorySub
				|| ItemCategorySub::MONSTER_KNOWLEDGE_SLATE_UNLOCK_HIT == schemaItemdata->categorySub
				|| ItemCategorySub::MONSTER_KNOWLEDGE_SLATE_UNLOCK_AMPLIFICATION == schemaItemdata->categorySub
				|| ItemCategorySub::MONSTER_KNOWLEDGE_SLATE_UNLOCK_DEFENCE == schemaItemdata->categorySub
				|| ItemCategorySub::MONSTER_KNOWLEDGE_SLATE_UNLOCK_RESISTANCE == schemaItemdata->categorySub
				|| ItemCategorySub::MONSTER_KNOWLEDGE_SLATE_UNLOCK_SURVIVAL == schemaItemdata->categorySub)
			{
				const UGsTableManager& tableManager = UGsTableManager::GetInstance();

				const UGsTableItemIngredientDetail* detailTable
					= Cast<UGsTableItemIngredientDetail>(tableManager.GetTable(TBaseStructure<FGsSchemaItemIngredientDetail>::Get()));
				if (nullptr == detailTable)
				{
					GSLOG(Error, TEXT("nullptr == detailTable"));
					continue;
				}

				const FGsSchemaItemIngredientDetail* schemaDetail = nullptr;
				if (false == detailTable->FindRowById(schemaItemdata->detailId, schemaDetail))
				{
					GSLOG(Error, TEXT("false == tableDetail->FindRowById(InItemData->detailId, schemaDetail) : [%d]"), schemaItemdata->detailId);
					continue;
				}

				const UGsTableItemEffectUnlockMonsterKnowledgeSlate* unlockSlateTable
					= Cast<UGsTableItemEffectUnlockMonsterKnowledgeSlate>(tableManager.GetTable(TBaseStructure<FGsSchemaItemEffectUnlockMonsterKnowledgeSlate>::Get()));

				for (int32 effectId : schemaDetail->effectIDList)
				{
					const FGsSchemaItemEffectUnlockMonsterKnowledgeSlate* schemaUnlockItemData = nullptr;
					if (true == unlockSlateTable->FindRowById(effectId, schemaUnlockItemData))
					{
						const FGsSchemaMonsterKnowledgeSlate* schemaSlateData = schemaUnlockItemData->SlateId.GetRow();
						if (nullptr == schemaSlateData)
						{
							GSLOG(Error, TEXT("nullptr == schemaSlateData [%d]"), effectId);
							continue;
						}

						OutSlateOpenItemSet.FindOrAdd(StaticCast<MonsterKnowledgeSlateId>(schemaSlateData->id)).Emplace(schemaItemdata->id);

						break;
					}
				}
			}
		}
		else if (ItemCategoryMain::ETC == schemaItemdata->categoryMain)
		{// 2023/1/31 PKT - 지식 석판 초기화 아이템
			if (ItemCategorySub::MONSTER_KNOWLEDGE_SLATE_RESET == schemaItemdata->categorySub)
			{
				const static Currency DEFAULT_COST_AMOUNT = 1;
				OutResetItem.AddCost(schemaItemdata->id, DEFAULT_COST_AMOUNT);
			}
		}
	}
}

void FGsMonsterCollectionManager::BuildBookSet(TArray<FGsMonsterKnowledgeBook>& OutBookHolder) const
{
	// ejrrb10 | 칼럼 삭제 요청으로 주석 처리
	//OutBookHolder.Empty();

	//const UGsTable* table = FGsSchemaNpcData::GetStaticTable();
	//
	//const TMap<FName, uint8*>* RowMap = nullptr;
	//if (nullptr == table || false == table->GetRowMap(RowMap))
	//{
	//	GSLOG(Error, TEXT("nullptr == table || false == table->GetRowMap(RowMap)"));
	//	return;
	//}

	//for (auto& RowKvIt : (*RowMap))
	//{
	//	const FGsSchemaNpcData* schemaNPCData = reinterpret_cast<const FGsSchemaNpcData*>(RowKvIt.Value);
	//	if (nullptr == schemaNPCData)
	//	{
	//		GSLOG(Error, TEXT("nullptr == schemaNPCData"));
	//		continue;
	//	}

	//	// 2021/12/03 PKT - 몬스터 도감에서 기획적으로 빠져야 할 몬스터는 제외
	//	if (MonsterKnowledgeBookGrade::NONE == schemaNPCData->monsterKnowledgeBookGrade)
	//	{
	//		continue;
	//	}

	//	// 2022/03/22 PKT - NPCId는 다르지만 같은 몬스터로 취급해야 하는 경우가 있다. 대체 ID가 있다면 수집하지 않겠다. 원본(?) 몬스터가 있으므로..
	//	if (schemaNPCData->id != schemaNPCData->monsterKnowledgeId)
	//	{
	//		continue;
	//	}

	//	OutBookHolder.Emplace(FGsMonsterKnowledgeBook(schemaNPCData));
	//}

	/**
	 * 정렬..꽤 많은 정보가 정렬이 되는데.. 부담되면 다른 방법을 알아 봐야 함.
	 */
	//OutBookHolder.Sort([](const FGsMonsterKnowledgeBook& lhr, const FGsMonsterKnowledgeBook& rhr) -> bool
	//	{
	//		return lhr.SortOrder() > rhr.SortOrder();
	//	}
	//);
}

void FGsMonsterCollectionManager::BuildSlate(const TMap<MonsterKnowledgeSlateId, TSet<ItemId>>& InSlateOpenItemSet , TArray<FGsMonsterKnowledgeSlate>& OutSlateHolder) const
{
	OutSlateHolder.Empty();

	const UGsTable* table = FGsSchemaMonsterKnowledgeSlate::GetStaticTable();

	const TMap<FName, uint8*>* RowMap = nullptr;
	if (nullptr == table || false == table->GetRowMap(RowMap))
	{
		GSLOG(Error, TEXT("nullptr == table || false == table->GetRowMap(RowMap)"));
		return;
	}

	for (auto& RowKvIt : (*RowMap))
	{
		const FGsSchemaMonsterKnowledgeSlate* schemaSlateData = reinterpret_cast<const FGsSchemaMonsterKnowledgeSlate*>(RowKvIt.Value);
		if (nullptr == schemaSlateData)
		{
			GSLOG(Error, TEXT("nullptr == schemaSlateData"));
			continue;
		}

		TArray<ItemId> costSet;
		if (InSlateOpenItemSet.Contains(schemaSlateData->id))
		{
			costSet = InSlateOpenItemSet[schemaSlateData->id].Array();
		}

		OutSlateHolder.Emplace(FGsMonsterKnowledgeSlate(schemaSlateData, costSet));
	}

	OutSlateHolder.Sort([](const FGsMonsterKnowledgeSlate& lhr, const FGsMonsterKnowledgeSlate& rhr)
		{
			return lhr.Id() < rhr.Id();
		}
	);
}