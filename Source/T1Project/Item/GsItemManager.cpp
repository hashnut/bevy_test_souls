// Fill out your copyright notice in the Description page of Project Settings.


#include "GsItemManager.h"
#include "T1Project.h"

#include "Misc/DateTime.h"

#include "GsItem.h"
#include "GsItemConsumable.h"
#include "GsItemEquipment.h"
#include "GsItemIngredient.h"

#include "GameObject/ObjectClass/GsGameObjectLocalPlayer.h"
#include "GameObject/Data/GsGameObjectDataCreature.h"
#include "GameObject/Data/GsGameObjectDataLocalPlayer.h"
#include "GameObject/Parts/GsPartsHandlerPlayer.h"
#include "GameObject/Abnormality/GsAbnormalityHandlerBase.h"
#include "GameObject/Abnormality/GsPassivityHelper.h"
#include "GameObject/UseItemEffect/SpiritShot/GsSpiritShotDefine.h"
#include "GameObject/Stat/GsStatHelper.h"
#include "GameObject/Fence/GsFenceHandler.h"
#include "GameObject/Fence/GsFence.h"

#include "Net/GsNetSendServiceItem.h"
#include "Net/GsNetSendService.h"

#include "Management/ScopeGlobal/GsOptionManager.h"
#include "Management/ScopeGlobal/GsSoundManager.h"
#include "Management/GsMessageHolder.h"
#include "Management/ScopeGlobal/GsGameFlowManager.h"
#include "Management/ScopeGlobal/GsUIManager.h"
#include "Management/ScopeGlobal/GsSkillManager.h"
#include "Management/ScopeGlobal/GsGameDataManager.h"
#include "Management/ScopeGlobal/GsGuildManager.h"
#include "Management/ScopeGlobal/GsLevelManager.h"
#include "Management/ScopeGame/GsDepotManager.h"
#include "Management/ScopeGame/GsInterServerManager.h"
#include "Management/ScopeGame/GsGameObjectManager.h"
#include "Management/ScopeGame/GsInventoryManager.h"
#include "Management/ScopeGame/GsCoolTimeManager.h"
#include "Management/ScopeGame/GsAIManager.h"
#include "Management/ScopeGlobal/GsQuestManager.h"
#include "Management/ScopeGame/GsSummonManager.h"
#include "Management/ScopeGame/GsItemCollectionManager.h"
#include "Management/ScopeGame/GsFairyManager.h"
#include "Management/ScopeGame/GsCampManager.h"
#include "Management/ScopeGame/GsUnlockManager.h"
#include "Management/ScopeGame/GsDungeonManager.h"
#include "Management/ScopeGame/GsValidatorChainManager.h"
#include "Management/ScopeGame/GsQuickSlotManager.h"
#include "Management/ScopeGame/GsMonsterCollectionManager.h"
#include "Management/ScopeGame/GsCostumeManager.h"
#include "Management/ScopeGame/GsInteractionManager.h"
#include "Management/ScopeGame/GsPersonalTradingManager.h"

#include "GameFlow/GsGameFlowGame.h"
#include "GameFlow/Stage/GsStageMode.h"
#include "GameFlow/Stage/StageGame/GsStageManagerGame.h"
#include "GameFlow/GameContents/ContentsGame/GsContentsManagerGame.h"
#include "GameFlow/GameContents/ContentsGame/GsGameStateDungeon.h"
#include "GameFlow/GameContents/GsContentsMode.h"

#include "AI/ContentsCondition/GsAIContentsConditionDefault.h"

#include "Summon/GsSummonFunc.h"

#include "Currency/GsCostPackage.h"
#include "Currency/GsCostPackageMultiple.h"

#include "Data/GsItemEquipEnchantLoader.h"
#include "Data/GsDataContainManager.h"
#include "Data/GsItemEquipEnchantLoader.h"

#include "DataSchema/GsSchemaEnums.h"
#include "DataSchema/GameObject/GsSchemaPartsShapeData.h"
#include "DataSchema/Skill/GsSchemaSkillCommon.h"
#include "DataSchema/Item/GsSchemaItemEquipSet.h"
#include "DataSchema/Item/GsSchemaItemEquipSetStatBalance.h"
#include "DataSchema/Item/GsSchemaItemExpiredTime.h"
#include "DataSchema/Map/GsSchemaMapData.h"
#include "DataSchema/Item/GsSchemaItemConsumeDetail.h"
#include "DataSchema/Item/GsSchemaItemIngredientDetail.h"
#include "DataSchema/Item/GsSchemaItemCommon.h"
#include "DataSchema/Item/GsSchemaItemEffectSpawnCamp.h"
#include "DataSchema/Abnormality/Category/GsSchemaCreatureActionTypeItemAffect.h"
#include "DataSchema/Item/GsSchemaItemGroup.h"

#include "Message/MessageParam/GsMessageParam.h"
#include "Message/MessageParam/GsEnchantMessageParam.h"
#include "Message/MessageParam/GsGameObjectMessageParam.h"
#include "Message/MessageParam/GsItemMessageParam.h"
#include "Message/MessageParam/GsItemContentsMessageParam.h"
#include "Message/MessageParam/GsCoolTimeMessageParam.h"
#include "Message/MessageParam/GsMonsterKnowledgeCollection.h"
#include "Message/MessageParam/GsInvenMessageParam.h"
#include "Message/MessageParam/GsPlayerDetailInfoMessageParam.h"
#include "Message/MessageParam/GsBMShopMessageParam.h"

#include "Shared/Client/SharedEnums/SharedAbnormalityEnum.h"
#include "Shared/Client/SharedEnums/SharedCreatureEnum.h"

#include "Item/InputBlock/GsInputBlockHandler.h"
#include "Item/InputBlock/GsInputBlockDefine.h"
#include "Item/Data/GsItemStatData.h"
#include "Item/GsItemHelper.h"

#include "UTIL/GsTableUtil.h"
#include "UTIL/GsGameObjectUtil.h"

#include "Skill/GsSkillHelper.h"
#include "Guild/GsGuildHelper.h"
#include "Preset/GsPresetHelper.h"
#include "Option/GsGameUserSettings.h"
#include "Sound/GsSoundPlayer.h"

#include "UI/UIContent/Helper/GsUIHelper.h"
#include "UI/UIContent/Popup/GsUIPopupInventory.h"
#include "UI/UIContent/Popup/GsUIPopupCommonItemConsume.h"
#include "UI/UIContent/Popup/GsUIPopupCalculator.h"
#include "Unlock/GsUnlockDefine.h"
#include "GsTable.h"
#include "DataSchema/Item/GsSchemaItemEquipDetail.h"
#include "Algo/Transform.h"

#include "T1Project/Sequence/GsSequencePlayer.h"


// Test
#include "T1Project/GSGameInstance.h"
#include "SentrySubsystem.h"
#include "Management/ScopeGame/GsStatChangeNotifyManager.h"



#define INVALID_ITEM_SET_ID 0

TMap<int32, TSet<ItemId>> UGsItemManager::_cachedItemIdsInItemGroup;

TMap<int32, ItemId> UGsItemManager::_cachedFirstItemIdInItemGroup;

UGsItemManager::~UGsItemManager()
{
}

void UGsItemManager::Finalize()
{
	UGsItemManager::_cachedFirstItemIdInItemGroup.Empty();
	UGsItemManager::_cachedItemIdsInItemGroup.Empty();
	_presetEquipItemContainer.Empty();
	
	_itemBuffers->ClearItems();
	delete _itemBuffers;
	_itemBuffers = nullptr;

	_allSetItemInfoMap.Empty();
	_itemTableDataMap.Empty();
	_reactionAffectItemTableDataMap.Empty();
	_currentDisableAffectItemTypList.Empty();

	if (_inputBlockHandler != nullptr)
	{
		_inputBlockHandler->Finalize();
		delete _inputBlockHandler;
		_inputBlockHandler = nullptr;
	}

	_loadedUserSettingDataDBID = 0;

	FGsMessageHolder* msg = GMessage();
	if (0 < _gameObjectMsgDelegateList.Num())
	{
		if (nullptr != msg)
		{
			for (MsgGameObjHandle& elemUI : _gameObjectMsgDelegateList)
			{
				msg->GetGameObject().Remove(elemUI);
			}
			_gameObjectMsgDelegateList.Empty();
		}
	}
	if (0 < _systemMsgDelegateList.Num())
	{	
		if (nullptr != msg)
		{
			for (MsgSystemHandle& elem : _systemMsgDelegateList)
			{
				msg->GetSystem().Remove(elem);
			}
			_systemMsgDelegateList.Empty();
		}
	}

	_itemDeliveryBoxData.Reset();
}

void UGsItemManager::Initialize()
{
	if (nullptr == _itemBuffers)
	{
		_itemBuffers = new FGsItemBuffers();
	}

	InitEquipItemContainer();
	InitIngredientUseSetting();

	FGsMessageHolder* msg = GMessage();
	if (nullptr != msg)
	{
		if (0 >= _gameObjectMsgDelegateList.Num())
		{
			// Game
			_gameObjectMsgDelegateList.Emplace(msg->GetGameObject().AddUObject(MessageGameObject::UPDATE_USER_DATA, this, &UGsItemManager::OnUserBasicDataChange));
			_gameObjectMsgDelegateList.Emplace(msg->GetGameObject().AddUObject(MessageGameObject::UPDATE_USER_PRESET_ID, this, &UGsItemManager::OnUpdateUserPresetId));
			// SpawnMe Complete
			_gameObjectMsgDelegateList.Emplace(msg->GetGameObject().AddUObject(MessageGameObject::LOCAL_SPAWN_ME_COMPLETE, this, &UGsItemManager::OnLocalPlayerSpawnComplete));
			_gameObjectMsgDelegateList.Emplace(msg->GetGameObject().AddUObject(MessageGameObject::LOCAL_REVIVE, this, &UGsItemManager::OnLocalPlayerReviveComplete));
			// Period Item Check
			_gameObjectMsgDelegateList.Emplace(msg->GetGameObject().AddUObject(MessageGameObject::LOCAL_COOLTIME_END, this, &UGsItemManager::FinishedPeriodItem));
		}
		if (0 >= _systemMsgDelegateList.Num())
		{
			_systemMsgDelegateList.Emplace(msg->GetSystem().AddUObject(MessageSystem::RECONNECT_END, this, &UGsItemManager::OnReconnectEnd));
			//_systemMsgDelegateList.Emplace(msg->GetSystem().AddUObject(MessageSystem::BACKTO_LOBBY, this, &UGsItemManager::BackToLobby));
		}
	}

	_inputBlockHandler = new FGsInputBlockHandler();
	_inputBlockHandler->Initialize();

	_loadedUserSettingDataDBID = 0;
	_isLockByReaction = false;

	_itemDeliveryBoxData.Initialize();

	_loginCreatureWeaponType = CreatureWeaponType::NONE;
}

void UGsItemManager::InitializeAfterResourceDownload()
{
	InitItemTableData();
	InitSetItemData();

}

void UGsItemManager::InitItemTableData()
{
	_itemTableDataMap.Empty();
	const UGsTableItemCommon* table = Cast<UGsTableItemCommon>(FGsSchemaItemCommon::GetStaticTable());
	if (nullptr == table)
	{
		return;
	}

	TArray<const FGsSchemaItemCommon*> rows;
	if (false == table->GetAllRows(rows))
	{
		return;
	}

	for (const FGsSchemaItemCommon* iter : rows)
	{
		if(nullptr == iter)
			continue;

		if (_itemTableDataMap.Contains(iter->categorySub))
		{
			TArray<const FGsSchemaItemCommon*>& dataList = _itemTableDataMap[iter->categorySub];
			dataList.Emplace(iter);
		}
		else
		{
			TArray<const FGsSchemaItemCommon*> dataList;
			dataList.Emplace(iter);
			_itemTableDataMap.Emplace(iter->categorySub, MoveTemp(dataList));
		}
	}

	_reactionAffectItemTableDataMap.Empty();
	const UGsTableCreatureActionTypeItemAffect* affectTable = Cast<UGsTableCreatureActionTypeItemAffect>(FGsSchemaCreatureActionTypeItemAffect::GetStaticTable());
	if (affectTable)
	{
		TArray<const FGsSchemaCreatureActionTypeItemAffect*> rowList;
		if (affectTable->GetAllRows(rowList))
		{
			TArray<ItemCategorySub> list;
			for (const FGsSchemaCreatureActionTypeItemAffect* iter : rowList)
			{
				list.Empty();
				for (ItemCategorySub iterType : iter->affectItemSubTypeList)
				{
					list.Emplace(iterType);
				}
				_reactionAffectItemTableDataMap.Emplace(iter->groupId, MoveTemp(list));
			}
		}
	}

	if (0 == UGsItemManager::_cachedItemIdsInItemGroup.Num())
	{
		const UGsTable* Table = FGsSchemaItemGroup::GetStaticTable();
		TArray<const FGsSchemaItemGroup*> AllRows;
		if (nullptr == Table || !Table->GetAllRows(AllRows))
		{
			GSLOG(Warning, TEXT("nullptr == Table || !Table->GetAllRows(AllRows)"));
		}
		else
		{
			for (const FGsSchemaItemGroup* ItemGroupRow : AllRows)
			{
				if (nullptr == ItemGroupRow)
				{
					GSLOG(Error, TEXT("nullptr == ItemGroupRow"));
					continue;
				}

				UGsItemManager::_cachedItemIdsInItemGroup.FindOrAdd(ItemGroupRow->itemGroupId).Add(ItemGroupRow->itemId);

				if (!UGsItemManager::_cachedFirstItemIdInItemGroup.Contains(ItemGroupRow->itemGroupId))
				{
					UGsItemManager::_cachedFirstItemIdInItemGroup.Add(ItemGroupRow->itemGroupId, ItemGroupRow->itemId);
				}
			}
		}
	}
}

const FGsSchemaItemCommon* UGsItemManager::FindItemSchemaDataBySubCategory(const ItemCategorySub InSubCategory, const ItemGrade InGradeType)
{
	if (_itemTableDataMap.Contains(InSubCategory))
	{
		TArray<const FGsSchemaItemCommon*> data = _itemTableDataMap[InSubCategory];
		const FGsSchemaItemCommon** findData = data.FindByPredicate([&, InGradeType](const FGsSchemaItemCommon* tableData)
			{
				if (tableData)
				{
					if (ItemGrade::NONE != InGradeType)
					{
						return tableData->grade == InGradeType ? tableData : nullptr;
					}
				}

				return tableData;
			});

		if (findData)
		{
			return *findData;
		}
	}

	return nullptr;
}

bool UGsItemManager::IsStatResetScroll(const FGsItem* inItem)
{	
	if (inItem->GetType() == ItemType::CONSUME &&
		inItem->GetSubCategory() == ItemCategorySub::RESET_BONUS_STAT)
	{		
		return true;
	}

	return false;
}

void UGsItemManager::InitIngredientUseSetting()
{
	_ingredientUseUIContentsMap.Empty();
	_ingredientUseUIContentsMap.Emplace(EGsIngredientUse_UIType::FairyUIType, [this](const FGsItem* InItem)
		{
			IngredientUse_FairyUI(InItem);
		}
	);
	_ingredientUseUIContentsMap.Emplace(EGsIngredientUse_UIType::MonsterKnowledgeUIType, [this](const FGsItem* InItem)
		{
			IngredientUse_MonsterKnowledgeUI(InItem);
		}
	);
	_ingredientUseUIContentsMap.Emplace(EGsIngredientUse_UIType::GuildCampUIType, [this](const FGsItem* InItem)
		{
			IngredientUse_GuildCampUI(InItem);
		}
	);
	_ingredientUseUIContentsMap.Emplace(EGsIngredientUse_UIType::ItemEnchantUIType, [this](const FGsItem* InItem)
		{
			IngredientUse_ItemEnchantUI(InItem);
		}
	);
	_ingredientUseUIContentsMap.Emplace(EGsIngredientUse_UIType::ItemMultiDecomposeUIType, [this](const FGsItem* InItem)
		{
			IngredientUse_ItemMultiDecomposeUI(InItem);
		}
	);
	_ingredientUseUIContentsMap.Emplace(EGsIngredientUse_UIType::DiscountCouponUIType, [this](const FGsItem* InItem)
		{
			IngredientUse_ItemCoupon(InItem);
		}
	);
	_ingredientUseUIContentsMap.Emplace(EGsIngredientUse_UIType::ChangeCostumeUIType, [this](const FGsItem* InItem)
		{
			IngredientUse_ExchangeCostumeUI(InItem);
		}
	);
}

void UGsItemManager::OnLocalPlayerReviveComplete(const struct IGsMessageParam* InParam)
{
	_isRecvSameEquipTypeProcPacket = true;
	_isRecvDiffEquipTypeProcPacket = true;
	_isLockByReaction = false;

	GSQuickSlot()->InvalidateAll();
}

void UGsItemManager::OnLocalPlayerSpawnComplete(const IGsMessageParam* InParam)
{
	_isRecvSameEquipTypeProcPacket = true;
	_isRecvDiffEquipTypeProcPacket = true;
	_isLockByReaction = false;

	//InitSetItemData();
	//OnLoadLocalPlyerSettingData();

	GSQuickSlot()->InvalidateAll();
	GSInventory()->CreateItemList();
}

void UGsItemManager::OnLoadLocalPlyerSettingData()
{
	const FGsNetUserData* userData = GGameData()->GetUserData();
	if (nullptr == userData)
		return;

	UGsGameUserSettings* userSetting = GGameUserSettings();
	if (nullptr == userSetting)
		return;

	if (0 >= userData->mUserDBId)
		return;

	/*
	if (0 >= _loadedUserSettingDataDBID)
	{
		userSetting->LoadContentsSetting(userData->mUserDBId);
		_loadedUserSettingDataDBID = userData->mUserDBId;
	}
	else
	{
		if (userData->mUserDBId != _loadedUserSettingDataDBID)
		{
			userSetting->LoadContentsSetting(userData->mUserDBId);
			_loadedUserSettingDataDBID = userData->mUserDBId;
		}
	}
	*/
}

void UGsItemManager::InitSetItemData()
{
#define DEFAULT_ITEMS_INDEX 0
#define AVAIALIBE_ITEMS_INDEX 1
#define STATS_PER_COUNT 2

	// 데이터 로딩 시점 전용 임시 컨테이너. 순서대로 대표세트아이템배열, 세트가능한모든아이템목록, 세트갯수별스탯정보
	using TempContainer = TTuple<TArray<ItemId>, TSet<ItemId>, TMap<SetNumType, TArray<FGsItemStatData>>>;

	// 키는 세트ID
	TMap<int32, TempContainer> TempMap;

	// 모든 세트 가능한 아이템ID 수집
	TArray<const FGsSchemaItemEquipDetail*> ItemEquipDetails;
	const UGsTableItemEquipDetail* TableItemEquipDetail = Cast<UGsTableItemEquipDetail>(FGsSchemaItemEquipDetail::GetStaticTable());
	if (nullptr == TableItemEquipDetail || !TableItemEquipDetail->GetAllRows(ItemEquipDetails))
	{
		GSLOG(Error, TEXT("nullptr == TableItemEquipDetail || !TableItemEquipDetail->GetAllRows(ItemEquipDetails"));
	}
	else
	{
		for (const FGsSchemaItemEquipDetail* ItemEquipDetail : ItemEquipDetails)
		{
			if (nullptr == ItemEquipDetail)
			{
				GSLOG(Warning, TEXT("nullptr == ItemEquipDetail"));
				continue;
			}

			if (INVALID_ITEM_SET_ID == ItemEquipDetail->setId)
			{
				continue;
			}

			TempMap.FindOrAdd(ItemEquipDetail->setId).Get<AVAIALIBE_ITEMS_INDEX>().Add(ItemEquipDetail->id);
		}
	}

	const UGsTableItemEquipSet* setTableData = Cast<UGsTableItemEquipSet>(FGsSchemaItemEquipSet::GetStaticTable());
	if (nullptr == setTableData)
	{
		return;
	}

	TArray<const FGsSchemaItemEquipSet*> allRow;
	if (false == setTableData->GetAllRows(allRow))
	{
		return;
	}

	TArray<ItemId> itemIdList;
	for (const FGsSchemaItemEquipSet* ItemEquipSet : allRow)
	{
		if(nullptr == ItemEquipSet || INVALID_ITEM_SET_ID == ItemEquipSet->setId)
			continue;

		itemIdList.Empty();

		if (INVALID_ITEM_ID != ItemEquipSet->itemId1)
		{
			itemIdList.Emplace(ItemEquipSet->itemId1);
		}
		if (INVALID_ITEM_ID != ItemEquipSet->itemId2)
		{
			itemIdList.Emplace(ItemEquipSet->itemId2);
		}
		if (INVALID_ITEM_ID != ItemEquipSet->itemId3)
		{
			itemIdList.Emplace(ItemEquipSet->itemId3);
		}
		if (INVALID_ITEM_ID != ItemEquipSet->itemId4)
		{
			itemIdList.Emplace(ItemEquipSet->itemId4);
		}
		if (INVALID_ITEM_ID != ItemEquipSet->itemId5)
		{
			itemIdList.Emplace(ItemEquipSet->itemId5);
		}
		if (INVALID_ITEM_ID != ItemEquipSet->itemId6)
		{
			itemIdList.Emplace(ItemEquipSet->itemId6);
		}
		if (INVALID_ITEM_ID != ItemEquipSet->itemId7)
		{
			itemIdList.Emplace(ItemEquipSet->itemId7);
		}
		if (INVALID_ITEM_ID != ItemEquipSet->itemId8)
		{
			itemIdList.Emplace(ItemEquipSet->itemId8);
		}

		TempMap.FindOrAdd(ItemEquipSet->setId).Get<DEFAULT_ITEMS_INDEX>() = itemIdList;
	}

	// 세트아이템 스탯 테이블 캐쉬
	const UGsTableItemEquipSetStatBalance* setBalanceTableData = Cast<UGsTableItemEquipSetStatBalance>(FGsSchemaItemEquipSetStatBalance::GetStaticTable());
	if (nullptr == setBalanceTableData)
	{
		return;
	}

	TArray<const FGsSchemaItemEquipSetStatBalance*> allBalanceDataRow;
	if (false == setBalanceTableData->GetAllRows(allBalanceDataRow))
	{
		return;
	}

	for (const FGsSchemaItemEquipSetStatBalance* ItemEquipSetStatBalance : allBalanceDataRow)
	{
		if (nullptr == ItemEquipSetStatBalance)
		{
			GSLOG(Error, TEXT("nullptr == ItemEquipSetStatBalance"));
			continue;
		}

		TMap<SetNumType, TArray<FGsItemStatData>>& SetItemInfoMap = TempMap.FindOrAdd(ItemEquipSetStatBalance->setId).Get<STATS_PER_COUNT>();
		SetItemInfoMap.Emplace(SetNumType::Set2, ItemEquipSetStatBalance->set2StatList);
		SetItemInfoMap.Emplace(SetNumType::Set3, ItemEquipSetStatBalance->set3StatList);
		SetItemInfoMap.Emplace(SetNumType::Set4, ItemEquipSetStatBalance->set4StatList);
		SetItemInfoMap.Emplace(SetNumType::Set5, ItemEquipSetStatBalance->set5StatList);
		SetItemInfoMap.Emplace(SetNumType::Set6, ItemEquipSetStatBalance->set6StatList);
		SetItemInfoMap.Emplace(SetNumType::Set7, ItemEquipSetStatBalance->set7StatList);
		SetItemInfoMap.Emplace(SetNumType::Set8, ItemEquipSetStatBalance->set8StatList);
	}

	// 임시 객체를 실사용 컨테이너로 이동
	_allSetItemInfoMap.Empty(TempMap.Num());
	for (TPair<int32, TempContainer>& TempIt : TempMap)
	{
		_allSetItemInfoMap.Emplace(TempIt.Key, FGsSetItemInfo(TempIt.Key, MoveTemp(TempIt.Value.Get<DEFAULT_ITEMS_INDEX>()), MoveTemp(TempIt.Value.Get<AVAIALIBE_ITEMS_INDEX>()), MoveTemp(TempIt.Value.Get<STATS_PER_COUNT>())));
	}
}

bool UGsItemManager::IsDefaultPartsShapeData(EGsPartsType InType, uint32 InPartsIndex)
{
	if (UGsGameObjectBase* base = GSGameObject()->FindObject(EGsGameObjectType::LocalPlayer))
	{
		UGsGameObjectLocalPlayer* localPlayer = base->CastGameObject<UGsGameObjectLocalPlayer>();
		if (nullptr == localPlayer)
			return false;

		FGsPartsHandlerPlayer* partsPlayer = localPlayer->GetCastParts<FGsPartsHandlerPlayer>();
		if (nullptr == partsPlayer)
			return false;

		uint32 modelIndex = partsPlayer->GetDefaultPartsModelIndex(InType);

		return modelIndex == InPartsIndex;
	}

	return false;
}

void UGsItemManager::OnReconnectEnd()
{
	_isRecvSameEquipTypeProcPacket = true;
	_isRecvDiffEquipTypeProcPacket = true;
	ResetData();
}

void UGsItemManager::ResetData()
{
	InitEquipItemContainer();
	InitIngredientUseSetting();

	_isRecvSameEquipTypeProcPacket = true;
	_isRecvDiffEquipTypeProcPacket = true;

	_itemBuffers->ResetData();

	_itemWeightPenaltyGradeType = WeightPenaltyGrade::NONE;
}

void UGsItemManager::BackToLobby()
{
	ResetData();
}

void UGsItemManager::InitEquipItemContainer()
{
	_presetEquipItemContainer.Empty();
	TMap<EGsEquipSlotType, uint64> equipItemMap;
	for (uint8 i = 0; i < (uint8)EGsEquipSlotType::SLOT_MAX; ++i)
	{
		equipItemMap.Add((EGsEquipSlotType)i, INVALID_ITEM_DB_ID);
	}
	_presetEquipItemContainer.Emplace((PresetValue)EPresetNumberBit::Preset_First, equipItemMap);
	_presetEquipItemContainer.Emplace((PresetValue)EPresetNumberBit::Preset_Second, equipItemMap);
	_presetEquipItemContainer.Emplace((PresetValue)EPresetNumberBit::Preset_Third, equipItemMap);

	_accSlotTypeContainer.Empty();
	TMap<uint8, EGsEquipSlotType> slotData;
	for (uint8 i = (uint8)ItemCategorySub::SWORD; i <= (uint8)ItemCategorySub::MAX_EQUIP; ++i)
	{
		slotData.Empty();
		if (false == IsValidItemCategorySub((ItemCategorySub)i))
			break;

		for (uint8 j = 0; j < ACC_SLOT_MAX_COUNT; ++j)
		{
			slotData.Emplace(j , GetEquipSlotType((ItemCategorySub)i, j));
		}
		_accSlotTypeContainer.Emplace((ItemCategorySub)i, slotData);
	}
}

EGsEquipSlotType UGsItemManager::GetEquipSlotType(const ItemCategorySub InSubCategory, const uint8 InSlotIndex) const
{
	switch (InSubCategory)
	{
	case ItemCategorySub::SWORD:
	case ItemCategorySub::DAGGER:
	case ItemCategorySub::BOW:
	case ItemCategorySub::SHOTGUN:
	case ItemCategorySub::WAND:
	case ItemCategorySub::WAEPON_ORB:
		return EGsEquipSlotType::WEAPON_TYPE;
	case ItemCategorySub::HELMET:
		return EGsEquipSlotType::HELMET_TYPE;
	case ItemCategorySub::BODY_ARMOR:
		return EGsEquipSlotType::ARMOR_TYPE;
	case ItemCategorySub::GLOVE:
		return EGsEquipSlotType::GLOVE_TYPE;
	case ItemCategorySub::SHOES:
		return EGsEquipSlotType::SHOES_TYPE;
	case ItemCategorySub::BELT:
		return EGsEquipSlotType::BELT_TYPE;
	case ItemCategorySub::SCARF:
		return EGsEquipSlotType::SCARF_TYPE;
	case ItemCategorySub::NECKLACE:
		return EGsEquipSlotType::NECKLACE_TYPE;
	case ItemCategorySub::EAR_RING:
		return (0 == InSlotIndex) ? EGsEquipSlotType::EAR_RING_LEFT_TYPE : EGsEquipSlotType::EAR_RING_RIGHT_TYPE;
	case ItemCategorySub::RING:
		return (0 == InSlotIndex) ? EGsEquipSlotType::RING_LEFT_TYPE : EGsEquipSlotType::RING_RIGHT_TYPE;
	case ItemCategorySub::CHARM:
		return (0 == InSlotIndex) ? EGsEquipSlotType::CHARM_LEFT_TYPE : EGsEquipSlotType::CHARM_RIGHT_TYPE;
	case ItemCategorySub::BRACELET:
		return (0 == InSlotIndex) ? EGsEquipSlotType::BRACELET_LEFT_TYPE : EGsEquipSlotType::BRACELET_RIGHT_TYPE;
	case ItemCategorySub::DURAL:
		return EGsEquipSlotType::DURAL_LEFT_TYPE;
	case ItemCategorySub::MARBLE:
		return (0 == InSlotIndex) ? EGsEquipSlotType::MARBLE_LEFT_TYPE : EGsEquipSlotType::MARBLE_RIGHT_TYPE;
	}

	return EGsEquipSlotType::SLOT_MAX;
}

void UGsItemManager::OnUserBasicDataChange(const IGsMessageParam* inParam)
{
	const FGsNetUserData* data = inParam->Cast<const FGsNetUserData>();
	if (0 >= _presetEquipItemContainer.Num())
	{
		InitEquipItemContainer();
	}

	_loginCreatureWeaponType = data->mLoginWeaponType;
	_localPlayerGameID = data->mGameId;
	_userPresetId = data->mUserPresetId;
	_userPresetBitId = FGsPresetHelper::GetPresetValueFromId(_userPresetId);
}

void UGsItemManager::OnUpdateUserPresetId(const struct IGsMessageParam* InParam)
{
	if (const FGsPrimitiveUInt8* param = InParam->Cast<const FGsPrimitiveUInt8>())
	{
		PresetId copyPresetId = _userPresetId;
		_userPresetId = param->_data;
		_userPresetBitId = FGsPresetHelper::GetPresetValueFromId(_userPresetId);

		SetCurrentLoginCreatureWeaponType();
		GSQuickSlot()->InvalidateAll();

		bool isEquip = false;
		bool isCompleteSpawn = UGsGameObjectUtil::GetSpawnMeCompleteFinFlag();
		if (false == isCompleteSpawn)
			return;

		if (false == _presetEquipItemContainer.Contains(_userPresetBitId))
			return;

		TMap<EGsEquipSlotType, uint64> curPresetData = _presetEquipItemContainer[_userPresetBitId];
		for (TPair<EGsEquipSlotType, uint64> iter : curPresetData)
		{
			ItemDBId copyPrestItemDbid = GetEquipedItemDBIDBySlotType(copyPresetId, iter.Key);
			if (copyPrestItemDbid != iter.Value)
			{
				isEquip = 0 < iter.Value;
				if (isEquip)
				{
					break;
				}
			}
		}
		
		// sound
		// 장비 장착 / 해지 에 따른 사운드 출력
		// swap 일때는 장착 사운드만 출력 해야함
		if (isCompleteSpawn)
		{
			FString fileName = isEquip ? "UI_EquipGear" : "UI_Cancel";
			UGsSoundPlayer* soundPlayer = GSoundPlayer();
			if (nullptr != soundPlayer)
			{
				soundPlayer->PlaySound2D(fileName);
			}
		}

		if (UGsUIManager* uiManager = GUI())
		{
			uiManager->CloseByKeyName(TEXT("PopupItemDetail"));
			uiManager->CloseByKeyName(TEXT("PopupItemDetail_CharDetail"));
			uiManager->CloseByKeyName(TEXT("PopupItemDetailCompare"));
		}

		GMessage()->GetPlayerDetailInfo().SendMessage(MessageContentPlayerInfo::EQUIP_ALL, nullptr);
		GMessage()->GetGameObject().SendMessage(MessageGameObject::UPDATE_USER_PRESET_EQUIP, nullptr);
	}
}

// hud 천마석(정령탄) 들어갈 아이템 세팅
void UGsItemManager::PickHudSpiritShotItem()
{
	// 똑같은 아이템이지만, 스텍카운트 넘어서 여러개일 경우, 
	// DBID 기준, 낮은 갯수부터 소모 처리함
	TArray<TWeakPtr<FGsItem>> findItemList = FindItem(ItemType::CONSUME, ItemCategorySub::SPIRIT_SHOT);
	if (0 == findItemList.Num())
	{
		_curHudSpiritShotDBID = 0;
		return;
	}

	ItemAmount ownedCount = 0;
	for (int32 i = 0; i < findItemList.Num(); ++i)
	{
		FGsItem* itemData = GetGsItemLowPointer(findItemList[i]);
		if (nullptr == itemData)
			continue;
		
		// 이전에 갯수 없다면
		if (0 == ownedCount)
		{
			// 세팅
			ownedCount = itemData->GetAmount();
			_curHudSpiritShotDBID = itemData->GetDBID();
			continue;
		}

		// 수량 같거나 많으면 바꿀 필요 없음
		if (itemData->GetAmount() >= ownedCount)
		{
			continue;
		}

		ownedCount = itemData->GetAmount();
		_curHudSpiritShotDBID = itemData->GetDBID();
	}


}
// 정령탄 상태 갱신
void UGsItemManager::SetSpiritShotStateByUIState(bool In_spiritShotUIState, bool In_invalidate)
{

	EGsSpiritShotState newState = EGsSpiritShotState::HUD_OFF;
	if (In_spiritShotUIState)
	{
		newState = EGsSpiritShotState::HUD_ON;
	}
	else
	{
		newState = EGsSpiritShotState::HUD_OFF;
	}

	UGsGameObjectManager* gameObjectManager = GSGameObject();
	if (gameObjectManager == nullptr)
	{
		return;
	}

	// 1. 현재 상태는
	UGsGameObjectBase* local = gameObjectManager->FindObject(EGsGameObjectType::LocalPlayer);
	if (nullptr == local)
	{
		return;
	}

	FGsGameObjectDataLocalPlayer* data = local->GetCastData<FGsGameObjectDataLocalPlayer>();
	
	if (data == nullptr)
	{
		return;
	}
	data->SetSpiritShotState(newState);	

	if (In_invalidate == true)
	{
		GMessage()->GetContentsHud().SendMessage(MessageContentHud::INVALIDATE_CONSUMEABLE);
	}
}

void UGsItemManager::UpdateSkillBookState(ItemId InSkillBookItemId)
{
	if (INVALID_ITEM_ID == InSkillBookItemId)
	{
		return;
	}

	// 배운 스킬에 대한 스킬북의 Learn 마크 표시를 위해 갱신하는 거라 
	// MessageContentHud::INVALIDATE_CONSUMEABLE 을 보낼 필요가 없다.
	TArray<TWeakPtr<FGsItem>> skillBookList = FindByTID(InSkillBookItemId, ItemType::CONSUME);
	FGsItem* itemData;
	for (TWeakPtr<FGsItem> item : skillBookList)
	{
		itemData = GetGsItemLowPointer(item);
		if(nullptr == itemData)
			continue;

		SendUpdateMessage(itemData->GetDBID(), true, false);
	}
}

/*
// 자동물약 사용
// 현재는 hp potion 만 자동 사용함
void UGsItemManager::TryAutoPotionItem()
{
	/// <summary>
	/// Exceptional Block
	/// </summary>

	// 스턴상태일때는 return 처리
	if (true == _isPlayerStunState)
		return;

	// 포션 자동사용에 대한 패킷을 보낸 후, 받기전에 또 보내는 경우 방지
	if (false == _isRecvAutoHpPotionPacket)
		return;

	// 포션 퀵슬롯에 등록된게 없음
	if (0 >= _curHudAutoPotionDBID)
		return;

	if (nullptr == GSGameObject())
		return;

	UGsGameObjectBase* local = GSGameObject()->FindObject(EGsGameObjectType::LocalPlayer);
	if (nullptr == local)
	{
		return;
	}

	FGsGameObjectDataCreature* creatureData = local->GetCastData<FGsGameObjectDataCreature>();
	if (nullptr == creatureData)
	{
		return;
	}

	// 사망시, 사용 안함
	if (true == creatureData->IsZeroHp())
		return;

	const FGsStatInfo* hp = creatureData->GetCreatureStatInfo(StatType::HP);
	if (nullptr == hp)
	{
		return;
	}

	/// <summary>
	/// Execution Block
	/// </summary>		
	// %율 - 100 분율 기준
	
	float hpPer = GSAI()->GetContentsConditionDefault()->GetAutoUseHPPotionRate();
	if (0.f >= hpPer)
		return;

	float curHP = (float)hp->GetStatValue();
	float calc = hp->GetMaxStatValue() * hpPer;
	float calc_round = round(calc);
	if (curHP <= calc_round)
	{
		FGsItem* itemData = FindItem(_curHudAutoPotionDBID,ItemType::CONSUME);
		if (nullptr == itemData)
			return;

		TryUseItem(itemData);
	}
}

// 소모성 아이템 쿨타임 종료 후, 체크하는 함수(특정 아이템에 대하여 이어서 사용해야하는 경우)
void UGsItemManager::TryAutoPotionItem(FGsItem& In_Item)
{
	TryAutoPotionItem();
}
*/

bool UGsItemManager::IsCurrentPresetItem(const FGsItem* In_Item)
{
	if (false == In_Item->IsEquipment())
		return false;

	if (0 >= _userPresetId)
	{
		FGsGameDataManager* gameData = GGameData();
		const FGsNetUserData* userData = gameData->GetUserData();
		_userPresetId = userData->mUserPresetId;
		_userPresetBitId = FGsPresetHelper::GetPresetValueFromId(_userPresetId);
	}

	return In_Item->GetEquipPresetValue() & _userPresetBitId;
}

/*
// 강제적으로 포션 아이템을 사용
void UGsItemManager::RetryConsumItem()
{
	TArray<FGsItem*> findItemList = FindItem(ItemType::CONSUME, ItemCategory::HP_POTION);
	if (0 < findItemList.Num())
	{
		FGsItem* findItem = findItemList[0];
		if (nullptr != findItem)
		{
			TryUseItem(findItem);
		}
	}
}
*/

// 아이템 제거
bool UGsItemManager::TryDeleteItem(const FGsItem* InTargetItem, const ItemAmount InAmount)
{
	if(false == FGsItem::IsValidItem(InTargetItem))
	{
		// 유효하지 않은 아이템
		return false;
	}
	
	// 상태이상일때 사용 못함
	if (true == IsReactionLockTypeItem(InTargetItem->GetSubCategory()))
	{
		// 스턴 상태일때 불가능 합니다
		FText findText;
		FText::FindText(TEXT("ItemUIText"), TEXT("ItemUi_Notice_Stunned"), findText);
		FGsUIHelper::TrayMessageTicker(findText);
		return false;
	}

	if (true == InTargetItem->IsEquipped() || InTargetItem->IsPresetEquipped())
	{
		// 장착중인 아이템
		// 장착한 아이템은 삭제할 수 없습니다
		FText findText;
		FText::FindText(TEXT("ItemUIText"), TEXT("ItemUi_Delete_Notice_Equiped"), findText);
		FGsUIHelper::TrayMessageTicker(findText);
		return false;
	}

	// 0보다 큰가
	if (0 >= InAmount)
	{
		// ticker - stack count error
		return false;
	}

	// 현재 lock 상태인가
	if (true == InTargetItem->GetLock())
	{
		// ticker - 아이템이 잠겨 있음
		// 잠긴 아이템은 삭제할 수 없습니다
		FText findText;
		FText::FindText(TEXT("ItemUIText"), TEXT("ItemUi_Delete_Notice_Locked"), findText);
		FGsUIHelper::TrayMessageTicker(findText);
		return false;
	}

	// delete 하려는 갯수보다 보유하고 있는 갯수가 적으면 보유하고 있는 갯수로 제거요청 함
	ItemAmount resultCount = InTargetItem->GetAmount() < InAmount ? InTargetItem->GetAmount() : InAmount;
	FGsNetSendServiceItem::SendDeleteItem(InTargetItem->GetDBID(), resultCount);
	return true;
}

bool UGsItemManager::TryUseItem(const uint64 InItemDBID, const ItemAmount InAmount, const bool InIsOnlyValidCheck)
{
	FGsItem* itemData = GetGsItemLowPointer(FindItem(InItemDBID));
	if (nullptr == itemData)
	{
		return false;
	}

	return TryUseItem(itemData, InAmount, InIsOnlyValidCheck);
}

// 모든 아이템 사용에 대해서는 이곳으로 단일화 함
bool UGsItemManager::TryUseItem(const FGsItem* In_Item, const ItemAmount InAmount, const bool InIsOnlyValidCheck)
{
	if (false == FGsItem::IsValidItem(In_Item))
	{
		// 유효하지 않은 아이템
		return false;
	}

	// 사용할 수 있는 레벨인지 확인
	if (false == IsUsableLevel(In_Item))
	{
		FText findText;
		FTextKey textKey = (ItemType::EQUIP == In_Item->GetType()) ? TEXT("Consume_Use_Limit_Equip") : TEXT("Consume_Use_Limit_Consume");
		FText::FindText(TEXT("ItemDescriptionText"), textKey, findText);
		FText resultText = FText::Format(findText, In_Item->GetUseLevel());
		FGsUIHelper::TrayMessageTicker(resultText);
		return false;
	}

	// 상태이상일때 사용 못함
	if (true == IsReactionLockTypeItem(In_Item->GetSubCategory()))
	{
		// 스턴 상태일때 불가능 합니다
		FText findText;
		FText::FindText(TEXT("ItemUIText"), TEXT("ItemUi_Notice_Stunned"), findText);
		FGsUIHelper::TrayMessageTicker(findText);
		return false;
	}

	// stack count 가 0보다 큰가
	if (0 >= In_Item->GetAmount())
	{
		return false;
	}

	ItemAmount resultAmount = InAmount;
	// 일괄 사용에 대한 처리
	if (0 < resultAmount)
	{
		if (resultAmount > In_Item->GetAmount())
		{
			resultAmount = In_Item->GetAmount();
		}
	}

	// 현재 lock 상태인가 
	// 장착류 무기들은 장착해지 가 되어야 하므로 제외처리
	if (true == In_Item->GetLock() && ItemType::EQUIP != In_Item->GetType())
	{
		// ticker - 아이템이 잠겨 있음
		FText findText;
		FText::FindText(TEXT("ItemUIText"), TEXT("ItemUi_Use_Notice_Locked"), findText);
		FGsUIHelper::TrayMessageTicker(findText);
		return false;
	}

	// 사용 가능 유효기간 체크
	// 다만 장착 중인 아이템일때 만료 되면 장착 해지 처리를 해줘야 함
	if (In_Item->IsPeriodItem() && In_Item->IsExpiredTime() && false == In_Item->IsEquipped())
	{
		FText findText;
		FText::FindText(TEXT("ItemUIText"), TEXT("ItemUi_Notice_CannotUse"), findText);
		FGsUIHelper::TrayMessageTicker(findText);
		return false;
	}

	// 소모품이나 사용성 재료형 아이템의 쿨타임 그룹 체크
	if (ItemType::CONSUME == In_Item->GetType() || ItemType::INGREDIENT == In_Item->GetType())
	{
		if (true == GCoolTime()->IsCoolTime(EGsCoolTime::Item, In_Item->GetCooldownGroupID(), 0))
		{
			// 해당 아이템의 GroupID 로 쿨타임이 돌고 있는 상태
			return false;
		}
	}

	// 각 아이템과 관련한 컨텐츠의 잠금 확인
	if (false == GSUnlock()->IsContentsUnlock(In_Item->GetSubCategory()))
	{
		GSUnlock()->ShowLockTicker(In_Item->GetSubCategory());
		return false;
	}

	if (FGsPersonalTradingManager* tradeManager = GSPersonalTrading())
	{
		if (tradeManager->IsSelectedItem(In_Item->GetDBID()))
		{
			return false;
		}
	}

	// [B1] | ejrrb10 | HP 가 0일때 모든 템 사용 못함 (추후 사망 시, 아이템 사용 불가 티커를 띄우지 않는 로직 처리)
	UGsGameObjectBase* local = GSGameObject()->FindObject(EGsGameObjectType::LocalPlayer);
	if (local == nullptr)
	{
		return false;
	}
	FGsGameObjectDataCreature* creatureData = local->GetCastData<FGsGameObjectDataCreature>();
	if (creatureData == nullptr || creatureData->IsZeroHp())
	{
		return false;
	}

	if (false == InIsOnlyValidCheck)
	{
		// itemType 분기 처리(cousume 사용 / equipment 사용 packet이 다름)
		switch (In_Item->GetType())
		{
		case ItemType::EQUIP:
			TryEquipItem(In_Item);
			break;
		case ItemType::CONSUME:
			//TODO : 사용한 Count 추가
			TryConsumItem(In_Item, resultAmount);
			break;
		case ItemType::INGREDIENT:
			TryIngredientItem(In_Item);
			break;
		case ItemType::ETC:
			break;

		default:
			// ticker - check TryUseItem In ItemType
			break;
		}
	}

	return true;
}

void UGsItemManager::TryConsumItem(const FGsItem* In_Item, const ItemAmount InAmount)
{
	if (true == CheckConsumItemCondition(In_Item))
	{	
		if (0 < InAmount)
		{
			ItemDBIdAmountPair pair;
			pair.mItemDBId = In_Item->GetDBID();
			pair.mItemAmount = InAmount;
			FGsNetSendServiceItem::SendSeveralUseItem(pair);
		}
		else
		{
			ItemCategorySub subCategory = In_Item->GetSubCategory();

			if (ItemCategorySub::SUMMON_FAIRY == subCategory)
			{
				GSStatChangeNotify()->Save(StatUpdateReason::FAIRY_COLLECTION);
			}
			else if (ItemCategorySub::SUMMON_COSTUME == subCategory)
			{
				GSStatChangeNotify()->Save(StatUpdateReason::COSTUME_COLLECTION);
			}

			FGsNetSendServiceItem::SendUseItem(In_Item->GetDBID(), subCategory);
		}

		if (FGsInteractionManager* interactionManager = GSInteraction())
		{
			interactionManager->TryConsumeItem(In_Item);
		}
	}
}

void UGsItemManager::TryIngredientItem(const FGsItem* In_Item)
{
	const FGsItemIngredient* ingredientItem = static_cast<const FGsItemIngredient*>(In_Item);
	if (nullptr == ingredientItem)
		return;

	if (true == CheckIngredientItemCondition(ingredientItem))
	{
		UseIngredientUseTypeItem(ingredientItem->GetIngredientUseContentsType(), In_Item);
		//ItemCategorySub subCategory = In_Item->GetSubCategory();
		//FGsNetSendServiceItem::SendUseItem(In_Item->GetDBID(), subCategory);
	}
}

void UGsItemManager::UseIngredientUseTypeItem(const EGsIngredientUse_UIType InType, const FGsItem* InItem)
{
	if (_ingredientUseUIContentsMap.Contains(InType))
	{
		if (nullptr == _ingredientUseUIContentsMap[InType])
		{
			InitIngredientUseSetting();
		}
		_ingredientUseUIContentsMap[InType](InItem);
	}
	else
	{
		GSLOG(Error, TEXT("[UGsItemManager::UseIngredientUseTypeItem] is Not Exist Type - InTypeNum : %d"), (uint8)InType);
	}
}

bool UGsItemManager::CheckIngredientItemCondition(const FGsItemIngredient* InIngredientItem)
{
	// Running CoolTime
	if (true == GCoolTime()->IsCoolTime(EGsCoolTime::Item, InIngredientItem->GetCooldownGroupID(), 0))
	{
		// 해당 아이템의 GroupID 로 쿨타임이 돌고 있는 상태
		return false;
	}
	if (InIngredientItem->IsExpiredTime())
	{
		// 아이템 유효기간 만료된 상태
		return false;
	}

	// 각 아이템 서브 카테고리 마다의 condition check
	switch (InIngredientItem->GetSubCategory())
	{
	case ItemCategorySub::SPAWN_CAMP:
	{
		// 성소 업데이트로 수호탑 설치 막기 처리
		// TEXT: 사용할 수 없습니다
		FText textMsg;
		FText::FindText(TEXT("ItemUIText"), TEXT("ItemUi_Notice_CannotUse"), textMsg);
		FGsUIHelper::TrayMessageTicker(textMsg);
		
		return false;


		// 안전지대 및 마을 안에서는 설치 불가능
		if (FGsGameFlowGame* gameFlow = GMode()->GetGameFlow())
		{
			if (FGsStageManagerGame* stage = gameFlow->GetStageManagerGame())
			{
				if (FGsStageMode::SAFETY_ZONE == stage->GetCurrentStageMode()
					|| FGsStageMode::TOWN == stage->GetCurrentStageMode()
					|| FGsStageMode::ARENA == stage->GetCurrentStageMode()
					|| FGsStageMode::ARENA_READY == stage->GetCurrentStageMode())
				{
					FText findText;
					FText::FindText(TEXT("GuildText"), TEXT("CAMP_ERROR_SAFE_TYPE_TERRITORY"), findText);
					FGsUIHelper::TrayMessageTicker(findText);
					return false;
				}
			}
		}

		if (FGsFenceHandler* fenceHandler = GFence())
		{
			FGsFence* fence = nullptr;
			if (fenceHandler->TryGetPlayerTopPriorityServerFence(fence))
			{
				if (TerritoryType::TOWN_CHAOS == fence->GetTerritoryType())
				{
					FText findText;
					FText::FindText(TEXT("GuildText"), TEXT("Alert_CannotBuilding"), findText);
					FGsUIHelper::TrayMessageTicker(findText);
					return false;
				}
			}
		}

		// 인터서버에서는 사용불가능
		if (GGameData()->IsInvadeWorld())
		{
			FText findText;
			FText::FindText(TEXT("GuildText"), TEXT("UI_Camp_Desc_NotUse_03"), findText);
			FGsUIHelper::TrayMessageTicker(findText);
			return false;
		}

		FGsGuildManager* guildMgr = GGuild();
		if (nullptr != guildMgr)
		{
			if (false == guildMgr->IsGuild())
			{
				FText findText;
				FText::FindText(TEXT("GuildText"), TEXT("UI_Camp_Desc_NotUse_01"), findText);
				FGsUIHelper::TrayMessageTicker(findText);
				return false;
			}
			if (false == FGsGuildHelper::CanManageCamp(guildMgr->GetAuthority()))
			{
				FText findText;
				FText::FindText(TEXT("GuildText"), TEXT("UI_Camp_Desc_NotUse_02"), findText);
				FGsUIHelper::TrayMessageTicker(findText);
				return false;
			}
			if (FGsCampManager* campMgr = GSCamp())
			{
				if (false == campMgr->IsAllowedBuildCamp())
				{
					FText findText;
					FText::FindText(TEXT("GuildText"), TEXT("UI_Camp_Desc_NotUse_06"), findText);
					FGsUIHelper::TrayMessageTicker(findText);
					return false;
				}
			}
		}
		else
		{
			return false;
		}

		const FGsSchemaItemEffectSpawnCamp* effectData = GSCamp()->FindCampEffectData(InIngredientItem->GetDBID());
		if (nullptr != effectData)
		{
			int32 levelKey = GLevel()->GetCurrentLevelId();
			const int32* findID = effectData->allowedMapIdList.FindByPredicate([&levelKey](int32 InMapID)
				{
					return levelKey == InMapID;
				});

			if (nullptr == findID)
			{
				FText findText;
				FText::FindText(TEXT("GuildText"), TEXT("UI_Camp_Desc_NotUse_03"), findText);
				FGsUIHelper::TrayMessageTicker(findText);
				return false;
			}

			// 캠프 레벨 체크
			uint16 currCampLevel = GGuild()->GetCampLevel();
			if (effectData->campLevel > currCampLevel)
			{
				FText findText;
				FText::FindText(TEXT("GuildText"), TEXT("UI_Camp_Desc_NotUse_05"), findText);
				FGsUIHelper::TrayMessageTicker(findText);
				return false;
			}
		}
		else
		{
			return false;
		}
	}
	case ItemCategorySub::ENCHANT_FAIRY:
	{
		break;
	}
	case ItemCategorySub::ENCHANT_SKILL_SCROLL:
	{
		break;
	}
	case ItemCategorySub::ENCHANT_WEAPON_SCROLL:
	case ItemCategorySub::BLESSED_ENCHANT_WEAPON:
	case ItemCategorySub::CURSED_ENCHANT_WEAPON:
	case ItemCategorySub::ENCHANT_ARMOR:
	case ItemCategorySub::BLESSED_ENCHANT_ARMOR:
	case ItemCategorySub::CURSED_ENCHANT_ARMOR:
	case ItemCategorySub::ENCHANT_ACCESSORY:
	case ItemCategorySub::BLESSED_ENCHANT_ACCESSORY:
	case ItemCategorySub::CURSED_ENCHANT_ACCESSORY:
	case ItemCategorySub::OPTION_STONE:
	case ItemCategorySub::BLESSED_OPTION_STONE:
	{
		break;
	}
	case ItemCategorySub::DECOMPOSE_EQUIP_KIT:
	case ItemCategorySub::DECOMPOSE_SKILLBOOK_KIT:
	{
		break;
	}
	case ItemCategorySub::MONSTER_KNOWLEDGE_SLATE_UNLOCK_ATTACK:
	case ItemCategorySub::MONSTER_KNOWLEDGE_SLATE_UNLOCK_HIT:
	case ItemCategorySub::MONSTER_KNOWLEDGE_SLATE_UNLOCK_AMPLIFICATION:
	case ItemCategorySub::MONSTER_KNOWLEDGE_SLATE_UNLOCK_DEFENCE:
	case ItemCategorySub::MONSTER_KNOWLEDGE_SLATE_UNLOCK_RESISTANCE:
	case ItemCategorySub::MONSTER_KNOWLEDGE_SLATE_UNLOCK_SURVIVAL:
	{
		// 2021/12/22 PKT - 몬스터 도감 전용 아이템(지식 석판 오픈 아이템)
		// 2021/12/22 PKT - 해당 아이템 사용 바로가기
		break;
	}
	case ItemCategorySub::DISCOUNT_COUPON:
		break;
	case ItemCategorySub::CHANGE_COSTUME_RANDOM:
	case ItemCategorySub::CHANGE_COSTUME_SELECT:
	{
		if (GSUnlock() && GSUnlock()->IsContentsUnlock(EGsUnlockContentsType::COSTUME, true) == false)
			return false;

		if (FGsGameFlowGame* gameFlow = GMode()->GetGameFlow())
		{
			if (FGsStageManagerGame* stage = gameFlow->GetStageManagerGame())
			{
				if (FGsStageMode::TOWN != stage->GetCurrentStageMode())
				{
					FText findText;
					FText::FindText(TEXT("CostumeText"), TEXT("Alert_Ticker_NotInTown"), findText);
					FGsUIHelper::TraySectionMessageTicker(EGsNotiSection::EXTSYSTEM, findText);
					return false;
				}

				if (auto player = Cast<UGsGameObjectPlayer>(GSGameObject()->FindObject(EGsGameObjectType::LocalPlayer)))
				{
					bool peaceful = player->IsBattleMode() == false;
					if (FGsAbnormalityHandlerBase* abnormalityHandler = peaceful ? player->GetAbnormalityHandler() : nullptr)
					{
						peaceful = abnormalityHandler->IsDamagedByAbnormality() == false;
					}

					if (!peaceful)
					{
						FText findText;
						FText::FindText(TEXT("CostumeText"), TEXT("Alert_Ticker_InCombat"), findText);
						FGsUIHelper::TraySectionMessageTicker(EGsNotiSection::EXTSYSTEM, findText);
						return false;
					}
				}
			}
		}
		break;
	}	
	}

	return InIngredientItem->IsUseItem();
}

bool UGsItemManager::CheckConsumItemCondition(const FGsItem* InItem)
{
	const FGsItemConsumable* consumItem = static_cast<const FGsItemConsumable*>(InItem);
	if (nullptr == consumItem)
	{
		return false;
	}

	// Running CoolTime
	if (true == GCoolTime()->IsCoolTime(EGsCoolTime::Item, consumItem->GetCooldownGroupID(), 0))
	{
		// 해당 아이템의 GroupID 로 쿨타임이 돌고 있는 상태
		return false;
	}

	// 각 아이템 서브 카테고리 마다의 condition check
	switch (InItem->GetSubCategory())
	{
	case ItemCategorySub::HP_POTION:
	case ItemCategorySub::HP_POTION_MANUAL:
	{
		if (UGsGameObjectBase* local = GSGameObject()->FindObject(EGsGameObjectType::LocalPlayer))
		{
			FGsGameObjectDataCreature* creatureData = local->GetCastData<FGsGameObjectDataCreature>();
			if (nullptr == creatureData)
				return false;

			const FGsStatInfo* curInfo = creatureData->GetCreatureStatInfo(((ItemCategorySub::HP_POTION == InItem->GetSubCategory()) || (ItemCategorySub::HP_POTION_MANUAL == InItem->GetSubCategory()))
				? StatType::HP : StatType::MP);
			if (nullptr != curInfo)
			{
				bool isMax = (curInfo->GetMaxStatValue() <= curInfo->GetStatValue()) ? true : false;
				return !isMax;
			}
		}
		break;
	}
	case ItemCategorySub::MP_POTION:
	case ItemCategorySub::POTION_BUFF:
	case ItemCategorySub::SCROLL_BUFF:
	case ItemCategorySub::STATUE_BUFF:
	case ItemCategorySub::MP_TICK_RECOVERY_POTION:
		if (consumItem->IsSpawnInvadePortal()) //old
		{
			if (GGameData()->IsInvadeWorld())
			{
				FText findText;
				FText::FindText(TEXT("InvadeText"), TEXT("Invade_Cant_Spawn_01"), findText);
				FGsUIHelper::TrayMessageTicker(findText);
				return false;
			}
			const FGsSchemaMapData* mapData = GLevel()->GetCurrentMapInfo();
			if (nullptr != mapData)
			{
				if (false == GSInterServer()->GetInvadeMapData(mapData->id).IsValid())
				{
					FText findText;
					FText::FindText(TEXT("NetText"), TEXT("INVADE_ERROR_INVALID_MAP"), findText);
					FGsUIHelper::TrayMessageTicker(findText);
					return false;
				}
			}
		}
		return true;	
	case ItemCategorySub::WARP_TO_BOOKMARK_SCROLL:
	{
		// 시네마틱 플레이 중이면?
		if (true == UGsSequencePlayer::IsSequencePlaying())
		{
			FText findText;
			FText::FindText(TEXT("NetText"), TEXT("WARP_ERROR_CINEMATIC_CANCLE"), findText);
			FGsUIHelper::TrayMessageTicker(findText);
			return false;
		}

		if (nullptr != InItem)
		{
			// 위치 즐겨 찾기 ui 열기
			if (UGsUIManager* uiMgr = GUI())
			{
				uiMgr->Open(TEXT("PopupBookmark"));
			}
			return false;
		}

		FGsPrimitiveInt32 param((int32)ItemCategorySub::WARP_TO_BOOKMARK_SCROLL);
		GMessage()->GetItemContents().SendMessage(MessageContentItem::USE_WARP_ITEM, &param);

		break;
	}
	case ItemCategorySub::COMMON_SKILL_BOOK:
	case ItemCategorySub::SWORD_SKILL_BOOK:
	case ItemCategorySub::DAGGER_SKILL_BOOK:
	case ItemCategorySub::BOW_SKILL_BOOK:
	case ItemCategorySub::WAND_SKILL_BOOK:
	case ItemCategorySub::ORB_SKILL_BOOK:
	{
		TArray<int32> userSkillSetIdList;
		if (consumItem->GetUserSkillSetIdList(userSkillSetIdList))
		{
			// 하나라도 사용 가능하면 사용 가능하다고 체크
			for (int i = 0; i < userSkillSetIdList.Num(); ++i)
			{
				if (FGsSkillHelper::CanUseSkillBook(userSkillSetIdList[i], true))
				{
					return true;
				}
			}
		}

		return false;
	}
	case ItemCategorySub::WARP_TO_TOWN_SCROLL:
	{
		// 시네마틱 플레이 중이면?
		if(true == UGsSequencePlayer::IsSequencePlaying())
		{
			FText findText;
			FText::FindText(TEXT("NetText"), TEXT("WARP_ERROR_CINEMATIC_CANCLE"), findText);
			FGsUIHelper::TrayMessageTicker(findText);
			return false;
		}

		// https://jira.com2us.com/jira/browse/C2URWQ-2476
		// check input lock(quest dialog server send -> try warp)
		if (true == UGsGameObjectUtil::IsInputFlagBlock(EGsKeyMappingInputFlags::INPUT_CONTENTS_WAIT_SERVER_RESPONSE))
		{
			return false;
		}

		// 2022/08/05 PKT - 파티 던전
		if (MapType::PARTY_DUNGEON == GLevel()->GetMapType())
		{
			FGsPrimitiveUInt64 message(InItem->GetDBID());
			GMessage()->GetDungeon().SendMessage(MessageContentDungeon::TRY_EXIT_DUNGEON, &message);
			
			// 2022/08/05 PKT - 메세지 처리 하는 곳에서 아이템 사용 패킷을 처리 할 것이므로 여기서는 False로 빠져 나감.
			return false;
		}

		FGsPrimitiveInt32 param((int32)ItemCategorySub::WARP_TO_TOWN_SCROLL);
		GMessage()->GetItemContents().SendMessage(MessageContentItem::USE_WARP_ITEM, &param);

		return true;
	}
	case ItemCategorySub::WARP_TO_RANDOM_SCROLL: // 랜덤 워프 불가는 서버에서 Ack Error 를 내려줌
	{
		// 시네마틱 플레이 중이면?
		if (true == UGsSequencePlayer::IsSequencePlaying())
		{
			FText findText;
			FText::FindText(TEXT("NetText"), TEXT("WARP_ERROR_CINEMATIC_CANCLE"), findText);
			FGsUIHelper::TrayMessageTicker(findText);
			return false;
		}

		// 2023/7/26 PKT - https://jira.com2us.com/jira/browse/CHR-21147
		//				   시련의 전당 인터서버 변경으로 스크롤 사용 금지
		// 2023/12/13 PKT - Arena 진영에 있다면 사용 금지
		if (MapType::ELITE_DUNGEON_1 == GLevel()->GetMapType() || MapType::GUILD_WAR == GLevel()->GetMapType())
		{
			FText findText;
			FText::FindText(TEXT("ItemUIText"), TEXT("ItemUi_Use_Notice_Place"), findText);
			FGsUIHelper::TrayMessageTicker(findText);
			return false;
		}

		if (true == UGsGameObjectUtil::IsInputFlagBlock(EGsKeyMappingInputFlags::INPUT_CONTENTS_WAIT_SERVER_RESPONSE))
		{
			return false;
		}

		FGsPrimitiveInt32 param((int32)ItemCategorySub::WARP_TO_RANDOM_SCROLL);
		GMessage()->GetItemContents().SendMessage(MessageContentItem::USE_WARP_ITEM, &param);

		return true;
	}
	case ItemCategorySub::WARP_TO_AGIT:
	{
		if (false == GGuild()->IsGuild())
		{
			// TEXT: 기사단에 가입되어 있지 않습니다
			FText findText;
			FText::FindText(TEXT("ItemUIText"), TEXT("UI_Ticker_WarpToAgit_NoGuild"), findText);
			FGsUIHelper::TraySectionMessageTicker(EGsNotiSection::EXTSYSTEM, findText);
			return false;
		}

		// 아지트 내부에서 사용 불가
		if (FGsGuildHelper::IsInAgit())
		{
			// TEXT: 해당 아이템을 사용할 수 없는 상태이거나 사용할 수 없는 장소입니다.
			FText findText;
			FText::FindText(TEXT("ItemUIText"), TEXT("ItemUi_Notice_Stunned"), findText);
			FGsUIHelper::TraySectionMessageTicker(EGsNotiSection::EXTSYSTEM, findText);
			return false;
		}

		if (true == UGsSequencePlayer::IsSequencePlaying())
		{
			FText findText;
			FText::FindText(TEXT("NetText"), TEXT("WARP_ERROR_CINEMATIC_CANCLE"), findText);
			FGsUIHelper::TrayMessageTicker(findText);
			return false;
		}		

		if (true == UGsGameObjectUtil::IsInputFlagBlock(EGsKeyMappingInputFlags::INPUT_CONTENTS_WAIT_SERVER_RESPONSE))
		{
			return false;
		}

		FGsPrimitiveInt32 param(static_cast<int32>(ItemCategorySub::WARP_TO_AGIT));
		GMessage()->GetItemContents().SendMessage(MessageContentItem::USE_WARP_ITEM, &param);

		return true;
	}
	case ItemCategorySub::REWARD_BOX:
	{
		// 선택형 상자
		if (IsPickOneTypeRewardItem(InItem))
		{
			// 선택형 상자는 다른 패킷을 사용한다.
			return false;
		}
		// 선택없이 사용하는 상자
		else
		{
			return true;
		}
		break; 
	}
	case ItemCategorySub::FOOD_BUFF:
	{
		CostumItemUseErrorType errorType;
		if (consumItem->IsItemUseLock(errorType))
		{
			switch (errorType)
			{
			case CostumItemUseErrorType::InvdeInvalidTime: 
				FGsUIHelper::TraySectionMessageTickerNetError(PD::Result::INVADE_ERROR_INVALID_TIME);
				break;
			case CostumItemUseErrorType::InvadeInvalidMap: 
				FGsUIHelper::TraySectionMessageTickerNetError(PD::Result::INVADE_ERROR_INVALID_MAP);
				break;
			default:
				break;
			}

			return false;
		}
	}
		return true;
	case ItemCategorySub::ARCAS:
		{
			bool isCharged = FGsPassivityHelper::GetIsCharged(consumItem->GetDetailID());
			if (!isCharged)
			{
				FText text;
				FText::FindText(TEXT("ArcasText"), TEXT("Arcas_Alert_Popup_Text"), text);

				uint64 dbid = InItem->GetDBID();
				ItemCategorySub category = InItem->GetSubCategory();

				FGsUIHelper::PopupYesNo(text, [=](bool isOk) {
					if (isOk)
					{
						FGsNetSendServiceItem::SendUseItem(dbid, category);
					}
					});

				return false;
			}
			
			return true;
		}
	case ItemCategorySub::SPAWN_TREASURE_MONSTER:
		return true;
	case ItemCategorySub::RESET_BONUS_STAT:
	{
		if (UGsUIManager* uiManager = GUI())
		{
			// 이미 열려있는 경우 : 스탯 분배 쪽 진입 후 사용
			if (uiManager->IsActiveWidget(TEXT("PopupCommonItemConsume")))
			{
				uint64 dbid = InItem->GetDBID();
				ItemCategorySub category = InItem->GetSubCategory();
				FGsNetSendServiceItem::SendUseItem(dbid, category);
			}
			// 닫혀있는 경우 : 인벤토리 등 아이템 사용
			else
			{
				if (UGsUIPopupCommonItemConsume* popupCommonItemConsume = Cast<UGsUIPopupCommonItemConsume>(GUI()->OpenAndGetWidget(TEXT("PopupCommonItemConsume"))))
				{
					FText topText;
					FText::FindText(TEXT("ItemUIText"), TEXT("ItemUi_Consume_Notice_Scroll_Reset"), topText);
					FText bottomText;

					uint64 dbid = InItem->GetDBID();
					ItemCategorySub category = InItem->GetSubCategory();

					popupCommonItemConsume->SetDataNotCheckItem(topText, bottomText, InItem->GetTID(), 1,
						[dbid, category](bool InIsOk)
						{
							if (InIsOk)
							{
								FGsNetSendServiceItem::SendUseItem(dbid, category);
							}
						});
				}
			}
		}

		return false;
	}
	case ItemCategorySub::QUEST_REPEAT:
	{
		if (GSQuest()->GetQuestCount(QuestType::REPEAT) >= MAX_ACTIVE_QUEST_REPEAT_COUNT)
		{
			// 반복 퀘스트 동시 수행 횟수 초과
			FText findText;
			FText::FindText(TEXT("QuestUIText"), TEXT("ExceedRepeat"), findText);
			FGsUIHelper::TrayMessageTicker(findText);

			return false;
		}

		if (nullptr != InItem)
			GSQuest()->SetQuestRepeatUseItemTlbId(InItem->GetDBID());

		return true;
	}
	case  ItemCategorySub::PVP_ALIGNMENT:
		return true;

	case ItemCategorySub::SUMMON_FAIRY:
	case ItemCategorySub::SUMMON_COSTUME:
	{
		return InnerCheck_IsValidSummonItem(InItem);
	}
	case ItemCategorySub::CHEAT:
	{
		FString cheatCode = InItem->GetToolTipDesc().ToString();
		GSLOG(Warning, TEXT("[CheckItemCondition] - ItemCategorySub::CHEAT - ToolTipDesc : %s"), *cheatCode);
		if (true == cheatCode.IsEmpty())
			return false;

		FGsNetSendService::SendWorldAdminCommandQA(cheatCode);
		return true;
	}
	case ItemCategorySub::GUILD_GIFT_BOX:
	{
		return true;
	}
	case ItemCategorySub::MONSTER_KNOWLEDGE_SLATE_RESET:
	{	// 2021/12/22 PKT - 몬스터 도감 전용 아이템(스탯 초기화)
		const FGsPrimitiveUInt64 message(InItem->GetDBID());
		GMessage()->GetMonsterKnowledgeCollection().SendMessage(MessageContentMonsterKnowledge::USE_ITEM_SLATE_RESET, &message);
		return false;
	}
	case ItemCategorySub::DUNGEON_TIME:
		return true;
	case ItemCategorySub::ELITE_DUNGEON_TICKET:
	case ItemCategorySub::ELITE_DUNGEON_BALL:
	{
		if (MapType::ELITE_DUNGEON_1 != GLevel()->GetMapType())
		{	// 2022/08/05 PKT - "해당 아이템을 사용할 수 없는 장소입니다."
			FText message;
			FText::FindText(TEXT("ItemUIText"), TEXT("ItemUi_Use_Notice_Place"), message);
			FGsUIHelper::PopupOK(message);
			return false;
		}

		return true;
	} 
	case ItemCategorySub::TICKET_CHARACTER_SLOT_UNLOCK:
		return true;
	case ItemCategorySub::DELIVERY_BOX_EQUIP:
	case ItemCategorySub::DELIVERY_BOX_FAIRY:
	case ItemCategorySub::DELIVERY_BOX_COSTUME:
	{
		_itemDeliveryBoxData.SetUsedBoxItemDBId(InItem->GetDBID());
		return true;
	}
	case ItemCategorySub::INVADE_PORTAL: //effectType 은 체크할 필요 없다
	{
		if (GGameData()->IsInvadeWorld())
		{
			FText findText;
			FText::FindText(TEXT("InvadeText"), TEXT("Invade_Cant_Spawn_01"), findText);
			FGsUIHelper::TrayMessageTicker(findText);
			return false;
		}
		const FGsSchemaMapData* mapData = GLevel()->GetCurrentMapInfo();
		if (nullptr != mapData)
		{
			if (false == GSInterServer()->GetInvadeMapData(mapData->id).IsValid())
			{
				FText findText;
				FText::FindText(TEXT("NetText"), TEXT("INVADE_ERROR_INVALID_MAP"), findText);
				FGsUIHelper::TrayMessageTicker(findText);
				return false;
			}
		}
		return true;
	}
	case ItemCategorySub::TICKET_CHARACTER_CUSTOMIZING:	
	{
		return true;
	}
	case ItemCategorySub::BONUS_STAT_POINT:
	{
		if (consumItem->GetEffectType() == ItemEffectType::BONUS_STAT_POINT)
		{
			if (const FGsNetUserData* userData = GGameData()->GetUserData())
			{
				int32 usedCount = userData->GetUsedBonusStatItemCount(); // 사용한 수량
				int32 maxCount = FGsStatHelper::GetMaxBonusStatItemCount(); // 최대 사용가능 수량
				if (usedCount < maxCount)
				{
					return true;
				}
				else
				{
					FText findText;
					FText::FindText(TEXT("StatText"), TEXT("Notice_BonusStatItem_MaxConsume"), findText);
					FGsUIHelper::TraySectionMessageTicker(EGsNotiSection::EXTSYSTEM, findText, TrayCondition::TC_Close);
					return false;
				}
			}
		}
		break;
	}
	case ItemCategorySub::DEATH_PENALTY_RECOVERY:
	{
		return true;
	}
	break;
	case ItemCategorySub::NICKNAME_CHANGE:
	{
		FGsPrimitiveUInt64 param(InItem->GetDBID());
		GMessage()->GetItemContents().SendMessage(MessageContentItem::CHANGE_NICKNAME_USE_TICKET, &param);
		return false;
	}
	break;
	case ItemCategorySub::SKILL_SCROLL:
	{
		return true;
	}break;
	}

	return false;
}

void UGsItemManager::TryEquipItem(const FGsItem* In_Item)
{
	if (0 >= In_Item->GetDBID())
		return;

	if (false == IsSameWeaponType(In_Item))
		return;

	if (false == _isRecvSameEquipTypeProcPacket)
		return;	

	const FGsItemEquipment* equipData = static_cast<const FGsItemEquipment*>(In_Item);
	if (nullptr == equipData)
		return;
	
	if (false == _accSlotTypeContainer.Contains(In_Item->GetSubCategory()))
		return;

	// 장착하려는 아이템의 슬롯 정보를 가져옴
	uint8 itemEquipSlotIndex = equipData->GetCurrentPresetSlotIndex();
	TMap<uint8, EGsEquipSlotType> data = _accSlotTypeContainer[In_Item->GetSubCategory()];
	if (false == data.Contains(itemEquipSlotIndex))
		return;

	EGsEquipSlotType slotType = data[itemEquipSlotIndex];
	if (false == _presetEquipItemContainer.Contains(_userPresetBitId))
		return;

	const TMap<EGsEquipSlotType, uint64>& equipItemContainer = _presetEquipItemContainer[_userPresetBitId];
	if (false == equipItemContainer.Contains(slotType))
		return;

	// 멀티 슬롯 여부에 따라 분기 처리
	if (true == IsMultiSlotItem(In_Item->GetSubCategory()))
	{
		uint64 oldDBId_left = GetEquipedItemDBID(In_Item->GetSubCategory(), 0);
		uint64 oldDBId_right = GetEquipedItemDBID(In_Item->GetSubCategory(), 1);

		// 선택되어진 아이템이 현재 착용중인 아이템이라면 Detache 처리
		if (oldDBId_left == In_Item->GetDBID() || oldDBId_right == In_Item->GetDBID())
		{
			uint64 oldDBID = (oldDBId_left == In_Item->GetDBID()) ? oldDBId_left : oldDBId_right;
			uint8 slotIndex = (oldDBId_left == In_Item->GetDBID()) ? 0 : 1;

			DetachEquipItem(oldDBID, slotIndex);
			return;
		}

		// 해당 멀티 슬롯이 모두 장착중인 상태
		if (0 < oldDBId_left && 0 < oldDBId_right)
		{
			FText findText;
			FText::FindText(TEXT("ItemUIText"), TEXT("ItemUi_Equip_Warning1"), findText);
			FGsUIHelper::TrayMessageTicker(findText);
			return;
		}
		
		uint32 oldTID_left = GetTIDByDBID(oldDBId_left);
		uint32 oldTID_right = GetTIDByDBID(oldDBId_right);
		// 이름(TID)이 동일한 장비를 장착중인 상태
		if (oldTID_left == In_Item->GetTID() ||oldTID_right == In_Item->GetTID())
		{
			FText findText;
			FText::FindText(TEXT("ItemUIText"), TEXT("ItemUi_Equip_Warning2"), findText);
			FGsUIHelper::TrayMessageTicker(findText);
			return;
		}

		int32 oldItemGroupIdLeft = GetItemGroupByDBID(oldDBId_left);
		int32 oldItemGroupIdRight = GetItemGroupByDBID(oldDBId_right);

		// 이름(ItemGroupId)이 동일한 장비를 장착중인 상태
		if (In_Item->GetItemGroupId() != INVALID_ITEM_ID &&
			(In_Item->GetItemGroupId() == oldItemGroupIdLeft ||	In_Item->GetItemGroupId() == oldItemGroupIdRight))
		{
			FText findText;
			FText::FindText(TEXT("ItemUIText"), TEXT("ItemUi_Equip_Warning2"), findText);
			FGsUIHelper::TrayMessageTicker(findText);
			return;
		}

		// 이름(equipGroupId)이 동일한 장비를 장착중인 상태(귀속, 비귀속, 상점 등)
		int32 oldEquipGroupId_left = GetEquipGroupIdByDBID(oldDBId_left);
		int32 oldEquipGroupId_right = GetEquipGroupIdByDBID(oldDBId_right);
		if (equipData->GetEquipGroupId() != 0 &&
			(equipData->GetEquipGroupId() == oldEquipGroupId_left ||	equipData->GetEquipGroupId() == oldEquipGroupId_right))
		{
			FText findText;
			FText::FindText(TEXT("ItemUIText"), TEXT("ItemUi_Equip_Warning3"), findText);
			FGsUIHelper::TrayMessageTicker(findText);
			return;
		}

		// 왼쪽부터 장착
		uint8 slotIndex = (0 == oldDBId_left) ? 0 : 1;
		AttachEquipItem(In_Item->GetDBID(), 0, slotIndex);
	}
	else
	{
		uint64 oldDBID = equipItemContainer[slotType];
		// 선택되어진 아이템이 현재 착용중인 아이템이라면 Detache 처리
		if (oldDBID == In_Item->GetDBID())
		{
			DetachEquipItem(oldDBID, 0);
			return;
		}
		else // 장착(or 교체) 처리
		{
			AttachEquipItem(In_Item->GetDBID(), oldDBID, 0);
			return;
		}
	}

	//GSLOG(Warning, TEXT("oldDBID : %llu"), oldDBID);
	//GSLOG(Warning, TEXT("newDBID : %llu"), In_Item->GetDBID()());
}

bool UGsItemManager::IsIngredientUseItem(const FGsItem* In_Item)
{
	if (In_Item->IsIngredient())
	{
		const FGsItemIngredient* ingredientItem = static_cast<const FGsItemIngredient*>(In_Item);
		if (nullptr != ingredientItem)
		{
			return ingredientItem->IsUseItem();
		}
	}
	return false;
}

bool UGsItemManager::IsMultiSlotItem(const ItemCategorySub InSubCategory)
{
	switch (InSubCategory)
	{
	case ItemCategorySub::EAR_RING:
	case ItemCategorySub::RING:
	case ItemCategorySub::CHARM:
	case ItemCategorySub::BRACELET:
	case ItemCategorySub::MARBLE:
		return true;

	default:
		return false;
	}

	return false;
}

uint64 UGsItemManager::GetEquipedItemDBIDBySlotType(const EGsEquipSlotType InSlotType)
{
	return GetEquipedItemDBIDBySlotType(_userPresetId, InSlotType);
}

uint64 UGsItemManager::GetEquipedItemDBIDBySlotType(const PresetId InPresetId, const EGsEquipSlotType InSlotType)
{
	PresetValue presetId = FGsPresetHelper::GetPresetValueFromId(InPresetId);
	if (false == _presetEquipItemContainer.Contains(presetId))
		return INVALID_ITEM_DB_ID;

	if (false == _presetEquipItemContainer[presetId].Contains(InSlotType))
		return INVALID_ITEM_DB_ID;

	return _presetEquipItemContainer[presetId][InSlotType];
}

uint64 UGsItemManager::GetEquipedItemDBID(const ItemCategorySub InSubCategory, const uint8 InSlotIndex)
{
	if (false == _accSlotTypeContainer.Contains(InSubCategory))
		return 0;

	// 장착하려는 아이템의 슬롯 정보를 가져옴
	TMap<uint8, EGsEquipSlotType> data = _accSlotTypeContainer[InSubCategory];
	if (false == data.Contains(InSlotIndex))
		return 0;

	return GetEquipedItemDBIDBySlotType(data[InSlotIndex]);
}

// 장착하려는 weapon 이 현재 착용중인 weapon 타입과 같지 않다면,
// 현재 장착중인 weapon 이 nullptr 일수도 있음..(미장착 상태)
bool UGsItemManager::IsSameWeaponType(const FGsItem* In_Item)
{
	if (false == _isRecvDiffEquipTypeProcPacket)
		return false;

	if (ItemCategoryMain::WEAPON != In_Item->GetMainCategory())
		return true;

	// 무기 변경 쿨타임 확인
	if (GCoolTime()->IsCoolTime(EGsCoolTime::Change_Weapon, 0, 0))
	{
		const FGsItemEquipment* targetEquipItem = static_cast<const FGsItemEquipment*>(In_Item);
		TWeakPtr<FGsItem> item = GetPlayerEquipedItemBySlotType(EGsEquipSlotType::WEAPON_TYPE);
		if (item.IsValid())
		{
			FGsItemEquipment* curEquipItem = item.Pin()->GetCastItem<FGsItemEquipment>();
			CreatureWeaponType type = curEquipItem->GetCreatureWeaponType();
			if (targetEquipItem->GetCreatureWeaponType() != type)
			{
				FText findText;
				FText::FindText(TEXT("ItemUIText"), TEXT("Notice_WeaponChangeCooltime"), findText);
				FGsUIHelper::TrayMessageTicker(findText);
				return false;
			}
		}
		else
		{
			if (targetEquipItem->GetCreatureWeaponType() != _loginCreatureWeaponType)
			{
				FText findText;
				FText::FindText(TEXT("ItemUIText"), TEXT("Notice_WeaponChangeCooltime"), findText);
				FGsUIHelper::TrayMessageTicker(findText);
				return false;
			}
		}
	}

	UGsGameObjectBase* local = GSGameObject()->FindObject(EGsGameObjectType::LocalPlayer);

	// 서버에서 최소 값이 0.5초
	float totalRuntime = 0.6f;
	if (const FGsSchemaSkillCommon* commonSkill = GSkill()->GetPlayerCommonActionData(local, CommonActionType::WEAPON_CHANGE))
	{
		totalRuntime = commonSkill->totalRuntime;
	}

	ItemDBId equippedItemDBID = GetEquipedItemDBIDBySlotType(EGsEquipSlotType::WEAPON_TYPE);
	if (0 >= equippedItemDBID) 
	{ // 현재 미장착 상태 - 장착 요청
		FGsNetSendServiceItem::SendItemEquipChangeWeapon(In_Item->GetDBID(), totalRuntime);
		//_isRecvDiffEquipTypeProcPacket = false;
		return false;
	}
	else
	{ // 현재 장착한 상태 - 교체 혹은 해제 요청
		FGsItem* itemData = GetGsItemLowPointer(FindItem(equippedItemDBID, ItemType::EQUIP));
		if (nullptr != itemData)
		{
			const FGsItemEquipment* inEquipData = static_cast<const FGsItemEquipment*>(In_Item);
			FGsItemEquipment* equippedEquipData = itemData->GetCastItem<FGsItemEquipment>();
			if (inEquipData->GetCreatureWeaponType() != equippedEquipData->GetCreatureWeaponType())
			{
				FGsNetSendServiceItem::SendItemEquipChangeWeapon(In_Item->GetDBID(), totalRuntime);
				//_isRecvDiffEquipTypeProcPacket = false;
				return false;
			}
			// [BY] 이부분은 정리가 필요해 보입니다
			// 현재 같은 타입의 무기인데 변경하려는 것과 착용된 것이 같다면 무기 해제로 판단
			// CS_REQ_ITEM_EQUIP 으로 해제처리는 서버에서 구조에 맞지 않다고 하여 (CS_REQ_ITEM_EQUIP 으로 무기해제하면 안됨)
			// CS_REQ_ITEM_EQUIP_CHANGE_WEAPON_TYPE 패킷으로 동기화
			else if (equippedItemDBID == In_Item->GetDBID())
			{			
				FGsNetSendServiceItem::SendItemEquipChangeWeapon(INVALID_ITEM_DB_ID, 0.f);
				//_isRecvDiffEquipTypeProcPacket = false;

				return false;
			}
			else
			{
				// 같은 WeaponType 이고 다른 DBID인 아이템인 경우 true를 반환하고 이후에
				// TryEquipItem 로직을 통해 처리된다.
			}
		}
	}
	return true;
}

TWeakPtr<FGsItem> UGsItemManager::GetPlayerEquipedItemBySlotType(const EGsEquipSlotType InSlotType)
{
	ItemDBId itemDBId = GetEquipedItemDBIDBySlotType(InSlotType);
	return FindItem(itemDBId, ItemType::EQUIP);
}

void UGsItemManager::SendItemLock(uint64 InItemDbid, bool InLock , ItemStorageType InStorageType)
{
	if (0 < InItemDbid)
	{
		// 상태이상일때 사용 못함
		if (true == IsReactionLockTypeItem(InItemDbid))
		{
			// 스턴 상태일때 불가능 합니다
			FText findText;
			FText::FindText(TEXT("ItemUIText"), TEXT("ItemUi_Notice_Stunned"), findText);
			FGsUIHelper::TrayMessageTicker(findText);
			return;
		}

		FGsNetSendServiceItem::SendItemLockState(InItemDbid, InLock, InStorageType);
	}
}

//////////////////////////////////////// Find Item ///////////////////////////////////////////////////////////////

// ItemTable 에서 Data를 가져옴
const FGsSchemaPartsShapeData* UGsItemManager::FindPartsSchemaDataByTID(int32 InItemTID) const
{
	const FGsSchemaItemEquipDetail* equipDetailData = GetEquipItemTableDataByTID(InItemTID);
	if (nullptr == equipDetailData)
		return nullptr;

	return UGsTableUtil::FindRowById<UGsTablePartsShapeData, FGsSchemaPartsShapeData>(
		equipDetailData->modelPath);
}

TArray<TWeakPtr<FGsItem>> UGsItemManager::FindByTID(const uint32 In_ItemTID, const ItemType In_ItemType)
{
	TArray<TWeakPtr<FGsItem>> ownItemList = _itemBuffers->FindByTID(In_ItemTID, (ItemType::MAX == In_ItemType) ? 
		GetItemTypeByTID(In_ItemTID) : In_ItemType);

	return ownItemList;
}

TArray<TWeakPtr<FGsItem>> UGsItemManager::FindItem(const ItemType In_ItemType, const ItemCategoryMain In_ItemCategoryMain, const ItemCategorySub In_ItemCategorySub)
{
	TArray<TWeakPtr<FGsItem>> ownItemList = _itemBuffers->Find(In_ItemType, In_ItemCategoryMain, In_ItemCategorySub);	
	return ownItemList;
}

TArray<TWeakPtr<FGsItem>> UGsItemManager::FindItem(const ItemType InType, const ItemCategoryMain In_ItemCategoryMain)
{
	TArray<TWeakPtr<FGsItem>> ownItemList = _itemBuffers->Find(InType, In_ItemCategoryMain);	
	return ownItemList;
}

TArray<TWeakPtr<FGsItem>> UGsItemManager::FindItem(const ItemType InType, const ItemCategorySub In_ItemCategorySub)
{
	TArray<TWeakPtr<FGsItem>> ownItemList = _itemBuffers->Find(InType, In_ItemCategorySub);	
	return ownItemList;
}

// Storage(ItemType) 으로 Find
TArray<TWeakPtr<FGsItem>> UGsItemManager::FindItem(const ItemType In_ItemType)
{
	TArray<TWeakPtr<FGsItem>> storageItems = _itemBuffers->FindByItemType(In_ItemType);
	return storageItems;
}

TArray<TWeakPtr<FGsItem>> UGsItemManager::FindItem(const ItemCategorySub InSubCategory)
{
	TArray<TWeakPtr<FGsItem>> findItems = _itemBuffers->Find(InSubCategory);
	return findItems;
}

TWeakPtr<FGsItem> UGsItemManager::FindItem(const uint64 In_ItemDBID , const ItemType In_ItemType)
{
	if (INVALID_ITEM_DB_ID >= In_ItemDBID)
		return nullptr;
	return _itemBuffers->Find(In_ItemDBID, In_ItemType);
}

const TArray<TWeakPtr<FGsItem>> UGsItemManager::FindAllItems()
{
	return _itemBuffers->GetAllItem();
}

TArray<TWeakPtr<FGsItem>> UGsItemManager::FindAllStackableItems()
{
	TArray<TWeakPtr<FGsItem>> resultItemList;
	TArray<ItemType> itemTypes = { ItemType::CONSUME, ItemType::INGREDIENT, ItemType::CURRENCY, ItemType::ETC };
	for (ItemType itemType : itemTypes)
	{
		TArray<TWeakPtr<FGsItem>> items = FindItem(itemType);
		for (int32 i = 0; i < items.Num(); ++i)
		{
			if(false == items[i].IsValid())
				continue;

			if (true == items[i].Pin()->IsStackable())
			{
				resultItemList.Emplace(items[i]);
			}
		}
	}

	return resultItemList;
}

TArray<TWeakPtr<FGsItem>> UGsItemManager::FindAllStackableItems(int32 inItemId)
{
	TArray<TWeakPtr<FGsItem>> stackableItemArray = FindAllStackableItems();
	TArray<TWeakPtr<FGsItem>> resultItemList;
	
	for (TWeakPtr<FGsItem> item : stackableItemArray)
	{
		if (item.IsValid() ? item.Pin()->GetTID() == inItemId : false)
		{
			resultItemList.Add(item);
		}
	}

	return resultItemList;
}

// PresetNum 으로 Find
TArray<TWeakPtr<FGsItem>> UGsItemManager::FindItemByPresetNum(ItemType InType , uint8 InPresetNum)
{
	/*if (0 >= _userPresetValue)
	{
		FGsGameDataManager* gameData = GGameData();
		if (nullptr != gameData)
		{
			const FGsNetUserData* userData = gameData->GetUserData();
			_userPresetValue = userData->mUserPresetId;
		}
	}

	uint8 presetNum = (0 >= InPresetNum) ? _userPresetValue : InPresetNum;
	return _itemBuffers->Find(presetNum, InType);*/
	return _itemBuffers->Find(InPresetNum, InType);
	// 이 함수가 쓰이는지 확인해봐야한다.
}

bool UGsItemManager::IsOwnedItem(const uint64 InItemDBID , const ItemType InItemType)
{
	TWeakPtr<FGsItem> findItem = FindItem(InItemDBID, InItemType);
	return true == findItem.IsValid();
}

bool UGsItemManager::IsOwnedItem(const uint64 InItemDBID)
{
	TWeakPtr<FGsItem> findItem = FindItem(InItemDBID);
	return true == findItem.IsValid();
}

bool UGsItemManager::IsOverMaxStackCountByTID(const int32 InItemTID)
{
	TArray<TWeakPtr<FGsItem>> findItemList = FindByTID(InItemTID);
	if (0 < findItemList.Num())
	{
		FGsItem* itemData;
		ItemAmount ownedItemCount = 0;
		for (int32 i = 0; i < findItemList.Num(); ++i)
		{
			itemData = GetGsItemLowPointer(findItemList[i]);
			if(nullptr == itemData)
				continue;
			
			if (false == itemData->IsStackable())
				return false;

			ownedItemCount += itemData->GetAmount();
			if (ownedItemCount > itemData->GetMaxStackCount())
				return true;
		}
	}

	return false;
}

//int32 UGsItemManager::GetBattleConsumeItemCount()
//{
//	TWeakPtr<FGsItem> findItem = GetPlayerEquipedItemBySlotType(EGsEquipSlotType::WEAPON_TYPE);
//	FGsItem* itemData = GetGsItemLowPointer(findItem);
//	if (nullptr == itemData || false == itemData->IsEquipment())
//		return 0;
//
//	ItemCategorySub subCategory = ItemCategorySub::NONE;
//	switch (itemData->GetSubCategory())
//	{
//	case ItemCategorySub::BOW:
//		subCategory = ItemCategorySub::ARROW;
//		break;
//	case ItemCategorySub::WAND:
//		subCategory = ItemCategorySub::MAGIC_STONE;
//		break;
//	}
//
//	if (ItemCategorySub::NONE == subCategory)
//		return 0;
//
//	TArray<TWeakPtr<FGsItem>> itemDataList = FindItem(ItemType::CONSUME, subCategory);
//	if (0 >= itemDataList.Num())
//	{
//		return 0;
//	}
//
//	int32 total = 0;
//	for (uint8 i = 0; i < itemDataList.Num(); ++i)
//	{
//		itemData = GetGsItemLowPointer(itemDataList[i]);
//		if(nullptr == itemData)
//			continue;
//
//		total += itemData->GetAmount();
//	}
//
//	return total;
//}

ItemAmount UGsItemManager::GetAllItemCountByTID(const int32 InItemTID, bool InCheckLock)
{
	ItemAmount ownedItemCount = 0;
	TArray<TWeakPtr<FGsItem>> findItemList = FindByTID(InItemTID);
	if (0 >= findItemList.Num())
		return 0;
	
	FGsItem* itemData;
	if (InCheckLock)
	{
		for (int32 i = 0; i < findItemList.Num(); ++i)
		{
			itemData = GetGsItemLowPointer(findItemList[i]);
			if (nullptr != itemData)
			{
				if (false == itemData->GetLock())
				{
					ownedItemCount += itemData->GetAmount();
				}
			}
		}
	}
	else
	{
		for (int32 i = 0; i < findItemList.Num(); ++i)
		{
			itemData = GetGsItemLowPointer(findItemList[i]);
			if (nullptr != itemData)
			{
				ownedItemCount += itemData->GetAmount();
			}
		}
	}

	return ownedItemCount;
}

ItemAmount UGsItemManager::GetAllItemCountByDBID(const uint64 InItemDBID, bool InCheckLock)
{
	FGsItem* itemData = GetGsItemLowPointer(FindItem(InItemDBID));
	if (nullptr == itemData)
		return 0;

	return GetAllItemCountByTID(itemData->GetTID(), InCheckLock);
}

bool UGsItemManager::IsPairConsumeItemFromCurWeaponType(const FGsItem* InConsumeItem)
{
	if (nullptr == InConsumeItem)
		return false;

	FGsItem* itemData = GetGsItemLowPointer(GetPlayerEquipedItemBySlotType(EGsEquipSlotType::WEAPON_TYPE));
	if (nullptr == itemData)
		return false;

	const FGsItemEquipment* equipItem = itemData->GetCastItem<FGsItemEquipment>();
	if (nullptr == equipItem)
		return false;

	switch (equipItem->GetCreatureWeaponType())
	{
	case CreatureWeaponType::BOW:
		return ItemCategorySub::ARROW == InConsumeItem->GetSubCategory();
	case CreatureWeaponType::WAND:
		return ItemCategorySub::MAGIC_STONE == InConsumeItem->GetSubCategory();
	case CreatureWeaponType::SWORD:
		return false;
	case CreatureWeaponType::DAGGER:
		return false;
	case CreatureWeaponType::HAND:
		return false;
	case CreatureWeaponType::ORB:
		return false;
	}

	return false;
}

void UGsItemManager::UpdateInvenSlotValue(const TArray<ItemDbIdSortPair>& InItemList)
{ 
	if (0 >= InItemList.Num())
		return;

	for (const ItemDbIdSortPair& iter : InItemList)
	{
		TWeakPtr<FGsItem> findItem = FindItem(iter.mItemDBId);
		if (findItem.IsValid())
		{
			findItem.Pin()->SetInvenSortValue(iter.mInventorySortValue);

			//GSLOG(Error, TEXT("[UpdateInvenSlotValue] - ItemName : %s"), *findItem.Pin()->GetName().ToString());
			//GSLOG(Error, TEXT("[UpdateInvenSlotValue] - GetInvenSortValue : %d"), findItem.Pin()->GetInvenSortValue());
		}
	}
	GMessage()->GetInventoryEvent().SendMessage(MessageContentInven::UPDATE_INVEN_SLOT_INDEX, nullptr);
}

void UGsItemManager::UpdateItemAmountByDBID(const uint64 InDBID, const ItemAmount InAmount)
{
	FGsItem* itemData = GetGsItemLowPointer(FindItem(InDBID));
	if (itemData)
	{
		// 순서 변경 ㄴㄴ
		bool isNewFlag = InAmount > itemData->GetAmount();
		itemData->SetIsNew(isNewFlag);
		itemData->SetAmount(InAmount);

		SendUpdateMessage(itemData->GetDBID());
		if (true == IsPairConsumeItemFromCurWeaponType(itemData))
		{
			GMessage()->GetUI().SendMessage(MessageUI::INVALIDATE_SLOT_AQUIRE_ITEM, nullptr);
		}

		if (true == isNewFlag)
		{
			GMessage()->GetHudRedDot().SendMessage(MessageContentHUDRedDot::INVEN, true);
		}
	}
}

void UGsItemManager::UpdateItemLockByDBID(const uint64 InDBID, const bool InLock , const ItemStorageType InStorageType)
{
	bool isInvenItem = ItemStorageType::INVENTORY == InStorageType;
	FGsItem* finditem = true == isInvenItem ? GetGsItemLowPointer(FindItem(InDBID)) : GetGsItemLowPointer(GSDepot()->FindDepotItemByDBID(InDBID));
	if (nullptr != finditem)
	{
		finditem->SetLock(InLock);
		if (true == isInvenItem)
		{
			SendUpdateMessage(finditem->GetDBID());
		}
		else
		{
			SendUpdateMessage(finditem->GetDBID(), InStorageType);

			FGsUpdateDepotItem param(finditem->GetDBID());
			GMessage()->GetItemContents().SendMessage(MessageContentItem::DEPOT_INVALIDATE, &param);
		}

		FText findText;
		if (true == InLock)
		{
			FText::FindText(TEXT("ItemUIText"), TEXT("ItemUi_Lock_On_Desc"), findText);
		}
		else
		{
			FText::FindText(TEXT("ItemUIText"), TEXT("ItemUi_Lock_Off_Desc"), findText);
		}
		FGsUIHelper::TrayMessageTicker(findText);
	}
}

void UGsItemManager::UsedItemEffect(const ItemId InItemId)
{
	if (INVALID_ITEM_ID == InItemId)
	{
		GSLOG(Error, TEXT("INVALID_ITEM_ID == InItemId"));
		return;
	}

	const FGsSchemaItemCommon* ItemCommon = FGsItem::GetTableDataByID(InItemId);
	if (nullptr == ItemCommon)
	{
		GSLOG(Error, TEXT("nullptr == ItemCommon, InItemId:%d"), InItemId);
		return;
	}

	UGsGameObjectManager* mgr = GSGameObject();
	if (nullptr == mgr)
	{
		return;
	}

	UGsGameObjectBase* objBase = mgr->FindObject(EGsGameObjectType::LocalPlayer);
	if (nullptr == objBase)
	{
		return;
	}

	if (UGsGameObjectPlayer* player = Cast<UGsGameObjectPlayer>(objBase))
	{
		// 카테고리별로 아이템 사용 이펙트 출력
		player->OnUseItem(ItemCommon->categorySub);
	}

	switch (ItemCommon->categorySub)
	{
	case ItemCategorySub::FOOD_BUFF:
	{
		// 음식 버프 사용했을때 출력되는 티커인데
		// 추후 들어가것 같아서 주석처리
		/*
		UGsGameObjectManager* mgr = GSGameObject();
		if (nullptr == mgr)
			return;

		UGsGameObjectBase* objBase = mgr->FindObject(EGsGameObjectType::LocalPlayer);
		if (nullptr == objBase)
			return;

		UGsGameObjectPlayer* player = Cast<UGsGameObjectPlayer>(objBase);
		if (nullptr == player)
			return;

		if (FGsAbnormalityHandlerBase* abnormality = player->GetAbnormalityHandler())
		{
			FGsItemConsumable* consumData = finditem->GetCastItem<FGsItemConsumable>();
			if (nullptr == consumData)
				return;

			if (true == consumData->IsOverlabUseBuffItem(abnormality))
			{
				FText findText;
				FText::FindText(TEXT("ItemUIText"), TEXT("ItemUi_Food_Overlap"), findText);
				FGsUIHelper::TrayMessageTicker(findText);
			}
		}
		*/
		break;
	}
	}
}

void UGsItemManager::UsedItemEffect(const uint64 InDBID)
{
	FGsItem* itemData = GetGsItemLowPointer(FindItem(InDBID, ItemType::CONSUME));
	if (itemData)
	{
		UGsGameObjectManager* mgr = GSGameObject();
		if (nullptr == mgr)
			return;

		UGsGameObjectBase* objBase = mgr->FindObject(EGsGameObjectType::LocalPlayer);
		if (nullptr == objBase)
			return;

		if (UGsGameObjectPlayer* player = Cast<UGsGameObjectPlayer>(objBase))
		{
			// 카테고리별로 아이템 사용 이펙트 출력
			player->OnUseItem(itemData->GetSubCategory());
		}

		switch (itemData->GetSubCategory())
		{
		case ItemCategorySub::FOOD_BUFF:
		{
			// 음식 버프 사용했을때 출력되는 티커인데
			// 추후 들어가것 같아서 주석처리
			/*
			UGsGameObjectManager* mgr = GSGameObject();
			if (nullptr == mgr)
				return;

			UGsGameObjectBase* objBase = mgr->FindObject(EGsGameObjectType::LocalPlayer);
			if (nullptr == objBase)
				return;

			UGsGameObjectPlayer* player = Cast<UGsGameObjectPlayer>(objBase);
			if (nullptr == player)
				return;

			if (FGsAbnormalityHandlerBase* abnormality = player->GetAbnormalityHandler())
			{
				FGsItemConsumable* consumData = finditem->GetCastItem<FGsItemConsumable>();
				if (nullptr == consumData)
					return;

				if (true == consumData->IsOverlabUseBuffItem(abnormality))
				{
					FText findText;
					FText::FindText(TEXT("ItemUIText"), TEXT("ItemUi_Food_Overlap"), findText);
					FGsUIHelper::TrayMessageTicker(findText);
				}
			}
			*/
			break;
		}
		}
	}
}

void UGsItemManager::SendItemEventUI(const ItemId InItemId)
{
	if (INVALID_ITEM_ID == InItemId)
	{
		GSLOG(Error, TEXT("INVALID_ITEM_ID == InItemId"));
		return;
	}

	const FGsSchemaItemCommon* ItemCommon = FGsItem::GetTableDataByID(InItemId);
	if (nullptr == ItemCommon)
	{
		GSLOG(Error, TEXT("nullptr == ItemCommon, InItemId:%d"), InItemId);
		return;
	}

	if (ItemType::CONSUME != ItemCommon->type && ItemType::INGREDIENT != ItemCommon->type)
	{
		return;
	}

	int32 groupID = 0;
	float cooldownTime = 0.f;
	if (ItemType::CONSUME == ItemCommon->type)
	{
		const UGsTableItemConsumeDetail* TableItemConsumeDetail = Cast<UGsTableItemConsumeDetail>(FGsSchemaItemConsumeDetail::GetStaticTable());
		if (nullptr == TableItemConsumeDetail)
		{
			GSLOG(Error, TEXT("nullptr == TableItemConsumeDetail"));
			return;
		}

		const FGsSchemaItemConsumeDetail* ItemConsumeDetail = nullptr;
		if (!TableItemConsumeDetail->FindRowById(ItemCommon->detailId, ItemConsumeDetail))
		{
			GSLOG(Error, TEXT("!TableItemConsumeDetail->FindRowById(%d, ItemConsumeDetail)"), ItemCommon->detailId);
			return;
		}

		groupID = ItemConsumeDetail->cooldownGroupId;
		cooldownTime = ItemConsumeDetail->cooldownTime;
	}
	else
	{
		const UGsTableItemIngredientDetail* TableItemDetail = Cast<UGsTableItemIngredientDetail>(FGsSchemaItemIngredientDetail::GetStaticTable());
		if (nullptr == TableItemDetail)
		{
			GSLOG(Error, TEXT("nullptr == TableItemDetail"));
			return;
		}

		const FGsSchemaItemIngredientDetail* ItemDetail = nullptr;
		if (!TableItemDetail->FindRowById(ItemCommon->detailId, ItemDetail))
		{
			GSLOG(Error, TEXT("!TableItemDetail->FindRowById(%d, ItemDetail)"), ItemCommon->detailId);
			return;
		}
		groupID = ItemDetail->cooldownGroupId;
		cooldownTime = ItemDetail->cooldownTime;
	}

	if (0.0f < cooldownTime)
	{
		GCoolTime()->SetCoolTime(EGsCoolTime::Item, groupID, 0, cooldownTime);

		FGsCoolTimeMessageParam messageParam(EGsCoolTime::Item, groupID, 0, cooldownTime);
		GMessage()->GetGameObject().SendMessage(MessageGameObject::LOCAL_COOLTIME_START, &messageParam);
	}

	GMessage()->GetContentsHud().SendMessage(MessageContentHud::INVALIDATE_CONSUMEABLE);
}

void UGsItemManager::SendItemEventUI(const uint64 InDBID)
{
	if (0 >= InDBID)
		return;

	FGsItem* itemData = GetGsItemLowPointer(FindItem(InDBID));
	if (nullptr == itemData)
		return;

	if (itemData->IsCooldownTimeItem())
	{
		FGsMessageHolder* msg = GMessage();
		FGsCoolTimeManager* coolTimeMgr = GCoolTime();

		int32 groupID = itemData->GetCooldownGroupID();
		if (false == coolTimeMgr->IsCoolTime(EGsCoolTime::Item, groupID, 0))
		{
			float maxCoolTime = itemData->GetMaxCoolTime();
			GCoolTime()->SetCoolTime(EGsCoolTime::Item, groupID, 0, maxCoolTime);
			FGsCoolTimeMessageParam messageParam(EGsCoolTime::Item, groupID, 0, maxCoolTime);
			msg->GetGameObject().SendMessage(MessageGameObject::LOCAL_COOLTIME_START, &messageParam);
			msg->GetContentsHud().SendMessage(MessageContentHud::INVALIDATE_CONSUMEABLE);
		}
	}
}

void UGsItemManager::SendItemEventUI(TArray<ItemDBId> InItemDbidList)
{
	FGsMessageHolder* msg = GMessage();
	FGsCoolTimeManager* coolTimeMgr = GCoolTime();
	for (const ItemDBId iter : InItemDbidList)
	{
		if (0 >= iter)
			return;

		FGsItem* itemData = GetGsItemLowPointer(FindItem(iter));
		if (nullptr == itemData)
			return;

		if (itemData->IsCooldownTimeItem())
		{
			int32 groupID = itemData->GetCooldownGroupID();
			if (false == coolTimeMgr->IsCoolTime(EGsCoolTime::Item, groupID, 0))
			{
				float maxCoolTime = itemData->GetMaxCoolTime();
				coolTimeMgr->SetCoolTime(EGsCoolTime::Item, groupID, 0, maxCoolTime);
				FGsCoolTimeMessageParam messageParam(EGsCoolTime::Item, groupID, 0, maxCoolTime);
				msg->GetGameObject().SendMessage(MessageGameObject::LOCAL_COOLTIME_START, &messageParam);
				msg->GetContentsHud().SendMessage(MessageContentHud::INVALIDATE_CONSUMEABLE);
			}
		}
	}
}

void UGsItemManager::RemoveItem(const uint64 InItemDBID)
{
	FGsItem* itemData = GetGsItemLowPointer(FindItem(InItemDBID));
	if (nullptr != itemData)
	{
		ItemId itemTID = itemData->GetTID();
		const Level currentLevel = itemData->GetLevel();
		bool isPairItem = IsPairConsumeItemFromCurWeaponType(itemData);
		itemData->SetIsNew(false);

		if (itemData->IsPresetEquipped())
		{
			// 프리셋 정보 제거
			for (auto& equipItemContainer : _presetEquipItemContainer)
			{
				for (auto& slotDBIdData : equipItemContainer.Value)
				{
					if (slotDBIdData.Value == InItemDBID)
					{
						equipItemContainer.Value[slotDBIdData.Key] = INVALID_ITEM_DB_ID;
					}
				}
			}
		}
		_itemBuffers->RemoveItem(itemData);

		if (_curHudSpiritShotDBID == InItemDBID)
		{
			// 천마석 상태 갱신
			PickHudSpiritShotItem();
		}

		FGsRemoveItem paramData(InItemDBID, itemTID, currentLevel);
		GMessage()->GetItem().SendMessage(MessageItem::REMOVEITEM, paramData);
		GMessage()->GetContentsHud().SendMessage(MessageContentHud::INVALIDATE_CONSUMEABLE);

		if (true == isPairItem)
		{
			GMessage()->GetUI().SendMessage(MessageUI::INVALIDATE_SLOT_AQUIRE_ITEM, nullptr);
		}
	}
}

void UGsItemManager::RemoveItemList(const TArray<ItemDBId>& InItemDBIdList)
{
	for (const ItemDBId ItemDBIdDeleted : InItemDBIdList)
	{
		RemoveItem(ItemDBIdDeleted);
	}

	FGsRemoveItemList param(InItemDBIdList);
	GMessage()->GetItem().SendMessage(MessageItem::REMOVEITEMS, param);
}

const bool UGsItemManager::IsMatchCategorySubByItemTID(const int32 InItemTID, const ItemCategorySub InCategorySub)
{
	const FGsSchemaItemCommon* itemData = GetItemTableDataByTID(InItemTID);
	if (nullptr == itemData)
		return false;

	return InCategorySub == itemData->categorySub;
}

ItemType UGsItemManager::GetItemTypeByTID(const int32 In_ItemTID)
{
	const FGsSchemaItemCommon* itemData = GetItemTableDataByTID(In_ItemTID);
	if (nullptr != itemData)
	{
		return itemData->type;
	}
	return ItemType::NONE;
}

const FGsSchemaItemCommon* UGsItemManager::GetItemTableDataByTID(int32 In_ItemTID)
{
	const UGsTableItemCommon* table = Cast<UGsTableItemCommon>(FGsSchemaItemCommon::GetStaticTable());
	if (nullptr == table)
	{
		return nullptr;
	}

	const FGsSchemaItemCommon* row = nullptr;
	if (false == table->FindRowById(In_ItemTID, row))
	{
		return nullptr;
	}

	return row;
}

const FGsSchemaItemEquipDetail* UGsItemManager::GetEquipItemTableDataByTID(int32 In_ItemTID)
{
	const FGsSchemaItemCommon* itemData = GetItemTableDataByTID(In_ItemTID);
	if (nullptr != itemData)
	{
		const UGsTableItemEquipDetail* table = Cast<UGsTableItemEquipDetail>(FGsSchemaItemEquipDetail::GetStaticTable());
		if (nullptr == table)
		{
			return nullptr;
		}

		const FGsSchemaItemEquipDetail* row = nullptr;
		if (false == table->FindRowById(itemData->detailId , row))
		{
			return nullptr;
		}
		return row;
	}
	return nullptr;
}

void UGsItemManager::SendUpdateMessage(uint64 InItemDBID, bool InSkipUpdateDetailInfo, bool InIsSendComsumable)
{
	FGsUpdateItem paramData(InItemDBID, InSkipUpdateDetailInfo);
	GMessage()->GetItem().SendMessage(MessageItem::UPDATEITEM, paramData);

	TArray<ItemDBId> itemDBIdList {InItemDBID};
	FGsUpdateItemList updateItemListParam(itemDBIdList);
	GMessage()->GetItem().SendMessage(MessageItem::UPDATEITEMLIST, updateItemListParam);

	if (InIsSendComsumable)
	{
		GMessage()->GetContentsHud().SendMessage(MessageContentHud::INVALIDATE_CONSUMEABLE);
	}
}

void UGsItemManager::SendUpdateMessage(uint64 InItemDBID, ItemStorageType InItemStroageType)
{
	FGsUpdateItem paramData(InItemDBID, InItemStroageType);
	GMessage()->GetItem().SendMessage(MessageItem::UPDATEITEM, paramData);

	TArray<ItemDBId> itemDBIdList {InItemDBID};
	FGsUpdateItemList updateItemListParam(itemDBIdList, InItemStroageType);
	GMessage()->GetItem().SendMessage(MessageItem::UPDATEITEMLIST, updateItemListParam);
}

void UGsItemManager::SendUpdateMessage(TArray<FGsItem*> InItemDataList, bool InSkipUpdateDetailInfo)
{
	if (0 >= InItemDataList.Num())
		return;

	TArray<ItemDBId> itemDbidList;
	for (FGsItem* itemData : InItemDataList)
	{
		if (nullptr == itemData)
			continue;

		itemDbidList.Emplace(itemData->GetDBID());

		FGsUpdateItem paramData(itemData->GetDBID(), InSkipUpdateDetailInfo);
		GMessage()->GetItem().SendMessage(MessageItem::UPDATEITEM, paramData);
	}

	if (FGsMessageHolder* msg = GMessage())
	{
		FGsUpdateItemList paramData(itemDbidList);
		msg->GetItem().SendMessage(MessageItem::UPDATEITEMLIST, paramData);
		msg->GetContentsHud().SendMessage(MessageContentHud::INVALIDATE_CONSUMEABLE);
	}
}

void UGsItemManager::AllOffItemRedDotByType(const ItemType InType)
{
	FGsItem* itemData;
	TArray<TWeakPtr<FGsItem>> findItemList = FindItem(InType);
	for (uint16 i = 0; i < findItemList.Num(); ++i)
	{
		itemData = GetGsItemLowPointer(findItemList[i]);
		if(nullptr == itemData)
			continue;

		if(false == itemData->IsNew())
			continue;

		itemData->SetIsNew(false);
	}
}

bool UGsItemManager::CheckRedDotItemByItemType(const ItemType InType)
{
	TArray<TWeakPtr<FGsItem>> findItemList = FindItem(InType);
	for(TWeakPtr<FGsItem> iter : findItemList)
	{
		if(false == iter.IsValid())
			continue;

		if (false == iter.Pin()->IsNew())
			continue;

		return true;
	}
	return false;
}

/////////////////////////////// For Test ////////////////////////////////////////
void UGsItemManager::PrintItemListLog()
{
	_itemBuffers->PrintItemListLog();
}

void UGsItemManager::PrintEquipedItemListLog() const
{
	for (const auto iter : _presetEquipItemContainer[_userPresetBitId])
	{
		if(0 >= iter.Value)
			continue;

		GSLOG(Warning, TEXT("[PrintEquipedItemListLog] ItemSlotTypeNum : %d"), iter.Key);
		GSLOG(Warning, TEXT("[PrintEquipedItemListLog] Item DBID : %llu"), iter.Value);
	}
}

const uint16 UGsItemManager::GetOwnedItemCount()
{
	return _itemBuffers->GetOwnedItemCount();
}

const int32 UGsItemManager::GetOwnedItemWeight()
{
	return _itemBuffers->GetOwnedItemWeight();
}

/////////////////////////////// Sorting Type ////////////////////////////////////////
void UGsItemManager::SortItemListByDBID(uint8 InSortTypeIndex , OUT TArray<uint64>& outDBIDList)
{
	FGsItem* itemData;
	TArray<FGsItem*> itemList;
	itemList.Empty();
	for (int32 i = 0; i < outDBIDList.Num(); ++i)
	{
		itemData = GetGsItemLowPointer(FindItem(outDBIDList[i]));
		if (nullptr == itemData)
			continue;

		itemList.Emplace(itemData);
	}

	switch (InSortTypeIndex)
	{
	case UGsUIPopupInventory::ESortUIList::DefaultType:
		SortDefaultType(itemList);
		break;
	case UGsUIPopupInventory::ESortUIList::GradeType:
		SortGradeType(itemList);
		break;
	case UGsUIPopupInventory::ESortUIList::WeightType:
		SortWeightType(itemList);
		break;
	case UGsUIPopupInventory::ESortUIList::ServerSlotType:
		SortInvenValueIndex(itemList);
		break;
	}
	                                                              
	outDBIDList.Empty();
	for (int32 i = 0; i < itemList.Num(); ++i)
	{
		outDBIDList.Emplace(itemList[i]->GetDBID());
	}
}

// 현재 인벤토리 아이템들의 슬롯 인덱스를 서버에 보내서 저장시킴
void UGsItemManager::TrySendInvenSlotIndex(TArray<ItemDBId> InItemDbidList, const bool InIsNeedRecord)
{
	if (FGsUnlockManager* unlockManager = GSUnlock())
	{
		if (false == unlockManager->IsContentsUnlock(EGsUnlockContentsType::INVENTORY, false))
		{
			return;
		}
	}

	if (0 >= InItemDbidList.Num())
		return;

	if (InIsNeedRecord)
	{
		FGsNetSendServiceItem::SendInventorySortValue(InItemDbidList);
	}
	else
	{
		bool isFind = false;
		for (const uint64& iter : InItemDbidList)
		{
			TWeakPtr<FGsItem> findData = FindItem(iter);
			if (false == findData.IsValid())
				continue;

			if (0 == findData.Pin()->GetInvenSortValue())
			{
				isFind = true;
				break;
			}
		}
		if (isFind)
		{
			FGsNetSendServiceItem::SendInventorySortValue(InItemDbidList);
		}
	}

	/*
	for (const uint64& iter : InItemDbidList)
	{
		TWeakPtr<FGsItem> findData = FindItem(iter);
		if (false == findData.IsValid())
			continue;

		//GSLOG(Error, TEXT("[TrySendInvenSlotIndex] - ItemName : %s"), *findData.Pin()->GetName().ToString());
		//GSLOG(Error, TEXT("[TrySendInvenSlotIndex] - GetInvenSortValue : %d"), findData.Pin()->GetInvenSortValue());
	}
	*/
}

void UGsItemManager::SortInvenValueIndex(OUT TArray<FGsItem*>& outItemList)
{
	if (outItemList.Num() > 1)
	{
		outItemList.Sort([](FGsItem& itemA, FGsItem& itemB)
			{
				// 서버 정렬 인덱스 기준으로 정렬
				return itemB.GetInvenSortValue() < itemA.GetInvenSortValue();
			}
		);
	}
}

void UGsItemManager::SortDefaultType(OUT TArray<FGsItem*>& outItemList)
{
	if (outItemList.Num() > 1)
	{
		outItemList.Sort([](FGsItem& itemA, FGsItem& itemB)
			{
				/*
				//  0. 서버 정렬 인덱스 기준으로 정렬
				int32 diff = itemB.GetInvenSortValue() - itemA.GetInvenSortValue();
				if (0 != diff)
				{
					return itemB.GetInvenSortValue() > itemA.GetInvenSortValue();
				}
				*/

				// 1. 재화형 아이템
				int32 diff = (uint8)itemB.IsCurrency() - (uint8)itemA.IsCurrency();
				if (0 != diff)
				{
					return (uint8)itemB.IsCurrency() < (uint8)itemA.IsCurrency();
				}

				// 2. 장착여부
				bool equiped = itemA.IsEquipped();
				if (equiped != itemB.IsEquipped())
				{
					return equiped ? true : false;
				}

				// 아이템 타입
				diff = (uint8)itemB.GetType() - (uint8)itemA.GetType();
				if (0 != diff)
				{
					return (uint8)itemB.GetType() > (uint8)itemA.GetType();
				}

				// 아이템 카테고리
				diff = (uint8)itemB.GetMainCategory() - (uint8)itemA.GetMainCategory();
				if (0 != diff)
				{
					return (uint8)itemB.GetMainCategory() > (uint8)itemA.GetMainCategory();
				}			

				// 아이템 등급
				diff = (uint8)itemB.GetGradeType() - (uint8)itemA.GetGradeType();
				if (0 != diff)
				{
					return (uint8)itemB.GetGradeType() < (uint8)itemA.GetGradeType();
				}

				// 아이템 세부 카테고리
				diff = (uint8)itemB.GetSubCategory() - (uint8)itemA.GetSubCategory();
				if (0 != diff)
				{
					return (uint8)itemB.GetSubCategory() > (uint8)itemA.GetSubCategory();
				}

				// 아이템 그룹 (비귀속, 귀속, 이벤트, 상점, ... )
				if (itemB.GetItemGroupId() != itemA.GetItemGroupId())
				{
					return itemB.GetItemGroupId() > itemA.GetItemGroupId();
				}

				// 아이템 TID
				diff = itemB.GetTID() - itemA.GetTID();
				if (0 != diff)
				{
					return itemB.GetTID() > itemA.GetTID();
				}

				// 아이템 단계
				diff = itemB.GetLevel() - itemA.GetLevel();
				if (0 != diff)
				{
					return itemB.GetLevel() < itemA.GetLevel();
				}
				
				// 아이템 보유 개수
				if (itemB.GetAmount() != itemA.GetAmount())
				{
					return itemB.GetAmount() < itemA.GetAmount();
				}
				
				// 고유 비교
				return itemB.GetDBID() < itemA.GetDBID();
			}
		);
	}
}

void UGsItemManager::SortGradeType(OUT TArray<FGsItem*>& outItemList)
{
	if (outItemList.Num() > 1)
	{
		outItemList.Sort([](FGsItem& itemA, FGsItem& itemB)
			{
				// 아이템 등급
				int32 diff = (uint8)itemB.GetGradeType() - (uint8)itemA.GetGradeType();
				if (0 != diff)
				{
					return (uint8)itemB.GetGradeType() < (uint8)itemA.GetGradeType();
				}

				diff = (uint8)itemB.IsCurrency() - (uint8)itemA.IsCurrency();
				if (0 != diff)
				{
					return (uint8)itemB.IsCurrency() < (uint8)itemA.IsCurrency();
				}

				bool equiped = itemA.IsEquipped();
				if (equiped != itemB.IsEquipped())
				{
					return equiped ? true : false;
				}

				// 아이템 타입
				diff = (uint8)itemB.GetType() - (uint8)itemA.GetType();
				if (0 != diff)
				{
					return (uint8)itemB.GetType() > (uint8)itemA.GetType();
				}

				// 아이템 카테고리
				diff = (uint8)itemB.GetMainCategory() - (uint8)itemA.GetMainCategory();
				if (0 != diff)
				{
					return (uint8)itemB.GetMainCategory() > (uint8)itemA.GetMainCategory();
				}

				// 아이템 세부 카테고리
				diff = (uint8)itemB.GetSubCategory() - (uint8)itemA.GetSubCategory();
				if (0 != diff)
				{
					return (uint8)itemB.GetSubCategory() > (uint8)itemA.GetSubCategory();
				}

				// 아이템 그룹 (비귀속, 귀속, 이벤트, 상점, ... )
				if (itemB.GetItemGroupId() != itemA.GetItemGroupId())
				{
					return itemB.GetItemGroupId() > itemA.GetItemGroupId();
				}

				// 아이템 TID
				diff = itemB.GetTID() - itemA.GetTID();
				if (0 != diff)
				{
					return itemB.GetTID() > itemA.GetTID();
				}

				// 아이템 단계
				diff = itemB.GetLevel() - itemA.GetLevel();
				if (0 != diff)
				{
					return itemB.GetLevel() < itemA.GetLevel();
				}
				
				// 아이템 보유 개수
				if (itemB.GetAmount() != itemA.GetAmount())
				{
					return itemB.GetAmount() < itemA.GetAmount();
				}

				// 고유 비교
				return itemB.GetDBID() < itemA.GetDBID();
			}
		);
	}
}

void UGsItemManager::SortWeightType(OUT TArray<FGsItem*>& outItemList)
{
	if (outItemList.Num() > 1)
	{
		outItemList.Sort([](FGsItem& itemA, FGsItem& itemB)
			{
				// 아이템 무게
				uint64 calcWeight1 = itemB.GetAmount() * itemB.GetWeight();
				uint64 calcWeight2 = itemA.GetAmount() * itemA.GetWeight();
				int64 diff = calcWeight1 - calcWeight2;
				if (0 != diff)
				{
					return calcWeight1 < calcWeight2;
				}

				diff = (uint8)itemB.IsCurrency() - (uint8)itemA.IsCurrency();
				if (0 != diff)
				{
					return (uint8)itemB.IsCurrency() < (uint8)itemA.IsCurrency();
				}

				// 1. 장착여부
				bool equiped = itemA.IsEquipped();
				if (equiped != itemB.IsEquipped())
				{
					return equiped ? true : false;
				}

				// 아이템 타입
				diff = (uint8)itemB.GetType() - (uint8)itemA.GetType();
				if (0 != diff)
				{
					return (uint8)itemB.GetType() > (uint8)itemA.GetType();
				}

				// 아이템 카테고리
				diff = (uint8)itemB.GetMainCategory() - (uint8)itemA.GetMainCategory();
				if (0 != diff)
				{
					return (uint8)itemB.GetMainCategory() > (uint8)itemA.GetMainCategory();
				}

				// 아이템 등급
				diff = (uint8)itemB.GetGradeType() - (uint8)itemA.GetGradeType();
				if (0 != diff)
				{
					return (uint8)itemB.GetGradeType() < (uint8)itemA.GetGradeType();
				}

				// 아이템 세부 카테고리
				diff = (uint8)itemB.GetSubCategory() - (uint8)itemA.GetSubCategory();
				if (0 != diff)
				{
					return (uint8)itemB.GetSubCategory() > (uint8)itemA.GetSubCategory();
				}

				// 아이템 그룹 (비귀속, 귀속, 이벤트, 상점, ... )
				if (itemB.GetItemGroupId() != itemA.GetItemGroupId())
				{
					return itemB.GetItemGroupId() > itemA.GetItemGroupId();
				}

				// 아이템 TID
				diff = itemB.GetTID() - itemA.GetTID();
				if (0 != diff)
				{
					return itemB.GetTID() > itemA.GetTID();
				}

				// 아이템 단계
				diff = itemB.GetLevel() - itemA.GetLevel();
				if (0 != diff)
				{
					return itemB.GetLevel() < itemA.GetLevel();
				}

				// 아이템 보유 개수
				if (itemB.GetAmount() != itemA.GetAmount())
				{
					return itemB.GetAmount() < itemA.GetAmount();
				}

				// 고유 비교
				return itemB.GetDBID() < itemA.GetDBID();
			}
		);
	}
}

// packet 에서 받는 id list 로 overwrite 함
void UGsItemManager::UpdateReactionItemLock(TArray<int32>& InAffectIdList)
{
	_isLockByReaction = 0 < InAffectIdList.Num();
	GSLOG(Error, TEXT("[UpdateReactionItemLock] - _isLockByReaction : %d"), (uint8)_isLockByReaction);

	_currentDisableAffectItemTypList.Empty();
	if (_isLockByReaction)
	{
		// disable 처리되어야 할 타입들 산출	
		for (const int32 iter : InAffectIdList)
		{
			if (_reactionAffectItemTableDataMap.Contains(iter))
			{
				TArray<ItemCategorySub> typeList = _reactionAffectItemTableDataMap[iter];
				for (const ItemCategorySub subIter : typeList)
				{
					if (INDEX_NONE == _currentDisableAffectItemTypList.Find(subIter))
					{
						_currentDisableAffectItemTypList.Emplace(subIter);
						GSLOG(Error, TEXT("[UpdateReactionItemLock] - _currentDisableAffectItemTypList : %d"), (uint8)subIter);
					}
				}
			}
		}
	}
	FGsReactionItemLock param(_isLockByReaction, _currentDisableAffectItemTypList);
	GMessage()->GetItem().SendMessage(MessageItem::LOCKITEM, param);
}

const bool UGsItemManager::IsReactionLockTypeItem(const ItemCategorySub InTargetItemType)
{
	if (false == _isLockByReaction)
		return false;

	return INDEX_NONE != _currentDisableAffectItemTypList.Find(InTargetItemType);
}

const bool UGsItemManager::IsReactionLockTypeItem(const ItemDBId InTargetItemDbid)
{
	if (false == _isLockByReaction)
		return false;

	TWeakPtr<FGsItem> findItem = FindItem(InTargetItemDbid);
	if (findItem.IsValid())
	{
		return INDEX_NONE != _currentDisableAffectItemTypList.Find(findItem.Pin()->GetSubCategory());
	}
	return false;
}

void UGsItemManager::UpdateWeightPacketData(uint8 InPenaltyType)
{
	UGsInventoryManager* invenMgr = GSInventory();
	if (nullptr == invenMgr)
		return;

	if (_itemWeightPenaltyGradeType != (WeightPenaltyGrade)InPenaltyType)
	{
		_itemWeightPenaltyGradeType = (WeightPenaltyGrade)InPenaltyType;
		if (WeightPenaltyGrade::NONE == _itemWeightPenaltyGradeType)
		{
			GSInventory()->SendPenaltyInfoFromPacketData(_itemWeightPenaltyGradeType);
			GMessage()->GetUI().SendMessage(MessageUI::INVALIDATE_SLOT_AQUIRE_STAT, nullptr);
			return;
		}

		FTextKey textKey = (_itemWeightPenaltyGradeType == WeightPenaltyGrade::FIRST) ?
			TEXT("ItemUi_Weight_Penalty_1stGrade") : TEXT("ItemUi_Weight_Penalty_2ndGrade");

		int32 value = invenMgr->GetOwnedItemPenaltyValueByType(_itemWeightPenaltyGradeType);
		FText findText;
		if (FText::FindText(TEXT("ItemUIText"), textKey, findText))
		{
			FText msg =  FText::Format(findText, value);
			FGsUIHelper::TraySectionMessageTicker(EGsNotiSection::EXTSYSTEM, msg);
		}
		GSInventory()->SendPenaltyInfoFromPacketData(_itemWeightPenaltyGradeType);
		GMessage()->GetUI().SendMessage(MessageUI::INVALIDATE_SLOT_AQUIRE_STAT, nullptr);
	}
}

/*
bool UGsItemManager::IsEquipSound(const ItemDataEquip* InEquipData)
{
	if (nullptr == InEquipData)
		return false;

	TWeakPtr<FGsItem> findItem = FindItem(InEquipData->mItemDBId, ItemType::EQUIP);
	if (findItem.IsValid())
	{
		return INVALID_ITEM_DB_ID < GetEquipedItemDBIDBySlotType(GetEquipSlotType(findItem.Pin()->GetSubCategory(), 
			InEquipData->mItemEquipSlotIndex));
	}
	return false;
}
*/

void UGsItemManager::UpdateEquipItem(TArray<const ItemDataEquip*> InItemsDataEquip)
{
	AddItem(InItemsDataEquip, false);
}

const bool UGsItemManager::IsInvenWeightFull(int32 InAddWeight)
{
	UGsGameObjectBase* local = GSGameObject()->FindObject(EGsGameObjectType::LocalPlayer);
	if (nullptr == local)
		return false;

	FGsGameObjectDataCreature* creatureData = local->GetCastData<FGsGameObjectDataCreature>();
	if (nullptr == creatureData)
		return false;

	const FGsStatInfo* weight = creatureData->GetCreatureStatInfo(StatType::WEIGHT);
	if (nullptr == weight)
		return false;

	int32 maxWeight = weight->GetMaxStatValue();
	int32 ownedItemTotalWeight = GetOwnedItemWeight();

	return (ownedItemTotalWeight + InAddWeight) >= maxWeight;
}

// 인자로 특정 퍼센트 수치를 받아서 해당 수치를 넘어갔다면 true , 아니라면 false 반환
const bool UGsItemManager::IsInvenWeightFull(const float InTargetPerValue)
{
	UGsGameObjectBase* local = GSGameObject()->FindObject(EGsGameObjectType::LocalPlayer);
	if (nullptr == local)
		return false;

	FGsGameObjectDataCreature* creatureData = local->GetCastData<FGsGameObjectDataCreature>();
	if (nullptr == creatureData)
		return false;

	const FGsStatInfo* weight = creatureData->GetCreatureStatInfo(StatType::WEIGHT);
	if (nullptr == weight)
		return false;

	float maxWeight = (float)weight->GetMaxStatValue();
	float ownedItemTotalWeight = (float)GetOwnedItemWeight();
	float calc = (maxWeight / 100.f) * InTargetPerValue;
	if (ownedItemTotalWeight >= calc)
	{
		return true;
	}

	return false;
}

// ItemId 와 ItemAmount 를 Pair 로 받아 아이템 리스트를 추가할 수 있는지 체크
const bool UGsItemManager::IsInvenWeightFull(const TArray<TPair<int32, ItemAmount>>& InitemTIdAmountPairList)
{
	// itemBagList 에 아이템이 없을 경우(재화 타입만 보상으로 주어질 경우) 보상은 언제나 수령 가능
	if (InitemTIdAmountPairList.Num() == 0)
	{
		return false;
	}

	int32 addWeight = 0;
	for (const auto itemTidAmount : InitemTIdAmountPairList)
	{
		const FGsSchemaItemCommon* itemData = FGsItem::GetTableDataByID(itemTidAmount.Key);
		if (nullptr == itemData)
		{
			GSLOG(Error, TEXT("item tid is missing : %d"), itemTidAmount.Key);
			return false;
		}

		addWeight += itemData->weight;
	}

	return IsInvenWeightFull(addWeight);
}

const bool UGsItemManager::IsInvenSlotFull()
{
	uint16 ownedCnt = GetOwnedItemCount();
	return ownedCnt >= GSInventory()->GetInvenSlotMaxCount();
}

const bool UGsItemManager::IsInvenSlotFull(const uint16 InExpectInputSlotCount)
{
	uint16 expectOwnedCnt = InExpectInputSlotCount + GetOwnedItemCount();
	return expectOwnedCnt > GSInventory()->GetInvenSlotMaxCount();
}

const bool UGsItemManager::IsCanItemAdd(int32 inItemTID, ItemAmount inItemAmount)
{	
	const FGsSchemaItemCommon* itemData = GetItemTableDataByTID(inItemTID);
	if (nullptr == itemData)
	{
		GSLOG(Error, TEXT("item tid is missing : %d"), inItemTID);
		return false;
	}
	
	bool isStackable = itemData->maxStackCount > 1;	
	if (isStackable)
	{
		int32 slot = GSInventory()->GetInvenSlotMaxCount() - GetOwnedItemCount();
		TArray<TWeakPtr<FGsItem>> stackableItemArray = FindAllStackableItems(itemData->id);
		ItemAmount itemMaxCount = slot * itemData->maxStackCount;		
		for (TWeakPtr<FGsItem> stackableItem : stackableItemArray)
		{
			if (false == stackableItem.IsValid())
			{
				continue;
			}

			if (stackableItem.Pin()->IsFull())
			{
				continue;
			}

			itemMaxCount += stackableItem.Pin()->GetLeftStackCount();
		}

		//현재 아이템 Amount와 획득할 아이템 갯수의 합이 최대 아이템 갯수보다 작거나 같을 때 true
		return inItemAmount <= itemMaxCount;
	}
	
	//중첩 가능하지 않은 아이템이고 슬롯이 가득 차 있으면 회득 불가능
	return (inItemAmount + GetOwnedItemCount()) <= GSInventory()->GetInvenSlotMaxCount();
}

const bool UGsItemManager::IsCanItemListAdd(const TArray<TPair<int32, ItemAmount>>& InitemTIdAmountPairList)
{
	// itemBagList 에 아이템이 없을 경우(재화 타입만 보상으로 주어질 경우) 보상은 언제나 수령 가능
	if (InitemTIdAmountPairList.Num() == 0)
	{
		return true;
	}

	// 만약 같은 종류의 아이템이 있을 경우 하나로 합친다
	TMap<int32, ItemAmount> itemAmountMap;
	for (const auto& pair : InitemTIdAmountPairList)
	{
		if (itemAmountMap.Contains(pair.Key) == false)
		{
			itemAmountMap.Emplace(pair.Key, pair.Value);
		}
		else
		{
			itemAmountMap[pair.Key] += pair.Value;
		}
	}

	// 추가로 필요한 슬롯의 총 갯수
	int64 totalSlotNeeded = 0;

	for (const TPair<int32, ItemAmount>& pair : InitemTIdAmountPairList)
	{
		const FGsSchemaItemCommon* itemData = GetItemTableDataByTID(pair.Key);
		if (nullptr == itemData)
		{
			GSLOG(Error, TEXT("item tid is missing : %d"), pair.Key);
			return false;
		}

		bool isStackable = itemData->maxStackCount > 1;

		// 스택 가능한 경우
		if (isStackable)
		{
			int32 slot = GSInventory()->GetInvenSlotMaxCount() - GetOwnedItemCount();
			TArray<TWeakPtr<FGsItem>> stackableItemArray = FindAllStackableItems(itemData->id);
			//ItemAmount itemMaxCount = slot * itemData->maxStackCount;
			ItemAmount itemMaxCountUsing = 0;
			for (TWeakPtr<FGsItem> stackableItem : stackableItemArray)
			{
				if (false == stackableItem.IsValid())
				{
					continue;
				}

				if (stackableItem.Pin()->IsFull())
				{
					continue;
				}

				itemMaxCountUsing += stackableItem.Pin()->GetLeftStackCount();
			}

			// 현재 아이템 Amount와 획득할 아이템 갯수의 합이 현재 사용중인 슬롯들의 빈칸의 합보다 크면,
			// 슬롯이 추가로 필요할 것이다
			if (pair.Value > itemMaxCountUsing)
			{
				totalSlotNeeded += ((pair.Value - itemMaxCountUsing) / itemData->maxStackCount) + 1;
			}
		}
		// 스택 불가능한 경우
		else
		{
			totalSlotNeeded += pair.Value;
		}
	}

	return (totalSlotNeeded + GetOwnedItemCount()) <= GSInventory()->GetInvenSlotMaxCount();
}

bool UGsItemManager::IsEnoughItemAmountByTID(const int32 InItemTID, const ItemAmount InAmount)
{
	const FGsSchemaItemCommon* commonData = GetItemTableDataByTID(InItemTID);
	if (nullptr == commonData)
	{
#if WITH_EDITOR
		GSLOG(Warning , TEXT("[IsEnoughItemAmountByTID] - InItemTID is WrongID : %d"), InItemTID);
#endif
		return false;
	}

	TArray<TWeakPtr<FGsItem>> ownedItemList = FindByTID(commonData->id, commonData->type);
	if (0 >= ownedItemList.Num())
	{
		return false;
	}

	FGsItem* itemData;
	ItemAmount ownedAmount = 0;
	for (int32 i = 0; i < ownedItemList.Num(); ++i)
	{
		itemData = GetGsItemLowPointer(ownedItemList[i]);
		if(nullptr == itemData)
			continue;

		ownedAmount += itemData->GetAmount();
		if (ownedAmount >= InAmount)
		{
			return true;
		}
	}

	return false;
}

//void UGsItemManager::PlayNotEnoughAquireCostTicker(const int32 InItemTID)
//{
//	const FGsSchemaItemCommon* commonData = GetItemTableDataByTID(InItemTID);
//	if (nullptr == commonData)
//	{
//#if WITH_EDITOR
//		GSLOG(Warning, TEXT("[PlayNotEnoughAquireCostTicker] - InItemTID is WrongID : %d"), InItemTID);
//#endif
//		return;
//	}
//
//	FText findText;
//	switch (commonData->categorySub)
//	{
//	case ItemCategorySub::ARROW:
//		FText::FindText(TEXT("ItemUIText"), TEXT("ItemUi_Notice_NotEnoughArrow"), findText);
//		break;
//	case ItemCategorySub::MAGIC_STONE:
//		FText::FindText(TEXT("ItemUIText"), TEXT("Alert_NoMagicStone"), findText);
//		break;
//	}
//
//	FGsUIHelper::TrayMessageTicker(findText);
//}

bool UGsItemManager::IsEquippedBattleConsumeWeapon()
{
	FGsItem* findItem = GetGsItemLowPointer(GetPlayerEquipedItemBySlotType(EGsEquipSlotType::WEAPON_TYPE));
	if (nullptr == findItem || false == findItem->IsEquipment())
		return false;

	FGsItemEquipment* equipItemData = findItem->GetCastItem<FGsItemEquipment>();
	if (nullptr == equipItemData)
		return false;
	
	switch (equipItemData->GetCreatureWeaponType())
	{
	case CreatureWeaponType::BOW:
	case CreatureWeaponType::WAND:
		return true;
	}

	return false;
}

uint8 UGsItemManager::GetMaxSetCountBySetId(const int32 InSetId) const
{
	const FGsSetItemInfo* SetItemInfo = _allSetItemInfoMap.Find(InSetId);
	// 세트 효과는 2개 이상부터 발생하므로, 실제 세트 효과의 갯수에서 하나를 제외함.
	return (nullptr != SetItemInfo) ? SetItemInfo->_defaultItemIds.Num() - 1 : 0;
}

uint8 UGsItemManager::GetOpenSetCountBySetId(const int32 InSetId)
{
	if (0 >= _presetEquipItemContainer[_userPresetBitId].Num())
		return 0;

	if (0 >= InSetId)
		return 0;

	const TSet<ItemId>* allItemIdsInSet = GetAllSetItemTidBySetId(InSetId);
	if (nullptr == allItemIdsInSet)
	{
		return 0;
	}

	uint8 matchedCount = 0;
	for (const auto& iter : _presetEquipItemContainer[_userPresetBitId])
	{
		FGsItem* itemData = GetGsItemLowPointer(FindItem(iter.Value, ItemType::EQUIP));
		if (nullptr == itemData)
			continue;

		if (allItemIdsInSet->Contains(itemData->GetTID()))
		{
			++matchedCount;
		}
	}

	return matchedCount;
}

TArray<TPair<bool, FText>> UGsItemManager::GetAllEquipStatusAndItemNames(const int32 InSetId)
{
	TArray<TPair<bool, FText>> EquipStatusAndItemNames;

	if (0 >= InSetId)
	{
		return MoveTemp(EquipStatusAndItemNames);
	}

	const TMap<EGsEquipSlotType, uint64>& EquipedItems = _presetEquipItemContainer[_userPresetBitId];
	if (0 >= EquipedItems.Num())
	{
		return MoveTemp(EquipStatusAndItemNames);
	}

	const TArray<ItemId>* DefaultSetItemIds = GetDefaultSetItemTids(InSetId);
	if (nullptr == DefaultSetItemIds)
	{
		return MoveTemp(EquipStatusAndItemNames);
	}

	// 먼저 디폴트 아이템 목록을 순환
	for (const ItemId DefaultItemId : *DefaultSetItemIds)
	{
		const FGsSchemaItemCommon* DefaultItemCommon = UGsItemManager::GetItemTableDataByTID(DefaultItemId);
		if (nullptr == DefaultItemCommon)
		{
			continue;
		}

		if (!IsValidItemCategorySub(DefaultItemCommon->categorySub))
		{
			continue;
		}

		// 멀티슬롯 아이템은 한번 더 순환.
		bool ItemFound = false;
		const uint8 TotalSlotCount = HasAnyEquippedMultiSlot(DefaultItemCommon->categorySub) ? ACC_SLOT_MAX_COUNT : 1;
		for (uint8 i = 0; i < TotalSlotCount; ++i)
		{
			const EGsEquipSlotType EquipSlotType = GetEquipSlotType(DefaultItemCommon->categorySub, i);
			const ItemDBId* CurrentItemDBIdPtr = EquipedItems.Find(EquipSlotType);
			if (nullptr == CurrentItemDBIdPtr || INVALID_ITEM_DB_ID == *CurrentItemDBIdPtr)
			{
				continue;
			}

			FGsItem* CurrentItem = GetGsItemLowPointer(FindItem(*CurrentItemDBIdPtr, ItemType::EQUIP));
			if (nullptr == CurrentItem)
			{
				continue;
			}

			const FGsItemEquipment* CurrentEquipItem = CurrentItem->GetCastItem<FGsItemEquipment>();
			if (nullptr != CurrentEquipItem && InSetId == CurrentEquipItem->GetSetId())
			{
				EquipStatusAndItemNames.Emplace(true, CurrentEquipItem->GetName());
				ItemFound = true;
				break;
			}
		}

		if (ItemFound)
		{
			continue;
		}

		EquipStatusAndItemNames.Emplace(false, DefaultItemCommon->name);
	}

	return MoveTemp(EquipStatusAndItemNames);
}

const TSet<ItemId>* UGsItemManager::GetAllSetItemTidBySetId(const int32 InSetId) const
{
	 const FGsSetItemInfo* SetItemInfo = _allSetItemInfoMap.Find(InSetId);
	 return (nullptr != SetItemInfo) ? &(SetItemInfo->_allItemIds) : nullptr;
}

const TArray<ItemId>* UGsItemManager::GetDefaultSetItemTids(const int32 InSetId) const
{
	const FGsSetItemInfo* SetItemInfo = _allSetItemInfoMap.Find(InSetId);
	return (nullptr != SetItemInfo) ? &(SetItemInfo->_defaultItemIds) : nullptr;
}

// outer 로 해당 스텟정보를 가져오고, return 값으로 해당 set 옵션이 적용 받고있는지 안받는지 flag 받음
bool UGsItemManager::GetSetItemStatInfo(const int32 InSetId, SetNumType InType, OUT const TArray<FGsItemStatData>*& OutItemStatDatas)
{
	const FGsSetItemInfo* SetItemInfo = _allSetItemInfoMap.Find(InSetId);
	if (nullptr == SetItemInfo)
		return false;

	OutItemStatDatas = SetItemInfo->_statsPerCount.Find(InType);

	const uint8 matchedCount = GetOpenSetCountBySetId(InSetId);

	if ((SetNumType::Set8 + 1) <= matchedCount) // 풀 매치카운트(풀세트) 상태
		return true;

	switch (InType)
	{
	case SetNumType::Set2:
		return (SetNumType::Set2 + 1) <= matchedCount;
	case SetNumType::Set3:
		return (SetNumType::Set3 + 1) <= matchedCount;
	case SetNumType::Set4:
		return (SetNumType::Set4 + 1) <= matchedCount;
	case SetNumType::Set5:
		return (SetNumType::Set5 + 1) <= matchedCount;
	case SetNumType::Set6:
		return (SetNumType::Set6 + 1) <= matchedCount;
	case SetNumType::Set7:
		return (SetNumType::Set7 + 1) <= matchedCount;
	case SetNumType::Set8:
		return (SetNumType::Set8 + 1) <= matchedCount;
	}

	return false;
}

bool UGsItemManager::IsEquippedItemByTid(const int32 InItemTid)
{
	FGsItem* itemData;
	for (const auto& iter : _presetEquipItemContainer[_userPresetBitId])
	{
		itemData = GetGsItemLowPointer(FindItem(iter.Value, ItemType::EQUIP));
		if (nullptr == itemData)
			continue;

		if (itemData->GetTID() == InItemTid)
		{
			return true;
		}
	}

	return false;
}

EGsEquipSlotType UGsItemManager::GetEquippedSlotTypeByDBID(const uint64 InItemDBID)
{
	for (const auto& iter : _presetEquipItemContainer[_userPresetBitId])
	{
		if (InItemDBID == iter.Value)
		{
			return iter.Key;
		}
	}

	return EGsEquipSlotType::SLOT_MAX;
}

uint32 UGsItemManager::GetTIDByDBID(const uint64 InItemDBID)
{
	FGsItem* itemData = GetGsItemLowPointer(FindItem(InItemDBID));
	if (nullptr == itemData)
		return 0;
	
	return itemData->GetTID();
}

int32 UGsItemManager::GetItemGroupByDBID(const uint64 InItemDBID)
{
	FGsItem* itemData = GetGsItemLowPointer(FindItem(InItemDBID));
	if (nullptr == itemData)
		return 0;

	return itemData->GetItemGroupId();
}

int32 UGsItemManager::GetEquipGroupIdByDBID(const uint64 InItemDBID)
{
	FGsItem* itemData = GetGsItemLowPointer(FindItem(InItemDBID));
	if (nullptr == itemData)
		return 0;

	const FGsItemEquipment* equipData = static_cast<const FGsItemEquipment*>(itemData);
	if (nullptr == equipData)
		return 0;

	return equipData->GetEquipGroupId();
}

void UGsItemManager::AttachEquipItem(uint64 InEquipDBID, uint64 InUnEquipDBID, uint8 InSlotIndex)
{
	FGsNetSendServiceItem::SendAttachEquipItem(_userPresetId, InEquipDBID, InSlotIndex, InUnEquipDBID, InSlotIndex);
	//_isRecvSameEquipTypeProcPacket = false;
}

void UGsItemManager::DetachEquipItem(uint64 InUnEquipDBID, uint8 InSlotIndex)
{
	FGsNetSendServiceItem::SendDetacheEquipItem(_userPresetId, InUnEquipDBID, InSlotIndex);
	//_isRecvSameEquipTypeProcPacket = false;
}

TSharedPtr<FGsItemEquipEnchantLoader> UGsItemManager::GetEquipEnchantLoader()
{
	if (!_equipEnchantLoader.IsValid())
	{
		_equipEnchantLoader = MakeShareable(new FGsItemEquipEnchantLoader);
	}

	check(_equipEnchantLoader.IsValid());
	return _equipEnchantLoader;
}

const FGsItemEquipEnchantBonus* UGsItemManager::GetItemEquipEnchantBonus(const int32 InId)
{
	return GetEquipEnchantLoader()->_enchantBonus.Find(InId);
}

const FGsItemEquipEnchantBalance* UGsItemManager::GetItemEquipEnchantBalance(const int32 InId)
{
	return GetEquipEnchantLoader()->_enchantBalance.Find(InId);
}

bool UGsItemManager::MakeDisposableItems(OUT TArray<ItemDBIdAmountPair>& OutItemAndAmounts, const FGsCostPackage& InCostPackage)
{
	return MakeDisposableItems(OutItemAndAmounts, *InCostPackage.MakeCostPackageMultiple());
}

bool SortOwnedAmountAsc(TWeakPtr<const FGsItem> InA, TWeakPtr<const FGsItem> InB)
{
	return InA.Pin()->GetAmount() < InB.Pin()->GetAmount();
}

bool UGsItemManager::MakeDisposableItems(OUT TArray<ItemDBIdAmountPair>& OutItemAndAmounts, const FGsCostPackageMultiple& InCostPackageMultiple)
{
	OutItemAndAmounts.Empty();
	TMap<ItemDBId, ItemAmount> itemAmountMap;

	const bool SearchWithDepot = ECachedValueItemDelegateType::CONSUMABLE_INVEN_DEPOT == InCostPackageMultiple.GetCachedValueItemDelegateType();

	for (int32 i = InCostPackageMultiple.GetCostElementsCount() - 1; 0 <= i; --i)
	{
		const FGsCostElement* CostElement = InCostPackageMultiple.GetSingleCostElementAt(i);
		if (nullptr == CostElement)
		{
			GSLOG(Error, TEXT("nullptr == CostElement, i:%d"), i);
			continue;
		}

		if (CostType::ITEM != CostElement->_costType)
		{
			continue;
		}

		// GetMultiple() 함수 사용 않하는 방법 강구 필요.
		ItemAmount RequireAmount = CostElement->GetCostAmount() * InCostPackageMultiple.GetMultiple();
		const uint8 CandidateCount = CostElement->GetCandidateCount();

		// ItemManager 에서 선탐색
		for (uint8 j = 0; j < CandidateCount; ++j)
		{
			const FGsCostElement::FGsCostElementItem* CostElementItem = StaticCast<const FGsCostElement::FGsCostElementItem*>(CostElement->GetCandidateAt(j));

			if (nullptr == CostElementItem)
			{
				GSLOG(Error, TEXT("nullptr == CostElementItem"));
			}
			else
			{
				TArray<TWeakPtr<FGsItem>> Items = FindByTID(CostElementItem->_costItemId);
				Items.Sort(&SortOwnedAmountAsc);

				FGsItem* itemData = nullptr;
				while (0 < RequireAmount && 0 < Items.Num())
				{
					itemData = GetGsItemLowPointer(Items.Pop(false));
					if(nullptr == itemData)
						continue;
					
					if (!CostElementItem->IsPassedItem(*itemData))
					{
						continue;
					}

					const ItemAmount OwnedItemAmount = itemData->GetAmount();
					check(0 < OwnedItemAmount);


					if (itemAmountMap.Contains(itemData->GetDBID()))
					{
						itemAmountMap[itemData->GetDBID()] += FMath::Min<ItemAmount>(RequireAmount, OwnedItemAmount);
					}
					else
					{
						itemAmountMap.Emplace(itemData->GetDBID(), FMath::Min<ItemAmount>(RequireAmount, OwnedItemAmount));
					}

					//OutItemAndAmounts.Emplace(itemData->GetDBID(), FMath::Min<ItemAmount>(RequireAmount, OwnedItemAmount));

					RequireAmount = RequireAmount - OwnedItemAmount;
				}
			}

			if (!CostElement->_jointCostCandidateElements)
			{
				// 대체아이템 합산 비용이 아닌 경우, 1회로 종료하기 위해 break
				break;
			}
		}

		// 창고에서 추가 탐색
		if (SearchWithDepot && 0 < RequireAmount)
		{
			FGsDepotManager* DepotManager = GSDepot();
			if (nullptr == DepotManager)
			{
				GSLOG(Error, TEXT("nullptr == DepotManager"));
			}
			else
			{
				for (uint8 j = 0; j < CandidateCount; ++j)
				{
					const FGsCostElement::FGsCostElementItem* CostElementItem = StaticCast<const FGsCostElement::FGsCostElementItem*>(CostElement->GetCandidateAt(j));

					if (nullptr == CostElementItem)
					{
						GSLOG(Error, TEXT("nullptr == CostElementItem"));
					}
					else
					{
						TArray<TWeakPtr<FGsItem>> Items = DepotManager->FindByTID(CostElementItem->_costItemId);
						Items.Sort(&SortOwnedAmountAsc);

						FGsItem* itemData = nullptr;
						while (0 < RequireAmount && 0 < Items.Num())
						{
							itemData = GetGsItemLowPointer(Items.Pop(false));
							if (nullptr == itemData)
								continue;

							if (!CostElementItem->IsPassedItem(*itemData))
							{
								continue;
							}

							const ItemAmount OwnedItemAmount = itemData->GetAmount();
							check(0 < OwnedItemAmount);

							if (itemAmountMap.Contains(itemData->GetDBID()))
							{
								itemAmountMap[itemData->GetDBID()] += FMath::Min<ItemAmount>(RequireAmount, OwnedItemAmount);
							}
							else
							{
								itemAmountMap.Emplace(itemData->GetDBID(), FMath::Min<ItemAmount>(RequireAmount, OwnedItemAmount));
							}

							//OutItemAndAmounts.Emplace(itemData->GetDBID(), FMath::Min<ItemAmount>(RequireAmount, OwnedItemAmount));

							RequireAmount = RequireAmount - OwnedItemAmount;
						}
					}

					if (!CostElement->_jointCostCandidateElements)
					{
						// 대체아이템 합산 비용이 아닌 경우, 1회로 종료하기 위해 break
						break;
					}
				}
			}
		}

		if (0 < RequireAmount)
		{
			GSLOG(Log, TEXT("0 < RequireAmount, RequireAmount:%d, i:%d"), RequireAmount, i);
			return false;
		}
	}

	Algo::Transform(itemAmountMap, OutItemAndAmounts, [](const auto& pair) -> ItemDBIdAmountPair
		{
			return ItemDBIdAmountPair(pair.Key, pair.Value);
		});

	return true;
}

bool UGsItemManager::IsTheSameGroup(const ItemId InItemIdA, const ItemId InItemIdB)
{
	const FGsSchemaItemCommon* ItemCommonA = FGsItem::GetTableDataByID(InItemIdA);
	const FGsSchemaItemCommon* ItemCommonB = FGsItem::GetTableDataByID(InItemIdB);
	return (nullptr != ItemCommonA) && (nullptr != ItemCommonB) && (INVALID_ITEM_GROUP_ID != ItemCommonA->itemGroupId) && (ItemCommonA->itemGroupId == ItemCommonB->itemGroupId);
}

ItemId UGsItemManager::GetFirstItemIdInGroup(const ItemId InItemId, const bool InIncludeExpired /*= false*/)
{
	const FGsSchemaItemCommon* ItemCommon = FGsItem::GetTableDataByID(InItemId);
	if (nullptr != ItemCommon)
	{
		if (INVALID_ITEM_GROUP_ID != ItemCommon->itemGroupId)
		{
			const ItemId* CurrentItemId = UGsItemManager::_cachedFirstItemIdInItemGroup.Find(ItemCommon->itemGroupId);
			if (nullptr != CurrentItemId)
			{
				return *CurrentItemId;
			}
		}
	}

	// 없으면 조회한 아이템ID 반환
	return InItemId;
}

TSet<ItemId> UGsItemManager::GetAllItemIdsTheSameGroup(const ItemId InItemId, const bool InIncludeExpired /*= false*/)
{
	TSet<ItemId> ItemIds;
	const FGsSchemaItemCommon* ItemCommon = FGsItem::GetTableDataByID(InItemId);
	if (nullptr != ItemCommon)
	{
		if (INVALID_ITEM_GROUP_ID != ItemCommon->itemGroupId)
		{
			const TSet<ItemId>* ItemIdSet = UGsItemManager::_cachedItemIdsInItemGroup.Find(ItemCommon->itemGroupId);
			if (nullptr != ItemIdSet)
			{
				ItemIds.Append(*ItemIdSet);
			}
		}

		// 테이블 조회 실패해도 최소 조회한 아이템ID는 입력
		ItemIds.Add(InItemId);
	}
	return ItemIds;
}

TArray<TWeakPtr<FGsItem>> UGsItemManager::GetAllOwnedItemsTheSameGroup(const ItemId InItemId, const bool InIncludeExpired /*= false*/)
{
	TArray<TWeakPtr<FGsItem>> Items;
	const TSet<ItemId> ItemIdSet = UGsItemManager::GetAllItemIdsTheSameGroup(InItemId);
	for (const ItemId CurrentItemId : ItemIdSet)
	{
		Items.Append(FindByTID(CurrentItemId));
	}
	return Items;
}

// pickone reward 아이템이면 별도 아이템 선택 팝업을 출력해줘야함
bool UGsItemManager::IsPickOneTypeRewardItem(const FGsItem* InItem)
{
	if (nullptr == InItem)
	{
		return false;
	}
	int32 id = InItem->GetPickOneTypeRewardID();
	if (0 >= id)
	{
		return false;
	}

	return true;
}

bool UGsItemManager::HasAnyEquippedMultiSlot(const ItemCategorySub InSubCategory)
{
	EGsEquipSlotType slotType1 = GetEquipSlotType(InSubCategory, 0);
	EGsEquipSlotType slotType2 = GetEquipSlotType(InSubCategory, 1);

	return ( (nullptr != GetPlayerEquipedItemBySlotType(slotType1)) || (nullptr != GetPlayerEquipedItemBySlotType(slotType2)) );
}

// 유효기간 만료 시에 삭제된 아이템만 준다고함
void UGsItemManager::ExpiredDeleteItem(TArray<ItemId> InItemTIDList)
{
	if (0 >= InItemTIDList.Num())
		return;

	FString  result;
	FText findText;
	for (int32 tid : InItemTIDList)
	{
		const FGsSchemaItemCommon* itemData = FGsItem::GetTableDataByID(tid);
		if (nullptr == itemData)
			continue;
		/*
		const UGsTableItemExpiredTime* table = Cast<UGsTableItemExpiredTime>(FGsSchemaItemExpiredTime::GetStaticTable());
		if (nullptr == table)
		{
			return;
		}
		const FGsSchemaItemExpiredTime* row = nullptr;
		if (false == table->FindRowById(itemData->expiredTimeId, row))
		{
			return;
		}
		*/

		FText::FindText(TEXT("ItemUIText"), TEXT("ItemUi_Expired_Delete"), findText);
		result = itemData->name.ToString() + findText.ToString();
		FGsUIHelper::TraySectionMessageTicker(EGsNotiSection::EXTSYSTEM, FText::FromString(result), TrayCondition::TC_Close, SectionPriority::None);
	}
}

void UGsItemManager::FinishedPeriodItem(const struct IGsMessageParam* InParam)
{
	const FGsCoolTimeMessageParam* param = InParam->Cast<const FGsCoolTimeMessageParam>();
	if (nullptr == param)
		return;

	if (EGsCoolTime::ItemPeriod != param->_type)
		return;

	FGsItem* itemData = GetGsItemLowPointer(FindItem(param->_groupId));
	if (nullptr == itemData)
		return;

	SendUpdateMessage(itemData->GetDBID(), true, false);

	if (false == itemData->IsRemoveAfterExpired())
	{
		FString  result;
		FText findText;
		FText::FindText(TEXT("ItemUIText"), TEXT("ItemUi_Expired_exceed"), findText);
		result = itemData->GetName().ToString() + findText.ToString();
		FGsUIHelper::TraySectionMessageTicker(EGsNotiSection::EXTSYSTEM, FText::FromString(result), TrayCondition::TC_Close);
	}
}

float UGsItemManager::GetInvenItemWithAddItemWeightRate(float In_addItemWeight)
{
	UGsGameObjectBase* local = GSGameObject()->FindObject(EGsGameObjectType::LocalPlayer);
	if (nullptr == local)
	{
		return 0.0f;
	}
	FGsGameObjectDataCreature* creatureData = local->GetCastData<FGsGameObjectDataCreature>();
	if (nullptr == creatureData)
	{
		return 0.0f;
	}

	const FGsStatInfo* weight = creatureData->GetCreatureStatInfo(StatType::WEIGHT);
	if (weight == nullptr)
	{
		return 0.0f;
	}

	float totalWeight = (float)GetOwnedItemWeight() + In_addItemWeight;
	return totalWeight / (float)weight->GetMaxStatValue();
}

FGsItem* UGsItemManager::GetGsItemLowPointer(TWeakPtr<FGsItem> InWeakPtr)
{
	if (false == InWeakPtr.IsValid())
		return nullptr;

	return InWeakPtr.Pin().Get();
}

bool UGsItemManager::IsUsableLevel(const FGsItem* In_Item)
{
	const FGsNetUserData* userData = GGameData()->GetUserData();
	if (nullptr == userData)
		return false;

	return userData->mLevel >= In_Item->GetUseLevel();
}

void UGsItemManager::IngredientUse_FairyUI(const FGsItem* InItem)
{
	if (nullptr != GSFairy() && nullptr != InItem)
	{
		GSFairy()->OnFairyStateSelectedPage(0, InItem->GetTID());
	}
}

void UGsItemManager::IngredientUse_MonsterKnowledgeUI(const FGsItem* InItem)
{
	// 2023/3/28 PKT - 컨텐츠 UnLock 검사
	if (false == GSUnlock()->IsContentsUnlock(EGsUnlockContentsType::MONSTER_KNOWLEDGE_COLLECTION, true))
	{
		return;
	}

	if (nullptr != InItem)
	{
		const int32 maxCount = GsMonsterCollection()->SlateMaxCount();
		for (int32 idx = 0; idx < maxCount; ++idx)
		{
			const auto& slateData = GsMonsterCollection()->SlateDataAt(idx);
			if (slateData && slateData->IsCostItem(InItem->GetTID()) && EMonsterKnowledgeSlateState::UnLock == slateData->State())
			{
				// 2023/1/31 PKT - 이미 활성화 되어 있는 상태
				FText ticker;
				FText::FindText(TEXT("MonsterKnowledgeText"), TEXT("ticker_AlreadyActivatedSlate"), ticker);
				FGsUIHelper::TraySectionMessageTicker(EGsNotiSection::EXTSYSTEM, ticker);

				return;
			}
		}
		

		/**
		 * 주의 : 몬스터 도감을 MessageContentMonsterKnowledge::USE_ITEM_SLATE 안에서 활성화 시키면 윈도우가 활성화 될때 이벤트 메세지 바인딩을 하게 되는데 그때 라우터의 리스트가 깨질 수 있음.(크래시 원인)
		 * 때문에 여기서 윈도우를 활성화 시키고 'MessageContentMonsterKnowledge' 타입의 메세지를 보내서 서로를 분리 시킨다.
		 */
		// 2021/12/22 PKT - 도감 Window Open
		GMessage()->GetContents().SendMessage(MessageContents::ACTIVE_WINDOW_MONSTER_COLLECTION);

		// 2021/12/22 PKT - 몬스터 도감 전용 아이템(지식 석판 오픈 아이템)
		const FGsPrimitiveInt32 message(InItem->GetTID());
		GMessage()->GetMonsterKnowledgeCollection().SendMessage(MessageContentMonsterKnowledge::USE_ITEM_SLATE, &message);
	}
}

void UGsItemManager::IngredientUse_GuildCampUI(const FGsItem* InItem)
{
	if (nullptr != GSCamp() && nullptr != InItem)
	{
		GSCamp()->TryUseCampConsumItem(InItem->GetDBID());
	}
}

void UGsItemManager::IngredientUse_ItemEnchantUI(const FGsItem* InItem)
{
	if (nullptr != InItem)
	{
		struct FGsOpenItemEnchant paramData(InItem->GetDBID(), true, true);
		GMessage()->GetItemEnchant().SendMessage(MessageContentEnchant::OPEN, &paramData);
	}
}

// 사용 안함
// 분해 키트 아이템 사라짐
void UGsItemManager::IngredientUse_ItemMultiDecomposeUI(const FGsItem* InItem)
{
	/*
	if (nullptr != InItem)
	{
		struct FGsItemDecomposeSingle paramData(InItem->GetDBID());
		GMessage()->GetItemContents().SendMessage(MessageContentItem::DECOMPOSE_SINGLE, &paramData);
	}
	*/
}

void UGsItemManager::IngredientUse_ItemCoupon(const FGsItem* InItem)
{
	if (nullptr != InItem)
	{
		MakeShared<FGsBMShopOpenSpecialTab>(EGsBMShopSpecialTab::CouponUsableTab)->Send();
	}
}

void UGsItemManager::IngredientUse_ExchangeCostumeUI(const FGsItem* InItem)
{
	if (GSCostume() && InItem)
	{
		GSCostume()->OnCostumeExchangeStateSelected(InItem);
	}
}

void UGsItemManager::ShowErrorTidTicker(const int32 InItemTid)
{
	FString temp = "[Super Error] Not Exist DC_TableData !!!!!! \n plz tell Client Team \n ItemTID : " + FString::FromInt(InItemTid);
	FGsUIHelper::TraySystemTicker(FText::FromString(temp),true);
}

const ItemAmount UGsItemManager::GetAllItemAmountByTypes(ItemType Intype, ItemCategoryMain InMainType, ItemCategorySub InSubType)
{
	TArray<TWeakPtr<FGsItem>> findItems = FindItem(Intype, InMainType, InSubType);
	if (0 >= findItems.Num())
	{
		return 0;
	}

	ItemAmount ownedAmount = 0;
	for (int32 i = 0; i < findItems.Num(); ++i)
	{
		if (findItems[i].IsValid())
		{
			if (findItems[i].Pin()->GetLock())
				continue;

			ownedAmount += findItems[i].Pin()->GetAmount();
		}
	}
	
	return ownedAmount;
}

void UGsItemManager::SetCurrentLoginCreatureWeaponType()
{
	TWeakPtr<FGsItem> item = GetPlayerEquipedItemBySlotType(EGsEquipSlotType::WEAPON_TYPE);
	if (item.IsValid())
	{
		if (FGsItemEquipment* equipItem = item.Pin()->GetCastItem<FGsItemEquipment>())
		{
			_loginCreatureWeaponType = equipItem->GetCreatureWeaponType();
		}
	}
}

void UGsItemManager::SetRealDisplayDivideValue(float InVal)
{
	if (0.f >= InVal)
		return;

	_divideValue = InVal;
}

ItemGrade UGsItemManager::GetItemGradeByDBId(const uint64 InItemDBId)
{
	FGsItem* item = GetGsItemLowPointer(FindItem(InItemDBId));

	if (item)
	{
		return item->GetGradeType();
	}
	else
	{
		return ItemGrade::NONE;
	}
}

int32 UGsItemManager::GetItemEnchantLevelByDBId(const uint64 InItemDBId)
{
	FGsItem* item = GetGsItemLowPointer(FindItem(InItemDBId));

	if (item)
	{
		return item->GetLevel();
	}
	else
	{
		return 0;
	}
}

bool UGsItemManager::IsItemLockByTId(const int32 InItemTID)
{
	TArray<TWeakPtr<FGsItem>> findItemList = FindByTID(InItemTID);

	// 아이템이 없다면 잠겨 있지 않다고 간주한다
	if (0 >= findItemList.Num())
		return false;

	FGsItem* itemData;
	for (int32 i = 0; i < findItemList.Num(); ++i)
	{
		itemData = GetGsItemLowPointer(findItemList[i]);
		if (nullptr != itemData)
		{
			if (itemData->GetLock())
			{
				return true;
			}
		}
	}

	return false;
}

bool UGsItemManager::IsItemEquipmentByDBId(const uint64 InItemDBId)
{
	FGsItem* item = GetGsItemLowPointer(FindItem(InItemDBId));

	if (item)
	{
		return item->IsEquipment();
	}
	else
	{
		return false;
	}
}

bool UGsItemManager::IsItemEquipmentByTId(const ItemId InItemTId)
{
	const UGsTableItemCommon* TableItemCommon = Cast<UGsTableItemCommon>(FGsSchemaItemCommon::GetStaticTable());
	if (nullptr == TableItemCommon)
	{
		GSLOG(Error, TEXT("nullptr == TableItemCommon"));
		return false;
	}

	const FGsSchemaItemCommon* ItemCommon = nullptr;
	if (!TableItemCommon->FindRowById(InItemTId, ItemCommon))
	{
		GSLOG(Error, TEXT("TableItemCommon->FindRowById(InItemTId, Row), InItemTId:%d"), InItemTId);
		return false;
	}

	if (nullptr == ItemCommon)
	{
		GSLOG(Error, TEXT("nullptr == ItemCommon, InItemTId:%d"), InItemTId);
		return false;
	}

	if (ItemCommon->type == ItemType::EQUIP)
	{
		return true;
	}
	else
	{
		return false;
	}
}

FText UGsItemManager::GetItemNameByDBId(const uint64 InItemDBId)
{
	FGsItem* item = GetGsItemLowPointer(FindItem(InItemDBId));

	if (item)
	{
		return item->GetName();
	}
	else
	{
		return FText::GetEmpty();
	}
}

FText UGsItemManager::GetItemNameByTId(const ItemId InItemTId)
{
	const FGsSchemaItemCommon* itemCommon = GetItemTableDataByTID(InItemTId);
	if (nullptr == itemCommon)
	{
		GSLOG(Error, TEXT("nullptr == itemCommon, InItemTId : %d"), InItemTId);
		return FText::GetEmpty();
	}

	return itemCommon->name;
}

ItemId UGsItemManager::GetItemTIdByDBId(const uint64 InItemDBId)
{
	FGsItem* item = GetGsItemLowPointer(FindItem(InItemDBId));

	if (item)
		return item->GetTID();
	else
		return INVALID_ITEM_ID;
}

// 처음엔 CheckConsumItemCondition를 public으로 바꿀려고 했는데
// 다른데서도 사용하면 사이드 이펙트가 생길수 있어서 별도 함수 분리
//	https://jira.com2us.com/jira/browse/C2URWQ-5204
// 외부용 뽑기 아이템 유효한지 체크 함수
bool UGsItemManager::IsValidSummonItem(const FGsItem* InItem)
{
	if (false == FGsItem::IsValidItem(InItem))
	{
		// 유효하지 않은 아이템
		return false;
	}

	// 사용할 수 있는 레벨인지 확인
	if (false == IsUsableLevel(InItem))
	{
		return false;
	}

	// 상태이상일때 사용 못함
	if (true == IsReactionLockTypeItem(InItem->GetSubCategory()))
	{
		return false;
	}

	// stack count 가 0보다 큰가
	if (0 >= InItem->GetAmount())
	{
		return false;
	}

	// 현재 lock 상태인가 
	// 장착류 무기들은 장착해지 가 되어야 하므로 제외처리
	if (true == InItem->GetLock() && ItemType::EQUIP != InItem->GetType())
	{
		return false;
	}

	// 사용 가능 유효기간 체크
	// 다만 장착 중인 아이템일때 만료 되면 장착 해지 처리를 해줘야 함
	if (InItem->IsPeriodItem() && InItem->IsExpiredTime() && false == InItem->IsEquipped())
	{
		return false;
	}

	// 소모품이나 사용성 재료형 아이템의 쿨타임 그룹 체크
	if (ItemType::CONSUME == InItem->GetType() || ItemType::INGREDIENT == InItem->GetType())
	{
		if (true == GCoolTime()->IsCoolTime(EGsCoolTime::Item, InItem->GetCooldownGroupID(), 0))
		{
			// 해당 아이템의 GroupID 로 쿨타임이 돌고 있는 상태
			return false;
		}
	}

	// 각 아이템과 관련한 컨텐츠의 잠금 확인
	if (false == GSUnlock()->IsContentsUnlock(InItem->GetSubCategory()))
	{
		return false;
	}

	if (FGsPersonalTradingManager* tradeManager = GSPersonalTrading())
	{
		if (tradeManager->IsSelectedItem(InItem->GetDBID()))
		{
			return false;
		}
	}

	// [B1] | ejrrb10 | HP 가 0일때 모든 템 사용 못함 (추후 사망 시, 아이템 사용 불가 티커를 띄우지 않는 로직 처리)
	UGsGameObjectBase* local = GSGameObject()->FindObject(EGsGameObjectType::LocalPlayer);
	if (local == nullptr)
	{
		return false;
	}
	FGsGameObjectDataCreature* creatureData = local->GetCastData<FGsGameObjectDataCreature>();
	if (creatureData == nullptr || creatureData->IsZeroHp())
	{
		return false;
	}

	const FGsItemConsumable* consumItem = static_cast<const FGsItemConsumable*>(InItem);
	if (nullptr == consumItem)
	{
		return false;
	}

	// Running CoolTime
	if (true == GCoolTime()->IsCoolTime(EGsCoolTime::Item, consumItem->GetCooldownGroupID(), 0))
	{
		// 해당 아이템의 GroupID 로 쿨타임이 돌고 있는 상태
		return false;
	}

	bool isValid = false;
	isValid = InnerCheck_IsValidSummonItem(InItem, true);

	return isValid;
}
//	https://jira.com2us.com/jira/browse/C2URWQ-5204
// 내부 함수 체크용(CheckConsumItemCondition 에서 호출)
bool UGsItemManager::InnerCheck_IsValidSummonItem(const FGsItem* InItem, const bool InCheckOnly)
{
	if(InItem == nullptr)
	{
		return false;
	}
	// 뽑기 가능한 상태인지
	auto IsSummonPossible = [this]()
	{
		// 워프 연출중에 아이템 사용 안되어서 사용불가되는 이슈
		// https://jira.com2us.com/jira/browse/C2URWQ-6479
		UGsGameObjectBase* localBase = GSGameObject()->FindObject(EGsGameObjectType::LocalPlayer);		
		if (true == localBase->IsCrowdControlLock(CreatureActionType::CREATURE_ACTION_TYPE_MOVE_DEFAULT))
		{
			return false;
		}

		// https://jira.com2us.com/jira/browse/C2URWQ-5204
		// 키보드 연타 때문에 잠겨있나 체크
		if (_inputBlockHandler != nullptr)
		{
			if (_inputBlockHandler->IsBlocked() == true)
			{
				return false;
			}
		}

		return true;
	};


	switch (InItem->GetSubCategory())
	{
	case ItemCategorySub::SUMMON_FAIRY:
	{
		if (IsSummonPossible() == false)
		{
			return false;
		}

		// 소환 대기 갯수 최대 인지 체크
		if (FGsSummonFunc::CheckFairySummonWaitCount(false, InCheckOnly) == false)
		{
			return false;
		}
		//https://jira.com2us.com/jira/browse/C2URWQ-5176
		// stack 갯수 최대 인지 체크
		if (FGsSummonFunc::CheckFairySummonStackCount(false, InCheckOnly) == false)
		{
			return false;
		}

		if (InCheckOnly == false)
		{
			if (_inputBlockHandler != nullptr)
			{
				TArray<EGsInputBlockConditionType> arrayCondition;
				arrayCondition.Add(EGsInputBlockConditionType::RECV_USE_ITEM);
				_inputBlockHandler->AddBlockConditions(arrayCondition);
			}
			// 등급 연출용 데이터 세팅
			GSSummon()->SetFairySummonItemParticleGrade(InItem->GetTID());
		}

		return true;
	}
	case ItemCategorySub::SUMMON_COSTUME:
	{
		if (IsSummonPossible() == false)
		{
			return false;
		}

		// 소환 대기 갯수 최대 인지 체크
		if (FGsSummonFunc::CheckCostumeSummonWaitCount(false, InCheckOnly) == false)
		{
			return false;
		}
		//https://jira.com2us.com/jira/browse/C2URWQ-5176
		// stack 갯수 최대 인지 체크
		if (FGsSummonFunc::CheckCostumeSummonStackCount(false, InCheckOnly) == false)
		{
			return false;
		}
		if (InCheckOnly == false)
		{
			if (_inputBlockHandler != nullptr)
			{
				TArray<EGsInputBlockConditionType> arrayCondition;
				arrayCondition.Add(EGsInputBlockConditionType::RECV_USE_ITEM);
				_inputBlockHandler->AddBlockConditions(arrayCondition);
			}
			// 등급 연출용 데이터 세팅
			GSSummon()->SetCostumeSummonItemParticleGrade(InItem->GetTID());
		}

		return true;
	}
	}

	return false;
}

// 장비 아이템을 제외한 아이템 처리
void UGsItemManager::AddItemList(TArray<ItemDataCreated> InItemList, bool InIsNewItem)
{
	TArray<const ItemData*> createdItemDataList;
	TArray<const ItemDataEquip*> createdEquipItemDataList;
	for (const ItemDataCreated& item : InItemList)
	{
		if (0 >= item.mTemplateId)
			continue;

		ItemType itemType = GetItemTypeByTID(item.mTemplateId);
		if (ItemType::EQUIP == itemType)
		{
			ItemDataEquip equipData(item);
			createdEquipItemDataList.Emplace(&equipData);
		}
		else
		{
			createdItemDataList.Emplace(&item);
		}
	}
	AddItem(createdItemDataList, InIsNewItem);
	AddItem(createdEquipItemDataList, InIsNewItem);
}

void UGsItemManager::AddEquipItemList(TArray<ItemDataEquip> InItemList, bool InIsNewItem)
{
	TArray<const ItemDataEquip*> createdEquipItemDataList;
	for (const ItemDataEquip& item : InItemList)
	{
		if (0 >= item.mTemplateId)
			continue;

		ItemType itemType = GetItemTypeByTID(item.mTemplateId);
		if (ItemType::EQUIP != itemType)
			continue;
		
		createdEquipItemDataList.Emplace(&item);
	}
	AddItem(createdEquipItemDataList, InIsNewItem);
}

/////////////// Add Item  //////////////////
void UGsItemManager::AddItem(TArray<const ItemData*> InAddItemList, bool In_isNewItem)
{
	if (0 >= InAddItemList.Num())
		return;

	TArray<FGsItem*> newItemDataList;
	TArray<FGsItem*> updateItemList;
	for (const ItemData* iter : InAddItemList)
	{
		if(nullptr == iter)
			continue;

		FGsItem* newItem = nullptr;
		TWeakPtr<FGsItem> findItem = FindItem(iter->mItemDBId);
		if (findItem.IsValid() && FGsItem::IsValidItem(findItem.Pin().Get()))
		{
			// 이미 보유중. 덮어씀
			findItem.Pin()->UpdateBy(iter);
			findItem.Pin()->SetIsDepotItem(false);
			updateItemList.Emplace(findItem.Pin().Get());
		}
		else
		{
			// 아이템 create
			 newItem = FGsItem::Create(iter);
			if (nullptr != newItem)
			{
				newItem->SetIsDepotItem(false);
				newItem->SetIsNew(In_isNewItem);
				_itemBuffers->AddItems(newItem);

				newItemDataList.Emplace(newItem);
			}
		}
		FGsItem* finalItemData = findItem.IsValid() ? findItem.Pin().Get() : newItem;
		FGsItemHelper::SetPeriodItemCoolTime(finalItemData);
	}

	SendAddMessage(newItemDataList);
	SendUpdateMessage(updateItemList);
}

/////////////// Add Item - EquipItem //////////////////
void UGsItemManager::AddItem(TArray<const ItemDataEquip*> InItemList, bool InIsNewItem)
{
	if (0 >= InItemList.Num())
		return;

	TArray<FGsItem*> allItemList;
	TArray<FGsItem*> updateItemList;
	TArray<FGsItem*> newItemDataList;
	for (const ItemDataEquip* iter : InItemList)
	{
		if(nullptr == iter)
			continue;

		FGsItem* newItem = nullptr;
		TWeakPtr<FGsItem> findItem = FindItem(iter->mItemDBId);
		if (findItem.IsValid() && FGsItem::IsValidItem(findItem.Pin().Get()))
		{
			// 이미 보유중. 덮어씀
			findItem.Pin()->UpdateBy(iter);
			findItem.Pin()->SetIsDepotItem(false);
			updateItemList.Emplace(findItem.Pin().Get());
		}
		else
		{
			// new item
			newItem = FGsItem::Create(iter);
			if (nullptr != newItem)
			{
				newItem->SetIsDepotItem(false);
				newItem->SetIsNew(InIsNewItem);
				_itemBuffers->AddItems(newItem);
				newItemDataList.Emplace(newItem);
			}
		}
		FGsItem* finalItemData = findItem.IsValid() ? findItem.Pin().Get() : newItem;
		FGsItemHelper::SetPeriodItemCoolTime(finalItemData);

		allItemList.Emplace(finalItemData);
	}

	SendAddMessage(newItemDataList);
	SetEquipItemData(allItemList);
	SendUpdateMessage(updateItemList);
}

void UGsItemManager::SetEquipItemData(TArray<FGsItem*> InEquipItemList)
{
	bool isDirty = false;
	bool isUnEquipped = false;
	for (FGsItem* iter : InEquipItemList)
	{
		if (nullptr == iter)
			return;

		FGsItemEquipment* equipData = iter->GetCastItem<FGsItemEquipment>();
		if (nullptr == equipData)
			return;

		PresetValue presetValue = equipData->GetEquipPresetValue();
		ItemDBId newDBID = equipData->GetDBID();
		for (TPair<PresetValue, TMap<EGsEquipSlotType, uint64>>& equipItemContainer : _presetEquipItemContainer)
		{
			TArray<uint8> slotIndexList;
			slotIndexList.Emplace(0);
			if (IsMultiSlotItem(equipData->GetSubCategory()))
			{
				slotIndexList.Emplace(1);
			}

			// SharedFunc::GetEquipSlotLocation 의 내용
			uint8 itemSlotIndex = ((equipData->GetSlotIndex() & equipItemContainer.Key) != 0) ? 1 : 0;

			for (uint8 slotIndex : slotIndexList)
			{
				EGsEquipSlotType slotType = GetEquipSlotType(equipData->GetSubCategory(), slotIndex);
				if (equipItemContainer.Value.Contains(slotType))
				{
					PresetValue presetId = equipItemContainer.Key;
					ItemDBId oldDBID = equipItemContainer.Value[slotType];
					// 일부 프리셋에 장착되어있을 때
					if (presetId & presetValue &&
						itemSlotIndex == slotIndex)
					{
						equipItemContainer.Value[slotType] = newDBID;
						if (false == isDirty)
						{
							isDirty |= (oldDBID != newDBID);
						}
						isUnEquipped = false;
					}
					else
					{
						// 장착 해제
						if (oldDBID == newDBID)
						{
							equipItemContainer.Value[slotType] = INVALID_ITEM_DB_ID;
							if (false == isDirty)
							{
								isDirty = true;
								isUnEquipped = true;
							}
						}
					}
				}
			}
		}
	}

	bool isCompleteSpawn = UGsGameObjectUtil::GetSpawnMeCompleteFinFlag();
	if (isCompleteSpawn && isDirty)
	{
		// sound
		// 장비 장착 / 해지 에 따른 사운드 출력
		// swap 일때는 장착 사운드만 출력 해야함
		FString fileName = isUnEquipped ? "UI_Cancel" : "UI_EquipGear";
		UGsSoundPlayer* soundPlayer = GSoundPlayer();
		if (nullptr != soundPlayer)
		{
			soundPlayer->PlaySound2D(fileName);
		}	
	}

	/*
	if (isDirty)
	{
		FGsInvalidateEquipSlotInfo param(slotType);
		GMessage()->GetPlayerDetailInfo().SendMessage(MessageContentPlayerInfo::EQUIP, &param);
	}
	*/
	GMessage()->GetPlayerDetailInfo().SendMessage(MessageContentPlayerInfo::EQUIP_ALL, nullptr);

	bool isSpawnComplete = UGsGameObjectUtil::GetSpawnMeCompleteFinFlag();
	if (isSpawnComplete)
	{
		ItemDBId dbid = GetEquipedItemDBIDBySlotType(EGsEquipSlotType::WEAPON_TYPE);
		TWeakPtr<FGsItem> findItemData = FindItem(dbid, ItemType::EQUIP);
		if (findItemData.IsValid())
		{
			FGsItemEquipment* equipItemData = findItemData.Pin()->GetCastItem<FGsItemEquipment>();
			_loginCreatureWeaponType = equipItemData->GetCreatureWeaponType();
		}	
	}

	//GSLOG(Warning, TEXT("[SetEquipItemData] Add Data - Category : %d"), InCategory);
	//GSLOG(Warning, TEXT("[SetEquipItemData] Add Data - DBID : %llu"), itemList[i]->GetDBID()());
}

void UGsItemManager::SendAddMessage(TArray<FGsItem*> InAddItemList)
{
	if (0 >= InAddItemList.Num())
		return;

	bool isCheckSpiritShotItem = false;
	TArray<ItemDBId> itemDbidList;
	for (FGsItem* itemData : InAddItemList)
	{
		if(nullptr == itemData)
			continue;

		itemDbidList.Emplace(itemData->GetDBID());

		if (ItemCategorySub::SPIRIT_SHOT == itemData->GetSubCategory())
		{
			if (0 == _curHudSpiritShotDBID)
			{
				isCheckSpiritShotItem = true;
			}
		}
	}

	if (isCheckSpiritShotItem)
	{
		// 천마석 상태 갱신
		PickHudSpiritShotItem();
	}
	
	if (FGsMessageHolder* msg = GMessage())
	{
		FGsAddItemList paramData(itemDbidList);
		msg->GetItem().SendMessage(MessageItem::ADDITEMLIST, paramData);

		msg->GetHudRedDot().SendMessage(MessageContentHUDRedDot::INVEN, true);
		msg->GetContentsHud().SendMessage(MessageContentHud::INVALIDATE_CONSUMEABLE);
		msg->GetUI().SendMessage(MessageUI::INVALIDATE_SLOT_AQUIRE_ITEM, nullptr);
	}
}

/*
void UGsItemManager::SendSentryMessage(FString InMsg)
{
	if (UGsGameInstance* instance = Cast<UGsGameInstance>(GetWorld()->GetGameInstance()))
	{
		USentrySubsystem* SentrySub = instance->GetSubsystem<USentrySubsystem>();
		SentrySub->CaptureMessage(TEXT("SendSentryMessage Test 123123"), ESentryLevel::Info);
	}
}
*/