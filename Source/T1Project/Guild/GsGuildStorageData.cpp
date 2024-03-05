// Fill out your copyright notice in the Description page of Project Settings.


#include "GsGuildStorageData.h"
#include "Guild/GsGuildHelper.h"

#include "DataSchema/Guild/GsSchemaGuildConfig.h"
#include "DataSchema/Guild/GsSchemaGuildStore.h"
#include "DataSchema/Guild/GsSchemaGuildStoreExtend.h"

#include "Item/GsItem.h"
#include "Item/GsItemEquipment.h"

#include "Management/GsMessageHolder.h"

#include "Management/ScopeGlobal/GsUIManager.h"
#include "Management/ScopeGlobal/GsGuildManager.h"
#include "Management/ScopeGlobal/GsGameDataManager.h"

#include "UTIL/GsTimeSyncUtil.h"

#include "Currency/GsCurrencyHelper.h"

#include "Engine/Engine.h"


void FGsGuildStorageData::SetData(PD::GC::PKT_GC_ACK_OPEN_GUILD_STORE_READ& InPacket)
{
	// 인벤토리 상태 설정
	SetInventoryInfo(InPacket.MaxSlotCount(), InPacket.SlotExtendedNumber());

	// 기사단원 정보 설정
	RemoveAllGuildMember();
	for (PD::GC::PKT_GC_ACK_OPEN_GUILD_STORE_READ::MemberListIterator iter = InPacket.GetFirstMemberListIterator();
		iter != InPacket.GetLastMemberListIterator(); ++iter)
	{
		FGsGuildStorageMemberData* data = _poolGuildMemberData.Claim();
		data->Set(*iter);

		_memberList.Emplace(data);
	}
	SetDirtyMember(true);

	// 아이템 획득 내역 정보 설정
	TArray<AcquiredGuildItemHistory> acquiredGuildItemHistoryList;
	InPacket.MakeAcquiredGuildItemHistoryListVector<TArray<AcquiredGuildItemHistory>, AcquiredGuildItemHistory>(acquiredGuildItemHistoryList);
	SetAcquireDetailList(acquiredGuildItemHistoryList);

	// 아이템 분배 내역 정보 설정
	// 같은 페이지에서 갱신되었을 때 레드닷 유지를 위해서
	// 추가된 기록은 생성, 기존 기록은 재활용한다.
	time_t lastDistributeHistoryTime = GetLastDistributeHistoryTime();
	TArray<FGsGuildStorageDistributionDetailData*> distributionDetailList = _distributionDetailList;
	_distributionDetailList.Empty();
	bool dirty = false;
	for (PD::GC::PKT_GC_ACK_OPEN_GUILD_STORE_READ::DistributeListIterator iter = InPacket.GetFirstDistributeListIterator();
		iter != InPacket.GetLastDistributeListIterator(); ++iter)
	{
		if (MaxDistributedItemHistoryCount <= _distributionDetailList.Num())
			break;

		// 기존 목록 내 최근 기록보다 최근 기록이 들어왔을 때 추가
		if (lastDistributeHistoryTime < iter->DistributedTime())
		{
			AddDistributionDetail(iter->ItemDataCreated(), iter->DistributedTime(), iter->DistributorName(), iter->ReceiverName());

			if (_lastDistributedCheckTime < iter->DistributedTime())
			{
				dirty = true;
			}
		}
	}

	AddRemainDistributionDetailList(distributionDetailList);
	SortDistributionDetailList();

	if (dirty)
	{
		SetDirtyDistributed(true);
	}

	GUI()->Open(TEXT("TrayGuildStorage"));
}

void FGsGuildStorageData::SetData(PD::GC::PKT_GC_ACK_GUILD_STORE_GUILD_MEMBER_LIST_READ& InPacket)
{
	// 기사단원 정보 설정
	RemoveAllGuildMember();
	for (PD::GC::PKT_GC_ACK_GUILD_STORE_GUILD_MEMBER_LIST_READ::MemberListIterator iter = InPacket.GetFirstMemberListIterator();
		iter != InPacket.GetLastMemberListIterator(); ++iter)
	{
		FGsGuildStorageMemberData* data = _poolGuildMemberData.Claim();
		data->Set(*iter);

		_memberList.Emplace(data);
	}
	SetDirtyMember(true);
}

void FGsGuildStorageData::SetData(PD::GC::PKT_GC_ACK_GUILD_STORE_ACQUIRED_ITEM_HISTORY_READ& InPacket)
{
	// 아이템 획득 내역 정보 설정
	TArray<AcquiredGuildItemHistory> acquiredGuildItemHistoryList;
	InPacket.MakeAcquiredGuildItemHistoryListVector<TArray<AcquiredGuildItemHistory>, AcquiredGuildItemHistory>(acquiredGuildItemHistoryList);
	SetAcquireDetailList(acquiredGuildItemHistoryList);
}

void FGsGuildStorageData::SetData(PD::GC::PKT_GC_ACK_GUILD_STORE_DISTRIBUTED_ITEM_HISTORY_READ& InPacket)
{
	// 아이템 분배 내역 정보 설정
	// 같은 페이지에서 갱신되었을 때 레드닷 유지를 위해서
	// 추가된 기록은 생성, 기존 기록은 재활용한다.
	time_t lastDistributeHistoryTime = GetLastDistributeHistoryTime();
	TArray<FGsGuildStorageDistributionDetailData*> distributionDetailList = _distributionDetailList;
	_distributionDetailList.Empty();
	bool dirty = false;
	for (PD::GC::PKT_GC_ACK_GUILD_STORE_DISTRIBUTED_ITEM_HISTORY_READ::DistributeListIterator iter = InPacket.GetFirstDistributeListIterator();
		iter != InPacket.GetLastDistributeListIterator(); ++iter)
	{
		if (MaxDistributedItemHistoryCount <= _distributionDetailList.Num())
			break;

		// 기존 목록 내 최근 기록보다 최근 기록이 들어왔을 때 추가
		if (lastDistributeHistoryTime < iter->DistributedTime())
		{
			AddDistributionDetail(iter->ItemDataCreated(), iter->DistributedTime(), iter->DistributorName(), iter->ReceiverName());

			if (_lastDistributedCheckTime < iter->DistributedTime())
			{
				dirty = true;
			}
		}
	}

	AddRemainDistributionDetailList(distributionDetailList);
	SortDistributionDetailList();

	if (dirty)
	{
		SetDirtyDistributed(true);
	}
}

void FGsGuildStorageData::SetData(PD::GC::PKT_GC_ACK_GUILD_STORE_ITEM_LIST_READ& InPacket)
{
	RemoveAllStorageItem();
	
	TSet<ItemDBId> itemDBIDSet;
	for (const auto& item : _itemList)
	{
		itemDBIDSet.Emplace(item.Key);
	}

	// 기사단 창고 아이템은 ItemDataEquip에 통합되어 들어오고 있다.
	const uchar* Invenbuf = InPacket.ItemDataEquipList(); // ItemData들의 메모리 시작점
	int size = InPacket.ItemDataEquipListSize(); // ItemData들의 바이트 크기

	if (0 < size)
	{
		int dataSize = sizeof(ItemDataEquip);
		int count = size / dataSize;

		int binIndex = 0;
		for (int i = 0; i < count; ++i)
		{
			const ItemDataEquip* equipData = reinterpret_cast<const ItemDataEquip*>(&Invenbuf[binIndex]);
			if (nullptr != equipData)
			{
				if (_itemList.Contains(equipData->mItemDBId))
				{
					_itemList[equipData->mItemDBId]->SetAmount(equipData->mItemAmount);
					itemDBIDSet.Remove(equipData->mItemDBId);
				}
				else
				{
					FGsItem* item = FGsItem::Create(equipData->mTemplateId, equipData->mItemAmount);
					if (nullptr == item)
					{
						binIndex += sizeof(ItemDataEquip);
						continue;
					}
					item->UpdateBy(equipData);
					item->SetIsNew((_lastInvenCheckTime < equipData->mExpiredTime - SafeStorageTimeMax) ? true : false);
					//item->SetIsGuildStorageItem(true);

					_itemList.Emplace(equipData->mItemDBId, item);
				}
			}
			binIndex += sizeof(ItemDataEquip);
		}
	}

	for (auto& itemDBID : itemDBIDSet)
	{
		if (_itemList.Contains(itemDBID))
		{
			delete _itemList[itemDBID];
			_itemList[itemDBID] = nullptr;

			_itemList.Remove(itemDBID);
		}
	}
}

void FGsGuildStorageData::SetLastItemInfo(time_t InAcquiredTime, time_t InDistributedTime)
{
	_lastItemAcquiredTime = InAcquiredTime;
	_lastItemDistributedTime = InDistributedTime;
}

void FGsGuildStorageData::ResetData()
{
	_memberList.Empty();
	_acquireDetailList.Empty();
	_distributionDetailList.Empty();

	_poolGuildMemberData.RemovePool();
	_poolAcquiredData.RemovePool();
	_poolDistributedData.RemovePool();

	RemoveAllStorageItem();
}

void FGsGuildStorageData::RemoveAllGuildMember()
{
	for (FGsGuildStorageMemberData* member : _memberList)
	{
		_poolGuildMemberData.Release(member);
	}
	_memberList.Empty();
}

void FGsGuildStorageData::RemoveAllAcquireDetail()
{
	for (FGsGuildStorageAcquireDetailData* acquireDetail : _acquireDetailList)
	{
		_poolAcquiredData.Release(acquireDetail);
	}
	_acquireDetailList.Empty();
}

void FGsGuildStorageData::RemoveAllDistributionDetail()
{
	for (FGsGuildStorageDistributionDetailData* distributionDetail : _distributionDetailList)
	{
		_poolDistributedData.Release(distributionDetail);
	}
	_distributionDetailList.Empty();
}

void FGsGuildStorageData::RemoveAllStorageItem()
{
	for (auto& item : _itemList)
	{
		delete item.Value;
		item.Value = nullptr;
	}
	_itemList.Empty();
}

void FGsGuildStorageData::RemoveAll()
{
	_maxSlotCount = 0;
	_slotExtendedCount = 0;

	_lastInvenCheckTime = 0;
	_lastAcquiredCheckTime = 0;
	_lastDistributedCheckTime = 0;

	_lastItemAcquiredTime = 0;
	_lastItemDistributedTime = 0;

	_dirtyMember = false;
	_dirtyAcquired = false;
	_dirtyDistributed = false;
	_dirtyItemList = false;

	_memberList.Empty();
	_acquireDetailList.Empty();
	_distributionDetailList.Empty();

	_poolGuildMemberData.RemovePool();
	_poolAcquiredData.RemovePool();
	_poolDistributedData.RemovePool();

	RemoveAllStorageItem();
}

void FGsGuildStorageData::SetAcquireDetailList(const TArray<AcquiredGuildItemHistory>& InAcquiredList)
{
	TArray<FGsGuildStorageAcquireDetailData*> acquireDetailList = _acquireDetailList;
	_acquireDetailList.Empty();

	time_t lastItemHistoryTime = 0;
	if (0 < acquireDetailList.Num())
	{
		lastItemHistoryTime = acquireDetailList[0]->_acquiredTime;
	}
	
	// 같은 페이지에서 갱신되었을 때 레드닷 유지를 위해서
	// 추가된 기록은 생성, 기존 기록은 재활용한다.
	ItemDataCreated item;
	bool dirty = false;
	for (const AcquiredGuildItemHistory& history : InAcquiredList)
	{
		if (MaxAcquiredItemHistoryCount <= _acquireDetailList.Num())
			break;

		// 기존 목록 내 최근 기록보다 최근 기록이 들어왔을 때 추가
		if (lastItemHistoryTime < history.mAcquiredTime)
		{
			// 아이템 생성
			item.mExpiredTime = history.mItemDataCreated.mExpiredTime;
			item.mIsLocked = history.mItemDataCreated.mIsLocked;
			item.mItemAmount = history.mItemDataCreated.mItemAmount;
			item.mItemDBId = history.mItemDataCreated.mItemDBId;
			item.mTemplateId = history.mItemDataCreated.mTemplateId;

			FGsItem* acquiredItem = FGsItem::Create(&item);
			if (nullptr == acquiredItem)
			{
				// 유효하지 않은 아이템
				continue;
			}

			FGsGuildStorageAcquireDetailData* data = _poolAcquiredData.Claim();
			data->_acquiredItem = acquiredItem;
			data->_acquiredItem->SetIsNew((_lastAcquiredCheckTime < history.mAcquiredTime) ? true : false);
			if (ItemType::EQUIP == data->_acquiredItem->GetType())
			{
				FGsItemEquipment* equipItem = data->_acquiredItem->GetCastItem<FGsItemEquipment>();
				if (nullptr != equipItem)
				{
					equipItem->SetLevel(history.mItemDataCreated.mLevel);
					equipItem->SetEnchantShieldCount(history.mItemDataCreated.mEnchantShield);
					equipItem->SetEnchantBonusIndex(history.mItemDataCreated.mEnchantBonusIndex);
				}
			}
			data->_acquiredTime = history.mAcquiredTime;
			data->_itemAcquireType = history.mItemAcquiredType;
			
			_acquireDetailList.Emplace(data);
			if (_lastAcquiredCheckTime < history.mAcquiredTime)
			{
				dirty = true;
			}
		}
	}

	// 추가되지 않은 기록은 기존 기록 중 최신 기록부터 추가
	for (int32 i = 0; i < acquireDetailList.Num(); ++i)
	{
		if (MaxAcquiredItemHistoryCount > _acquireDetailList.Num())
		{
			_acquireDetailList.Emplace(acquireDetailList[i]);
		}
		else
		{
			_poolAcquiredData.Release(acquireDetailList[i]);
		}
	}

	// 정렬
	_acquireDetailList.Sort([](const FGsGuildStorageAcquireDetailData& A, const FGsGuildStorageAcquireDetailData& B)
		{
			return (A._acquiredTime >= B._acquiredTime) ? true : false;
		});

	if (dirty)
	{
		SetDirtyAcquired(true);
	}
}

void FGsGuildStorageData::AddDistributionDetail(const ItemDataCreated& InItemData, time_t InDistributedTime, const FString& InDistributorName, const FString& InReceiverName)
{
	FGsItem* distributedItem = FGsItem::Create(&InItemData);
	if (nullptr == distributedItem)
	{
		// 유효하지 않은 아이템
		return;
	}

	// 아이템 생성
	FGsGuildStorageDistributionDetailData* data = _poolDistributedData.Claim();
	data->_distributedItem = distributedItem;
	data->_distributedItem->SetIsNew((_lastDistributedCheckTime < InDistributedTime) ? true : false);
	if (ItemType::EQUIP == data->_distributedItem->GetType())
	{
		FGsItemEquipment* equipItem = data->_distributedItem->GetCastItem<FGsItemEquipment>();
		if (nullptr != equipItem)
		{
			equipItem->SetLevel(InItemData.mLevel);
			equipItem->SetEnchantShieldCount(InItemData.mEnchantShield);
			equipItem->SetEnchantBonusIndex(InItemData.mEnchantBonusIndex);
		}
	}
	data->_giverName = InDistributorName;
	data->_receiverName = InReceiverName;
	data->_distributedTime = InDistributedTime;

	_distributionDetailList.Emplace(data);
}

void FGsGuildStorageData::AddRemainDistributionDetailList(const TArray<FGsGuildStorageDistributionDetailData*>& InDistributionDetailList)
{
	// 추가되지 않은 기록은 기존 기록 중 최신 기록부터 추가
	for (int32 i = 0; i < InDistributionDetailList.Num(); ++i)
	{
		if (MaxDistributedItemHistoryCount > _distributionDetailList.Num())
		{
			_distributionDetailList.Emplace(InDistributionDetailList[i]);
		}
		else
		{
			_poolDistributedData.Release(InDistributionDetailList[i]);
		}
	}
}

void FGsGuildStorageData::SortDistributionDetailList()
{
	_distributionDetailList.Sort([](const FGsGuildStorageDistributionDetailData& A, const FGsGuildStorageDistributionDetailData& B)
		{
			return (A._distributedTime >= B._distributedTime) ? true : false;
		});
}

void FGsGuildStorageData::SetInventoryInfo(ItemAmount InMaxSlotCount, SlotExtendNumber InSlotExtendedCount)
{
	_maxSlotCount = InMaxSlotCount;
	_slotExtendedCount = InSlotExtendedCount;
}

void FGsGuildStorageData::UpdateInvenCheckTime()
{
	_lastInvenCheckTime = FGsTimeSyncUtil::GetServerNowSecond();
	GGuild()->GetRedDotCheckData().UpdateStorageCheckTime(this);
}

void FGsGuildStorageData::UpdateAcquiredCheckTime()
{
	_lastAcquiredCheckTime = FGsTimeSyncUtil::GetServerNowSecond();
	GGuild()->GetRedDotCheckData().UpdateStorageCheckTime(this);
}

void FGsGuildStorageData::UpdateDistributedCheckTime()
{
	_lastDistributedCheckTime = FGsTimeSyncUtil::GetServerNowSecond();
	GGuild()->GetRedDotCheckData().UpdateStorageCheckTime(this);
}

TMap<ItemDBId, FGsItem*>& FGsGuildStorageData::GetItemList()
{
	return _itemList;
}

FGsItem* FGsGuildStorageData::GetItemData(ItemDBId InItemDBId)
{
	if (_itemList.Contains(InItemDBId))
	{
		return _itemList[InItemDBId];
	}

	return nullptr;
}

void FGsGuildStorageData::Initialize()
{
	const FGsSchemaGuildConfig* guildConfig = FGsGuildHelper::GetGuildConfig();
	if (nullptr == guildConfig)
		return;

	if (false == guildConfig->guildStoreList.IsValidIndex(0))
		return;

	const FGsSchemaGuildStore* storageConfig = &(guildConfig->guildStoreList[0]);
	MaxAcquiredItemHistoryCount = storageConfig->maxAcquiredItemHistoryCount;
	MaxDistributedItemHistoryCount = storageConfig->maxDistributedItemHistoryCount;
	SafeStorageTimeMax = storageConfig->safeStorageTimeMax;
	SafeCapacityDefault = storageConfig->safeCapacityDefault;
	ExtendSlotCount = storageConfig->extendSlotCount;

	ExtendCurrencyList.Empty();
	for (const FGsSchemaGuildStoreExtend& data : storageConfig->extendList)
	{
		ExtendCurrencyList.Emplace(data.extendNumber, TPair<CurrencyType, Currency>(data.currencyType, data.currencyCost));
	}

	_maxSlotCount = SafeCapacityDefault;

	if (GGuild()->IsGuild())
	{
		const FGsGuildRedDotCheckData& info = GGuild()->GetRedDotCheckData();
		if (info._acquiredCheckTime != 0)
		{
			_lastAcquiredCheckTime = info._acquiredCheckTime;
			_lastDistributedCheckTime = info._distributedCheckTime;
			_lastInvenCheckTime = info._invenCheckTime;
		}
		else
		{
			_lastAcquiredCheckTime = FGsTimeSyncUtil::GetServerNowSecond();
			_lastDistributedCheckTime = FGsTimeSyncUtil::GetServerNowSecond();
			_lastInvenCheckTime = FGsTimeSyncUtil::GetServerNowSecond();
			GGuild()->GetRedDotCheckData().UpdateStorageCheckTime(this);
		}

		if (_lastAcquiredCheckTime < _lastItemAcquiredTime)
		{
			SetDirtyAcquired(true, false);
		}
		if (_lastDistributedCheckTime < _lastItemDistributedTime)
		{
			SetDirtyDistributed(true, false);
		}
	}
}

bool FGsGuildStorageData::IsEnoughExtendSlotCost()
{
	if (ExtendCurrencyList.Contains(_slotExtendedCount + 1))
	{
		Currency curValue = FGsCurrencyHelper::GetCurrencyAmount(ExtendCurrencyList[_slotExtendedCount + 1].Key);
		if (curValue >= ExtendCurrencyList[_slotExtendedCount + 1].Value)
		{
			return true;
		}
	}
	
	return false;
}

void FGsGuildStorageData::SetDirtyMember(bool bIsDirty)
{
	_dirtyMember = bIsDirty;
}

void FGsGuildStorageData::SetDirtyAcquired(bool bIsDirty, bool bIsNotify)
{
	_dirtyAcquired = bIsDirty;

	// 레드닷 체크
	GGuild()->CheckRedDotByType(FGsGuildManager::EGsGuildRedDotType::GuildStorage);

	// 레드닷 갱신
	GMessage()->GetHudRedDot().SendMessage(MessageContentHUDRedDot::GUILD, true);

	if (bIsNotify)
	{
		GMessage()->GetGuild().SendMessage(MessageContentGuild::STORAGE_NOTIFY_ACQUIRED, nullptr);
	}
}

void FGsGuildStorageData::SetDirtyDistributed(bool bIsDirty, bool bIsNotify)
{
	_dirtyDistributed = bIsDirty;

	// 레드닷 체크
	GGuild()->CheckRedDotByType(FGsGuildManager::EGsGuildRedDotType::GuildStorage);

	// 레드닷 갱신
	GMessage()->GetHudRedDot().SendMessage(MessageContentHUDRedDot::GUILD, true);

	if (bIsNotify)
	{
		GMessage()->GetGuild().SendMessage(MessageContentGuild::STORAGE_NOTIFY_DISTRIBUTED, nullptr);
	}
}

void FGsGuildStorageData::SetDirtyItemList(bool bIsDirty)
{
	_dirtyItemList = bIsDirty;
}

time_t FGsGuildStorageData::GetLastDistributeHistoryTime()
{
	time_t lastItemHistoryTime = 0;
	if (0 < _distributionDetailList.Num())
	{
		lastItemHistoryTime = _distributionDetailList[0]->_distributedTime;
	}

	return lastItemHistoryTime;
}

void FGsGuildStorageMemberData::Set(PD::GC::PKT_GC_ACK_OPEN_GUILD_STORE_READ::MemberList& InData)
{
	_userDBId = InData.UserDBId();
	_userName = InData.UserName();
	_level = InData.UserLevel();
	_memberGrade = InData.MemberGrade();
	_weeklyContribution = InData.ContributionWeekend();
	_accumContribution = InData.ContributionAccuml();
}

void FGsGuildStorageMemberData::Set(PD::GC::PKT_GC_ACK_GUILD_STORE_GUILD_MEMBER_LIST_READ::MemberList& InData)
{
	_userDBId = InData.UserDBId();
	_userName = InData.UserName();
	_level = InData.UserLevel();
	_memberGrade = InData.MemberGrade();
	_weeklyContribution = InData.ContributionWeekend();
	_accumContribution = InData.ContributionAccuml();
}
