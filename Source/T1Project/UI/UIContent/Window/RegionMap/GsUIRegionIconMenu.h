#pragma once
#include "CoreMinimal.h"

#include "UMG/Public/Blueprint/UserWidget.h"

#include "Map/WorldMap/GsWorldMapDefine.h"

#include "Runtime/DataCenter/Public/Shared/Shared/SharedInclude/SharedTypes.h"

#include "GsUIRegionIconMenu.generated.h"

class UGsButton;

class UWidgetAnimation;
class UWidgetSwitcher;

UCLASS()
class UGsUIRegionIconMenu : public UUserWidget
{
	GENERATED_BODY()
protected:
	// button: auto move
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnAutoMove;
	// button: teleport
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnTeleport;
	// animation
	UPROPERTY(BlueprintReadOnly, Category = Animations, meta = (BindWidgetAnim), Transient)
	UWidgetAnimation* _aniMenu;

	// 텔레포트 이동 가능or 불가능 스위처
	// 0: 가능, 1: 불가능(던전)
	// https://jira.com2us.com/jira/browse/CHR-23756
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidgetSwitcher* _switcherTeleport;

private:
	// 텔레포트 가능
	const int TELEPORT_POSSIBLE = 0;
	// 텔레포트 불가능(던전)
	const int TELEPORT_IMPOSSIBLE = 1;

private:
	// 일반 포탈 정보(리스트 인덱스)
	// left list select index
	int _listSelectIndex;

	// 침공 포탈 정보
	// 침공 spotId
	int64 _invasionSpotId;	
	// 침공 포털 텍스트
	FString _invasionPortalText;

	// 몬스터 드랍 정보
	MapId _monsterDropMapId;
	CreatureId _monsterDropCreatureId;
	SpawnGroupId _monsterDropSpawnGroupId;
	// 몬스터 드랍 이동할때 너무 안으로 들어가서
	// 적정 거리 밖에서 이동 정지 처리용값
	// https://jira.com2us.com/jira/browse/C2URWQ-5004
	float _monsterDropAcceptanceRange;

	// 클릭한 성소 정보
	SanctumAreaId _sanctumId;

	// 공용 정보
	// 이동할 위치 정보
	FVector _movePos;
	
	// 메뉴 타입
	EGsIconMenuType _iconMenuType = EGsIconMenuType::NormalPortal;
protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

	// logic function
public:
	UFUNCTION()
	void OnClickAutoMove();
	UFUNCTION()
	void OnClickTeleport();
	// 텔레포트 버튼 상태 갱신(던전은 사용x, 아니면 강제로 딤드 만들지)
	void InvalidateTeleportState(bool In_isForcedDimmed = false);

	// set
public:
	// 일반 포탈 데이터 세팅
	void SetNormalPortalData(int In_index);
	// 침공 포탈 데이터 세팅
	void SetInvasionPortalData(int64 In_portalSpotId, FVector In_portalPos,
		FString In_invasionText);
	// 몬스터 드랍 데이터 세팅
	void SetMonsterDropData(MapId In_mapId, CreatureId In_npcId, 
		SpawnGroupId In_spawnGroupId, FVector In_pos);

	// 성소 데이터 세팅
	void SetSanctumData(SanctumAreaId In_id);
	
};