// Fill out your copyright notice in the Description page of Project Settings.


#include "GsRewardHelper.h"
#include "Item/GsItem.h"
#include "DataSchema/Reward/GsSchemaRewardData.h"
#include "DataSchema/Reward/GsSchemaRewardBoxData.h"
#include "Management/ScopeGlobal/GsGameDataManager.h"
#include "Management/ScopeGlobal/GsQuestManager.h"
#include "Item/GsItemManager.h"
#include "Currency/GsCurrencyHelper.h"
#include "UI/UIContent/Helper/GsUIHelper.h"
#include "Paper2D/Classes/PaperSprite.h"
#include "UTIL/GsTableUtil.h"
#include "UTIL/GsItemUtil.h"
#include "Reward/GsRewardIconDataWrapper.h"
#include "T1Project.h"
#include "Quest/GsQuestData.h"
#include "DataSchema/User/Level/GsSchemaLevelExpTable.h"

#define MaxVal(a, b) (((a) > (b)) ? (a) : (b))

//----------------------------------------------------------------------------------------------------------------------
void FGsRewardUnboxingData::Clear()
{
	_rewardType = RewardType::MAX;	

	_expPair.Key = 0;
	_expPair.Value = 0;

	_currencyType = CurrencyType::NONE;
	_currencyPair.Key = 0;
	_currencyPair.Value = 0;	

	_itemDataList.Empty();

	_bUseCurrencyIcon = false;
	
	_bIsAdded = false;
	_tempCurrencyMap.Empty();
}

void FGsRewardUnboxingData::SetData(const FGsRewardUnboxingData* InData)
{
	_rewardType = InData->_rewardType;

	_expPair.Key = InData->_expPair.Key;
	_expPair.Value = InData->_expPair.Value;

	_currencyType = InData->_currencyType;
	_currencyPair.Key = InData->_currencyPair.Key;
	_currencyPair.Value = InData->_currencyPair.Value;

	_itemDataList.Append(InData->_itemDataList);

	_bUseCurrencyIcon = InData->_bUseCurrencyIcon;
}

bool FGsRewardUnboxingData::IsExp() const
{
	if (0 < _expPair.Key &&
		0 < _expPair.Value)
	{
		return true;
	}

	return false;
}

Exp FGsRewardUnboxingData::GetCommonExp() const
{
	// Min, Max 중 Min값을 사용
	return _expPair.Key;
}

double FGsRewardUnboxingData::GetExpPercent() const
{
	double expRate = 0;
	if (FGsGameDataManager* dataMgr = GGameData())
	{
		if (const FGsNetUserData* userData = dataMgr->GetUserData())
		{
			expRate = static_cast<double>(FGsRewardHelper::GetExpRate(userData->mLevel, GetCommonExp(), false));
		}
	}

	return expRate * 100;
}

bool FGsRewardUnboxingData::IsCurrency() const
{
	if (CurrencyType::NONE == _currencyType ||
		CurrencyType::MAX_ACCOUNT_TYPE == _currencyType ||
		CurrencyType::MAX_USER_TYPE == _currencyType)
	{
		return false;
	}

	if (0 >= _currencyPair.Key &&
		0 >= _currencyPair.Value)
	{
		return false;
	}

	return true;
}

Currency FGsRewardUnboxingData::GetCommonCurrencyAmount() const
{
	// Min, Max 중 Min값을 사용
	return _currencyPair.Key;
}

void FGsRewardUnboxingData::GetCurrencyList(OUT TArray<TPair<CurrencyType, Currency>>& OutList) const
{
	if (_bIsAdded)
	{
		for (const TPair<CurrencyType, TPair<Currency, Currency>>& itPair : _tempCurrencyMap)
		{
			OutList.Emplace(itPair.Key, itPair.Value.Key);
		}
	}
	else
	{
		if (false == IsCurrency())
		{
			return;
		}

		OutList.Emplace(_currencyType, GetCommonCurrencyAmount());
	}
}

void FGsRewardUnboxingData::ItemSort()
{
	_itemDataList.Sort([](const FGsSchemaRewardItemBagData& source, const FGsSchemaRewardItemBagData& dest)->bool {
		const FGsSchemaItemCommon* sourceItem = UGsTableUtil::FindRowById<UGsTableItemCommon, FGsSchemaItemCommon>(source.itemId);
		const FGsSchemaItemCommon* destItem = UGsTableUtil::FindRowById<UGsTableItemCommon, FGsSchemaItemCommon>(dest.itemId);

		if (nullptr == sourceItem)
		{
			return false;
		}

		if (nullptr == destItem)
		{
			return false;
		}

		if (sourceItem->grade > destItem->grade)
		{
			return true;
		}
		else if (sourceItem->grade == destItem->grade)
		{
			return false;
		}

		return false;});
}

void FGsRewardUnboxingData::ItemSortCurrencyItemPriority()
{
	_itemDataList.Sort([](const FGsSchemaRewardItemBagData& source, const FGsSchemaRewardItemBagData& dest)->bool {
		const FGsSchemaItemCommon* sourceItem = UGsTableUtil::FindRowById<UGsTableItemCommon, FGsSchemaItemCommon>(source.itemId);
		const FGsSchemaItemCommon* destItem = UGsTableUtil::FindRowById<UGsTableItemCommon, FGsSchemaItemCommon>(dest.itemId);

		if (nullptr == sourceItem)
		{
			return false;
		}

		if (nullptr == destItem)
		{
			return false;
		}

		// 재화형 아이템 우선
		if (UGsItemUtil::GetItemSortOrder(sourceItem->type) > UGsItemUtil::GetItemSortOrder(destItem->type))
		{
			return true;
		}
		else if (UGsItemUtil::GetItemSortOrder(sourceItem->type) == UGsItemUtil::GetItemSortOrder(destItem->type))
		{
			if (sourceItem->grade > destItem->grade)
			{
				return true;
			}
			else if (sourceItem->grade == destItem->grade)
			{
				if (source.itemId > dest.itemId)
				{
					return true;
				}
			}
		}

		return false;
		});
}

void FGsRewardUnboxingData::AddData(const FGsRewardUnboxingData& InData)
{
	if (false == _bIsAdded)
	{
		if (IsCurrency())
		{
			_tempCurrencyMap.Emplace(_currencyType, _currencyPair);
		}

		_bIsAdded = true;
	}

	_expPair.Key += InData._expPair.Key;
	_expPair.Value += InData._expPair.Value;

	// 안쓰므로 할 필요가 없을 것
	//if (_currencyType == InData._currencyType)
	//{
	//	_currencyPair.Key += InData._currencyPair.Key;
	//	_currencyPair.Value += InData._currencyPair.Value;
	//}

	if (InData.IsCurrency())
	{
		if (TPair<Currency, Currency>* pair = _tempCurrencyMap.Find(InData._currencyType))
		{
			pair->Key += InData._currencyPair.Key;
			pair->Value += InData._currencyPair.Value;
		}
		else
		{
			_tempCurrencyMap.Emplace(InData._currencyType, InData._currencyPair);
		}
	}

	_itemDataList.Append(InData._itemDataList);
}

//----------------------------------------------------------------------------------------------------------------------
bool FGsRewardHelper::UnboxingRewardData(RewardId InRewardTId, OUT FGsRewardUnboxingData& OutData)
{
	const FGsSchemaRewardData* row =
		UGsTableUtil::FindRowById<UGsTableRewardData, FGsSchemaRewardData>(InRewardTId);
	if (nullptr == row)
	{
		return false;
	}

	return UnboxingRewardData(row, OutData);
}

bool FGsRewardHelper::UnboxingRewardData(const FGsSchemaRewardData* InRow, OUT FGsRewardUnboxingData& OutData)
{
	OutData._rewardType = InRow->rewardType;

	OutData._expPair.Key = InRow->expMin;
	OutData._expPair.Value = InRow->expMax;

	OutData._currencyType = InRow->currencyType;
	OutData._currencyPair.Key = InRow->currencyAmountMin;
	OutData._currencyPair.Value = InRow->currencyAmountMax;

	// 주의: PICK_REWARD_BOX_LIST 는 서버에서 받은 정보로 채우는 타입이므로 채우지 않음
	if (RewardType::PICK_REWARD_BOX_LIST != InRow->rewardType)
	{
		for (const FGsSchemaRewardBox& box : InRow->rewardBoxList)
		{
			if (const FGsSchemaRewardBoxData* boxData = box.rewardBoxDataRow.GetRow())
			{
				UnboxingRewardBox(boxData->id, OutData._itemDataList);
			}
		}
	}

	return true;
}

bool FGsRewardHelper::UnboxingRewardData(TArray<RewardId>& InRewardTId, OUT FGsRewardUnboxingData& OutData)
{
	FGsRewardUnboxingData totalData;
	for (RewardId id : InRewardTId)
	{
		FGsRewardUnboxingData data;
		UnboxingRewardData(InRewardTId, data);
		totalData.AddData(data);
	}

	return true;
}

bool FGsRewardHelper::UnboxingRewardBox(RewardBoxId InRewardBoxTId, TArray<const FGsSchemaRewardItemBagData*>& OutList)
{
	const FGsSchemaRewardBoxData* row =
		UGsTableUtil::FindRowById<UGsTableRewardBoxData, FGsSchemaRewardBoxData>(InRewardBoxTId);
	if (nullptr == row)
	{
		return false;
	}

	for (const FGsSchemaRewardItemBagData& bagData : row->itemBagList)
	{
		// 빈 데이터는 넣지 않는다
		if (CurrencyType::NONE == bagData.currencyType &&
			0 == bagData.itemId)
		{
			continue;
		}

		OutList.Add(&bagData);
	}

	return true;
}

RewardType FGsRewardHelper::GetRewardType(RewardId InRewardId)
{
	if (const FGsSchemaRewardData* row = UGsTableUtil::FindRowById<UGsTableRewardData, FGsSchemaRewardData>(InRewardId))
	{
		return row->rewardType;
	}

	return RewardType::MAX;
}

bool FGsRewardHelper::GetRewardBoxItemIdPairList(QuestId InQuestId, QuestIndex InQuestIndex, OUT TArray<RewardBoxItemIdPair>& OutList)
{
	TSharedPtr<FGsQuestData> questData = GSQuest()->GetQuestData(InQuestId);
	if (questData.IsValid())
	{		
		if (FGsQuestRepeatData* repeatData = static_cast<FGsQuestRepeatData*>(questData.Get()))
		{
			repeatData->GetDynamicRewardBoxItemIdPairList(InQuestIndex, OutList);
		}

		if (0 < OutList.Num())
		{
			return true;
		}
	}

	return false;
}

bool FGsRewardHelper::UnboxingRewardData(RewardId InRewardId, const TArray<RewardBoxItemIdPair>& InList,
	OUT FGsRewardUnboxingData& OutData)
{
	const FGsSchemaRewardData* row =
		UGsTableUtil::FindRowById<UGsTableRewardData, FGsSchemaRewardData>(InRewardId);
	if (nullptr == row)
	{
		return false;
	}

	// 주의: PICK_REWARD_BOX_LIST 는 서버에서 받은 정보로 채우는 타입이므로, _itemDataList를 채우지 않음
	UnboxingRewardData(row, OutData);

	// 선택된 _itemDataList 로 채운다
	for (int i = 0; i < InList.Num(); ++i)
	{
		if (const FGsSchemaRewardItemBagData* bagData = GetItemBagDataByPairId(InList[i]))
		{
			OutData._itemDataList.Emplace(bagData);
		}
		else
		{
			return false;
		}
	}

	return true;
}

const FGsSchemaRewardItemBagData* FGsRewardHelper::GetItemBagDataByPairId(const RewardBoxItemIdPair& InPair)
{
	const FGsSchemaRewardBoxData* row =
		UGsTableUtil::FindRowById<UGsTableRewardBoxData, FGsSchemaRewardBoxData>(InPair.mRewardBoxId);
	if (row)
	{
		if (row->itemBagList.IsValidIndex(InPair.mItemBagIndex))
		{
			return &row->itemBagList[InPair.mItemBagIndex];
		}
	}

#if WITH_EDITOR
	GSLOG(Error, TEXT("[Reward] Fail to get ItemBagData. rewardBoxId: %d, itemBagIndex: %d"), 
		InPair.mRewardBoxId, InPair.mItemBagIndex);
#endif

	return nullptr;
}

Exp FGsRewardHelper::GetLevelMaxExp(Level InLevel)
{
	const FGsSchemaLevelExpTable* row = 
		UGsTableUtil::FindRowById<UGsTableLevelExpTable, FGsSchemaLevelExpTable>(InLevel);
	if (row)
	{
		return row->Exp;
	}

	return 0;
}

Exp FGsRewardHelper::GetLevelAccumMaxExp(Level InLevel)
{
	if (0 == InLevel)
	{
		return 0;
	}

	TArray<const FGsSchemaLevelExpTable*> levelExpList;
	if (const UGsTable* table = FGsSchemaLevelExpTable::GetStaticTable())
	{
		if (const UGsTableLevelExpTable* levelExpTable = Cast<UGsTableLevelExpTable>(table))
		{
			levelExpTable->GetAllRows(levelExpList);
		}
	}
	
	if (0 == levelExpList.Num())
	{
		return 0;
	}

	Exp outAccumExp = 0;
	for (const FGsSchemaLevelExpTable* iter : levelExpList)
	{
		if (iter->level > InLevel)
		{
			break;
		}
		else
		{
			outAccumExp += iter->Exp;
		}
	}

	return outAccumExp;
}

float FGsRewardHelper::GetExpRate(Level InLevel, Exp InExp, bool bIsAccumExp)
{
	if (0 == InLevel ||
		0 == InExp)
	{
		return 0.f;
	}

	if (bIsAccumExp)
	{
		// 2022/11/07 PKT - 누적 경험치일 경우 구간 별 경험치로 변경 
		InExp -= GetLevelAccumMaxExp(InLevel - 1);
		InExp = (0 < InExp) ? InExp : 0;
	}
	
	float outRate = 0.f;
	Exp currLevelMaxExp = GetLevelMaxExp(InLevel);
	if (0 < currLevelMaxExp)
	{
		double tempExp = static_cast<double>(InExp) / static_cast<double>(currLevelMaxExp);
		outRate = static_cast<float>(tempExp);
	}

	return outRate;
}

Level FGsRewardHelper::GetLevelByExp(Exp InAccumExp)
{
	TArray<const FGsSchemaLevelExpTable*> levelExpList;
	if (const UGsTable* table = FGsSchemaLevelExpTable::GetStaticTable())
	{
		if (const UGsTableLevelExpTable* levelExpTable = Cast<UGsTableLevelExpTable>(table))
		{
			levelExpTable->GetAllRows(levelExpList);
		}
	}

	if (0 == levelExpList.Num())
	{
		return 0;
	}

	Exp prevAccumExp = 0;
	Exp currAccumExp = 0;
	for (const FGsSchemaLevelExpTable* iter : levelExpList)
	{
		currAccumExp += iter->Exp;

		if (prevAccumExp <= InAccumExp &&
			currAccumExp > InAccumExp)
		{
			return iter->level;
		}

		prevAccumExp = currAccumExp;
	}

	// 위에서 리턴이 안된 경우, 최대 레벨 값을 초과한 것이므로 최대 레벨로 리턴 함
	return MAX_LEVEL;
}

bool FGsRewardHelper::IsSubRewardType(RewardType InType)
{
	switch(InType)
	{
	case RewardType::SUB_PICK_ONE:
	case RewardType::SUB_RANDOM_ONE:
		return true;
	}

	return false;
}

void FGsRewardHelper::GetIconDataList(const FGsRewardUnboxingData* InData, OUT TArray<FGsRewardIconDataWrapper>& OutList)
{
	if (nullptr == InData)
	{
		return;
	}
	
	// PICK_REWARD_BOX_LIST는 itemBag 순서가 매우 중요하고, RewardBox의 재화를 아이콘 리스트에 표시하지 않음
	if (InData->_bUseCurrencyIcon &&
		RewardType::PICK_REWARD_BOX_LIST != InData->_rewardType)
	{
		// EXP 정보
		if (0 < InData->GetCommonExp())
		{
			OutList.Emplace(FGsRewardIconDataWrapper(InData->GetExpPercent()));
		}

		if (InData->_bIsAdded)
		{
			for (const TPair<CurrencyType, TPair<Currency, Currency>>& itPair : InData->_tempCurrencyMap)
			{
				if (CurrencyType::NONE != itPair.Key &&
					0 < itPair.Value.Key)
				{
					OutList.Emplace(FGsRewardIconDataWrapper(itPair.Key, itPair.Value.Key, itPair.Value.Value));
				}
			}
		}
		else
		{
			// 재화를 아이콘으로 찍을 경우, 재화부터 찍는다
			if (CurrencyType::NONE != InData->_currencyType &&
				0 < InData->_currencyPair.Key)
			{
				OutList.Emplace(FGsRewardIconDataWrapper(InData->_currencyType, InData->_currencyPair.Key, InData->_currencyPair.Value));
			}
		}
	}

	for (int i = 0; i < InData->_itemDataList.Num(); ++i)
	{
		OutList.Emplace(FGsRewardIconDataWrapper(InData->_itemDataList[i]));
	}
}


bool FGsRewardHelper::CheckInventoryState(RewardId InMainRewardTId, RewardId InSubRewardTId, bool bUseMessage)
{
	/*
	1. 인벤토리 여유공간 체크
	 - RewardType이 ALL이 아닐 경우 1칸 있는지 체크
	 - 스택형 아이템이고 이미 보유중인 슬롯이 있을 경우, min~max의 max값 기준 체크하여 합쳐질 수 있는지 확인
	 - 스택형이 아니거나 보유중이 아닐 경우 1칸씩으로 체크
	 - 다만 MainReward / SubReward 두 가지 경우가 있어 조합을 잘 체크해야함

	2. 무게체크
	 - 랜덤일 경우 가장 무거운 것을 기준으로 함
	*/

	// SubReward 여부부터 체크한다 :

	// 1. SubReward 가 있을 경우, MainReward 의 RewardType 은 무조건 ALL 이며, 동시에 체크해야 함
	if (INVALID_REWARD_ID != InSubRewardTId)
	{
		return CheckMainSubRewardState(InMainRewardTId, InSubRewardTId, bUseMessage);
	}
	// 2. SubReward 가 없을 경우, MainReward 만 체크하면 됨
	else
	{
		return CheckMainRewardState(InMainRewardTId, bUseMessage);
	}
}

bool FGsRewardHelper::CheckMainSubRewardState(RewardId InMainRewardTId, RewardId InSubRewardTId, bool bUseMessage)
{
	if (INVALID_REWARD_ID == InMainRewardTId)
	{
#if WITH_EDITOR
		FGsUIHelper::PopupSystemMsg(FText::FromString(TEXT("[RewardHelper] MainRewardTId is invalid!")));
#endif
		GSLOG(Error, TEXT("[RewardHelper] MainRewardTId is invalid!"));
		return false;
	}

	// MainReward 가져오기
	const FGsSchemaRewardData* mainRewardData = nullptr;
	if (false == GetRewardDataFromId(InMainRewardTId, mainRewardData))
	{
		return false;
	}

	// MainReward 의 RewardType 이 ALL 이 아니라면 리턴
	if (RewardType::ALL != mainRewardData->rewardType)
	{
#if WITH_EDITOR
		FGsUIHelper::PopupSystemMsg(FText::FromString(TEXT("[RewardHelper] MainReward's RewardType should be ALL when SubReward exists!")));
#endif
		GSLOG(Error, TEXT("[RewardHelper] MainReward's RewardType should be ALL when SubReward exists!"));
		return false;
	}

	// 받을 수 있는 아이템들 리스트와 갯수
	TArray<TPair<int32, ItemAmount>> itemTIdAmountPairList;

	// 메인 리워드의 아이템들 리스트 저장
	{
		TArray<const FGsSchemaRewardItemBagData*> mainItemBagDataList;

		// 리워드 박스의 갯수 체크는 GetRewardDataFromId 에서 이미 완료
		// 리워드 박스의 갯수는 무조건 1개로 설정되어 있음
		UnboxingRewardBox(mainRewardData->rewardBoxList[0].rewardBoxId, mainItemBagDataList);

		for (const FGsSchemaRewardItemBagData* itemBag : mainItemBagDataList)
		{
			if (itemBag->itemId == INVALID_ITEM_ID)
				continue;

			itemTIdAmountPairList.Emplace(itemBag->itemId, static_cast<ItemAmount>(itemBag->itemCountMax));
		}
	}

	// ItemManager 이용하여 아이템 추가 가능 여부 확인할 것임 (IsCanItemListAdd 함수 추가)
	UGsItemManager* itemMgr = GItem();
	if (nullptr == itemMgr)
	{
#if WITH_EDITOR
		FGsUIHelper::PopupSystemMsg(FText::FromString(TEXT("[RewardHelper] nullptr == ItemManager")));
#endif
		GSLOG(Error, TEXT("[RewardHelper] nullptr == ItemManager"));
		return false;
	}

	// 인벤토리 슬롯 체크한다 (메인 리워드부터)
	if (itemMgr->IsCanItemListAdd(itemTIdAmountPairList) == false)
	{
		InventorySlotErrorTickerQuest(bUseMessage);
		GSLOG(Warning, TEXT("[RewardHelper] MainReward - Inventory Slot Full!"));
		return false;
	}

	// 무게 체크한다 (메인 리워드부터)
	if (itemMgr->IsInvenWeightFull(itemTIdAmountPairList))
	{
		InventoryWeightErrorTickerQuest(bUseMessage);
		GSLOG(Warning, TEXT("[RewardHelper] MainReward - Weight Full!"));
		return false;
	}

	// SubReward 체크 (이제 MainReward + SubReward 를 받을 수 있는지 체크할 것이다)

	// RewardId 로부터 유효한 RewardData 를 가져올 수 있는지 체크 (RewardType 분류를 위해 필요)
	const FGsSchemaRewardData* subRewardData = nullptr;
	if (false == GetRewardDataFromId(InSubRewardTId, subRewardData))
	{
		return false;
	}

	// SubReward 타입에 따라 분류 인벤토리 여유공간 및 무게 체크 
	switch (subRewardData->rewardType)
	{
		case RewardType::ALL:
		{
			// 메인 보상의 RewardType 이 All 일 경우 쓰일 것 (받을 수 있는 아이템들 리스트와 갯수)
			TArray<const FGsSchemaRewardItemBagData*> subItemBagDataList;

			// 리워드 박스의 갯수 체크는 GetRewardDataFromId 에서 이미 완료
			// 리워드 박스의 갯수는 무조건 1개로 설정되어 있음
			UnboxingRewardBox(subRewardData->rewardBoxList[0].rewardBoxId, subItemBagDataList);

			for (const FGsSchemaRewardItemBagData* itemBag : subItemBagDataList)
			{
				if (itemBag->itemId == INVALID_ITEM_ID)
					continue;

				itemTIdAmountPairList.Emplace((TPair<int32, ItemAmount>(itemBag->itemId, static_cast<ItemAmount>(itemBag->itemCountMax))));
			}

			// 인벤토리 체크
			if (itemMgr->IsCanItemListAdd(itemTIdAmountPairList) == false)
			{
				InventorySlotErrorTickerQuest(bUseMessage);
				GSLOG(Warning, TEXT("[RewardHelper] MainReward + SubReward - Inventory Slot Full!"));
				return false;
			}

			// 무게 체크
			if (itemMgr->IsInvenWeightFull(itemTIdAmountPairList))
			{
				InventoryWeightErrorTickerQuest(bUseMessage);
				GSLOG(Warning, TEXT("[RewardHelper] MainReward + SubReward - Weight Full!"));
				return false;
			}

			break;
		}
		// 인벤토리 : 1칸 비어있는지만 체크, 무게는 SubReward 항목 중 가장 무게가 많이 나가는 것으로 체크
		case RewardType::RANDOM_ONE:
		case RewardType::PICK_ONE:
		case RewardType::PICK_REWARD_BOX_LIST:
		{
			// 메인 보상의 RewardType 이 All 일 경우 쓰일 것 (받을 수 있는 아이템들 리스트와 갯수)
			TArray<const FGsSchemaRewardItemBagData*> subItemBagDataList;

			// 리워드 박스의 갯수 체크는 GetRewardDataFromId 에서 이미 완료
			// 리워드 박스의 갯수는 무조건 1개로 설정되어 있음
			UnboxingRewardBox(subRewardData->rewardBoxList[0].rewardBoxId, subItemBagDataList);

			int32 addWeight = GetWeightSumFromRewardBox(subRewardData->rewardBoxList[0]);

			// SubReward 중 최고 무게
			int32 subWeightMax = 0;

			// 인벤토리 체크 (ItemBagList 돌면서 한개 넣었다 뺐다를 반복)
			for (const FGsSchemaRewardItemBagData* itemBag : subItemBagDataList)
			{
				// 아이템 없이 재화만 들어가 있을 수도 있다
				if (itemBag->itemId == INVALID_ITEM_ID)
					continue;

				// 하나 넣어보고
				itemTIdAmountPairList.Emplace((TPair<int32, ItemAmount>(itemBag->itemId, static_cast<ItemAmount>(itemBag->itemCountMax))));

				// 인벤토리 슬롯 체크한다
				if (itemMgr->IsCanItemListAdd(itemTIdAmountPairList) == false)
				{
					InventorySlotErrorTickerQuest(bUseMessage);
					GSLOG(Warning, TEXT("[RewardHelper] MainReward + SubReward - Inventory Slot Full!"));
					return false;
				}

				// SubReward 중 최고 무게 갱신
				const FGsSchemaItemCommon* itemData = FGsItem::GetTableDataByID(itemBag->itemId);
				if (nullptr == itemData)
				{
#if WITH_EDITOR
					FGsUIHelper::PopupSystemMsg(FText::FromString(TEXT("[RewardHelper] ItemData is invalid!")));
#endif
					GSLOG(Error, TEXT("[RewardHelper] ItemData is invalid!"));
					return false;
				}

				subWeightMax = MaxVal(subWeightMax, itemData->weight * itemBag->itemCountMax);

				// 다음 녀석을 테스트하기 위해 빼준다
				itemTIdAmountPairList.Pop();
			}

			// 만약 타입이 RewardType::PICK_REWARD_BOX_LIST 일 경우, 가능 아이템 풀을 전부 뒤져야 한다. 
			// 각 풀에서의 최댓값을 누적해서 구함(최적화 이슈 있을 수 있음);
			if (RewardType::PICK_REWARD_BOX_LIST == subRewardData->rewardType)
			{
				subWeightMax = MaxVal(subWeightMax, GetMaxWeightFromRewardData(subRewardData));
			}

			// 최종 무게 체크
			if (itemMgr->IsInvenWeightFull(addWeight + subWeightMax))
			{
				InventoryWeightErrorTickerQuest(bUseMessage);
				GSLOG(Warning, TEXT("[RewardHelper] MainReward + SubReward - Weight Full!"));
				return false;
			}

			break;
		}
		// RANDOM_GROUP 일 경우, 데이터가 잘못 입력되었다는 뜻 (필드 전용이므로)
		case RewardType::RANDOM_GROUP:
		{
#if WITH_EDITOR
			FGsUIHelper::PopupSystemMsg(FText::FromString(TEXT("[RewardHelper] SubReward's RewardType cannot be RANDOM_GROUP!")));
#endif
			GSLOG(Error, TEXT("[RewardHelper] SubReward's RewardType cannot be RANDOM_GROUP!"));
			return false;
		}
	}

	return true;
}

bool FGsRewardHelper::CheckMainRewardState(RewardId InMainRewardTId, bool bUseMessage)
{
	// MainReward, SubReward 둘 다 없는 경우는 데이터 이슈로 판단한다
	if (INVALID_REWARD_ID == InMainRewardTId)
	{
#if WITH_EDITOR
		FGsUIHelper::PopupSystemMsg(FText::FromString(TEXT("[RewardHelper] MainRewardTId is invalid!")));
#endif
		GSLOG(Error, TEXT("[RewardHelper] MainRewardTId is invalid!"));
		return false;
	}

	// MainReward 를 체크한다 :

	// RewardId 로부터 유효한 RewardData 를 가져올 수 있는지 체크 (RewardType 분류를 위해 필요)
	const FGsSchemaRewardData* mainRewardData = nullptr;
	if (false == GetRewardDataFromId(InMainRewardTId, mainRewardData))
	{
		return false;
	}

	// ItemManager 이용하여 아이템 추가 가능 여부 확인
	UGsItemManager* itemMgr = GItem();
	if (nullptr == itemMgr)
	{
#if WITH_EDITOR
		FGsUIHelper::PopupSystemMsg(FText::FromString(TEXT("[RewardHelper] nullptr == ItemManager")));
#endif
		GSLOG(Error, TEXT("[RewardHelper] nullptr == ItemManager"));
		return false;
	}

	// 메인 보상의 RewardType 이 All 일 경우 쓰일 것 (받을 수 있는 아이템들 리스트와 갯수)
	TArray<TPair<int32, ItemAmount>> itemTIdAmountPairList;
	TArray<const FGsSchemaRewardItemBagData*> mainItemBagDataList;

	// 리워드 박스의 갯수 체크는 GetRewardDataFromId 에서 이미 완료
	// 리워드 박스의 갯수는 무조건 1개로 설정되어 있음
	UnboxingRewardBox(mainRewardData->rewardBoxList[0].rewardBoxId, mainItemBagDataList);

	for (const FGsSchemaRewardItemBagData* itemBag : mainItemBagDataList)
	{
		if (itemBag->itemId == INVALID_ITEM_ID)
			continue;

		itemTIdAmountPairList.Emplace((TPair<int32, ItemAmount>(itemBag->itemId, static_cast<ItemAmount>(itemBag->itemCountMax))));
	}
	
	// itemBagList 에 아이템이 없을 경우 (즉, 재화 타입만 보상으로 주어질 경우) 보상은 언제나 수령 가능하다
	if (itemTIdAmountPairList.Num() == 0)
	{
		return true;
	}

	// 인벤토리 여유공간 체크
	switch (mainRewardData->rewardType)
	{
		case RewardType::ALL:
		{
			if (itemMgr->IsCanItemListAdd(itemTIdAmountPairList) == false)
			{
				InventorySlotErrorTickerQuest(bUseMessage);
				GSLOG(Warning, TEXT("[RewardHelper] MainReward - Inventory Slot Full!"));
				return false;
			}

			// 무게 체크
			if (itemMgr->IsInvenWeightFull(itemTIdAmountPairList))
			{
				InventoryWeightErrorTickerQuest(bUseMessage);
				GSLOG(Warning, TEXT("[RewardHelper] MainReward + SubReward - Weight Full!"));
				return false;
			}

			break;
		}
		case RewardType::RANDOM_ONE:
		case RewardType::PICK_ONE:
		case RewardType::PICK_REWARD_BOX_LIST:
		{
			// 인벤토리가 1칸 비어있는지만 체크한다
			if (itemMgr->IsInvenSlotFull())
			{
				InventorySlotErrorTickerQuest(bUseMessage);
				GSLOG(Warning, TEXT("[RewardHelper] MainReward - Inventory Slot Full!"));
				return false;
			}

			// 무게 체크
			int32 addWeight = GetMaxWeightFromRewardData(mainRewardData);

			if (itemMgr->IsInvenWeightFull(addWeight))
			{
				InventoryWeightErrorTickerQuest(bUseMessage);
				GSLOG(Warning, TEXT("[RewardHelper] MainReward - Inventory Slot Full!"));
				return false;
			}

			break;
		}
		// RANDOM_GROUP 일 경우, 데이터가 잘못 입력되었다는 뜻 (필드 전용이므로)
		case RewardType::RANDOM_GROUP:
		{
#if WITH_EDITOR
			FGsUIHelper::PopupSystemMsg(FText::FromString(TEXT("[RewardHelper] Main RewardType cannot be RANDOM_GROUP!")));
#endif
			GSLOG(Error, TEXT("[RewardHelper] Main RewardType cannot be RANDOM_GROUP!"));
			return false;
		}
	}

	return true;
}

bool FGsRewardHelper::GetRewardDataFromId(const RewardId InRewardTId, OUT const FGsSchemaRewardData*& OutRewardData)
{
	// RewardId 가 InValid 한 값일 경우 리턴
	if (INVALID_REWARD_ID == InRewardTId)
	{
		GSLOG(Warning, TEXT("[RewardHelper] RewardId is invalid!"));
		return false;
	}

	// RewardTable 이 없을 경우 리턴
	const UGsTableRewardData* rewardTableData = Cast<UGsTableRewardData>(FGsSchemaRewardData::GetStaticTable());
	if (nullptr == rewardTableData)
	{
		GSLOG(Warning, TEXT("[RewardHelper] RewardTable is not set!"));
		return false;
	}
	
	// RewardData 가 없을 경우 리턴
	if (rewardTableData->FindRowById(InRewardTId, OutRewardData) == false)
	{
		GSLOG(Warning, TEXT("[RewardHelper] RewardData is not set!"));
		return false;
	}

	//RewardBox 가 없을 경우 해당 경우는 데이터 오류 (RewardBox 내에 아이템이 없을 수는 있음)
	if (OutRewardData->rewardBoxList.Num() <= 0)
	{
		GSLOG(Warning, TEXT("[RewardHelper] RewardBox is not set(RewardBox data missing)!"));
		return false;
	}

	return true;
}

int32 FGsRewardHelper::GetMaxWeightFromRewardData(const FGsSchemaRewardData* InRewardData)
{
	int32 weightMaxForEachBox = 0;

	for (const FGsSchemaRewardBox& rewardBox : InRewardData->rewardBoxList)
	{
		TArray<const FGsSchemaRewardItemBagData*> itemBagList;

		UnboxingRewardBox(rewardBox.rewardBoxId, itemBagList);

		for (const FGsSchemaRewardItemBagData* itemBag : itemBagList)
		{
			const FGsSchemaItemCommon* itemData = FGsItem::GetTableDataByID(itemBag->itemId);
			if (nullptr == itemData)
			{
#if WITH_EDITOR
				FGsUIHelper::PopupSystemMsg(FText::FromString(TEXT("[RewardHelper] ItemData is invalid!")));
#endif
				GSLOG(Error, TEXT("[RewardHelper] ItemData is invalid! : %d"), itemBag->itemId);
				return false;
			}

			weightMaxForEachBox = MaxVal(weightMaxForEachBox, itemData->weight * itemBag->itemCountMax);
		}
	}

	return weightMaxForEachBox;
}

int32 FGsRewardHelper::GetWeightSumFromRewardBox(const FGsSchemaRewardBox& InRewardBox)
{
	int32 weightSum = 0;

	TArray<const FGsSchemaRewardItemBagData*> itemBagList;

	UnboxingRewardBox(InRewardBox.rewardBoxId, itemBagList);

	for (const FGsSchemaRewardItemBagData* itemBag : itemBagList)
	{
		const FGsSchemaItemCommon* itemData = FGsItem::GetTableDataByID(itemBag->itemId);
		if (nullptr == itemData)
		{
#if WITH_EDITOR
			FGsUIHelper::PopupSystemMsg(FText::FromString(TEXT("[RewardHelper] ItemData is invalid!")));
#endif
			GSLOG(Error, TEXT("[RewardHelper] ItemData is invalid! : %d"), itemBag->itemId);
			return false;
		}

		weightSum += itemData->weight * itemBag->itemCountMax;
	}

	return weightSum;
}

bool FGsRewardHelper::CheckRewardState(RewardId InRewardTId, bool bUseMessage /*= true*/)
{
	// InRewardTId 가 0이면, true 로 판단한다 (보상이 없으니까, 수령 가능하다고 판단)
	if (INVALID_REWARD_ID == InRewardTId)
	{
		GSLOG(Log, TEXT("[RewardHelper] InRewardTId(%d) is invalid!"), InRewardTId);
		return true;
	}

	// RewardId 로부터 유효한 RewardData 를 가져올 수 있는지 체크 (RewardType 분류를 위해 필요)
	const FGsSchemaRewardData* mainRewardData = nullptr;
	if (false == GetRewardDataFromId(InRewardTId, mainRewardData))
	{
		return false;
	}

	// ItemManager 이용하여 아이템 추가 가능 여부 확인
	UGsItemManager* itemMgr = GItem();
	if (nullptr == itemMgr)
	{
#if WITH_EDITOR
		FGsUIHelper::PopupSystemMsg(FText::FromString(TEXT("[RewardHelper] nullptr == ItemManager")));
#endif
		GSLOG(Error, TEXT("[RewardHelper] nullptr == ItemManager"));
		return false;
	}

	// 메인 보상의 RewardType 이 All 일 경우 쓰일 것 (받을 수 있는 아이템들 리스트와 갯수)
	TArray<TPair<int32, ItemAmount>> itemTIdAmountPairList;
	TArray<const FGsSchemaRewardItemBagData*> mainItemBagDataList;

	// 리워드 박스의 갯수 체크는 GetRewardDataFromId 에서 이미 완료
	// 리워드 박스의 갯수는 무조건 1개로 설정되어 있음
	UnboxingRewardBox(mainRewardData->rewardBoxList[0].rewardBoxId, mainItemBagDataList);

	for (const FGsSchemaRewardItemBagData* itemBag : mainItemBagDataList)
	{
		if (itemBag->itemId == INVALID_ITEM_ID)
			continue;

		itemTIdAmountPairList.Emplace((TPair<int32, ItemAmount>(itemBag->itemId, static_cast<ItemAmount>(itemBag->itemCountMax))));
	}

	// itemBagList 에 아이템이 없을 경우 (즉, 재화 타입만 보상으로 주어질 경우) 보상은 언제나 수령 가능하다
	if (itemTIdAmountPairList.Num() == 0)
	{
		return true;
	}

	// 인벤토리 여유공간 체크
	switch (mainRewardData->rewardType)
	{
	case RewardType::ALL:
	{
		if (itemMgr->IsCanItemListAdd(itemTIdAmountPairList) == false)
		{
			InventorySlotErrorTickerNet(bUseMessage);
			GSLOG(Warning, TEXT("[RewardHelper] MainReward - Inventory Slot Full!"));
			return false;
		}

		// 무게 체크
		if (itemMgr->IsInvenWeightFull(itemTIdAmountPairList))
		{
			InventorySlotErrorTickerNet(bUseMessage);
			GSLOG(Warning, TEXT("[RewardHelper] MainReward + SubReward - Weight Full!"));
			return false;
		}

		break;
	}
	case RewardType::RANDOM_ONE:
	case RewardType::PICK_ONE:
	case RewardType::PICK_REWARD_BOX_LIST:
	{
		// 인벤토리가 1칸 비어있는지만 체크한다
		if (itemMgr->IsInvenSlotFull())
		{
			InventorySlotErrorTickerNet(bUseMessage);
			GSLOG(Warning, TEXT("[RewardHelper] MainReward - Inventory Slot Full!"));
			return false;
		}

		// 무게 체크
		int32 addWeight = GetMaxWeightFromRewardData(mainRewardData);

		if (itemMgr->IsInvenWeightFull(addWeight))
		{
			InventorySlotErrorTickerNet(bUseMessage);
			GSLOG(Warning, TEXT("[RewardHelper] MainReward - Inventory Slot Full!"));
			return false;
		}

		break;
	}
	// RANDOM_GROUP 일 경우, 데이터가 잘못 입력되었다는 뜻 (필드 전용이므로)
	case RewardType::RANDOM_GROUP:
	{
#if WITH_EDITOR
		FGsUIHelper::PopupSystemMsg(FText::FromString(TEXT("[RewardHelper] Main RewardType cannot be RANDOM_GROUP!")));
#endif
		GSLOG(Error, TEXT("[RewardHelper] Main RewardType cannot be RANDOM_GROUP!"));
		return false;
	}
	}

	return true;
}

void FGsRewardHelper::InventorySlotErrorTickerQuest(bool bUseMessage)
{
	FText findText;

	// 인벤토리 슬롯 부족으로 퀘스트 보상 수령 불가능한 경우
	if (FText::FindText(TEXT("QuestUIText"), TEXT("InvenError"), findText))
	{
		FGsUIHelper::TrayMessageTicker(findText);
	}
}

void FGsRewardHelper::InventoryWeightErrorTickerQuest(bool bUseMessage)
{
	FText findText;

	// 무게 부족으로 퀘스트 보상 수령 불가능한 경우
	if (FText::FindText(TEXT("QuestUIText"), TEXT("weightError"), findText))
	{
		FGsUIHelper::TrayMessageTicker(findText);
	}
}

void FGsRewardHelper::InventorySlotErrorTickerNet(bool bUseMessage)
{
	FText findText;

	// 인벤토리 슬롯 부족으로 보상 수령 불가능한 경우
	if (FText::FindText(TEXT("NetText"), TEXT("ITEM_ERROR_ADD_INVENTORY_FULL"), findText))
	{
		FGsUIHelper::TrayMessageTicker(findText);
	}
}

void FGsRewardHelper::InventoryWeightErrorTickerNet(bool bUseMessage)
{
	FText findText;

	// 무게 부족으로 보상 수령 불가능한 경우
	if (FText::FindText(TEXT("NetText"), TEXT("ITEM_ERROR_ADD_WEIGHT_FULL"), findText))
	{
		FGsUIHelper::TrayMessageTicker(findText);
	}
}
