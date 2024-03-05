#pragma once

struct FNetCoolTime
{
	FNetCoolTime(uint16 inId, float inCoolTime) : id(inId), coolTime(inCoolTime)	{}

	uint16 id;
	float coolTime;
};

// 주의 !!
// 전송후 패킷을 받았을때 Remove처리 필수
// 쿨타임을 제거하려고 할때 전송패킷과 수신 패킷의 구조 (Pair, 단방향, Kick)의 구조를 이해하고 적용해야
// 같은 패킷 재전송을 못하는 상황을 방지할수 있습니다.


class FGsNetCoolTimeSend
{
	TArray<FNetCoolTime>	_coolTimeList;
public:
	// 재연결등의 이슈일때는 전체비움 처리
	void Clear();
	// 시간 체크후 완료시
	void Update(float fDeltaTime);

public:
	// 서버와 보내고 받는 패킷ID가 다르기에 보내는 패킷기준으로 활용
	void AddCoolTime(uint16 id, float defaultCoolTime = 0.5f);
	// 제거
	void RemoveCoolTime(uint16 id);
	// 쿨타임 중인지 검사
	bool IsCoolTime(uint16 id);
	// 잔여 쿨타임 확인
	float GetCoolTimeLast(uint16 id);
};

