#include "GsRegionMapIconSelect.h"

#include "UI/UIContent/Window/RegionMap/MapIcon/GsUIIconRegionSelect.h"

// 선택 처리
void UGsRegionMapIconSelect::SetSelect(bool In_select)
{
	UGsUIIconRegionSelect* selectIcon = Cast<UGsUIIconRegionSelect>(_icon);

	if (selectIcon == nullptr)
	{
		return;
	}

	selectIcon->SetSelect(In_select);
}

void UGsRegionMapIconSelect::SetSpotName(FText In_name)
{
	UGsUIIconRegionSelect* selectIcon = Cast<UGsUIIconRegionSelect>(_icon);
	if (selectIcon == nullptr)
	{
		return;
	}

	selectIcon->SetSpotName(In_name);
}
