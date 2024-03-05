#pragma once
#include "CoreMinimal.h"

#include "Map/GsMapIconType.h"
#include "Map/WorldMap/GsWorldMapDefine.h"
#include "Message/GsMessageGameObject.h"
#include "Message/GsMessageContentHud.h"
#include "Message/GsMessageUI.h"

#include "PKBook/GsPKBookStruct.h"

#include "Shared/Client/SharedEnums/SharedIffEnum.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"

#include "GsMinimapIconHandler.generated.h"


/*
    미니맵 아이콘 관련 처리
*/
class UGsMinimapIconBase;
class UGsWidgetPoolManager;
class UGsMinimapIconLocal;
class UGsGameObjectBase;
class UGsUIIconBase;
class UGsGameObjectLocalPlayer;

class FGsPartyMemberInfo;
class FGsRegionInvadeIconInfo;

struct IGsMessageParam;
struct FGsMinimapIconTeamMessageParam;
struct FGsSchemaMinimapIconData;


UCLASS()
class UGsMinimapIconHandler : public UObject
{
    GENERATED_BODY()

    // 멤버 변수
private:
	// 모든 아이콘
	// map은 UPROPERTY 적용이안되어서 
	// TArray으로  통합 관리
	// tmap은 검색용으로만 사용
	UPROPERTY()
	TArray<UGsMinimapIconBase*> _listSpawnIcons;

	// 아이콘 들
	TMap<int64, UGsMinimapIconBase*> _mapIcons;
	// 클라 스폰 아이콘
	TMap<int64, UGsMinimapIconBase*> _mapClientSpawnIcons;
	// 퀘스트 위치 아이콘
	// 퀘스트 위치정보는 스폰 되는게 아니라 FVector정보로만 존재해서
	// 키값이 스팟 id이다
	// 원래는 맵 + 스팟 id가 키값인데
	// 미니맵에서는 현재맵만 출력하므로 미니맵 퀘스트 위치 아이콘 키값은 스팟 id만 존재하면 된다
	TMap<int, UGsMinimapIconBase*> _mapQuestPosIcons;

	// 위치 즐겨 찾기 아이콘
	// 키값은 book mark db id이다
	TMap<uint64, UGsMinimapIconBase*> _mapBookmarkIcons;

	// 파티 아이콘
	TMap<int64, UGsMinimapIconBase*> _mapPartyIcons;

	// 시공의 틈새 포탈
	TMap<int64, UGsMinimapIconBase*> _mapSpaceCrackPortalIcons;

	// 2022/09/29 PKT - PK Book 관련 경계 대상 아이콘
	TMap<int64, UGsMinimapIconBase*> _mapPKDetectionTargetIcons;
	// invade npc icon
	TMap<EGsRegionInvadeNPCType, UGsMinimapIconBase*> _mapInvadeNPCIcons;
	// invasion portal icon
	TMap<int64, UGsMinimapIconBase*> _mapInvasionPortalIcons;
	// 월드맵 개선
	// 자동 이동 경로 표시 아이콘
	// id는 이동 순서
	// https://jira.com2us.com/jira/browse/CHR-23756
	TMap<int, UGsMinimapIconBase*> _mapAutoMovePathIcons;
	

private:
	// 아이콘 메시지 델리게이트
	MsgGameObjHandleArray _arrIconDelegate;

	//스폰 아이콘 테스트
	MsgGameObjHandle _delegateSpawnIconType;
	// 아이콘 팀변경
	MsgMinimapHandle _delegateIconTeamChange;
	// ui delegate msg
	MsgUIHandleArray _msgUIHandlerList;
	// 아이콘 풀매니저
	TWeakObjectPtr<UGsWidgetPoolManager> _iconPoolManager;
	// 로컬 아이콘
	UPROPERTY()
	UGsMinimapIconLocal* _iconLocal;
	// 일반 몬스터 보여줄지
	bool _isNormalMonsterOn = false;
	// 로컬 이전 위치
	FVector _oldLocalPos = FVector::ZeroVector;
	// 자동 이동 경로: 최근 인덱스
	int _currentAutoMovePathIndex = 0;
	// 자동 이동 경로: 최대 인덱스
	int _maxAutoMovePathIndex;
    // 소멸자
public:
    virtual ~UGsMinimapIconHandler();

    // 로직 함수
public:
    // 초기화
    void Initialize();
    // 정리
    void Finalize();
	// 업데이트
	void Update(float In_delta, bool In_isLocalMoved);

	void InitializeMessage();
	void FinalizeMessage();

	// icon spawn
public:
	// game id로 디스폰
	void DespawnIcon(int64 In_gameID);
	// 부모에게서 때기
	void ReleaseIcon(UGsMinimapIconBase* In_targetIcon);
	// 타입으로 아이콘 데이터 얻기
	const FGsSchemaMinimapIconData* FindMapIconData(EGsMapIconType In_type);

	// clear icon
public:
	// 모든 아이콘 삭제
	void ClearAllIcon();
	// 모든 오브젝트 아이콘 삭제
	void ClearObjIcon();	
	// 모든 클라 스폰 아이콘 삭제
	void ClearClientSpawnIcon();
	// 모든 퀘스트 위치 아이콘 삭제
	void ClearQuestPosIcon();
	// 모든 위치 즐겨찾기 아이콘 삭제
	void ClearBookmarkIcon();
	// 모든 파티 아이콘 삭제
	void ClearPartyIcon();
	// 모든 시공의 틈새 아이콘 삭제
	void ClearSpaceCrackPortalIcon();
	// 2022/09/29 PKT - PK Book 관련 경계 대상 아이콘 삭제
	void ClearPKDetectionTargetIcon();
	// clear all invasion npc icon
	void ClearInvasionNPCIcon();
	// clear all invasion portal
	void ClearInvasionPortal();
	// 자동 이동 경로 아이콘 삭제
	void ClearAutoMovePathIcon();

public:
	// 리모트 팀변경
	void ChangeRemoteTeam(UGsMinimapIconBase* In_icon);
	// 스폰 아이콘
	UGsMinimapIconBase* SpawnIcon(UGsGameObjectBase* In_target, EGsMapIconType In_iconType);
	// 클라이언트 스폰 아이콘
	void ClientSpawnIcon(int64 In_id, int In_tableId,EGsMapIconType In_iconType, FVector2D In_pos);
	// 클라이언트 디스폰 아이콘
	void ClientDespawnIcon(int64 In_id);

	// 퀘스트 위치 아이콘 생성
	void MakeQuestPosIcon(int In_spotId, FVector2D In_pos);
	// 퀘스트 위치 아이콘 삭제
	void DelQuestPosIcon(int In_spotId);

	// 북마크 아이콘 생성
	void MakeBookmarkIcon(uint64 In_bookmarkId, FVector2D In_pos, int In_mapId);
	// 북마크 아이콘 삭제
	void DelBookmarkIcon(uint64 In_bookmarkId);


	// 시공의 틈새 포털 아이콘 생성
	void MakeSpaceCrackPortal(int64 In_gameId, FVector2D In_pos);
	// 시공의 틈새 포털 아이콘 삭제
	void DelSpaceCrackPortal(int64 In_gameId);

	// 클라이언트 스폰 npc, 포털, bookmark 생성용
	UGsMinimapIconBase* MakeClientSpawnNpcIcon(int64 In_id, EGsMapIconType In_iconType, FVector2D In_pos,
		int In_tableId = -1);

	// 파티 스폰 아이콘
	void MakePartyMemberIcon(FGsPartyMemberInfo* In_partyInfo, EGsMapIconType In_iconType);
	// 파티 디스폰 아이콘
	void DelPartyMemberIcon(int64 In_id);

	// invasion npc(server info)
public:
	// maske invasion npc icon(guardian, monster, wagon)
	void MakeInvasionNPCIcon(FGsRegionInvadeIconInfo* In_iconInfo, EGsMapIconType In_iconType);
	// delete invasion npc icon (guardian, monster, wagon)
	void DelInvasionNPCIcon(EGsRegionInvadeNPCType In_npcType);

	// invasion portal
public:
	void MakeInvasionPortal(int64 In_spotId, FVector2D In_pos);
	void DelInvasionPortal(int64 In_spotId);

	 // 자동 이동 경로 아이콘 한개씩 생성, 삭제
public:
	void MakeAutoMovePathIcon(int In_index, FVector2D In_pos, float In_deleteDist);
	void DelAutoMovePathIcon(int In_index);

	// array
	// quest monster
public:
	// 퀘스트 몬스터 추가
	void AddQuestMonster(TArray<int> In_arrId);
	// 퀘스트 몬스터 삭제
	void DelQuestMonster(TArray<int> In_arrId);
	// 퀘스트 npc 추가
	void AddQuestNpc(TArray<int> In_arrId, EGsMapIconType In_iconType);
	// 퀘스트 npc 삭제
	void DelQuestNpc(TArray<int> In_arrId, EGsMapIconType In_iconType);

	// sub quest
public:
	// 서브 퀘스트 npc 추가
	// 기존엔 AddQuestNpc로 같이 썼는데
	// (game object 스폰, 디스폰시 아이콘 생성, 삭제)
	// 서브 퀘스트 아이콘이 멀리서도 보여야 한다고 해서
	// client spawn 처럼 맵 진입시 스폰, 맵이동시 디스폰, 퀘스트 갱신시 스폰, 디스폰
	// 처리로 바꿈
	void AddSubQuestNpc(TArray<int> In_arrId, EGsMapIconType In_iconType);
	// 서브 퀘스트 npc 삭제
	void DelSubQuestNpc(TArray<int> In_arrId, EGsMapIconType In_iconType);

	// quest pos
public:
	// 퀘스트 위치 추가
	void AddQuestPos(const TArray<int>& In_arrData);
	// 퀘스트 위치 삭제
	void DelQuestPos(const TArray<int>& In_arrData);

	// party
public:
	// 파티 멤버 추가
	void AddPartyMember(const TArray<int64>& In_arrId);
	// 파티 멤버 삭제
	void DelPartyMember(const TArray<int64>& In_arrId);

	// PKDetectionTarget
public:
	// 2022/09/29 PKT - 경계 대상 추가 / 삭제
	void AddPKDetectionTargetIcon(UserDBId InId, int64 InGameId, const TArray<FGsPKDetectionEffectInfo>& InEffectInfoSet);
	void DelPKDetectionTargetIcon(int64 InGameId);

	// invasion npc
public:
	void AddInvasionNPC(const TArray<EGsRegionInvadeNPCType> In_arrayType);
	void DelInvasionNPC(const TArray<EGsRegionInvadeNPCType> In_arrayType);

	// 자동 이동 경로 배열로 생성, 삭제
public:
	void AddAllAutoMovePath(TArray<FVector> In_arrayPos);
	// 경로 path 추가로 만들기
	TArray<FVector2D> MakeAddAutoMovePath(TArray<FVector> In_arrayPos);
	// 특정 거리만큼 띄워서 거리 채움
	void AddDistMovePath(FVector2D In_startPos, FVector2D In_endPos, float In_minimapAddAutoPathDist,
		TArray<FVector2D>& Out_arrayTotalPath, FVector2D& Out_lastPos);
	// 이미 지나간 경로이거나 도착하면 삭제하는 처리
	void InvalidateAutoMovePath();

public:
	UGsUIIconBase* CreateMapIcon(EGsMapIconType InType);

	// client spawn list load finish
	// total make icon
public:
	// 북마크 데이터 로드하기
	void LoadBookmarkData();
	// 모든 시공의 틈 포탈 아이콘 만들기(현재맵)
	void MakeAllSpaceCrackPortalIcons();
	// all invasion npc icons
	void MakeAllInvasionNPCIcons();
	// make all invasion portal
	void MakeIconInvasionPortal();

	// 이벤트
public:
	void OnSpawnIcon(const IGsMessageParam* In_param);
	void OnDespawnIcon(const IGsMessageParam* In_param);
	void OnDespawnAllIcon(const IGsMessageParam*);
	void OnDespawnObjAllIcon(const IGsMessageParam*);
	void OnSpawnIconType(int32 In_type);
	void OnIconTeamChange(const FGsMinimapIconTeamMessageParam* In_param);
	// 일반 몬스터 on, off
	void OnNormalMonsterOnOff(const IGsMessageParam* In_param);
	// 클라 스폰 리스트 로드 끝
	void OnClientSpawnListLoadFinish(const IGsMessageParam* In_param);
	// 퀘스트 타겟(npc)
	void OnQuestTargetNpc(const IGsMessageParam* In_param);
	// 퀘스트 타겟(몬스터)
	void OnQuestTargetMonster(const IGsMessageParam* In_param);
	// 퀘스트 서브 인터랙션 npc
	void OnQuestSubInterationNpc(const IGsMessageParam* In_param);
	// 퀘스트 서브 인터랙션 npc List
	void OnQuestSubInterationNpcList(const IGsMessageParam* In_param);
	// 퀘스트 타겟(위치)
	void OnQuestTargetPos(const IGsMessageParam* In_param);
	// 파티 멤버 (추가, 삭제)
	void OnPartyMember(const IGsMessageParam* In_param);
	// 시공의 틈새 포탈(추가, 삭제)
	void OnSpaceCrackPortal(const IGsMessageParam* In_param);
	// invasion npc (add, del)
	void OnInvasionNPC(const IGsMessageParam* In_param);
	// abnormality update
	void OnAbnormalityAllUpdate(const IGsMessageParam* In_param);

	void OnUpdateInvasionPortalActive(const IGsMessageParam* InParam);
	void OnUpdateInvasionPortalSpawnInfo(const IGsMessageParam* InParam);
	// 자동 이동 경로 아이콘 정보
	void OnAutoMovePathIconInfo(const IGsMessageParam* InParam);
	// auto move state 종료
	void OnLocalAutoMoveStop(const IGsMessageParam*);

	// get, set
public:
	// 일반 몬스터 켰는지
	bool GetIsNormalMonsterOn() { return _isNormalMonsterOn; }
	// 만들수 있는 몬스터 아이콘인가
	bool CanMakeMonsterIcon(EGsMapIconType In_type);
private:
	const FGsSchemaMinimapIconData* GetTableData(EGsMapIconType InType) const;

	// check abnormality hide obj
	bool IsHideObj(UGsGameObjectBase* In_obj);
};