#pragma once
#include "CoreMinimal.h"
#include "UI/UIContent/Window/RegionMap/MapIcon/GsRegionMapIconFixed.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "GsRegionMapIconSelectSanctum.generated.h"

/*
*  己家 钎矫 酒捞能(劝己, 厚劝己)
*/
struct FGsSanctumData;

UCLASS()
class UGsRegionMapIconSelectSanctum : public UGsRegionMapIconFixed
{
	GENERATED_BODY()

private:
	// 己家 id
	SanctumAreaId _sanctumId;

	// get, set
public:
	// 己家 id 技泼
	SanctumAreaId GetSanctumId(){return _sanctumId;}
	// 己家 沥焊 技泼
	void SetSanctumInfo(const FGsSanctumData* In_data, FText In_spotName);

	// 急琶 贸府
	void SetSelect(bool In_select);
};