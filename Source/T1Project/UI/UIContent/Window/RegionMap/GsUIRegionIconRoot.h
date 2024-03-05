#pragma once
#include "CoreMinimal.h"

#include "UMG/Public/Blueprint/UserWidget.h"
#include "SlateCore/Public/Layout/Geometry.h"

#include "Map/WorldMap/GsWorldMapDefine.h"
#include "Map/GsMapIconType.h"

#include "Message/GsMessageGameObject.h"
#include "Message/GsMessageBoss.h"

#include "Message/GsMessageUI.h"

#include "Shared/Shared/SharedInclude/SharedTypes.h"

#include "GsUIRegionIconRoot.generated.h"

/*
	지역맵 아이콘 관리
*/

class UCanvasPanel;

class UGsUIIconBase;
class UGsUIRegionIconMenu;

class UGsUIIconRegionMapInvasion;

class UGsRegionMapIconBase;
class UGsRegionMapIconObject;
class UGsRegionMapIconFixed;
class UGsRegionMapIconSelect;
class UGsRegionMapIconSelectMonster;
class UGsRegionMapIconSpot;
class UGsRegionMapIconFieldBoss;
class UGsRegionMapIconSelectSanctum;

class UGsGameObjectBase;
class UGsUIRegionTouchMenu;

class FGsRegionInvadeIconInfo;
class FGsPartyMemberInfo;
class FGsRegionMapSanctumInfo;

struct FGsSanctumData;

struct IGsMessageParam;

UCLASS()
class T1PROJECT_API UGsUIRegionIconRoot : public UUserWidget
{
	GENERATED_BODY()
protected:

	// 붙이는곳
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UCanvasPanel* _iconRoot;
	// menu to be displayed when the portal icon is clicked
	// 포탈/몬스터 드랍 아이템 획득처 클릭시 이동/ 워프 메뉴
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUIRegionIconMenu* _iconMenu;
	// menu displayed when touching the moveable terrain
	// 땅찍고 이동시 자동 이동 메뉴
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUIRegionTouchMenu* _touchMenu;
	// 포탈/몬스터 드랍 아이템 획득처 클릭시 이동/ 워프 메뉴
	// 버튼이 아래로 나오는 버전(우상단 콤보 박스랑 켭치는 이슈)
	//https://jira.com2us.com/jira/browse/C2URWQ-7362
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUIRegionIconMenu* _iconMenuDown;
	// 버튼이 아래로 나오는 버전(우상단 콤보 박스랑 켭치는 이슈)
	//https://jira.com2us.com/jira/browse/C2URWQ-7362
	// 땅찍고 이동시 자동 이동 메뉴
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUIRegionTouchMenu* _touchMenuDown;

	// 모든 아이콘들
	UPROPERTY()
	TArray<UGsRegionMapIconBase*> _listRegionIcons;
	// 서버 스폰(나, 파티)
	TMap<int64, UGsRegionMapIconBase*> _mapServerSpawnIcons;
	// 파티 프로토콜(시야 밖)
	TMap<int64, UGsRegionMapIconBase*> _mapPartyIcons;

	// 즐겨찾기 아이콘
	TMap<uint64, UGsRegionMapIconSelect*> _mapBookmarkIcons;

	// 포털 아이콘
	// key: spot id, value: 선택 아이콘
	// (선택 표시 가능, 좌측리스트 연동, 이름 표시)
	TMap<int64, UGsRegionMapIconSelect*> _mapPortalIcons;

	// 목적지(자동 이동)
	UGsRegionMapIconSelect* _autoMovePosIcon;

	// 몬스터 스폰 위치 아이콘
	TMap<int64, UGsRegionMapIconSelect*> _mapMonsterDropIcons;

	// 텔레포트 아이콘
	// key: spot id, value: spot 아이콘
	// (이름 표시)
	TMap<int64, UGsRegionMapIconSpot*> _mapTeleportIcons;

	// 시공의 틈새 아이콘
	TMap<int64, UGsRegionMapIconBase*> _mapSpaceCrackPortalIcons;

	// 필드 보스 아이콘
	TMap<CreatureId, UGsRegionMapIconFieldBoss*> _mapFieldBossIcons;

	// 파티 보물 몬스터 아이콘
	//UGsRegionMapIconBase* _myTreasureMonsterIcon{ nullptr };
	TMap<int64, UGsRegionMapIconBase*> _mapPartyTreasureMonsterIcons;

	// 침공 포털 아이콘
	TMap<int64, UGsRegionMapIconBase*> _mapInvasionPortalIcons;
	// invade npc icons
	TMap<EGsRegionInvadeNPCType, UGsRegionMapIconBase*> _mapInvadeNPCIcons;
	// 성소 아이콘들
	TMap<int32, UGsRegionMapIconBase*> _mapSanctumIcons;

	// gameobject 메시지 처리용
	MsgGameObjHandleArray _arrIconDelegate;
	
	MsgBossHandleArray _msgBossHandleList;

	MsgUIHandleArray _msgUIHandlerList;

	// 로컬 오브젝트
	UGsGameObjectBase* _localObject = nullptr;

	// 로컬 이전 위치
	FVector _oldLocalPos = FVector::ZeroVector;
	// 자동 이동 목적지 선택 dirty 인지
	bool _isDirtyAutoMovePosSel = false;
	int64 _localGameId;

	// 몬스터 드랍 아이콘 최대 갯수
	int32 _monsterIconCount = 3;

private:
	TFunction<void(UGsRegionMapIconSelect*)> _callbackClickPortalIcon;
	TFunction<void(UGsRegionMapIconSelectMonster*, FVector)> _callbackClickMonsterDropIcon;

	TFunction<void(UGsUIIconRegionMapInvasion*, int64, FVector, FString)> _callbackClickInvasionPortalIcon;
	// 성소 클릭 콜백
	TFunction<void(UGsRegionMapIconSelectSanctum*)> _callbackClickSanctum;
	
protected:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	// 이벤트
public:
	void OnSpawnServerIcon(const IGsMessageParam* In_param);
	void OnDespawnServerIcon(const IGsMessageParam* In_param);
	// 파티 멤버 (추가, 삭제)
	void OnPartyMember(const IGsMessageParam* In_param);
	// 시공의 틈새 포탈(추가, 삭제)
	void OnSpaceCrackPortal(const IGsMessageParam* In_param);

	// 필드 보스 메시지
	void OnFieldBossSpawn(const IGsMessageParam* InParam);
	void OnFieldBossDespawn(const IGsMessageParam* InParam);
	void OnFieldBossDie(const IGsMessageParam* InParam);
	void OnAddFieldBossInfo(const IGsMessageParam* InParam);
	void OnDelFieldBossInfo(const IGsMessageParam* InParam);

	// 로컬플레이어 보물 몬스터 추가 삭제
	void OnTreasureMonsterSpawn(const IGsMessageParam* InParam);
	void OnUpdateTreasureMonsterInfo(const IGsMessageParam* InParam);

	// 침공포털 spawn
	void OnInvasioinPortalSpawn(const IGsMessageParam* InParam);
	void OnUpdateInvasionPortalActive(const IGsMessageParam* InParam);
	void OnUpdateInvasionPortalSpawnInfo(const IGsMessageParam* InParam);

	void OnChangeRegionIconFiltering(const IGsMessageParam* InParam);

	void OnInvasionNPC(const IGsMessageParam* In_param);

	// 몬스터 스폰 위치 아이콘
	void OnMonsterDropSpawn(const IGsMessageParam* In_param);
	// 성소 정보 갱신됨
	void OnUpdatedSanctumInfoUpdated(const IGsMessageParam*);

	// 로직 함수
public:
	void InitializeMessage();
	void FinalizeMessage();

	// 모든 아이콘 생성
	void MakeAllIcon();
	// 모든 아이콘 삭제
	void ClearAllIcon();
	// 파티 아이콘 만들기
	void MakeIconParty();
	// 클라이언트 스폰 아이콘 만들기
	void MakeIconClientSpawn();
	// 목적지 아이콘 만들기
	void MakeIconAutoMovePos();
	// 즐겨찾기 아이콘 만들기
	void MakeIconBookmark();
	// 시공의 틈새 포탈 아이콘 만들기
	void MakeIconSpaceCrackPortal();
	// 필드 보스 아이콘 만들기
	void MakeIconFieldBoss();
	// 내 보물 몬스터 아이콘 만들기
	void MakeIconTreasureMonster();
	// 침공 포털 아이콘 만들기
	void MakeIconInvasionPortal();
	// invasion NPC(Guardian, Monster, Wagon)
	void MakeIconInvasionNPC();

	void MonsterIconVisibleToggle(bool In_isVisible);
	// 성소 아이콘 만들기
	void MakeIconSanctum();

	// spawn, despawn func
public:
	// 서버 스폰 아이콘 만들기
	UGsRegionMapIconObject* SpawnIconServer(UGsGameObjectBase* In_target, EGsRegionMapIconType In_iconType);
	// 서버 스폰 아이콘 삭제
	void DespawnIconServer(int64 In_gameId);
	// 파티 정보로 아이콘 만들기(시야내에 파티 없을때)
	void SpawnIconParty(FGsPartyMemberInfo* In_partyInfo, EGsRegionMapIconType In_iconType);
	// 파티 정보로 만든 아이콘 삭제
	void DespawnIconParty(int64 In_gameId);

	// 텔레포트 아이콘 만들기(스팟)
	void SpawnIconTeleport(int64 In_spotId, FVector In_pos, EGsRegionMapIconType In_iconType,
		FText In_name);
	// 텔레포트 아이콘 삭제
	void DespawnIconTeleport(int64 In_spotId);


	// 포탈 아이콘 만들기(선택 처리)
	void SpawnIconPortal(int64 In_spotId, FVector In_pos, EGsRegionMapIconType In_iconType,
		FText In_name, int In_selectIndex);
	// 포탈 아이콘 삭제(선택 처리)
	void DespawnIconPortal(int64 In_spotId);


	// 즐겨찾기 아이콘 만들기
	void SpawnIconBookmark(uint64 In_bookmarkId, FVector In_pos, EGsRegionMapIconType In_iconType);
	// 즐겨찾기 아이콘 삭제
	void DespawnIconBookmark(uint64 In_bookmarkId);


	// 목적지 아이콘 만들기
	void SpawnIconAutoMovePos(FVector In_pos, EGsRegionMapIconType In_iconType);
	// 목적지 아이콘 삭제
	void DespawnIconAutoMovePos();


	// 시공의 틈새 포탈 아이콘 만들기
	void SpawnIconSpaceCrackPortal(int64 In_gameId, const FVector& In_pos, EGsRegionMapIconType In_iconType);
	// 시공의 틈새 포탈 아이콘 삭제
	void DespawnIconSpaceCrackPortal(int64 In_gameId);

	// 필드 보스 아이콘 만들기
	void SpawnIconFieldBoss(CreatureId In_creatureId, const FVector& In_pos, EGsRegionMapIconType In_iconType,
		FDateTime In_spawnTime,
		bool In_isHaveSpawnTime);
	// 필드 보스 아이콘 삭제
	void DespawnIconFieldBoss(CreatureId In_creatureId);

	// 파티 보물 몬스터 아이콘 만들기
	void SpawnIconTreasureMonster(int64 In_gameId, const FVector& In_pos, EGsRegionMapIconType In_iconType);
	void DespawnIconTreasureMonster(int64 In_gameId);

	// 포탈 아이콘 만들기(선택 처리)
	void SpawnIconInvasionPortal(int64 InSpotId, FVector InPos, EGsRegionMapIconType InIconType = EGsRegionMapIconType::InvasionPortal);
	// 포탈 아이콘 삭제(선택 처리)
	void DespawnIconInvasionPortal(int64 InSpotId);

	// 몬스터 스폰 위치 아이콘 만들기
	void SpawnIconMonsterDrop(int64 InSpotId, int32 InMapId, CreatureId InNpcId, int32 InSpawnGroupId, FVector InPos, EGsRegionMapIconType InIconType, int InSelectIndex);
	// 몬스터 스폰 위치 아이콘 삭제
	void DespawnIconMonsterDrop(int64 InSpotId);

	// 성소 아이콘 만들기
	void SpawnIconSanctum(const FGsSanctumData* In_sanctumData, FGsRegionMapSanctumInfo* In_regionSanctumInfo);
	// 성소 아이콘 삭제
	void DespawnIconSanctum(int32 In_sanctumAreaId);

	// invasion npc
public:
	// spawn invasion npc
	void SpawnIconInvasionNPC(FGsRegionInvadeIconInfo* In_iconInfo, EGsRegionMapIconType In_iconType);
	// despawn invasion npc
	void DepspawnIconInvasionNPC(EGsRegionInvadeNPCType In_NPCType);


	UGsUIIconBase* CreateMapIcon(EGsRegionMapIconType In_type, bool In_useVisibleCheck = false);
	// 북마크 아이콘 선택
	void SelectBookmarkIcon(
		bool In_isSelect, uint64 In_bookmarkId = 0);
	// 포탈 아이콘 선택
	void SelectPortalIcon(bool In_isSelect, int64 In_spotId = 0);
	// 자동 이동 목표 위치 아이콘 선택
	void SelectAutoMoveTargetPosIcon(bool In_isSelect);
	// 자동 이동 목표 위치 아이콘 선택
	void SelectMonsterDropIcon(bool In_isSelect, int64 In_spotId = 0);

	// 아이콘 선택 공용
	// 1. 아이콘 타입
	// 2. 선택 인지
	// 3. 즐겨찾기:uin64, 포탈: int64
	void SelectIcon(EGsRegionIconSelectType In_type, bool In_isSelect, uint64 In_id =0);
	// 파티 멤버 추가
	void AddPartyMember(const TArray<int64>& In_arrId);
	// 파티 멤버 삭제
	void DelPartyMember(const TArray<int64>& In_arrId);

	// add invasion npc 
	void AddInvasionNPC(const TArray<EGsRegionInvadeNPCType> In_arrayType);
	// delete invasion npc
	void DelInvasionNPC(const TArray<EGsRegionInvadeNPCType> In_arrayType);

	// add monster drop icon
	void AddMonsterDropIcon(MapId In_mapId, CreatureId In_npcId, const TArray<SpawnGroupId>& In_SpawnGroupIdArray, const TArray<FVector>& In_posArray);
		
	// show icon menu
	void ShowIconMenu(UGsRegionMapIconSelect* In_clickedIconObj);
	// 아이콘 메뉴 출력(침공 포탈)
	void ShowInvasionIconMenu(UGsUIIconRegionMapInvasion* In_icon, int64 In_spotId, FVector In_pos, FString In_text);
	// 아이콘 메뉴 show: 성소 아이콘 클릭으로
	void ShowSanctumIconMenu(UGsRegionMapIconSelectSanctum* In_clickedIconObj);
	// 성소 id로 아이콘 메뉴 show
	void ShowSanctumIconMenuBySanctumId(SanctumAreaId In_id);

	// hide icon menu
	void HideIconMenu();
	void ShowIconMenuByPortalIndex(int64 inIndex);

	void ShowIconMenuByBookmarkIndex(int64 inIndex);
	// 몬스터 드롭위치에 아이콘 메뉴 열기
	void ShowMonsterDropIconMenu(UGsRegionMapIconSelectMonster* In_iconObj, FVector In_pos);

	void HideTouchMenu();

public:
	void UnselectAllPortal();
	void UnselectAllBookmark();
	// 성소 모든 아이콘 비선택으로 변경
	void UnselectAllSanctum();

	// event call 
public:
	void OnClickPortalIcon(UGsRegionMapIconSelect* In_clickedIconObj);
	// 침공전 포털 아이콘 클릭
	void OnClickInvasionIcon(UGsUIIconRegionMapInvasion* In_invasionIcon, 
		int64 In_spotId, FVector In_pos, FString In_portalName);

	void OnClickMonsterDropIcon(UGsRegionMapIconSelectMonster* In_clickedIconObj, FVector In_pos);

	void OnClickMoveableTerrain(FVector In_pos);

	void OnClickFieldBossIcon(CreatureId In_creatureId);
	// 성소 클릭
	void OnClickSanctum(UGsRegionMapIconSelectSanctum* In_clickSanctumObj);

	// get, set
public:
	UGsGameObjectBase* GetLocal();

	void SetDirtyAutoMovePosSel(bool In_sel)
	{
		_isDirtyAutoMovePosSel = In_sel;
	}

	void SetCallbackClickPortalIcon(TFunction<void(UGsRegionMapIconSelect*)> In_callback)
	{
		_callbackClickPortalIcon = In_callback;
	}

	void SetCallbackClickInvasionPortalIcon(
		TFunction<void(UGsUIIconRegionMapInvasion*, int64, FVector, FString)> In_callback)
	{
		_callbackClickInvasionPortalIcon = In_callback;
	}

	void SetCallbackClickMonsterDropIcon(TFunction<void(UGsRegionMapIconSelectMonster*, FVector)> In_callback)
	{
		_callbackClickMonsterDropIcon = In_callback;
	}

	void SetCallbackClickSanctum(TFunction<void(UGsRegionMapIconSelectSanctum*)> In_callback)
	{
		_callbackClickSanctum = In_callback;
	}
	
	// 현재 아이콘 위치로 아이콘 메뉴(이동, 워프) 가져오기(위/아래버전 선택)
	UGsUIRegionIconMenu* GetNowIconMenu(FVector2D In_iconPos);

	// 현재 아이콘 위치로 터치 메뉴(자동 이동) 가져오기(위/아래버전 선택)
	UGsUIRegionTouchMenu* GetNowTouchMenu(FVector2D In_iconPos);
};