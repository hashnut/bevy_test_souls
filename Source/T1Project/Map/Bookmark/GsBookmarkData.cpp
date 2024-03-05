#include "GsBookmarkData.h"

// 데이터 세팅
void FGsBookmarkData::SetAllData(const FString& In_name, const FVector& In_pos, int In_mapId)
{
	_name = In_name;
	_pos = In_pos;
	_mapId = In_mapId;
}
// 이름 변경
void FGsBookmarkData::SetName(const FString& In_name)
{
	_name = In_name;
}