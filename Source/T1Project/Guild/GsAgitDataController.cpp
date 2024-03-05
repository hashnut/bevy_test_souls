// Fill out your copyright notice in the Description page of Project Settings.


#include "GsAgitDataController.h"
#include "Guild/GsAgitDungeonRaidData.h"
#include "UTIL/GsTimeSyncUtil.h"


FGsAgitDataController::~FGsAgitDataController()
{
	Clear();
}

void FGsAgitDataController::Clear()
{
	_rentalEndDate = 0;

	if (_dungeonRaidData)
	{
		delete _dungeonRaidData;
	}
	_dungeonRaidData = nullptr;
}

void FGsAgitDataController::ResetDungeon()
{
	if (_dungeonRaidData)
	{
		_dungeonRaidData->Reset();
	}
}

FGsAgitDungeonRaidData* FGsAgitDataController::GetRaidData()
{
	if (nullptr == _dungeonRaidData)
	{
		_dungeonRaidData = new FGsAgitDungeonRaidData();
		_dungeonRaidData->Initialize();
	}

	return _dungeonRaidData;
}

bool FGsAgitDataController::IsRented() const
{
	time_t currServeTime = FGsTimeSyncUtil::GetServerNowSecond();

	// 주의: 대여 기간이 지난 후 다시 들어올 경우 0을 주는게 아니라, 마지막 대여 시간을 준다고 함.
	// 따라서 현재 시간보다 미래시간일 경우에 대여되었다고 판단하도록 함.
	return (currServeTime < _rentalEndDate) ? true : false;
}
