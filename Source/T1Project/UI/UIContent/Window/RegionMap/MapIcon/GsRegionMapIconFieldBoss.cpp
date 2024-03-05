#include "GsRegionMapIconFieldBoss.h"

#include "UI/UIContent/Window/RegionMap/MapIcon/GsUIIconRegionFieldBoss.h"

#include "UTIL/GsTimeSyncUtil.h"

#include "UI/UIContent/Helper/GsTimeStringHelper.h"



// 보스 상태 스위칭
void UGsRegionMapIconFieldBoss::SetBossState(EGsRegionFieldBossState In_bossState)
{
	_bossState = In_bossState;

	UGsUIIconRegionFieldBoss* fieldBossIcon = Cast<UGsUIIconRegionFieldBoss>(_icon);
	if (fieldBossIcon == nullptr)
	{
		return;
	}

	fieldBossIcon->SetBossState(In_bossState);
}
// 남은 시간 세팅
void UGsRegionMapIconFieldBoss::SetRemainTime(FText In_remainTime)
{
	UGsUIIconRegionFieldBoss* fieldBossIcon = Cast<UGsUIIconRegionFieldBoss>(_icon);
	if (fieldBossIcon == nullptr)
	{
		return;
	}

	fieldBossIcon->SetRemainTime(In_remainTime);
}

void UGsRegionMapIconFieldBoss::Tick(float In_delta)
{

	if (_bossState == EGsRegionFieldBossState::NotSpawn && _isHaveSpawnTime)
	{
		UpdateState();
	}
	UpdateRemainTime(false);
}


void UGsRegionMapIconFieldBoss::SetSpawnTime(bool In_isHaveSpawnTime, FDateTime In_time)
{
	_isHaveSpawnTime = In_isHaveSpawnTime;
	_spawnTime = In_time;
}
// 남은시간 갱신
void UGsRegionMapIconFieldBoss::UpdateRemainTime(bool In_isForced)
{
	if (_bossState != EGsRegionFieldBossState::SpawnReady)
	{
		return;
	}
	FTimespan remainDateTime =
		_spawnTime - FGsTimeSyncUtil::GetServerNowDateTimeUTC();

	float newTotalSecond = remainDateTime.GetTotalSeconds();

	if (newTotalSecond == _oldTotalSecond &&
		In_isForced == false)
	{
		return;
	}

	_oldTotalSecond = newTotalSecond;

	FString strRemainTime;
	FGsTimeStringHelper::GetTimeStringHMS(remainDateTime, strRemainTime);

	SetRemainTime(FText::FromString(strRemainTime));

}

// 상태 갱신
void UGsRegionMapIconFieldBoss::UpdateState(bool In_isForeced)
{
	EGsFieldBossState bossState = FGsFieldBossHandler::GetBossState(_isHaveSpawnTime ,_spawnTime);
	EGsRegionFieldBossState uiState = GetUIState(bossState);
	// 이전이랑 다르던가
	// 강제던가
	if (uiState != _bossState ||
		In_isForeced == true)
	{
		SetBossState(uiState);
	}

}

EGsRegionFieldBossState UGsRegionMapIconFieldBoss::GetUIState(EGsFieldBossState In_state)
{
	EGsRegionFieldBossState uiSwitchState = EGsRegionFieldBossState::Spawn;

	switch (In_state)
	{
	case EGsFieldBossState::Despawn:
		uiSwitchState = EGsRegionFieldBossState::NotSpawn;
		break;
	case EGsFieldBossState::Ready:
		uiSwitchState = EGsRegionFieldBossState::SpawnReady;
		break;
	case EGsFieldBossState::Spawn:
		uiSwitchState = EGsRegionFieldBossState::Spawn;
		break;
	}
	return uiSwitchState;
}

void UGsRegionMapIconFieldBoss::SetClickCallback(TFunction<void()> In_callback)
{
	UGsUIIconRegionFieldBoss* fieldBossIcon = Cast<UGsUIIconRegionFieldBoss>(_icon);
	if (fieldBossIcon == nullptr)
	{
		return;
	}

	fieldBossIcon->SetClickCallback(In_callback);
}