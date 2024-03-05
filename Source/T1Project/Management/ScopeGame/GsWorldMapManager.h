#pragma once

#include "CoreMinimal.h"
#include "Classes/GsManager.h"

#include "Management/GsScopeHolder.h"
#include "Management/GsScopeGame.h"

#include "Map/WorldMap/GsWorldMapDefine.h"

#include "Message/GsMessageGameObject.h"
#include "Message/GsMessageSystem.h"
#include "Message/GsMessageInvasion.h"

#include "EventProgress/GsEventProgressInterface.h"
#include "EventProgress/GsEventProgressDefine.h"

#include "Shared/Client/SharedEnums/SharedCommonEnum.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "Currency/GsCostPackage.h"

class UGsGameObjectLocalPlayer;

class FGsContinentalInfo;
class FGsMovementHandlerLocalPlayer;
class FGsRegionInfo;
class FGsRegionMonsterInfo;
class FGsPortalInfo;
class FGsRegionGroupInfoBase;
class FGsContinentalSpaceCrackMapInfo;
class FGsRegionInvadeIconInfo;
class FGsGameStateRegionMap;
class FGsRegionMapSanctumInfo;
class FGsRegionMonsterSameIDInfo;

struct FGsSchemaContinentalMapData;
struct FGsSchemaSpotInfo;
struct FGsSchemaSpaceCrackData;
struct FGsSchemaDungeonGroupData;

struct IGsMessageParam;

namespace PD { namespace SC { struct PKT_SC_INVADE_MINIMAP_INFO_UPDATE_READ; } }


//파티 보 물몬스터 위치 정보
struct FGsTreasureMonsterMapInfo
{
	int64   _seperateId{ 0 };
	MapId	_posMapId{ 0 };
	FVector	_position{ 0.f, 0.f, 0.f };
	bool	_localPlayerOwn{ false };

	bool IsNoTreasureMonster() { return _seperateId <= 0; }
};

class FGsWorldMapManager : public IGsManager, public IGsEventProgressInterface
{
	// 멤버 변수
private:
	// 대륙 정보
	TMap<int, FGsContinentalInfo*> _mapContinentalInfo;
	// MapId 를 소유하는 ContinentId Map
	TMap<MapId, ContinentId> _continentIdPerMapId;
	// 대륙 리스트 순서
	TArray<int> _arrContinentalOrder;
	// dungeon info
	TMap<int, FGsRegionGroupInfoBase* > _mapDungeonInfo;
	// space crack map
	TMap<int, FGsRegionGroupInfoBase* > _mapSpaceCrackInfo;
	// 아지트 정보
	// https://jira.com2us.com/jira/browse/CHR-24959
	FGsRegionGroupInfoBase* _agitInfo;

	TArray<FGsContinentalSpaceCrackMapInfo*> _arraySpaceCrackSortInfo;
	// 맵전환 타입
	EGsWorldMapChangeType _mapChangeType = EGsWorldMapChangeType::NONE;
	// 보여줄 지역맵 id
	int _showRegionMapId = 0;
	// 지역맵 보여줄 타입
	EGsRegionMapType _showRegionMapType = EGsRegionMapType::Spoils;
	// 현재 지역 정보
	FGsRegionInfo* _currentRegionInfo;
	// 씬 이미지 사이즈/ 실제 맵 사이즈 비율
	float _uiSceneImageRate;
	// 실제 텍스쳐 사이즈/ bp내의 이미지 사이즈 비율
	float _uiTextureRate;
	// 이동시킬 값(테이블)
	FVector2D _uiShiftMapPos;
	// 지역맵 이미지 반크기 
	FVector2D _regionImageHalfSize;
	// 캐싱 지역정보
	TMap<int, FGsRegionInfo*> _mapRegionCacheData;

	// 위치 즐겨찾기 id(지역지도 보기에서 선택할 id)
	uint64 _selBookMarkId = 0;

	// 플레이어
	UGsGameObjectLocalPlayer* _localPlayer = nullptr;
	// 현재 level id
	int _currentLevelId = 0;

	// message bind
private:
	// 메시지 해제용
	MsgGameObjHandleArray _objectDelegates;

	// system 메세지 해제용
	MsgSystemHandleArray _systemMsg;

	MsgInvasionHandleArray _messageInvasionList;

	// dungeon info
private:
	// 던전 이동 정보
	// 던전 입구 지역맵 id
	int _dungonEntranceRegionMapId;
	// 던전 입구 위치
	FVector _dungeonEntrancePos;
	// 던전 입구 spot id
	int64 _dungeonEntranceSpotId;
	// 던전 맵 id
	int _dungeonId;

	// 예약 데이터(팝업 워프)
	TSharedRef<FGsCostPackage> _reservedCostPackage = MakeShared<FGsCostPackage>();
	EGsWorldmapPopupWarpType _reservePopupWarpType = EGsWorldmapPopupWarpType::Bookmark;
	FString _reserveMoveTargetText;

	int _reserveSelectIndex;

	int _reserveMapId;
	int64 _reserveSpotId;

	FVector _reserveSharePos;	
	ItemDBId _reserveItemDBId;
	int _reserveChannelId;

	CreatureId _reserveNpcId;
	int _reserveSpawnGroupId;

	EGsEventProgressType _currentProgressType;

	bool _testShowRegionMapTouchColor = false;
	// 텍스쳐로 pick 이동 할지(테스트 on 처리용)
	bool _testPickMoveTexture = false;

	// back up data sel monster list(back key open monster list issue)
	int _backupSelMonsterListIndex = -1;

	bool _isUIRegionMapConstruct = false;

	bool _isUseMonsterSelectIndex = false;

	// backup data(server send check)
	WorldMapFilterInfo _oldRegionMapFilterServerOption;

	int _currentSpaceCrackListMapId = 0;
	// 던전 메뉴 보여줄건지(획득처에서 갈때 사용)
	bool _isShowDungeonMenu = false;

	// regoin map: invade info
private:
	TMap<EGsRegionInvadeNPCType, FGsRegionInvadeIconInfo*> _mapRegionInvadeIconInfo;

	// 파티 보물몬스터 정보 - 맵에 표시
protected:
	TMap<int64, FGsTreasureMonsterMapInfo> _treasureMonsterMapInfo;

	// 현재 몬스터 드랍 아이콘 필터링이 적용 되었는지
	// 원래는 UGsUIRegionScen에 있던 변수인데 다른데서도 사용해서 매니저단으로 옮김
	bool _isMonsterDropIconFiltered = false;

	// 몬스터 정보 팝업
private:
	// 선택된 몬스터 id
	int _selectInfoPopupMonsterId;
	// 선택된 몬스터 레벨
	int _selectInfoPopupMonsterLevel;

	// 가상 함수
public:
	// 초기화
	virtual void Initialize()override;
	// 정리
	virtual void Finalize()override;

	// 이벤트 시작
	virtual void StartEvent(EGsEventProgressType In_startType, EGsEventProgressType In_preType) override;
	// 이벤트 종료
	virtual void FinishEvent(EGsEventProgressType In_finishType, EGsEventProgressType In_nextType) override;

	// 로직 함수
public:
	// 테이블 데이터 가공
	void ProcessTableData();
	// 추가로 사용할 데이터 가공
	void InitializeDatas();
	// 포털 스팟 정보 구하기
	FGsRegionInfo* FindRegionInfo(int In_mapId);
	// 포털 스팟 정보 저장하기
	void SetRegionInfo(int In_mapId, FGsRegionInfo* In_info);

	// 지역 맵 데이터 로드
	void LoadRegionData(int In_mapId);

	// 포털 워프 시도
	void TryPortalWarp(int In_index);
	// 침공 포털 워프 시도
	void TryInvasionPortalWarp(int64 In_spotId);
	// 몬스터 스폰 위치 워프 시도
	void TryMonsterWarp(MapId In_mapId, CreatureId In_npcId, SpawnGroupId In_spawnGroupId);
	// 현재 지역맵 id로 상위 맵 table id 구하기
	int FindContinentalTblIdByCurrentRegionId();
	// get dungeon group tbl id by current region id
	int FindDungeonGroupTblIdByCurrentRegionId();
	// get dungeon group tbl id by map id
	int FindDungeonGroupTblIdByMapId(int In_mapId);
	// 맵 id로 던전 그룹 테이블 접근
	const FGsSchemaDungeonGroupData* FindDungeonGroupTblByMapId(int In_mapId);

	// MapId로 ContinentId 구하기
	ContinentId FindContinentalIdBy(const MapId InMapId) const;
	// 현재 지역맵 id로 상위 맵 인덱스 구하기(지역맵에서 대륙맵 갈때 사용)
	int FindContinentalIndexByCurrentRegionId();
	// get group info by current region map id
	FGsRegionGroupInfoBase* FindGroupInfoByCurrentRegionId();
	// 대상 맵이랑 같은지
	bool IsTargetMapSame(EGsWorldMapChangeType In_changeType);
	FGsMovementHandlerLocalPlayer* GetMovementHandlerLocalPlayer();
	// 자동 이동 표시 가능한 상태인가
	bool IsUseableShowAutoMovePos();
	// 로컬 포인터 저장
	void SetCharacter(UGsGameObjectLocalPlayer* In_player);
	// 로컬 포인터 참조 제거
	void RemoveCharacter();

	// 워프 예약(지역 아이템)
	void ReserveWarpRegionItem(
		EGsWorldmapPopupWarpType In_type, 
		FString In_moveTargetText,
		TSharedRef<FGsCostPackage> InCostPackage,
		int In_selectIndex);

	// 워프 예약(침공 포털 아이콘)
	void ReserveWarpInvasionPortal(
		EGsWorldmapPopupWarpType In_type,
		FString In_moveTargetText,
		TSharedRef<FGsCostPackage> InCostPackage,
		int64 In_spotId);

	// 워프 예약(지역 아이템)
	void ReserveWarpDungeonSide(
		FString In_moveTargetText,
		TSharedRef<FGsCostPackage> InCostPackage,
		int In_mapId, int64 In_spotId);

	// 워프 예약(북마크 공유)
	void ReserveWarpBookmarkShare(
		FString In_moveTargetText,
		TSharedRef<FGsCostPackage> InCostPackage,
		int In_mapId, FVector In_pos, ItemDBId In_itemDBId, int In_channelId);

	// 워프 예약(지역 아이템)
	void ReserveWarpBookmarkItem(
		FString In_moveTargetText,
		TSharedRef<FGsCostPackage> InCostPackage,
		int In_selectIndex);

	// 워프 예약(파티 리더 따라가기)
	void ReserveWarpMoveToPartyLeader(
		FString In_moveTargetText,
		TSharedRef<FGsCostPackage> InCostPackage);

	// 워프 예약(몬스터 드랍)
	void ReserveWarpRegionMonster(
		EGsWorldmapPopupWarpType In_type,
		FString In_moveTargetText,
		TSharedRef<FGsCostPackage> InCostPackage,
		MapId In_mapId, CreatureId In_npcId, SpawnGroupId In_spawnGroupId);

	void SaveServerOptionRegionMapFilterInfo();
	
	void ClearRegionInvadeIconInfo();

private:
	/**
	 * 현재 예약된 비용 정보를 반환하고, 예약 정보를 클리어함.
	 * 예약된 비용 정보가 없어도 크래시를 방지하기 위해 에러 출력 후 Gold 0원 비용을 임의 생성하여 반환함.
	 */
	TSharedRef<FGsCostPackage> PopValidReservedCostPackage();

	// 메시지
public:
	// 로컬 스폰 끝
	void OnLocalPlayerSpawnComplete(const IGsMessageParam* In_mapIdParam);
	// load to po start(level load server call)
	void OnLoadTopoStart(const IGsMessageParam* inParam);
	// invasion finish
	void OnInvasionFinish(const IGsMessageParam* );

	// get, set
public:
	// 대륙 갯수 구하기
	int GetContinentalCount() 
	{ 
		return _mapContinentalInfo.Num(); 
	}
	// get space crack info by continental id
	FGsRegionGroupInfoBase* GetSpaceCrackGroupData(int In_condinentalId);

	// 인덱스(리스트 순서)로 맵 id 구하기
	int GetContinentalMapId(int In_index);
	// 인덱스로 대륙맵 데이터 구하기
	const FGsSchemaContinentalMapData* FindContinentalMapDataByIndex(int In_index);
	// 인덱스로 대륙맵 안인지 체크하기
	bool IsLocalInContinentalMapByIndex(int In_index);
	// 로컬이 보여지는 지역맵에 있는지
	bool IsLocalInShowRegionMap();

	void SetShowRegionMapId(int In_mapId )
	{
		_showRegionMapId = In_mapId;
	}
	int GetShowRegionMapId()
	{
		return _showRegionMapId;
	}
	// 현재 보여지는 맵테이블타입
	MapType GetShowRegionMapTableType();

	// 현재 보고있는 지역맵의 그룹 타입이 뭔가
	EGsWorldMapGroupType GetShowRegionMapGroupType();
	// 보여줄 맵 타입 세팅
	void SetShowRegionMapType(EGsRegionMapType In_type);
	EGsRegionMapType GetShowRegionMapType()
	{
		return _showRegionMapType;
	}

	FText GetPortalName(int In_index);
	void SetSceneImageRate(float In_rate)
	{
		_uiSceneImageRate = In_rate;
	}
	// 실제 텍스쳐 사이즈/ bp내의 이미지 사이즈 비율
	void SetTextureRate(float In_rate)
	{
		_uiTextureRate = In_rate;
	}
	void SetUIShiftMapPos(const FVector2D& In_val)
	{
		_uiShiftMapPos = In_val;
	}


	// ui 위치 구하기
	FVector2D GetUIPos(const FVector2D& In_objPos);
	// uiscene image pos -> real pos
	FVector2D GetRealPosFromUILocalPos(const FVector2D& In_uiPos);

	// ui 위치 -> 텍스쳐 위치
	FVector2D GetTexturePosFromRealUiPos(const FVector2D& In_uiPos);

	// 최근 지역 정보 접근
	void SetCurrentRegionInfo(FGsRegionInfo* In_currentInfo) 
	{ 
		_currentRegionInfo = In_currentInfo; 
	}
	// 최근 지역 정보 접근
	FGsRegionInfo* GetCurrentRegionInfo() { return _currentRegionInfo; }
	// 최근 지역의 몬스터 정보 얻기(ui 인덱스로)
	FGsRegionMonsterInfo* GetCurrentRegionMonsterInfoByIndex(int In_index);
	// 최근 지역의 포털 정보 얻기(인덱스로)
	FGsPortalInfo* GetCurrentRegionPortalInfoByIndex(int In_index);
	// 자동 이동 목적지 위치 구하기
	FVector GetAutoMovePos();

	// 던전 입구 저장
	void SetDungeonEntranceInfo(int In_dungeonId, int In_mapId, const FVector& In_pos, int64 In_spotId)
	{
		_dungeonId = In_dungeonId;
		_dungonEntranceRegionMapId = In_mapId;
		_dungeonEntrancePos = In_pos;
		_dungeonEntranceSpotId = In_spotId;
	}

	int GetDungeonEntranceMapId() { return _dungonEntranceRegionMapId; }
	const FVector& GetDungeonEntrancePos() { return _dungeonEntrancePos; }
	int64 GetDungeonEntranceSpotId() { return _dungeonEntranceSpotId; }
	int GetDungeonId() { return _dungeonId; }

	// 선택 id
	void SetBookmarkSelectId(uint64 In_bookMarkId)
	{
		_selBookMarkId = In_bookMarkId;
	}
	uint64 GetBookmarkSelectId()
	{
		return _selBookMarkId;
	}

	EGsEventProgressType GetCurrentProgressType()
	{
		return _currentProgressType;
	}

	// Continental 이름 반환
	FText GetContinentalName(const ContinentId InContinentId) const;

	bool GetTestShowRegionMapTouchColor()
	{
		return _testShowRegionMapTouchColor;
	}
	void SetTestShowRegionMapTouchColor(bool In_val)
	{
		_testShowRegionMapTouchColor = In_val;
	}
	// 테스트로 지도찍고 이동 텍스쳐로 할지(참조)
	bool GetTestPickMoveTexture()
	{
		return _testPickMoveTexture;
	}
	// 테스트로 지도찍고 이동 텍스쳐로 할지(세팅)
	void SetTestPickMoveTexture(bool In_val)
	{
		_testPickMoveTexture = In_val;
	}

	int GetBackupSelMonsterListIndex()
	{
		return _backupSelMonsterListIndex;
	}

	void SetBackupSelMonsterListIndex(int In_index)
	{
		_backupSelMonsterListIndex = In_index;
	}

	void SetIsUIRegionMapConstruct(bool In_val)
	{
		_isUIRegionMapConstruct = In_val;
	}

	bool GetRegionMapIsBackkeyOpen();

	bool IsChangedServerOptionRegionMapFilterInfo();

	void SetIsUseMonsterSelectIndex(bool In_val)
	{
		_isUseMonsterSelectIndex = In_val;
	}
	bool GetIsUseMonsterSelectIndex()
	{
		return _isUseMonsterSelectIndex;
	}
	int GetContinentalOrderCount()
	{
		return _arrContinentalOrder.Num();
	}

	void SetArraySpaceCrackSortInfo(TArray<FGsContinentalSpaceCrackMapInfo*> In_info)
	{
		_arraySpaceCrackSortInfo = In_info;
	}

	FGsContinentalSpaceCrackMapInfo* FindSpaceCrackSortInfoByIndex(int In_index);
	TArray<FGsContinentalSpaceCrackMapInfo*> GetSpaceCrackSortInfo()
	{
		return _arraySpaceCrackSortInfo;
	}

	const FGsSchemaSpaceCrackData* FindSpaceCrackData(int In_mapId);

	void SetCurrentSpaceCrackListMapId(int In_continentalMapId)
	{
		_currentSpaceCrackListMapId = In_continentalMapId;
	}

	int GetCurrentSpaceCrackListMapId()
	{
		return _currentSpaceCrackListMapId;
	}

	void SetRegionInvadeInfoByServerPacket(PD::SC::PKT_SC_INVADE_MINIMAP_INFO_UPDATE_READ* In_serverInvaeInfo);

	EGsRegionInvadeNPCNewInfoResponse SetRegionInvadeInfo(EGsRegionInvadeNPCType In_type, int64 In_gameId, FVector& In_pos);

	TMap<EGsRegionInvadeNPCType, FGsRegionInvadeIconInfo*> GetRegionInvadeInfo()
	{
		return _mapRegionInvadeIconInfo;
	}

	FGsRegionInvadeIconInfo* GetRegionInvadeInfoByNPCType(EGsRegionInvadeNPCType In_NPCType);
	// https://jira.com2us.com/jira/browse/C2URWQ-4984
	// 대륙맵에 던전 입구로 이동할때 세팅
	// 던전 획득저에서 사용
	void SetDungeonSideShow(int In_mapId);
	// https://jira.com2us.com/jira/browse/C2URWQ-4984
	// 던전 메뉴 보여 줄지
	void SetIsShowDungeonMenu(bool In_val)
	{
		_isShowDungeonMenu = In_val;
	}
	// 던전 메뉴 보여 줄지
	bool GetIsShowDungeonMenu()
	{
		return _isShowDungeonMenu;
	}
	// 지역맵 game state 가져오기
	FGsGameStateRegionMap* GetGameStateRegionMap();

	// 현재 열린 맵의 지역맵 성소 정보 가져오기(성소 id로)
	FGsRegionMapSanctumInfo* GetCurrentRegionSanctumInfoBySanctumId(SanctumAreaId In_id);

	// 보물몬스터 맵, 위치 정보
public:
	void SetTreasureMonsterMapInfo(int64 seperator, MapId id, FVector pos, bool localPlayerOwn = false);
	void RemoveTreasureMonsterMapInfo(int64 seperator);

	// mapId를 넣어줄 때는 보물몬스터가 해당맵에 있을때 데이터
	const FGsTreasureMonsterMapInfo& GetTreasureMonsterMapInfo(int64 seperator, int mapId = 0);

	// 현재 보이는 맵에 있는 모든 보물 몬스터 정보
	int GetShowRegionTreasureMonsterInfo(TArray<FGsTreasureMonsterMapInfo>& infoList); 

	// 드랍 필터링중인지
	void SetIsMonsterDropIconFiltered(bool In_val)
	{ 
		_isMonsterDropIconFiltered = In_val;
	}
	bool GetIsMonsterDropIconFiltered()
	{
		return _isMonsterDropIconFiltered;
	}

	// 몬스터 정보 팝업
public:
	// 현재 몬스터 팝업에서 몬스터 id 정보
	void SetSelectInfoPopupMonsterId(int In_monsterId)
	{
		_selectInfoPopupMonsterId = In_monsterId;
	}
	int GetSelectInfoPopupMonsterId()
	{
		return _selectInfoPopupMonsterId;
	}
	// 현재 몬스터 팝업에서 몬스터 레벨 정보
	void SetSelectInfoPopupMonsterLevel(int In_monsterLevel)
	{
		_selectInfoPopupMonsterLevel = In_monsterLevel;
	}
	int GetSelectInfoPopupMonsterLevel()
	{
		return _selectInfoPopupMonsterLevel;
	}
	// 인덱스로 몬스터 정보 세팅
	void SetSelectMonsterInfoByIndex(int In_selectIndex);
	// 현재 선택된 몬스터 레벨 인덱스로 구하기
	int GetCurrentSelectMonsterLevelByLevelIndex(int In_levelIndex);
	// 현재 선택된 몬스터(npc tbl id) + 선택된 레벨로 
	// 몬스터 인덱스(지역맵 정보 인덱스) 구하기
	int GetCurrentSelectMonsterIndexByLevelIndex(int In_levelIndex);
	// 현재 몬스터 id 기반 정보(몬스터 팝업에서 사용)
	FGsRegionMonsterSameIDInfo* GetCurrentMonsterSameIdInfo();
	// static
public:
	// Map 이름 반환
	static FText GetMapName(const MapId InMapId);

	static MapType GetMapType(const MapId InMapId);
};

#define GSWorldMap() UGsScopeHolder::GetGameManagerFType<FGsWorldMapManager>(UGsScopeGame::EManagerType::WorldMap)