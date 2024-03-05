// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#define  LEVEL_LOADED_DEBUG true

#include "CoreMinimal.h"
#include "Runtime/Engine/Classes/Engine/WorldComposition.h"
#include "Runtime/Engine/Classes/Engine/LevelStreaming.h"
#include "GsGameModeBase.h"
#include "GsWorldComponent.h"
#include "UTIL/GsLevelUtil.h"
#include "GsGameModeWorldBase.h"
#include "GsGameModeWorld.generated.h"

/**
 *
 */

 class ULevelStreamingDynamic;

UCLASS()
class T1PROJECT_API AGsGameModeWorld : public AGsGameModeWorldBase
{
	GENERATED_BODY()

	//World composition streaming level
private:
	UPROPERTY()
	class UGsWorldComponent* _worldComponent;
	//플레이어가 스테틱 메쉬 위에 스폰 됐는지 확인하기 위한 테스트용 함수
	FTimerHandle _waitPlayerSpawnTimer;

	// 인게임에서 pc 스폰전에 해당 위치 레벨 로드 되었는지 체크
	// TrySweepToLand 함수 사용하기 위해 실린더 radius, height 값 저장해놓는다	
	// FGsSchemaCreatureCommonInfo 테이블 찾을때 1, 2 가 pc 남, 여 키값인데 
	// 일단은 gender 값이랑 같아서 그걸로 찾자 (로비에서 정확히 가리키는 값있다면 그걸로 변경 필요)
	// https://jira.com2us.com/jira/browse/C2URWQ-5675
	FTimerHandle _waitTerrainLoadFinish;
	// 무한 대기 안걸리개 10번 횟수 대기 후 풀게 처리
	int _waitTotalCount = 0;

public:
	AGsGameModeWorld();

public:
	virtual void StartPlay() override;
	virtual void EndPlay(EEndPlayReason::Type inType) override;
	// 현재 모드 타입 구해오기
	virtual EGsGameModeType GetGameModeType() override { return EGsGameModeType::World; }
	/// <summary>
	/// 플레이어를 스폰한다. 실제 게임 시작은 아니다. 지형 로드체크를 한다. 
	/// 언리얼 엔진에서 플레이어가 미리 생성되어야 Worldcomposition기능이 정상 동작한다. 
	/// </summary>
	/// <param name="inSameArea">같은 레벨인지 확인한다. </param>
	void StartSpawnPlayer(bool inIsSameLevel = false);
	
public:
	void TerrainLoadFinish();
	void CheckTerrainLoad();
	// 인게임에서 pc 스폰전에 해당 위치 레벨 로드 되었는지 체크
	// TrySweepToLand 함수 사용하기 위해 실린더 radius, height 값 저장해놓는다	
	// FGsSchemaCreatureCommonInfo 테이블 찾을때 1, 2 가 pc 남, 여 키값인데 
	// 일단은 gender 값이랑 같아서 그걸로 찾자 (로비에서 정확히 가리키는 값있다면 그걸로 변경 필요)
	// https://jira.com2us.com/jira/browse/C2URWQ-5675
	void CheckTerrainLoadByTrySweepToLand();

	void ClearWaitTerrainLoadFinish();

	bool IsTerrainLoaded();

public:
	//Call when SC_SPAWN_ME_COMPLETE
	void PlayerGameStart();

	//Network
private:
	void Send_CG_ACK_LOAD_TOPO_FIN();	
};
