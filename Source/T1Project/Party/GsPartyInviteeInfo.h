#pragma once
#include "CoreMinimal.h"
#include "Shared/Client/SharedEnums/SharedCreatureEnum.h"
#include "Public/Shared/Shared/SharedInclude/SharedTypes.h"

#include "Party/GsPartySlotInfo.h"

#include "Data/GsDataContainManager.h"
#include "Data/GsGlobalConstant.h"

#include "Core/Public/Misc/DateTime.h"
/*
	파티 초대 받은 사람 정보(요청 보낸 사람이 가지고 있는 정보)
	파티 수락전 상태
*/
class FGsPartyInviteeInfo : public IGsPartySlotInfo
{
private:
	// 이름
	FString _name;
	// 슬롯 번호
	int _partySlotId;
	// 본토 월드 id
	WorldId _homeWorldId = 0;

	// 시작 시간
	int64 _startTimeTick;
	// 타이머 시작했는가
	bool _timmerStarted = false;
	
	// 생성자
public:
	FGsPartyInviteeInfo(const FString& In_name,int In_partySlotId, WorldId In_homeWorldId):
		_name(In_name), _partySlotId(In_partySlotId), _homeWorldId(In_homeWorldId)
	{
	}
	// 가상 함수
public:
	virtual const FString& GetName() override
	{
		return _name;
	}
	virtual bool GetIsMember() override
	{
		return false;
	}
	// 로직 함수
public:
	// 타이머 시작
	void StartTimer()
	{
		_startTimeTick = FDateTime::UtcNow().GetTicks();
		_timmerStarted = true;
	}
	// 갱신(시간다되면 클라에서삭제)
	// 원래는 서버에서 삭제해주는데
	// 대상이 앱종료해버리면 한참뒤에 수정되어서
	// 클라에서 삭제도 추가
	void Update()
	{
		if (_timmerStarted == false)
		{
			return;
		}

		int64 nowTick = FDateTime::UtcNow().GetTicks();
		int64 diffTick = nowTick - _startTimeTick;

		static float maxTimeTick = 
			GData()->GetGlobalData()->_partyInviteWaitSecond * ETimespan::TicksPerSecond;

		// 시간 종료
		if (diffTick >= maxTimeTick)
		{
			// 삭제 요청
			_timmerStarted = false;
		}
	}
	// get, set
public:
	int GetPartySlotId() { return _partySlotId; }
	void SetData(const FString& In_name,int In_partySlotId, WorldId In_homeWorldId)
	{
		_name = In_name;
		_partySlotId = In_partySlotId;
		_homeWorldId = In_homeWorldId;
	}
	// 시작 시간 가져오기
	int64 GetStartTimeTick()
	{
		return _startTimeTick;
	}
	// 타이머 시작되었는가
	bool GetIsTimmerStarted()
	{
		return _timmerStarted;
	}

	WorldId GetHomeWorldId()
	{
		return _homeWorldId;
	}
};