
#include "GsTitleCollectionReward.h"

#include "Shared/Client/SharedEnums/SharedPassivityEnum.h"

#include "Management/GsMessageHolder.h"

#include "Message/MessageParam/GsMessageParam.h"

#include "GsTableManager.h"
#include "DataSchema/Title/GsSchemaTitleCollectionReward.h"
#include "T1Project.h"


FGsTitleCollectionReward::FGsTitleCollectionReward() 
	: FGsTitleCollectionData(PassivityOwnerType::TITLE_COLLECTION), _currentAcquire(ZERO_ACQUIRE_COUNT)
{
	/**/
}


void FGsTitleCollectionReward::BuildCollectionReward()
{
	// 2022/01/13 PKT - Table Manager
	const UGsTableManager& tableManager = UGsTableManager::GetInstance();

	// 2022/01/13 PKT - Title Reward Table
	const UGsTableTitleCollectionReward* collectionTable
		= Cast<UGsTableTitleCollectionReward>(tableManager.GetTable(TBaseStructure<FGsSchemaTitleCollectionReward>::Get()));

	if (nullptr == collectionTable)
	{
		GSLOG(Error, TEXT("nullptr == collectionTable"));
		return;
	}

	const TMap<FName, uint8*>* RowMap = nullptr;

	if (false == collectionTable->GetRowMap(RowMap))
	{
		GSLOG(Error, TEXT("false == collectionTable->GetRowMap(RowMap)"));
		return;
	}

	// 2022/01/13 PKT - Size Reserve
	_collectionRewardList.Empty(RowMap->Num());

	// 2022/01/13 PKT - Add List
	for (auto& RowKvIt : *RowMap)
	{
		const FGsSchemaTitleCollectionReward* data = reinterpret_cast<const FGsSchemaTitleCollectionReward*>(RowKvIt.Value);
		if (nullptr == data)
		{
			GSLOG(Error, TEXT("nullptr == data"));
			return;
		}

		// 2022/01/13 PKT - Add..
		_collectionRewardList.Emplace(FGsTitleRewardData(data));
	}

	// 2022/01/13 PKT - Sort
	_collectionRewardList.Sort([](const FGsTitleRewardData& lhr, const FGsTitleRewardData& rhr)
		{
			return lhr.GetAcquisitionCount() < rhr.GetAcquisitionCount();
		}
	);
}


void FGsTitleCollectionReward::UpdateCollectionState(bool InReverse /* = false */, bool IsInitData /* = false */)
{
	CleanUp();

	const int32 maxCount = _collectionRewardList.Num();
	for (int32 i = 0; i < maxCount; ++i)
	{
		int32 index = (false == InReverse) ? i : (maxCount - 1) - i;
		if (false == _collectionRewardList.IsValidIndex(index))
		{
			GSLOG(Error, TEXT("false == _collectionRewardList.IsValidIndex(index) : maxCount [%d], index[%d]"), maxCount, index);
			return;
		}

		FGsTitleRewardData& data = _collectionRewardList[index];
		ETitleRewardState prevState = data.GetState();

		if (_currentAcquire < data.GetAcquisitionCount())
		{
			data.SetState(ETitleRewardState::Disable);
		}
		else
		{
			data.SetState(ETitleRewardState::Activate);

			for (PassivityId passivityId : data.GetPassivityIds())
			{
				FGsTitleCollectionData::Add(passivityId);
			}
		}

		// 2022/01/26 PKT - 초기화가 아닐때 상태가 달라 졌다면..EffectList 추가
		if (false == IsInitData && prevState != data.GetState())
		{
			_updateEffectList.Emplace(TPair<int32, ETitleRewardState>(data.GetId(), data.GetState()));
		}
	}

	HudCollectionUpdate();
}


void FGsTitleCollectionReward::SendRedDotHUD()
{
	bool isPrevRedDot = IsRedDot();

	SetRedDot(CheckRedDot());

	if (isPrevRedDot != IsRedDot())
	{
		// 2022/01/14 PKT - RedDot 변경 내용을 Hud에 알린다.
		GMessage()->GetHudRedDot().SendMessage(MessageContentHUDRedDot::TITLE, false);
	}
}


void FGsTitleCollectionReward::Initialized(int32 InAcquireCount, int32 maxAcquireCount)
{
	_updateEffectList.Empty();

	_currentAcquire = InAcquireCount;

	_maxAcquireCount = maxAcquireCount;

	// 2022/01/14 PKT - Copllector
	UpdateCollectionState(false, true);

	// 2022/01/14 PKT - RedDot Off
	SetRedDot(false);
}


void FGsTitleCollectionReward::Finalize()
{
	// 2022/01/14 PKT - collection Data
	_collectionRewardList.Empty();

	// 2022/01/14 PKT - 패시비티 정보 초기화
	CleanUp();

	_currentAcquire = ZERO_ACQUIRE_COUNT;

	// 2022/01/14 PKT - RedDot Off
	SetRedDot(false);
}


bool FGsTitleCollectionReward::CheckRedDot(bool bInUpdate /*= true*/)
{
	bool isRedDot = false;

	for (const auto& pair : _updateEffectList)
	{
		if (ETitleRewardState::Activate == pair.Value)
		{
			isRedDot = true;
			break;
		}
	}

	return isRedDot;
}


void FGsTitleCollectionReward::Confirm()
{
	_updateEffectList.Empty();

	// 2022/01/14 PKT - HUD Reddot 
	SendRedDotHUD();

	// 2022/01/14 PKT - Send To Window
	GMessage()->GetTitle().SendMessage(MessageContentTitle::UPDATE_COLLECTION, nullptr);
}


void FGsTitleCollectionReward::AddTitle()
{
	_currentAcquire = FMath::Min(_currentAcquire + 1, _maxAcquireCount);
	
	// 2022/01/13 PKT - passivity 재계산
	UpdateCollectionState(false);

	// 2022/01/14 PKT - HUD Reddot 
	SendRedDotHUD();

	// 2022/01/14 PKT - Send To Window
	GMessage()->GetTitle().SendMessage(MessageContentTitle::UPDATE_COLLECTION, nullptr);
}


void FGsTitleCollectionReward::RemoveTitle()
{
	_currentAcquire = FMath::Max(_currentAcquire - 1, 0);

	// 2022/01/13 PKT - passivity 재계산(뒤에부터 계산)
	UpdateCollectionState(true);

	// 2022/01/14 PKT - HUD Reddot 
	SendRedDotHUD();

	// 2022/01/14 PKT - Send To Window
	GMessage()->GetTitle().SendMessage(MessageContentTitle::UPDATE_COLLECTION, nullptr);
}


int32 FGsTitleCollectionReward::GetAcquire() const 
{
	return _currentAcquire;
}


int32 FGsTitleCollectionReward::GetMaxAcquire() const
{
	return _maxAcquireCount;
}


int32 FGsTitleCollectionReward::GetCollectionCount() const
{
	return _collectionRewardList.Num();
}

const TArray<TPair<int32, ETitleRewardState>>& FGsTitleCollectionReward::GetUpdateEffectList() const
{
	return _updateEffectList;
}


const FGsTitleRewardData* FGsTitleCollectionReward::FindRewardData(int32 InIndex) const
{
	return (_collectionRewardList.IsValidIndex(InIndex)) ? &(_collectionRewardList[InIndex]) : nullptr;
}


bool FGsTitleCollectionReward::FindPassivityIds(int32 InStep, TArray<PassivityId>& OutPassivityListst)
{
	OutPassivityListst.Empty();

	if (false == _collectionRewardList.IsValidIndex(InStep))
	{
		GSLOG(Error, TEXT("Invalid InStep [%d]"), InStep);
		return false;
	}
	
	OutPassivityListst.Append(_collectionRewardList[InStep].GetPassivityIds());
	return true;
}