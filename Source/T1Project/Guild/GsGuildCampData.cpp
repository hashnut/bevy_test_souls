// Fill out your copyright notice in the Description page of Project Settings.


#include "GsGuildCampData.h"
#include "UTIL/GsTimeSyncUtil.h"
#include "T1Project.h"


void FGsGuildCampData::SetData(const GuildCampInfo& InData)
{
	_mapId = InData.mMapId;
	_campId = InData.mCampId;
	_campState = InData.mGuildCampStateType;
	_endTime = InData.mEndDateTime;
	_level = InData.mLevel;

#if WITH_EDITOR
	FDateTime endDateTime = FGsTimeSyncUtil::ConvertToDateTime(_endTime);
	FDateTime nowDateTime = FGsTimeSyncUtil::GetServerNowDateTime();
	GSLOG(Warning, TEXT("[CAMP] endTime: %s(%ld), serverNow: %s"), 
		*endDateTime.ToString(), _endTime, *nowDateTime.ToString());
#endif
}
