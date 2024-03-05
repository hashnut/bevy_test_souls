// Fill out your copyright notice in the Description page of Project Settings.


#include "GsTutorialWeaponEquip.h"
#include "Message/MessageParam/GsTutorialMessageParam.h"
#include "DataSchema/Tutorial/GsSchemaTutorialTargetId.h"
#include "Tutorial/Action/GsTutorialActionBase.h"
#include "Reward/GsRewardHelper.h"
#include "Item/GsItemManager.h"
#include "Item/GsItem.h"


bool FGsTutorialWeaponEquip::OnPrevStart()
{
	if (_bIsComplete)
	{
		return false;
	}

	if (0 == _targetItemList.Num())
	{
		if (false == SetTargetItemList())
		{
			return false;
		}
	}

	// 인벤토리에 보유하고 있는지 검사
	UGsItemManager* itemMgr = GItem();
	if (nullptr == itemMgr)
	{
		return false;
	}

	TWeakPtr<FGsItem> itemPtr;
	for (const int32& itemId : _targetItemList)
	{
		// 가장 먼저 찾아진 것을 대상으로 함
		TArray<TWeakPtr<FGsItem>> foundList = itemMgr->FindByTID(itemId, ItemType::EQUIP);
		if (0 < foundList.Num())
		{
			if (foundList[0].IsValid())
			{
				itemPtr = foundList[0].Pin();
				break;
			}
		}
	}

	// 없으면 종료
	if (false == itemPtr.IsValid())
	{
		return false;
	}

	_itemId = itemPtr.Pin()->GetTID();

	return Super::OnPrevStart();
}

bool FGsTutorialWeaponEquip::StartActionInternal()
{
	EGsTutorialActionType actionType = _currAction.Pin()->GetActionType();
	if (EGsTutorialActionType::INVENTORY_SELECT_ITEM == actionType)
	{
		// 대상 스킬 정보를 넘겨줌
		FGsTutorialMsgParamUInt32 param(static_cast<uint32>(_itemId));
		_currAction.Pin()->OnPrevStart(&param);
	}

	return Super::StartActionInternal();
}

bool FGsTutorialWeaponEquip::SetTargetItemList()
{
	if (const FGsSchemaTutorialTargetId* targetIdRow = _table->conditionRow.GetRowByType<FGsSchemaTutorialTargetId>())
	{
		RewardId rewardId = static_cast<RewardId>(targetIdRow->targetId);

		FGsRewardUnboxingData unboxData;
		FGsRewardHelper::UnboxingRewardData(rewardId, unboxData);

		_targetItemList.Empty();
		for (const FGsSchemaRewardItemBagData* iter : unboxData._itemDataList)
		{
			_targetItemList.Emplace(iter->itemId);
		}

		if (0 < _targetItemList.Num())
		{
			return true;
		}
	}

	return false;
}
