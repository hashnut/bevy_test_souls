#pragma once
#include "CoreMinimal.h"
#include "Shared/Client/SharedEnums/SharedCommonEnum.h"
#include "Shared/Client/SharedEnums/PD_ResultEnum.h"
/*
	워프 함수
*/

class FGsWarpFunc
{
public:
	// 재화 충분한지
	static bool IsCurrencyEnougth(CurrencyType In_type, int In_cost);
	// 워프 실패 체크
	static bool CheckWarpFailure(PD::Result In_result);
};