#pragma once
#include "CoreMinimal.h"
#include "Classes/GsManager.h"

#include "Management/GsScopeHolder.h"
#include "Management/GsScopeGame.h"

#include "Map/Bookmark/GsBookmarkDefine.h"
#include "Map/Bookmark/GsBookmarkShareInfo.h"

#include "Shared/Shared/SharedInclude/SharedTypes.h"

#include "Shared/Client/SharedEnums/SharedBookmarkEnum.h"

#include "Shared/Shared/SharedPackets/PD_World_Client_Bypass.h"

#include "Message/GsMessageUI.h"

#include "Engine/Classes/Engine/EngineTypes.h"

/*
	위치 즐겨 찾기 관리자
*/
class FGsBookmarkData;
struct IGsMessageParam;

class FGsBookmarkManager : public IGsManager
{
	// 멤버 변수
private:
	TArray<FGsBookmarkData*> _arrBookmarkData;
	// 검색용 데이터(맵별)
	TMap<int, TArray<FGsBookmarkData*>> _mapBookmarkList;
	// 변경 이름 저장
	FString _reqChangeName;
	// 삭제 id
	uint64 _deleteId;
	// 정렬 타입
	EGsBookmarkSortType _currSortType = EGsBookmarkSortType::EnrollTime;

	// 북마크 리스트 정렬해야하는지
	bool _isDirtySortBookMarkList = false;

	// 최근 공유 타입
	BookmarkShareType _currentSharedType = BookmarkShareType::MAX;

	// 공유 정보
	FGsBookmarkShareInfo _shareInfo;

	// 공유 쿨타임 시작인지
	bool _isShareCoolTimeStart = false;
	// 공유 쿨타임 시작 시작
	int64 _shareCoolTimeStartTime = 0;

	MsgUIHandleArray			_listUiDelegate;

	FTimerHandle _testExpTimeHandle;

	int _testCurrentCollectionIndex = 0;

	bool _isStartTestCollectionCheat = false;

	// 가상 함수
public:
	// 초기화
	virtual void Initialize()override;
	// 정리
	virtual void Finalize()override;
	// 갱신
	virtual void Update(float In_delta) override;

	// 로직 함수
protected:
	// 서버 데이터 초기화(가져오기)
	void InitServerData();

	// 정렬
	void Sort(EGsBookmarkSortType In_sortType);
	// 맵 검색용 리스트 정렬
	void SortMapList(int In_mapId, EGsBookmarkSortType In_sortType = EGsBookmarkSortType::EnrollTime);
	// 데이터 클리어
	void ClearData();
	// 맵 리스트에 추가
	void AddMapList(FGsBookmarkData* In_data);
	// 맵 리스트에서 삭제
	void DelMapList(FGsBookmarkData* In_data);

	

	// 요청
public:
	// 북마크 만들기 요청
	void SendBookmarkCreate(const FString& In_name);
	// 북마크 수정
	void SendBookmarkUpdate(int In_itemIndex,const  FString& In_name);
	// 북마크 삭제(아이템 인덱스로 옴)
	void SendBookmarkDelete(int In_itemIndex);

	// 응답
public:
	// 북마크 만들기 결과
	void RecvBookmarkCreate(uint64 In_id, const FString& In_name,
		const FVector& In_pos, int In_mapId);
	// 북마크 수정 결과
	void RecvBookmarkUpdate(uint64 In_id);
	// 북마크 삭제 결과
	void RecvBookmarkDelete(uint64 In_id);

	// 이벤트
public:
	// 정렬 변경
	void OnSortChange(EGsBookmarkSortType In_sortType);
	// ui 열기전 호출(정리할껀 정리해라 이말이야)
	void OnPreOpenUI();

	// 메시지
public:
	// 북마크 모든정보 초기화
	void OnInitBookMarkAllInfo(const IGsMessageParam*);

	// get, set
public:
	// id로 데이터 찾기
	FGsBookmarkData* GetBookmarkDataById(uint64 In_id);
	// 배열 인덱스로 데이터 찾기
	FGsBookmarkData* GetBookmarkDataByIndex(int In_idex);
	// 북마크 데이터 접근
	TArray<FGsBookmarkData*> GetBookmarkData() { return _arrBookmarkData; }

	EGsBookmarkSortType GetCurrSortType() { return _currSortType; }
	// 해당 맵의 북마크 정보 가져오기
	TArray<FGsBookmarkData*>* GetBookmarkDataByMapId(int In_mapId);

	void SetCurrentShareType(BookmarkShareType In_shareType)
	{
		_currentSharedType = In_shareType;
	}
	BookmarkShareType GetCurrentShareType()
	{
		return _currentSharedType;
	}
	

	FGsBookmarkShareInfo& GetShareInfo()
	{
		return _shareInfo;
	}

	void StartShareCoolTime();
	

	bool GetIsShareCoolTime()
	{
		return _isShareCoolTimeStart;
	}

	// test
public:
	void TestExp(bool In_on);

	void TestAllCollectionComplete();
	void UpdateAllCollectionComplete();
};

#define GSBookmark() UGsScopeHolder::GetGameManagerFType<FGsBookmarkManager>(UGsScopeGame::EManagerType::Bookmark)