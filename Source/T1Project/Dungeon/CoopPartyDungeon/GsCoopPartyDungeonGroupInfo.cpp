

#include "GsCoopPartyDungeonGroupInfo.h"
#include "Management/GsMessageHolder.h"
#include "Message/MessageParam/GsItemMessageParam.h"
#include "Management/ScopeGame/GsUnlockManager.h"
#include "Item/GsItemManager.h"
#include "DataSchema/Item/GsSchemaItemCommon.h"
#include "DataSchema/Dungeon/GsSchemaDungeonData.h"
#include "DataSchema/Dungeon/GsSchemaDungeonGroupData.h"
#include "../Message/MessageParam/GsMessageParam.h"




FGsCoopPartyDungeonGroupInfo::FGsCoopPartyDungeonGroupInfo(const struct FGsSchemaDungeonGroupData* InDungeonGroupData, DungeonDifficulty InDungeonDifficulty)
	: FGsDungeonGroupInfo(InDungeonGroupData, NoneNum, NoneNum)
	, _clearDifficulty(InDungeonDifficulty)
	, _dungeonKeyId(_dungeonGroupData->dungeonKey.GetRow()->id)
	, _costDungeonKey(MakeCostPackage(_dungeonKeyId))
{	
	/**
	 * Create CoopParty Dungeon Group Data
	 */
	RegisterMessage();
}

void FGsCoopPartyDungeonGroupInfo::Clear()
{
	UnRegisterMessage();
}

/************************************************************************/
/* Message Delegate                                                     */
/************************************************************************/
void FGsCoopPartyDungeonGroupInfo::RegisterMessage()
{
	FGsMessageHolder* msg = GMessage();
	if (msg)
	{
	//	// 2021/08/02 PKT - inventory Item - New ..
	//	_listItemActionDelegates.Emplace(msg->GetItem().AddRaw(MessageItem::ADDITEM, this, &FGsCoopPartyDungeonGroupInfo::AddItem));
	//	// 2021/08/02 PKT - inventory Item - Update ..
	//	_listItemActionDelegates.Emplace(msg->GetItem().AddRaw(MessageItem::UPDATEITEM, this, &FGsCoopPartyDungeonGroupInfo::UpdateItem));
	//	// 2021/08/02 PKT - inventory Item - Remove ..
	//	_listItemActionDelegates.Emplace(msg->GetItem().AddRaw(MessageItem::REMOVEITEM, this, &FGsCoopPartyDungeonGroupInfo::RemoveItem));
		
		// 2022/05/25 PKT - UnLock Contents
		_unlockDelegate = msg->GetUnlock().AddRaw(MessageContentUnlock::NOTIFY_CONTENTS_UNLOCK,this, &FGsCoopPartyDungeonGroupInfo::OnContentsUnlock);
	}
}

void FGsCoopPartyDungeonGroupInfo::UnRegisterMessage()
{
	FGsMessageHolder* msg = GMessage();
	if (msg)
	{
	//	for (const TPair<MessageItem, FDelegateHandle>& ItemDelegate : _listItemActionDelegates)
	//	{
	//		msg->GetItem().Remove(ItemDelegate);
	//	}
	//	_listItemActionDelegates.Empty();

		msg->GetUnlock().Remove(_unlockDelegate);
	}
}


// 2022/05/24 PKT - Item 상태 변화
void FGsCoopPartyDungeonGroupInfo::AddItem(FGsItemMessageParamBase& InParam)
{
	const FGsAddItem* Param = StaticCast<const FGsAddItem*>(&InParam);
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
		ChangedDungeonKey(item.Pin().Get()->GetTID());
	}
}

void FGsCoopPartyDungeonGroupInfo::UpdateItem(FGsItemMessageParamBase& InParam)
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
		ChangedDungeonKey(item.Pin().Get()->GetTID());
	}
}

void FGsCoopPartyDungeonGroupInfo::RemoveItem(FGsItemMessageParamBase& InParam)
{
	const FGsRemoveItem* Param = StaticCast<const FGsRemoveItem*>(&InParam);
	check(Param);
	ChangedDungeonKey(Param->_itemTID);
}

void FGsCoopPartyDungeonGroupInfo::ChangedDungeonKey(const ItemId InItemId)
{
	int32 elementCount = _costDungeonKey->GetCostElementsCount();
	for (int32 idx = 0; idx < elementCount; ++idx)
	{
		const auto& element = _costDungeonKey->GetCostElementAt(idx);		
		if (element && InItemId == element->GetItemId())
		{	// 2022/05/25 PKT - UI 갱신 요청
			RequestRefreshUI();
			break;
		}
	}
}

// 2022/05/25 PKT - UnLock Contents
void FGsCoopPartyDungeonGroupInfo::OnContentsUnlock(const IGsMessageParam* InParam)
{
	//if (_dungeonGroupData && StaticCast<EGsUnlockContentsType>(InParam) == _dungeonGroupData->unLockContentsType)
	//{	// 2022/05/25 PKT - UI 갱신 요청
	//	RequestRefreshUI();
	//}
}

void FGsCoopPartyDungeonGroupInfo::RequestRefreshUI()
{	
	// 2022/05/25 PKT - UI 갱신 요청(Dungeon Manager에게 알린다.)
	_onRequestRefreshUI.ExecuteIfBound();
}

/************************************************************************/
/* 2022/05/23 PKT - setter                                              */
/************************************************************************/
// 2022/05/25 PKT - 진입 가능한 난이도 설정(서버에서 설정을 내려 줌)
void FGsCoopPartyDungeonGroupInfo::SetDifficulty(DungeonDifficulty InType)
{	// 2022/05/23 PKT - 진행 가능한 난이도
	_clearDifficulty = InType;
}


//
///************************************************************************/
///* 2022/05/23 PKT - getter                                              */
///************************************************************************/
//bool FGsCoopPartyDungeonGroupInfo::IsUnLockContents() const
//{
//	return GSUnlock()->IsContentsUnlock(GetUnLockContensType());
//}
//
//EGsUnlockContentsType FGsCoopPartyDungeonGroupInfo::GetUnLockContensType() const
//{
//	return _dungeonGroupData->unLockContentsType;
//}
//
//
//DungeonDifficulty FGsCoopPartyDungeonGroupInfo::IsPossibleDifficulty() const
//{
//	if (false == GSUnlock()->IsContentsUnlock(GetUnLockContensType()))
//	{
//		return DungeonDifficulty::NONE;
//	}
//
//	return (DungeonDifficulty::EASY < _clearDifficulty) ? _clearDifficulty : DungeonDifficulty::EASY;
//}
//

DungeonDifficulty FGsCoopPartyDungeonGroupInfo::GetClearDifficulty() const
{
	return _clearDifficulty;
}

int32 FGsCoopPartyDungeonGroupInfo::GetWeeeklyRewardCount() const
{
	return _weeeklyRewardCount;
}

CurrencyDelta FGsCoopPartyDungeonGroupInfo::GetDungeonKeyAmount() const
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
				outAmount += element->GetCostAmount();
			}
		}
	}

	return outAmount;
}


/************************************************************************/
/* 2022/05/23 PKT - Finder                                              */
/************************************************************************/
// 2022/05/25 PKT - 난이도 별 던전 Data
const FGsSchemaDungeonData* FGsCoopPartyDungeonGroupInfo::FindDungeonDataByDifficulty(DungeonDifficulty InType) const
{
	for (const auto& dungeonRow : _dungeonGroupData->dungeonList)
	{
		const FGsSchemaDungeonData* data = dungeonRow.GetRow();
		if (data && InType == data->levelOfDifficulty)
		{
			return data;
		}
	}
	// 2022/05/23 PKT - not find...
	return nullptr;
}

TSharedRef<const FGsCostPackage> FGsCoopPartyDungeonGroupInfo::MakeCostPackage(const ItemId InDungeonKeyId)
{
	TSharedRef<FGsCostPackage> costPackage = MakeShared<FGsCostPackage>();
	costPackage->SetJointCostCandidateElements(true);

	const static int32 DEFAULT_CURRENCY_DELTA = 1;
	costPackage->AddCost(InDungeonKeyId, DEFAULT_CURRENCY_DELTA);

	if (0 >= costPackage->GetCostElementsCount())
	{
		GSLOG(Error, TEXT("0 >= costPackage->GetCostElementsCount()"));
	}
	return costPackage;
}
