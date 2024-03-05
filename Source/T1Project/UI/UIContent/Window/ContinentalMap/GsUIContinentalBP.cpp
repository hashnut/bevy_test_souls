#include "GsUIContinentalBP.h"

#include "UI/UIControlLib/ContentWidget/GsButton.h"

#include "UI/UIContent/Window/ContinentalMap/GsUIRegionItem.h"

#include "Core/Public/Delegates/Delegate.h"

#include "Engine/Classes/Engine/GameViewportClient.h"

#include "T1Project.h"

// 지역 item 찾기
UGsUIRegionItem* UGsUIContinentalBP::FindRegionItem(int In_mapId)
{
	UGsUIRegionItem* findItem = nullptr;
	for (UGsUIRegionItem* item : _arrayRegionItems)
	{
		if (item == nullptr)
		{
			continue;
		}
		if (item->GetRegionMapId() == In_mapId)
		{
			findItem = item;
			break;
		}
	}
	return findItem;
}
// 지역 item 찾기(그룹 정보도 참조)
// https://jira.com2us.com/jira/browse/C2URWQ-7502
UGsUIRegionItem* UGsUIContinentalBP::FindRegionItemMapIdAndGroupIds(int In_mapId)
{
	UGsUIRegionItem* findItem = nullptr;
	for (UGsUIRegionItem* item : _arrayRegionItems)
	{
		if (item == nullptr)
		{
			continue;
		}
		if (item->GetRegionMapId() == In_mapId)
		{
			findItem = item;
			break;
		}
		// 그룹에 속하는지
		// 
		TArray<int> groupIds =  item->GetRegionGroup();
		if (groupIds.Contains(In_mapId) == true)
		{
			findItem = item;
			break;
		}
	}
	return findItem;
}

bool UGsUIContinentalBP::IsRegionItemUseable()
{
	if (_arrayRegionItems.Num() == 0)
	{
		return false;
	}

	UGsUIRegionItem* item = _arrayRegionItems[0];
	if (item == nullptr)
	{
		return false;
	}
	FGeometry targetGeometry = item->GetCachedGeometry();
	FVector2D getSize = targetGeometry.GetLocalSize();

	return (getSize.IsZero() == true) ? false : true;
}