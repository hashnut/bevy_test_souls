

#include "GsDungeonGroupNormal.h"
#include "GsDungeonData.h"
#include "Item/GsItem.h"
#include "Item/GsItemManager.h"
#include "Management/ScopeGlobal/GsGameDataManager.h"
#include "Message/MessageParam/GsItemMessageParam.h"

#include "../UTIL/GsTimeSyncUtil.h"

#include "DataSchema/Dungeon/GsSchemaDungeonData.h"
#include "DataSchema/Dungeon/GsSchemaDungeonGroupData.h"
#include "T1Project.h"


FGsDungeonGroupNormal::FGsDungeonGroupNormal(const FGsSchemaDungeonGroupData* InSchemaData)
	: FGsDungeonGroupBase(InSchemaData), _costChargeItem(CostItemPackage(InSchemaData->chargeCostItemId.GetRow()))
{
	
}

void FGsDungeonGroupNormal::Initialized()
{
	FGsDungeonGroupBase::Initialized();

	static const int32 ZERO_TIME = 0;
	SetTime(ZERO_TIME, ZERO_TIME);
}

void FGsDungeonGroupNormal::Finalized()
{
	static const int32 ZERO_TIME = 0;
	SetTime(ZERO_TIME, ZERO_TIME);

	FGsDungeonGroupBase::Finalized();
}

void FGsDungeonGroupNormal::AddItem(struct FGsItemMessageParamBase& InParam)
{
	FGsAddItemList* param = static_cast<FGsAddItemList*>(&InParam);
	if (nullptr == param)
	{
		return;
	}

	TWeakObjectPtr<UGsItemManager> itemManager = GItem();

	for (int32 idx = 0; idx < param->_itemDbidList.Num(); ++idx)
	{
		TWeakPtr<FGsItem> item = itemManager->FindItem(param->_itemDbidList[idx]);
		if (false == item.IsValid())
		{
			GSLOG(Error, TEXT("nullptr == Item, Param->_itemDBID:%lld"), param->_itemDbidList[idx]);
			return;
		}

		CheckedCostItem(item.Pin().Get()->GetTID());
	}
}

void FGsDungeonGroupNormal::UpdateItem(struct FGsItemMessageParamBase& InParam)
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

		CheckedCostItem(item.Pin().Get()->GetTID());
	}
}

void FGsDungeonGroupNormal::RemoveItem(struct FGsItemMessageParamBase& InParam)
{
	const FGsRemoveItem* Param = StaticCast<const FGsRemoveItem*>(&InParam);
	check(Param);

	CheckedCostItem(Param->_itemTID);
}

bool FGsDungeonGroupNormal::IsDungeonUnLockContentsDelegate(const FGsDungeonData* InTarget, bool InShowTicker /*= false*/)
{
	if (false == FGsDungeonGroupBase::IsDungeonUnLockContentsDelegate(InTarget, InShowTicker))
	{	// 2023/2/21 PKT - Group 에서 이미 락에 걸려 있다면 던전 검사는 하지 않는다.
		return false;
	}

	// 2022/08/18 PKT - 정예 던전을 제외한 나머지 던전은 각 레벨에 따른 세부 언락 조건이 존재 한다.
	const FGsSchemaDungeonData* schemaData = InTarget->GetSchema();
	if (DungeonCategory::ELITE != schemaData->dungeonCategory)
	{
		const FGsNetUserData* UserData = GGameData()->GetUserData();
		if (nullptr == UserData)
		{
			GSLOG(Error, TEXT("nullptr == UserData"));
			return true;
		}
		// 2023/2/21 PKT - 조건보다 현재 레벨이 같거나 크다면 락 해제
		return schemaData->unlockLevel <= UserData->mLevel;
	}

	return true;
}

void FGsDungeonGroupNormal::CheckedCostItem(const ItemId InItemId)
{
	int32 elementCount = _costChargeItem->GetCostElementsCount();
	for (int32 idx = 0; idx < elementCount; ++idx)
	{
		const auto& element = _costChargeItem->GetCostElementAt(idx);
		if (element && InItemId == element->GetItemId())
		{	// 2022/05/25 PKT - UI 갱신 요청
			ChangedState();
			break;
		}
	}
}

void FGsDungeonGroupNormal::SetTime(int32 InNormalTime, int32 InChargeTime)
{
	_normalTime = InNormalTime;
	_chargeTime = InChargeTime;

	ChangedState();
}

ItemAmount FGsDungeonGroupNormal::CostAmount() const
{
	ItemAmount outAmount = 0;
	if (true == _costChargeItem->PlayerHasEnough())
	{	// 2022/05/23 PKT - cost Item은 1개일테지만 일단 다 돌아준다.
		const int32 elementCount = _costChargeItem->GetCostElementsCount();
		for (int32 index = 0; index < elementCount; ++index)
		{
			const auto& element = _costChargeItem->GetCostElementAt(index);
			if (element && 0 < element->PlayerHasAmountMax())
			{
				outAmount += element->PlayerHasAmountMax();
			}
		}
	}
	return outAmount;
}

int32 FGsDungeonGroupNormal::GetNormalTime() const
{
	return _normalTime;
}

int32 FGsDungeonGroupNormal::GetChargeTime() const
{
	return _chargeTime;
}

const FDateTime FGsDungeonGroupNormal::GetTime() const
{
	return FGsTimeSyncUtil::GetServerNowDateTime() + FTimespan::FromSeconds(_normalTime + _chargeTime);
}

bool FGsDungeonGroupNormal::IsTimeLimit() const
{
	return (-1 == _normalTime) ? false : true;
}

bool FGsDungeonGroupNormal::IsTimeOver() const
{
	return (0 < _normalTime + _chargeTime) ? false : true;
}