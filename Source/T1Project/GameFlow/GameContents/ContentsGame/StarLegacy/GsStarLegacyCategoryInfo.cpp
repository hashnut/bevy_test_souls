#include "GsStarLegacyCategoryInfo.h"

#include "GameFlow/GameContents/ContentsGame/StarLegacy/GsStarLegacySlotMagnitudeInfo.h"

#include "Runtime/DataCenter/Public/DataSchema/StarLegacy/GsSchemaStarLegacyCategory.h"

// Á¤¸®
void FGsStarLegacyCategoryInfo::Finalize()
{
	if (_arraySlotMagnitudeInfo.Num() != 0)
	{
		for (FGsStarLegacySlotMagnitudeInfo* magnitudeIter: _arraySlotMagnitudeInfo)
		{
			if (magnitudeIter == nullptr)
			{
				continue;
			}

			magnitudeIter->Finalize();
			delete magnitudeIter;
		}

		_arraySlotMagnitudeInfo.Empty();
	}
}