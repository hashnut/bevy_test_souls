#pragma once

#include "CoreMinimal.h"

class FGsStarLegacySlotItemInfo;

// 별의 유산 확률 정보(ex:5등성)
// https://jira.com2us.com/wiki/pages/viewpage.action?pageId=396019674

class FGsStarLegacySlotMagnitudeInfo
{
public:
	// 배열: 획득 확율(%, 추가 슬롯 선택에 따른)
	TArray<float> _arrayAcquisitionProbabilityPercentage;
	// 획득 가능 보상품들
	TArray<FGsStarLegacySlotItemInfo*> _arraySlotItemInfo;
	// 텍스트 정보(ex: 5등성)
	FText _nameText;
	// 슬롯 id
	int _slotId;

	// 로직
public:
	// 정리
	void Finalize();
	// 천배율에서 100분율로 만들기
	void SetPercentageByThousandMagnification(TArray<int> In_arrayThousandVal);
};