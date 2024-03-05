#pragma once

#include "EventProgress/GsEventProgressInterface.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "EventProgress/GsEventProgressDefine.h"

class FGsDungeonReserveWarpHandler : public IGsEventProgressInterface
{
private:
	MapId _mapId = INVALID_MAP_ID;
	int32 _spotId = 0;
	CurrencyType _costType = CurrencyType::NONE;
	int32 _costAmount = 0;
	FText _warpText = FText::GetEmpty();
	const class FGsDungeonGroupBase* _destGroupData = nullptr;

	// 2023/11/24 PKT - Battle Arena 에서 사용
	FText _enterMapText = FText::GetEmpty();
public:
	/**
	 * 2022/10/26 PKT - IGsEventProgressInterface
	 */
	 // 이벤트 시작
	 virtual void StartEvent(EGsEventProgressType In_startType, EGsEventProgressType In_preType) override;
	 // 이벤트 종료
	 virtual void FinishEvent(EGsEventProgressType In_finishType, EGsEventProgressType In_nextType) override;

public:
	void Clear();

	void ReserveWarp(int32 InMapId
		, int32 InSpotId
		, CurrencyType InCostType
		, int32 InCostAmount
		, FText InWarpText
		, const class FGsDungeonGroupBase* InDestGroupData);

	void ReserveWarp(int32 InMapId, FText InTitle, FText InWarpText, const class FGsDungeonGroupBase* InDestGroupData);

	void ReserveCancel();
};