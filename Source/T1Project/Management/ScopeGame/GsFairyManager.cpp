// Fill out your copyright notice in the Description page of Project Settings.


#include "GsFairyManager.h"
#include "DataSchema/Fairy/GsSchemaFairyCommon.h"
#include "DataSchema/Fairy/GsSchemaFairyDetail.h"
#include "DataSchema/Fairy/GsSchemaFairyLevel.h"
#include "DataSchema/Fairy/GsSchemaFairyCollection.h"
#include "DataSchema/Fairy/GsSchemaFairyEnchant.h"
#include "DataSchema/Fairy/GsSchemaFairySummon.h"
#include "DataSchema/Fairy/GsSchemaFairyBaseEffect.h"
#include "DataSchema/Fairy/GsSchemaFairyCompose.h"
#include "DataSchema/Fairy/GsSchemaVehicleConfig.h"

#include "Fairy/GsFairyData.h"
#include "Fairy/GsFairyCollection.h"
#include "Fairy/GsFairySummonConfirmData.h"

#include "Summon/GsSummonDefine.h"

#include "Management/GsMessageHolder.h"
#include "Message/MessageParam/GsFairyMessageParam.h"

#include "Management/ScopeGlobal/GsGameDataManager.h"
#include "Management/ScopeGlobal/GsUIManager.h"
#include "Management/ScopeGlobal/GsGameFlowManager.h"
#include "Management/ScopeGame/GsGameObjectManager.h"
#include "Management/ScopeGame/GsSummonManager.h"
#include "Management/ScopeGame/GsEventProgressManager.h"
#include "Management/ScopeGame/GsStatChangeNotifyManager.h"

#include "GameObject/ObjectClass/GsGameObjectPlayer.h"
#include "GameObject/Data/GsGameObjectDataLocalPlayer.h"
#include "GameObject/ObjectClass/GsGameObjectLocalPlayer.h"

#include "GameFlow/GsGameFlowGame.h"
#include "GameFlow/GameContents/ContentsGame/GsContentsManagerGame.h"
#include "GameFlow/GameContents/ContentsGame/Hud/GsGameStateHud.h"
#include "GameFlow/GameContents/ContentsGame/Hud/Summon/GsSummonHandler.h"
#include "GameFlow/GameContents/GsContentsMode.h"

#include "Data/GsDataContainManager.h"
#include "Data/GsGlobalConstant.h"

#include "EventProgress/GsEventProgressDefine.h"
#include "Message/MessageParam/GsItemContentsMessageParam.h"

#include "UI/UIContent/Helper/GsUIHelper.h"
#include "UI/UIContent/Tray/GsUITrayTickerCollectionMulti.h"
#include "UI/UIContent/Popup/GsUIPopupReceiveAchievementRewardsAll.h"
#include "UI/UIControlLib/TextDecorator/GsRichTextBlockLinkDecorator.h"
#include "Components/RichTextBlock.h"
#include "Components/RichTextBlockDecorator.h"

#include "Preset/GsPresetHelper.h"

#include "Algo/ForEach.h"
#include "Algo/Transform.h"
#include "Algo/Copy.h"

#include "Math/UnrealMathUtility.h"
#include "DataSchema/Skill/Passivity/GsSchemaPassivitySet.h"
#include "DataSchema/Skill/Passivity/GsSchemaPassivityEffectStatSet.h"

static TMap<FairyGrade, FString> FairyGradeKey = { 
	{FairyGrade::NORMAL, TEXT("NORMAL")}, {FairyGrade::MAGIC, TEXT("MAGIC")}, {FairyGrade::RARE, TEXT("RARE")}, 
	{FairyGrade::EPIC, TEXT("EPIC")}, {FairyGrade::UNIQUE, TEXT("UNIQUE")}, {FairyGrade::LEGEND, TEXT("LEGEND")} };

void UGsFairyManager::Initialize()
{
	InitFairyTable();

	_contentsStatAll.Empty();

	if (auto gameDataManager = GGameData())
	{
		if (gameDataManager->IsFairyDataComplete())
		{
			UpdateFairyDatatoItem(
				gameDataManager->GetFairyRawData(),
				gameDataManager->GetFairyRawDataSize(), true);

			UpdateFairySummonWaitingItemList(
				gameDataManager->GetFairySummonWaitingList(), 
				gameDataManager->GetFairySummonWaitingsPacketTime());

			gameDataManager->ResetFairyData();
		}

		UpdateFairyCollectionCompleteList(gameDataManager->GetFairyCollectionCompleteList());
		UpdateFairyCollectionCompleteConditionList(gameDataManager->GetFairyCollectionCompleteConditionList());

		gameDataManager->ResetFairyCollectionData();
	}


	auto& fairyMsgManager = GMessage()->GetFairy();
	_actMsgHandler = fairyMsgManager.AddUObject(MessageFairy::SUMMON_COMPLETE,
		this, &UGsFairyManager::OnSummonComplete);

	auto& contentsHud = GMessage()->GetContentsHud();
	_hudMsgHandler = contentsHud.AddUObject(MessageContentHud::INVALIDATE_STAT,
		this, &UGsFairyManager::OnInvalidCombatPower);
}

void UGsFairyManager::Finalize()
{
	auto& fairyMsgManager = GMessage()->GetFairy();
	fairyMsgManager.Remove(_actMsgHandler);	

	_fairyDictionaryList.Reset();
	_fairyCollectionList.Reset();

	_fairyDictionaryData.Reset();
	_fairyCollectionData.Reset();

	_fairyItem.Reset();
	_equipFairy.Reset();

	_contentsStatAll.Empty();
}

void UGsFairyManager::InitFairyTable()
{
	TArray<const FGsSchemaFairyCommon*> fairyCommonList;
	TArray<const FGsSchemaFairyCollection*> fairyCollectionList;
	TMap<FairyGrade, TArray<FGsSchemaFairyEffect>> fairyGradeEffect;

	if (const UGsTable* table = FGsSchemaFairyBaseEffect::GetStaticTable())
	{
		TArray<const FGsSchemaFairyBaseEffect*> list;
		table->GetAllRows<FGsSchemaFairyBaseEffect>(list);
		for (const auto row : list)
		{
			fairyGradeEffect.Add(row->grade, row->effect);
		}
	}

	if (const UGsTable* table = FGsSchemaFairyCommon::GetStaticTable())
	{
		table->GetAllRows<FGsSchemaFairyCommon>(fairyCommonList);
	}

	if (const UGsTable* table = FGsSchemaFairyCollection::GetStaticTable())
	{
		table->GetAllRows<FGsSchemaFairyCollection>(fairyCollectionList);
	}

	TArray<bool> ComposableGradeList = { false, false, false, false, false, false, false, false };

	if (const UGsTable* table = FGsSchemaFairyCompose::GetStaticTable())
	{
		TArray<const FGsSchemaFairyCompose*> fairyComposeData;
		table->GetAllRows<FGsSchemaFairyCompose>(fairyComposeData);

		for (auto grade : { FairyGrade::LEGEND, FairyGrade::UNIQUE, FairyGrade::EPIC, FairyGrade::RARE, FairyGrade::MAGIC, FairyGrade::NORMAL })
		{
			ComposableGradeList[static_cast<int>(grade)] = false;
			if (fairyComposeData.FindByPredicate([grade](auto e) { return e->grade == grade;}) != nullptr)
				ComposableGradeList[static_cast<int>(grade)] = true;
		}
	}

	for (const auto row : fairyCommonList)
	{
		FairyId fairyID = row->id;

		TSharedPtr<FGsFairyData> fairyDicElem = MakeShared<FGsFairyData>(row);
		_fairyDictionaryData.Insert(fairyDicElem, 0);
		_fairyDictionaryList.Add(fairyID, fairyDicElem);

		if (auto iter = fairyGradeEffect.Find(fairyDicElem->GetFairyGrade()))
		{
			fairyDicElem->_fairyBaseEffects = *iter;
		}
		fairyDicElem->_isEnableCompose = ComposableGradeList[static_cast<int>(fairyDicElem->GetFairyGrade())];
	}

	//sort 사용하면 위에서 읽어들인 순서가 유지되지 않음. 순서를 유지하면서 grade sort를 미리 해둔다.
	_fairyDictionaryData.StableSort([](TWeakPtr<FGsFairyData> le, TWeakPtr<FGsFairyData> re) {

		auto lData = le.Pin();
		auto rData = re.Pin();

		auto lGrade = lData->GetFairyGrade();
		auto rGrade = rData->GetFairyGrade();

		auto lTier = lData->GetFairyTier();
		auto rTier = rData->GetFairyTier();

		if (lGrade != rGrade) return lGrade > rGrade; //grade 가 높은 순
		if (lTier != rTier) return lTier > rTier; //tier가 높은 순
		return lData->GetId() > rData->GetId(); //id가 낮은 순 -> 높은 순으로 변경 24.01.24
	});

	_fairyIdCollectionList.Add(INVALID_FAIRY_ID, TArray<TWeakPtr<FGsFairyCollection>>());

	//차후 패킷으로 full list가 오면 아래는 삭제
	for (const auto row : fairyCollectionList)
	{
		TSharedPtr<FGsFairyCollection> fairyCollection = MakeShared<FGsFairyCollection>(row);
		_fairyCollectionData.Add(fairyCollection);
		_fairyCollectionList.Add(row->id, fairyCollection);

		for (auto& item : fairyCollection->GetCollectionList())
		{
			_fairyConditionList.FindOrAdd(item._conditionId).Emplace(fairyCollection);

			auto& idList = _fairyIdCollectionList.FindOrAdd(item._id);
			if (idList.Find(fairyCollection) == INDEX_NONE)
			{
				idList.Emplace(fairyCollection);
			}
		}
	}
}

int UGsFairyManager::GetCollection(TArray<TWeakPtr<FGsFairyCollection>>& list) const
{
	list.Append(_fairyCollectionData);
	list.StableSort([](auto re, auto pe) {
		return re.IsValid() && re.Pin()->_isNew && pe.IsValid() && !pe.Pin()->_isNew;
	});

	return list.Num();
}

int32 UGsFairyManager::GetGradeFairyDictionary(FairyGrade grade, TArray<TWeakPtr<FGsFairyData>>& list, bool equipApply)
{
	for (auto data : _fairyDictionaryData)
	{
		if (FairyGrade::MAX == grade || data->GetFairyGrade() == grade)
			list.Add(data);
	}

	auto equipFairy = _equipFairy;
	list.StableSort([equipFairy, equipApply](TWeakPtr<FGsFairyData> le, TWeakPtr<FGsFairyData> re) {

			auto lData = le.Pin();
			auto rData = re.Pin();

			if (equipApply && (lData->_fairyItem == equipFairy || rData->_fairyItem == equipFairy))
			{
				return lData->_fairyItem == equipFairy; //1. equip 우선
			}

			auto lCollected = lData->isCollectedFairy();
			auto rCollected = rData->isCollectedFairy();

			if (lCollected != rCollected)
			{
				return lCollected; //2. 획득
			}

			auto lGrade = lData->GetFairyGrade();
			auto rGrade = rData->GetFairyGrade();

			if (lGrade != rGrade)
			{
				return lGrade > rGrade; // 3. grade 가 높은 순
			}

			auto lTier = lData->GetFairyTier();
			auto rTier = rData->GetFairyTier();

			if (lTier != rTier)
			{
				return lTier > rTier; //4. tier가 높은 순
			}

			return lData->GetId() > rData->GetId(); // id가 높은 순
		});

	return list.Num();
}

//보유한 페어리이고, 해당 grade와 그 페어리에 맞는 테이블이 있어야 한다.
const FGsSchemaFairyEnchant* UGsFairyManager::GetFairyEnchantData(FairyId fairyId, FairyLevelId level /*= INVALID_FAIRY_LEVEL*/) const
{
	if (auto fairyData = GetFairyData(fairyId).Pin())
	{
		if (fairyData->isCollectedFairy())
		{
			if (auto keyIter = FairyGradeKey.Find(fairyData->GetFairyGrade()))
			{
				const FGsSchemaFairyEnchant* outData = nullptr;
				if (FGsSchemaFairyEnchant::GetStaticTable()->FindRow(**keyIter, outData))
				{
					auto lv = level == INVALID_FAIRY_LEVEL ? fairyData->GetCurrentLevel() : level;
					if(outData->enchantLevelList.FindByPredicate([lv](auto& e) {return e.level == lv; }))
						return outData;
				}
			}
		}
	}
	return nullptr;
}

const TWeakPtr<FGsFairyData> UGsFairyManager::GetFairyData(FairyId fairyId) const
{
	if (auto iter = _fairyDictionaryList.Find(fairyId))
		return *iter;

	return nullptr;
}

const TWeakPtr<FGsFairyCollection> UGsFairyManager::GetFairyCollectionData(CollectionId collectionId)
{
	if (auto iter = _fairyCollectionList.Find(collectionId))
		return *iter;

	return nullptr;
}

int UGsFairyManager::GetOwnItemCollectionList(OUT TArray<TWeakPtr<FGsFairyCollection>>& collections, FairyId fairyId, FairyLevelId lv /*= INVALID_FAIRY_LEVEL*/) const
{
	if (auto iter = _fairyIdCollectionList.Find(fairyId))
	{
		if (lv == INVALID_FAIRY_LEVEL)
		{
			collections.Append(*iter);
		}
		else
		{
			collections.Append((*iter).FilterByPredicate([fairyId, lv](auto collect) {
				return collect.IsValid() && collect.Pin()->GetCollectionList().FindByPredicate([fairyId, lv](auto& e) {
					return fairyId == e._id && lv == e._needLv; }) != nullptr; }));
		}
	}

	return collections.Num();
}

const FGsSchemaVehicleConfig* UGsFairyManager::GetVehicleConfigTable()
{
	if (nullptr == _fairyVehicleConfigTable)
	{
		// 여러개 있으면 어떻하지...
		const UGsTableVehicleConfig* table = Cast<UGsTableVehicleConfig>(FGsSchemaVehicleConfig::GetStaticTable());
		if (table)
		{
			TArray<const FGsSchemaVehicleConfig*> listVehicleData;
			table->GetAllRows(listVehicleData);
			if (listVehicleData.Num() > 0)
			{
				_fairyVehicleConfigTable = listVehicleData[0];
			}
		}
	}
	return _fairyVehicleConfigTable;
}

FairyId UGsFairyManager::GetFairyId(PresetId InPresetId)
{
	for (TWeakPtr<FGsFairyItem> fairyItem : _fairyItem)
	{
		if (fairyItem.Pin()->GetPresetValue() & FGsPresetHelper::GetPresetValueFromId(InPresetId))
		{
			return fairyItem.Pin()->GetFaryID();
		}
	}

	return INVALID_FAIRY_ID;
}

TSet<StatType>& UGsFairyManager::ContentsStatAll() const
{
	if (0 >= _contentsStatAll.Num())
	{
		for (const auto& item : _fairyCollectionData)
		{
			if (item.IsValid())
			{
				for (PassivityId passivityId : item.Get()->GetRewardPassivity())
				{
					const FGsSchemaPassivitySet* passivityData = UGsTableUtil::FindRowById<UGsTablePassivitySet, FGsSchemaPassivitySet>(passivityId);
					if (0 < passivityData->effectInfoList.Num())
					{
						int effectId = passivityData->effectInfoList[0].effectId;
						const FGsSchemaPassivityEffectStatSet* statData
							= UGsTableUtil::FindRowById<UGsTablePassivityEffectStatSet, FGsSchemaPassivityEffectStatSet>(effectId);
						for (const auto& stat : statData->param)
						{
							_contentsStatAll.Emplace(stat.type);
						}
					}
				}
			}
		}
	}

	return _contentsStatAll;
}

void UGsFairyManager::InitFairyCollectionList()
{
	for (auto e : _fairyConditionList)
		e.Value.Reset();

	_fairyConditionList.Reset();
	_fairyCollectionList.Reset();
	_fairyCollectionData.Reset();
}

void UGsFairyManager::SetEffectPlaying(bool run /*= true*/, bool saveCollectionStat /*= false*/)
{
	_isRunningSummon = run;

	if (saveCollectionStat)
	{
		GSStatChangeNotify()->Save(StatUpdateReason::FAIRY_COLLECTION);
	}
}

void UGsFairyManager::StartEvent(EGsEventProgressType InStartType, EGsEventProgressType InPreType)
{
	if (InPreType == EGsEventProgressType::SAVE_BATTERY || 
		InPreType == EGsEventProgressType::SUMMON_ROOM)
	{
		return;
	}

	if (InStartType == EGsEventProgressType::FAIRY_ROOM)
	{
		GMessage()->GetContents().SendMessage(MessageContents::ACTIVE_WINDOW_FAIRY);

		if (_reserveTab.IsValid())
		{
			GMessage()->GetFairy().SendMessage(MessageFairy::INITTAB_CHANGE, _reserveTab.Get());
			_reserveTab = nullptr;
		}
	}
}

void UGsFairyManager::FinishEvent(EGsEventProgressType InFinishType, EGsEventProgressType InNextType)
{
	if (InNextType == EGsEventProgressType::SAVE_BATTERY ||
		InNextType == EGsEventProgressType::SUMMON_ROOM)
	{
		return;
	}

	if (InFinishType == EGsEventProgressType::FAIRY_ROOM)
	{
		GUI()->CloseByKeyName(TEXT("WindowFairy"));
		GMessage()->GetContents().SendMessage(MessageContents::ACTIVE_HUD);
	}
}

void UGsFairyManager::FairyEquipChange(FairyId fairyId)
{	
	_equipFairy.Reset();
	if (fairyId != INVALID_FAIRY_ID)
	{	
		if (auto data = GetFairyData(fairyId).Pin())
		{
			_equipFairy = data->_fairyItem;
		}
	}

	FGsFairyMsgParamInt param(fairyId);
	GMessage()->GetFairy().SendMessage(MessageFairy::CHANGE_EQUIP, &param);
}

void UGsFairyManager::InitFairyItemList() //list 초기화
{
	_fairyItem.Reset();
}

void UGsFairyManager::UpdateFairyDatatoItem(const uchar* rawData, int32 size, bool isComplete)
{
	InitFairyItemList();

	size_t iterSize = sizeof(FairyData);
	auto fairyDataCount = size / iterSize;

	if (fairyDataCount > 0)
	{
		TArray<FairyData> list;

		for (size_t i = 0; i < fairyDataCount; i++)
		{
			const uchar* rawfairyDatum = rawData + iterSize * i;
			const FairyData* fairyData = reinterpret_cast<const FairyData*>(rawfairyDatum);

			list.Add(*fairyData);
		}

		UpdateFairyItemList(list, isComplete, false);
	}
}

void UGsFairyManager::UpdateFairyItemList(TArray<FairyData>& arrFairyData, bool isComplete, bool isNewAdd)
{
	TArray<FairyId> newList;

	for (auto& data : arrFairyData)
	{
		FairyId fairyID = data.mFairyId;

		if (isNewAdd && !_fairyItem.FindByPredicate([fairyID](auto e) { return e->GetFaryID() == fairyID; }))
		{
			if (auto iter = _fairyDictionaryList.Find(fairyID))
			{
				iter->Pin()->_isNew = true;
				iter->Pin()->_isSummonNew = true;
			}
		}

		UpdateFairyItem(data, false);
		newList.Add(fairyID);
	}

	if (isComplete && !isNewAdd)
	{
		auto equipFairyId = GGameData()->GetUserData()->mLookInfo.mFairy;
		if (equipFairyId != INVALID_FAIRY_ID)
		{
			if (auto data = GetFairyData(equipFairyId).Pin())
			{
				_equipFairy = data->_fairyItem;
			}
		}
	}

	if (isNewAdd)
	{
		_isFairyRedDot = _fairyDictionaryData.FindByPredicate([](auto e) {
			return e->_isNew;
			}) != nullptr;

		
		FGsFairyMsgParamUpdateList param(newList);
		GMessage()->GetFairy().SendMessage(MessageFairy::UPDATED, &param);
		GMessage()->GetHudRedDot().SendMessage(MessageContentHUDRedDot::FAIRY, true);
	}
}

void UGsFairyManager::UpdateAmountFairyItemList(TArray<FairyIdAmountPair>& arr)
{
	for (auto& pair : arr)
	{
		FairyData data;

		data.mFairyId = pair.mFairyId;
		data.mAmount = pair.mAmount;
		data.mLevel = MAX_FAIRY_LEVEL + 1;
		data.mHighestLevel = MAX_FAIRY_LEVEL + 1;
		data.mPresetValue = 1 << MAX_PRESET_ID;

		UpdateFairyItem(data, true);
	}
}

void UGsFairyManager::UpdateFairyItem(FairyData data, bool isNewAdd)
{
	TSharedPtr<FGsFairyItem> item = nullptr;
	FairyId fairyID = data.mFairyId;
	if (auto iter = _fairyDictionaryList.Find(fairyID))
	{
		if (auto preItem = _fairyItem.FindByPredicate([fairyID](auto e) { return e->GetFaryID() == fairyID; }))
		{
			if (data.mAmount > MAX_ITEM_AMOUNT) data.mAmount = (*preItem)->GetAmount();
			if (data.mLevel > MAX_FAIRY_LEVEL)  data.mLevel = (*preItem)->GetCurrentLevel();
			if (data.mHighestLevel > MAX_FAIRY_LEVEL) data.mHighestLevel = (*preItem)->GetHighestLevel();
			if (data.mLevel > data.mHighestLevel) data.mHighestLevel = data.mLevel;
			if (data.mPresetValue == 1 << MAX_PRESET_ID) data.mPresetValue = (*preItem)->GetPresetValue();
			
			(*preItem)->UpdateFairyItem(data);
		}
		else //합성에 의한 새로운 페어리 추가
		{
			if (data.mAmount > MAX_ITEM_AMOUNT) data.mAmount = MIN_ITEM_AMOUNT;
			if (data.mLevel > MAX_FAIRY_LEVEL)  data.mLevel = MIN_FAIRY_LEVEL;
			if (data.mHighestLevel > MAX_FAIRY_LEVEL)  data.mHighestLevel = MIN_FAIRY_LEVEL;
			if (data.mLevel > data.mHighestLevel) data.mHighestLevel = data.mLevel;
			if (data.mPresetValue == 1 << MAX_PRESET_ID) data.mPresetValue = INVALID_PRESET_VALUE;
		
			auto idx = _fairyItem.Add(MakeShared<FGsFairyItem>(fairyID, data));
			iter->Pin()->_fairyItem = _fairyItem[idx];

			if (isNewAdd)
			{
				iter->Pin()->_isNew = true;
				iter->Pin()->_isSummonNew = true;
			}
		}

		if (isNewAdd)
		{
			_isFairyRedDot = _fairyDictionaryData.FindByPredicate([](auto e) {
				return e->_isNew;
				}) != nullptr;

			TArray<FairyId> addList = { fairyID };
			FGsFairyMsgParamUpdateList param(addList);
			GMessage()->GetFairy().SendMessage(MessageFairy::UPDATED, &param); //amount, level 변화도 갱신
			GMessage()->GetHudRedDot().SendMessage(MessageContentHUDRedDot::FAIRY, true);
		}
	}
}

//이 부분은 불릴 일이 없다. Collected 된 아이템은 안 사라짐 - 그냥 둠
void UGsFairyManager::RemoveFairyItem(FairyId id)
{
	if (auto data = GetFairyData(id).Pin())
	{
		if(data->_fairyItem.IsValid())
			_fairyItem.Remove(data->_fairyItem.Pin());
	}
}

void UGsFairyManager::UpdateFairyCollectionCompleteList(const TArray<CollectionId>& arr, bool isUpdate)
{
	TArray<CollectionId> updateList = arr;
	for (CollectionId collectionId : arr)
	{
		if (auto collection = GetFairyCollectionData(collectionId).Pin())
		{	
			collection->SetCollectionComplete(isUpdate);
		}
	}

	TArray<TWeakPtr<FGsFairyCollection>> completeCollections;
	GetCollectionSearchItems(FairySearchType::Complete, FText::GetEmpty(), completeCollections);

	_completeRewards.UpdatePassivity(completeCollections);

	if (isUpdate && updateList.Num() > 0)
	{
		_isFairyCollectionRedDot = _fairyCollectionData.FindByPredicate([](auto e) { return e->_isNew; })  != nullptr;
		
		OpenCollectionTicker(updateList);

		//ui collection updated
		{ 
			FGsFairyMsgParamCollectionList param(updateList, FGsFairyMsgParamCollectionList::CollectionUpdateType::Collection);
			GMessage()->GetFairy().SendMessage(MessageFairy::COLLECTION_UPDATE, &param);
		}

		//hud reddot updated
		{
			GMessage()->GetHudRedDot().SendMessage(MessageContentHUDRedDot::FAIRY, true);
		}

		//passivity collector update
		{ 
			FGsPrimitiveInt32 param(static_cast<int32>(PassivityOwnerType::FAIRY_COLLECTION));
			GMessage()->GetGameObject().SendMessage(MessageGameObject::PASSIVITY_UPDATE_COLLECTION, &param);
		}

		if (_statNotify == NotiProc::None)
		{
			ReserverStatNotify(NotiProc::StatNoti);
			GSStatChangeNotify()->Save(StatUpdateReason::FAIRY_COLLECTION);
		}
	}
}

void UGsFairyManager::UpdateFairyCollectionCompleteConditionList(const TArray<CollectionConditionId>& arr, bool isUpdate)
{
	TArray<CollectionId> updateList;
	for (CollectionConditionId id : arr)
	{
		if (auto iter = _fairyConditionList.Find(id))
		{
			for (auto e : *iter)
			{
				if (auto collection = e.Pin())
				{
					if(collection->CheckCollectionItemComplete(id))
						updateList.Emplace(collection->GetCollectionId());

					collection->IsCompleted();
				}
			}
		}
	}

	if (isUpdate && updateList.Num() > 0)
	{
		FGsFairyMsgParamCollectionList param(updateList, FGsFairyMsgParamCollectionList::CollectionUpdateType::Condition);
		GMessage()->GetFairy().SendMessage(MessageFairy::COLLECTION_UPDATE, &param); //amount, level 변화도 갱신FAIRY_COLLECTION_UPDATE
	}
}

void UGsFairyManager::OnAckEnchantResult(FairyId id, FairyEnchantResult result)
{
	if(result == FairyEnchantResult::NONE)
		SetEffectPlaying(false);

	FGsFairyMsgParamEnchantResult param(id, result);
	GMessage()->GetFairy().SendMessage(MessageFairy::ENCHANT_RESULT, &param);
}

void UGsFairyManager::OnAckCeilingResult(bool success)
{
	FGsFairyMsgParamEnchantResult param(0, success ? FairyEnchantResult::NONE : FairyEnchantResult::DECREASE);
	GMessage()->GetFairy().SendMessage(MessageFairy::CEILING_RESULT, &param);
}

void UGsFairyManager::AckFairyComposeRewardList(TArray<ItemIdAmountPair>& rewardItems)
{
	_isNotifyRewarded = rewardItems.Num() > 0;
	_reserveComposeRewards.Reset();

	Algo::Transform(rewardItems, _reserveComposeRewards, [](auto e) { return TPair<ItemId, ItemAmount>{ e.mItemId, e.mItemAmount }; });
	_reserveComposeRewards.KeyStableSort([](auto le, auto re) { return le < re; });
}

void UGsFairyManager::OnUpdateObtainedFairyList(FairyObtainReason obtainReason, TArray<FairyId>& resultList)
{
	FGsFairyMsgParamObtainList param(obtainReason, resultList);
	GMessage()->GetFairy().SendMessage(MessageFairy::OBTAIN_RESULT, &param);

	if (!OnSummonStart(obtainReason, resultList))
	{
		GMessage()->GetFairy().SendMessage(MessageFairy::SUMMON_COMPLETE, nullptr);
	}
}

void UGsFairyManager::UpdateFairySummonWaitingItemList(TArray<FairyConfirmData> list,  FDateTime nowTime, bool isUpdate)
{
	TArray<FairyConfirmId> changed;

	for (auto& data : list)
	{
		auto id = data.mFairyConfirmId;
		auto probabilityId = data.mFairyProbabilityId;
		auto fairyData = GetFairyData(data.mFairyId).Pin();

		TArray<FGsFairySummonChangeData> summonableList;
		const FGsSchemaFairyProbability* summonProbabilityData{ nullptr };
		const FGsSchemaFairySummonChangeCurrencyInfo* currencyData = nullptr;
		if (fairyData && data.mRemainChangeCount > 0)
		{	
			if (const auto table = Cast<UGsTableFairyProbability>(FGsSchemaFairyProbability::GetStaticTable()))
			{
				if (table->FindRowById(probabilityId, summonProbabilityData))
				{
					TArray<FairyId> excludeFairyList = { data.mFairyId };
					for(auto excludeId : data.mHistoryFairyIdArray)
					if(excludeId != INVALID_FAIRY_ID) excludeFairyList.Add(excludeId);

					Algo::TransformIf(summonProbabilityData->fairyList, summonableList, [&excludeFairyList](auto& e) {
							return  e.fairyId.GetRow() && excludeFairyList.Find(e.fairyId.GetRow()->id) == INDEX_NONE;  }, [](auto& e) { 
							return FGsFairySummonChangeData{ GSFairy()->GetFairyData(e.fairyId.GetRow()->id), e.probability };
						});

				}
			}
		}

		if (auto iter = _fairySummonWaitings.FindByPredicate([id](auto e) { return e->_fairyConfirmId == id; }))
		{
			(*iter)->UpdateItemData(data, nowTime, fairyData, summonableList, summonProbabilityData);
			changed.Add(id);
		}
		else
		{
			_fairySummonWaitings.Add(MakeShared<FGsFairySummonConfirmData>(data, nowTime, fairyData, summonableList, summonProbabilityData));
			if(isUpdate) changed.Add(id);
		}
	}

	for (auto summonId : changed)
	{
		FGsFairyMsgParamFairySummonWait param(summonId);
		GMessage()->GetFairy().SendMessage(MessageFairy::SUMMONWAIT_UPDATE, &param);
	}

	GMessage()->GetHudRedDot().SendMessage(MessageContentHUDRedDot::FAIRY, IsFairySummonItemRedDot());
}

void UGsFairyManager::ChangeFairySummonWaitingItem(FairyConfirmData data, FDateTime nowTime)
{	
	auto id = data.mFairyConfirmId;
	auto probabilityId = data.mFairyProbabilityId;
	auto fairyData = GetFairyData(data.mFairyId).Pin();

	TArray<FGsFairySummonChangeData> summonableList;
	const FGsSchemaFairyProbability* summonProbabilityData{ nullptr };
	const FGsSchemaFairySummonChangeCurrencyInfo* currencyData = nullptr;
	if (fairyData && data.mRemainChangeCount > 0)
	{
		if (const auto table = Cast<UGsTableFairyProbability>(FGsSchemaFairyProbability::GetStaticTable()))
		{
			if (table->FindRowById(probabilityId, summonProbabilityData))
			{
				TArray<FairyId> excludeFairyList = { data.mFairyId };
				for (auto excludeId : data.mHistoryFairyIdArray)
				if (excludeId != INVALID_FAIRY_ID) excludeFairyList.Add(excludeId);

				Algo::TransformIf(summonProbabilityData->fairyList, summonableList, [&excludeFairyList](auto& e) {
					return  e.fairyId.GetRow() && excludeFairyList.Find(e.fairyId.GetRow()->id) == INDEX_NONE;  }, [](auto& e) {
						return FGsFairySummonChangeData{ GSFairy()->GetFairyData(e.fairyId.GetRow()->id), e.probability };
					});

			}
		}
	}

	if (auto iter = _fairySummonWaitings.FindByPredicate([id](auto e) { return e->_fairyConfirmId == id; }))
	{
		(*iter)->UpdateItemData(data, nowTime, fairyData, summonableList, summonProbabilityData);
	}
	else
	{
		_fairySummonWaitings.Add(MakeShared<FGsFairySummonConfirmData>(data, nowTime, fairyData, summonableList, summonProbabilityData));
	}

	// 소환연출(소환석 뽑는부분는 skip, 페어리 나오는 연출부터)
	TArray<FairyId> resultList;
	resultList.Add(data.mFairyId);

	if (resultList.Num() > 0)
	{
		FGsFairyMsgParamObtainList param(FairyObtainReason::NONE, resultList);
		GMessage()->GetFairy().SendMessage(MessageFairy::OBTAIN_RESULT, &param);

		if (!OnSummonStart(FairyObtainReason::COMPOSE, resultList))
		{
			GMessage()->GetFairy().SendMessage(MessageFairy::SUMMON_COMPLETE, nullptr);
		}
	}

	FGsFairyMsgParamFairySummonWait param(id, FGsFairyMsgParamFairySummonWait::SummonUpdateType::Change, data.mFairyId);
	GMessage()->GetFairy().SendMessage(MessageFairy::SUMMONWAIT_UPDATE, &param);
}

void UGsFairyManager::RemoveFairySummonWaitingItem(FairyConfirmId waitingId)
{
	_fairySummonWaitings.RemoveAll([waitingId](auto e) {
		return e->_fairyConfirmId == waitingId; 
	});

	FGsFairyMsgParamFairySummonWait param(waitingId, FGsFairyMsgParamFairySummonWait::SummonUpdateType::Confirm);
	GMessage()->GetFairy().SendMessage(MessageFairy::SUMMONWAIT_UPDATE, &param);
	GMessage()->GetHudRedDot().SendMessage(MessageContentHUDRedDot::FAIRY, IsFairySummonItemRedDot());
}

bool UGsFairyManager::OnSummonStart(FairyObtainReason obtainReason, TArray<FairyId>& resultList)
{
	_isRunningSummon = false;
	while (1)
	{
		if (GMode() == nullptr)
		{
			break;
		}
		FGsGameFlowGame* gameFlow = GMode()->GetGameFlow();
		if (gameFlow == nullptr)
		{
			break;
		}
		FGsContentsManagerGame* contents = gameFlow->GetContentsManagerGame();
		if (contents == nullptr)
		{
			break;
		}
		if (contents->GetCurrentContentsMode() == FGsContentsMode::InGame::ContentsFairy &&
			obtainReason == FairyObtainReason::USE_ITEM)
		{
			// 페어리 ui 닫기
			GUI()->CloseByKeyName("WindowFairy");
		}
		if (contents->GetCurrentContentsMode() == FGsContentsMode::InGame::ContentsRecompose &&
			obtainReason == FairyObtainReason::COMPOSE)
		{
			GSSummon()->SetSummonOriginContent(EGsSummonOriginContentType::SUMMON_RE_COMPOSE);
		}

		// 시연에서 소환 연출 안볼수도 있다고 해서 flag 값으로 뺌
		bool isShowSummon = GData()->GetGlobalData()->_summonIsShow;
		if (isShowSummon && resultList.Num() > 0)
		{
			FGsSummonHandler* summonHandler = UGsSummonManager::GetSummonHandler();//handler가 현재 소유중인지를 알면 좋겠다만...
			if (summonHandler == nullptr)
			{
				break;
			}

			Algo::ForEach(_summonList, [this](auto id) { if (auto iter = _fairyDictionaryList.Find(id)) iter->Pin()->removeNewSummonMark(); });

			_summonList.Reset();
			_summonList.Append(resultList);

			// ack 보다 연출 패킷이 먼저 왔을 때도 isSummonNew set 되어 있도록 - 소환은 연출 패킷이 먼저 오고, 합성이나 교환은 연출이 나중에 온다
			for (auto id : resultList)
			{
				if (auto iter = _fairyDictionaryList.Find(id))
				{
					if(iter->IsValid() && iter->Pin()->isCollectedFairy() == false)
						iter->Pin()->_isSummonNew = true;
				}
			}

			summonHandler->SetSummonFairyInfo(resultList);
			summonHandler->StartSummon(EGsSummonType::SUMMON_FAIRY);

			GUI()->HideTicker();
			_isRunningSummon = true;

			ReserverStatNotify(NotiProc::CmdNoti);

			return true;
		}
		break;
	}
	return false;
}
void UGsFairyManager::OnSummonComplete(const IGsMessageParam* InParam)
{	
	_summonList.Reset();
	Algo::ForEach(_fairyDictionaryData, [](auto e) { e->removeNewSummonMark(); });

	OpenReservedReward();

	GUI()->ShowTicker();

	ReserverStatNotify(NotiProc::None);
	_isRunningSummon = false;
}

void UGsFairyManager::OpenCollectionTicker(TArray<CollectionId> collections)
{
	if (collections.Num() <= 0) return;

	UGsUITrayTickerCollectionMulti* Ticker =
		Cast<UGsUITrayTickerCollectionMulti>(GUI()->OpenAndGetWidget(TEXT("TrayTickerCollectionMulti")).Get());

	for (auto collectionId : collections)
	{
		if (auto collection = GetFairyCollectionData(collectionId).Pin())
		{
			if (Ticker)
			{
				UGsUITrayTickerCollectionMulti::FCollectionTickerInitParam InitData
				{
					collection->GetCollectionName(),
					collection->GetCollectioinRewardsText()
				};
				Ticker->SetData(InitData);
			}
		}
	}
}

void UGsFairyManager::OpenReservedReward()
{
	if (_isNotifyRewarded)
	{
		TMap<uint32, Currency> currencyList;

		auto widget = GUI()->OpenAndGetWidget(TEXT("PopupReceiveAchievementRewardAll")).Get();
		if (auto popup = Cast<UGsUIPopupReceiveAchievementRewardsAll>(widget))
		{
			FText FairyUI_Compose_FailureReward;
			FText::FindText(TEXT("FairyText"), TEXT("FairyUI_Compose_FailureReward"), FairyUI_Compose_FailureReward);

			FText DefaultPopupButtonOK;
			FText::FindText(TEXT("UICommonText"), TEXT("DefaultPopupButtonOK"), DefaultPopupButtonOK);
			popup->SetDetail(FairyUI_Compose_FailureReward, DefaultPopupButtonOK);
			popup->SetData(currencyList, _reserveComposeRewards);
		}
	}

	_isNotifyRewarded = false;
}

bool UGsFairyManager::OnProcessError(PD::Result res)
{
	if (res == PACKET_RESULT_SUCCESS)
	{
		return true;
	}

	if (res == PD::Result::FAIRY_CONFRIM_ERROR_EXPIRED)
	{
		FGsFairyMsgParamFairySummonWait param(INVALID_FAIRY_CONFIRM_ID, FGsFairyMsgParamFairySummonWait::SummonUpdateType::Change, INVALID_FAIRY_ID);
		GMessage()->GetFairy().SendMessage(MessageFairy::SUMMONWAIT_UPDATE, &param);

		FGsUIHelper::TraySectionMessageTickerNetError(PD::Result::FAIRY_CONFRIM_ERROR_EXPIRED);
		return true;
	}

	return false;
}

void UGsFairyManager::OnFairyStateSelectedPage(int32 tab, int32 optData /*= 0*/)
{
	GSEventProgress()->OnReqStartEvent(EGsEventProgressType::FAIRY_ROOM, this);

	_reserveTab = MakeShared<FGsFairyMsgParamChangeTab>(tab, optData);
}

bool UGsFairyManager::IsEnableStackFairy(FairyGrade grade /*= FairyGrade::MAX*/)
{
	return _fairyItem.FindByPredicate([](auto fairy) { return fairy && fairy->GetAmount() >= MAX_FAIRY_STACK_AMOUNT;  }) == nullptr;
}

int32 UGsFairyManager::GetRemainStackCountFairy()
{
	if (_fairyItem.IsValidIndex(0))
	{
		TArray<TWeakPtr<FGsFairyItem>> fairies;
		Algo::Copy(_fairyItem, fairies);

		fairies.StableSort([](TWeakPtr<FGsFairyItem> le, TWeakPtr<FGsFairyItem> re) {
			return le.Pin()->GetAmount() > re.Pin()->GetAmount();
			});

		return MAX_FAIRY_STACK_AMOUNT - fairies[0].Pin()->GetAmount();
	}

	return MAX_FAIRY_STACK_AMOUNT + 1;
}

bool UGsFairyManager::CheckComposeStackableFairy(TMap<FairyId, ItemAmount>& fairyList)
{
	for (auto fairy : _fairyItem)
	{
		int32 removeCount = 0;
		if (auto iterAmount = fairyList.Find(fairy->GetFaryID()))
		{
			removeCount = *iterAmount;
		}
		
		if (FMath::Min(fairy->GetAmount(), MAX_FAIRY_STACK_AMOUNT) - removeCount >= MAX_FAIRY_STACK_AMOUNT)
		{
			return false;
		}
	}

	return true;
}

const FString UGsFairyManager::HYPERLINK_ID = TEXT("summon_popup");
void UGsFairyManager::RegisterHyperLinkDelegate(URichTextBlock* InRichTextBlock)
{
	if (nullptr == InRichTextBlock)
	{
		GSLOG(Warning, TEXT("nullptr == InRichTextBlock"));
		return;
	}

	URichTextBlockDecorator* Decorator = InRichTextBlock->GetDecoratorByClass(UGsRichTextBlockLinkDecorator::StaticClass());
	if (nullptr == Decorator)
	{
		GSLOG(Error, TEXT("nullptr == Decorator"));
		return;
	}

	UGsRichTextBlockLinkDecorator* GsDecorator = Cast<UGsRichTextBlockLinkDecorator>(Decorator);
	if (nullptr == GsDecorator)
	{
		GSLOG(Error, TEXT("nullptr == GsDecorator"));
		return;
	}

	GsDecorator->AddUniqueOnClickDelegate(UGsFairyManager::HYPERLINK_ID, 
		UGsRichTextBlockLinkDecorator::FGsOnClickRichTextBlockLinkDecorator::CreateUObject(this, &UGsFairyManager::OnClickHyperLink));
}

static constexpr TCHAR SUMMON_KEY_CONTENTS_TYPE[] = TEXT("ContentsType");
static constexpr TCHAR SUMMON_VALUE_SUMMON_POPUP[] = TEXT("SummonPopup");
static constexpr TCHAR SUMMON_KEY_SUMMON_TYPE[] = TEXT("SummonType");
static constexpr TCHAR SUMMON_KEY_SUMMON_DETAILID[] = TEXT("DetailId");

void UGsFairyManager::OnClickHyperLink(const TMap<FString, FString>& InParamMap)
{
	auto FindEnumValue = [](IN const FString enumName, IN const FName enumValueName) {

		if (const UEnum* enumPtr = FindObject<UEnum>(ANY_PACKAGE, *enumName, true))
		{
			if (enumPtr->IsValidEnumName(enumValueName))
			{
				return enumPtr->GetValueByName(enumValueName, EGetByNameFlags::None);
			}
		}
		return (int64)-1;
	};

	if (const auto iter = InParamMap.Find(SUMMON_KEY_CONTENTS_TYPE))
	{
		FString contentsTypeStr = *iter;
		if (contentsTypeStr.Compare(SUMMON_VALUE_SUMMON_POPUP, ESearchCase::IgnoreCase) == 0)
		{
			IGsBasePortrait::PortraitType type = IGsBasePortrait::PortraitType::Fairy;
			int32 detailId = 0;

			if (auto typeIter = InParamMap.Find(SUMMON_KEY_SUMMON_TYPE))
			{
				type = static_cast<IGsBasePortrait::PortraitType>(FindEnumValue(TEXT("IGsBasePortrait::PortraitType"), FName(*typeIter)));
			}
			if (auto idIter = InParamMap.Find(SUMMON_KEY_SUMMON_DETAILID))
			{
				FString strDetailId = *idIter;
				detailId = FCString::Atoi(*strDetailId);
			}

			if (nullptr != GMessage())
			{
				FGsItemDetailSummonTickerParam param{ static_cast<uint8>(type) , detailId };
				GMessage()->GetItemContents().SendMessage(MessageContentItem::DETAIL_OPEN_POPUP_SUMMONDETAIL, &param);
			}
		}
	}
}

FText UGsFairyManager::MakeRichText(IGsBasePortrait::PortraitType type, int32 detailId) const
{
	auto FindEnumName = [](IN const FString enumName, IN const int64 enumValue) {

		if (const UEnum* enumPtr = FindObject<UEnum>(ANY_PACKAGE, *enumName, true))
		{
			if (enumPtr->IsValidEnumValue(enumValue))
			{
				return enumPtr->GetNameByValue(enumValue);
			}
		}
		return FName("");
	};

	FName typeName = FindEnumName(TEXT("IGsBasePortrait::PortraitType"), static_cast<int64>(type));
	TMap<FString, FString> parameterMap = 
	{
		{SUMMON_KEY_CONTENTS_TYPE, SUMMON_VALUE_SUMMON_POPUP},
		{SUMMON_KEY_SUMMON_TYPE, *typeName.ToString()},
		{SUMMON_KEY_SUMMON_DETAILID, LexToString(detailId)},
	};

	FText titleText;
	FText::FindText(TEXT("ItemUIText"), TEXT("UI_Button_SummonDetailInfo"), titleText);
	
	return UGsRichTextBlockLinkDecorator::MakeLinkText(UGsFairyManager::HYPERLINK_ID, titleText, parameterMap);
}

void UGsFairyManager::ReserverStatNotify(NotiProc proc)
{
	_statNotify = proc;
}

void UGsFairyManager::CollecitonStatNotify()
{
	_statNotify = NotiProc::None;
	GSStatChangeNotify()->CheckNotify(StatUpdateReason::FAIRY_COLLECTION);
}

void UGsFairyManager::OnInvalidCombatPower()
{
	if (_statNotify == NotiProc::StatNoti)
	{
		CollecitonStatNotify();
	}
}
