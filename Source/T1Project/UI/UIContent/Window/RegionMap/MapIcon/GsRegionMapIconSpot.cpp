#include "GsRegionMapIconSpot.h"

#include "UI/UIContent/Window/RegionMap/MapIcon/GsUIIconRegionSpot.h"

void UGsRegionMapIconSpot::SetSpotName(FText In_name)
{
	UGsUIIconRegionSpot* spotIcon = Cast<UGsUIIconRegionSpot>(_icon);
	if (spotIcon == nullptr)
	{
		return;
	}

	spotIcon->SetSpotName(In_name);
}