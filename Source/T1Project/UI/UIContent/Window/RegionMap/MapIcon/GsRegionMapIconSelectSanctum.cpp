#include "GsRegionMapIconSelectSanctum.h"

#include "Sanctum/GsSanctumData.h"

#include "UI/UIContent/Window/RegionMap/MapIcon/GsUIIconRegionSelectButtonSanctum.h"

// 己家 沥焊 技泼
void UGsRegionMapIconSelectSanctum::SetSanctumInfo(const FGsSanctumData* In_data, FText In_spotName)
{
	if (In_data == nullptr)
	{
		return;
	}
	UGsUIIconRegionSelectButtonSanctum* selectIcon = Cast<UGsUIIconRegionSelectButtonSanctum>(_icon);
	if (selectIcon == nullptr)
	{
		return;
	}

	selectIcon->SetSanctumInfo(In_data, In_spotName);

	_sanctumId = In_data->GetSanctumId();
}

// 急琶 贸府
void UGsRegionMapIconSelectSanctum::SetSelect(bool In_select)
{
	UGsUIIconRegionSelectButtonSanctum* selectIcon = Cast<UGsUIIconRegionSelectButtonSanctum>(_icon);

	if (selectIcon == nullptr)
	{
		return;
	}

	selectIcon->SetSelect(In_select);
}