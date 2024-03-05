#pragma once

#include "CoreMinimal.h"
#include "Shared/Client/SharedEnums/SharedStatEnum.h"

struct FGsMonsterKnowledgeUICaptureData
{
	uint32 _mainTab = 0;
	uint32 _categoryTab = 0;
	float _categoryScrollOffset = 0.f;

	uint64 _lastViewParam = 0;
	TArray<StatType> _StatFilterSet;

	uint32 _reservedId = 0;

	uint32 _selectedNodeId = 0;
	bool _isSelectedNodeAll = false;
	uint32 _selectedSpecialNodeId = 0;

	// 리뉴얼 도감에서 되돌아올 대륙과 맵 그룹 정보
	uint32 _currentCategoryId = 0;
	uint32 _currentMapGroupId = 0;

	// 선택된 맵 그룹 인덱스
	int32 _mapGroupIndex = -1;

	// 지식의 결정 사용 팝업을 열었는지 여부
	bool _isUseKnowledgeGemPopup = false;

	// 몬스터를 선택해서 월드맵으로 갔을 경우, 해당 몬스터의 인덱스로 이동
	int32 _monsterTileIndex = -1;
};


enum class EMonsterCollectionSlateCallbackReason
{
	ChangedSelectedAll,
	SelectedSpecialNode,
	ReqOpenSlate,
	ReqActiveNode,
	ReqOpenSpecialNode,
};