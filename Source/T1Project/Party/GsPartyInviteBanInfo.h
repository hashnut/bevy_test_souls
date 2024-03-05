#pragma once

#include "CoreMinimal.h"

// 파티 초대 차단 정보
// 60초 안에 3회 초대시 차단 당함(15초 동안)
class FGsPartyInviteBanInfo
{
private:
	// 이름
	FString _name;
	// 초대한 시간 배열	
	TArray<int64> _arrInviteTime;

	// 차단인지
	bool _isBan = false;
	// 차단 시작 시간
	int64 _banStartTime = 0;

	// 생성자
public:
	FGsPartyInviteBanInfo(const FString& In_name) :_name(In_name)
	{
		AddInviteTime();
	}

	// 로직 함수
public:

	// 갱신
	bool Update();
	// 초대 시간 추가
	void AddInviteTime();

	// get
public:
	// 차단 대상인지
	bool GetIsBan()
	{
		return _isBan;
	}
};