#pragma once
#include "CoreMinimal.h"

#include "Core/Public/Misc/DateTime.h"

#include "Shared/Shared/SharedInclude/SharedTypes.h"

/*
	파티 초대한 사람 정보(요청 받은 사람이 가지고 있는 정보)
	파티 수락전 상태
*/

class FGsPartyInviterInfo
{
	// 멤버 변수
private:
	// 이름
	FString _name;
	// 레벨
	int _level;
	// 채널 id
	int _channelId;
	// 본토 world id
	WorldId _homeWorldId = 0;

	// 시작 시간
	int64 _startTimeTick;
	// 초대자 있는지
	bool _isInviterExist = false;

	// 로직 함수
public:	
	// 타이머 시작
	void StartTimer()
	{
		_startTimeTick = FDateTime::UtcNow().GetTicks();
	}
	void ClearInviter();
	
	// get, set
public:
	void SetData(const FString& In_name, int In_level,
		int In_channelId, WorldId In_homeWorldId)
	{
		_name = In_name;
		_level = In_level;
		_channelId = In_channelId;
		_homeWorldId = In_homeWorldId;

		_isInviterExist = true;
	}
	// 시작 시간 가져오기
	int64 GetStartTimeTick()
	{
		return _startTimeTick;
	}
	const FString& GetName()
	{
		return _name;
	}
	int GetLevel()
	{
		return _level;
	}

	bool GetIsInviterExist()
	{
		return _isInviterExist;
	}
	int GetChannelId()
	{
		return _channelId;
	}
	WorldId GetHomeWorldId()
	{
		return _homeWorldId;
	}
};