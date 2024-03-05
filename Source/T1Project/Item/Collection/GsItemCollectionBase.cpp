
#include "GsItemCollectionBase.h"
#include "Item/GsItem.h"
#include "Item/Collection/GsItemCollectionSlotState.h"
#include "GsTableManager.h"
#include "UI/UIContent/Helper/GsEffectStringHelper.h"
#include "UTIL/GsTimeSyncUtil.h"
#include "T1Project/T1Project.h"


FGsItemCollectionBase::FGsItemCollectionBase(const CollectionId InCollectionId, const FText& InName)
	: TGsState(EItemCollectionState::Default), _collectionId(InCollectionId), _name(InName), _timeOfValidity(FDateRange::Empty())
	, _bookMark(false), _cachedStatEffectName(FText::GetEmpty())
{
	if (INVALID_COLLECTION_ID == _collectionId)
	{
		GSLOG(Error, TEXT("_collectionId == INVALID_COLLECTION_ID"));
		return;
	}	

	_globalSlotState = EItemCollectionSlotState::Impossible;

	_cachedTrimVaildNames.Empty();

	_passivityIds.Empty();

	_statInfos.Empty();

	_innerSlots.Empty();

	_volatilityRedDot = false;

	_isUnableToRegister = false;
}

FGsItemCollectionBase::FGsItemCollectionBase(const CollectionId InCollectionId
	, const FText& InName
	, const FString& InStartDate
	, const FString& InEndData
	, const TArray<int32>& InPassivityIds)
	: TGsState(EItemCollectionState::Default), _collectionId(InCollectionId), _name(InName)
	, _timeOfValidity(ParseDateTime(InStartDate, InEndData)), _bookMark(false)
{
	if (INVALID_COLLECTION_ID == _collectionId)
	{
		GSLOG(Error, TEXT("_collectionId == INVALID_COLLECTION_ID"));
		return;
	}

	_globalSlotState = EItemCollectionSlotState::Impossible;

	_cachedTrimVaildNames.Empty();

	BuildStatInfo(InPassivityIds);

	_volatilityRedDot = false;
}
//
//FGsItemCollectionBase::FGsItemCollectionBase(const CollectionId InCollectionId
//	, const FText& InName
//	, const FString& InStartDate
//	, const FString& InEndData
//	, const TArray<int32>& InPassivityIds
//	, const SlotComponentDataList& InCollectionSlotList)
//	: TGsState(EItemCollectionState::Default), _collectionId(InCollectionId), _name(InName)
//	, _timeOfValidity(ParseDateTime(InStartDate, InEndData)), _bookMark(false)
//{
//	if (INVALID_COLLECTION_ID == _collectionId)
//	{
//		GSLOG(Error, TEXT("_collectionId == INVALID_COLLECTION_ID"));
//		return;
//	}
//
//	_globalSlotState = EItemCollectionSlotState::Impossible;
//
//	_cachedTrimVaildNames.Empty();
//
//	BuildStatInfo(InPassivityIds);
//
//	CreateCollectionInnerSlotItems(InCollectionSlotList);
//
//	_volatilityRedDot = false;
//}

void FGsItemCollectionBase::CreateCollectionInnerSlotItems(const SlotComponentDataList& InSlotComponentDataList)
{
	_innerSlots.Empty();
	
	// 2021/09/17 PKT - Build Collection Item
	for (const ParserSlotComponentData& componentData : InSlotComponentDataList)
	{
		if (INVALID_COLLECTION_CONDITION_ID == componentData.collectionConditionId)
		{
			GSLOG(Warning, TEXT("INVALID_COLLECTION_CONDITION_ID == componentData.collectionConditionId"));
			continue;
		}

		FGsItemCollectionSlotState& CollectionSlotStateRef = 
			_innerSlots.Emplace_GetRef(FGsItemCollectionSlotState(componentData.collectionConditionId, componentData.materialItemSet, componentData.rewardId));

		CollectionSlotStateRef.OnCollectionInnnerSlotChangeState.BindRaw(this, &FGsItemCollectionBase::UpdateByState);
	}
}

void FGsItemCollectionBase::BuildStatInfo(const TArray<int32>& InPassivityIds)
{
	const UGsTableManager& TableManager = UGsTableManager::GetInstance();
	const UGsTablePassivitySet* PassivitySetTable =
		Cast<UGsTablePassivitySet>(TableManager.GetTable(TBaseStructure<FGsSchemaPassivitySet>().Get()));

	if (nullptr == PassivitySetTable)
	{
		GSLOG(Error, TEXT("nullptr == PassivitySetTable"));
		return;
	}

	TArray<const FGsSchemaPassivitySet*> passivityList;

	for (const int32 PassivityIdIt : InPassivityIds)
	{
		if (INVALID_PASSIVITY_ID == PassivityIdIt)
		{
			continue;
		}

		const FGsSchemaPassivitySet* PassivitySet = nullptr;
		if (!PassivitySetTable->FindRowById(PassivityIdIt, PassivitySet) || nullptr == PassivitySet)
		{
			GSLOG(Error, TEXT("!PassivitySetTable->FindRowById(PassivityIdIt, PassivitySet) || nullptr == PassivitySet, PassivityIdIt : %d"), PassivityIdIt);
			continue;
		}

		passivityList.Emplace(PassivitySet);
	}

	MakeStatInfoAndEffectText(passivityList);
}

void FGsItemCollectionBase::MakeTimeOfValidity(const FString& InBeginDate, const FString& InEndDate)
{
	_timeOfValidity = ParseDateTime(InBeginDate, InEndDate);
}


void FGsItemCollectionBase::UpdateByState(ECollectionStateChangeType InChangeType)
{
	if (ECollectionStateChangeType::EBookMark == InChangeType)
	{	// 2021/10/14 PKT - BookMark 변경을 알린다.
		OnCollectionStateChange.ExecuteIfBound(ECollectionStateChangeType::EBookMark, this, this->GetType(), this->GetGlobalSlotState());
		return;
	}	
	else if ( ECollectionStateChangeType::EActive == InChangeType || ECollectionStateChangeType::ECollectionSlot == InChangeType )
	{
		// 2021/09/28 PKT - 이전 상태 저장
		EItemCollectionState preItemCollectionState = GetType();
		EItemCollectionSlotState prevGlobalSlotState = _globalSlotState;

		// 2022/01/06 PKT - 초기화
		_globalSlotState = EItemCollectionSlotState::Impossible;

		int32 completeCount = 0;
		for (const FGsItemCollectionSlotState& innerSlot : _innerSlots)
		{
			if (EItemCollectionSlotState::Completed == innerSlot.GetType())
			{	// 2021/09/23 PKT - 완료한 카운트
				++completeCount;
			}

			if (_globalSlotState > innerSlot.GetType())
			{
				_globalSlotState = innerSlot.GetType();
			}
		}

		EItemCollectionState nextCollectionState = EItemCollectionState::Opened;
		if (true == IsActive())
		{	// 2021/09/23 PKT - 활성화 상태일때만 상태를 업데이트 할 수 있다.

			if (0 < completeCount)
			{	// 2021/09/23 PKT - 하나라도 완료 상태라면 진행 중..
				nextCollectionState = EItemCollectionState::Progressing;
			}

			if (0 < _innerSlots.Num() && completeCount == _innerSlots.Num())
			{	// 2021/09/23 PKT - 전체 완료 했다면..
				nextCollectionState = EItemCollectionState::Completed;
				_globalSlotState = EItemCollectionSlotState::Completed;
			}
		}
		
		SetType(nextCollectionState);
		OnCollectionStateChange.ExecuteIfBound(InChangeType, this, preItemCollectionState, prevGlobalSlotState);
	}
}


void FGsItemCollectionBase::OnBookMarkToggle() const
{
	// 2021/10/15 PKT - FGsItemCollectionBase를 관리하는 Object에 Toggle를 알림.
	OnCollectionBookMarkToggle.ExecuteIfBound(this);
}


void FGsItemCollectionBase::UpdateOverrallSlotState()
{
	if (EItemCollectionState::Completed == GetType())
	{
		_globalSlotState = EItemCollectionSlotState::Completed;
		return;
	}

	_globalSlotState = EItemCollectionSlotState::Impossible;

	for (const FGsItemCollectionSlotState& innerSlot : _innerSlots)
	{
		if (_globalSlotState > innerSlot.GetType())
		{
			_globalSlotState = innerSlot.GetType();
		}
	}
}


void FGsItemCollectionBase::ActiveVolatilityRedDot()
{
	_volatilityRedDot = true;
}


void FGsItemCollectionBase::SlotRemoveAll()
{
	_innerSlots.Empty();
	_globalSlotState = EItemCollectionSlotState::Impossible;
}


void FGsItemCollectionBase::ForceToReset(bool InIsBookNMakrReset /*= true*/)
{
	for (FGsItemCollectionSlotState& innerSlot : _innerSlots)
	{
		innerSlot.Reset();
	}

	_volatilityRedDot = false;

	if (true == InIsBookNMakrReset)
	{
		_bookMark = false;
	}
}

void FGsItemCollectionBase::ForceToCompleted()
{
	for (FGsItemCollectionSlotState& innerSlot : _innerSlots)
	{
		innerSlot.UpdateByCompleted();
	}
}

void FGsItemCollectionBase::SetUnableToRegister(bool InFlag)
{
	bool prevFlag = _isUnableToRegister;
	_isUnableToRegister = InFlag;
	
	if (prevFlag != _isUnableToRegister)
	{
		UpdateByState(ECollectionStateChangeType::ECollectionSlot);
	}	
}

void FGsItemCollectionBase::UpdateByNewItem(const FGsItem* InItem)
{
	if (EItemCollectionState::Completed == GetType())
	{	// 2021/09/17 PKT - 완료 상태라면 굳이..
		return;
	}

	for (FGsItemCollectionSlotState& innerSlot : _innerSlots)
	{
		innerSlot.UpdateByNewItem(InItem);
	}
}

void FGsItemCollectionBase::UpdateByUpdateItem(const FGsItem* InItem)
{
	if (EItemCollectionState::Completed == GetType())
	{	// 2021/09/17 PKT - 완료 상태라면 굳이..
		return;
	}

	for (FGsItemCollectionSlotState& innerSlot : _innerSlots)
	{
		innerSlot.UpdateByUpdateItem(InItem);
	}
}

void FGsItemCollectionBase::UpdateByRemoveItem(ItemId InItemId, ItemDBId InItemDBId)
{
	if (EItemCollectionState::Completed == GetType())
	{	// 2021/09/17 PKT - 완료 상태라면 굳이..
		return;
	}

	for (FGsItemCollectionSlotState& innerSlot : _innerSlots)
	{
		innerSlot.UpdateByRemoveItem(InItemId, InItemDBId);
	}
}

void FGsItemCollectionBase::UpdateByBookMarkFlag( bool InFlag )
{
	_bookMark = InFlag;

	UpdateByState(ECollectionStateChangeType::EBookMark);
}

void FGsItemCollectionBase::RegisterCollection(CollectionConditionId InCollectionConditionId)
{
	for ( FGsItemCollectionSlotState& innerSlot : _innerSlots )
	{
		if (innerSlot.GetItemCollectionConditionId() == InCollectionConditionId)
		{	// 2021/09/30 PKT - 완료 처리
			innerSlot.UpdateByCompleted();
		}
	}
}

CollectionId FGsItemCollectionBase::GetCollectionId() const
{
	return _collectionId;
}

const FText& FGsItemCollectionBase::GetName() const
{
	return _name;
}

const FText& FGsItemCollectionBase::GetStatEffectText() const
{
	return _cachedStatEffectName;
}

bool FGsItemCollectionBase::IsValidPeriod(const FDateTime& _dateTimeUtc) const
{
	// 2021/09/30 PKT - 유효 기간을 갖고 있지 않다면 언제나 유효
	return (_timeOfValidity.IsEmpty()) ? true : _timeOfValidity.Contains(_dateTimeUtc);
}

FText FGsItemCollectionBase::GetPeriodText() const
{
	if (_timeOfValidity.IsEmpty())
	{	// 2021/12/19 PKT - 유효 기간을 갖고 있지 않음.
		return FText::GetEmpty();
	}

	const FDateTime& upperDate = _timeOfValidity.GetUpperBoundValue();
	FString textUpper = FString::Printf(TEXT("%d/%02d/%02d %02d:%02d")
		, upperDate.GetYear(), upperDate.GetMonth(), upperDate.GetDay(), upperDate.GetHour(), upperDate.GetMinute());

	// 2022/01/27 PKT - UTC YY/MM/DD HH:MM ~ YY/MM/DD HH:MM (24H)
	return FText::FromString(FString::Format(TEXT("~ UTC {0}"), { *textUpper }));
}

const TArray<StatInfo>& FGsItemCollectionBase::GetAllStatInfos() const
{
	return _statInfos;
}

const TArray<PassivityId>& FGsItemCollectionBase::GetPassivityId() const
{
	return _passivityIds;
}

EItemCollectionSlotState FGsItemCollectionBase::GetGlobalSlotState() const
{
	return _globalSlotState;
}

int32 FGsItemCollectionBase::GetCollectionItemsCount() const
{
	return _innerSlots.Num();
}

const FGsItemCollectionSlotState* FGsItemCollectionBase::FindInnerSlotByIndex(int32 InIndex) const
{
	if (InIndex <= INDEX_NONE || InIndex >= _innerSlots.Num())
	{
		GSLOG(Warning, TEXT("InIndex <= INDEX_NONE || InIndex >= _innerSlots.Num()"));
		return nullptr;
	}

	return &(_innerSlots[InIndex]);
}

const FGsItemCollectionSlotState* FGsItemCollectionBase::FindInnerSlotByCollectionConditionId(CollectionConditionId InId) const
{
	return _innerSlots.FindByPredicate([InId](const FGsItemCollectionSlotState& innerSlot)
		{
			return (innerSlot.GetItemCollectionConditionId() == InId);
		}
	);
}


int32 FGsItemCollectionBase::FindInnerSlotIndexByCollectionConditionId(CollectionConditionId InId) const
{
	return _innerSlots.IndexOfByPredicate([InId](const FGsItemCollectionSlotState& innerSlot)
		{
			return (innerSlot.GetItemCollectionConditionId() == InId);
		}
	);
}

bool FGsItemCollectionBase::IsItemAvailableItem(const FGsItem* InItem) const
{
	if (nullptr == InItem || true == InItem->GetLock() || true == InItem->IsEquipped() || InItem->IsPresetEquipped())
	{	// 2021/09/27 PKT - 아이템 정보가 없거나, 잠김 or 장착 중인 상태는 패스..
		return false;
	}

	if (false == IsActive() || EItemCollectionState::Completed == GetType())
	{	// 2021/09/27 PKT - 현재 활성화 상태가 아니라면 or 완료 상태라면 패스..
		return false;
	}

	const FDateTime& serverNowDateTimeUTC = FGsTimeSyncUtil::GetServerNowDateTimeUTC();
	if (false == IsValidPeriod(serverNowDateTimeUTC))
	{	// 2021/09/30 PKT - 유효기간 검사
		return false;
	}

	for (const FGsItemCollectionSlotState& innerSlot : _innerSlots)
	{
		if (true == innerSlot.IsMaterial(InItem))
		{
			return true;
		}
	}

	return false;
}

bool FGsItemCollectionBase::IsItemCanCollectItem(const FGsItem* InItem) const
{
	if (nullptr == InItem || true == InItem->GetLock() || true == InItem->IsEquipped() || InItem->IsPresetEquipped())
	{	// 2021/09/27 PKT - 아이템 정보가 없거나, 잠김 or 장착 중인 상태는 패스..
		return false;
	}

	if (false == IsActive() || EItemCollectionState::Completed == GetType())
	{	// 2021/09/27 PKT - 현재 활성화 상태가 아니라면 or 완료 상태라면 패스..
		return false;
	}

	const FDateTime& serverNowDateTimeUTC = FGsTimeSyncUtil::GetServerNowDateTimeUTC();
	if (false == IsValidPeriod(serverNowDateTimeUTC))
	{	// 2021/09/30 PKT - 유효기간 검사
		return false;
	}

	for (const FGsItemCollectionSlotState& innerSlot : _innerSlots)
	{
		if (true == innerSlot.IsCanCollect(InItem))
		{
			return true;
		}
	}

	return false;
}

bool FGsItemCollectionBase::IsOnBookMark() const
{
	return (true == _bookMark);
}


bool FGsItemCollectionBase::IsRedDot() const
{
	do 
	{
		if (false == IsActive() || EItemCollectionState::Completed == GetType())
		{	// 2021/09/23 PKT - 비활성화 혹은 완료 상태라면 레드닷 대응 안함.
			break;
		}

		if (true == _volatilityRedDot)
		{
			return true;
		}

		for (const FGsItemCollectionSlotState& innerSlot : _innerSlots)
		{	// 2021/09/23 PKT - 즉시 등록 가능 할 경우에만 레드닷
			if (EItemCollectionSlotState::ImmediatePossible == innerSlot.GetType())
			{
				return true;
			}
		}

	} while (0);

	return false;
}


void FGsItemCollectionBase::VolatilityRedDotOff()
{
	_volatilityRedDot = false;
}


bool FGsItemCollectionBase::IsUnableToRegister() const
{
	return _isUnableToRegister;
}


bool FGsItemCollectionBase::IsHighlightBackGround() const
{
	return false;
}


bool FGsItemCollectionBase::TitleAndCollectionItemNameContains(const FString& InSerachText) const
{
	if (_cachedTrimVaildNames.IsEmpty())
	{
		TArray<FText> availdName;
		// 2021/10/05 PKT - title
		availdName.Emplace(GetName());

		for (const FGsItemCollectionSlotState& innerSlot : _innerSlots)
		{
			availdName.Emplace(innerSlot.GetMaterialItemNames());
		}

		_cachedTrimVaildNames = FText::Join(FText::FromString(TEXT("\n")), availdName).ToString();

		_cachedTrimVaildNames.ReplaceInline(TEXT(" "), TEXT(""));
	}

	return _cachedTrimVaildNames.Contains(InSerachText);
}


FDateRange FGsItemCollectionBase::ParseDateTime(const FString& InStartDate, const FString& InEndData) const
{
	FDateRange outReturn = FDateRange(0, 0);

	do 
	{
		// 2021/09/16 PKT - 유효 기간 정보 없음.
		if (InStartDate.IsEmpty() || InEndData.IsEmpty())
		{
			break;
		}

		FString startDate = InStartDate;
		startDate.ReplaceInline(TEXT(" "), TEXT("T"));
				
		FDateTime LowerDateTimeUtc;
		if (false == FDateTime::ParseIso8601(*startDate, LowerDateTimeUtc))
		{
			GSLOG(Error, TEXT("false == FDateTime::ParseIso8601(*InStartDate, LowerDateTimeUtc)[%d]"), *startDate);
			break;
		}

		FString endDate = InEndData;
		endDate.ReplaceInline(TEXT(" "), TEXT("T"));
		FDateTime UpperDateTimeUtc;
		if (false == FDateTime::ParseIso8601(*endDate, UpperDateTimeUtc))
		{
			GSLOG(Error, TEXT("false == FDateTime::ParseIso8601(*InEndData, UpperDateTimeUtc), InEndData:%s"), *endDate);
			break;
		}

		outReturn = FDateRange(LowerDateTimeUtc, UpperDateTimeUtc);

	} while (0);

	return outReturn;
}


void FGsItemCollectionBase::MakeStatInfoAndEffectText(const TArray<const FGsSchemaPassivitySet*>& InPassivityList)
{
	_passivityIds.Empty();
	_statInfos.Empty();

	FString AllStatEffectString;

	for (const FGsSchemaPassivitySet* passivity : InPassivityList)
	{	
		if (nullptr == passivity || INVALID_PASSIVITY_ID == passivity->id)
		{
			GSLOG(Error, TEXT("nullptr == passivity || INVALID_PASSIVITY_ID == passivity->id"));
			continue;
		}

		// 2021/09/23 PKT - Id
		_passivityIds.Emplace(passivity->id);

		/**
		 * Stat Info
		 */
		FGsEffectStringHelper::FGsEffectAddData EffectAddData;
		if (!FGsEffectStringHelper::AddPassivityEffect(passivity, EffectAddData))
		{
			if (0 < passivity->effectInfoList.Num())
			{
				int effectId = passivity->effectInfoList[0].effectId;
				GSLOG(Error, TEXT("!FGsEffectStringHelper::AddPassivityEffect(PassivitySet, EffectAddData), id:%d, type:%d, effectId:%d")
					, passivity->id, passivity->type, effectId);
			}

			continue;
		}

		for (auto& StatTypeAndValue : EffectAddData._statMap)
		{	// 2021/09/23 PKT - stat info
			_statInfos.Emplace(StatTypeAndValue.Key, StatTypeAndValue.Value);
		}
		
		/**
		 * Effect Text
		 */
		FString StatEffectString;

		if (!FGsEffectStringHelper::GetPassivityEffectString(passivity->id, StatEffectString))
		{				
			GSLOG(Error, TEXT("!FGsEffectStringHelper::GetPassivityEffectString(%d, StatEffectString)"), passivity->id);
			continue;
		}
		
		AllStatEffectString.Append(StatEffectString);
	}

	// 2021/09/23 PKT - 효과 이름
	_cachedStatEffectName = FText::FromString(AllStatEffectString);
}