#include "GsStarLegacySlotMagnitudeInfo.h"

#include "GameFlow/GameContents/ContentsGame/StarLegacy/GsStarLegacySlotItemInfo.h"

#include "UTIL/GsUIUtil.h"

// 정리
void FGsStarLegacySlotMagnitudeInfo::Finalize()
{	
	if (_arraySlotItemInfo.Num() != 0)
	{
		for (FGsStarLegacySlotItemInfo* itemInfo: _arraySlotItemInfo)
		{
			if (itemInfo == nullptr)
			{
				continue;
			}

			delete itemInfo;
		}

		_arraySlotItemInfo.Empty();
	}
}

// 천배율에서 100분율로 만들기
void FGsStarLegacySlotMagnitudeInfo::SetPercentageByThousandMagnification(TArray<int> In_arrayThousandVal)
{
	// 테이블 값 8600 -> 8.6 %
	// 1000으로 나눠야 함
	for (int thousandVal : In_arrayThousandVal)
	{	
		float percentage = UGsUIUtil::GetPercentageByThousandMagnification(thousandVal);
		_arrayAcquisitionProbabilityPercentage.Add(percentage);
	}

}