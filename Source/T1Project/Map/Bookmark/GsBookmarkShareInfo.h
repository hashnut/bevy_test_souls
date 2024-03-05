#pragma once
#include "CoreMinimal.h"

#include "Core/Public/Misc/DateTime.h"

/*
	위치 즐겨찾기 공유 정보
*/

class FGsBookmarkShareInfo
{
	// 멤버 변수
private:
	// 공유 정보
	// 공유자 이름
	FString _shareUserName;
	// 즐겨찾기 이름
	FString _shareBookmarkName;
	// 공유 맵 id
	int _shareMapId;
	// 공유 위치
	FVector _sharePos;
	// 채널 id
	int _channelId;

	// 시작 시간
	int64 _startTimeTick;
	// 공유 정보 있는지
	bool _isShareInfoExist = false;

	// 로직 함수
public:
	// 타이머 시작
	void StartTimer()
	{
		_startTimeTick = FDateTime::UtcNow().GetTicks();
	}
	void ClearShareInfo()
	{
		_isShareInfoExist = false;
	}

	// get, set
public:
	// 공유 정보 저장
	void SetShareInfo(const FString& In_userName, const FString& In_bookmarkName,
		int In_mapId, const FVector& In_pos,
		int In_channelId)
	{
		_shareUserName = In_userName;
		_shareBookmarkName = In_bookmarkName;
		_shareMapId = In_mapId;
		_sharePos = In_pos;
		_channelId = In_channelId;

		_isShareInfoExist = true;
	}
	// 시작 시간 가져오기
	int64 GetStartTimeTick()
	{
		return _startTimeTick;
	}
	const FString& GetUserName()
	{
		return _shareUserName;
	}
	const FString& GetBookmarkName()
	{
		return _shareBookmarkName;
	}
	int GetMapId()
	{
		return _shareMapId;
	}
	const FVector& GetPos()
	{
		return _sharePos;
	}
	int GetChannelId()
	{
		return _channelId;
	}
};