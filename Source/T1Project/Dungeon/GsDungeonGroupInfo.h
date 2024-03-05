

#pragma once


#include "CoreMinimal.h"
#include "../UTIL/GsTableUtil.h"
#include "UTIL/GsTimeSyncUtil.h"
#include "Dungeon/GsSchemaDungeonGroupData.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"

#include "Shared/Shared/SharedPackets/PD_Client_World_Bypass.h"

struct FTimespan;
struct FGsTimeSyncUtil;
/*
	던전 그룹 정보
*/
class FGsDungeonGroupInfo
{
protected:
		
	const FGsSchemaDungeonGroupData* _dungeonGroupData;

	int _myBaseTime;
	int _myAddTime;
	FDateTime _UTCTimeEnd;

public:
	FGsDungeonGroupInfo() = default;
	virtual ~FGsDungeonGroupInfo() = default;

	explicit FGsDungeonGroupInfo(const FGsSchemaDungeonGroupData* InGroupData, int InMyBaseTime, int InMyAddTime)
		: _dungeonGroupData(InGroupData),
		_myBaseTime(InMyBaseTime),
		_myAddTime(InMyAddTime)
	{		
		UpdateTime();
	}

 public:
	 virtual void Clear() {/**/ }
	// getter
 	const DungeonGroupId GetDungeonGroupId() const { return _dungeonGroupData->id; }
	const int GetListOrder() const { return _dungeonGroupData->listOrder; }
	const int GetMyBaseTime() const { return _myBaseTime; }
	const int GetMyAddTime() const { return _myAddTime; }
	const FGsSchemaDungeonGroupData* GetDungeonGroupData() const { return _dungeonGroupData; }
	const FDateTime& GetTime() { return _UTCTimeEnd; }	

	// setter
	void SetMyBaseTime(int inValue) { _myBaseTime = inValue; }
	void SetMyAddTime(int inValue) { _myAddTime = inValue; }
	void SetUTCTimeEnd(const FDateTime& InElapsedTime) { _UTCTimeEnd = InElapsedTime; }
	void UpdateTime() { _UTCTimeEnd = FGsTimeSyncUtil::GetServerNowDateTime() + FTimespan::FromSeconds(_myBaseTime + _myAddTime); }

	bool IsTimeLimit() { return (-1 == _dungeonGroupData->baseTimeLimit) ? false : true; }
	bool IsTimeOver() { return (0 < _myBaseTime + _myAddTime) ? false : true; }
};