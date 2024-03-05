// Fill out your copyright notice in the Description page of Project Settings.


#include "GsGuildBuildingData.h"
#include "Guild/GsGuildData.h"
#include "DataSchema/Guild/GsSchemaGuildBuilding.h"
#include "DataSchema/Guild/GsSchemaGuildBuildingLevelData.h"
#include "DataSchema/Skill/Passivity/GsSchemaPassivitySet.h"
#include "DataSchema/Abnormality/GsSchemaAbnormalitySet.h"
#include "Guild/GsGuildHelper.h"
#include "GameObject/Abnormality/GsPassivityHelper.h"
#include "UTIL/GsTimeSyncUtil.h"
#include "T1Project.h"


void FGsGuildBuildingData::Set(const GuildBuildingData& InData)
{
	_id = InData.mBuildingId;
	
	// 주의: 오픈/업그레이드 시 이미 다음 레벨로 온다. GetCurrBuildingLevel 을 통해 쓰도록 한다
	_level = InData.mBuildingLevel;
	_buildingState = InData.mBuildingStateType;
	_lastLevelUpTime = InData.mLastLevelUpTime;

	_upgradeFinishTime = InData.mEndTime;
	_tableData = FGsGuildHelper::GetGuildBuilding(_id);

	_passivityList.Empty();
	for (int i = 0; i < InData.mGuildPassivityCount; ++i)
	{
		FGsGuildPassivityData buffData;
		buffData._id = InData.mGuildPassivityArray[i].mPassivityId;
		buffData._buffEndTime = InData.mGuildPassivityArray[i].mEndTime;

		_passivityList.Emplace(buffData);

//#if WITH_EDITOR
//		FDateTime testBuff = FGsTimeSyncUtil::ConvertServerTimeToUTCDateTime(buffData._buffEndTime);
//		FTimespan remain = testBuff - FGsTimeSyncUtil::GetServerNowDateTimeUTC();
//
//		GSLOG(Log, TEXT("[Guild] BuildingId: %d, PassivityId: %d, EndUTC: %s, Remain: %s"),
//			_id, buffData._id, *testBuff.ToString(), *remain.ToString());
//#endif
	}

#if 0
#if WITH_EDITOR
	FDateTime testLevelUpTime = FGsTimeSyncUtil::ConvertServerTimeToClientDateTime(_lastLevelUpTime);
	GSLOG(Log, TEXT("[REDDOT] Building id: %d, Notice: %s"), _id, *(testLevelUpTime.ToString()));
#endif
#endif
}

void FGsGuildBuildingData::SetUpgradeLevel(GuildBuildingLevel InNextLevel)
{
	_buildingState = GuildBuildingStateType::GUILD_BUILDING_STATE_UPGRADE;

	const FGsSchemaGuildBuildingLevelData* levelData = FGsGuildHelper::GetBuildingLevelData(_tableData, InNextLevel);
	if (levelData)
	{
		_upgradeFinishTime = FGsTimeSyncUtil::GetServerNowSecond() + levelData->levelUpSecond;
	}
}

PassivityId FGsGuildBuildingData::GetPassivityId(int32 InIndex)
{
	if (_passivityList.IsValidIndex(InIndex))
	{
		return _passivityList[InIndex]._id;
	}

	// 길드버프는 버프 시간이 있을때만 _passivityList가 오므로 테이블에서 읽어서 준다
	if (_tableData &&
		GuildStatType::GUILD_BUFF == _tableData->guildStatType)
	{
		const FGsSchemaGuildBuildingLevelData* levelData = FGsGuildHelper::GetBuildingLevelData(_tableData, _level);
		if (levelData)
		{
			if (const FGsSchemaPassivitySet* passivitySet = levelData->passivityId.GetRow())
			{
				return passivitySet->id;
			}
		}
	}

	return INVALID_PASSIVITY_ID;
}

void FGsGuildBuildingData::SetActiveGuildBuff(PassivityId InPassivityId, float InCoolDownTime)
{
	bool bUpdateSuccess = false;
	for (FGsGuildPassivityData& iter : _passivityList)
	{
		if (InPassivityId == iter._id)
		{
			iter._buffEndTime = FGsTimeSyncUtil::GetServerNowSecond() + FMath::FloorToInt(InCoolDownTime);
			bUpdateSuccess = true;

//#if WITH_EDITOR
//			FDateTime testBuff = FGsTimeSyncUtil::ConvertServerTimeToUTCDateTime(iter._buffEndTime);
//			FTimespan remain = testBuff - FGsTimeSyncUtil::GetServerNowDateTimeUTC();
//
//			GSLOG(Log, TEXT("[Guild]ACK BuildingId: %d, PassivityId: %d, coolTime: %f, EndUTC: %s, Remain: %s"),
//				_id, iter._id, InCoolDownTime, *testBuff.ToString(), *remain.ToString());
//#endif
		}
	}

	if (false == bUpdateSuccess)
	{
		FGsGuildPassivityData newData;
		newData._id = InPassivityId;
		newData._buffEndTime = FGsTimeSyncUtil::GetServerNowSecond() + FMath::FloorToInt(InCoolDownTime);

		_passivityList.Emplace(newData);
	}
}

bool FGsGuildBuildingData::IsBuffActive() const
{
	time_t currServerTime = FGsTimeSyncUtil::GetServerNowSecond();

	for (const FGsGuildPassivityData& iter : _passivityList)
	{
		// 아직 종료시간을 지나지 않았다면
		if (currServerTime < iter._buffEndTime)
		{
			return true;
		}
	}

	return false;
}

bool FGsGuildBuildingData::IsBuildingOpened() const
{
	if (0 == GetCurrBuildingLevel() ||
		GuildBuildingStateType::GUILD_BUILDING_STATE_OPEN == _buildingState)
	{
		return false;
	}

	return true;
}

GuildBuildingLevel FGsGuildBuildingData::GetCurrBuildingLevel() const
{
	return _level;
	/*
	GuildBuildingLevel outLevel = _level;

	// 건설중/레벨업중 이므로 
	if (GuildBuildingStateType::GUILD_BUILDING_STATE_OPEN == _buildingState || 
		GuildBuildingStateType::GUILD_BUILDING_STATE_UPGRADE == _buildingState)
	{
		if (0 < outLevel)
		{
			outLevel -= 1;
		}
	}

	return outLevel;
	*/
}

float FGsGuildBuildingData::GetBuffRemainTime() const
{
	time_t currServerTime = FGsTimeSyncUtil::GetServerNowSecond();

	for (const FGsGuildPassivityData& iter : _passivityList)
	{
		time_t remainTime = iter._buffEndTime - currServerTime;
		if (0 < remainTime)
		{
			return static_cast<float>(remainTime);
		}
	}

	return 0.f;
}

float FGsGuildBuildingData::GetBuffDuration() const
{
	const FGsSchemaGuildBuildingLevelData* levelData = FGsGuildHelper::GetBuildingLevelData(_tableData, GetCurrBuildingLevel());
	if (levelData)
	{
		const FGsSchemaAbnormalitySet* abnormalitySet = FGsPassivityHelper::GetInvokeAbnormality(levelData->passivityId.GetRow());
		if (abnormalitySet)
		{
			return abnormalitySet->durationMin;
		}
	}

	return 0.f;
}