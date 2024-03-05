// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#define CLIENT_SPAWN_MANAGER_DEBUG true
#define CLIENTSPAWN_UNIQUE_ID_DEBUG false

#include "CoreMinimal.h"

//t1 project
#include "Shared/Client/SharedEnums/SharedCreatureEnum.h"
#include "Shared/Shared/SharedPackets/PD_World_Client_Bypass.h"
#include "GameObject/Define/GsGameObjectDefine.h"
#include "Classes/GsManager.h"
#include "Management/ScopeGame/GsGameObjectManager.h"
#include "GameObject/ObjectClass/GsGameObjectLocalPlayer.h"

#include "DataSchema/Map/GsSchemaMapData.h"
#include "DataSchema/Map/GsSchemaMapSpotInfo.h"
#include "DataSchema/Map/GsSchemaMapWarpInfo.h"
#include "DataSchema/Map/GsSchemaMapWarpSpot.h"
#include "DataSchema/Map/Spot/GsSchemaSpotData.h"
#include "DataSchema/Map/Spot/GsSchemaSpotInfo.h"

#include "GameObject/Define/GsClientSpawnDefine.h"

#include "Quest/Dialog/GsDialogType.h"

#include "Message/GsMessageContents.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"

/**
 *
 */

const int64 CLIENT_PACKET = 0x8000000000000000;
const int64 CLIENT_SPAWN_INSTANCE_MAX = 1048575;
const int64 CLIENT_SPAWN_OBJECT_MAX = 255;

class AGsCharacterLocalPlayer;
class UGsGameObjectLocalPlayer;
class UGsGameObjectBase;
class ACharacter;

struct FGsSchemaNpcDialogActorData;
struct FGsSchemaClientSpawnData;
struct FGsSchemaNpcData;

/**
* Client내에서 스폰을 관장하는 클래스
*
* 클라이언트 타입 패킷을 만들어 GameObjectManager를 사용하여 스폰을 관리한다.
*
* 주로 서버에 영향이 없는 기능성 NPC, Warp 등을 관리한다. 
*
*/

// 워프 정렬 정보
struct FGsWarpSortInfo
{
	// 테이블 값
	const FGsSchemaMapWarpInfo* _warpInfo;
	// 시작점과 워프 넘어가는 스팟(sourceSpot) 과의 거리(제곱값)
	float _distSquared;
	// 워프 넘어가는 스팟(sourceSpot) 위치
	FVector _warpSourceSpotPos;
	// 넘어가는 스팟 도착 남는 거리
	float _warpSourceCalcAcceptanceRadius;
};


class T1PROJECT_API FGsClientSpawnManager : public IGsManager
{
	//스폰 정보
private:
	TMap<EGsGameObjectType, TSharedPtr<FGsClientSpawnParamContainer>> _spawnDataMap;

	//거리로 스폰될 클라이언트 스폰 액터	
private:	
	TMap<EGsGameObjectType, TSharedPtr<FGsSpawnedGameObjContainer>> _spawnObjectMap;
	//Be spawned actor	
	TArray<TSharedPtr<FGsSpawnParam>> _spawnList;
	TArray<TSharedPtr<FGsSpawnParam>> _despawnList;
	TArray<FGsSchemaSpawnElement> _npcSpawnDataList;
	TArray<const struct FGsSchemaSpawnData*> _allZoneArray;
	TArray<const struct FGsSchemaSpawnData*> _zoneArray;
	TWeakObjectPtr<AGsCharacterLocalPlayer> _playerCharacter;
	TWeakObjectPtr<UWorld> _world;

	//이벤트로 스폰될 클라이언트 스폰 액터
private:
	TMap<EGsGameObjectType, TSharedPtr<FGsSpawnedGameObjContainer>> _eventSpawnObjectMap;	

private:	
	float _cullingDistance = 3000;
	bool _active = false;	
	int32 _levelKey;

public:	
	// 대사 연출 스폰 맵(연출 종료시 일괄 디스폰 한다)
	// + 대사 연출내의 페어리
	// 1. int: npc tblId
	// 2. int: spawn index(대사 인스턴싱 인덱스)
	TMap<int, int> _mapDialogSpawn;
	// 다음에 사용 할 인덱스(순차적 증가, 연출 끝나고 0으로 클리어)
	int _dialogIndexOrder = 0;

	// 대사연출 + 글로벌 스폰된 id(대사연출내에서 hide 안하게 처리할려고)
	// + 대사 연출내의 페어리
	TArray<int>	_arrayDialogSpawnAll;
	// 대화 스폰 페어리 id
	int _dialogSpawnFairyTblId = 0;
	// 대화 스폰 페어리 위치 타입
	EGsNpcDialogFairyPosType _dialogSpawnFairyPosType;
	// 대사연출중 삭제하는 id들
	// quest id 갱신전에 잠시 스폰되었다 사라지는 이슈때문에
	// 대사에서 삭제한걸 다시 스폰 못하게하는 처리위해 만듬
	// 다음 퀘스트 갱신후 삭제
	TArray<int> _arrayDialogCutGlobalDespawn;

	// 퀘스트에 의한 show 테이블 id
	// 생성될때 이펙트 붙이고 array에서 삭제하자
	TArray<int> _arrayQuestShowTblId;
	// 퀘스트에 의한 hide 테이블 id
	// 소멸될때 이펙트 붙이고 array에서 삭제하자
	TArray<int> _arrayQuestHideTblId;
	TArray<const FGsSchemaMapWarpInfo*> _warpArray;

	bool _isShowCurrentWarpPos = false;


	// bind message 
private:
	TArray<TPair<MessageContentQuest, FDelegateHandle>> _msgDelegates;

	//update
private:
	float _currentTime;
	float _updateRate = 0.25f;

public:
	virtual ~FGsClientSpawnManager();

public:
	virtual void Initialize() override;
	virtual void Update(float inDelta) override;
	virtual void Finalize() override;
	//virtual void Shutdown() override;

public:
	void InitializeClientSpawnGameObject(class AGsCharacterLocalPlayer* inPlayer);

public:
	void InitNpc(int32 inLevelKey);
	void InitWarp(int32 inLevelKey);

public:
	void Clear();
	void DespawnAll();

	// 강제로 업데이트
	// 이슈: 퀘스트 워프 후 바로 대화 시도시 
	// npc가 없을 수도 있어서 
	// local spawn complete 시점에 스폰 처리 1회 먼저 처리
	// In_isCallByQuest: 퀘스트에 의한 호출인지(스폰/디스폰 이펙트 때문에)
	void ForcedUpdate(bool In_isCallByQuest = false);

	//거리로 스폰
private:
	void CullingGameObjectFromPlayer(bool In_isForced, bool In_isCallByQuest = false);
	void UpdateSpawnList();
	void UpdateDespawnList();
	void SpawnGameObject(TSharedPtr<FGsSpawnParam> inSpawnParam);
	void SpawnWarp(TSharedPtr<FGsSpawnParam> inSpawnParam);
	void AddSpawnedGameObject(TSharedPtr<FGsSpawnParam> inParam);
	void DespawnGameObject(TSharedPtr<FGsSpawnParam> inParam);
	void RemoveSpawnedGameObject(TSharedPtr<FGsSpawnParam> inParam);

	//debug
private:
	void UpdateZoneList();
	void DrawZone();	
	
public:	
	void SpawnEventGameObject(TSharedPtr<FGsSpawnParam> inSpawnParam);
	void DespawnEventGameObject(TSharedPtr<FGsSpawnParam> inSpawnParam);

	//Dialogue
public:
	// 대사 연출 npc 스폰
	// 1. 스폰 정보
	// 2. 스폰시 위치 정할때 중심이 될 오브젝트
	// 3. 로컬 오브젝트
	void SpawnDialogObject(TArray<const FGsSchemaNpcDialogActorData*> In_arrSpawnInfo,
		UGsGameObjectBase* In_originObject, UGsGameObjectBase* In_localObject);
	// 대사 연출 npc 디스폰
	void DespawnDialogObject(TArray<int> In_arrTblId);
	// 대사 연출 내에서 npc 스폰
	// 연출 끝나도 상태 유지
	void SpawnGlobalObject(TArray<int> In_arrTblId);
	// 대사 연출 내에서 npc 디스폰
	// 연출 끝나도 상태 유지
	void DespawnGlobalObject(TArray<int> In_arrTblId);
	// npc object 디스폰
	void DespawnNpcByTblId(int In_tblId);
	// 대사 연출 종료
	void FinishDialog();
	// 대사 페어리 스폰
	void SpawnDialogFairy(const FGsSchemaNpcData* In_tblData, UGsGameObjectBase* In_localObject,
		EGsNpcDialogFairyPosType In_posType);
	// 대사 페어리 위치 갱신
	void RepositionDialogFairy(const FVector& In_lookPos);


	// npc hide/show
public:
	// 퀘스트 show 리스트 사용하게 id 삭제
	// 있으면 true
	bool FindAndRemoveQuestShowListId(int In_tblId);
	// 퀘스트 hide 리스트 사용하게 id 삭제
	// 있으면 true
	bool FindAndRemoveQuestHideListId(int In_tblId);

	// 메시지
private:
	// 퀘스트 완료
	void OnQuestMainCompleted(const IGsMessageParam*);
	// 퀘스트 수락
	void OnQuestMainAccepted(const IGsMessageParam*);

private:
	bool IsSpawned(TSharedPtr<FGsSpawnParam> inParam);
	// bool In_isCheckComplteNextQuest: 대사연출에서 스폰후
	// 다음 퀘스트 progress에서 show 일경우 깜박이는 이슈때문에
	// 이 인자가 true면 
	// 1. 현재 퀘스트는 false
	// 2. 다음 퀘스트는 true이면 true로 처리
	bool IsShow(TSharedPtr<FGsSpawnParam> inParam, 
		bool In_isCheckCompleteNextQuest = false);

	// 대사연출내에서 디스폰 되었는지
	bool IsDialogDespawned(TSharedPtr<FGsSpawnParam> inParam);
	UGsGameObjectBase* FindSpawnedGameObject(TSharedPtr<FGsSpawnParam> inParam);
	const FGsSpawnParam* FindNpcSpawnParamDataBySpawnID(const uint32 InSpawnID);

public:	
	bool IsSpawned(UGsGameObjectBase* inGameObject);	
	const NpcFunctionType GetNpcFunctionTypeByServerSpawnID(const uint32 InServerSpawnID);
	TArray<TSharedPtr<FGsSpawnParam>> GetAllClientSpawnParamList(EGsGameObjectType inType);


public:
	const FGsSchemaMapWarpInfo* TryGetWarp(int32 inLevelKey, int32 inIndex);
	bool TryGetSpot(int32 inLevelKey, int64 inIndex, FGsSchemaSpotInfo& outSpot);	
	// map warp info로 위치 구하기
	FVector FindSpotPos(int32 In_levelKey,const FGsSchemaMapWarpInfo& In_mapWarpInfo, 
		bool In_isSource);
public:
	//퀘스트에 스폰할 액터 
	bool TryGetNpcSpawnData(FGsSchemaSpawnElement& outData, int32 inNpcID);
	bool TryGetNpcSpawnData(FGsSpawnParam& out, int32 inNpcID);
	// 스폰용 데이터 가져오기
	bool TryGetNpcSpawnParam(TSharedPtr<FGsSpawnParam>& Out_param, int32 In_npcId);
public:
	// 다음 맵을 가기위한 워프 정보 가져오기
	bool TryGetNextMapWarp(int In_startLevelKey, int In_nextLevelKey, TArray<int>& In_exceptionMapIds,
		TArray<const FGsSchemaMapWarpInfo*>& Out_orderWarps);
	// 다음 맵을 가기위한 워프의 위치 정보 가져오기
	bool TryGetNextMapWarpPos(int In_startLevelKey, int In_nextLevelKey, 
		FGsSchemaSpotInfo& Out_spotInfo, int& Out_warpId);
	//같은 맵에서 워프하기 위해 가까운 워프 좌표 찾는 함수


	void ShowCurrentWarpPos();

public:
	// 현재 맵워프정보 구하기(워프 id, 워프 spot info), 1개만 찾기(일반적인 상황)
	bool TryGetCurrentMapWarpInfo(int32 In_startLevelKey, 
		const FVector& In_sourcePos,const FVector& In_destPos,
		int& Out_warpId, FGsSchemaSpotInfo& Out_spotInfo, 
		// 목적지가 같은 워프가 복수개 있는지
		// https://jira.com2us.com/jira/browse/CHR-24660
		bool In_multiSameDestWarp = false);
	// 내부 함수(워프 정보 구하기, 1개만 찾기)
	bool TryGetCurrentMapWarp(int32 In_startLevelKey, 
		const FVector& In_sourcePos,const FVector& In_destPos, 
		TArray<int>& In_exceptionGroupIds,
		TArray<const FGsSchemaMapWarpInfo*>& Out_orderWarps);

	// 내부 함수(워프 정보 구하기, 복수개중 찾기, 그림자 전장)
	bool TryGetCurrentMapWarpShortestDist(int32 In_startLevelKey,
		const FVector& In_sourcePos, const FVector& In_destPos,
		TArray<int>& In_exceptionGroupIds,
		TArray<const FGsSchemaMapWarpInfo*>& Out_orderWarps);

	static bool TryGetCurrentMapWarpPos(UWorld* inWorld, int32 inStartLevelKey, FVector& inSourcePos, FVector& inDestPos, TArray<FGsSchemaMapWarpInfo>& outWarpArray);
	static bool TryGetCurrentMapWarpPosInternal(UWorld* inWorld, const TArray<FGsSchemaMapWarpInfo>& inWarpArray, const TArray<FGsSchemaSpotInfo>& inSpotArray
	, TArray<FGsSchemaMapWarpInfo>& inVisitedWarpArray
	, FVector& inSourcePos, FVector& inDestPos
	, TArray<FGsSchemaMapWarpInfo>& outWarpArray);

	AActor* GetPlayer();
	static uint32 GetUniqueID(int64 inClientGameID);
	void SetActive(bool inActive);
	// npc game id 찾기 (spawn id로)
	bool FindNpcGameIdBySpawnId(SpawnId In_spawnId, int64 In_gameId, int64& Out_gameId);
	bool FindNpcByNpcTableId(int32 inTableId, OUT int64& Out_gameId);
	//Object id는 Territory tool과 출동하니 반드시 배치툴 id와 확인해서 할 것 
	static bool TryGenerateClientGameIDInRuntime(OUT int64& outUniqueID, int32 inInstanceID);	
	TMap<int, int> GetDialogSpawnObjects()
	{
		return _mapDialogSpawn;
	}
	// 대사 연출에서 스폰한 모든 id(대사, 글로벌)
	TArray<int> GetDialogAllSpawnID()
	{
		return _arrayDialogSpawnAll;
	}
	// 현재 대사 스폰 인덱스 구하기
	int GetDialogSpawnIndex();
	TArray<const FGsSchemaClientSpawnData*> GetCurrentClientSpawnData(UWorld* inWorld);
	TArray<const FGsSchemaClientSpawnData*> GetClientSpawnData(int32 inLevelKey);
	bool IsClientSpawn(EGsGameObjectType inType, UGsGameObjectBase* inObject);
	const FGsSchemaSpawnElement* FindSpawnData(int64 inClientGameId);
	// tbl id로 npc 퀘스트 진행에 따른 show인지
	bool IsQuestProgressShowNpcByTblId(int In_npdTblId);

	FVector GetDialogFairyPos(UGsGameObjectBase* In_localGameObject, 
		EGsNpcDialogFairyPosType In_posType, float In_fairyRadius, float In_fairyHalfHeiht);

	// 대화 연출 스폰된 페어리 id가져오기
	int GetDialogSpawnedFairyTblId()
	{
		return _dialogSpawnFairyTblId;
	}
	// check game id by CLIENT_PACKET value
	bool IsClientSpawnGameId(int64 In_gameId);

	void SetIsShowCurrentWarpPos(bool In_val)
	{
		_isShowCurrentWarpPos = In_val;
	}
};

#define GSClientSpawn() UGsScopeHolder::GetGameManagerFType<FGsClientSpawnManager>(UGsScopeGame::EManagerType::ClientSpawn)
