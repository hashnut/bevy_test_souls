#include "GsCostumeManager.h"
#include "Shared/Client/SharedEnums/SharedCreatureEnum.h"

#include "DataSchema/Costume/GsSchemaCostumeCommon.h"
#include "DataSchema/Costume/GsSchemaCostumeCollection.h"
#include "DataSchema/Costume/GsSchemaCostumeSummon.h"
#include "DataSchema/Costume/GsSchemaCostumeCompose.h"
#include "DataSchema/GameObject/GsSchemaPartsShapeData.h"
#include "DataSchema/Item/GsSchemaItemIngredientDetail.h"
#include "DataSchema/Item/GsSchemaItemEffectChangeCostume.h"
#include "DataSchema/Skill/Passivity/GsSchemaPassivitySet.h"
#include "DataSchema/Skill/Passivity/GsSchemaPassivityEffectStatSet.h"
#include "DataSchema/UI/GsSchemaWeaponTypeMenu.h"

#include "Costume/GsCostumeData.h"
#include "Costume/GsCostumeCollection.h"
#include "Costume/GsCostumeSummonConfirmData.h"

#include "Management/GsMessageHolder.h"
#include "Message/MessageParam/GsUIMessageParam.h"
#include "Message/MessageParam/GsCostumeMessageParam.h"

#include "Item/GsItemManager.h"
#include "Management/ScopeGlobal/GsGameDataManager.h"
#include "Management/ScopeGlobal/GsUIManager.h"
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

#include "../UTIL/GsTableUtil.h"

#include "UI/UIContent/Helper/GsUIHelper.h"
#include "UI/UIContent/Tray/GsUITrayTickerCollectionMulti.h"
#include "UI/UIContent/Popup/GsUIPopupReceiveAchievementRewardsAll.h"

#include "Preset/GsPresetHelper.h"

#include "Algo/Transform.h"
#include "Algo/Copy.h"

#include "Math/UnrealMathUtility.h"

#include "EnumRange.h"

void UGsCostumeManager::Initialize()
{
	InitCostumeTable();

	_contentsStatAll.Empty();

	if (auto gameDataManager = GGameData())
	{
		if (gameDataManager->IsCostumeDataComplete())
		{
			UpdateCostumeDatatoItem(
				gameDataManager->GetCostumeRawData(),
				gameDataManager->GetCostumeRawDataSize(), true);

			UpdateCostumeSummonWaitingItemList(
				gameDataManager->GetCostumeSummonWaitingList(),
				gameDataManager->GetCostumeSummonWaitingsPacketTime());

			gameDataManager->ResetCostumeData();
		}

		UpdateCollectionCompleteList(gameDataManager->GetCostumeCollectionCompleteList());
		UpdateCollectionCompleteConditionList(gameDataManager->GetCostumeCollectionCompleteConditionList());

		gameDataManager->ResetCostumeCollectionData();

		auto& costumeMsgManager = GMessage()->GetCostume();
		_actMsgHandler = costumeMsgManager.AddUObject(MessageCostume::SUMMON_COMPLETE,
			this, &UGsCostumeManager::OnSummonComplete);

		auto& contentsHud = GMessage()->GetContentsHud();
		_hudMsgHandler = contentsHud.AddUObject(MessageContentHud::INVALIDATE_STAT,
			this, &UGsCostumeManager::OnInvalidCombatPower);
	}
}

void UGsCostumeManager::Finalize()
{
	auto& costumeMsgMng = GMessage()->GetCostume();
	costumeMsgMng.Remove(_actMsgHandler);

	_costumeDictionaryData.Reset();
	_costumeDictionaryList.Reset();

	_costumeDictionaryData.Reset();
	_costumeDictionaryList.Reset();

	_costumeItems.Reset();
	_equipCostume.Reset();

	_contentsStatAll.Empty();
}

const TWeakPtr<FGsCostumeData> UGsCostumeManager::GetCostumeData(CostumeId id) const
{
	if (auto iter = _costumeDictionaryList.Find(id))
		return *iter;
	return nullptr;
}

int32 UGsCostumeManager::GetGradeCostumeDictionary(CostumeGrade grade, TArray<TWeakPtr<FGsCostumeData>>& list, bool equipApply)
{
	return GetGradeCostumeDictionary(grade, CreatureWeaponType::NONE, list, equipApply);
}

constexpr CreatureWeaponType LastCreatureWeaponType = static_cast<CreatureWeaponType>(static_cast<int>(CreatureWeaponType::HAND) - 1);
ENUM_RANGE_BY_FIRST_AND_LAST(CreatureWeaponType, CreatureWeaponType::SWORD, LastCreatureWeaponType)
int32 UGsCostumeManager::GetGradeCostumeDictionary(CostumeGrade grade, CreatureWeaponType selectWeapon, TArray<TWeakPtr<FGsCostumeData>>& list, bool equipApply /*= false*/)
{
	static auto IsWeaponTypeValidRange = [](CreatureWeaponType val)->bool
	{
		static TArray<CreatureWeaponType> _range;
		if (_range.Num() <= 0)
		{
			for (auto e : TEnumRange<CreatureWeaponType>()) _range.Add(e);
		}

		return _range.Find(val) != INDEX_NONE;
	};

	if (!IsWeaponTypeValidRange(selectWeapon)) selectWeapon = CreatureWeaponType::NONE;

	list.Append(_costumeDictionaryData.FilterByPredicate([grade, selectWeapon](auto e) {
		bool use = e && (CostumeGrade::MAX == grade || e->GetCostumeGrade() == grade);
		return use && (selectWeapon == CreatureWeaponType::NONE || e->IsRecommendedWeaponType(selectWeapon));
		}));

	auto equipCostume = _equipCostume;
	list.StableSort([equipCostume, equipApply](TWeakPtr<FGsCostumeData> re, TWeakPtr<FGsCostumeData> pe) {
		auto rData = re.Pin();
		auto pData = pe.Pin();

		if (equipApply && (rData->_costumeItem == equipCostume || pData->_costumeItem == equipCostume))
		{
			return rData->_costumeItem == equipCostume; //1. equip 우선
		}

		auto rCollected = rData->isCollectedCostume();
		auto pCollected = pData->isCollectedCostume();

		if (rCollected != pCollected)
		{
			return rCollected; //2. 획득
		}

		auto rGrade = rData->GetCostumeGrade();
		auto pGrade = pData->GetCostumeGrade();

		if (rGrade != pGrade)
		{
			return rGrade > pGrade; // 3. grade 가 높은 순
		}

		auto rTier = rData->GetCostumeTier();
		auto pTier = pData->GetCostumeTier();

		if (rTier != pTier)
		{
			return rTier > pTier; //4. tier가 높은 순
		}

		return rData->GetId() > pData->GetId(); // id가 낮은 순 -> 높은 순으로 변경됨
		});

	return list.Num();

}

int32 UGsCostumeManager::GetGradeAppearenceCostume(CostumeGrade grade, TArray<TWeakPtr<FGsCostumeData>>& list)
{
	list.Append(_costumeDictionaryData.FilterByPredicate([grade](auto e) {
		return e && e->IsUsableAppearanceCostume() && (CostumeGrade::MAX == grade || e->GetCostumeGrade() == grade); }));

	auto equipCostume = _equipCostume;
	list.StableSort([equipCostume](TWeakPtr<FGsCostumeData> le, TWeakPtr<FGsCostumeData> re) {
		auto lData = le.Pin();
		auto rData = re.Pin();

		if (lData->_costumeItem == equipCostume || rData->_costumeItem == equipCostume)
		{
			return lData->_costumeItem == equipCostume; //1. equip 우선
		}

		auto lCollected = lData->isCollectedCostume();
		auto rCollected = rData->isCollectedCostume();

		if (lCollected != rCollected)
		{
			return lCollected; //2. 획득
		}

		auto lGrade = lData->GetCostumeGrade();
		auto rGrade = rData->GetCostumeGrade();

		if (lGrade != rGrade)
		{
			return lGrade > rGrade; // 3. grade 가 높은 순
		}

		auto lTier = lData->GetCostumeTier();
		auto rTier = rData->GetCostumeTier();

		if (lTier != rTier)
		{
			return lTier > rTier; //4. tier가 높은 순
		}

		return lData->GetId() > rData->GetId(); // id가 높은 순
		});

	return list.Num();
}

TWeakPtr<FGsCostumeData> UGsCostumeManager::GetWearOnCostumeData()
{
	if (_equipCostume.IsValid())
	{
		return GetCostumeData(_equipCostume.Pin()->GetCostumeID());
	}
	return nullptr;
}

CostumeId UGsCostumeManager::GetCostumeId(PresetId InPresetId)
{
	for (auto costumeItem : _costumeItems)
	{
		if (costumeItem->GetPresetValue() & FGsPresetHelper::GetPresetValueFromId(InPresetId))
		{
			return costumeItem->GetCostumeID();
		}
	}

	return INVALID_COSTUME_ID;
}

void UGsCostumeManager::CostumeEquipChange(CostumeId costumeId)
{
	_equipCostume.Reset();
	_equipCostumeActiveSkillId = 0;
	if (costumeId != 0)
	{
		if (auto data = GetCostumeData(costumeId).Pin())
		{
			_equipCostume = data->_costumeItem;

			if (const auto activeSkill = data->GetCostumeActiveSkill())
			{
				if (auto userSkill = activeSkill->userSkillSetId.GetRow())
				{
					const auto skillData = (GGameData()->GetUserData()->mGender == CreatureGenderType::MALE) ?
						userSkill->maleSkillId.GetRow() : userSkill->femaleSkillId.GetRow();

					_equipCostumeActiveSkillId = skillData->id;
				}
			}
		}
	}

	FGsUIMsgParamInt param(costumeId);
	GMessage()->GetCostume().SendMessage(MessageCostume::CHANGE_WEARON, &param);
}
void UGsCostumeManager::AppearanceChange(CostumeId costumeId, int equipType /*= 0*/)
{
	_appearanceCostume.Reset();
	if (costumeId != 0)
	{
		if (auto data = GetCostumeData(costumeId).Pin())
		{
			_appearanceCostume = data->_costumeItem;
			if (auto costumeItem = _appearanceCostume.Pin())
			{
				costumeItem->SetEquipState(CostumeParts(equipType));
			}

			if (const auto activeSkill = data->GetCostumeActiveSkill())
			{
				if (auto userSkill = activeSkill->userSkillSetId.GetRow())
				{
					const auto skillData = (GGameData()->GetUserData()->mGender == CreatureGenderType::MALE) ?
						userSkill->maleSkillId.GetRow() : userSkill->femaleSkillId.GetRow();

					_equipCostumeActiveSkillId = skillData->id;
				}
			}
		}
	}

	FGsUIMsgParamInt param(costumeId);
	GMessage()->GetCostume().SendMessage(MessageCostume::CHANGE_WEARON, &param);
}

void UGsCostumeManager::InitCostumeItemList()
{
	_costumeItems.Reset();
}

void UGsCostumeManager::UpdateCostumeDatatoItem(const uchar* rawData, int32 size, bool isComplete)
{
	InitCostumeItemList();

	auto iterSize = sizeof(CostumeData);
	auto costumeDataCount = size / iterSize;

	if (costumeDataCount > 0)
	{
		TArray<CostumeData> list;

		for (int i = 0; i < costumeDataCount; i++)
		{
			const uchar* rawCostumeDatum = rawData + iterSize * i;
			const CostumeData* costumeData = reinterpret_cast<const CostumeData*>(rawCostumeDatum);

			list.Add(*costumeData);
		}

		UpdateCostumeItemList(list, isComplete);
	}
}

void UGsCostumeManager::UpdateCostumeItemList(TArray<CostumeData>& arr, bool isComplete, bool isNewAdd)
{
	TArray<CostumeId> newList;

	for (auto& data : arr)
	{
		if (data.mIsActivity == false) continue;

		CostumeId costumeId = data.mCostumeId;

		if (isNewAdd && !_costumeItems.FindByPredicate([costumeId](auto e) { return e->GetCostumeID() == costumeId; }))
		{
			if (auto iter = _costumeDictionaryList.Find(costumeId))
			{
				iter->Pin()->_isNew = true;
				iter->Pin()->_isSummonNew = true;
			}
		}

		UpdateCostumeItem(data, false);
		newList.Add(costumeId);
	}

	if (isComplete && !isNewAdd)
	{
		auto equipCostumeId = GGameData()->GetUserData()->mLookInfo.mCostumeId;
		auto appearanceCostumeId = GGameData()->GetUserData()->mLookInfo.mTransformCostumeId;

		_equipCostumeActiveSkillId = 0;
		if (equipCostumeId != INVALID_COSTUME_ID)
		{
			if (auto data = GetCostumeData(equipCostumeId).Pin())
			{
				_equipCostume = data->_costumeItem;

				if (const auto activeSkill = data->GetCostumeActiveSkill())
				{
					if (auto userSkill = activeSkill->userSkillSetId.GetRow())
					{
						const auto skillData = (GGameData()->GetUserData()->mGender == CreatureGenderType::MALE) ?
							userSkill->maleSkillId.GetRow() : userSkill->femaleSkillId.GetRow();

						_equipCostumeActiveSkillId = skillData->id;
					}
				}
			}
		}

		if (appearanceCostumeId != INVALID_COSTUME_ID)
		{
			if (auto data = GetCostumeData(appearanceCostumeId).Pin())
			{
				_appearanceCostume = data->_costumeItem;
				if (auto costumeItem = _appearanceCostume.Pin())
				{
					// 차후 적용 타입에 대한 선택
				//auto state = GGameData()->GetUserData()->mLookInfo.Appearance.State;
					costumeItem->SetEquipState(CostumeParts::FULLSET);
				}
			}
		}
	}

	if (isNewAdd)
	{
		_isCostumeRedDot = _costumeDictionaryData.FindByPredicate([](auto e) {
			return e->_isNew;
			}) != nullptr;


		FGsUIMsgMsgUpdateList param(newList);
		GMessage()->GetCostume().SendMessage(MessageCostume::UPDATED, &param);
		GMessage()->GetHudRedDot().SendMessage(MessageContentHUDRedDot::COSTUME, true);
	}
}

void UGsCostumeManager::UpdateAmountCostumeItemList(TArray<CostumeIdAmountPair>& arr, bool isNewAdd)
{
	TArray<CostumeId> newList;

	for (auto& data : arr)
	{
		CostumeId costumeId = data.mCostumeId;

		if (auto costumeItem = _costumeItems.FindByPredicate([costumeId](auto e) { return e->GetCostumeID() == costumeId; }))
		{
			(*costumeItem)->SetAmount(data.mAmount);
		}

		newList.Add(costumeId);
	}

	if (isNewAdd)
	{
		FGsUIMsgMsgUpdateList param(newList);
		GMessage()->GetCostume().SendMessage(MessageCostume::UPDATED, &param);
	}
}

void UGsCostumeManager::UpdateCostumeItem(CostumeData& data, bool isNewAdd)
{
	if (data.mIsActivity == false) return;

	CostumeId costumeId = data.mCostumeId;
	if (auto iter = _costumeDictionaryList.Find(costumeId))
	{
		if (auto preItem = _costumeItems.FindByPredicate([costumeId](TSharedPtr<FGsCostumeItem> e) { return e->GetCostumeID() == costumeId; }))
		{
			if (data.mAmount > MAX_ITEM_AMOUNT) data.mAmount = (*preItem)->GetAmount();
			(*preItem)->SetAmount(data.mAmount);
			//(*preItem)->SetAutoUseActiveSkill(data.mActiveSkillAutoUse); //equip 되지 않는 이상 변동없으므로 처리하지 않음
			(*preItem)->SetPresetValue(data.mPresetValue);
		}
		else
		{
			if (data.mAmount > MAX_ITEM_AMOUNT) data.mAmount = MIN_ITEM_AMOUNT;

			auto idx = _costumeItems.Add(MakeShared<FGsCostumeItem>(data));
			iter->Pin()->_costumeItem = _costumeItems[idx];

			if (isNewAdd)
			{
				iter->Pin()->_isNew = true;
				iter->Pin()->_isSummonNew = true;
			}
		}

		if (isNewAdd)
		{
			_isCostumeRedDot = _costumeDictionaryData.FindByPredicate([](auto e) {
				return e->_isNew;
				}) != nullptr;

			TArray<CostumeId> addList = { costumeId };
			FGsUIMsgMsgUpdateList param(addList);
			GMessage()->GetCostume().SendMessage(MessageCostume::UPDATED, &param); //amount, level 변화도 갱신
			GMessage()->GetHudRedDot().SendMessage(MessageContentHUDRedDot::COSTUME, true);
		}
	}
}

void UGsCostumeManager::RemoveCostumeItem(CostumeId id, bool isUpdate)
{
	if (auto data = GetCostumeData(id).Pin())
	{
		if (data->_costumeItem.IsValid())
		{
			data->_isNew = false;
			_costumeItems.Remove(data->_costumeItem.Pin());

			if (isUpdate)
			{
				TArray<CostumeId> removeList = { data->_id };
				FGsUIMsgMsgUpdateList param(removeList);
				GMessage()->GetCostume().SendMessage(MessageCostume::UPDATED, &param);
				GMessage()->GetHudRedDot().SendMessage(MessageContentHUDRedDot::COSTUME, true);
			}
		}
	}
}

void UGsCostumeManager::SetCostumeItemIsLearnSkill(CostumeId costumeId, bool isLearn)
{
	if (auto costumeItem = _costumeItems.FindByPredicate([costumeId](TSharedPtr<FGsCostumeItem> e) { return e->GetCostumeID() == costumeId; }))
	{
		(*costumeItem)->SetIsLearnSkill(isLearn);
	}
	//for (TWeakPtr<FGsCostumeItem> costumeItem : _costumeItems) // 하나뿐인데 루프여서 수정합니다
	//{
	//	if (costumeId == costumeItem.Pin()->GetCostumeID())
	//	{
	//		costumeItem.Pin()->SetIsLearnSkill(isLearn);
	//	}
	//}
}

void UGsCostumeManager::InitCostumeTable()
{
	TArray<const FGsSchemaCostumeCommon*> costumeTable;
	TArray<const FGsSchemaCostumeCollection*> costumeCollectionList;

	if (auto table = Cast< UGsTableCostumeCommon>(FGsSchemaCostumeCommon::GetStaticTable()))
	{
		table->GetAllRows(costumeTable);
	}

	if (const UGsTable* table = FGsSchemaCostumeCollection::GetStaticTable())
	{
		table->GetAllRows<FGsSchemaCostumeCollection>(costumeCollectionList);
	}

	TArray<bool> ComposableGradeList = { false, false, false, false, false, false, false, false };

	if (const UGsTable* table = FGsSchemaCostumeCompose::GetStaticTable())
	{
		TArray<const FGsSchemaCostumeCompose*> costumeComposeData;
		table->GetAllRows<FGsSchemaCostumeCompose>(costumeComposeData);

		for (auto grade : { CostumeGrade::LEGEND, CostumeGrade::UNIQUE, CostumeGrade::EPIC, CostumeGrade::RARE, CostumeGrade::MAGIC, CostumeGrade::NORMAL })
		{
			ComposableGradeList[static_cast<int>(grade)] = false;
			if (costumeComposeData.FindByPredicate([grade](auto e) { return e->grade == grade; }) != nullptr)
				ComposableGradeList[static_cast<int>(grade)] = true;
		}
	}

	for (auto e : costumeTable)
	{
		if (auto data = FGsCostumeData::loadCostume(e))
		{
			data->_isEnableCompose = ComposableGradeList[static_cast<int>(data->GetCostumeGrade())];
			_costumeDictionaryData.Add(data);
			_costumeDictionaryList.Add(data->_id, data);
		}
	}

	//sort 사용하면 위에서 읽어들인 순서가 유지되지 않음. 순서를 유지하면서 grade sort를 미리 해둔다.
	_costumeDictionaryData.StableSort([](TWeakPtr<FGsCostumeData> le, TWeakPtr<FGsCostumeData> re) {

		auto lData = le.Pin();
		auto rData = re.Pin();

		auto lGrade = lData->GetCostumeGrade();
		auto rGrade = rData->GetCostumeGrade();

		auto lTier = lData->GetCostumeTier();
		auto rTier = rData->GetCostumeTier();

		if (lGrade != rGrade) return lGrade > rGrade; //grade 가 높은 순
		if (lTier != rTier) return lTier > rTier; //tier가 높은 순
		return lData->GetId() > rData->GetId(); //id가 낮은 순 -> 높은 순
		});

	TArray<TWeakPtr<FGsCostumeCollection>> emptyCollection;
	_costumeIdCollectionList.Add(INVALID_COSTUME_ID, emptyCollection);

	for (const auto row : costumeCollectionList)
	{
		TSharedPtr<FGsCostumeCollection> costumeCollection = MakeShared<FGsCostumeCollection>(row);
		_costumeCollectionData.Add(costumeCollection);
		_costumeCollectionList.Add(row->id, costumeCollection);

		for (auto& item : costumeCollection->GetCollectionList())
		{
			if (_costumeConditionList.Find(item._conditionId) == nullptr)
			{
				TArray<TWeakPtr<FGsCostumeCollection>> arr;
				_costumeConditionList.Add(item._conditionId, arr);
			}

			_costumeConditionList[item._conditionId].Add(costumeCollection);

			if (_costumeIdCollectionList.Find(item._id) == nullptr)
			{
				TArray<TWeakPtr<FGsCostumeCollection>> arr;
				_costumeIdCollectionList.Add(item._id, arr);
			}

			_costumeIdCollectionList[item._id].Add(costumeCollection);
		}
	}

	_constumeMenus.Reset();
	TArray<const FGsSchemaWeaponTypeMenu*> menus;
	if (const auto table = Cast<UGsTableWeaponTypeMenu>(FGsSchemaWeaponTypeMenu::GetStaticTable()))
	{
		table->GetAllRows(menus);

		Algo::TransformIf(menus, _constumeMenus, [](const auto e) {
			return e->option.Contains(EGsHelpContents::Costume) && e->useMenu && 
				   e->iconInfo.FindByPredicate([](auto& info) { return info.iconType == EGsPurposeIcon::Normal; }) != nullptr;
			}, [](const auto e) {
			auto iter = e->iconInfo.FindByPredicate([](auto& info) { return info.iconType == EGsPurposeIcon::Normal; });
			return FGsCostumeWeaponMenuEntry{ e->type, e->name, iter->iconPath, iter->imageSize, iter->tintColor, e->order };
		});

		_constumeMenus.StableSort([](auto& pe, auto& fe) { return pe._order < fe._order; });
	}
}

int UGsCostumeManager::GetCollection(TArray<TWeakPtr<FGsCostumeCollection>>& list) const
{
	list.Append(_costumeCollectionData);
	list.StableSort([](auto re, auto pe) {
		return re.IsValid() && re.Pin()->_isNew && pe.IsValid() && !pe.Pin()->_isNew;
		});

	return list.Num();
}

void UGsCostumeManager::InitCollectionList()
{
	for (auto e : _costumeConditionList)
		e.Value.Reset();

	_costumeConditionList.Reset();
	_costumeCollectionList.Reset();
	_costumeCollectionData.Reset();
}

void UGsCostumeManager::SetEffectPlaying(bool run /*= true*/, bool saveCollectionStat /*= false*/)
{
	_isRunningSummon = run;

	if (saveCollectionStat)
	{
		GSStatChangeNotify()->Save(StatUpdateReason::COSTUME_COLLECTION);
	}
}

const TWeakPtr<FGsCostumeCollection> UGsCostumeManager::GetCollectionData(CollectionId id)
{
	if (auto iter = _costumeCollectionList.Find(id))
		return *iter;

	return nullptr;
}

const TArray<FGsCostumeWeaponMenuEntry>& UGsCostumeManager::GetCostumeSelectMenuData()
{
	return _constumeMenus;
}

const TArray<TWeakPtr<FGsCostumeCollection>>& UGsCostumeManager::GetOwnItemCollectionList(CostumeId costumeId)
{
	if (auto iter = _costumeIdCollectionList.Find(costumeId))
		return *iter;

	return _costumeIdCollectionList[INVALID_COSTUME_ID];
}


void UGsCostumeManager::AckCostumeComposeRewardList(TArray<ItemIdAmountPair>& rewardItems)
{
	_isNotifyRewarded = rewardItems.Num() > 0;
	_reserveComposeRewards.Reset();

	Algo::Transform(rewardItems, _reserveComposeRewards, [](auto e) { return TPair<ItemId, ItemAmount>{ e.mItemId, e.mItemAmount }; });
	_reserveComposeRewards.KeyStableSort([](auto le, auto re) { return le < re; });
}

void UGsCostumeManager::UpdateCollectionCompleteList(const TArray<CollectionId>& arr, bool isUpdate)
{
	TArray<CollectionId> updateList = arr;
	for (CollectionId collectionId : arr)
	{
		if (auto collection = GetCollectionData(collectionId).Pin())
		{
			collection->SetCollectionComplete(isUpdate);
		}
	}

	TArray<TWeakPtr<FGsCostumeCollection>> completeCollections;
	GetCollectionSearchItems(CostumeSearchType::Complete, FText::GetEmpty(), completeCollections);

	_completeRewards.UpdatePassivity(completeCollections);

	if (isUpdate && updateList.Num() > 0)
	{
		_isCostumeCollectionRedDot = _costumeCollectionData.FindByPredicate([](auto e) { return e->_isNew; }) != nullptr;

		OpenCollectionTicker(updateList);

		//ui collection updated
		{
			FGsCostumeMsgParamCollectionList param(updateList, FGsCostumeMsgParamCollectionList::CollectionUpdateType::Collection);
			GMessage()->GetCostume().SendMessage(MessageCostume::COLLECTION_UPDATE, &param);
		}

		//hud reddot updated
		{
			GMessage()->GetHudRedDot().SendMessage(MessageContentHUDRedDot::COSTUME, true);
		}

		//passivity collector update
		{
			FGsPrimitiveInt32 param(static_cast<int32>(PassivityOwnerType::COSTUME_COLLECTION));
			GMessage()->GetGameObject().SendMessage(MessageGameObject::PASSIVITY_UPDATE_COLLECTION, &param);
		}

		if (_statNotify == NotiProc::None)
		{
			ReserverStatNotify(NotiProc::StatNoti);
			GSStatChangeNotify()->Save(StatUpdateReason::COSTUME_COLLECTION);
		}
	}
}

void UGsCostumeManager::UpdateCollectionCompleteResetList(const TArray<CollectionId>& arr, bool isUpdate)
{
	TArray<CollectionId> updateList = arr;
	for (CollectionId collectionId : arr)
	{
		if (auto collection = GetCollectionData(collectionId).Pin())
		{
			collection->SetCollectionCompleteReset();
		}
	}

	TArray<TWeakPtr<FGsCostumeCollection>> completeCollections;
	GetCollectionSearchItems(CostumeSearchType::Complete, FText::GetEmpty(), completeCollections);

	_completeRewards.UpdatePassivity(completeCollections);

	if (isUpdate && updateList.Num() > 0)
	{
		_isCostumeCollectionRedDot = _costumeCollectionData.FindByPredicate([](auto e) { return e->_isNew; }) != nullptr;

		//ui collection updated
		{
			FGsCostumeMsgParamCollectionList param(updateList, FGsCostumeMsgParamCollectionList::CollectionUpdateType::Collection);
			GMessage()->GetCostume().SendMessage(MessageCostume::COLLECTION_UPDATE, &param);
		}

		//hud reddot updated
		{
			GMessage()->GetHudRedDot().SendMessage(MessageContentHUDRedDot::COSTUME, true);
		}

		//passivity collector update
		{
			FGsPrimitiveInt32 param(static_cast<int32>(PassivityOwnerType::COSTUME_COLLECTION));
			GMessage()->GetGameObject().SendMessage(MessageGameObject::PASSIVITY_UPDATE_COLLECTION, &param);
		}
	}
}

void UGsCostumeManager::UpdateCollectionCompleteConditionList(const TArray<CollectionConditionId>& arr, bool isUpdate)
{
	TArray<CollectionId> updateList;
	for (CollectionConditionId id : arr)
	{
		if (auto iter = _costumeConditionList.Find(id))
		{
			for (auto e : *iter)
			{
				if (auto collection = e.Pin())
				{
					if (collection->CheckCollectionItemComplete(id))
						updateList.Emplace(collection->GetCollectionId());
				}
			}
		}
	}

	if (isUpdate && updateList.Num() > 0)
	{
		FGsCostumeMsgParamCollectionList param(updateList, FGsCostumeMsgParamCollectionList::CollectionUpdateType::Condition);
		GMessage()->GetCostume().SendMessage(MessageCostume::COLLECTION_UPDATE, &param); //amount, level 변화도 갱신
	}
}

void UGsCostumeManager::RemovedCollectionCompleteConditionList(const TArray<CollectionConditionId>& arr, bool isUpdate)
{
	TArray<CollectionId> updateList;
	for (CollectionConditionId id : arr)
	{
		if (auto iter = _costumeConditionList.Find(id))
		{
			for (auto e : *iter)
			{
				if (auto collection = e.Pin())
				{
					if (collection->CheckCollectionCompleteReset(id))
					{
						updateList.Emplace(collection->GetCollectionId());
						collection->SetCollectionCompleteReset();
					}
				}
			}
		}
	}

	if (isUpdate && updateList.Num() > 0)
	{
		FGsCostumeMsgParamCollectionList param(updateList, FGsCostumeMsgParamCollectionList::CollectionUpdateType::Condition);
		GMessage()->GetCostume().SendMessage(MessageCostume::COLLECTION_UPDATE, &param); //amount, level 변화도 갱신
	}
}

void UGsCostumeManager::UpdateCostumeSummonWaitingItemList(TArray<CostumeConfirmData> list, FDateTime nowTime, bool isUpdate)
{
	TArray<CostumeConfirmId> changed;

	for (auto& data : list)
	{
		auto id = data.mCostumeConfirmId;
		auto probabilityId = data.mCostumeProbabilityId;
		auto costumeData = GetCostumeData(data.mCostumeId).Pin();

		TArray<FGsCostumeSummonChangeData> summonableList;
		const FGsSchemaCostumeProbability* summonProbabilityData{ nullptr };
		const FGsSchemaCostumeSummonChangeCurrencyInfo* currencyData = nullptr;
		if (costumeData && data.mRemainChangeCount > 0)
		{
			if (const auto table = Cast<UGsTableCostumeProbability>(FGsSchemaCostumeProbability::GetStaticTable()))
			{
				if (table->FindRowById(probabilityId, summonProbabilityData))
				{
					TArray<CostumeId> excludeCostumeList = { data.mCostumeId };
					for (auto excludeId : data.mHistoryCostumeIdArray)
						if (excludeId != INVALID_COSTUME_ID) excludeCostumeList.Add(excludeId);

					Algo::TransformIf(summonProbabilityData->costumeList, summonableList, [&excludeCostumeList](auto& e) {
						return  e.costumeId.GetRow() && excludeCostumeList.Find(e.costumeId.GetRow()->id) == INDEX_NONE;  }, [](auto& e) {
							return FGsCostumeSummonChangeData{ GSCostume()->GetCostumeData(e.costumeId.GetRow()->id), e.probability };
						});

				}
			}
		}
		if (auto iter = _summonWaitings.FindByPredicate([id](auto e) { return e->_confirmId == id; }))
		{
			(*iter)->UpdateItemData(data, nowTime, costumeData, summonableList, summonProbabilityData);
			changed.Add(id);
		}
		else
		{
			_summonWaitings.Add(MakeShared<FGsCostumeSummonConfirmData>(data, nowTime, costumeData, summonableList, summonProbabilityData));
			if (isUpdate) changed.Add(id);
		}

	}

	for (auto summonId : changed)
	{
		FGsCostumeMsgParamCostumeSummonWait param(summonId);
		GMessage()->GetCostume().SendMessage(MessageCostume::SUMMONWAIT_UPDATE, &param);
	}
	GMessage()->GetHudRedDot().SendMessage(MessageContentHUDRedDot::COSTUME, IsCostumeSummonItemRedDot());
}

void UGsCostumeManager::ChangeCostumeSummonWaitingItem(CostumeConfirmData data, FDateTime nowTime)
{
	auto id = data.mCostumeConfirmId;
	auto probabilityId = data.mCostumeProbabilityId;
	auto costumeData = GetCostumeData(data.mCostumeId).Pin();

	TArray<FGsCostumeSummonChangeData> summonableList;
	const FGsSchemaCostumeProbability* summonProbabilityData{ nullptr };
	const FGsSchemaCostumeSummonChangeCurrencyInfo* currencyData = nullptr;
	if (costumeData && data.mRemainChangeCount > 0)
	{
		if (const auto table = Cast<UGsTableCostumeProbability>(FGsSchemaCostumeProbability::GetStaticTable()))
		{
			if (table->FindRowById(probabilityId, summonProbabilityData))
			{
				TArray<CostumeId> excludeCostumeList = { data.mCostumeId };
				for (auto excludeId : data.mHistoryCostumeIdArray)
					if (excludeId != INVALID_COSTUME_ID) excludeCostumeList.Add(excludeId);

				Algo::TransformIf(summonProbabilityData->costumeList, summonableList, [&excludeCostumeList](auto& e) {
					return  e.costumeId.GetRow() && excludeCostumeList.Find(e.costumeId.GetRow()->id) == INDEX_NONE;  }, [](auto& e) {
						return FGsCostumeSummonChangeData{ GSCostume()->GetCostumeData(e.costumeId.GetRow()->id), e.probability };
					});

			}
		}

	}

	if (auto iter = _summonWaitings.FindByPredicate([id](auto e) { return e->_confirmId == id; }))
	{
		(*iter)->UpdateItemData(data, nowTime, costumeData, summonableList, summonProbabilityData);
	}
	else
	{
		_summonWaitings.Add(MakeShared<FGsCostumeSummonConfirmData>(data, nowTime, costumeData, summonableList, summonProbabilityData));
	}

	// 소환연출(소환석 뽑는부분는 skip, 페어리 나오는 연출부터)
	TArray<CostumeId> resultList;
	resultList.Add(data.mCostumeId);

	if (resultList.Num() > 0)
	{
		FGsCostumeMsgParamObtainList param(CostumeObtainReason::NONE, resultList);
		GMessage()->GetCostume().SendMessage(MessageCostume::OBTAIN_RESULT, &param);

		if (!OnSummonStart(CostumeObtainReason::COMPOSE, resultList))
		{
			GMessage()->GetCostume().SendMessage(MessageCostume::SUMMON_COMPLETE, nullptr);
		}
	}

	FGsCostumeMsgParamCostumeSummonWait param(id, FGsCostumeMsgParamCostumeSummonWait::SummonUpdateType::Change, data.mCostumeId);
	GMessage()->GetCostume().SendMessage(MessageCostume::SUMMONWAIT_UPDATE, &param);
}

void UGsCostumeManager::RemoveCostumeSummonWaitingItem(CostumeConfirmId waitingId)
{
	_summonWaitings.RemoveAll([waitingId](auto e) {
		return e->_confirmId == waitingId;
		});

	FGsCostumeMsgParamCostumeSummonWait param(waitingId, FGsCostumeMsgParamCostumeSummonWait::SummonUpdateType::Confirm);
	GMessage()->GetCostume().SendMessage(MessageCostume::SUMMONWAIT_UPDATE, &param);
	GMessage()->GetHudRedDot().SendMessage(MessageContentHUDRedDot::COSTUME, IsCostumeSummonItemRedDot());
}

void UGsCostumeManager::UpdateObtainedCostumeList(CostumeObtainReason obtainReason, TArray<CostumeId>& resultList)
{
	FGsCostumeMsgParamObtainList param(obtainReason, resultList);
	GMessage()->GetCostume().SendMessage(MessageCostume::OBTAIN_RESULT, &param);

	if (!OnSummonStart(obtainReason, resultList))
	{
		GMessage()->GetCostume().SendMessage(MessageCostume::SUMMON_COMPLETE, nullptr);
	}
}

void UGsCostumeManager::OnAckCeilingResult(bool success)
{
	FGsMsgCostumeCeilingResult param(success);
	GMessage()->GetCostume().SendMessage(MessageCostume::CEILING_RESULT, &param);
}

void UGsCostumeManager::OnAckExchangeCostume(CostumeId id)
{
	TArray<CostumeId> resultList;
	resultList.Add(id);

	if (resultList.Num() > 0)
	{
		FGsCostumeMsgParamObtainList param(CostumeObtainReason::EXCHANGE, resultList);
		GMessage()->GetCostume().SendMessage(MessageCostume::OBTAIN_RESULT, &param);

		if (!OnSummonStart(CostumeObtainReason::EXCHANGE, resultList))
		{
			GMessage()->GetCostume().SendMessage(MessageCostume::SUMMON_COMPLETE, nullptr);
		}
	}

	//FGsCostumeMsgParamCostumeSummonWait param(id, FGsCostumeMsgParamCostumeSummonWait::SummonUpdateType::Change, id);
	//GMessage()->GetCostume().SendMessage(MessageCostume::SUMMONWAIT_UPDATE, &param);
}

bool UGsCostumeManager::OnSummonStart(CostumeObtainReason obtainReason, TArray<CostumeId>& resultList)
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
		if (contents->GetCurrentContentsMode() == FGsContentsMode::InGame::ContentsCostume &&
			obtainReason == CostumeObtainReason::USE_ITEM)
		{
			// 페어리 ui 닫기
			GUI()->CloseByKeyName("WindowCostume");
		}
		if (contents->GetCurrentContentsMode() == FGsContentsMode::InGame::ContentsRecompose &&
			obtainReason == CostumeObtainReason::COMPOSE)
		{
			GSSummon()->SetSummonOriginContent(EGsSummonOriginContentType::SUMMON_RE_COMPOSE);
			GSSummon()->SetNotUseRetry();
		}
		if (contents->GetCurrentContentsMode() == FGsContentsMode::InGame::ContentsExchangeCostume &&
			obtainReason == CostumeObtainReason::EXCHANGE)
		{
			GSSummon()->SetSummonOriginContent(EGsSummonOriginContentType::SUMMON_EXCHANGE);
			GSSummon()->SetNotUseRetry();
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

			Algo::ForEach(_summonList, [this](auto id) { if (auto iter = _costumeDictionaryList.Find(id)) iter->Pin()->removeNewSummonMark(); });

			_summonList.Reset();
			_summonList.Append(resultList);

			// ack 보다 연출 패킷이 먼저 왔을 때도 isSummonNew set 되어 있도록 - 소환은 연출 패킷이 먼저 오고, 합성이나 교환은 연출이 나중에 온다
			for (auto id : resultList)
			{
				if (auto iter = _costumeDictionaryList.Find(id))
				{
					if (iter->IsValid() && iter->Pin()->isCollectedCostume() == false)
						iter->Pin()->_isSummonNew = true;
				}
			}

			summonHandler->SetSummonCostumeInfo(resultList);
			summonHandler->StartSummon(EGsSummonType::SUMMON_COSTUME);

			GUI()->HideTicker();

			_isRunningSummon = true;

			ReserverStatNotify(NotiProc::CmdNoti);
			return true;
		}
		break;
	}
	return false;
}

void UGsCostumeManager::OnSummonComplete(const IGsMessageParam* InParam)
{
	_summonList.Reset();
	Algo::ForEach(_costumeDictionaryData, [](auto e) { e->removeNewSummonMark(); });

	OpenReservedReward();

	GUI()->ShowTicker();
	_isRunningSummon = false;
	ReserverStatNotify(NotiProc::None);

	GMessage()->GetCostume().SendMessage(MessageCostume::EXCHANGE_EVENT_COMPLETE, nullptr);
}

void UGsCostumeManager::OpenCollectionTicker(TArray<CollectionId> collections)
{
	if (collections.Num() <= 0) return;
	UGsUITrayTickerCollectionMulti* Ticker =
		Cast<UGsUITrayTickerCollectionMulti>(GUI()->OpenAndGetWidget(TEXT("TrayTickerCollectionMulti")).Get());

	for (auto collectionId : collections)
	{
		if (auto collection = GetCollectionData(collectionId).Pin())
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

	//	_reservedCollectionTickers.Empty();
}

void UGsCostumeManager::OpenReservedReward()
{
	if (_isNotifyRewarded)
	{
		TMap<uint32, Currency> currencyList;

		auto widget = GUI()->OpenAndGetWidget(TEXT("PopupReceiveAchievementRewardAll")).Get();
		if (auto popup = Cast<UGsUIPopupReceiveAchievementRewardsAll>(widget))
		{
			FText CostumeUI_Compose_FailureReward;
			FText::FindText(TEXT("CostumeText"), TEXT("CostumeUI_Compose_FailureReward"), CostumeUI_Compose_FailureReward);

			FText DefaultPopupButtonOK;
			FText::FindText(TEXT("UICommonText"), TEXT("DefaultPopupButtonOK"), DefaultPopupButtonOK);
			popup->SetDetail(CostumeUI_Compose_FailureReward, DefaultPopupButtonOK);
			popup->SetData(currencyList, _reserveComposeRewards);
		}
	}

	_isNotifyRewarded = false;
}

void UGsCostumeManager::StartEvent(EGsEventProgressType InStartType, EGsEventProgressType InPreType)
{
	if (InPreType == EGsEventProgressType::SAVE_BATTERY ||
		InPreType == EGsEventProgressType::SUMMON_ROOM)
	{
		return;
	}

	if (InStartType == EGsEventProgressType::EXCHANGE_COSTUME_ROOM)
	{
		GMessage()->GetContents().SendMessage(MessageContents::ACTIVE_WINDOW_EXCHANGE_CUSTUME);

		if (_eventParam)
		{
			GMessage()->GetCostume().SendMessage(MessageCostume::EXCHANGE_COSTUME, _eventParam.Get());
			_eventParam = nullptr;
		}
	}
}

void UGsCostumeManager::FinishEvent(EGsEventProgressType InFinishType, EGsEventProgressType InNextType)
{
	if (InNextType == EGsEventProgressType::SAVE_BATTERY ||
		InNextType == EGsEventProgressType::SUMMON_ROOM)
	{
		return;
	}

	if (InFinishType == EGsEventProgressType::EXCHANGE_COSTUME_ROOM)
	{
		GUI()->CloseByKeyName(TEXT("WindowExchangeCostume"));
		GMessage()->GetContents().SendMessage(MessageContents::ACTIVE_HUD);
	}
}

void UGsCostumeManager::OnCostumeExchangeStateSelected(const FGsItem* item)
{
	GSEventProgress()->OnReqStartEvent(EGsEventProgressType::EXCHANGE_COSTUME_ROOM, this);
	_eventParam = MakeShared<FGsCostumeMsgParamExchangeCostume>(item);
}

TSet<StatType> UGsCostumeManager::GetContentsStatAll() const
{
	if (0 >= _contentsStatAll.Num())
	{
		for (const auto& collection : _costumeCollectionData)
		{
			for (auto passivityId : collection->GetRewardPassivity())
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

	return _contentsStatAll;
}

bool UGsCostumeManager::FindCostumeExchangeTicketEffectData(ItemId itemId, const FGsSchemaItemEffectChangeCostume*& effectData)
{
	if (const auto itemData = GItem()->GetItemTableDataByTID(itemId))
	{
		if (itemData->categoryMain == ItemCategoryMain::COSTUME &&
			(itemData->categorySub == ItemCategorySub::CHANGE_COSTUME_RANDOM || itemData->categorySub == ItemCategorySub::CHANGE_COSTUME_SELECT))
		{
			if (const auto ingredientTable = Cast<UGsTableItemIngredientDetail>(FGsSchemaItemIngredientDetail::GetStaticTable()))
			{
				const FGsSchemaItemIngredientDetail* ingredientEffect = nullptr;
				if (ingredientTable->FindRowById(itemData->detailId, ingredientEffect))
				{
					if (ingredientEffect->effectType == ItemEffectType::CHANGE_COSTUME)
					{
						int32 effectId = ingredientEffect->effectIDList.IsValidIndex(0) ? ingredientEffect->effectIDList[0] : itemData->detailId;
						if (const auto table =
							Cast<UGsTableItemEffectChangeCostume>(FGsSchemaItemEffectChangeCostume::GetStaticTable()))
						{
							table->FindRowById(effectId, effectData);

							return GItem()->FindByTID(itemId, ItemType::MAX).Num() > 0;//보유 여부
						}
					}
				}
			}
		}
	}

	return false;
}

bool UGsCostumeManager::OnProcessError(PD::Result res)
{
	if (res == PD::Result::COSTUME_CONFRIM_ERROR_EXPIRED)
	{
		FGsCostumeMsgParamCostumeSummonWait param(INVALID_COSTUME_CONFIRM_ID, FGsCostumeMsgParamCostumeSummonWait::SummonUpdateType::Change, INVALID_COSTUME_ID);
		GMessage()->GetCostume().SendMessage(MessageCostume::SUMMONWAIT_UPDATE, &param);

		FGsUIHelper::TraySectionMessageTickerNetError(PD::Result::COSTUME_CONFRIM_ERROR_EXPIRED);
		return true;
	}

	return false;
}

bool UGsCostumeManager::IsEnableStackCostume(CostumeGrade grade /*= CostumeGrade::MAX*/)
{
	return _costumeItems.FindByPredicate([](auto costume) { return costume && costume->GetAmount() >= MAX_COSTUME_STACK_AMOUNT;  }) == nullptr;
}

int32 UGsCostumeManager::GetRemainStackCountCostume()
{
	if (_costumeItems.IsValidIndex(0))
	{
		TArray<TWeakPtr<FGsCostumeItem>> costumes;
		Algo::Copy(_costumeItems, costumes);

		costumes.StableSort([](TWeakPtr<FGsCostumeItem> le, TWeakPtr<FGsCostumeItem> re) {
			return le.Pin()->GetAmount() > re.Pin()->GetAmount();
			});

		return MAX_COSTUME_STACK_AMOUNT - costumes[0].Pin()->GetAmount();
	}
	return MAX_COSTUME_STACK_AMOUNT + 1;
}


bool UGsCostumeManager::CheckComposeStackableCostume(TMap<CostumeId, ItemAmount>& costumeList)
{
	for (auto costume : _costumeItems)
	{
		int32 removeCount = 0;
		if (auto iterAmount = costumeList.Find(costume->GetCostumeID()))
		{
			removeCount = *iterAmount;
		}

		if (FMath::Min(costume->GetAmount(), MAX_COSTUME_STACK_AMOUNT) - removeCount >= MAX_COSTUME_STACK_AMOUNT)
		{
			return false;
		}
	}

	return true;
}

void UGsCostumeManager::ReserverStatNotify(NotiProc proc)
{
	_statNotify = proc;
}

void UGsCostumeManager::CollecitonStatNotify()
{
	_statNotify = NotiProc::None;
	GSStatChangeNotify()->CheckNotify(StatUpdateReason::COSTUME_COLLECTION);
}

void UGsCostumeManager::OnInvalidCombatPower()
{
	if (_statNotify == NotiProc::StatNoti)
	{
		CollecitonStatNotify();
	}
}
