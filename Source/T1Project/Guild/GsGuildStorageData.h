// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Classes/GsPool.h"
#include "Guild/GsGuildMemberData.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "Shared/Client/SharedEnums/SharedGuildEnum.h"
#include "Shared/Client/SharedEnums/SharedCommonEnum.h"
#include "Shared/Shared/SharedPackets/PD_Gateway_Client.h"


class FGsItem;
class FGsGuildStorageData;
struct FGsGuildStorageMemberData;
struct FGsGuildStorageAcquireDetailData;
struct FGsGuildStorageDistributionDetailData;
//----------------------------------------------------------------------------------------------------------------------
/**
 * 기사단 창고 데이터
 */
class T1PROJECT_API FGsGuildStorageData final
{
public:
	int32 MaxAcquiredItemHistoryCount = 0; // 획득 내역 최대 출력 수
	int32 MaxDistributedItemHistoryCount = 0; // 분배 내역 최대 출력 수
	ItemAmount SafeCapacityDefault = 0; // 창고 기본 용량
	int32 SafeStorageTimeMax = 0; // 창고 아이템 유효 기간
	ItemAmount ExtendSlotCount = 0; // 늘어나는 슬롯 수
	TMap<int32, TPair<CurrencyType,Currency>> ExtendCurrencyList; // 슬롯 확장 비용 리스트

public:
	time_t _lastInvenCheckTime = 0;
	time_t _lastAcquiredCheckTime = 0;
	time_t _lastDistributedCheckTime = 0;
	time_t _lastItemAcquiredTime = 0;
	time_t _lastItemDistributedTime = 0;
	TArray<FGsGuildStorageMemberData*> _memberList;
	TArray<FGsGuildStorageAcquireDetailData*> _acquireDetailList;
	TArray<FGsGuildStorageDistributionDetailData*> _distributionDetailList;
	TMap<ItemDBId, FGsItem*> _itemList;
	ItemAmount _maxSlotCount = 0;
	SlotExtendNumber _slotExtendedCount = 0;
	TGsPoolDefault<FGsGuildStorageMemberData> _poolGuildMemberData;
	TGsPoolDefault<FGsGuildStorageAcquireDetailData> _poolAcquiredData;
	TGsPoolDefault<FGsGuildStorageDistributionDetailData> _poolDistributedData;

protected:
	bool _dirtyMember = false;
	bool _dirtyAcquired = false;
	bool _dirtyDistributed = false;
	bool _dirtyItemList = false;

public:
	FGsGuildStorageData() = default;
	~FGsGuildStorageData() = default;

public:
	void Initialize();

public:
	void SetData(PD::GC::PKT_GC_ACK_OPEN_GUILD_STORE_READ& InPacket);
	void SetData(PD::GC::PKT_GC_ACK_GUILD_STORE_GUILD_MEMBER_LIST_READ& InPacket);
	void SetData(PD::GC::PKT_GC_ACK_GUILD_STORE_ACQUIRED_ITEM_HISTORY_READ& InPacket);
	void SetData(PD::GC::PKT_GC_ACK_GUILD_STORE_DISTRIBUTED_ITEM_HISTORY_READ& InPacket);
	void SetData(PD::GC::PKT_GC_ACK_GUILD_STORE_ITEM_LIST_READ& InPacket);
	void SetLastItemInfo(time_t InAcquiredTime, time_t InDistributedTime);
	void ResetData();

public:
	void SetAcquireDetailList(const TArray<AcquiredGuildItemHistory>& InAcquiredList);
	void AddDistributionDetail(const ItemDataCreated& InItemData, time_t InDistributedTime, const FString& InDistributorName, const FString& InReceiverName);
	void AddRemainDistributionDetailList(const TArray<FGsGuildStorageDistributionDetailData*>& InDistributionDetailList);
	void SortDistributionDetailList();
	void SetInventoryInfo(ItemAmount InMaxSlotCount, SlotExtendNumber InSlotExtendedCount);

public:
	void SetDirtyMember(bool bIsDirty);
	void SetDirtyAcquired(bool bIsDirty, bool bIsNotify = false);
	void SetDirtyDistributed(bool bIsDirty, bool bIsNotify = false);
	void SetDirtyItemList(bool bIsDirty);

public:
	bool GetDirtyMember() { return _dirtyMember; }
	bool GetDirtyAcquired() { return _dirtyAcquired; }
	bool GetDirtyDistributed() { return _dirtyDistributed; }
	bool GetDirtyItemList() { return _dirtyItemList; }

public:
	TMap<ItemDBId, FGsItem*>& GetItemList();
	FGsItem* GetItemData(ItemDBId InItemDBId);

	time_t GetLastDistributeHistoryTime();

public:
	void UpdateInvenCheckTime();
	void UpdateAcquiredCheckTime();
	void UpdateDistributedCheckTime();

private:
	void RemoveAllGuildMember();
	void RemoveAllAcquireDetail();
	void RemoveAllDistributionDetail();
	void RemoveAllStorageItem();

public:
	void RemoveAll();

public:
	bool IsEnoughExtendSlotCost();
};

//----------------------------------------------------------------------------------------------------------------------
/**
 * 기사단 창고 아이템 분배 기사단원 목록 데이터
 */
struct T1PROJECT_API FGsGuildStorageMemberData final
{
public:
	UserDBId _userDBId;
	FString _userName;
	Level _level = INVALID_LEVEL;
	GuildMemberGradeType _memberGrade;
	int32 _weeklyContribution;
	int32 _accumContribution;
	
public:
	FGsGuildStorageMemberData() = default;
	FGsGuildStorageMemberData(UserDBId InUserDBId, FString InUserName, Level InLevel,
		GuildMemberGradeType InMemberGrade, int32 InWeeklyContribution, int32 InAccumContribution)
		: _userDBId(InUserDBId), _userName(InUserName), _level(InLevel),
		_memberGrade(InMemberGrade), _weeklyContribution(InWeeklyContribution), _accumContribution(InAccumContribution) {}
	~FGsGuildStorageMemberData() = default;

	void Set(PD::GC::PKT_GC_ACK_OPEN_GUILD_STORE_READ::MemberList& InData);
	void Set(PD::GC::PKT_GC_ACK_GUILD_STORE_GUILD_MEMBER_LIST_READ::MemberList& InData);
};

//----------------------------------------------------------------------------------------------------------------------
/**
 * 기사단 창고 획득 내역 데이터
 */
struct T1PROJECT_API FGsGuildStorageAcquireDetailData final
{
public:
	FGsItem* _acquiredItem = nullptr;	// 획득 아이템 정보
	time_t _acquiredTime = 0;	// 획득 시간
	GuildItemAcquireType _itemAcquireType = GuildItemAcquireType::NONE;	// 획득 위치

public:
	FGsGuildStorageAcquireDetailData() = default;
	~FGsGuildStorageAcquireDetailData() = default;

};
//----------------------------------------------------------------------------------------------------------------------
/**
 * 기사단 창고 분배 내역 데이터
 */
struct T1PROJECT_API FGsGuildStorageDistributionDetailData final
{
public:
	FGsItem* _distributedItem = nullptr;	// 분배 아이템 정보
	FString _giverName;				// 분배한 사람
	FString _receiverName;			// 분배 받은 사람
	time_t _distributedTime = 0;	// 분배 시간

public:
	FGsGuildStorageDistributionDetailData() = default;
	~FGsGuildStorageDistributionDetailData() = default;

};
