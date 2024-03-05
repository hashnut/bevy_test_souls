#pragma once
#include "CoreMinimal.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"

#include "UTIL/GsTimeSyncUtil.h"

/**
 *  ½Ã°øÀÇ Æ´»õ Æ÷Å»Á¤º¸
 */
class FGsSpaceCrackAlarmInfo
{
public:
	MapId _regionMapId;
	FenceId _fenceId;
	FVector _pos;	

public:	
	FGsSpaceCrackAlarmInfo(const SpaceCrackAlarm& inAlarm)
	{
		Initialize(inAlarm);
	}
	FGsSpaceCrackAlarmInfo(	MapId In_regionMapId
		, FenceId inFenceId
		, const FVector& In_pos)
		: _regionMapId(In_regionMapId), _fenceId(inFenceId), _pos(In_pos)
	{
	}

public:
	void Initialize(const SpaceCrackAlarm& inAlarm)
	{		
		_regionMapId = inAlarm.mMapId;
		_fenceId = inAlarm.mFenceId;
		_pos = FVector(inAlarm.mX, inAlarm.mY, inAlarm.mZ);	
	}
};

class FGsSpaceCrackActivationInfo
{
public:	
	MapId _mapId;
	FenceId _fenceId;
	// Áö¿ª ¸Ê id
	int _minTimeMinute;
	int _maxTimeMinute;
	FDateTime _lastActivateTime;
	bool _isLastActive = false;

public:
	FGsSpaceCrackActivationInfo(const SpaceCrackActivation& inActivate)
	{
		Initialize(inActivate);
	}

public:
	void Initialize(const SpaceCrackActivation& inActivate)
	{
		_mapId = inActivate.mMapId;
		_fenceId = inActivate.mFenceId;
		_minTimeMinute = inActivate.mActivationIntervalMin;
		_maxTimeMinute = inActivate.mActivationIntervalMax;
		_lastActivateTime = FGsTimeSyncUtil::ConvertServerTimeToClientDateTime(inActivate.mLastActivationTime);

		_isLastActive = (inActivate.mLastActivationTime == 0) ? false : true;
	}
};

