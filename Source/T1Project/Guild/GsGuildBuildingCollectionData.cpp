// Fill out your copyright notice in the Description page of Project Settings.


#include "GsGuildBuildingCollectionData.h"
#include "Guild/GsGuildBuildingData.h"
#include "Shared/Client/SharedEnums/SharedGuildEnum.h"
#include "DataSchema/Guild/GsSchemaGuildBuilding.h"
#include "UTIL/GsTimeSyncUtil.h"
#include "T1Project.h"


void FGsGuildBuildingCollectionData::Empty()
{
	_dataList.Empty();
	_collectionList.Empty();
	_buffReactivateTime = 0;
}

int32 FGsGuildBuildingCollectionData::GetPassivityCount() const
{
	return _collectionList.Num();
}

PassivityId FGsGuildBuildingCollectionData::GetPassivityId(int32 InIndex) const
{
	if (_collectionList.IsValidIndex(InIndex))
	{
		return _collectionList[InIndex];
	}

	return INVALID_PASSIVITY_ID;
}

void FGsGuildBuildingCollectionData::SetBuildingData(const GuildBuildingData& InData)
{
	FGsGuildBuildingData* data = GetBuildingDataInter(InData.mBuildingId);
	if (data)
	{
		RemoveCollection(data);

		// 내용 갱신
		data->Set(InData);
	}
	else
	{
		FGsGuildBuildingData newData;
		newData.Set(InData);
		data = &_dataList.Add_GetRef(newData);
	}

	AddCollection(data);
}

void FGsGuildBuildingCollectionData::SetBuffReactivateTime(float InRemainSecond)
{
#if WITH_EDITOR
	GSLOG(Warning, TEXT("[Guild] Buff Reactive remainSecond: %f"), InRemainSecond);
#endif

	_buffReactivateTime = FGsTimeSyncUtil::GetServerNowSecond() + FMath::FloorToInt(InRemainSecond);
}

void FGsGuildBuildingCollectionData::SetUpgradeLevel(GuildBuildingId InId, GuildBuildingLevel InNextLevel)
{
	if (FGsGuildBuildingData* data = GetBuildingDataInter(InId))
	{
		RemoveCollection(data);

		data->SetUpgradeLevel(InNextLevel);

		AddCollection(data);
	}
}

void FGsGuildBuildingCollectionData::SetActiveGuildBuff(GuildBuildingId InId, PassivityId InPassivityId, 
	float InCoolDownTime)
{
	if (FGsGuildBuildingData* data = GetBuildingDataInter(InId))
	{
		data->SetActiveGuildBuff(InPassivityId, InCoolDownTime);
	}
}

bool FGsGuildBuildingCollectionData::IsExistActiveBuff() const
{
	for (const FGsGuildBuildingData& iter : _dataList)
	{
		if (iter.IsBuffActive())
		{
			return true;
		}
	}

	return false;
}

const FGsGuildBuildingData* FGsGuildBuildingCollectionData::GetBuildingData(GuildBuildingId InId) const
{
	for (const FGsGuildBuildingData& iter : _dataList)
	{
		if (InId == iter._id)
		{
			return &iter;
		}
	}

	return nullptr;
}

FGsGuildBuildingData* FGsGuildBuildingCollectionData::GetBuildingDataInter(GuildBuildingId InId)
{
	for (FGsGuildBuildingData& iter : _dataList)
	{
		if (InId == iter._id)
		{
			return &iter;
		}
	}

	return nullptr;
}

void FGsGuildBuildingCollectionData::RemoveCollection(FGsGuildBuildingData* InData)
{
	if (InData->_tableData &&
		GuildStatType::GUILD_PASSIVE_STAT == InData->_tableData->guildStatType)
	{
		for (int32 i = 0; i < InData->GetPassivityCount(); ++i)
		{
			_collectionList.Remove(InData->GetPassivityId(i));
		}
	}
}

void FGsGuildBuildingCollectionData::AddCollection(FGsGuildBuildingData* InData)
{
	// 처음 건설중일 때는 패시비티 없음
	if (GuildBuildingStateType::GUILD_BUILDING_STATE_OPEN != InData->_buildingState)
	{
		// 건설하지 않은 건물 정보는 서버에서 오지 않으므로, 타입 체크만 진행
		if (InData->_tableData &&
			GuildStatType::GUILD_PASSIVE_STAT == InData->_tableData->guildStatType)
		{
			for (int32 i = 0; i < InData->GetPassivityCount(); ++i)
			{
				_collectionList.AddUnique(InData->GetPassivityId(i));
			}
		}
	}
}
