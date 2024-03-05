#pragma once

#include "CoreMinimal.h"
#include "Shared/Client/SharedEnums/PD_ResultEnum.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
/*
	위치 즐겨찾기 함수
*/

class FGsBookmarkFunc
{
public:
	// 위치 즐겨찾기 글자 크기 체크
	static bool CheckBookmarkNameLength(const FString& In_name);
	// 북마크 스크롤 아이템 갯수 구하기
	static int GetBookmarkScrollItemCount();
	// 즐겨찾기 아이템 있는지(없으면 티커 출력)
	static bool IsBookmarkScrollExist(uint64& Out_itemDBId, OUT ItemId& OutItemId);
	// 공유 응답 패킷 result 티커 출력
	static bool ShowTickerBookmarkShareError(PD::Result In_result);
	// 공유 이동 응답 패킷 result 티커 출력
	static bool ShowTickerBookmarkShareWarpError(PD::Result In_result);
	// 북마크 공유 성공 티커 출력
	static void ShowTickerBookmarkShareSuccess();
};