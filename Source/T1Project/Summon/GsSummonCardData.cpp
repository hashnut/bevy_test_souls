#include "GsSummonCardData.h"

#include "Fairy/GsFairyData.h"

bool FGsSummonCardDataFairy::IsHighCard()
{
	return (int)_grade >= (int)FairyGrade::RARE;
}

bool FGsSummonCardDataCostume::IsHighCard()
{

	return (int)_grade >= (int)CostumeGrade::RARE;
}