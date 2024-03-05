// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine/DataAsset.h"
#include "GsGameClientVersion.generated.h"

UENUM(BlueprintType)
enum class EGsGameClientBranchType : uint8
{
	NONE = 0,
	TEST_AUTO_MOVE_CHANGE,		// 목적지 이동 개선 작업 https://jira.com2us.com/jira/browse/C2URWQ-6334
	TEST_AUTO_MOVE_IDLE_CHECK,  // 위치(pos) 목적지 이동시 제자리 뛰기 개선 https://jira.com2us.com/jira/browse/C2URWQ-6331
	TEST_COLLISION_AVOIDANCE_FEATURE,	// 2023/8/22 PKT - https://jira.com2us.com/jira/browse/CHR-21494( 클라 단독 작업 - 할당된 JIRA 없으므로 BTS로 올라온 것 참조용으로 링크 )
	TEST_MAP_DROP_ITEM_FREEZING,		// 지도 드롭아이템 프리징현상 수정 https://jira.com2us.com/jira/browse/C2URWQ-6455
	GUILD_AUCTION,	// 기사단 분배&경매
	MOBILE_NET_REST_MODE,			// 2023/9/8 PKT - https://jira.com2us.com/jira/browse/CHR-21828
	SUMMON_LEGEND_COSTUME_APPEAR,	// 뽑기 레전드 코스튬: 등장 시네마틱 분기(리소스 9월 말 앱빌드에 안들어가서 막는처리) https://jira.com2us.com/jira/browse/CHR-22631
	AUTO_MOVE_REMAIN_LENGTH_SHOW,			// 땅터치 자동 이동중 auto 키거나 피격시 남은 거리 표시 이슈 https://jira.com2us.com/jira/browse/C2URWQ-6526
	ABNORMALTIY_CC_TAUNT_SKILL,				// 도발 상태이상 적중시 도발 대상 공격 적용 https://jira.com2us.com/jira/browse/C2URWQ-6723
	SUMMON_CINEMATIC_IOS_MEMORY_ISSUE,				// 뽑기 시네마틱 데이터 안날리고 들고있을때 메모리 이슈 https://jira.com2us.com/jira/browse/C2URWQ-6794
	MATERIAL_PARAM_RESTORE_PROCESS,				// 머터리얼 파라미터 조작 클래스 Stop 함수 처리 변경
	REGION_MAP_PICK_MOVE_TEXTURE,				// 지역맵 찍고 이동 텍스쳐 사용 https://jira.com2us.com/jira/browse/C2URWQ-4949
	MAGICAL_FORGE, // 장비 마법 부여 시스템(개화)
	NPC_SLOT_BATTLE_ARENA,						// NPC 슬롯 그림자 전장 대응 https://jira.com2us.com/jira/browse/CHR-24545
	PC_RVO,										// RVO시스템 활용 유무 https://jira.com2us.com/jira/browse/CHR-24697
	TEST_STAR_LEGACY,							// 별의 유산 테스트  https://jira.com2us.com/jira/browse/CHR-25775
	MAX,
};

/**
 * 클라이언트 버전을 얻어와서 로직, 데이터 참조 변수등의 분기를 결정한다. 
 */
UCLASS(BlueprintType, Category = "ClientVersion")
class T1PROJECT_API UGsGameClientVersion : public UDataAsset
{
	GENERATED_BODY()
public:
	UGsGameClientVersion();

public:
	UPROPERTY(EditAnywhere, Category = "Version")
	float _Version = 1.0f;

	UPROPERTY(EditAnywhere, Category = "Version")
	TArray<EGsGameClientBranchType> _BranchType;

private:
	bool _branchFlag[256] = { false, };

public:
	void Initialize();
	bool CheckType(EGsGameClientBranchType Type);
};