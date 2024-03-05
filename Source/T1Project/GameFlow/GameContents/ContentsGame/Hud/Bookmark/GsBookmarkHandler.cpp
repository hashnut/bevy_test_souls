#include "GsBookmarkHandler.h"

#include "Management/ScopeGame/GsWorldMapManager.h"
#include "Management/ScopeGame/GsBookmarkManager.h"
#include "Management/ScopeGame/GsAIManager.h"
#include "Management/ScopeGlobal/GsLevelManager.h"

#include "Map/Bookmark/GsBookmarkData.h"

#include "Item/GsItemManager.h"
#include "Item/GsItem.h"

#include "Shared/Client/SharedEnums/SharedItemEnum.h"

#include "UI/UIContent/Helper/GsUIHelper.h"

#include "Net/GsNetSendServiceWorld.h"

#include "GameObject/Movement/LocalPlayer/GsMovementAutoDefine.h"
#include "../UTIL/GsLevelUtil.h"

// 북마크 이동 요청
void FGsBookmarkHandler::SendBookmarkTeleportUI(EGsBookmarkUIType In_type, int In_itemIndex)
{
	uint64 bookmarkId = 0;
	if (In_type == EGsBookmarkUIType::Bookmark)
	{
		FGsBookmarkData* data = GSBookmark()->GetBookmarkDataByIndex(In_itemIndex);
		if (data == nullptr)
		{
			return;
		}
		bookmarkId = data->GetId();
	}
	else
	{
		int currShowMapId = GSWorldMap()->GetShowRegionMapId();		
		TArray<FGsBookmarkData*>* listData = GSBookmark()->GetBookmarkDataByMapId(currShowMapId);
		if (listData == nullptr)
		{
			return;
		}

		if (In_itemIndex >= listData->Num())
		{
			return;
		}
		FGsBookmarkData* data = (*listData)[In_itemIndex];
		if (data == nullptr)
		{
			return;
		}
		bookmarkId = data->GetId();
	}
	
	SendBookmarkTeleport(bookmarkId);		
}

// 북마크 이동 요청
void FGsBookmarkHandler::SendBookmarkTeleport(uint64 In_bookmakrId)
{
	TArray<TWeakPtr<FGsItem>> findItems =
		GItem()->FindItem(ItemType::CONSUME, ItemCategorySub::WARP_TO_BOOKMARK_SCROLL);

	//LSJ : findItems 원소가 없을수도 있을텐데?? 일부러 이렇게 처리하는건가...
	
	// item id 나오면 체크후 보낸다
	FGsNetSendServiceWorld::SendBookmarkTeleport(In_bookmakrId, findItems[0].Pin()->GetDBID());
}

// 북마크 이동(지역맵)
void FGsBookmarkHandler::MoveToBookmarkRegionMap(int In_itemIndex)
{
	int currShowMapId = GSWorldMap()->GetShowRegionMapId();
	TArray<FGsBookmarkData*>* listData = GSBookmark()->GetBookmarkDataByMapId(currShowMapId);
	if (listData == nullptr)
	{
		return;
	}

	if (In_itemIndex >= listData->Num())
	{
		return;
	}
	FGsBookmarkData* data = (*listData)[In_itemIndex];
	if (data == nullptr)
	{
		return;
	}

	GSAI()->MoveToPosAuto(EGsMovementAutoContentsType::RegionMapBookmarkMove, currShowMapId,
		data->GetPos());

}

// 즐겨찾기 id가져오기 (지역맵 item 인덱스)
bool FGsBookmarkHandler::GetBookmarkIdByRegionMapItem(int In_index, uint64& Out_bookmarkId)
{
	if (In_index == -1)
	{
		return false;
	}

	int currShowMapId = GSWorldMap()->GetShowRegionMapId();
	TArray<FGsBookmarkData*>* listData = GSBookmark()->GetBookmarkDataByMapId(currShowMapId);
	if (listData == nullptr)
	{
		return false;
	}

	if (In_index >= listData->Num() ||
		In_index < 0)
	{
		return false;
	}
	FGsBookmarkData* data = (*listData)[In_index];
	if (data == nullptr)
	{
		return false;
	}

	Out_bookmarkId = data->GetId();
	return true;
}

// 북마크 이동(즐겨찾기 팝업)
void FGsBookmarkHandler::AutoMoveToBookmarkPopup(int In_itemIndex)
{

	FGsBookmarkData* data = GSBookmark()->GetBookmarkDataByIndex(In_itemIndex);
	if (data == nullptr)
	{
		return;
	}

	GSAI()->MoveToPosAuto(EGsMovementAutoContentsType::RegionMapBookmarkMove, data->GetMapId(),
		data->GetPos());
}