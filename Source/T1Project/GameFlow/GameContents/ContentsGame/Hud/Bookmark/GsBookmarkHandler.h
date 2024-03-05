#pragma once

#include "CoreMinimal.h"
#include "Map/Bookmark/GsBookmarkDefine.h"

/*
	위치 즐겨 찾기 핸들러:
	1) bookmark 매니저에서 다른 게임 매니저 참조 하는것들 여기서 처리하자
*/

class FGsBookmarkHandler
{
public:
	// 북마크 이동 요청
	void SendBookmarkTeleportUI(EGsBookmarkUIType In_type, int In_itemIndex);
	// 북마크 이동 요청
	void SendBookmarkTeleport(uint64 In_bookmakrId);
	// 북마크 이동(지역맵)
	void MoveToBookmarkRegionMap(int In_itemIndex);

	// 북마크 이동(즐겨찾기 팝업)
	void AutoMoveToBookmarkPopup(int In_itemIndex);
	// 즐겨찾기 id가져오기 (지역맵 item 인덱스)
	bool GetBookmarkIdByRegionMapItem(int In_index, uint64& Out_bookmarkId);
};
