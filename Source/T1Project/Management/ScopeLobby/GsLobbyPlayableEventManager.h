#pragma once

#include "CoreMinimal.h"
#include "CoreUObject/Public/UObject/Object.h"

#include "Classes/GsManager.h"
#include "PlayableEvent/GsPlayableEventDefine.h"
#include "PlayableEvent/EGsPlayableEventQuestGoalType.h"

#include "GsLobbyPlayableEventManager.generated.h"

class AActor;
class AController;

struct FGsSchemaPlayableEventQuest;

class UGsGameObjectPlayableEventNonPlayer;
class AGsActorPlayableEventMonsterSpawn;
class UGsGameObjectBase;
class AGsActorPlayableEventNPCAniPlay;
class AGsActorPlayableEventCameraData;

UCLASS()
class UGsLobbyPlayableEventManager : 
	public UObject,
	public IGsManager
{
	GENERATED_BODY()

private:
	UPROPERTY(Transient, VisibleInstanceOnly, Meta = (AllowPrivateAccess = true))
	TArray<UGsGameObjectPlayableEventNonPlayer*> _arrayPlayableEventNonPlayer;

	// 검색용
	TMap<int, UGsGameObjectPlayableEventNonPlayer*> _mapPlayableEventNonPlayer;
	// 디스폰 할 것(다음 틱에 삭제)
	TArray<int> _arrayDespawn;

	TArray<int> _arrayDespawnBP;

	UPROPERTY(Transient, VisibleInstanceOnly, Meta = (AllowPrivateAccess = true))
	TArray<AActor*> _arrayPlayableEventBP;

	// 검색용
	TMap<int, AActor*> _mapPlayableEventBP;


	TArray<AGsActorPlayableEventNPCAniPlay*> _arrayPlayableEventNPCAniPlay;
	TMap<int, AGsActorPlayableEventNPCAniPlay*> _mapPlayableEventNPCAniPlay;

	TArray<AGsActorPlayableEventCameraData*> _arrCamData;

	EGsPlayableEventServerCheckPointType _testCheckPointType = EGsPlayableEventServerCheckPointType::PrePlayableMovieComplete;


private:
	// id 순서 (진행순서)로 정렬된 테이블
	TArray<const FGsSchemaPlayableEventQuest* > _arrayPlayableEventQuestTable;
	// 진행 퀘스트 인덱스(정렬된 테이블 array index)
	int _progressQuestIndex = 0;
	// 현재 목표 타입
	EGsPlayableEventQuestGoalType _currentGoalType;
	// 현재 목표 위치
	FVector _currentTargetPos;
	// 목적지 도착 반경
	float _acceptanceDistance;
	// 목적지 도착 반경 제곱값
	float _squaredAcceptanceDistance;
	// 현재 목표 킬카운트
	int _currentGoalKillCount;
	// 현재 진행 킬카운트
	int _currentProgressKillCount;
	// 체크 포인트(서버)
	EGsPlayableEventServerCheckPointType _currentCheckPoint = EGsPlayableEventServerCheckPointType::First;

	bool _isTestPlayableEventEnter = false;

public:
	virtual void Initialize() override;
	virtual void Update(float inTick) override;
	// 해제
	virtual void Finalize() override;
public:
	// 테이블 정렬
	void SortTable();
	// 현재 퀘스트 데이터로 세팅
	void SetNowQuestData(int In_index);
	// 킬카운트 증가
	bool AddKillCount(int In_tblId);

	// 모든 몬스터의 게임 오브젝트를 만든다
	void MakeAllMonster();

	void MakeQuestTargetObject(int In_questId);

	UGsGameObjectPlayableEventNonPlayer* MakeMonster(AActor* In_spawnActor, TSubclassOf<AController> In_aiControllClass);

	AActor* MakeBP(AActor* In_spawnActor);

	void RequestMakeMonster(AGsActorPlayableEventMonsterSpawn* In_spawnActor);
	// 모든 몬스터 디스폰
	void DespawnAllMonster(bool In_directDespawn = false);

	void ClearAllMonsterSpawnTimmer();
	// 몬스터 디스폰
	void DespawnObject(UGsGameObjectPlayableEventNonPlayer* In_obj);
	void DespawnObjectInternal(UGsGameObjectPlayableEventNonPlayer* In_obj);

	void DespawnBPObject(AActor* In_actor);
	void DespawnBPObjectInternal(AActor* In_actor);

	// 리스트에 저장된 몬스터 삭제
	void DespawnList();
	// 디스폰 리스테에 추가
	void AddDespawnList(int In_spawnId);
	void AddDespawnListBP(int In_spawnId);

	void CollectArrangedNPCAniPlayBP();

	void CollectArrangedCameraData();

	// get
public:
	TArray<const FGsSchemaPlayableEventQuest* > GetSortedQuestTbl()
	{
		return _arrayPlayableEventQuestTable;
	}
	int GetProgressQuestIndex()
	{
		return _progressQuestIndex;
	}
	const FGsSchemaPlayableEventQuest* GetCurrentQuestTbl();

	void SetCurrentGoalType(EGsPlayableEventQuestGoalType In_type)
	{
		_currentGoalType = In_type;
	}

	EGsPlayableEventQuestGoalType GetCurrentGoalType()
	{
		return _currentGoalType;
	}
	FVector GetCurrentGoalPos()
	{
		return _currentTargetPos;
	}

	float GetSquaredAcceptanceDist()
	{
		return _squaredAcceptanceDistance;
	}
	int GetCurrentProgressKillCount()
	{
		return _currentProgressKillCount;
	}
	bool IsLastQuest();
	
	bool IsCurrentKillQuestComplete();

	void SetCurrentCheckPoint(EGsPlayableEventServerCheckPointType In_checkPoint)
	{
		_currentCheckPoint = In_checkPoint;
	}
	EGsPlayableEventServerCheckPointType GetCurrentCheckPoint()
	{
		return _currentCheckPoint;
	}
	// 제일 가까운 몬스터 찾기
	UGsGameObjectBase* FindNearTarget();
	TArray<UGsGameObjectPlayableEventNonPlayer*> GetAllMonsters()
	{
		return _arrayPlayableEventNonPlayer;
	}
	int GetMonsterCount()
	{
		return _arrayPlayableEventNonPlayer.Num();
	}

	AGsActorPlayableEventNPCAniPlay* FindAniPlayBP(int In_spawnId);

	AGsActorPlayableEventCameraData* FindCamData(bool In_isFirstCamera);

	// get, set
public:
	void SetIsTestPlayableEventEnter(bool In_val)
	{
		_isTestPlayableEventEnter = In_val;
	}
	
	bool GetAllIsTestEnterVal();

	void SetTestCheckPoint(int In_type);

	EGsPlayableEventServerCheckPointType GetTestCheckPoint()
	{
		return _testCheckPointType;
	}
};

#define LSLobbyPlayableEvent() UGsScopeHolder::GetLobbyManager<UGsLobbyPlayableEventManager>(UGsScopeLobby::EManagerType::PlayableEvent)