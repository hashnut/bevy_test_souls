#pragma once

#include "CoreMinimal.h"

#include "UI/UIContent/Window/RegionMap/MapIcon/GsRegionMapIconFixed.h"

#include "GsRegionMapIconSelect.generated.h"

/*
	선택 표시 아이콘(북마크, 포탈)
*/

UCLASS()
class UGsRegionMapIconSelect : public UGsRegionMapIconFixed
{
	GENERATED_BODY()

private:
	// left list index(for data access)
	int _listSelectIndex;

	bool _isSelect = false;

	// 로직 함수
public:
	// 선택 처리
	void SetSelect(bool In_select);

	void SetSpotName(FText In_name);

	void SetSelectIndex(int In_index)
	{
		_listSelectIndex = In_index;
	}

	// get
public:
	int GetListSelectIndex()
	{
		return _listSelectIndex;
	}

	bool GetSelect()
	{
		return _isSelect;
	}
};