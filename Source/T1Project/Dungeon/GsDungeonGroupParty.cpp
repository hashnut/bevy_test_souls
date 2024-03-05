

#include "GsDungeonGroupParty.h"
#include "GsDungeonData.h"
#include "../Item/GsItem.h"
#include "../Item/GsItemManager.h"
#include "../Management/GsMessageHolder.h"
#include "../Message/GsMessageItem.h"
#include "../Message/MessageParam/GsItemMessageParam.h"
#include "DataSchema/Dungeon/GsSchemaDungeonGroupData.h"
#include "DataSchema/Dungeon/GsSchemaDungeonData.h"
#include "DataSchema/Item/GsSchemaItemCommon.h"
#include "../T1Project.h"



FGsDungeonGroupParty::FGsDungeonGroupParty(const struct FGsSchemaDungeonGroupData* InSchemaData, int32 InDefaultRewardCount)
	: FGsDungeonGroupBase(InSchemaData), _lastClearDifficulty(DungeonDifficulty::NONE)
	, _costDungeonKey(CostItemPackage(InSchemaData->dungeonKey.GetRow()))
	, _defaultRewardCount(InDefaultRewardCount)
{

}

void FGsDungeonGroupParty::AddItem(struct FGsItemMessageParamBase& InParam)
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

void FGsDungeonGroupParty::UpdateItem(struct FGsItemMessageParamBase& InParam)
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

void FGsDungeonGroupParty::RemoveItem(struct FGsItemMessageParamBase& InParam)
{
	const FGsRemoveItem* Param = StaticCast<const FGsRemoveItem*>(&InParam);
	check(Param);
	CheckedCostItem(Param->_itemTID);
}

void FGsDungeonGroupParty::CheckedCostItem(const ItemId InItemId)
{
	int32 elementCount = _costDungeonKey->GetCostElementsCount();
	for (int32 idx = 0; idx < elementCount; ++idx)
	{
		const auto& element = _costDungeonKey->GetCostElementAt(idx);
		if (element && InItemId == element->GetItemId())
		{	// 2022/05/25 PKT - UI 갱신 요청
			ChangedState();
			break;
		}
	}
}

bool FGsDungeonGroupParty::IsDungeonUnLockContentsDelegate(const FGsDungeonData* InTarget, bool InShowTicker /*= false*/)
{
	if (false == FGsDungeonGroupBase::IsDungeonUnLockContentsDelegate(InTarget, InShowTicker))
	{	// 2023/2/21 PKT - Group 에서 이미 락에 걸려 있다면 던전 검사는 하지 않는다.
		return false;
	}

	// 2022/08/17 PKT - 그룹 언락이 해제 되었다면 던전 언락을 검사.
	const FGsSchemaDungeonData* schemaData = InTarget->GetSchema();
	
	if (schemaData->levelOfDifficulty <= DifficultyToEnter())
	{
		return true;
	}

	return false;
}

void FGsDungeonGroupParty::SetClearDifficulty(DungeonDifficulty InType)
{
	_lastClearDifficulty = InType;

	for (auto& item : _dungeonDataHolder)
	{
		if (false == item.IsUnLockState())
		{
			item.IsUnLockContents(false);
		}
	}

	ChangedState();
}

void FGsDungeonGroupParty::SetHaveWeekRewardCount(int32 InCount)
{
	_defaultRewardCount = InCount;
	
	ChangedState();
}

DungeonDifficulty FGsDungeonGroupParty::DifficultyToEnter() const
{
	int32 maxDifficulty = StaticCast<int32>(DungeonDifficulty::MAX) - 1;
	int32 difficultyToEnter = StaticCast<int32>(_lastClearDifficulty) + 1;

	return difficultyToEnter <= maxDifficulty ? StaticCast<DungeonDifficulty>(difficultyToEnter) : StaticCast<DungeonDifficulty>(maxDifficulty);
}

DungeonDifficulty FGsDungeonGroupParty::CurrentDifficulty() const
{
	return _lastClearDifficulty;
}

// 2022/05/25 PKT - 던전 보상 받을 수 있는 아이템 갯수
CurrencyDelta FGsDungeonGroupParty::GetDungeonKeyAmount() const
{
	CurrencyDelta outAmount = 0;
	if (true == _costDungeonKey->PlayerHasEnough())
	{	// 2022/05/23 PKT - cost Item은 1개일테지만 일단 다 돌아준다.
		const int32 elementCount = _costDungeonKey->GetCostElementsCount();
		for (int32 index = 0; index < elementCount; ++index)
		{
			const auto& element = _costDungeonKey->GetCostElementAt(index);
			if (element && 0 < element->GetCostAmount())
			{
				outAmount += element->PlayerHasAmountMax();
			}
		}
	}
	return outAmount;
}

int32 FGsDungeonGroupParty::GetDefaultHaveRewardCount() const
{
	return _defaultRewardCount;
}

const FGsDungeonData* FGsDungeonGroupParty::FindDungeonByDifficulty(DungeonDifficulty InType) const
{
	for (const auto& item : _dungeonDataHolder)
	{
		const FGsSchemaDungeonData* schemaData = item.GetSchema();
		if (schemaData && schemaData->levelOfDifficulty == InType)
		{
			return &item;
		}
	}

	return nullptr;
}
