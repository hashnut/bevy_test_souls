#pragma once

#include "CoreMinimal.h"

#include "Classes/GsManager.h"

#include "Management/GsScopeHolder.h"
#include "Management/GsScopeGame.h"

#include "Runtime/DataCenter/Public/Shared/Client/SharedEnums/SharedFairyEnum.h"


class FGsStarLegacySlotArrangementInfo;
class FGsStarLegacyCategoryInfo;
class FGsGameStateStarLegacy;

struct FGsSchemaStarLegacySlot;
struct FGsSchemaStarLegacySlotSpawn;
struct FGsSchemaStarLegacyConfig;
struct FGsSchemaStarLegacyCategory;

// 별의 유산 관리 클래스
// https://jira.com2us.com/wiki/pages/viewpage.action?pageId=396019674

class FGsStarLegacyManager : public IGsManager
{
private:
	// 배치된 슬롯 정보(획득 가능한, 서버에서 보낸 정보)
	// 카테고리 id가 키인 맵데이터(카테고리 id: n성계)
	TMap<int, TArray<FGsStarLegacySlotArrangementInfo*>> _mapSlotArrangInfo;
	// 확률정보(카테고리)
	TMap<int ,FGsStarLegacyCategoryInfo*> _mapCategoryInfo;
	// 카테고리 id 순서
	TArray<int> _arrayCategoryIds;

	// 슬롯 갯수
	const int MAX_SLOT_COUNT = 10;
	// 이때까지 등급 오픈 갯수
	int _currentOpenMagnitudeCount = 0;
	// 현재 선택된 별의 유산 카테고리 id(n 성계)
	int _currentStarLegacyCategoryId = 0;
	// 선택된 슬롯 id들
	TArray<int> _arraySelectSlotIds;
	// 한칸 넘어갈때 대기 시간(틱 시간)
	TArray<float> _arrayTimeTickWaitingTimeToPass;
	// 룰렛 회전 횟수(n바퀴)
	int _numberOfRouletteSpins;

	// 테스트 데이터로 만들지
	bool _isMakeTestStarLegacyData = false;	
	// 카테고리별 서버 선택 슬롯 id
	TMap<int, int> _mapCategoryServerPickSlotId;

	// 가상 함수
public:
	// 초기화
	virtual void Initialize()override;
	// 정리
	virtual void Finalize() override;

	// 로직 함수
public:

	// 테이블 데이터 가공
	void ProcessTableData();
	// 테스트 배치 데이터 만들기
	void MakeTestSlotArrangementInfo();

	// 테스트 배치 데이터 만들기(카테고리별)
	void MakeTestSlotArrangementInfoByCategoryId(int In_categoryId);
	
	// 더미 정보 가져오기
	void GetDummyTestSlotArrangementInfo(TArray<FGsStarLegacySlotArrangementInfo*>& Out_info);
	// 데이터 초기화
	void InitializeData();
	// 리롤 초기화
	void InitializeReroll();
	// 슬롯 정보 클리어(전부)
	void ClearAllSlotArrangeInfo();
	// 슬롯 정보 카테고리당 클리어
	void ClearSlotArrangeInfoByCategory(int In_categoryId);
	// 오픈 정보 초기화
	void InitOpenInfo();

	// get, set
public:
	// 카테고리 id로 슬롯 array 테이블 값 가져오기
	TArray<const FGsSchemaStarLegacySlot*> FindSlotTbl(int In_categoryId);

	// 슬롯 id로 아이템 스폰 정보 array 테이블 값 가저오기
	TArray<const FGsSchemaStarLegacySlotSpawn*> FindSlotRewardTbl(int In_slotId);

	// 별의 유산 game state 접근
	FGsGameStateStarLegacy* GetGameStateStarLegacy();

	// 별의 유산 config 테이블 접근
	const FGsSchemaStarLegacyConfig* FindStarLegacyConfig();

	// 최대 등급 오픈 갯수
	int GetMaxMagnitudeOpenCount(FairyGrade In_fairyGrade);
	// 현재 등급 오픈 갯수 접근
	int GetCurrentOpenMagnitudeCount()
	{
		return _currentOpenMagnitudeCount;
	}
	// 현재 등급 오픈 갯수 저장
	void SetCurrentOpenMagnitudeCount(int In_count)
	{
		_currentOpenMagnitudeCount = In_count;
	}
	// 슬롯 정보 가져오기
	TArray<FGsStarLegacySlotArrangementInfo*> GetSlotInfo(int In_catetoryId);
	
	// 현재 카테고리 id
	int GetCurrentCategoryId()
	{
		return _currentStarLegacyCategoryId;
	}
	// 카테고리 id 세팅
	void SetCurrentCategoryId(int In_catetoryId)
	{
		_currentStarLegacyCategoryId = In_catetoryId;
	}
	// 테스트로 데이터를 만들지 세팅
	void SetIsMakeTestStarLegacyData(bool In_val)
	{
		_isMakeTestStarLegacyData = In_val;
	}
	// 테스트로 데이터를 만들지 접근
	bool GetIsMakeTestStarLegacyData()
	{
		return _isMakeTestStarLegacyData;
	}
	// 카테고리 제일 앞에꺼 id
	int GetFirstCategoryId();
	// 선택 슬롯 클리어
	void ClearSelectSlotId();
	// 선택 슬롯 id 추가
	void AddSelectSlotId(int In_slotId);
	// 선택 슬롯 id 삭제
	void DelSelectSlotId(int In_slotId);
	// 선택된 id인가
	bool IsSelectedId(int In_slotId);
	// 선택 슬롯 갯수
	int GetSelectSlotCount();
	// 현재 탭(카테고리, n성계) 테이블 접근
	const FGsSchemaStarLegacyCategory* GetCurrentCategoryTbl();

	// 한칸 넘어갈때 대기 시간(틱 시간)
	float GetTimeTickWaitingTimeToPass(int In_index);

	// 룰렛 회전 횟수(n바퀴)
	int GetNumberOfRouletteSpins()
	{
		return _numberOfRouletteSpins;
	}
	// 서버에서 pick 한 슬롯 id
	int GetServerPickSlotId();
	// 선택한 슬롯 id 접근
	TArray<int> GetArraySelectSlotIds()
	{
		return _arraySelectSlotIds;
	}
	// 카테고리 id 가져오기
	TArray<int> GetArrayCategoryIds()
	{
		return _arrayCategoryIds;
	}
	// 탭인덱스(0,1)로 카테고리 정보 가져오기
	const FGsStarLegacyCategoryInfo* FindCategoryInfoByTabIndex(int In_tablIndex);
};
#define GSStarLegacy() UGsScopeHolder::GetGameManagerFType<FGsStarLegacyManager>(UGsScopeGame::EManagerType::StarLegacy)