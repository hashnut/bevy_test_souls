

#include "GsGuildItemCollectionData.h"

#include "Classes/GsState.h"
#include "Guild/GsGuildHelper.h"

#include "Item/GsItem.h"
#include "Item/GsItemManager.h"
#include "Item/Collection/GsItemCollectionBase.h"

#include "Management/GsMessageHolder.h"
#include "Management/ScopeGame/GsDepotManager.h"
#include "Management/ScopeGame/GsGameObjectManager.h"
#include "Management/ScopeGlobal/GsGuildManager.h"
#include "Management/ScopeGlobal/GsUIManager.h"
#include "Management/ScopeGlobal/GsNetManager.h"

#include "Message/GsMessageGameObject.h"
#include "Message/MessageParam/GsItemContentsMessageParam.h"
#include "Message/MessageParam/GsItemCollectionMessageParam.h"
#include "Message/MessageParam/GsItemMessageParam.h"
#include "Message/MessageParam/GsMessageParam.h"

#include "GameObject/ObjectClass/GsGameObjectBase.h"

#include "Public/GsTableManager.h"
#include "DataSchema/Guild/GsSchemaGuildItemCollection.h"
#include "DataSchema/Guild/GsSchemaGuildConfig.h"

#include "UI/UIContent/Tray/GsUITrayTickerCollection.h"
#include "Net/GsNetSendServiceGuild.h"



FGsGuildItemCollectionData::FGsGuildItemCollectionData() : _guildManager(nullptr), _ItemCollectionBuildSuccess(false)
{

}

// 2021/09/27 PKT - 길드 입장 및 InGame 접속시 길드가입 상태면 호출
void FGsGuildItemCollectionData::Enter(const FGsGuildManager* InGuildManager)
{
	_guildManager = InGuildManager;

	// 2021/09/30 PKT - 현재 등록한 갯수 초기화
	_registedUseCount = INVALID_REGISTER_USE_COUNT;

	// 2021/10/26 PKT - 아이템 컬렉션 리스트 요청에 대한 응답이 왔을 시 상태 변경
	_ItemCollectionBuildSuccess = false;

	// 2021/09/27 PKT - Message Bind
	RegisterMessages();

	// 2021/10/07 PKT - Item Collection Data Schema를 토대로 Collection List를 만든다.( 골격만 만들어 놓고 .. )
	BuildItemCollectionList();

	// 2021/10/29 PKT - InGame 접속 하자 마자 콜렉션 리스트를 요청하면 이때 시점에서는 NetWork가 준비가 안되어 있는 상태이다.
	// 	                입장 하자 요청은 로컬 플레이어가 스폰 되었을 타이밍에 보내고 지금 보내는 것은 InGame내 길드 가입 했을 시 보냄.
	TSharedPtr<FGsNetBase> netBase = (nullptr != GNet()) ? GNet()->GetActive() : nullptr;
	if (netBase.IsValid())
	{
		Request_ItemCollectionSlotList();
	}
}

// 2021/09/27 PKT - Item Collection Reset
void FGsGuildItemCollectionData::ResetList()
{
	ItemCollectionCleanUp();

	// 2021/10/29 PKT - Window UI 에 CleanUp
	GMessage()->GetGuild().SendMessage(MessageContentGuild::ITEM_COLLECTION_CLEANUP, nullptr);

	// 2021/10/29 PKT - InGame 접속 하자 마자 콜렉션 리스트를 요청하면 이때 시점에서는 NetWork가 준비가 안되어 있는 상태이다.
	// 	                입장 하자 요청은 로컬 플레이어가 스폰 되었을 타이밍에 보내고 지금 보내는 것은 InGame내 길드 가입 했을 시 보냄.
	TSharedPtr<FGsNetBase> netBase = (nullptr != GNet()) ? GNet()->GetActive() : nullptr;
	if (netBase.IsValid())
	{
		Request_ItemCollectionSlotList();
	}
}


// 2021/09/27 PKT - 길드 탈퇴
void FGsGuildItemCollectionData::Leave()
{
	ItemCollectionCleanUp();

	// 패시비티
	const FGsPrimitiveInt32 Param(StaticCast<int32>(PassivityOwnerType::GUILD_ITEM_COLLECTION));
	GMessage()->GetGameObject().SendMessage(MessageGameObject::PASSIVITY_UPDATE_COLLECTION, &Param);
	
	_guildManager = nullptr;

	// 2021/09/27 PKT - Message Un Bind
	UnRegisterMessages();
}

const IGsCollectionData* FGsGuildItemCollectionData::GetCollectionData() const
{
	return this;
}

PassivityOwnerType FGsGuildItemCollectionData::GetPassivityOwnerType() const
{
	return PassivityOwnerType::GUILD_ITEM_COLLECTION;
}

int32 FGsGuildItemCollectionData::GetPassivityCount() const
{
	return _activatedPassivityIds.Num();
}

PassivityId FGsGuildItemCollectionData::GetPassivityId(int32 InIndex) const
{
	if (false == _activatedPassivityIds.IsValidIndex(InIndex))
	{
		GSLOG(Warning, TEXT("false == _activatedPassivityIds.IsValidIndex(InIndex)"));
		return INVALID_PASSIVITY_ID;
	}

	return _activatedPassivityIds[InIndex];
}


bool FGsGuildItemCollectionData::CheckRedDot(bool bInUpdate /*= true*/)
{
	bool isRedDot = false;

	do 
	{
		if (false == _ItemCollectionBuildSuccess)
		{
			break;
		}

		if (true == IsRegisterOverCount() || false == _ItemCollectionBuildSuccess)
		{	// 2021/10/10 PKT - 이미 등록 가능한 갯수를 초과 하였다면 레드닷 없음.
			break;
		}

		// 2021/10/08 PKT - 즉시 등록 가능한 아이템이 있을 경우에만 레드닷을 표시 하겠다.
		if (false == _arrangeItemCollectionList.Contains(ESortPriority::ImmediatePossible)
			|| 0 >= _arrangeItemCollectionList[ESortPriority::ImmediatePossible].Num())
		{
			break;
		}

		for (const FGsGuildItemCollectionState* itemcollection : _itemCollectionList)
		{
			if (itemcollection->IsRedDot())
			{
				isRedDot = true;
				break;
			}
		}

	} while (0);

	bool isPrevRedDot = IsRedDot();

	IGsRedDotCheck::SetRedDot(isRedDot);

	if (IsRedDot() != isRedDot)
	{	// 2021/08/13 PKT - RedDot의 변경을 HUD에 알린다.
		GMessage()->GetHudRedDot().SendMessage(MessageContentHUDRedDot::GUILD, false);
	}

	// 2021/10/29 PKT - 길드 메니져에게 레드닷 캐싱을 요청한다.
	const FGsPrimitiveInt32 param(StaticCast<int32>(FGsGuildManager::EGsGuildRedDotType::ItemCollection));
	GMessage()->GetGuild().SendMessage(MessageContentGuild::CHECK_REDDOT, &param);

	return IsRedDot();
}


// 2021/09/27 PKT - 이벤트 메세지 등록
void FGsGuildItemCollectionData::RegisterMessages()
{
	FGsMessageHolder* MessageManager = GMessage();
	if (nullptr == MessageManager)
	{
		GSLOG(Error, TEXT("nullptr == MessageManager"));
		return;
	}

	// 2021/08/02 PKT - inventory Item - New ..
	_listItemActionDelegates.Emplace(MessageManager->GetItem().AddRaw(MessageItem::ADDITEMLIST
		, this
		, &FGsGuildItemCollectionData::OnInventoryAddItem));
	// 2021/08/02 PKT - inventory Item - Update ..
	_listItemActionDelegates.Emplace(MessageManager->GetItem().AddRaw(MessageItem::UPDATEITEM
		, this
		, &FGsGuildItemCollectionData::OnInventoryUpdateItem));
	// 2021/08/02 PKT - inventory Item - Remove ..
	_listItemActionDelegates.Emplace(MessageManager->GetItem().AddRaw(MessageItem::REMOVEITEM
		, this
		, &FGsGuildItemCollectionData::OnInventoryRemoveItem));

	// 2021/08/02 PKT - Depot Item - New ..
	_itemContentsActionTypeDelegates.Emplace(MessageManager->GetItemContents().AddRaw(MessageContentItem::DEPOT_ADD_ITEM
		, this
		, &FGsGuildItemCollectionData::OnDepotAddItem));
	// 2021/08/02 PKT - Depot Item - Update ..
	_itemContentsActionTypeDelegates.Emplace(MessageManager->GetItemContents().AddRaw(MessageContentItem::DEPOT_INVALIDATE
		, this
		, &FGsGuildItemCollectionData::OnDepotUpdateItem));
	// 2021/08/02 PKT - Depot Item - Remove ..
	_itemContentsActionTypeDelegates.Emplace(MessageManager->GetItemContents().AddRaw(MessageContentItem::DEPOT_REMOVE_ITEM
		, this
		, &FGsGuildItemCollectionData::OnDepotRemoveItem));

	// 2021/10/28 PKT - spawn
	_listGameObjectDelegate.Emplace(MessageManager->GetGameObject().AddRaw(MessageGameObject::LOCAL_SPAWN_ME_COMPLETE
		, this
		, &FGsGuildItemCollectionData::OnLocalPlayerSpawnComplete));
}

// 2021/09/27 PKT - 이벤트 메세지 해제
void FGsGuildItemCollectionData::UnRegisterMessages()
{
	FGsMessageHolder* MessageManager = GMessage();
	if (nullptr == MessageManager)
	{
		GSLOG(Error, TEXT("nullptr == MessageManager"));
		return;
	}

	for (const TPair<MessageItem, FDelegateHandle>& ItemDelegate : _listItemActionDelegates)
	{
		MessageManager->GetItem().Remove(ItemDelegate);
	}
	_listItemActionDelegates.Empty();

	for (const TPair<MessageContentItem, FDelegateHandle>& ItemDelegate : _itemContentsActionTypeDelegates)
	{
		MessageManager->GetItemContents().Remove(ItemDelegate);
	}
	_itemContentsActionTypeDelegates.Empty();

	for (const MsgGameObjHandle& ItemDelegate : _listGameObjectDelegate)
	{
		MessageManager->GetGameObject().Remove(ItemDelegate);
	}
	_listGameObjectDelegate.Empty();
}


void FGsGuildItemCollectionData::OnLocalPlayerSpawnComplete(const IGsMessageParam* InParam)
{
	if (nullptr == GSGameObject())
		return;

	UGsGameObjectManager* mgr = GSGameObject();
	UGsGameObjectBase* findLocalPlayer = mgr->FindObject(EGsGameObjectType::LocalPlayer);
	if (nullptr != findLocalPlayer)
	{
		FGsGameObjectData* gameObjectData = findLocalPlayer->GetData();
		if (nullptr == gameObjectData)
		{
			GSLOG(Error, TEXT("nullptr == gameObjectData"));
			return;
		}

		// 2021/10/28 PKT - 리스트를 요청한다.
		if (_guildManager->IsGuild())
		{	// 2021/10/28 PKT - 아이템 컬렉션 리스트를 요청한다.
			Request_ItemCollectionSlotList();
		}
	}
}


// 2021/09/27 PKT - 소유하고 있는 아이템 추가
void FGsGuildItemCollectionData::AddItem(const FGsItem* InItem)
{
	if (nullptr == InItem || INVALID_ITEM_ID == InItem->GetTID())
	{
		GSLOG(Warning, TEXT("nullptr == InItem || INVALID_ITEM_ID == InItem->GetTID()"));
		return;
	}

	if (false == _ItemCollectionBuildSuccess)
	{	// 2021/10/10 PKT - 이미 등록 가능한 갯수를 초과 하였다면 더이상 수집은 의미가 없음.
		return;
	}

	if (false == _searchToItemcollectionsByItemId.Contains(InItem->GetTID()))
	{	// 2021/09/27 PKT - Item Collection 과 관련 없는 아이템
		return;
	}

	ArrayCollectionList_Ptr& AvaildcollectionList = _searchToItemcollectionsByItemId[InItem->GetTID()];
	for ( FGsGuildItemCollectionState* itemCollection : AvaildcollectionList)
	{
		itemCollection->UpdateByNewItem(InItem);
	}
}

// 2021/09/27 PKT - 소유 하고 있는 아이템 변경
void FGsGuildItemCollectionData::UpdateItem(const FGsItem* InItem)
{
	if (nullptr == InItem || INVALID_ITEM_ID == InItem->GetTID())
	{
		GSLOG(Warning, TEXT("nullptr == InItem || INVALID_ITEM_ID == InItem->GetTID()"));
		return;
	}

	if (false == _ItemCollectionBuildSuccess)
	{	// 2021/10/10 PKT - 이미 등록 가능한 갯수를 초과 하였다면 더이상 수집은 의미가 없음.
		return;
	}

	if (false == _searchToItemcollectionsByItemId.Contains(InItem->GetTID()))
	{	// 2021/09/27 PKT - Item Collection 과 관련 없는 아이템
		return;
	}

	ArrayCollectionList_Ptr& AvaildcollectionList = _searchToItemcollectionsByItemId[InItem->GetTID()];
	for (FGsGuildItemCollectionState* itemCollection : AvaildcollectionList)
	{
		itemCollection->UpdateByUpdateItem(InItem);
	}
}

// 2021/09/27 PKT - 소유 하고 있는 아이템 삭제
void FGsGuildItemCollectionData::RemoveItem(const ItemId InTId, const ItemDBId InDBId)
{
	if (INVALID_ITEM_ID == InTId)
	{
		GSLOG(Warning, TEXT("INVALID_ITEM_ID == InItem->GetTID()"));
		return;
	}

	if (false == _ItemCollectionBuildSuccess)
	{	// 2021/10/10 PKT - 이미 등록 가능한 갯수를 초과 하였다면 더이상 수집은 의미가 없음.
		return;
	}

	if (false == _searchToItemcollectionsByItemId.Contains(InTId))
	{	// 2021/09/27 PKT - Item Collection 과 관련 없는 아이템
		return;
	}

	ArrayCollectionList_Ptr& AvaildcollectionList = _searchToItemcollectionsByItemId[InTId];
	for (FGsGuildItemCollectionState* itemCollection : AvaildcollectionList)
	{
		itemCollection->UpdateByRemoveItem(InTId, InDBId);
	}
}

// 2021/09/27 PKT - inventory Item - New ..
void FGsGuildItemCollectionData::OnInventoryAddItem(FGsItemMessageParamBase& InParam)
{
	const FGsAddItemList* Param = StaticCast<const FGsAddItemList*>(&InParam);
	check(Param);

	TWeakObjectPtr<UGsItemManager> itemManager = GItem();
	for (int32 idx = 0; idx < Param->_itemDbidList.Num(); ++idx)
	{
		TWeakPtr<FGsItem> item = itemManager->FindItem(Param->_itemDbidList[idx]);
		if (false == item.IsValid())
		{
			GSLOG(Error, TEXT("nullptr == findData, Param->_itemDBID : %lld"), Param->_itemDbidList[idx]);
			return;
		}

		AddItem(item.Pin().Get());
	}
}

// 2021/09/27 PKT - inventory Item - Update ..
void FGsGuildItemCollectionData::OnInventoryUpdateItem(FGsItemMessageParamBase& InParam)
{
	const FGsUpdateItem* Param = StaticCast<const FGsUpdateItem*>(&InParam);
	check(Param);

	TWeakObjectPtr<UGsItemManager> itemManager = GItem();
	if (itemManager.IsValid())
	{
		TWeakPtr<FGsItem> item = itemManager->FindItem(Param->_itemDBID);
		if (false == item.IsValid())
		{
			GSLOG(Error, TEXT("nullptr == Item, Param->_itemDBID:%lld"), Param->_itemDBID);
			return;
		}
		UpdateItem(item.Pin().Get());
	}
}

// 2021/09/27 PKT - inventory Item - Remove ..
void FGsGuildItemCollectionData::OnInventoryRemoveItem(FGsItemMessageParamBase& InParam)
{
	const FGsRemoveItem* Param = StaticCast<const FGsRemoveItem*>(&InParam);
	check(Param);
	RemoveItem(Param->_itemTID, Param->_itemDBID);
}

// 2021/09/27 PKT - Depot Item - New ..
void FGsGuildItemCollectionData::OnDepotAddItem(const IGsMessageParam* InParam)
{
	const FGsAddDepotItem* Param = StaticCast<const FGsAddDepotItem*>(InParam);
	check(Param);

	FGsDepotManager* depotManager = GSDepot();
	if (nullptr != depotManager)
	{
		TWeakPtr<const FGsItem> item = depotManager->FindDepotItemByDBID(Param->_itemDBID);
		if (false == item.IsValid())
		{
			GSLOG(Error, TEXT("nullptr == Item, Param->_itemDBID:%lld"), Param->_itemDBID);
			return;
		}
		AddItem(item.Pin().Get());
	}
}

// 2021/09/27 PKT - Depot Item - Update ..
void FGsGuildItemCollectionData::OnDepotUpdateItem(const IGsMessageParam* InParam)
{
	const FGsUpdateDepotItem* Param = StaticCast<const FGsUpdateDepotItem*>(InParam);
	check(Param);

	FGsDepotManager* depotManager = GSDepot();
	if (nullptr != depotManager)
	{
		TWeakPtr<FGsItem> item = depotManager->FindDepotItemByDBID(Param->_itemDBID);
		if (false == item.IsValid())
		{
			GSLOG(Error, TEXT("nullptr == Item, Param->_itemDBID:%lld"), Param->_itemDBID);
			return;
		}
		UpdateItem(item.Pin().Get());
	}
}

// 2021/09/27 PKT - Depot Item - Remove ..
void FGsGuildItemCollectionData::OnDepotRemoveItem(const IGsMessageParam* InParam)
{
	const FGsRemoveDepotItem* Param = StaticCast<const FGsRemoveDepotItem*>(InParam);
	check(Param);
	RemoveItem(Param->_itemTID, Param->_itemDBID);
}


void FGsGuildItemCollectionData::Request_ItemCollectionSlotList() const
{
	// 2021/10/05 PKT - 아이템 콜렉션 리스트 요청
	FGsNetSendServiceGuild::SendRItemCollectionSlotList();
}


void FGsGuildItemCollectionData::Request_ItemCollection_Register(const CollectionConditionId InCollectionConditionId, const ItemDBId InItemId) const
{
	// 2021/10/05 PKT - 아이템 콜렉션 등록 요청
	FGsNetSendServiceGuild::SendRItemCollectionRegister(InCollectionConditionId, InItemId);
}


void FGsGuildItemCollectionData::Request_ItemCollection_BookMark(const CollectionId InCollectionId, bool InActive) const
{
	// 2021/10/05 PKT - BookMark 상태 변경 요청
	FGsNetSendServiceGuild::SendItemCollectionBookMark(InCollectionId, InActive);
}


void FGsGuildItemCollectionData::Ack_CompleteItemCollectionCondition(const CollectionConditionId InCollectionConditionId
																	, const FText& InRegisterName
																	, const int32 InRegisterCount /* = INVALID_REGISTER_USE_COUNT */)
{
	if (false == _ItemCollectionBuildSuccess)
	{	// 2021/10/26 PKT - Item Collection List가 만들어 지지 않았다면 Ack를 받지 않겠다.
		return;
	}

	// 2021/10/05 PKT - 아이템 콜렉션 완료 상태 변경
	if (_searchToItemcollectionByCollectionConditionId.Contains(InCollectionConditionId))
	{
		FGsGuildItemCollectionState* itemCollectionState = _searchToItemcollectionByCollectionConditionId[InCollectionConditionId];
		TPair<CollectionConditionId, FText> pair;
		pair.Key = InCollectionConditionId;
		pair.Value = InRegisterName;
		itemCollectionState->RegisterCollection(pair);

		//	// 2021/10/26 PKT - 등록가능한 횟수 갱신
		if (INVALID_REGISTER_USE_COUNT != InRegisterCount)
		{	// 2021/10/26 PKT - 본인이 등록한 경우에는 갱신 카운트가 온다.
			_registedUseCount = InRegisterCount;

			if (IsRegisterOverCount())
			{
				for (FGsGuildItemCollectionState* state : _itemCollectionList)
				{
					state->SetUnableToRegister(true);
				}
			}
			
		}

		SendMessageToWindow(ECollectionStateChangeType::ECollectionSlot, { itemCollectionState->GetCollectionId() });

		if (EItemCollectionState::Completed == itemCollectionState->GetType())
		{
			ComplectedEffectTicker(itemCollectionState->GetCollectionId());

			// 2021/10/09 PKT - Item Collection Stat Status 갱신 요청			
			GMessage()->GetGuild().SendMessage(MessageContentGuild::ITEM_COLLECTION_INVAILDATE_STAT_STATUS, nullptr);
		}		
	}
}


void FGsGuildItemCollectionData::Ack_ItemCollectionSlotListAll(const TArray<GuildItemCollectionInfo>& InBuildItemCollectionList 
															, const TArray<GuildCollectionRegisterData>& InRegisterCollectionConditionList
															, const TArray<CollectionId>& bookMarkCollectionList 
															, const int32 InRegisterCount)
{
	// 2021/10/10 PKT - 초기화 진행
	ItemCollectionSlotCleanUp();
	
	// 2021/10/05 PKT - 길드 아이템 콜렉션 정보 만들기
	BuildItemCollectionConditionList(InBuildItemCollectionList);

	// 2021/10/26 PKT - Item Collection 등록 상태 변경
	for (const GuildCollectionRegisterData& item : InRegisterCollectionConditionList)
	{
		if (_searchToItemcollectionByCollectionConditionId.Contains(item._collectionConditionId))
		{
			TPair<CollectionConditionId, FText> pair;
			pair.Key = item._collectionConditionId;
			pair.Value = item._registerUserName;
				
			_searchToItemcollectionByCollectionConditionId[item._collectionConditionId]->RegisterCollection(pair);
		}
	}

	// 2021/10/26 PKT - Item Collection 북마크 상태 변경
	for (const CollectionId collectionId : bookMarkCollectionList)
	{
		if (_itemCollectionHolder.Contains(collectionId))
		{
			_itemCollectionHolder[collectionId].UpdateByBookMarkFlag(true);
		}
	}

	_registedUseCount = InRegisterCount;
	bool isOverCount = IsRegisterOverCount();
	for (FGsGuildItemCollectionState* state : _itemCollectionList)
	{
		state->SetUnableToRegister(isOverCount);
	}

	// 2021/10/07 PKT - Slot 정보가 만들어 졌다면.. Inventory / Depot 을 뒤져서 관련 아이템에 따른 상태를 업데이트 시킨다.
	UGsItemManager* itemManager = GItem();
	if (nullptr != itemManager)
	{
		for (int32 idx = 0; idx < StaticCast<int32>(ItemType::MAX); ++idx)
		{
			TArray<TWeakPtr<FGsItem>> findItems = itemManager->FindItem(StaticCast<ItemType>(idx));
			for (TWeakPtr<FGsItem> item : findItems)
			{
				if(false == item.IsValid())
					continue;

				AddItem(item.Pin().Get());
			}
		}
	}

	FGsDepotManager* depotManager = GSDepot();
	if (nullptr != depotManager)
	{
		for (const auto& itemBuffer : depotManager->GetItemBuffers())
		{
			if (0 < itemBuffer.Value.Num())
			{
				for (const auto& pair : itemBuffer.Value)
				{
					AddItem(pair.Value.Get());
				}
			}
		}
	}

	// 2021/10/29 PKT - 초기화 이후 요청에 의해 리스트가 다시 만들어 진것을 대비해 Window에 메세지로 통보 한다.
	GMessage()->GetGuild().SendMessage(MessageContentGuild::ITEM_COLLECTION_INVAILDATE_ALL, nullptr);
}


void FGsGuildItemCollectionData::Ack_ItemCollectionBookMark(const CollectionId InCollectionId, bool InFlag)
{
	if (false == _ItemCollectionBuildSuccess)
	{	// 2021/10/26 PKT - Item Collection List가 만들어 지지 않았다면 Ack를 받지 않겠다.
		return;
	}

	// 2021/10/05 PKT - 아이템 콜렉션 북마크 상태 변경
	if (true == _itemCollectionHolder.Contains(InCollectionId))
	{	// 2021/10/05 PKT - 북마크 상태 업데이트
		_itemCollectionHolder[InCollectionId].UpdateByBookMarkFlag(InFlag);
	}
}


void FGsGuildItemCollectionData::ItemCollectionCleanUp()
{
	ItemCollectionSlotCleanUp();

	_itemCollectionHolder.Empty();
	_itemCollectionList.Empty();	
	_itemCollectionViewList.Empty();	
}


void FGsGuildItemCollectionData::ItemCollectionSlotCleanUp()
{
	_arrangeItemCollectionList.Empty();
	_activatedPassivityIds.Empty();	
	_IsSortDirty.Empty();
	_searchToItemcollectionsByItemId.Empty();
	_searchToItemcollectionByCollectionConditionId.Empty();

	_registedUseCount = INVALID_REGISTER_USE_COUNT;

	for (auto& pair : _itemCollectionHolder)
	{	// 2021/10/07 PKT - 내부 슬롯 전체 지우기
		pair.Value.SlotRemoveAll();
	}

	_ItemCollectionBuildSuccess = false;
}


void FGsGuildItemCollectionData::BuildItemCollectionList()
{
	ItemCollectionCleanUp();

	const UGsTableManager& tableManager = UGsTableManager::GetInstance();

	// 2021/10/06 PKT - Item Collection Table
	const UGsTableGuildItemCollection* collectionTable 
		= Cast<UGsTableGuildItemCollection>(tableManager.GetTable(TBaseStructure<FGsSchemaGuildItemCollection>::Get()));

	if (nullptr == collectionTable)
	{
		GSLOG(Error, TEXT("nullptr == collectionTable"));
		return;
	}

	const TMap<FName, uint8*>* RowMap = nullptr;
	if (false == collectionTable->GetRowMap(RowMap))
	{
		GSLOG(Error, TEXT("false = collectionTable->GetRowMap(RowMap)"));
		return;
	}

	// 2021/10/07 PKT - Size 를 미리 잡아 둔다.
	_itemCollectionHolder.Reserve(RowMap->Num());
	_itemCollectionList.Reserve(RowMap->Num());	

	for (auto& RowKvIt : *RowMap)
	{
		const FString RowName = RowKvIt.Key.ToString();
		if (false == RowName.IsNumeric())
		{
			GSLOG(Warning, TEXT("!RowName.IsNumeric(), RowName:%s"), *RowName);
			continue;
		}
		
		const CollectionId itemCollectionId = FCString::Atoi(*RowName);
		FGsGuildItemCollectionState* itemCollectionStatePtr
			= &(_itemCollectionHolder.Emplace(itemCollectionId, reinterpret_cast<const FGsSchemaGuildItemCollection*>(RowKvIt.Value)));

		// 2021/10/06 PKT - 검색용.. 
		_itemCollectionList.Emplace(itemCollectionStatePtr);

		// 2021/10/06 PKT - Delegate Func
		itemCollectionStatePtr->OnCollectionStateChange.BindRaw(this, &FGsGuildItemCollectionData::ItemCollectionStateChangemCollector);
		itemCollectionStatePtr->OnCollectionBookMarkToggle.BindRaw(this, &FGsGuildItemCollectionData::OnItemCollectionBookMarkToggle);
	}
}


void FGsGuildItemCollectionData::BuildItemCollectionConditionList(const TArray<GuildItemCollectionInfo>& InBuildItemCollectionList)
{
	for (const GuildItemCollectionInfo& info : InBuildItemCollectionList)
	{
		if (false == _itemCollectionHolder.Contains(info.mCollectionId))
		{
			GSLOG(Error, TEXT("false == _itemCollectionHolder.Contains(info.mCollectionId)"));
			continue;
		}

		FGsGuildItemCollectionState& itemCollection = _itemCollectionHolder[info.mCollectionId];
		
		TArray<GuildCollectionCondition> makeSlotParam;
		for (const GuildCollectionCondition& condition : info.mGuildCollectionCondition)
		{
			if (INVALID_COLLECTION_CONDITION_ID == condition.mCollectionConditionId
				|| INVALID_COLLECTION_RANDOM_DATA_ID == condition.mCollectionRandomDataId)
			{
				continue;
			}
			makeSlotParam.Emplace(condition);
		}
		itemCollection.CreateSlotList(makeSlotParam);

		int32 maxCount = itemCollection.GetCollectionItemsCount();
		for (int32 idx = 0; idx < maxCount; ++idx)
		{
			const FGsItemCollectionSlotState* itemCollectionSlotState = itemCollection.FindInnerSlotByIndex(idx);
			if (nullptr == itemCollectionSlotState)
			{
				continue;
			}

			const FGsItemCollectionSlotState::MsterialSet& materialSet = itemCollectionSlotState->GetMaterialDataSet();
			for ( const FGsItemCollectionSlotState::MaterialData& materialData : materialSet )
			{	// 2021/10/28 PKT - 관련 아이템 아이디로 컬렉션을 찾는다.
				_searchToItemcollectionsByItemId.FindOrAdd(materialData.Key).Emplace(&itemCollection);
			}

			// 2021/10/28 PKT - 컬렉션 컨디션 아이디로 컬렉션을 찾는다..
			_searchToItemcollectionByCollectionConditionId.FindOrAdd(itemCollectionSlotState->GetItemCollectionConditionId()) = &itemCollection;
		}

		// 2021/10/28 PKT - Item Collection 정렬 상태 정보
		ESortPriority SortPriority = (true == itemCollection.IsActive()) ? ESortPriority::Idel : ESortPriority::InActive;
		_arrangeItemCollectionList.FindOrAdd(SortPriority).AddUnique(&itemCollection);
		if (false == _IsSortDirty.Contains(SortPriority))
		{
			// 2021/10/06 PKT - Default타입 정렬 필요.
			_IsSortDirty.Emplace(SortPriority);
		}
	}

	// 2021/10/08 PKT - Item Collectinon Build 완료
	_ItemCollectionBuildSuccess = true;
}


ESortPriority FGsGuildItemCollectionData::GetSortPriorityType(EItemCollectionState InState, EItemCollectionSlotState InGlobalSlotState)
{
	ESortPriority outValue = ESortPriority::Idel;

	switch (InGlobalSlotState)
	{
	case EItemCollectionSlotState::ImmediatePossible:
		outValue = ESortPriority::ImmediatePossible;
		break;
	case EItemCollectionSlotState::ConditionalPossible:
		outValue = ESortPriority::ConditionalPossible;
		break;
	case EItemCollectionSlotState::Impossible:
	{
		if (EItemCollectionState::Progressing == InState)
		{
			outValue = ESortPriority::Progressing;
		}
		else if (EItemCollectionState::Opened == InState)
		{
			outValue = ESortPriority::Idel;
		}
	}break;
	case EItemCollectionSlotState::Completed:
		outValue = ESortPriority::Completed;
		break;
	default:
		break;
	};

	return outValue;
}

void FGsGuildItemCollectionData::ItemCollectionStateChangemCollector(ECollectionStateChangeType InStateChangeType
																	, const FGsItemCollectionBase* InItemCollectionBase
																	, EItemCollectionState InPrevItemCollectionState
																	, EItemCollectionSlotState InPrevItemCollectionSlotState)
{
	const FGsGuildItemCollectionState* itemCollection = StaticCast<const FGsGuildItemCollectionState*>(InItemCollectionBase);
	
	if (ECollectionStateChangeType::EBookMark == InStateChangeType)
	{	// 2021/10/14 PKT - BookMark는 정렬과 상관이 없음(Window 상에서 BookMark Tab 이라면 Window에서 갱신을 요청하는 구조...)
		/**
		 * None
		 */
	}
	else
	{
		// 2021/09/27 PKT - InnerSlot 상태 변경
		ArrangeItemCollectionByItemCollectionState(InStateChangeType
													, itemCollection
													, InPrevItemCollectionState
													, InPrevItemCollectionSlotState);
	}

	CheckRedDot();

	SendMessageToWindow(InStateChangeType, { itemCollection->GetCollectionId() });
}


void FGsGuildItemCollectionData::ArrangeItemCollectionByItemCollectionState(ECollectionStateChangeType InType 
																		, const FGsGuildItemCollectionState* InItemCollection
																		, EItemCollectionState InPrevItemCollectionState 
																		, EItemCollectionSlotState InPrevItemCollectionSlotState)
{

	if (false == InItemCollection->IsActive())
	{	// 2021/10/14 PKT - 현재 Collection 상태가 비활성화 라면 다른 계산 필요 없음. 무조건 비활성화에 넣어 준다.
		_arrangeItemCollectionList.FindOrAdd(ESortPriority::InActive).AddUnique(InItemCollection);
	}
	else
	{	// 2021/10/14 PKT - 활성화 상태라면..

		if (ECollectionStateChangeType::EActive == InType && true == _arrangeItemCollectionList.Contains(ESortPriority::InActive))
		{	// 2021/10/14 PKT - Collection 상태가 활성화 로 바뀌었다면 Map에서 제거..
			_arrangeItemCollectionList[ESortPriority::InActive].Remove(InItemCollection);
		}
		else if (ECollectionStateChangeType::ECollectionSlot == InType)
		{
			// 2021/10/14 PKT - 이전 상태를 구해서 Map에서 제거
			ESortPriority prvSortPriority = GetSortPriorityType(InPrevItemCollectionState, InPrevItemCollectionSlotState);
			if (true == _arrangeItemCollectionList.Contains(prvSortPriority))
			{
				_arrangeItemCollectionList[prvSortPriority].Remove(InItemCollection);
			}
		}

		// 2021/10/14 PKT - 현재 상태를 구한다.
		ESortPriority currentSortPriority = GetSortPriorityType(InItemCollection->GetType(), InItemCollection->GetGlobalSlotState());
		_arrangeItemCollectionList.FindOrAdd(currentSortPriority).AddUnique(InItemCollection);
		// 2021/10/14 PKT - 삭제 한것은 무시.. 추가 한 것에 대한 정렬을 한다.
		if (false == _IsSortDirty.Contains(currentSortPriority))
		{
			_IsSortDirty.Emplace(currentSortPriority);
		}
	}

	if (EItemCollectionState::Completed == InItemCollection->GetType())
	{
		for (PassivityId id : InItemCollection->GetPassivityId())
		{
			_activatedPassivityIds.AddUnique(id);
		}		

		// 패시비티
		const FGsPrimitiveInt32 Param(StaticCast<int32>(PassivityOwnerType::GUILD_ITEM_COLLECTION));
		GMessage()->GetGameObject().SendMessage(MessageGameObject::PASSIVITY_UPDATE_COLLECTION, &Param);
	}
}


void FGsGuildItemCollectionData::ComplectedEffectTicker(const CollectionId InCollectionId) const
{
	if (false == _itemCollectionHolder.Contains(InCollectionId))
	{
		GSLOG(Warning, TEXT("not find ItemCollectionState By CollectionId [%d]"), InCollectionId);
		return;
	}

	const FGsGuildItemCollectionState* itemCollection = &(_itemCollectionHolder[InCollectionId]);
	if (nullptr == itemCollection)
	{
		GSLOG(Error, TEXT("nullptr == itemCollection"));
		return;
	}

	UGsUIManager* uiManager = GUI();
	if (nullptr == uiManager)
	{
		GSLOG(Error, TEXT("nullptr == uiManager"));
		return;
	}

	UGsUITrayTickerCollection* ticker = Cast<UGsUITrayTickerCollection>(uiManager->OpenAndGetWidget(TEXT("TrayTickerCollection")).Get());
	if (nullptr == ticker)
	{
		GSLOG(Error, TEXT("nullptr == Ticker"));
		return;
	}

	const UGsUITrayTickerCollection::FInitData InitData 
	{
		itemCollection->GetName(), itemCollection->GetStatEffectText() 
	};
	
	ticker->SetData(InitData);
}

void FGsGuildItemCollectionData::OnItemCollectionBookMarkToggle(const FGsItemCollectionBase* InItemCollection)
{
	if (nullptr == InItemCollection)
	{
		GSLOG(Error, TEXT("nullptr == InItemCollection"));
		return;
	}

	// 2021/10/27 PKT - 서버에 갱신을 요청
	bool bToggle = (false == InItemCollection->IsOnBookMark());
	Request_ItemCollection_BookMark(InItemCollection->GetCollectionId(), bToggle);
}

void FGsGuildItemCollectionData::SendMessageToWindow(ECollectionStateChangeType InType, TArray<CollectionId> InItemCollectionId)
{
	if (false == _ItemCollectionBuildSuccess)
	{
		return;
	}

	/*bool IsInvalidInventory = (ECollectionStateChangeType::ECollectionSlot == InType);
	const FGsItemCollectionMessageParam param(InType, { InItemCollectionId }, IsInvalidInventory);
	GMessage()->GetGuild().SendMessage(MessageContentGuild::ITEM_COLLECTION_UPDATE_LIST, &param);*/
}


void FGsGuildItemCollectionData::ReadjustItemCollectionViewList(const ECategoryValue InCategoryValue
																, OUT ArrayCollectionList_CPtr& InItemCollectionList) const
{
	InItemCollectionList.Empty();

	auto CommonSortFunc = [](const FGsGuildItemCollectionState& lhr, const FGsGuildItemCollectionState& rhr) -> bool
	{
		if (lhr.GetCollectionType() < rhr.GetCollectionType())
		{
			return true;
		}
		else if (lhr.GetCollectionType() > rhr.GetCollectionType())
		{
			return false;
		}

		return (lhr.GetCollectionId() < rhr.GetCollectionId());
	};

	auto SortAfterCheckFunc = [&](ESortPriority InSortPriority) -> void
	{
		if (true == _IsSortDirty.Contains(InSortPriority))
		{
			_arrangeItemCollectionList.FindOrAdd(InSortPriority).Sort(CommonSortFunc);
			_IsSortDirty.Remove(InSortPriority);
		}
		if (_arrangeItemCollectionList.Contains(InSortPriority))
		{
			InItemCollectionList.Append(_arrangeItemCollectionList[InSortPriority]);
		}		
	};
	
	if (true == EnumHasAnyFlags(InCategoryValue, ECategoryValue::Incomplete) || InCategoryValue == ECategoryValue::BookMark)
	{	// 2021/10/07 PKT - 미진행 상태의 Flag
		SortAfterCheckFunc(ESortPriority::ImmediatePossible);
		SortAfterCheckFunc(ESortPriority::ConditionalPossible);
		SortAfterCheckFunc(ESortPriority::Progressing);
		SortAfterCheckFunc(ESortPriority::Idel);
	}

	if (true == EnumHasAnyFlags(InCategoryValue, ECategoryValue::Complete) || InCategoryValue == ECategoryValue::BookMark)
	{	// 2021/10/07 PKT - 완료 상태의 Flag
		SortAfterCheckFunc(ESortPriority::Completed);
	}

	// 2021/09/28 PKT - 정렬이 끝났으면 Dirty 초기화
	_IsSortDirty.Empty();
}


bool FGsGuildItemCollectionData::IsItemAvailableForItemCollection(const FGsItem* InItem) const
{
	if (nullptr == InItem || true == InItem->GetLock() || true == InItem->IsEquipped() || InItem->IsPresetEquipped())
	{	// 2021/09/27 PKT - 아이템 정보가 없거나, 잠김 or 장착 중인 상태는 패스..
		return false;
	}

	if (false == _searchToItemcollectionsByItemId.Contains(InItem->GetTID()))
	{	// 2021/09/27 PKT - 등록가능한 아이템이 아니다.
		return false;
	}

	const ArrayCollectionList_Ptr& collectionList = _searchToItemcollectionsByItemId[InItem->GetTID()];
	for (const FGsGuildItemCollectionState* itemCollection : collectionList)
	{
		if (nullptr == itemCollection)
		{
			GSLOG(Warning, TEXT("nullptr == itemCollection"));
			continue;
		}

		if (true == itemCollection->IsItemAvailableItem(InItem))
		{	// 2021/09/27 PKT - Item Collection 에서 검사.
			return true;
		}
	}

	return false;
}


int32 FGsGuildItemCollectionData::BuildItemCollectionViewList(const ECategoryValue InCategoryValue, const TArray<TSharedPtr<GuildItemCollectionFilter>>& InFilters) const
{
	// 2021/09/27 PKT - 수집된 Data를 View 정렬에 맞게 재조정
	_itemCollectionViewList.Empty();
	ReadjustItemCollectionViewList(InCategoryValue, _itemCollectionViewList);

	// 2021/09/27 PKT - Filter
	TArray<const FGsGuildItemCollectionState*> bufferList;
	for (TSharedPtr<GuildItemCollectionFilter> filter : InFilters)
	{
		bufferList.Empty(_itemCollectionViewList.Num());
		filter->Execute(_itemCollectionViewList, bufferList);
		// 2021/09/27 PKT - 결과 Swap
		_itemCollectionViewList = MoveTemp(bufferList);
	}

	return _itemCollectionViewList.Num();
}


const FGsGuildItemCollectionData::ArrayCollectionList_CPtr& FGsGuildItemCollectionData::ItemCollectionViewList() const
{
	return _itemCollectionViewList;
}


const FGsGuildItemCollectionState* FGsGuildItemCollectionData::FindItemCollectionViewListByCollectionId(CollectionId InId) const
{
	return *(_itemCollectionViewList.FindByPredicate([InId](const FGsGuildItemCollectionState* InItemCollection)
		{
			return (InId == InItemCollection->GetCollectionId());
		}
	));
}


const FGsGuildItemCollectionState* FGsGuildItemCollectionData::FindItemCollectionByCollectionConditionId(CollectionConditionId InCollectionConditionId) const
{
	if (_searchToItemcollectionByCollectionConditionId.Contains(InCollectionConditionId))
	{
		return _searchToItemcollectionByCollectionConditionId[InCollectionConditionId];
	}	
	return nullptr;
}



void FGsGuildItemCollectionData::GetRegisterUsedCount(OUT int32* InUseCount, OUT int32* InMaxCount) const
{
	// 2021/09/29 PKT - 현재 등록한 아이템 갯수
	(*InUseCount) = _registedUseCount;
	// 2021/09/29 PKT - 최대 등록 가능한 아이템 갯수
	const FGsSchemaGuildConfig* guildConfig = FGsGuildHelper::GetGuildConfig();
	if (nullptr == guildConfig)
	{
		(*InMaxCount) = INVALID_REGISTER_USE_COUNT;
		GSLOG(Warning, TEXT("nullptr == guildConfig"));
	}
	else
	{
		(*InMaxCount) = guildConfig->itemCollectionRegisterUseLimitCount;
	}	
}


bool FGsGuildItemCollectionData::IsRegisterOverCount() const
{
	const FGsSchemaGuildConfig* guildConfig = FGsGuildHelper::GetGuildConfig();
	if (nullptr == guildConfig)
	{		
		GSLOG(Warning, TEXT("nullptr == guildConfig"));
		return false;
	}

	return (_registedUseCount >= guildConfig->itemCollectionRegisterUseLimitCount);
}