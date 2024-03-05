

#include "GsDungeonGroupBase.h"
#include "GsDungeonData.h"
#include "../Item/GsItemManager.h"
#include "../Item/GsItem.h"
#include "../UTIL/GsCronUtil.h"
#include "../UTIL/GsTableUtil.h"
#include "../Management/GsMessageHolder.h"
#include "../Message/GsMessageContents.h"
#include "../Management/ScopeGame/GsUnlockManager.h"
#include "../Management/ScopeGlobal/GsGameDataManager.h"
#include "../Management/ScopeGame/GsGameObjectManager.h"
#include "../Message/MessageParam/GsItemMessageParam.h"

#include "../GameObject/ObjectClass/GsGameObjectBase.h"
#include "../GameObject/Define/GsGameObjectDefine.h"

#include "DataSchema/Dungeon/GsSchemaDungeonGroupData.h"
#include "DataSchema/Dungeon/GsSchemaDungeonData.h"
#include "DataSchema/Unlock/GsSchemaUnlockContents.h"
#include "DataSchema/Item/GsSchemaItemCommon.h"
#include "../GameObject/Data/GsGameObjectData.h"
#include "DataSchema/Map/GsSchemaMapData.h"
#include "DataSchema/Map/GsSchemaMapBalance.h"
#include "DataSchema/Dungeon/GsSchemaEventDungeonScheduleData.h"
#include "../T1Project.h"




FGsDungeonGroupBase::FGsDungeonGroupBase(const FGsSchemaDungeonGroupData* InSchemaData) : _schemaData(InSchemaData), _environmentPassivityData(nullptr)
{
	auto CheckUnLockFunc = [&](const FGsDungeonData* InTarget, bool InShowTicker) -> bool
	{
		return IsDungeonUnLockContentsDelegate(InTarget, InShowTicker);
	};

	for (const auto& row : InSchemaData->dungeonList)
	{
		const FGsSchemaDungeonData* schemaDungeonData = row.GetRow();
		if (schemaDungeonData && schemaDungeonData->visible)
		{			
			_dungeonDataHolder.Emplace(FGsDungeonData(schemaDungeonData, CheckUnLockFunc));
		}
	}
}

void FGsDungeonGroupBase::Initialized()
{
	_isVisible = false;

	_environmentPassivityData = nullptr;


	RegisterMessage(GMessage());
}

void FGsDungeonGroupBase::Finalized()
{
	UnRegisterMessage(GMessage());

	_isVisible = false;

	_environmentPassivityData = nullptr;
}

void FGsDungeonGroupBase::SetVisible(bool InIsVisible)
{
	_isVisible = InIsVisible;

	OnChangePlayerLevelDelegate(EGsGameObjectDataMember::Level);
}

bool FGsDungeonGroupBase::IsEventGroup() const
{
	// 2023/2/28 PKT - 'visible'은 이벤트 던전 그룹일 경우 False로 놓고 사용 함.
	return (false == _schemaData->visible);
}

bool FGsDungeonGroupBase::IsUIHidden() const
{
	// 2023/2/28 PKT - Visible이 꺼져 있거나 퀘스트 전용 Data이면 Window UI에서 보이지 않는다.
	return (IsDisable() || _schemaData->checkQuest);
}

bool FGsDungeonGroupBase::IsDisable() const
{
	return (false == _isVisible && false == _schemaData->visible);
}

void FGsDungeonGroupBase::RegisterMessage(FGsMessageHolder* InMessageHolder)
{	
	// 2021/08/02 PKT - inventory Item - New ..
	_listItemActionDelegates.Emplace(
		InMessageHolder->GetItem().AddRaw(MessageItem::ADDITEMLIST, this, &FGsDungeonGroupBase::OnAddItem)
	);
	// 2021/08/02 PKT - inventory Item - Update ..
	_listItemActionDelegates.Emplace(
		InMessageHolder->GetItem().AddRaw(MessageItem::UPDATEITEM, this, &FGsDungeonGroupBase::OnUpdateItem)
	);
	// 2021/08/02 PKT - inventory Item - Remove ..
	_listItemActionDelegates.Emplace(
		InMessageHolder->GetItem().AddRaw(MessageItem::REMOVEITEM, this, &FGsDungeonGroupBase::OnRemoveItem)
	);

	// 2022/05/25 PKT - UnLock Contents
	_unlockDelegate = InMessageHolder->GetUnlock().AddRaw(
		MessageContentUnlock::NOTIFY_CONTENTS_UNLOCK, this, &FGsDungeonGroupBase::OnContentsUnlock);

	// 2021/07/31 PKT - spawn / level up
	_listGameObjectDelegate.Emplace(InMessageHolder->GetGameObject().AddRaw(MessageGameObject::LOCAL_SPAWN_ME_COMPLETE
		, this
		, &FGsDungeonGroupBase::OnLocalPlayerSpawnComplete));

	_listGameObjectDelegate.Emplace(InMessageHolder->GetGameObject().AddRaw(MessageGameObject::LOCAL_REVIVE
		, this
		, &FGsDungeonGroupBase::OnLocalPlayerSpawnComplete));
}

void FGsDungeonGroupBase::UnRegisterMessage(FGsMessageHolder* InMessageHolder)
{
	for (auto& item : _listItemActionDelegates)
	{
		InMessageHolder->GetItem().Remove(item);
	}

	_listItemActionDelegates.Empty();

	if (_unlockDelegate.Value.IsValid())
	{
		InMessageHolder->GetUnlock().Remove(_unlockDelegate);
	}

	for (auto& item : _listGameObjectDelegate)
	{
		InMessageHolder->GetGameObject().Remove(item);
	}

	_listGameObjectDelegate.Empty();
}

void FGsDungeonGroupBase::OnLocalPlayerSpawnComplete(const IGsMessageParam* InParam)
{
	UGsGameObjectBase* findLocalPlayer = GSGameObject()->FindObject(EGsGameObjectType::LocalPlayer);
	if (nullptr != findLocalPlayer)
	{
		FGsGameObjectData* gameObjectData = findLocalPlayer->GetData();
		if (nullptr == gameObjectData)
		{
			GSLOG(Error, TEXT("nullptr == gameObjectData"));
		}
		else if (false == gameObjectData->OnDelegate().IsBoundToObject(this))
		{
			gameObjectData->OnDelegate().AddRaw(this, &FGsDungeonGroupBase::OnChangePlayerLevelDelegate);
		}
	}
}

void FGsDungeonGroupBase::OnChangePlayerLevelDelegate(EGsGameObjectDataMember _InMeberType)
{
	// 2023/2/21 PKT - 레벨 변경에 따른 이벤트
	if (EGsGameObjectDataMember::Level == _InMeberType)
	{
		if (IsUnLockContents(false))
		{
			bool isInvalidate = false;
			for (auto& item : _dungeonDataHolder)
			{
				bool isPrevUnlock = item.IsUnLockState();
				bool isCurrentUnlock = item.IsUnLockContents(false);
				if (isCurrentUnlock && isPrevUnlock != isCurrentUnlock)
				{
					isInvalidate = true;
				}
			}

			if (isInvalidate)
			{	// 2023/2/21 PKT - 언락 상태 변경
				ChangedState();
			}
		}
	}
}

void FGsDungeonGroupBase::OnAddItem(struct FGsItemMessageParamBase& InParam)
{
	AddItem(InParam);
}

void FGsDungeonGroupBase::OnUpdateItem(struct FGsItemMessageParamBase& InParam)
{
	UpdateItem(InParam);
}

void FGsDungeonGroupBase::OnRemoveItem(struct FGsItemMessageParamBase& InParam)
{
	RemoveItem(InParam);
}

void FGsDungeonGroupBase::OnContentsUnlock(const IGsMessageParam* InParam)
{
	if (nullptr == InParam)
	{
		return;
	}

	const FGsSchemaUnlockContents* schemaUnlockData = _schemaData->unLockContentsId.GetRow();
	if (schemaUnlockData)
	{
		const FGsPrimitivePairInt32* param = StaticCast<const FGsPrimitivePairInt32*>(InParam);
		if (StaticCast<EGsUnlockContentsType>(param->_first) == schemaUnlockData->contentsType && param->_second == schemaUnlockData->id)
		{
			// 2023/2/21 PKT - UnLock 상태가 변경 되었다면 던전도 상태를 업데이트 한다.
			for (auto& item : _dungeonDataHolder)
			{
				item.IsUnLockContents(false);
			}

			ChangedState();
		}
	}
}

bool FGsDungeonGroupBase::IsDungeonUnLockContentsDelegate(const FGsDungeonData* InTarget, bool InShowTicker /* = false */)
{
	return IsUnLockContents(InShowTicker);
}

void FGsDungeonGroupBase::ChangedState()
{
	if (_schemaData && _OnInvalidate.IsBound())
	{
		_OnInvalidate.ExecuteIfBound(_schemaData->id);
	}
}

const FGsSchemaDungeonGroupData* FGsDungeonGroupBase::GetSchemaData() const
{
	return _schemaData;
}

DungeonGroupId FGsDungeonGroupBase::GetGroupId() const
{
	return _schemaData->id;
}

DungeonCategory FGsDungeonGroupBase::GetCategory() const
{
	return _schemaData->dungeonCategory;
}

int32 FGsDungeonGroupBase::GetListOrder() const
{
	return _schemaData->listOrder;
}

const FGsSchemaPassivitySet* FGsDungeonGroupBase::GetEnvPassivity() const
{
	if (nullptr == _environmentPassivityData && DungeonCategory::PARTY != GetCategory())
	{
		if (0 < _dungeonDataHolder.Num())
		{
			const FGsDungeonData& dungeonData = _dungeonDataHolder[0];
			const FGsSchemaMapData* schemaMapData = dungeonData.GetSchema()->mapId.GetRow();
			const FGsSchemaMapBalance* mapBalanceData = (schemaMapData) ? schemaMapData->balanceId.GetRow() : nullptr;
			if (nullptr == mapBalanceData)
			{
				return nullptr;
			}

			if (false == mapBalanceData->passivityIdList.IsValidIndex(mapBalanceData->passivityIdListIndexEnv))
			{
				return nullptr;
			}

			_environmentPassivityData = mapBalanceData->passivityIdList[mapBalanceData->passivityIdListIndexEnv].GetRow();
		}
	}
	return _environmentPassivityData;
}

bool FGsDungeonGroupBase::IsUnLockContents(bool InShowTicker /* = false */) const
{
	if (_schemaData && _schemaData->unLockContentsId.GetRow())
	{
		const FGsSchemaUnlockContents* unlockContetnsData = _schemaData->unLockContentsId.GetRow();
		return GSUnlock()->IsContentsUnlock(unlockContetnsData->id, InShowTicker);
	}	
	return true;
}

void FGsDungeonGroupBase::LockContentsReason(FText& OutText) const
{
	OutText = FText::GetEmpty();

	if (_schemaData && _schemaData->unLockContentsId.GetRow())
	{
		const FGsSchemaUnlockContents* unlockContetnsData = _schemaData->unLockContentsId.GetRow();
		GSUnlock()->GetUnLockMessage(unlockContetnsData->id, OutText);
	}
}

int32 FGsDungeonGroupBase::DungeonCountAll() const
{
	return _dungeonDataHolder.Num();
}

const FGsDungeonData* FGsDungeonGroupBase::FindDungeonByMapId(MapId InMapId) const
{
	for (const FGsDungeonData& item : _dungeonDataHolder)
	{
		const FGsSchemaDungeonData* schemaData = item.GetSchema();
		if (schemaData && schemaData->mapId.GetRow() && schemaData->mapId.GetRow()->id == InMapId)
		{
			return &item;
		}
	}

	return nullptr;
}

const FGsDungeonData* FGsDungeonGroupBase::FindDungeonByIndex(int32 InIndex) const
{
	return _dungeonDataHolder.IsValidIndex(InIndex) ? &(_dungeonDataHolder[InIndex]) : nullptr;
}

int32 FGsDungeonGroupBase::FindIndexByData(const FGsDungeonData* InData) const
{
	for (int32 idx = 0; idx < _dungeonDataHolder.Num(); ++idx)
	{
		const auto& item = _dungeonDataHolder[idx];
		if (InData && item.GetSchema()->mapId == InData->GetSchema()->mapId)
		{
			return idx;
		}
	}
	
	return -1;
}

bool FGsDungeonGroupBase::IsValidEnterDateTime(FText& OutMessage) const
{	
	/**
	 * 1. 던전 운영 시간은 조건이 없거나 있다면 시간은 무조건 있어야 한다.
	 * 2. 날짜로 운영되는 던전 조건은 요일이 지정되어 있으면 안된다.
	 * 3. 요일로 운영되는 던전 조건은 날짜가 지정되어 있으면 안된다.
	 */
	OutMessage = FText::GetEmpty();

	if (nullptr == _schemaData || true == IsDisable())
	{	// 2022/10/26 PKT - 스키마 데이터가 없거나 활성화 되어 있는 그룹이 아니라면 할 수 있는게 없음.
		return false;
	}

	// 2022/10/26 PKT - Schema Data에서 Cron 정보 가져오기
	FGsSchemaEventDungeonScheduleData scheduleData = _schemaData->eventDungeonSchedule;
	FString openMakeCron = scheduleData.openCycling;
	int playTimeMinute = scheduleData.playTimeMinute;

	if (0 >= playTimeMinute || openMakeCron.IsEmpty())
	{	// 2022/10/26 PKT - 설정 시간이 없음.( 가능 함. )
		return true;
	}

	// 2022/10/26 PKT - 날짜를 갖고 있는가?	
	TArray<int32> openDays;
	FGsCronUtil::GetDays(openMakeCron, openDays);

	// 2022/10/26 PKT - 요일을 갖고 있는가?
	TArray<int32> openWeekDays;
	FGsCronUtil::GetWeekDays(openMakeCron, openWeekDays);

	// 2022/10/26 PKT - 시간을 갖고 있는가?
	const FTimespan openTimespawn = FGsCronUtil::GetTimespan(openMakeCron);
	
	if (0 == openDays.Num() && 0 == openWeekDays.Num() && openTimespawn.IsZero())
	{	// 2022/10/26 PKT - 설정된 시간이 있음에도 정상적으로 Data를 가져오지 못함.( 실패 처리 )
		GSLOG(Error, TEXT("Invalid Cron Data : %s"), *openMakeCron);
		return false;
	}

	// 2022/10/26 PKT - 클라이언트 기준 시
	const FDateTime localTime = FGsTimeSyncUtil::GetClientNowDateTime();

	if (0 < openDays.Num())
	{
		/**
		 * 2022/10/27 PKT - 날짜를 갖고 있다면 요일 검사는 하지 않음.( 날짜 + 시간 )
		 */
		
		// 2022/10/27 PKT - 연/월은 계산하지 않을 것이므로 빼줌.
		FTimespan currentLocalTimeSpan = FTimespan(localTime.GetDay(), localTime.GetHour(), localTime.GetMinute(), localTime.GetSecond());

		for (const auto& openDay : openDays)
		{
			// 2022/10/28 PKT - Open / Close Time 조합
			FDateTime openDate = FDateTime(openTimespawn.GetTicks() + (openDay * ETimespan::TicksPerDay));
			FDateTime closeDate = FDateTime(openDate.GetTicks() + ( playTimeMinute * ETimespan::TicksPerMinute ));

			if (openDate.GetTicks() <= currentLocalTimeSpan.GetTicks() && closeDate.GetTicks() > currentLocalTimeSpan.GetTicks())
			{	// 2022/10/27 PKT - 입장 가능
				return true;
			}
		}
	}
	else if (0 < openWeekDays.Num())
	{	
		/**
		 * 2022/10/27 PKT - 요일을 갖고 있다면 날짜는 검사하지 않는다.
		 */

		// 2022/10/27 PKT - Cron은 (0 or 7)이 일요일 / DateTime은 0이 월요일
		static auto CronToDateConverterWeekDay = [](const int32 InCronWeekDay) -> EDayOfWeek
		{
			return (InCronWeekDay == 0 || InCronWeekDay == 7) ? EDayOfWeek::Sunday : EDayOfWeek(InCronWeekDay - 1);
		};

		// 2022/10/27 PKT - 연/월은 계산하지 않을 것이므로 빼줌.
		FTimespan currentLocalTimeSpan = FTimespan(localTime.GetHour(), localTime.GetMinute(), localTime.GetSecond());
		
		for (const auto& openWeekDay : openWeekDays)
		{
			FDateTime openDate = FDateTime(openTimespawn.GetTicks());
			FDateTime closeDate = FDateTime(openDate.GetTicks() + (playTimeMinute * ETimespan::TicksPerMinute));
			
			// 2022/10/28 PKT - Open / Close 요일을 구함.
			EDayOfWeek openDateWeekDay = CronToDateConverterWeekDay(openWeekDay);
			EDayOfWeek closeDateWeekDay = openDateWeekDay;
			if (openDate.GetDay() != closeDate.GetDay())
			{
				const static int32 ONE_WEEK_DAYS = 7;
				int32 closeCronWeekDay = (ONE_WEEK_DAYS <= openWeekDay + 1) ? 0 : openWeekDay + 1;
				closeDateWeekDay = CronToDateConverterWeekDay(closeCronWeekDay);
			}

			if (openDateWeekDay == localTime.GetDayOfWeek() && closeDateWeekDay == localTime.GetDayOfWeek())
			{
				if (openDate.GetTicks() <= currentLocalTimeSpan.GetTicks() && closeDate.GetTicks() > currentLocalTimeSpan.GetTicks())
				{
					return true;
				}
			}			
		}
	}
	else
	{
		/**
		 * 시간 설정만 되어 있다면...
		 */

		// 2022/10/27 PKT - 연/월은 계산하지 않을 것이므로 빼줌.
		FTimespan currentLocalTimeSpan = FTimespan(localTime.GetHour(), localTime.GetMinute(), localTime.GetSecond());

		FDateTime openDate = FDateTime(openTimespawn.GetTicks());
		FDateTime closeDate = FDateTime(openDate.GetTicks() + (playTimeMinute * ETimespan::TicksPerMinute));

		if (openDate.GetTicks() <= currentLocalTimeSpan.GetTicks() && closeDate.GetTicks() > currentLocalTimeSpan.GetTicks())
		{
			return true;
		}
	}

	/**
	 * 2022/10/28 PKT - 입장이 불가능하다는 노티를 준다.( 메세지는 수정 될 예정.. 요청은 했음. - feat.정윤일님)
	 */
	// 지정된 시간과 요일에만 입장이 가능합니다.		
	FText::FindText(TEXT("UICommonText"), TEXT("DungeonEnterAllowNotice"), OutMessage);

	return false;
}

ContentsBindingType FGsDungeonGroupBase::ContentsBindType() const
{
	return _schemaData->bindingType;
}

TSharedRef<const FGsCostPackage> FGsDungeonGroupBase::CostItemPackage(const FGsSchemaItemCommon* InItemData) const
{
	TSharedRef<FGsCostPackage> costPackage = MakeShared<FGsCostPackage>();

	if (InItemData)
	{
		const static int32 DEFAULT_CURRENCY_DELTA = 1;
		// 2023/8/9 PKT - SetJointCostCandidateElements 이걸 먼저 설정 해주고 AddCost를 해줘야 Elements가 설정이 된다. 
		costPackage->SetJointCostCandidateElements(true);
		costPackage->AddCost(InItemData->id, DEFAULT_CURRENCY_DELTA, true);

		if (0 >= costPackage->GetCostElementsCount())
		{
			GSLOG(Error, TEXT("0 >= costPackage->GetCostElementsCount()"));
		}
	}
	
	return costPackage;
}