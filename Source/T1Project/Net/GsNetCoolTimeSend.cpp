#include "GSNetCooltimeSend.h"

void FGsNetCoolTimeSend::Clear()
{
	_coolTimeList.Empty();
}

void FGsNetCoolTimeSend::Update(float fDeltaTime)
{
	for (FNetCoolTime& itr : _coolTimeList)
	{
		itr.coolTime -= fDeltaTime;
	}

	_coolTimeList.RemoveAll([](const FNetCoolTime& elem)->bool
		{
			return (elem.coolTime <= 0) ? true : false;
		}
	);

}

void FGsNetCoolTimeSend::AddCoolTime(uint16 id, float defaultCoolTime)
{
	// 동일한 id를 넣어도 그냥 쌓고 지울때 한번에 지운다.
	// 쿨타임 리셋효과
	_coolTimeList.Add(FNetCoolTime(id, defaultCoolTime));
}

void FGsNetCoolTimeSend::RemoveCoolTime(uint16 id)
{
	_coolTimeList.RemoveAll([id](const FNetCoolTime& elem)->bool
		{
			return (elem.id == id) ? true : false;
		}
	);
}

bool FGsNetCoolTimeSend::IsCoolTime(uint16 id)
{
	int index =	_coolTimeList.FindLastByPredicate([id](const FNetCoolTime& elem)->bool
		{
			return (elem.id == id) ? true : false;
		}
	);
	return (INDEX_NONE != index) ? true : false;
}

float FGsNetCoolTimeSend::GetCoolTimeLast(uint16 id)
{
	int index = _coolTimeList.FindLastByPredicate([id](const FNetCoolTime& elem)->bool
		{
			return (elem.id == id) ? true : false;
		}
	);
	return (INDEX_NONE != index) ? _coolTimeList[index].coolTime : -1.0f;
}
