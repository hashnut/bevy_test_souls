#pragma once
#include "CoreMinimal.h"

#include "GameFlow/GameContents/ContentsGame/GsContentsGameBase.h"

class UGsUIWindowStarLegacy;


// 별의 유산 컨텐츠 구성 코드
// https://jira.com2us.com/wiki/pages/viewpage.action?pageId=396019674
class FGsGameStateStarLegacy : public FGsContentsGameBase
{
	using Super = FGsContentsGameBase;
private:
	TWeakObjectPtr<UGsUIWindowStarLegacy> _windowUI;

public:
	FGsGameStateStarLegacy();

	// 가상 함수
public:
	virtual void Enter() override;
	virtual void Exit() override;

	// 로직 함수
public:
	// 등급 오픈 처리
	void OpenMagnitude(int In_slotNum);
	// 슬롯 선택 처리
	void SelectSlot(int In_slotNum);
	// 별의 유산 소환 시작
	void StartStarLegacySummon();
	// 슬롯 갱신 요청
	void ReqReroll();
	// 카테고리 탭 변경
	void ChangedCategoryTab(int In_tabIndex);

	// get
public:	
	// 등급 오픈 남은 갯수
	int GetMagnitudeOpenRemainCount();
	// bm 아이템 갯수 구하기
	int FindBMItemCount();
	
};