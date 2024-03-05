// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

class FGsAgitDungeonRaidData;

/**
 * 길드 영지 관련 정보들 관리
 */
class FGsAgitDataController final
{
private:
	time_t _rentalEndDate = 0; // 대여 만료 날짜
		
	// Shared로 해야할지 고민이 된다
	FGsAgitDungeonRaidData* _dungeonRaidData = nullptr;  // 이걸 포인터로 해서 날려버리자

public:
	FGsAgitDataController() = default;
	~FGsAgitDataController();

public:
	void Clear();
	void ResetDungeon();

	FGsAgitDungeonRaidData* GetRaidData();

	// 렌트상태인가
	bool IsRented() const;
	time_t GetRentalEndDate() const { return _rentalEndDate; }
	void SetRentalEndDate(const time_t InRentalEndDate) { _rentalEndDate = InRentalEndDate; }
};
