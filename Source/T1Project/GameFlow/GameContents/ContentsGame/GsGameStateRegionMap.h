#pragma once
#include "CoreMinimal.h"
#include "GsContentsGameBase.h"
#include "Message/GsMessageUI.h"
#include "Message/GsMessageBoss.h"
#include "Map/WorldMap/GsWorldMapDefine.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "UI/UILib/Define/GsUIDefine.h"
#include "DataSchema/Map/WorldMap/GsSchemaWorldMapSpoilsItem.h"

/*
	지역맵 컨텐츠 코드
*/
class UGsUIWindowRegionMap;
class FGsRegionInfo;
class FGsMovementHandlerLocalPlayer;
class FGsRegionMonsterItemInfo;
class FGsRegionMonsterInfo;
class FGsRegionMapSanctumInfo;

struct IGsMessageParam;


class FGsGameStateRegionMap : public FGsContentsGameBase
{
public:
	struct FGsSpoilsMonster
	{
		MapId mapId = INVALID_MAP_ID;
		CreatureId npcId = INVALID_CREATURE_ID;
		Level level = INVALID_LEVEL;

	public:
		FGsSpoilsMonster() = default;
		virtual ~FGsSpoilsMonster() = default;

	public:
		void Set(MapId inMapId, CreatureId inNpcId, Level inLevel)
		{
			mapId = inMapId;
			npcId = inNpcId;
			level = inLevel;
		}

		void Clear()
		{
			mapId = INVALID_MAP_ID;
			npcId = INVALID_CREATURE_ID;
			level = INVALID_LEVEL;
		}

		bool IsValid()
		{
			return !(INVALID_MAP_ID == mapId
				|| INVALID_CREATURE_ID == npcId
				|| INVALID_LEVEL == level);
		}
	};

	struct FGsSpoils
	{
	public:
		bool isSelect = false;
		FGsSchemaWorldMapSpoilsItem spoilsItem;

	public:
		FGsSpoils() = default;
		virtual ~FGsSpoils() = default;

	public:
		void Set(const FGsSchemaWorldMapSpoilsItem& inSpoils)
		{
			spoilsItem = inSpoils;
			isSelect = true;
		}

		void Clear()
		{
			isSelect = false;
		}

		bool IsValid()
		{
			return isSelect;
		}
	};

	using Super = FGsContentsGameBase;

private:
	// 지역맵 성소 아이콘 갯수(2개)
	const int REGION_MAP_SANCTUM_ICON_COUNT = 2;

	FGsSpoilsMonster _selectSpoilsMonster;
	FGsSpoils _selectSpoils;

private:
	TWeakObjectPtr<UGsUIWindowRegionMap> _windowUI;

	// 메시지(해지용)
private: 
	MsgUIHandleArray _uiDelegates;
	TArray<TPair<MessageBoss, FDelegateHandle>> _bossDelegates;	

public:
	FGsGameStateRegionMap();
	virtual ~FGsGameStateRegionMap();

	// static
public:

	static bool IsInvadePossibleMap(int In_mapId);

	// 가상 함수
public:
	virtual void Enter() override;
	virtual void Exit() override;

	// 메시지
public:
	// CHANGE_REGION_MAP
	void OnChangeRegionMap(const IGsMessageParam*);
	void OnChangeRegionMapScene(const IGsMessageParam*);

	// 로직 함수
public:
	// 지역정보 만들기
	FGsRegionInfo* MakeRegionInfo(int In_mapId);
	// 맵 변경 타입 저장
	void SetMapChangeType(EGsWorldMapChangeType In_type, 
		int In_mapId = 0, 
		unsigned long long In_bookMarkId = 0, 
		int In_selectMonsterId= 0);
	// 포털로 이동
	void MoveToPortal(int In_index);
	// 침공전 포털로 이동(지역맵에서)
	// 몬스터 드랍 위치로 이동
	void MoveToPortalPos(FVector In_pos, float In_useAcceptanceRadius = 0.0f);

	// 지역맵 이동과 이동 팝업 띄우기(return false면 이동 못하는곳임)
	bool ShowRegionMapAndMovePopup(int In_mapId);

	bool ShowCreatureMap(const MapId InMapId, const CreatureId InCreatureId);
	// 몬스터 팝업과 지도 열기
	// 지역맵 안열어달라는 요청이 와서 일단 옵션으로 뺌
	bool ShowMonsterInfoPopupAndMap( MapId InMapId,  CreatureId InCreatureId, bool In_isRegionWindowOpen = false);
	bool ShowDropItemMap(const MapId InMapId, const CreatureId InCreatureId, const Level InLevel, EGsToggleType inToggleType = EGsToggleType::Auto);
	// 1. 몬스터 위치(획득처) 아이콘 표시
	// 2. 필터링 버튼(우측 버튼) 상태 on
	// 현재ShowDropItemMap 함수는 지도 열어서 획득처 탭변경 까지 하는데
	// 지도내에 몬스터 리스트 클릭시에도 출력해야되어서 별도 함수 생성
	// https://jira.com2us.com/jira/browse/CHR-23756
	void ShowSpoilIconAndFilterButtonOn(MapId In_mapId, CreatureId In_creatureId, Level In_level, EGsToggleType In_toggleType = EGsToggleType::Active);
	// 몬스터 드랍 필터 갱신
	// https://jira.com2us.com/jira/browse/CHR-23756
	void InvalidateMonsterDropFilter();


	void SetMarkerOfMonster(const MapId InMapId, const CreatureId InCreatureId, const Level InLevel, OUT TArray<SpawnGroupId>& InSpawnGroupIdArray, OUT TArray<FVector>& InPosArray);

	void SortEquipItem(TArray< FGsRegionMonsterItemInfo*>& In_sortTarget );
	void SortEtcItem(TArray< FGsRegionMonsterItemInfo*>& In_sortTarget);

	// 월드맵 개선 
	// 드랍 아이템 통합 출력
	// https://jira.com2us.com/jira/browse/CHR-23756
	void SortItemAll(TArray< FGsRegionMonsterItemInfo*>& In_sortTarget);
	// 성소 정보 정렬
	// id 오름 차순
	// https://jira.com2us.com/jira/browse/CHR-24218
	void SortSanctumInfo(TArray<FGsRegionMapSanctumInfo*>& In_sortTarget);

	// map id와 몬스터 id로 지역맵 데이터 몬스터 인덱스 구하기
	int FindMonsterIndex(int In_mapId, int In_monsterId);

	// map id와 몬스터 id와 레벨로 지역맵 데이터 몬스터 인덱스 구하기
	int FindMonsterIndexByIdAndLevel(int In_mapId, int In_monsterId, int In_monsterLevel);

	// message
public:
	void OnOpenBossPopup(const IGsMessageParam* inParam);
	void OnCloseBossPopup(const IGsMessageParam* inParam);

	//Spoil
public:
	void SelectSpoils(const FGsSchemaWorldMapSpoilsItem& inSpoils);
	void ClearSpoils();
	bool IsEmptySpoils();
	bool IsEqualsSpoils(const FGsSchemaItemCommon* inItem);

	//spoils monster
public:
	void SelectSpoilsMonster(MapId inMapId, CreatureId inCreatureId, Level inLevel);
	void ClearSpoilsMonster();
	bool IsEmptySpoilsMonster();
	bool TryGetSpoilsMonster(OUT FGsSpoilsMonster& outSpoilsMonster);
	bool IsSpoilsMonsterExist(MapId inMapId);

	// get
public:
	FGsMovementHandlerLocalPlayer* GetMovementHandlerLocalPlayer();

	// find region map info
	// create infomation if it doesn't exist
	FGsRegionInfo* FindRegionInfo(int In_mapId);

	// 몬스터 아이템 검색할 타입 찾기(일반, 침공 공격, 침공 수비)
	EGsRegionItemInfoSetType GetNowMapItemInfoSetType(FGsRegionMonsterInfo* In_monsterInfo);

	
};