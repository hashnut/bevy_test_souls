// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Classes/GsManager.h"

#include "Management/GsScopeHolder.h"
#include "Data/GsResourceManager.h"
#include "DataSchema/GameObject/GsSchemaShapeData.h"
#include "DataSchema/GameObject/GsSchemaCreatureCommonInfo.h"

#include "Message/GsMessageContents.h"
#include "Message/GsMessageGameObject.h"
#include "Message/GsMessageSystem.h"

#include "Scan/GsScanDefine.h"
#include "GameObject/Define/GsGameObjectDefine.h"
#include "GameObject/ObjectClass/GsGameObjectProjectile.h"
#include "GameObject/ObjectClass/GsGameObjectLocalPlayer.h"
#include "GameObject/ObjectClass/GsGameObjectRemotePlayer.h"
#include "GameObject/ObjectClass/GsGameObjectWarp.h"
#include "GameObject/ObjectClass/GsGameObjectPropTouchInvasion.h"
#include "GameObject/Define/GsGameObjectStruct.h"
#include "Classes/GsSpawn.h"
#include "GsGameObjectManager.generated.h"


DECLARE_MULTICAST_DELEGATE(FOnUpdateLockOnTargetDelegate);

/**
* Object Spawn / Despawn 관련 처리 클래스
*
* GameMode에 Attach되는 컴포넌트로써 생명주는 GameMode를 따르며
* 전역관리가 필요한 객체는 GsGlobalGameObjects를 살펴보아야 한다.
*
*  생성시 클래스 타입별 설정된 BitFlag값을 기준으로 그룹화된 List를 가지고 있음 
*  @see : TMap<EGsGameObjectType, TArray<UGsGameObjectBase*>> TypeSpawns
*
*  특정 타입의 Object를 얻어오고 싶을때 검색 비용 0
*  ex) FindObject(EGsGameObjectType::Player) -> Player 이하의 모든 객체를 던짐..
*      이후 sort및 특정 기준으로 Itoration을 실행 하더라도 검색비용측면에서 유리
*/
class FGsAttackInfoScan;
class FGsAttackInfoCounterAttack;
class UGsGameObjectBase;
class FGsScanMemberInfo;

UCLASS()
class T1PROJECT_API UGsGameObjectManager : public UObject, public IGsManager
{
	GENERATED_BODY()
	
	using DefGameIdMap			= TMap<int64, class UGsGameObjectBase*>;
	// 스폰id로 정보 찾는용
	using DefSpawnIdMap			= TMap<int64, const struct FGsSchemaSpawnElement*>;
protected:
	// 액터 객체 관리
	// 전체 대상 시리얼라이즈 포함
	UPROPERTY(Transient, VisibleInstanceOnly, Meta = (AllowPrivateAccess = true))
	TArray<class UGsGameObjectBase*> _listSpawns{ TArray<UGsGameObjectBase*>()};

	UPROPERTY()
	class UGsGlobalGameObjects* _globalSpawns{ nullptr };

	// 빠른 검색정보	
	TMap<EGsGameObjectType, DefGameIdMap>	_mapTypeSpawns{ TMap<EGsGameObjectType, DefGameIdMap>() };

	// 스캔된 정보
	TMap<int64, class FGsScanMemberInfo*> _mapScanMemberInfo{ TMap<int64, FGsScanMemberInfo*>() };
	
	// 스캔 타이머 핸들(딤드 체크 처리)
	FTimerHandle _timerHandleScan;
	// 자동 스캔 타이머
	FTimerHandle _timerHandleAutoScan;

	// 스캔 거리 제곱값
	float _scanDistSquared = 0.0f;

	// group target type
	uint32 _scanGroupTargetType = 0;
	// 스캔 탐지 대상
	uint32 _scanDetectType = 0;

	// 자동 스캔인지
	bool _isAutoScan = false;
	// 타이머 인덱스(3,5,10초)
	int _scanTimmerIndex = 0;
	// 스캔 공격 정보
	TMap<int64, FGsAttackInfoScan*> _mapScanAttackInfo{ TMap<int64, FGsAttackInfoScan*>() };

	TArray<int64> _arrayRemoveAttackInfo { TArray<int64>() };

	FGsAttackInfoCounterAttack* _counterAttackInfo{ nullptr };


	// 메시지 델리게이트
	MsgGameObjHandleArray _arrMsgDelegate{ MsgGameObjHandleArray() };
	// 퀘스트 메시지 델리게이트
	TArray<TPair<MessageContentQuest, FDelegateHandle>> _arrQuestMsgDelegates { TArray<TPair<MessageContentQuest, FDelegateHandle>>() };
	// 시네마틱 메세지 델리게이트
	MsgSystemHandleArray	_arrSystemDelegate{ MsgSystemHandleArray() };

	//Prop list
private:
	// 퀘스트 진행도에 따른 hide/show 정보 찾는용도
	// 서버 스폰일때
	// prop touch, sandbag 사용
	TMap<EGsGameObjectType, DefSpawnIdMap> _mapVisibleSpawnInfo{ TMap<EGsGameObjectType, DefSpawnIdMap>() };

	// 나 스폰 되었는지
	bool _isSpawnMeComplete = false;

public:
	// 판정 영역 처리 플레그
	bool _isCollisionDebugShape = false;
	// 루트 모션 동기화 확인용 플레그
	bool _isNotSyncRootmotion = false;

	// 루트 모션 위치 영역 확인용 플레그
	bool _isRootmotionDebugShape = false;

#if	!UE_BUILD_SHIPPING
	// NPC AI 상태 등의 정보창 띄워줄지 여부의 플래그
	bool _isShowNpcState = false;
#endif

private:
	bool _isInListSpawn = false;
	TArray<class UGsGameObjectBase*>	_InListSpawnUpdateWithDelete { TArray<UGsGameObjectBase*>() };

private:
	TSharedPtr<class FGsSpherePackFactory>	_sphereNodeHandler{ nullptr };

public: 
	//UGsGameObjectManager() = default; //상속 객체가 정의되어 있을 때 = default 는 base::base(대리생성자 가 되는 듯), 멤버 초기화가 안됨
	UGsGameObjectManager() {} //멤버 초기화가 될 것 같지만, 일단 멤버들을 초기화 해준다
	virtual ~UGsGameObjectManager();

public:
	//IGsManager
	virtual void Initialize() override;
	virtual void Update(float DeltaTime) override;
	virtual void Finalize() override;

	//bak1210 : 맵이동시마다 반복적인 생성소멸이 필요없는 객체들을 핸들링
public:
	static void SystemInit();
	static void SystemShutdown();
public:
	void PreSpawnMe(const FVector& SpawnPos, const FRotator& SpawnRot);

	// 오브젝트 검색
public:
	class UGsGameObjectBase*			FindObject(EGsGameObjectType InType, int64 InGameId);
	class UGsGameObjectBase*			FindObject(EGsGameObjectType InType);
	class UGsGameObjectBase*			FindObject(AActor* Actor, EGsGameObjectType type = EGsGameObjectType::Base);
	TArray<class UGsGameObjectBase*>	FindObjectArray(EGsGameObjectType Type);
	TArray<class UGsGameObjectBase*>	FindObjectArrayByTid(EGsGameObjectType Type, int InTid);
	TArray<AActor*>						FindActorArray(EGsGameObjectType Type);

	// 제일 가까운 obj 찾기
	class UGsGameObjectBase*			FindObjectNearest(AActor* In_From, EGsGameObjectType In_Type);
	class UGsGameObjectBase*			FindObjectNearest(const FVector2D& origin, EGsGameObjectType In_Type);

	// 조건에 맞는 제일 가까운 obj 찾기
	class UGsGameObjectBase*			FindObjectNearestConditionFunc(const FVector2D& origin, EGsGameObjectType In_Type,
		TFunction<bool(class UGsGameObjectBase* obj)> In_conditionFunc = nullptr);

	// 타입으로 제일 가까운 obj 찾기
	// 정렬을 외부 함수로 
	class UGsGameObjectBase*			FindObjectSortFuncByType(const FVector2D& origin, EGsGameObjectType In_Type,
		TArray<TFunction<bool(class UGsGameObjectBase* obj)>> In_arrConditionFunc,
		TFunction<bool(const TPair<class UGsGameObjectBase*, float>& lhs, 
			const TPair<class UGsGameObjectBase*, float >& rhs)> In_sortFunc = nullptr);

	// 제일 가까운 nonplayer obj 찾기(id로 찾기)
	class UGsGameObjectBase*			FindObjectNearestNonPlayerByNPCId(AActor* In_from, int In_ID);
	class UGsGameObjectBase*			FindObjectNearestNonPlayerByNPCId(const FVector2D& origin, int In_ID);
	// 로컬에 제일 가까운 nonplayer 찾기
	class UGsGameObjectBase*			FindObjectLocalNearestNonPlayerByNPCId(int In_ID);
	// npc id로 대상 npcgameobject 리스트 가져오기
	TArray<class UGsGameObjectBase*>	FindObjectNonPlayerByNPCId(int In_id);

	// 제일 가까운 nonplayer obj 찾기(server id로 찾기)
	class UGsGameObjectBase* FindObjectNearestNonPlayerByGameId(AActor* In_from, int64 In_serverID);
	class UGsGameObjectBase* FindObjectNearestNonPlayerByGameId(const FVector2D& origin, int64 In_serverID);

	// 제일 가까운 DropObject 찾기(자동 옵션 등급 체크할지)
	class UGsGameObjectDropObject* FindDropObjectNearest(bool In_autoAction = false);
	// 제일 가까운 DropObject 찾기(복수개)(자동 옵션 등급 체크할지)
	TArray< class UGsGameObjectBase*> FindDropObjectsNearest(bool In_autoAction = false);
	class UGsGameObjectDropObject* FindDropObjectByActor(const AActor* inActor);
	// 활성화된 가까운 Prop 오브젝트 찾기
	class UGsGameObjectPropInteraction* FindPropObjectNearest(bool In_isCheckQuestTarget = false);
	TArray<class UGsGameObjectBase*> FindPropObjectsNearest(bool In_isCheckQuestTarget = false);

	// id로 몬스터 1개 찾기
	class UGsGameObjectBase*			FindObjectNonPlayerByID(int In_ID);
	TArray<class UGsGameObjectBase*>	FindObjectPropById(int In_id);

public:
	// 오브젝트 스폰 / 디스폰 외부 제공
	class UGsGameObjectBase* SpawnObject(const struct FGsSpawnParam* SpawnParam);
	void DespawnObject(class UGsGameObjectBase* Despawn);
	void DespawnAllObject(EGsGameObjectType inType);
	void DespawnAllObject(bool includeLocal = true);

private:
	void DespawnObjectInternal(class UGsGameObjectBase* Despawn);

private:
	class UGsGameObjectBase* SpawnPropObject(const FGsSpawnParam * inSpawnParam);
	class UGsGameObjectBase* SpawnCampObject(const FGsSpawnParam * inSpawnParam);
	class UGsGameObjectBase* SpawnSanctumObject(const FGsSpawnParam* inSpawnParam);

public:
	// 클라이언트에서만 사용되는 Actor 스폰
	// 기본 LifeTime을 필수로 가진다.
	AActor* SpawnClientActor(int ShapeId, const FVector& Pos, const FRotator& Rot, float LifeTime = 1.0f);
	AActor* SpawnClientActor(UClass* BpClass, const FVector& Pos, const FRotator& Rot, float LifeTime = 1.0f);

	// 검색 키삭제
	void RemoveSearchKey(class UGsGameObjectBase* In_removeTarget);

private:
	template<class tGameobject>
	tGameobject*		SpawnObject(const struct FGsSpawnParam* SpawnParam)
	{
		if (tGameobject* object = NewObject<tGameobject>(this))
		{
			return SpawnObjectInternal(object, SpawnParam);
		}
		return nullptr;
	}

	void InsertList(class UGsGameObjectBase* GameObject);

protected:
	template<class tGameobject>
	tGameobject* SpawnObjectInternal(tGameobject* Instance, const struct FGsSpawnParam* SpawnParam)
	{
		Instance->Initialize(SpawnParam);
		InsertList(Instance);
		
		if (SpawnParam->_async)
		{
			auto shapeData = SpawnParam->GetShapeData(SpawnParam->_tId);
			if (nullptr != shapeData)
			{
				Instance->SetActorLocation(SpawnParam->_pos);
				Instance->SetActorRotation(SpawnParam->_rot);
				Instance->SetActorState(ActorState::LOADING);
				TWeakObjectPtr<tGameobject> weakThis(Instance);
				GResource()->LoadAsync(shapeData->bPClass.ToString(), FStreamableDelegateParam::CreateWeakLambda(this, [weakThis](UObject* inRes)
					{
						if (tGameobject* StrongThis = weakThis.Get())
						{
							StrongThis->AsyncLoadClassComplete(inRes);
						}
					}
				), true);
			}
		}
		else
		{
			auto shapeData = SpawnParam->GetShapeData(SpawnParam->_tId);
			if (nullptr != shapeData)
			{
				FString path = shapeData->bPClass.ToString();
				path += TEXT("_C");
				UClass* uClass = LoadObject<UClass>(Instance, *path);
				FVector calcSpawnPos = SpawnParam->_pos;
//				float scale = SpawnParam->GetScale();
//				//bak1210 : 스케일이 없다면 선행높이 보정
//				if (true == FMath::IsNearlyZero(scale - 1.f))
//				{
//					FVector2D CapsuleSize(0.0f, 0.0f);
//					// 스폰 Position 갱신
//					if (const FGsSchemaCreatureCommonInfo* commonTable = Instance->GetCommonInfoTable())
//					{
//						CapsuleSize = FVector2D(commonTable->cylinderRadius * scale, commonTable->cylinderHeight * scale);
//					}
//
//					FHitResult hitResult;
//#pragma todo("client Spawn Data Spawn Height Correction")
//#if 1
//					GetActorFeetLocation(GetWorld(), calcSpawnPos, hitResult, SpawnParam->_pos, CapsuleSize);
//#else
//					if (UGsLevelUtil::TrySweepToLand(hitResult, GetWorld(), SpawnParam->_pos, CapsuleSize.X, CapsuleSize.Y))
//					{
//						calcSpawnPos = hitResult.ImpactPoint + FVector(0, 0, CapsuleSize.Y);
//					}
//#endif
//				}
				AActor* actor = GsSpawn::BPClass(GetWorld(), uClass, calcSpawnPos, SpawnParam->_rot /*, name*/);
				Instance->ActorSpawned(actor);
			}
		}		
		return Instance;
	}

protected:
	// 메시지 초기화
	void InitializeMessage();
	// 메시지 해지
	void FinalizeMessage();

	
public:
	// prop visible 갱신
	// sand bag 갱신
	void UpdateServerSpawnVisible();

	// 정보 추가
	void AddVisibleSpawnInfo(EGsGameObjectType In_type, int64 In_spawnId, const struct FGsSchemaSpawnElement* In_tbl);

	// 스캔 정보 클리어
	void ClearScanInfo();
	// 스캔 공격 정보 클리어
	void ClearScanAttackInfo();

	// scan 공격자 id 추가
	void AddScanAttackerId(int64 In_gameId);
	// 공격자 id 빼기
	void RemoveScanAttackerId(int64 In_gameId);
	// 스캔타이머 업데이트(딤드 처리)
	void UpdateScanTimmer();

	// 자동 스캔 타이머
	void UpdateAutoScanTimmer();
	// 자동 스캔 시작
	void StartAutoScan();
	// 자동 스캔 끝
	void EndAutoScan();

	// 스캔 멤버 정보 삭제
	bool RemoveScanMemberInfo(int64 In_gameId);
	// 디스폰시(스캔)
	void OnDespawnScan(int64 In_gameId);
	// 스캔 멤버 인포 찾기
	class FGsScanMemberInfo* FindScanMemberInfo(int64 In_gameId);

	void StartCounterAttackShow(int64 In_targetGameId);

	void OnDespawnCounterAttack(int64 In_gameId);
	void StopCounterAttackShow(int64 In_gameId);

	void SaveScanOption();
	void LoadScanOption();

	// 메시지
private:
	// 퀘스트 타겟(npc)
	void OnQuestTargetNpc(const struct IGsMessageParam* In_param);
	// 퀘스트 타겟(몬스터)
	void OnQuestTargetMonster(const struct IGsMessageParam* In_param);
	// 퀘스트 타겟(prop)
	void OnQuestTargetSandbag(const struct IGsMessageParam* In_param);
	// 퀘스트 타겟(prop)
	void OnQuestTargetProp(const struct IGsMessageParam* In_param);
	// 퀘스트 서브 인터랙션 npc
	void OnQuestSubInterationNpc(const struct IGsMessageParam* In_param);
	// 퀘스트 서브 인터랙션 npc List
	void OnQuestSubInterationNpcList(const struct IGsMessageParam* In_param);
	// 카메라 대화 연출 시작
	void OnCameraDialogStart(const struct IGsMessageParam* In_param);
	// 카메라 대화 연출 끝
	void OnCameraDialogEnd(const struct IGsMessageParam* In_param);
	// 카메라 대화 모드 뷰 변경
	void OnCameraDialogModeViewChange(const struct IGsMessageParam* In_param);

	// 퀘스트 메시지
private:
	// 퀘스트 완료
	void OnQuestMainCompleted(const struct IGsMessageParam*);
	// 퀘스트 수락
	void OnQuestMainAccepted(const struct IGsMessageParam*);

private:
	void OnPlaySequence(const struct IGsMessageParam*);
	void OnPlaySequenceEnd(const struct IGsMessageParam*);

public:
	static bool GetActorFeetLocation(UWorld* inWorld, FVector& outLoc, FHitResult& outResult, const FVector& InOrigin, const FVector2D& inCapsuleSize);
	static bool TryFindNearestNavmesh(OUT FNavLocation& outNavLoc, class UWorld* inWorld, const FVector& inOrigin, const FVector2D& inCapsuleSize = FVector2D(50, 50));
protected:
	FVector CalcOnGround(UClass* Uclass, const FVector& Pos);

protected:
	TStatId GetStatId() const
	{
		return TStatId();
	}
public:
	bool GetIsSpawnMeComplete() { return _isSpawnMeComplete; }
	void SetIsSpawnMeComplete(bool In_val)
	{
		_isSpawnMeComplete = In_val;
	}

public:
	TArray<class UGsGameObjectBase*> FindPropObjectsByTID(int32 inTid);
	void InitVisibleSpawnInfo(int32 inLevelKey);
	void ClearVisibleSpawnInfo();
	const struct FGsSchemaSpawnElement* FindVisibleSpawnInfo(EGsGameObjectType In_type, int64 In_spawnId);

	// 스캔정보 저장
	void SetScanInfo(TMap<int64, class FGsScanMemberInfo*> In_info)
	{
		_mapScanMemberInfo = In_info;
	}

	TMap<int64, class FGsScanMemberInfo*> GetScanInfo()
	{
		return _mapScanMemberInfo;
	}
	// 공격자인지
	bool GetIsAttackerId(int64 In_gameId);

	// 스캔 거리 제곱값 접근
	float GetScanDistSquared()
	{
		return _scanDistSquared;
	}

	// get group type
	bool GetScanGroupTargetType(EGsScanOptionGroupTargetType In_type)
	{
		return(_scanGroupTargetType & (uint32)In_type);
	}
	// add group type
	void AddScanGroupTargetType(EGsScanOptionGroupTargetType In_type)
	{
		_scanGroupTargetType |= (uint32)In_type;
	}
	// remove group type
	void RemoveScanGroupTargetType(EGsScanOptionGroupTargetType In_type)
	{
		_scanGroupTargetType ^= (uint32)In_type;
	}
	// 탐지 타입 가져오기
	bool GetScanDetectType(EGsScanOptionTargetType In_type)
	{
		return(_scanDetectType & (uint32)In_type);
	}
	// 탐지 타입 추가
	void AddScanDetectType(EGsScanOptionTargetType In_type)
	{
		_scanDetectType |= (uint32)In_type;
	}
	// 탐지 타입 빼기
	void RemoveScanDetectType(EGsScanOptionTargetType In_type)
	{
		_scanDetectType ^= (uint32)In_type;
	}
	// 자동 스캔 인지
	bool GetIsAutoScan()
	{
		return _isAutoScan;
	}
	// 자동 스캔 세팅
	void SetIsAutoScan(bool In_val);
	// 스캔 타이머 인덱스 가져오기
	int GetScanTimmerIndex()
	{
		return _scanTimmerIndex;
	}
	// 스캔 타이머 인덱스 세팅
	void SetScanTimmerIndex(int In_index);

	void AddRemoveScanAttackInfo(int64 In_gameId);

	FGsAttackInfoCounterAttack* GetAttackInfoCounterAttack()
	{
		return _counterAttackInfo;
	}
public:
	bool IsLocalPlayerDead();
	bool IsAllRemotePlayerDead();


public:
	void InitSphereTree(const FVector& levelCenter, float levelRadius);
	TSharedPtr<class FGsSpherePackFactory> GetSphereHandler() { return _sphereNodeHandler; }

public:
	void UpdateInvaderNumViaAllInvasionProps();
};

//bak1210 : template 특수화의 경우 일반적으로 cpp에서 처리해도 무방하나 언리얼 유니티 빌드의 문제로 같은 함수가 다른 obj에 묶여
	//			심볼충돌의 경우가 있다.. 이를 방지하기 위해 다른 파일에 함께묶이는 현상을 제거하기위해 명시적으로 선언
template<>
UGsGameObjectProjectile* UGsGameObjectManager::SpawnObject(const struct FGsSpawnParam* SpawnParam);

template<>
UGsGameObjectLocalPlayer* UGsGameObjectManager::SpawnObject(const struct FGsSpawnParam* SpawnParam);

template<>
UGsGameObjectRemotePlayer* UGsGameObjectManager::SpawnObject(const struct FGsSpawnParam* SpawnParam);

template<>
UGsGameObjectWarp* UGsGameObjectManager::SpawnObject(const struct FGsSpawnParam* SpawnParam);

template<>
UGsGameObjectPropTouchInvasion* UGsGameObjectManager::SpawnObject(const struct FGsSpawnParam* SpawnParam);

#define GSGameObject() UGsScopeHolder::GetGameManager<UGsGameObjectManager>(UGsScopeGame::EManagerType::GameObject)




