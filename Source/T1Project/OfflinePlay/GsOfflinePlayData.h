#pragma once
#include "Shared/Shared/SharedInclude/SharedTypes.h"


/**
 * 2022/11/02 PKT - 오프라인 플레이 결과 값
 */
struct FGsOfflinePlayResultData
{
	int32 _runningTime = 0;				// 2021/06/21 PKT - 이용 시간
	OffLineExp _Exp = 0;				// 2021/06/21 PKT - 습득한 경험치
	CurrencyTable _CcurrencyTable;		// 재화 정보
	TArray<ItemDataCreated> _itemList;	// 습득한 아이템

	bool _isPrevAutoState = false;		// 2023/5/25 PKT - 접속 전 오프라인이 진행 되고 있었는가?
};