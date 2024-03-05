
#include "GsGameObjectManager.h"
#include "Engine/World.h"
#include "Components/SphereComponent.h"
#include "NavigationSystem/Public/NavigationSystem.h"
#include "Components/CapsuleComponent.h"

#include "ActorEx/GsActorWarp.h"
#include "ActorEx/GsCharacterBase.h"
#include "ActorEx/GsActorFieldFairy.h"
#include "ActorEx/GsActorInvasionPropTouch.h"
#include "ActorComponentEx/GsQueryFloorHeightComponent.h"

#include "Classes/GsSpawn.h"
#include "Camera/Define/EGsDialogCameraViewType.h"

#include "GameObject/Damage/GsDamageHandlerBase.h"
#include "GameObject/GsGlobalGameObjects.h"
#include "GameObject/Define/GsGameObjectStruct.h"
#include "GameObject/ObjectClass/GsGameObjectNonPlayerBase.h"
#include "GameObject/ObjectClass/GsGameObjectPropInteraction.h"
#include "GameObject/ObjectClass/GsGameObjectDropObject.h"
#include "GameObject/ObjectClass/GsGameObjectNonPlayer.h"
#include "GameObject/ObjectClass/GsGameObjectSandbag.h"
#include "GameObject/ObjectClass/GsGameObjectSpaceCrackNonPlayer.h"
#include "GameObject/ObjectClass/GsGameObjectRemotePlayer.h"
#include "GameObject/ObjectClass/GsGameObjectPropTouch.h"
#include "GameObject/ObjectClass/GsGameObjectCamp.h"
#include "GameObject/ObjectClass/GsGameObjectSanctumDefault.h"
#include "GameObject/ObjectClass/GsGameObjectSanctumNexus.h"
#include "GameObject/Fairy/GsFairyHandlerPlayer.h"
#include "GameObject/ObjectClass/GsGameObjectPropInteractionPotal.h"
#include "GameObject/ObjectClass/GsGameObjectPropTouchInvasion.h"
#include "GameObject/ObjectClass/GsGameObjectPropInvasionTreasure.h"

#include "Management/GsMessageHolder.h"

#include "Management/ScopeGlobal/GsOptionManager.h"
#include "Management/ScopeGlobal/GsGameDataManager.h"
#include "Management/ScopeGlobal/GsGameFlowManager.h"

#include "Management/ScopeGame/GsAIManager.h"
#include "Management/ScopeGlobal/GsQuestManager.h"

#include "GameFlow/GsGameFlowGame.h"
#include "GameFlow/GameContents/ContentsGame/GsContentsManagerGame.h"
#include "GameFlow/GameContents/ContentsGame/Hud/GsGameStateHud.h"
#include "GameFlow/GameContents/ContentsGame/Hud/Scan/GsScanHandler.h"
#include "GameFlow/GameContents/ContentsGame/Hud/CounterAttack/GsCounterAttackHandler.h"

#include "Data/GsDataContainManager.h"
#include "DataSchema/GameObject/GsSchemaPlayerShape.h"
#include "DataSchema/User/GsSchemaUserData.h"
#include "DataSchema/GameObject/Prop/GsSchemaPropData.h"
#include "DataSchema/Item/GsSchemaItemCommon.h"
#include "DataSchema/Map/Spawn/GsSchemaSpawnVisibleData.h"

#include "Message/GsMessageUI.h"
#include "Message/MessageParam/GsMinimapMessageParam.h"
#include "Message/MessageParam/GsCameraMessageParam.h"
#include "Message/MessageParam/GsUIMessageParam.h"
#include "Message/MessageParam/GsQuestMessageParam.h"

#include "AI/Data/GsAIDefine.h"
#include "AI/ContentsCondition/GsAIContentsConditionDefault.h"

#include "Option/GsGameUserSettings.h"
#include "Option/GsOptionValueDefine.h"
#include "Option/GsOptionFunc.h"
#include "Option/GsServerOption.h"

#include "Scan/GsScanDefine.h"
#include "Scan/GsAttackInfoScan.h"
#include "Scan/GsScanMemberInfo.h"

#include "GameObject/CounterAttack/GsAttackInfoCounterAttack.h"

#include "Quest/Management/GsQuestTargetManagement.h"
#include "Quest/GsQuestData.h"

#include "SphereTree/GsSphereTree.h"

#include "UTIL/GsTableUtil.h"
#include "UTIL/GsLevelUtil.h"
#include "GameObject/ObjectClass/GsGameObjectPropQuest.h"

UGsGameObjectManager::~UGsGameObjectManager()
{
}
void UGsGameObjectManager::Initialize()
{

	_listSpawns.Empty();

	_mapTypeSpawns.Reset();

	// 전역 객체링크 처리
	_globalSpawns = GGamObjects();
	check(_globalSpawns);

	//타입별 빈공간 생성
	for (auto el : TEnumRange<EGsGameObjectType>())
	{
		_mapTypeSpawns.Emplace(el, DefGameIdMap()); //초기화 위해
	}

	InitializeMessage();

	// 스캔 타이머 등록
	GetWorld()->GetTimerManager().SetTimer(_timerHandleScan,
		FTimerDelegate::CreateUObject(this, &UGsGameObjectManager::UpdateScanTimmer), 
		GData()->GetGlobalData()->_scanTimmerSecond, true);

	// 스캔 거리 제곱값 저장
	float scanDist = GData()->GetGlobalData()->_scanDist;
	_scanDistSquared = scanDist * scanDist;


	_sphereNodeHandler = MakeShareable(new FGsSpherePackFactory());

	_counterAttackInfo = new FGsAttackInfoCounterAttack();

	uint32 poolcount = GData()->GetGlobalData()->_HitPoolCount;

	if (FGsDamageRunner::PoolNotify->GetFreeCount() != poolcount)
	{
		if (FGsDamageRunner::PoolNotify->GetFreeCount() == 0)
		{
			FGsDamageRunner::PoolNotify->SetPoolCount(poolcount);
		}
		else
		{
			FGsDamageRunner::PoolNotify->ReleasePool();
			FGsDamageRunner::PoolNotify->SetPoolCount(poolcount);
		}
	}

	poolcount *= 0.2f;
	if (FGsHealRunner::PoolHealNotify->GetFreeCount() != poolcount)
	{
		if (FGsHealRunner::PoolHealNotify->GetFreeCount() == 0)
		{
			FGsHealRunner::PoolHealNotify->SetPoolCount(poolcount);
		}
		else
		{
			FGsHealRunner::PoolHealNotify->ReleasePool();
			FGsHealRunner::PoolHealNotify->SetPoolCount(poolcount);
		}
	}
}

void UGsGameObjectManager::LoadScanOption()
{
	RadarOptionInfo& optionData = GServerOption()->GetScanOption();
	int32 findScanDetectGroupType = optionData.mDetectGroupOption;
	_scanGroupTargetType = (findScanDetectGroupType == -1) ? 0 : (uint32)findScanDetectGroupType;

	int32 findScanDetectType = optionData.mDetectOption;
	_scanDetectType = (findScanDetectType == -1) ? 0 : (uint32)findScanDetectType;

	int32 findIsAutoScan = optionData.mAutoScan;
	_isAutoScan = (findIsAutoScan == 0 || findIsAutoScan == -1) ? false : true;

	int32 findScanTimmerIndex = optionData.mScanTimerSecond;
	_scanTimmerIndex = (findScanTimmerIndex == -1) ? 0 : findScanTimmerIndex;

	if (_isAutoScan == true)
	{
		StartAutoScan();
	}

	FGsUIMsgParamBool param(_isAutoScan);
	GMessage()->GetUI().SendMessage(MessageUI::AUTO_SCAN_ACTIVE, &param);
}

void UGsGameObjectManager::Finalize()
{
	_sphereNodeHandler.Reset();

	for (UGsGameObjectBase* el : _listSpawns)
	{
		DespawnObjectInternal(el);
	}

	_mapTypeSpawns.Empty();
	_listSpawns.Empty();

	FinalizeMessage();

	// 스캔 정보 클리어
	ClearScanInfo();

	// 공격자 id 정보 클리어
	ClearScanAttackInfo();

	// 타이머 클리어
	if (UWorld* world = GetWorld())
	{
		world->GetTimerManager().ClearTimer(_timerHandleScan);
		_timerHandleScan.Invalidate();
	}
	// 오토 스캔 종료
	EndAutoScan();
	
	if (_counterAttackInfo != nullptr)
	{
		_counterAttackInfo->Finalize();
		SAFE_DELETE(_counterAttackInfo);
	}
}

void UGsGameObjectManager::SystemInit()
{
	FGsDamageRunner::PoolNotify = new TGsPoolPreSizeAlloc<FGsDamageNotifyInfo>();
	FGsHealRunner::PoolHealNotify = new TGsPoolPreSizeAlloc<FGsHealNotifyInfo>();
	check(FGsDamageRunner::PoolNotify);
	check(FGsHealRunner::PoolHealNotify);	
}

void UGsGameObjectManager::SystemShutdown()
{
	if (nullptr != FGsDamageRunner::PoolNotify)
	{
		FGsDamageRunner::PoolNotify->ReleasePool();		
		SAFE_DELETE(FGsDamageRunner::PoolNotify);
	}

	if (nullptr != FGsHealRunner::PoolHealNotify)
	{
		FGsHealRunner::PoolHealNotify->ReleasePool();		
		SAFE_DELETE(FGsHealRunner::PoolHealNotify);
	}	

	check(!FGsDamageNotifyInfo::AllocCount);
	check(!FGsHealNotifyInfo::AllocCount);
}

void UGsGameObjectManager::SaveScanOption()
{
	RadarOptionInfo& optionData = GServerOption()->GetScanOption();
	int32 findScanDetectGroupType = optionData.mDetectGroupOption;
	uint32 oldScanGroupTargetType = (findScanDetectGroupType == -1) ? 0 : (uint32)findScanDetectGroupType;

	int32 findScanDetectType = optionData.mDetectOption;
	uint32 oldScanDetectType = (findScanDetectType == -1) ? 0 : (uint32)findScanDetectType;

	int32 findIsAutoScan = optionData.mAutoScan;
	bool oldIsAutoScan = (findIsAutoScan == 0 || findIsAutoScan == -1) ? false : true;

	int32 findScanTimmerIndex = optionData.mScanTimerSecond;
	int oldScanTimmerIndex = (findScanTimmerIndex == -1) ? 0 : findScanTimmerIndex;

	// check changed
	if (oldScanGroupTargetType != _scanGroupTargetType ||
		oldScanDetectType != _scanDetectType ||
		oldIsAutoScan != _isAutoScan ||
		oldScanTimmerIndex != _scanTimmerIndex)
	{
		optionData.mDetectGroupOption = _scanGroupTargetType;
		optionData.mDetectOption = _scanDetectType;
		optionData.mAutoScan = (uint32)_isAutoScan;
		optionData.mScanTimerSecond = _scanTimmerIndex;

		GServerOption()->SendToServerGameOption();
	}
}
void UGsGameObjectManager::Update(float DeltaTime)
{
#if !UE_BUILD_SHIPPING
	SCREEN_MESSAGE(40005, 200.0f, FColor::Red, "DamagePool Free[%d] Used[%d]", FGsDamageRunner::PoolNotify->GetFreeCount(), FGsDamageRunner::PoolNotify->GetUsedCount());
#endif

	for (UGsGameObjectBase* reserveDelete : _InListSpawnUpdateWithDelete)
	{
		if (reserveDelete)
		{
			DespawnObject(reserveDelete);
		}		
	}
	_InListSpawnUpdateWithDelete.Empty();

	// 스폰 오브젝트 업데이트
	_isInListSpawn = true;
	for (UGsGameObjectBase* el : _listSpawns)
	{
		el->Update(DeltaTime);
	}
	_isInListSpawn = false;

	if (_arrayRemoveAttackInfo.Num() != 0)
	{
		for (int64 delId :_arrayRemoveAttackInfo)
		{
			RemoveScanAttackerId(delId);
		}
		_arrayRemoveAttackInfo.Empty();
	}

	if (_sphereNodeHandler.IsValid())
	{
		_sphereNodeHandler->Process();
	}

	
}

void UGsGameObjectManager::AddRemoveScanAttackInfo(int64 In_gameId)
{
	if (_arrayRemoveAttackInfo.Contains(In_gameId) == true)
	{
		return;
	}

	_arrayRemoveAttackInfo.Add(In_gameId);
}

void UGsGameObjectManager::PreSpawnMe(const FVector& SpawnPos, const FRotator& SpawnRot)
{

	FGsGameDataManager* gameDataMgr = GGameData();
	// LocalPlayer만 tid는 Basic Data에 있음
	// 추후 SpawnMe 패킷에서 반영되게 된다면 변경
	const FGsNetUserData* userData = gameDataMgr->GetUserData();
	/*const FGsSchemaUserData* pcTable = UGsTableUtil::FindRow<FGsSchemaUserData>(userData->mTid);
	if (nullptr == pcTable)
	{
		GSLOG(Error, TEXT("not found UserData Table"));
	}

	const FGsSchemaPlayerShape* pcShapeTable = pcTable->shapeId.GetRow();
	if (nullptr == pcShapeTable)
	{
		GSLOG(Error, TEXT("not found PlayerShape Table"));
		return;
	}

	UClass* bpClass = nullptr;
	if (const FGsSchemaShapeData* shapeData = pcShapeTable->localplayerShapeId.GetRow())
	{
		FString path = shapeData->bPClass.ToString();
		path += TEXT("_C");

		bpClass = LoadObject<UClass>(nullptr, *path);
	}*/

	//if (bpClass)
	{
		FGsSpawnParamLocalPlayer param(EGsGameObjectType::LocalPlayer, SpawnPos, SpawnRot, userData);
		// GameObject 클래스 객체는 이미 생성 되어 있을수도 있음
		// 이곳에서 동적할당 하는 객체나 Initialize / DeInitialize 페어가 되는 설정은 금지
		// 관련 로직은 GameObject 클래스 내부에서 처리
		if (UGsGameObjectBase* local = SpawnObject(&param))
		{
			UGsGameObjectLocalPlayer* castLocal = local->CastGameObject<UGsGameObjectLocalPlayer>();

			if (FGsGameObjectData* gameObjectData = local->GetData())
			{
				gameObjectData->SetName(userData->mUserName);
				
				if (castLocal)
				{
					castLocal->UpdateBillboard(EGsBillboardUpdateType::All);
				}
			}
		}
	}
}

UGsGameObjectBase* UGsGameObjectManager::FindObject(AActor* Actor, EGsGameObjectType Type)
{
	if (Type == EGsGameObjectType::Base)
	{
		if (UGsGameObjectBase** findobj = _listSpawns.FindByPredicate([Actor](UGsGameObjectBase* el)
		{
			auto a = el->GetActor();
			return nullptr != a && a == Actor;
		}))
		{
			return *findobj;
		}
	}
	else
	{
		TArray<UGsGameObjectBase*> valueArray = FindObjectArray(Type);
		if (UGsGameObjectBase** findobj = valueArray.FindByPredicate([Actor](UGsGameObjectBase* el)
		{
			auto a = el->GetActor();
			return nullptr != a && a == Actor;
		}))
		{
			return *findobj;
		}
	}

	return nullptr;
}

UGsGameObjectBase* UGsGameObjectManager::FindObject(EGsGameObjectType Type)
{
	if (false == _mapTypeSpawns.Contains(Type))
	{
		return nullptr;
	}

	if (_mapTypeSpawns[Type].Num() > 0)
	{
		return _mapTypeSpawns[Type].CreateIterator()->Value;
	}

	return nullptr;
}

UGsGameObjectBase* UGsGameObjectManager::FindObject(EGsGameObjectType InType, int64 InGameId)
{
	if (DefGameIdMap* findValue = _mapTypeSpawns.Find(InType))
	{
		if (UGsGameObjectBase** findObject = findValue->Find(InGameId))
		{
			return *findObject;
		}
	}

	return nullptr;
}

TArray<UGsGameObjectBase*> UGsGameObjectManager::FindObjectArray(EGsGameObjectType Type)
{
	TArray<UGsGameObjectBase*> valueArray;
	if (DefGameIdMap* find = _mapTypeSpawns.Find(Type))
	{
		find->GenerateValueArray(valueArray);
	}
	return valueArray;
}

TArray<UGsGameObjectBase*> UGsGameObjectManager::FindObjectArrayByTid(EGsGameObjectType Type, int InTid)
{
	TArray<UGsGameObjectBase*> objTypes = FindObjectArray(Type);
	if (objTypes.Num() > 0)
	{
		objTypes.RemoveAll([InTid](UGsGameObjectBase* el)
			{
				return el->GetData()->GetTableId() != InTid;
			});
	}
	return objTypes;
}

TArray<AActor*> UGsGameObjectManager::FindActorArray(EGsGameObjectType Type)
{
	TArray<UGsGameObjectBase*> objectList = GSGameObject()->FindObjectArray(EGsGameObjectType::Base);	
	TArray<AActor*> actorList;
	for (UGsGameObjectBase* gameObject : objectList)
	{
		if (gameObject && gameObject->GetActor())
		{
			actorList.Add(gameObject->GetActor());
		}
	}

	return actorList;
}

// 제일 가까운 obj 찾기
UGsGameObjectBase* UGsGameObjectManager::FindObjectNearest(AActor* In_From, EGsGameObjectType In_Type)
{
	if (nullptr == In_From)
	{
		return nullptr;
	}

	FVector2D origin = FVector2D(In_From->GetActorLocation());
	return FindObjectNearest(origin, In_Type);
}

UGsGameObjectBase* UGsGameObjectManager::FindObjectNearest(const FVector2D& origin, EGsGameObjectType In_Type)
{
	UGsGameObjectBase* findObj = nullptr;

	// 타겟 검색
	TArray<UGsGameObjectBase*> spawnList = FindObjectArray(In_Type);

	// hp 0인 것들을 거른다
	TArray<UGsGameObjectBase*> liveSpawnList;

	for (auto iter : spawnList)
	{
		if (iter &&
			iter->IsZeroHP() == false)
		{
			liveSpawnList.Add(iter);
		}
	}

	if (liveSpawnList.Num() > 0)
	{
		liveSpawnList.Sort([&origin](const UGsGameObjectBase& lhs, const UGsGameObjectBase& rhs)
		{
			return (lhs.GetLocation2D() - origin).SizeSquared() < (rhs.GetLocation2D() - origin).SizeSquared();
		});
		findObj = liveSpawnList[0];
	}

	return findObj;
}


// 조건에 맞는 제일 가까운넘 찾기
UGsGameObjectBase* UGsGameObjectManager::FindObjectNearestConditionFunc(const FVector2D& origin, EGsGameObjectType In_Type,
	TFunction<bool(UGsGameObjectBase* obj)> In_conditionFunc)
{
	UGsGameObjectBase* findObj = nullptr;

	// 타겟 검색
	TArray<UGsGameObjectBase*> spawnList = FindObjectArray(In_Type);
	if (spawnList.Num() > 0)
	{
		if (In_conditionFunc != nullptr)
		{
			for (int i = spawnList.Num() - 1; i >= 0; --i)
			{
				if (nullptr != spawnList[i])
				{
					// 외부 조건 함수가 false이면
					if (false == In_conditionFunc(spawnList[i]))
					{
						// 제외
						spawnList.RemoveAt(i);
					}
				}
			}
		}

		if (spawnList.Num() <= 0)
		{
			return nullptr;
		}

		spawnList.Sort([&origin](const UGsGameObjectBase& lhs, const UGsGameObjectBase& rhs)
		{
			return (lhs.GetLocation2D() - origin).SizeSquared() < (rhs.GetLocation2D() - origin).SizeSquared();
		});
		findObj = spawnList[0];
	}

	return findObj;
}

UGsGameObjectBase* UGsGameObjectManager::FindObjectSortFuncByType(const FVector2D& origin, EGsGameObjectType In_Type,
	TArray<TFunction<bool(UGsGameObjectBase* obj)>> In_arrConditionFunc,
	TFunction<bool(const TPair<UGsGameObjectBase*, float>& lhs, const TPair<UGsGameObjectBase*, float >& rhs)> In_sortFunc)
{
	UGsGameObjectBase* findObj = nullptr;

	// 타겟 검색
	TArray<UGsGameObjectBase*> spawnList = FindObjectArray(In_Type);
	if (spawnList.Num() > 0)
	{
		// 내 플레이어 얻기
		UGsGameObjectBase* local = FindObject(EGsGameObjectType::LocalPlayer);
		if (nullptr == local)
		{
			return findObj;
		}

		UGsGameObjectLocalPlayer* castLocal = local->CastGameObject<UGsGameObjectLocalPlayer>();

		// 조건 만족하는 놈들
		TArray<TPair<UGsGameObjectBase*, float>> arrConditionPassed;
		for (UGsGameObjectBase* iter : spawnList)
		{
			if (nullptr != iter)
			{
				// 친한넘이 아니고
				// 외부 조건 함수가 모두 true 이면 추가                    
				bool externalFuncRes = true;
				for (auto funcIter : In_arrConditionFunc)
				{
					if (funcIter != nullptr)
					{
						// 하나라도 실패면 실패
						if (false == funcIter(iter))
						{
							externalFuncRes = false;
							break;
						}
					}
				}

				if (castLocal->IsEnemy(iter) && (true == externalFuncRes))
				{
					arrConditionPassed.Add(
						TPair<UGsGameObjectBase*, float>(iter, (iter->GetLocation2D() - origin).SizeSquared()));
				}
			}
		}

		if (arrConditionPassed.Num() > 0)
		{
			arrConditionPassed.Sort(In_sortFunc);
			findObj = arrConditionPassed[0].Key;
		}
	}

	return findObj;
}

// 제일 가까운 nonplayer obj 찾기(id로 찾기)
UGsGameObjectBase* UGsGameObjectManager::FindObjectNearestNonPlayerByNPCId(AActor* In_from, int In_ID)
{
	if (nullptr == In_from)
	{
		return nullptr;
	}

	FVector2D origin = FVector2D(In_from->GetActorLocation());
	return FindObjectNearestNonPlayerByNPCId(origin, In_ID);
}

// 제일 가까운 nonplayer obj 찾기(id로 찾기)
UGsGameObjectBase* UGsGameObjectManager::FindObjectNearestNonPlayerByNPCId(const FVector2D& origin, int In_ID)
{
	UGsGameObjectBase* findObj = nullptr;

	// 타겟 검색
	TArray<UGsGameObjectBase*> spawnList = FindObjectArray(EGsGameObjectType::NonPlayerBase);
	if (spawnList.Num() > 0)
	{
		TArray<TPair<UGsGameObjectBase*, float>> arrSameID;
		for (UGsGameObjectBase* iter : spawnList)
		{
			if (const FGsSchemaNpcData* data = Cast<UGsGameObjectNonPlayerBase>(iter)->GetNpcData())
			{
				UGsGameObjectCreature* creature = Cast<UGsGameObjectCreature>(iter);
				check(creature);

				// 같은 id만 뽑음
				// hp zero 아닌넘만 뽑음
				// 사망상태는 안됨
				if (data->id == In_ID &&
					iter->IsZeroHP() == false &&
					creature->IsDeadState() == false)
				{
					arrSameID.Add(
						TPair<UGsGameObjectBase*, float>(iter, (iter->GetLocation2D() - origin).SizeSquared()));
				}
			}
		}

		if (arrSameID.Num() > 0)
		{
			arrSameID.Sort([](const TPair<UGsGameObjectBase*, float>& lhs, const TPair<UGsGameObjectBase*, float>& rhs)
			{
				return (lhs.Value < rhs.Value);
			});
			findObj = arrSameID[0].Key;
		}
	}
	return findObj;
}

// 로컬에 제일 가까운 nonplayer 찾기
UGsGameObjectBase* UGsGameObjectManager::FindObjectLocalNearestNonPlayerByNPCId(int In_ID)
{
	UGsGameObjectBase* local = FindObject(EGsGameObjectType::LocalPlayer);
	if (local == nullptr)
	{
		return nullptr;
	}
	return FindObjectNearestNonPlayerByNPCId(local->GetActor(), In_ID);
}

// npc id로 대상 npcgameobject 리스트 가져오기
TArray<UGsGameObjectBase*>	UGsGameObjectManager::FindObjectNonPlayerByNPCId(int In_id)
{
	TArray<UGsGameObjectBase*> findArrayObj;

	// 타겟 검색
	TArray<UGsGameObjectBase*> spawnList = FindObjectArray(EGsGameObjectType::NonPlayerBase);
	if (spawnList.Num() > 0)
	{
		TArray<TPair<UGsGameObjectBase*, float>> arrSameID;
		for (UGsGameObjectBase* iter : spawnList)
		{
			if (const FGsSchemaNpcData* data = Cast<UGsGameObjectNonPlayerBase>(iter)->GetNpcData())
			{
				UGsGameObjectCreature* creature = Cast<UGsGameObjectCreature>(iter);
				check(creature);

				// 같은 id만 뽑음
				// hp zero 아닌넘만 뽑음
				// 사망상태는 안됨
				if (data->id == In_id &&
					iter->IsZeroHP() == false &&
					creature->IsDeadState() == false)
				{
					findArrayObj.Add(iter);

				}
			}
		}
	}
	return findArrayObj;
}

TArray<UGsGameObjectBase*> UGsGameObjectManager::FindObjectPropById(int In_id)
{
	TArray<UGsGameObjectBase*> findArrayObj;
	TArray<UGsGameObjectBase*> spawnList = FindObjectArray(EGsGameObjectType::PropInteraction);
	if (spawnList.Num() > 0)
	{
		TArray<TPair<UGsGameObjectBase*, float>> arrSameID;
		for (UGsGameObjectBase* iter : spawnList)
		{
			if (const UGsGameObjectPropInteraction* data = Cast<UGsGameObjectPropInteraction>(iter))
			{				
				if (data->GetPropTableData()->id == In_id
					&& data->GetCanInteraction())
				{
					findArrayObj.Add(iter);
				}
			}
		}
	}

	return findArrayObj;
}

// 제일 가까운 nonplayer obj 찾기(server id로 찾기)
UGsGameObjectBase* UGsGameObjectManager::FindObjectNearestNonPlayerByGameId(AActor* In_from, int64 In_serverID)
{
	if (nullptr == In_from)
	{
		return nullptr;
	}

	FVector2D origin = FVector2D(In_from->GetActorLocation());
	return FindObjectNearestNonPlayerByGameId(origin, In_serverID);
}
// 제일 가까운 nonplayer obj 찾기(server id로 찾기)
UGsGameObjectBase* UGsGameObjectManager::FindObjectNearestNonPlayerByGameId(const FVector2D& origin, int64 In_serverID)
{
	// 타겟 검색
	TArray<UGsGameObjectBase*> spawnList = FindObjectArray(EGsGameObjectType::NonPlayer);
	if (spawnList.Num() > 0)
	{
		TArray<TPair<UGsGameObjectBase*, float>> arrSameID;
		for (UGsGameObjectBase* iter : spawnList)
		{
			if (FGsGameObjectData* data = iter->GetData())
			{
				UGsGameObjectCreature* creature = Cast<UGsGameObjectCreature>(iter);
				check(creature);

				// 같은 id만 뽑음
				// hp zero 아닌넘만 뽑음
				// 사망상태는 안됨
				if (data->GetGameId() == In_serverID &&
					iter->IsZeroHP() == false &&
					creature->IsDeadState() == false)
				{
					return iter;
				}
			}
		}
	}
	return nullptr;
}
// id로 몬스터 1개 찾기
UGsGameObjectBase* UGsGameObjectManager::FindObjectNonPlayerByID(int In_ID)
{
	UGsGameObjectBase* findObj = nullptr;
	// 타겟 검색
	TArray<UGsGameObjectBase*> spawnList = FindObjectArray(EGsGameObjectType::NonPlayer);
	if (spawnList.Num() > 0)
	{
		for (auto iter : spawnList)
		{
			if (const FGsSchemaNpcData* data = Cast<UGsGameObjectNonPlayerBase>(iter)->GetNpcData())
			{
				// 같은 id만 뽑음
				if (data->id == In_ID)
				{
					findObj = iter;
				}
			}
		}
	}
	return findObj;
}

UGsGameObjectDropObject* UGsGameObjectManager::FindDropObjectNearest(bool In_autoAction)
{	
	EGsAIAutoLootDropItemGradeType autoLootOption =
		GSAI()->GetContentsConditionDefault()->GetAutoLootDropItemGrade();

	bool isAutoItemLootOtherUsers = true;	
	if (UGsGameUserSettings* gameUserSettings = GGameUserSettings())
	{
		const float isAutoItemLootOtherUsersFloat =
			gameUserSettings->GetAutoSetting(EGsOptionAuto::AUTO_ITEM_LOOT_OTHER_USERS);

		isAutoItemLootOtherUsers = (0.0f == isAutoItemLootOtherUsersFloat) ? false : true;		
	}

	EGsOptionValueAutoItemLootWeightLimit weightLimitOption = EGsOptionValueAutoItemLootWeightLimit::OFF;
	if (FGsServerOption* serverOption = GOption()->GetServerOption())
	{
		weightLimitOption = serverOption->GetAutoDropItemWeight();
	}

	const FGsGameDataManager* gameDataManager = GGameData();
	if (gameDataManager == nullptr)
	{
		return nullptr;
	}

	const FGsNetUserData* netUserData = gameDataManager->GetUserData();
	if (netUserData == nullptr)
	{
		return nullptr;
	}

	UserDBId localDBId = netUserData->mUserDBId;

	// https://jira.com2us.com/jira/browse/CHR-14498
	float returnDist = 
		(In_autoAction == true)? GSAI()->GetReturnDist() : GData()->GetGlobalData()->_autoReturnDist4;

	if (UGsGameObjectBase* local = GSGameObject()->FindObject(EGsGameObjectType::LocalPlayer))
	{
		// sentry nullptr 체크 추가
		// https://com2us-raon.sentry.io/issues/4284052264/events/b3cab119447b4847787c9b6df881aadf/
		AActor* localActor = local->GetActor();
		if (localActor == nullptr)
		{
			return nullptr;
		}


		TArray<UGsGameObjectBase*> listDropObject = GSGameObject()->FindObjectArray(EGsGameObjectType::DropObject);

		// 읎다
		if (listDropObject.Num() == 0)
		{
			return nullptr;
		}


		listDropObject.Sort([local](const UGsGameObjectBase& lhs, const UGsGameObjectBase& rhs)
		{
			return (lhs.GetLocation2D() - local->GetLocation2D()).SizeSquared() <
				(rhs.GetLocation2D() - local->GetLocation2D()).SizeSquared();
		});

		for (UGsGameObjectBase* el : listDropObject)
		{
			UGsGameObjectDropObject* castObject = el->CastGameObject<UGsGameObjectDropObject>();
			// 루팅 연출 시간이 아닌가
			// 내꺼인가
			if (castObject->GetState() == EDropObjectState::Lootable)
			{
				// 다른 유저 아이템 획득 옵션 꺼졌나
				// 오토에 의한거만 체크하기(보스 아이템 루팅 버튼으로 안주워지는 이슈: PD님 제보)
				if ((In_autoAction == true) &&				
				(isAutoItemLootOtherUsers == false) &&
					(false == castObject->IsInitOwnerDBId(localDBId)))
				{
					continue;
				}
			
				// 탐색 거리 안인지 체크
				float distanceToTarget =
					localActor->GetHorizontalDistanceTo(castObject->GetActor());
				// 젤 가까운 넘이 아니면 아닌거여
				// 시야 범위 안인지
				// 일단 전투 시야 체크랑 같이 쓴다				
				int searchRange = returnDist;
				// 시야 밖이면 실패
				if (distanceToTarget > searchRange)
				{
					return nullptr;
				}
				const FGsSchemaItemCommon* itemTbl = castObject->GetItemTable();
				// 아이템 테이블이 없다면 문제있음 넘어가자
				if (itemTbl == nullptr)
				{
					continue;
				}
				// 자동일때 비장착 아이템 줍기 옵션 체크
				if (In_autoAction == true && ItemType::EQUIP != itemTbl->type &&
				false == FGsOptionFunc::IsPossibleNoneEquipItemLootByGradeOption(itemTbl))
				{
					continue;
				}

				// 자동 줍기 옵션 체크하면서
				// 옵션값이 다주움이 아니면
				// 루팅등급 값 참조
				// 장비일때만 등급체크함
				if (In_autoAction == true &&
					autoLootOption != EGsAIAutoLootDropItemGradeType::All &&
					itemTbl->type == ItemType::EQUIP)
				{
					// 장비일때 줍기 옵션 off 이면 안줍기
					if (autoLootOption == EGsAIAutoLootDropItemGradeType::NotLoot)
					{
						continue;
					}

					int intItemGrade = (int)itemTbl->grade;
					int compareVal = (autoLootOption == EGsAIAutoLootDropItemGradeType::OverMagic) ?
						(int)ItemGrade::MAGIC : (int)ItemGrade::RARE;

					// 아이템 등급이 최소 등급보다 낮으면 다음 아이템 검색
					if (compareVal > intItemGrade)
					{
						continue;
					}
				}

				// check weight option						
				if ((true == In_autoAction) &&
					(false == FGsOptionFunc::IsPossibleLootByWeightOption(weightLimitOption, 
						castObject->GetItemWeight())))
				{
					continue;
				}


				return castObject;
			}
		}
	}
	return nullptr;
}
// 제일 가까운 DropObject 찾기(복수개)(자동 옵션 등급 체크할지)
TArray<UGsGameObjectBase*> UGsGameObjectManager::FindDropObjectsNearest(bool In_autoAction)
{
	EGsAIAutoLootDropItemGradeType autoLootOption =
		GSAI()->GetContentsConditionDefault()->GetAutoLootDropItemGrade();

	bool isAutoItemLootOtherUsers = true;
	if (UGsGameUserSettings* gameUserSettings = GGameUserSettings())
	{
		const float isAutoItemLootOtherUsersFloat =
			gameUserSettings->GetAutoSetting(EGsOptionAuto::AUTO_ITEM_LOOT_OTHER_USERS);

		isAutoItemLootOtherUsers = (0.0f == isAutoItemLootOtherUsersFloat) ? false : true;
	}

	EGsOptionValueAutoItemLootWeightLimit weightLimitOption = EGsOptionValueAutoItemLootWeightLimit::OFF;
	if (FGsServerOption* serverOption = GOption()->GetServerOption())
	{
		weightLimitOption = serverOption->GetAutoDropItemWeight();
	}

	TArray<UGsGameObjectBase*> arrayFindDropObjects;

	const FGsGameDataManager* gameDataManager = GGameData();
	if (gameDataManager == nullptr)
	{
		return arrayFindDropObjects;
	}

	const FGsNetUserData* netUserData = gameDataManager->GetUserData();
	if (netUserData == nullptr)
	{
		return arrayFindDropObjects;
	}

	UserDBId localDBId = netUserData->mUserDBId;

	// https://jira.com2us.com/jira/browse/CHR-14498
	float returnDist =
		(In_autoAction == true) ? GSAI()->GetReturnDist() : GData()->GetGlobalData()->_autoReturnDist4;;

	if (UGsGameObjectBase* local = GSGameObject()->FindObject(EGsGameObjectType::LocalPlayer))
	{

		// sentry nullptr 체크 추가
		// https://com2us-raon.sentry.io/issues/4284052264/events/b3cab119447b4847787c9b6df881aadf/
		AActor* localActor = local->GetActor();
		if (localActor == nullptr)
		{
			return arrayFindDropObjects;
		}


		TArray<UGsGameObjectBase*> listDropObject = GSGameObject()->FindObjectArray(EGsGameObjectType::DropObject);

		// 읎다
		if (listDropObject.Num() == 0)
		{
			return arrayFindDropObjects;
		}


		listDropObject.Sort([local](const UGsGameObjectBase& lhs, const UGsGameObjectBase& rhs)
			{
				return (lhs.GetLocation2D() - local->GetLocation2D()).SizeSquared() <
					(rhs.GetLocation2D() - local->GetLocation2D()).SizeSquared();
			});

		for (UGsGameObjectBase* el : listDropObject)
		{
			UGsGameObjectDropObject* castObject = el->CastGameObject<UGsGameObjectDropObject>();
			// 루팅 연출 시간이 아닌가
			// 내꺼인가
			if (castObject->GetState() == EDropObjectState::Lootable)
			{
				// 다른 유저 아이템 획득 옵션 꺼졌나
				if ((isAutoItemLootOtherUsers == false) &&
					(false == castObject->IsInitOwnerDBId(localDBId)))
				{
					continue;
				}

				// 탐색 거리 안인지 체크
				float distanceToTarget =
					localActor->GetHorizontalDistanceTo(castObject->GetActor());
				
				// 시야 범위 안인지
				// 일단 전투 시야 체크랑 같이 쓴다				
				int searchRange = returnDist;
				// 시야 밖이면 못주움
				if (distanceToTarget > searchRange)
				{
					continue;
				}
				const FGsSchemaItemCommon* itemTbl = castObject->GetItemTable();
				// 아이템 테이블이 없다면 문제있음 넘어가자
				if (itemTbl == nullptr)
				{
					continue;
				}

				// 자동일때 비장착 아이템 줍기 옵션 체크
				if (
					In_autoAction == true && ItemType::EQUIP != itemTbl->type &&
				false == FGsOptionFunc::IsPossibleNoneEquipItemLootByGradeOption(itemTbl))
				{
					continue;
				}

				// 자동 줍기 옵션 체크하면서
				// 옵션값이 다주움이 아니면
				// 루팅등급 값 참조
				// 장비일때만 등급체크함
				if (In_autoAction == true &&
					autoLootOption != EGsAIAutoLootDropItemGradeType::All &&
					itemTbl->type == ItemType::EQUIP)
				{
					// 장비일때 줍기 옵션 off 이면 안줍기
					if (autoLootOption == EGsAIAutoLootDropItemGradeType::NotLoot)
					{
						continue;
					}

					int intItemGrade = (int)itemTbl->grade;
					int compareVal = (autoLootOption == EGsAIAutoLootDropItemGradeType::OverMagic) ?
						(int)ItemGrade::MAGIC : (int)ItemGrade::RARE;

					// 아이템 등급이 최소 등급보다 낮으면 다음 아이템 검색
					if (compareVal > intItemGrade)
					{
						continue;
					}
				}

				// check weight option						
				if ((true == In_autoAction) &&
					(false == FGsOptionFunc::IsPossibleLootByWeightOption(weightLimitOption,
						castObject->GetItemWeight())))
				{
					continue;
				}


				arrayFindDropObjects.Add(el);
			}
		}
	}
	return arrayFindDropObjects;
}

class UGsGameObjectDropObject* UGsGameObjectManager::FindDropObjectByActor(const AActor* inActor)
{
	TArray<UGsGameObjectBase*> listDropObject = GSGameObject()->FindObjectArray(EGsGameObjectType::DropObject);

	for (UGsGameObjectBase* dropObject : listDropObject)
	{
		if (dropObject->GetActor() == inActor)
		{
			return Cast<UGsGameObjectDropObject>(dropObject);
		}
	}

	return nullptr;
}

UGsGameObjectPropInteraction* UGsGameObjectManager::FindPropObjectNearest(bool In_isCheckQuestTarget)
{
	FGsQuestTargetManagement* questTargetManagement = GSQuest()->GetQuestTargetManagement();
	if (questTargetManagement == nullptr)
	{
		return nullptr;
	}

	if (UGsGameObjectBase* local = GSGameObject()->FindObject(EGsGameObjectType::LocalPlayer))
	{
		TArray<UGsGameObjectBase*> listPropObject = GSGameObject()->FindObjectArray(EGsGameObjectType::PropInteraction);
		listPropObject.Sort([local](const UGsGameObjectBase& lhs, const UGsGameObjectBase& rhs)
		{
			return (lhs.GetLocation2D() - local->GetLocation2D()).SizeSquared() <
				(rhs.GetLocation2D() - local->GetLocation2D()).SizeSquared();
		});

		int searchRange = GData()->GetGlobalData()->_autoReturnDist4;
		for (UGsGameObjectBase* el : listPropObject)
		{
			if (false == el->IsAutoTarget())
			{
				continue;
			}

			if (In_isCheckQuestTarget == true &&
				el->GetData() != nullptr)
			{
				int tblId = 
					el->GetData()->GetTableId();

				// check quest target
				if (false == questTargetManagement->IsQuestProp(tblId))
				{
					continue;
				}
			}

			// 탐색 거리 안인지 체크
			float distanceToTarget =
				local->GetActor()->GetHorizontalDistanceTo(el->GetActor());
			// 이미 거리 정렬을 완료 했으므로 넘어선다면 더이상 의미 없음
			if (distanceToTarget > searchRange)
			{
				break;
			}

			UGsGameObjectPropInteraction* castObject = el->CastGameObject<UGsGameObjectPropInteraction>();
			// 채집 가능 상태 확인
			if (castObject->GetQuestTargetActive())
			{
				return castObject;
			}
		}
	}

	return nullptr;
}
TArray< UGsGameObjectBase*> UGsGameObjectManager::FindPropObjectsNearest(bool In_isCheckQuestTarget)
{
	TArray< UGsGameObjectBase*> arrayPropInteraction;
	FGsQuestTargetManagement* questTargetManagement = GSQuest()->GetQuestTargetManagement();
	if (questTargetManagement == nullptr)
	{
		return arrayPropInteraction;
	}

	if (UGsGameObjectBase* local = GSGameObject()->FindObject(EGsGameObjectType::LocalPlayer))
	{
		TArray<UGsGameObjectBase*> listPropObject = GSGameObject()->FindObjectArray(EGsGameObjectType::PropInteraction);
		listPropObject.Sort([local](const UGsGameObjectBase& lhs, const UGsGameObjectBase& rhs)
			{
				return (lhs.GetLocation2D() - local->GetLocation2D()).SizeSquared() <
					(rhs.GetLocation2D() - local->GetLocation2D()).SizeSquared();
			});

		int searchRange = GData()->GetGlobalData()->_autoReturnDist4;
		for (UGsGameObjectBase* el : listPropObject)
		{
			if (false == el->IsAutoTarget())
			{
				continue;
			}

			if (In_isCheckQuestTarget == true &&
				el->GetData() != nullptr)
			{
				int tblId =
					el->GetData()->GetTableId();

				// check quest target
				if (false == questTargetManagement->IsQuestProp(tblId))
				{
					continue;
				}
			}

			// 탐색 거리 안인지 체크
			float distanceToTarget =
				local->GetActor()->GetHorizontalDistanceTo(el->GetActor());
			// 이미 거리 정렬을 완료 했으므로 넘어선다면 더이상 의미 없음
			if (distanceToTarget > searchRange)
			{
				break;
			}

			UGsGameObjectPropInteraction* castObject = el->CastGameObject<UGsGameObjectPropInteraction>();
			// 채집 가능 상태 확인
			if (castObject->GetQuestTargetActive())
			{
				arrayPropInteraction.Add(el);
			}
		}
	}

	return arrayPropInteraction;
}

UGsGameObjectBase* UGsGameObjectManager::SpawnObject(const FGsSpawnParam* SpawnParam)
{
	// GameObject 클래스가 소멸되지 않았을경우 Actor만 스폰함
	/*if (UGsGameObjectBase* findObject = FindObject(SpawnParam._type, SpawnParam._gameId))
	{
		return findObject;
	}*/

	switch (SpawnParam->_type)
	{
		case EGsGameObjectType::NonPlayer:
		{
			return SpawnObject<UGsGameObjectNonPlayer>(SpawnParam);
		}
		case EGsGameObjectType::Sandbag:
		{
			return SpawnObject<UGsGameObjectSandbag>(SpawnParam);
		}
		case EGsGameObjectType::SpaceCrackNonPlayer:
		{
			return SpawnObject<UGsGameObjectSpaceCrackNonPlayer>(SpawnParam);
		}
		/*case EGsGameObjectType::Vehicle:
		* {
				return SpawnObject<UGsGameObjectWheelVehicle>(Uclass, SpawnParam);
		}*/
		case EGsGameObjectType::LocalPlayer:
		{
			return SpawnObject<UGsGameObjectLocalPlayer>(SpawnParam);
		}
		case  EGsGameObjectType::RemotePlayer:
		{
			return SpawnObject<UGsGameObjectRemotePlayer>(SpawnParam);
		}
		case EGsGameObjectType::Projectile:
		{
			return SpawnObject<UGsGameObjectProjectile>(SpawnParam);
		}
		case EGsGameObjectType::Warp:
		{
			return SpawnObject<UGsGameObjectWarp>(SpawnParam);
		}
		case EGsGameObjectType::DropObject:
		{
			return SpawnObject<UGsGameObjectDropObject>(SpawnParam);
		}
		case EGsGameObjectType::PropInteraction:
		{
			return SpawnPropObject(SpawnParam);
		}
		break;
		case EGsGameObjectType::Camp:
		{
			return SpawnCampObject(SpawnParam);
			//SpawnParam->_async = false;
			//return SpawnObject<UGsGameObjectCamp>(SpawnParam);
		}
		break;
		case EGsGameObjectType::Sanctum:
		{
			return SpawnSanctumObject(SpawnParam);
		}
		break;
	}
	return nullptr;
}

template<>
UGsGameObjectProjectile* UGsGameObjectManager::SpawnObject(const FGsSpawnParam* SpawnParam)
{
	if (UGsGameObjectProjectile* projectile = NewObject<UGsGameObjectProjectile>(this))
	{
		SpawnObjectInternal(projectile, SpawnParam);

		// 기본 판정은 서버에서 진행
		// 추후 폴리싱 이슈로 클라이언트에서 판정 처리 할수도 있다.
		// 만약 클라이언트 판정시 Fan 타입을 지원해야 되기 때문에 이부분은 추후 확인이 필요함
		/*if (AGsActorProjectile* actor = Cast<AGsActorProjectile>(projectile->GetActor()))
		{
			if (USphereComponent* collider = actor->GetSphereComponent())
			{
				collider->OnComponentHit.AddDynamic(projectile, &UGsGameObjectProjectile::ComponentHit);
			}
		}*/
		return projectile;
	}
	/*GSLOG(Error, TEXT("UGsSpawnComponent SpawnObject Fail -{%s}"), *Uclass->GetName());*/
	return nullptr;
}

template<>
UGsGameObjectLocalPlayer* UGsGameObjectManager::SpawnObject(const FGsSpawnParam* SpawnParam)
{
	// UGsGlobalGameObjects에서 정말 없는가 검사
	UGsGameObjectLocalPlayer* local = nullptr;
	if (_globalSpawns->GetLocalPlayer())
	{
		GSLOG(Error, TEXT("not destroy LocalPlayer Object !!"));
		local =  _globalSpawns->GetLocalPlayer();
	}
	else
	{
		local = NewObject<UGsGameObjectLocalPlayer>(this);		
		_globalSpawns->SetLocalPlayer(local);
	}

	if (nullptr != local)
	{
		local->Initialize(SpawnParam);
		InsertList(local);

		const FGsSpawnParamLocalPlayer* localSpawn = static_cast<const FGsSpawnParamLocalPlayer*>(SpawnParam);
		
		auto shapeData = localSpawn->GetShapeData(localSpawn->_netUserData->mTid);
		if (nullptr != shapeData)
		{
			FString path = shapeData->bPClass.ToString();
			path += TEXT("_C");
			UClass* uClass = LoadObject<UClass>(nullptr, *path);

			FVector calcSpawnPos = SpawnParam->_pos;
			FVector2D CapsuleSize(0.0f, 0.0f);
			// 스폰 Position 갱신
			if (const FGsSchemaCreatureCommonInfo* commonTable = local->GetCommonInfoTable())
			{
				CapsuleSize = FVector2D(commonTable->cylinderRadius, commonTable->cylinderHeight);
			}

			FHitResult hitResult;
			GetActorFeetLocation(GetWorld(), calcSpawnPos, hitResult, SpawnParam->_pos, CapsuleSize);
			AActor* actor = GsSpawn::BPClass(GetWorld(), uClass, calcSpawnPos, SpawnParam->_rot /*, name*/);
			local->ActorSpawned(actor);
		}		
		return local;
	}
	/*GSLOG(Error, TEXT("UGsSpawnComponent SpawnObject Fail -{%s}"), *Uclass->GetName());*/
	return nullptr;
}

template<>
UGsGameObjectRemotePlayer* UGsGameObjectManager::SpawnObject(const FGsSpawnParam* SpawnParam)
{
	if (UGsGameObjectRemotePlayer* remote = NewObject<UGsGameObjectRemotePlayer>(this))
	{
		remote->Initialize(SpawnParam);
		InsertList(remote);

		const FGsSpawnParamPlayer* Spawn = static_cast<const FGsSpawnParamPlayer*>(SpawnParam);
		
		auto shapeData = Spawn->GetShapeData(Spawn->_tId);
		if (nullptr != shapeData)
		{
			FString path = shapeData->bPClass.ToString();
			path += TEXT("_C");
			UClass* uClass = LoadObject<UClass>(nullptr, *path);

			FVector calcSpawnPos = SpawnParam->_pos;
			FVector2D CapsuleSize(0.0f, 0.0f);
			// 스폰 Position 갱신
			if (const FGsSchemaCreatureCommonInfo* commonTable = remote->GetCommonInfoTable())
			{
				CapsuleSize = FVector2D(commonTable->cylinderRadius, commonTable->cylinderHeight);
			}

			FHitResult hitResult;
			GetActorFeetLocation(GetWorld(), calcSpawnPos, hitResult, SpawnParam->_pos, CapsuleSize);
			AActor* actor = GsSpawn::BPClass(GetWorld(), uClass, calcSpawnPos, SpawnParam->_rot /*, name*/);
			remote->ActorSpawned(actor);
			return remote;
		}
		
	}
	return nullptr;
}

template<>
UGsGameObjectWarp* UGsGameObjectManager::SpawnObject(const FGsSpawnParam* SpawnParam)
{
	if (UGsGameObjectWarp* warp = NewObject<UGsGameObjectWarp>(this))
	{
		SpawnObjectInternal(warp, SpawnParam);

		if (AGsActorWarp* actor = Cast<AGsActorWarp>(warp->GetActor()))
		{
			if (USphereComponent* collider = actor->GetSphereComponent())
			{
				collider->OnComponentBeginOverlap.AddDynamic(warp, &UGsGameObjectWarp::OnBeginOverlap);
				collider->OnComponentEndOverlap.AddDynamic(warp, &UGsGameObjectWarp::OnEndOverlap);
			}
		}

		return warp;
	}
		
	return nullptr;
}

template<>
UGsGameObjectPropTouchInvasion* UGsGameObjectManager::SpawnObject(const FGsSpawnParam* SpawnParam)
{
	if (UGsGameObjectPropTouchInvasion* touchProb = NewObject<UGsGameObjectPropTouchInvasion>(this))
	{
		SpawnObjectInternal(touchProb, SpawnParam);

		touchProb->SetAciveCallbackInvasion(GGameData()->IsInvadeWorld() ? false : true); // 여기서 active
		//if (const FGsSpawnParamProp* castParam = static_cast<const FGsSpawnParamProp*>(SpawnParam))
		//{
		//	touchProb->SetAciveCallbackInvasion(castParam->IsInvasionActive());
		//}

		return touchProb;
	}

	return nullptr;
}

void UGsGameObjectManager::InsertList(UGsGameObjectBase* inGameObject)
{
	// 스폰목록에 추가
#ifdef UE_BUILD_SHIPPING 
	if (false == _listSpawns.Contains(inGameObject))
#endif
	{
		_listSpawns.Emplace(inGameObject);
	}
#ifdef UE_BUILD_SHIPPING
	else
#endif
	{
		return;
	}

	// ObjectType Flag 검색 리스트에 추가
	uint32 key = inGameObject->GetObjectTypeMask();
	for (auto el2 : TEnumRange<EGsGameObjectType>())
	{
		if (CHECK_FLAG_TYPE(key, el2))
		{
			if (DefGameIdMap* find = _mapTypeSpawns.Find(el2))
			{
				if (false == find->Contains(inGameObject->GetGameId()))
				{
					find->Emplace(inGameObject->GetGameId(), inGameObject);
				}
				else
				{
					if (_globalSpawns->GetLocalPlayer() != inGameObject)
					{
						GSLOG(Error, TEXT("Already Inserted _mapTypeSpawns!"));
					}
				}
			}
		}
	}
	// Actor 이름 정의
	//FString enumName = UGsGameObjectBase::GetObjectTypeName(GameObject->GetObjectType());
	//FName name(*FString::Printf(TEXT("%s_%I64d"), *enumName, GameObject->GetGameId()));

	//FVector calcSpawnPos = inPos;
	//FVector2D CapsuleSize(0.0f, 0.0f);	
	//// 스폰 Position 갱신
	//if (const FGsSchemaCreatureCommonInfo* commonTable = inGameObject->GetCommonInfoTable())
	//{
	//	float scale = 1.0f;
	//	if(UGsGameObjectNonPlayer* nonPlayer = Cast<UGsGameObjectNonPlayer>(inGameObject))
	//	{
	//		scale = nonPlayer->GetNpcData()->scale;
	//	}

	//	CapsuleSize = FVector2D(commonTable->cylinderRadius * scale, commonTable->cylinderHeight * scale);
	//}

	//FHitResult hitResult;	
	//bool canTeleport = GetActorFeetLocation(calcSpawnPos, hitResult, inPos, CapsuleSize);

	//AActor* actor = GsSpawn::BPClass(GetWorld(), inUclass, calcSpawnPos, inRot/*, name*/);
	//inGameObject->ActorSpawned(actor);	

#if WITH_EDITOR
	/*AGsCharacterBase* character = Cast<AGsCharacterBase>(actor);
	if (character)
	{
		character->_ServerID = FString::Printf(TEXT("%lld"), inGameObject->GetGameId());
	}*/
#endif

#if SPAWN_LOCATION_DEBUG && WITH_EDITOR
	//FString actorLabel = inGameObject->GetActor() ? inGameObject->GetActor()->GetName() : TEXT("None");
	//FString hitActorLabel;

	//if (canTeleport)
	//{
	//	hitActorLabel = hitResult.GetActor() != nullptr ? hitResult.GetActor()->GetActorLabel() : TEXT("None");
	//	GSLOG(Log, TEXT("teleport is valid in spawn - name :  %s"), *hitActorLabel);
	//}	

	/*FGsSpawnLocationLog log;
	log.capsuleSize = CapsuleSize.Y;
	log.clientSpawnLocation = calcSpawnPos;
	log.serverSpawnLocation = inPos;
	log.hitActorName = hitResult.GetActor() ? hitResult.GetActor()->GetActorLabel() : TEXT("no hit");
	log.hitActorLocation = canTeleport ? hitResult.Location : calcSpawnPos;
	log.isCanTeleport = canTeleport;

	if(character)
	{
		character->_spawnLog = log;
	}*/
#endif
}

AActor* UGsGameObjectManager::SpawnClientActor(int ShapeId, const FVector& Pos, const FRotator& Rot, float LifeTime)
{
	if (const FGsSchemaShapeData* shapeData =
		UGsTableUtil::FindRowById<UGsTableShapeData, FGsSchemaShapeData>(ShapeId))
	{
		FString path = shapeData->bPClass.ToString();
		path += TEXT("_C");

		return SpawnClientActor(LoadObject<UClass>(nullptr, *path), Pos, Rot, LifeTime);
	}
	return nullptr;
}

AActor* UGsGameObjectManager::SpawnClientActor(UClass* BpClass, const FVector& Pos, const FRotator& Rot, float LifeTime)
{
	if (BpClass)
	{
		AActor* actor = GsSpawn::BPClass(GetWorld(), BpClass, Pos, Rot);
		actor->SetLifeSpan(LifeTime);
		return actor;
	}
	return nullptr;
}

void UGsGameObjectManager::DespawnObject(UGsGameObjectBase* Despawn)
{
	// ClearLockOnTargetByGameId(Despawn->GetGameId());
	if (false == _isInListSpawn)
	{
		int64 despawnGameId = Despawn->GetGameId();
		OnDespawnScan(despawnGameId);
		OnDespawnCounterAttack(despawnGameId);
		DespawnObjectInternal(Despawn);
		_listSpawns.Remove(Despawn);
		Despawn = nullptr;
	}
	else
	{
		if (Despawn)
		{
			_InListSpawnUpdateWithDelete.Add(Despawn);
		}
	}
}

void UGsGameObjectManager::DespawnObjectInternal(UGsGameObjectBase* Despawn)
{
	if (_listSpawns.Contains(Despawn))
	{
		RemoveSearchKey(Despawn);

		Despawn->Finalize();
		// 액터 소멸
		if (AActor* actor = Despawn->GetActor())
		{
			actor->Destroy();
			actor->MarkPendingKill();
		}

		Despawn->ClearActor();

		if (Despawn->IsRooted())
		{
			Despawn->RemoveFromRoot();
		}

		Despawn->MarkPendingKill();
	}
}

class UGsGameObjectBase* UGsGameObjectManager::SpawnPropObject(const FGsSpawnParam* inSpawnParam)
{
	const FGsSpawnParamProp* castParam = static_cast<const FGsSpawnParamProp*> (inSpawnParam);
	if (castParam)
	{
		switch (castParam->GetPropType())
		{		
		case PropType::INVADE_PORTAL:
		{
			return SpawnObject<UGsGameObjectPropTouchInvasion>(inSpawnParam);
		}			
			break;
		case PropType::ELITE_DUNGEON_PORTAL_UP:
		case PropType::ELITE_DUNGEON_PORTAL_HIDDEN:
		{
			return SpawnObject<UGsGameObjectPropInteractionPotal>(inSpawnParam);
		}			
			break;
		case PropType::INVADE_TREASUREBOX:
		{
			return SpawnObject<UGsGameObjectPropInvasionTreasure>(inSpawnParam);
		}
		break;
		case PropType::QUEST:
		{
			return SpawnObject<UGsGameObjectPropQuest>(inSpawnParam);
		}
		break;
		}
	}

	return SpawnObject<UGsGameObjectPropTouch>(inSpawnParam);
}

UGsGameObjectBase* UGsGameObjectManager::SpawnCampObject(const FGsSpawnParam* inSpawnParam)
{
	if (UGsGameObjectCamp* campObject = NewObject<UGsGameObjectCamp>(this))
	{
		campObject->Initialize(inSpawnParam);
		InsertList(campObject);

		/*
		const FGsSchemaShapeData* shapeData = inSpawnParam->GetShapeData(inSpawnParam->_tId);
		if (nullptr != shapeData)
		{
			FString path = shapeData->bPClass.ToString();
			path += TEXT("_C");
			UClass* uClass = LoadObject<UClass>(campObject, *path);
			FVector spawnPos = inSpawnParam->_pos;

			FActorSpawnParameters spawnParam = FActorSpawnParameters();
			spawnParam.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
			spawnParam.Name = NAME_None;
			AActor* actor = GetWorld()->SpawnActor(uClass, &spawnPos, &inSpawnParam->_rot, spawnParam);
			campObject->ActorSpawned(actor);
			GSLOG(Error, TEXT("SpawnCampObject - actor pos 222 : %s"), *actor->GetActorLocation().ToString());
		}
		*/
		return campObject;
	}
	return nullptr;
}

UGsGameObjectBase* UGsGameObjectManager::SpawnSanctumObject(const FGsSpawnParam* inSpawnParam)
{
	if (inSpawnParam &&
		inSpawnParam->_npcData)
	{
		switch (inSpawnParam->_npcData->npcFunctionType)
		{
		case NpcFunctionType::SANCTUM:
		{
			return SpawnObject<UGsGameObjectSanctumDefault>(inSpawnParam);
		}	
		case NpcFunctionType::SANCTUM_NEXUS:
		{
			return SpawnObject<UGsGameObjectSanctumNexus>(inSpawnParam);
		}	
		}
	}

	return nullptr;
}

void UGsGameObjectManager::DespawnAllObject(bool includeLocal)
{
	_isSpawnMeComplete = false;
	UGsGameObjectBase* local = nullptr;
	if (false == includeLocal)
	{
		// 내케릭터는 액터만 삭제합니다.
		local = FindObject(EGsGameObjectType::LocalPlayer);
		if (nullptr != local)
		{
			if (AActor* actor = local->GetActor())
			{
				actor->Destroy();
			}
			local->ClearActor();
			local->ActorDespawn();
		}
	}

	for (UGsGameObjectBase* el : _listSpawns)
	{
		if (el != local)
		{
			DespawnObjectInternal(el);
		}
	}
	_listSpawns.Empty();

	if (false == includeLocal && nullptr != local)
	{
		_listSpawns.Emplace(local);
	}
}

void UGsGameObjectManager::DespawnAllObject(EGsGameObjectType inType)
{
	//despawn all
	TArray<UGsGameObjectBase*> gameObjectList = FindObjectArray(inType);
	for (UGsGameObjectBase* gameObject : gameObjectList)
	{
		DespawnObject(gameObject);
	}

	const auto& Find = _mapTypeSpawns.Find(inType);
	Find->Empty();
}

// 검색 키삭제
void UGsGameObjectManager::RemoveSearchKey(UGsGameObjectBase* In_removeTarget)
{
	if (nullptr == In_removeTarget)
	{
		return;
	}

	uint32 key = In_removeTarget->GetObjectTypeMask();
	int64 gameid = In_removeTarget->GetGameId();
	for (auto el2 : TEnumRange<EGsGameObjectType>())
	{
		if (CHECK_FLAG_TYPE(key, el2))
		{
			if (DefGameIdMap* find = _mapTypeSpawns.Find(el2))
			{
				find->Remove(gameid);
			}
		}
	}
}

bool UGsGameObjectManager::GetActorFeetLocation(UWorld* inWorld, FVector& OutLoc, FHitResult& outResult, const FVector& InOrigin, const FVector2D& capsuleSize)
{
	if (nullptr == inWorld)
		return false;

#if 1 // 네비게이션 데이터 위치는 실제 메쉬 위치와 이격이 있음을 확인
	FNavLocation navLocaiton;
	TryFindNearestNavmesh(navLocaiton, inWorld, InOrigin, capsuleSize);
#endif	

	FVector VerifyPos = InOrigin;
	if (navLocaiton.Location != FVector::ZeroVector)
	{
		if (VerifyPos.Z < navLocaiton.Location.Z)
		{
			VerifyPos = navLocaiton.Location;
		}
	}
	
	FVector teleportLocation;	
	if (UGsQueryFloorHeightComponent::TryGetTeleportLocation(teleportLocation, outResult, inWorld, VerifyPos, capsuleSize.Y))
	{
		// 실제 네비 데이터가 지형보다 낮게 설정되어 있을경우를 비교한다.
		// 이미 이곳에 들어왔다면 지형 Pick에 성공한 경우이므로 다시 재계산하는건 크게 의미는 없어 보인다.
		if (VerifyPos.Z < outResult.Location.Z)
		{
			GSLOG(Error, TEXT("Navi height is lower Pick Navi Height : %f, Pick Terrain Height : %f "), VerifyPos.Z, outResult.Location.Z);
		}

		OutLoc = teleportLocation;

		return true;
	}

	// 실패에 대한 로그를 추가
	GSLOG(Error, TEXT("ActorFeetLocation Failed, Spawn Pos : %f, %f, %f "), InOrigin.X, InOrigin.Y, InOrigin.Z);
	
	return false;
}

bool UGsGameObjectManager::TryFindNearestNavmesh(OUT FNavLocation& outNavLoc, class UWorld* inWorld, const FVector& inOrigin, const FVector2D& inCapsuleSize)
{	
	UNavigationSystemBase* navigaton = inWorld->GetNavigationSystem();
	if (navigaton)
	{
		UNavigationSystemV1* navigationSystemV1 = Cast<UNavigationSystemV1>(navigaton);
		if (navigationSystemV1)
		{
			// 서버 기준점 높이보다 3M 올려서 내리찍는다. (서버도 기준좌표가 없을수 있기에)
			return navigationSystemV1->ProjectPointToNavigation(inOrigin + FVector(0, 0, 300.0f), outNavLoc, FVector(inCapsuleSize.X, inCapsuleSize.Y, 20000.0f));			
		}
	}

	return false;
}

FVector UGsGameObjectManager::CalcOnGround(UClass* Uclass, const FVector& Pos)
{
	//[Todo] Class UCharacterMovementComponent
			//void UCharacterMovementComponent::FindFloor(const FVector& CapsuleLocation,
			// FFindFloorResult& OutFloorResult, bool bCanUseCachedLocation, const FHitResult* DownwardSweepResult) const
			//내부 구현을 참고 하여 개선한다.
	FVector rayOri = Pos + FVector(0.f, 0.f, 1000.f);
	FVector rayDes = rayOri + FVector(0.f, 0.f, -1.f) * 1200.f;

	FHitResult HitResult;
	TArray<AActor*> actorsToIgnore;
	//생성도 되기전에 얻어오는게 맞는지 모르것음...
	if (AActor* castActor = Uclass->GetDefaultObject<AActor>())
	{
		UKismetSystemLibrary::LineTraceSingle(GetWorld(), rayOri, rayDes, UEngineTypes::ConvertToTraceType(ECC_WorldStatic),
			false, actorsToIgnore, EDrawDebugTrace::ForDuration, HitResult, true, FLinearColor::Green, FLinearColor::Red);
		if (HitResult.bBlockingHit)
		{
			FVector location = HitResult.Location + FVector(0.f, 0.f, 0.5f);
			if (UCapsuleComponent* Capshule = castActor->FindComponentByClass<UCapsuleComponent>())
			{
				location.Z += Capshule->GetUnscaledCapsuleHalfHeight();
			}

			return location;
		}
	}
	return Pos;
}



TArray<UGsGameObjectBase*> UGsGameObjectManager::FindPropObjectsByTID(int32 inTid)
{
	TArray<UGsGameObjectBase*> findArrayObj;
	// 타겟 검색
	TArray<UGsGameObjectBase*> spawnList = FindObjectArray(EGsGameObjectType::PropObject);
	if (spawnList.Num() > 0)
	{
		TArray<TPair<UGsGameObjectBase*, float>> arrSameID;
		for (UGsGameObjectBase* iter : spawnList)
		{
			if (const FGsSchemaPropData* data = Cast<UGsGameObjectPropInteraction>(iter)->GetPropTableData())
			{
				UGsGameObjectCreature* creature = Cast<UGsGameObjectCreature>(iter);
				check(creature);

				if (data->id == inTid)
				{
					findArrayObj.Add(iter);
				}
			}
		}
	}

	return findArrayObj;
}

void UGsGameObjectManager::InitVisibleSpawnInfo(int32 inLevelKey)
{
	// 기존꺼 날리고
	ClearVisibleSpawnInfo();

	TArray<const FGsSchemaSpawnVisibleData*> spawnVisibleList = UGsLevelUtil::GetSpawnVisibleData(inLevelKey);
	for (const FGsSchemaSpawnVisibleData* visibleData : spawnVisibleList)
	{
		if (visibleData)
		{
			// 프랍인지
			if (visibleData->spawnType == CreatureSpawnType::PROP)
			{
				AddVisibleSpawnInfo(EGsGameObjectType::PropInteraction,
					visibleData->spawnId, &visibleData->spawnData);
			}
			else
			{
				// npc id로 npc function이 sandbag 인지 확인					
				if (visibleData->npcFunctionType == NpcFunctionType::SANDBAG)
				{
					AddVisibleSpawnInfo(EGsGameObjectType::Sandbag,
						visibleData->spawnId, &visibleData->spawnData);
				}
			}
		}
	}
}
// 정보 추가
void UGsGameObjectManager::AddVisibleSpawnInfo(EGsGameObjectType In_type, int64 In_spawnId, const FGsSchemaSpawnElement* In_tbl)
{
	if (_mapVisibleSpawnInfo.Contains(In_type) == false)
	{
		DefSpawnIdMap findInfo;
		findInfo.Add(In_spawnId, In_tbl);
		_mapVisibleSpawnInfo.Add(In_type, findInfo);
	}
	else
	{
		DefSpawnIdMap* findInfo = _mapVisibleSpawnInfo.Find(In_type);
		findInfo->Add(In_spawnId, In_tbl);
	}
}

void UGsGameObjectManager::ClearVisibleSpawnInfo()
{
	GSLOG(Error, TEXT("[ClearVisibleSpawnInfo] _mapVisibleSpawnInfo.Num() : %d"), _mapVisibleSpawnInfo.Num());

	if (_mapVisibleSpawnInfo.Num() > 0)
	{
		_mapVisibleSpawnInfo.Empty();
	}

	for (UGsGameObjectBase* reserveDelete : _InListSpawnUpdateWithDelete)
	{
		if (reserveDelete)
		{
			DespawnObject(reserveDelete);
		}		
	}
	_InListSpawnUpdateWithDelete.Empty();
}

const FGsSchemaSpawnElement* UGsGameObjectManager::FindVisibleSpawnInfo(EGsGameObjectType In_type, int64 In_spawnId)
{
	if (_mapVisibleSpawnInfo.Contains(In_type) == false)
	{
		return nullptr;
	}

	DefSpawnIdMap* findInfo = _mapVisibleSpawnInfo.Find(In_type);
	return findInfo->FindRef(In_spawnId);
}

// 메시지 초기화
void UGsGameObjectManager::InitializeMessage()
{
	FGsMessageHolder* msg = GMessage();

	// 게임오브젝트 메시지
	_arrMsgDelegate.Emplace(
		msg->GetGameObject().AddUObject(
			MessageGameObject::QUEST_TARGET_NPC, this,
			&UGsGameObjectManager::OnQuestTargetNpc)
	);
	_arrMsgDelegate.Emplace(
		msg->GetGameObject().AddUObject(
			MessageGameObject::QUEST_TARGET_MONSTER, this,
			&UGsGameObjectManager::OnQuestTargetMonster)
	);
	_arrMsgDelegate.Emplace(
		msg->GetGameObject().AddUObject(
			MessageGameObject::QUEST_TARGET_PROP, this,
			&UGsGameObjectManager::OnQuestTargetProp)
	);
	_arrMsgDelegate.Emplace(
		msg->GetGameObject().AddUObject(
			MessageGameObject::QUEST_SUB_INTERATION_NPC, this,
			&UGsGameObjectManager::OnQuestSubInterationNpc)
	);
	_arrMsgDelegate.Emplace(
		msg->GetGameObject().AddUObject(
			MessageGameObject::QUEST_SUB_INTERATION_NPC_LIST, this,
			&UGsGameObjectManager::OnQuestSubInterationNpcList)
	);
	_arrMsgDelegate.Emplace(
		msg->GetGameObject().AddUObject(
			MessageGameObject::CAMERA_DIALOG_START, this,
			&UGsGameObjectManager::OnCameraDialogStart)
	);
	_arrMsgDelegate.Emplace(
		msg->GetGameObject().AddUObject(
			MessageGameObject::CAMERA_DIALOG_END, this,
			&UGsGameObjectManager::OnCameraDialogEnd)
	);
	_arrMsgDelegate.Emplace(
		msg->GetGameObject().AddUObject(
			MessageGameObject::CAMERA_DIALOG_MODE_VIEW_CHANGE, this,
			&UGsGameObjectManager::OnCameraDialogModeViewChange)
	);

	// 퀘스트 메시지
	_arrQuestMsgDelegates.Emplace(
		msg->GetQuest().AddUObject(MessageContentQuest::QUEST_MAIN_COMPLETED, this,
			&UGsGameObjectManager::OnQuestMainCompleted)
	);
	_arrQuestMsgDelegates.Emplace(
		msg->GetQuest().AddUObject(MessageContentQuest::QUEST_MAIN_ACCEPTED, this,
			&UGsGameObjectManager::OnQuestMainAccepted)
	);

	_arrSystemDelegate.Emplace(
		msg->GetSystemParam().AddUObject(
			MessageSystem::SEQUENCE_PLAYER_START, this,
			&UGsGameObjectManager::OnPlaySequence)
	);

	_arrSystemDelegate.Emplace(
		msg->GetSystemParam().AddUObject(
			MessageSystem::SEQUENCE_PLAYER_END, this,
			&UGsGameObjectManager::OnPlaySequenceEnd)
	);	

	_arrSystemDelegate.Emplace(
		msg->GetSystemParam().AddUObject(
			MessageSystem::SEQUENCE_PLAYER_STOP_ONLY_PLAYER, this,
			&UGsGameObjectManager::OnPlaySequenceEnd)
	);	
}
// 메시지 해지
void UGsGameObjectManager::FinalizeMessage()
{
	FGsMessageHolder* msg = GMessage();

	if (_arrMsgDelegate.Num() != 0)
	{
		for (auto iter : _arrMsgDelegate)
		{
			msg->GetGameObject().Remove(iter);
		}
		_arrMsgDelegate.Empty();
	}

	if (_arrQuestMsgDelegates.Num() != 0)
	{
		for (auto iter : _arrQuestMsgDelegates)
		{
			msg->GetQuest().Remove(iter);
		}
		_arrQuestMsgDelegates.Empty();
	}

	if (_arrSystemDelegate.Num() != 0)
	{
		for (auto iter : _arrSystemDelegate)
		{
			msg->GetSystemParam().Remove(iter);
		}
		_arrSystemDelegate.Empty();
	}
}

// 퀘스트 타겟(npc)
void UGsGameObjectManager::OnQuestTargetNpc(const IGsMessageParam* In_param)
{
	const FGsMinimapQuestTargetNpcParam* paramTarget =
		In_param->Cast<const FGsMinimapQuestTargetNpcParam>();

	for (int iter : paramTarget->_tableId)
	{
		TArray<UGsGameObjectBase*> arrTarget = FindObjectNonPlayerByNPCId(iter);
		for (UGsGameObjectBase* obj : arrTarget)
		{
			UGsGameObjectNonPlayerBase* nonPlayer = obj->CastGameObject<UGsGameObjectNonPlayerBase>();
			if (nonPlayer)
			{
				nonPlayer->SetQuestTargetActive(paramTarget->_isAdd, paramTarget->_interactionIndex);
			}
		}
	}
}
// 퀘스트 타겟(몬스터)
void UGsGameObjectManager::OnQuestTargetMonster(const IGsMessageParam* In_param)
{
	const FGsMinimapQuestTargetMonsterParam* paramTarget =
		In_param->Cast<const FGsMinimapQuestTargetMonsterParam>();

	for (int iter : paramTarget->_tableId)
	{
		TArray<UGsGameObjectBase*> arrTarget = FindObjectNonPlayerByNPCId(iter);

		for (UGsGameObjectBase* obj : arrTarget)
		{
			UGsGameObjectNonPlayerBase* nonPlayer = obj->CastGameObject<UGsGameObjectNonPlayerBase>();
			if (nonPlayer)
			{
				nonPlayer->SetQuestTargetActive(paramTarget->_isAdd);
			}
		}
	}
}

void UGsGameObjectManager::OnQuestTargetSandbag(const IGsMessageParam* In_param)
{
	const FGsMinimapQuestTargetPropParam* paramTarget =
		In_param->Cast<const FGsMinimapQuestTargetPropParam>();

	for (int iter : paramTarget->_tableId)
	{
		TArray<UGsGameObjectBase*> arrTarget = FindObjectNonPlayerByNPCId(iter);
		for (UGsGameObjectBase* obj : arrTarget)
		{
			UGsGameObjectSandbag* sandbag = obj->CastGameObject<UGsGameObjectSandbag>();
			if (sandbag)
			{
				sandbag->SetQuestTargetActive(paramTarget->_isAdd);
			}
		}
	}
}

void UGsGameObjectManager::OnQuestTargetProp(const IGsMessageParam* In_param)
{
	const FGsMinimapQuestTargetPropParam* paramTarget =
		In_param->Cast<const FGsMinimapQuestTargetPropParam>();

	for (int iter : paramTarget->_tableId)
	{
		TArray<UGsGameObjectBase*> arrTarget = FindObjectPropById(iter);
		for (UGsGameObjectBase* obj : arrTarget)
		{
			UGsGameObjectPropInteraction* prop = obj->CastGameObject<UGsGameObjectPropInteraction>();
			if (prop)
			{
				prop->SetQuestTargetActive(paramTarget->_isAdd);
			}
		}
	}
}

void UGsGameObjectManager::OnQuestSubInterationNpc(const IGsMessageParam* In_param)
{
	const FGsMinimapQuesSubInterationNpcParam* paramTarget =
		In_param->Cast<const FGsMinimapQuesSubInterationNpcParam>();

	TArray<UGsGameObjectBase*> arrTarget = FindObjectNonPlayerByNPCId(paramTarget->_tableId);
	for (UGsGameObjectBase* obj : arrTarget)
	{
		UGsGameObjectNonPlayer* nonPlayer = obj->CastGameObject<UGsGameObjectNonPlayer>();
		if (nonPlayer)
		{
			nonPlayer->SetInteractionActive(paramTarget->_isInterationActive, paramTarget->_interationIconIndex);
		}
	}
}

void UGsGameObjectManager::OnQuestSubInterationNpcList(const IGsMessageParam* In_param)
{
	const FGsQuesSubInterationNpcMarkParam* paramTarget =
		In_param->Cast<const FGsQuesSubInterationNpcMarkParam>();

	TArray<FGsSubQuestNpcMarkData> MarkDataList = paramTarget->_npcMarkDataList;
	for (FGsSubQuestNpcMarkData& data: MarkDataList)
	{
		TArray<UGsGameObjectBase*> arrTarget = FindObjectNonPlayerByNPCId(data.GetNpcId());
		for (UGsGameObjectBase* obj : arrTarget)
		{
			UGsGameObjectNonPlayer* nonPlayer = obj->CastGameObject<UGsGameObjectNonPlayer>();
			if (nonPlayer)
			{				
				nonPlayer->SetInteractionActive(data.GetIsActive(), data.GetQuestNpcStateIndex());
			}
		}
	}
}

// 카메라 대화 연출 시작
void UGsGameObjectManager::OnCameraDialogStart(const IGsMessageParam* In_param)
{
	const FGsCameraDialogMessageParam* paramTarget =
		In_param->Cast<const FGsCameraDialogMessageParam>();
	if (nullptr == paramTarget ||
		0 == paramTarget->_arrDialogTargetObj.Num() ||
		nullptr == paramTarget->_viewData)
	{
		return;
	}
	EGsDialogCameraViewType viewType = paramTarget->_viewData->type;

	// 내 플레이어 얻기
	UGsGameObjectBase* local = FindObject(EGsGameObjectType::LocalPlayer);

	for (UGsGameObjectBase* iter : _listSpawns)
	{
		if (iter)
		{
			// local effect hide
			if (iter == local)
			{
				// abnormality effect hide
				iter->SetRepresentAbnormalityEffectVisible(false);
			}

			// 카메라 뷰 모드가 그리팅 npc가 아니고
			// not gretting fairy view
			//(npc 면 타겟 npc 외 다 hide)
			// 로컬이면 hide 안함
			if ((viewType != EGsDialogCameraViewType::DialogGreetingNpc &&
				viewType != EGsDialogCameraViewType::DialogGreetingNpcFairy &&
				viewType != EGsDialogCameraViewType::DialogGreetingSpawnFairy)
				&& iter == local)
			{
				continue;
			}

			// 카메라 뷰모드가 그리팅 local이 아닐때 타겟인지 체크(local이면 local 외에는 다 hide)
			if (viewType != EGsDialogCameraViewType::DialogGreetingLocal)
			{
				bool isTarget = false;
				for (auto targetIter : paramTarget->_arrDialogTargetObj)
				{
					if (targetIter == iter)
					{
						isTarget = true;
						break;
					}
				}
				if (isTarget == true)
				{
					continue;
				}
			}

			// 대사 스폰이면 안숨긴다
			if (iter->IsObjectType(EGsGameObjectType::NonPlayer))
			{
				if (UGsGameObjectNonPlayer* nonPlayer = Cast<UGsGameObjectNonPlayer>(iter))
				{
					int npcTblId = nonPlayer->GetNpcTblId();
					// 스폰된거면 안숨긴다
					if (paramTarget->_arrayDialogSpawn.Contains(npcTblId))
					{
						continue;
					}
				}
			}

			if (iter->GetActor())
			{
				iter->SetGameObjectActorVisible(false);
			}



			// player면 페어리도 hidden true
			if (iter->IsObjectType(EGsGameObjectType::Player) == true)
			{
				if (UGsGameObjectPlayer* player = Cast<UGsGameObjectPlayer>(iter))
				{
					if (FGsFairyHandlerPlayer* fairyHandler = player->GetFairyHandler())
					{
						if (AGsActorFieldFairy* actorFairy = fairyHandler->GetFairyActor())
						{
							actorFairy->SetActorHiddenInGame(true);
						}
					}
				}
			}

		}
	}
}
// 카메라 대화 연출 끝
void UGsGameObjectManager::OnCameraDialogEnd(const IGsMessageParam* In_param)
{
	// 내 플레이어 얻기
	UGsGameObjectBase* local = FindObject(EGsGameObjectType::LocalPlayer);

	for (UGsGameObjectBase* iter : _listSpawns)
	{
		if (iter)
		{
			// local effect show
			if (iter == local)
			{
				// abnormality effect show
				iter->SetRepresentAbnormalityEffectVisible(true);
			}


			if (iter->GetActor())
			{
				iter->SetGameObjectActorVisible(true);
			}

			// player면 페어리도 hidden false
			if (iter->IsObjectType(EGsGameObjectType::Player) == true)
			{
				if (UGsGameObjectPlayer* player = Cast<UGsGameObjectPlayer>(iter))
				{
					if (FGsFairyHandlerPlayer* fairyHandler = player->GetFairyHandler())
					{
						if (AGsActorFieldFairy* actorFairy = fairyHandler->GetFairyActor())
						{
							actorFairy->SetActorHiddenInGame(false);
						}
					}
				}
			}
		}
	}
}
// 카메라 대화 모드 뷰 변경
void UGsGameObjectManager::OnCameraDialogModeViewChange(const IGsMessageParam* In_param)
{
	const FGsCameraDialogMessageParam* paramTarget =
		In_param->Cast<const FGsCameraDialogMessageParam>();
	if (nullptr == paramTarget ||
		0 == paramTarget->_arrDialogTargetObj.Num() ||
		nullptr == paramTarget->_viewData)
	{
		return;
	}
	EGsDialogCameraViewType viewType = paramTarget->_viewData->type;

	// 내 플레이어 얻기
	UGsGameObjectBase* local = FindObject(EGsGameObjectType::LocalPlayer);

	// 보여줘야할 타겟(이전 컷에서 hidden 했을수도 있어서 hidden 꺼야할 대상)
	TArray<UGsGameObjectBase*> listShowTarget;

	for (UGsGameObjectBase* iter : _listSpawns)
	{
		if (iter)
		{
			// 카메라 뷰 모드가 그리팅 npc가 아니고
			// not gretting fairy view
			//(npc 면 타겟 npc 외 다 hide)
			// 로컬이면 hide 안함
			if ((viewType != EGsDialogCameraViewType::DialogGreetingNpc && 
				viewType != EGsDialogCameraViewType::DialogGreetingNpcFairy &&
				viewType != EGsDialogCameraViewType::DialogGreetingSpawnFairy)
				&& iter == local)
			{
				// 로컬의 액터 hidden 끔
				listShowTarget.Add(iter);
				continue;
			}

			// 카메라 뷰모드가 그리팅 local이 아닐때 타겟인지 체크(local이면 local 외에는 다 hide)
			if (viewType != EGsDialogCameraViewType::DialogGreetingLocal)
			{
				bool isTarget = false;
				for (auto targetIter : paramTarget->_arrDialogTargetObj)
				{
					if (targetIter == iter)
					{
						isTarget = true;
						break;
					}
				}
				if (isTarget == true)
				{
					// 타겟의 액터 hidden 끔
					listShowTarget.Add(iter);
					continue;
				}
			}

			// 대사 스폰이면 안숨긴다
			if (iter->IsObjectType(EGsGameObjectType::NonPlayer))
			{
				if (UGsGameObjectNonPlayer* nonPlayer = Cast<UGsGameObjectNonPlayer>(iter))
				{
					int npcTblId = nonPlayer->GetNpcTblId();
					// 스폰된거면 안숨긴다
					if (paramTarget->_arrayDialogSpawn.Contains(npcTblId))
					{
						listShowTarget.Add(iter);
						continue;
					}
				}
			}

			if (iter->GetActor())
			{
				iter->SetGameObjectActorVisible(false);
			}
		}
	}

	for (UGsGameObjectBase* iter : listShowTarget)
	{
		if (iter)
		{
			if (iter->GetActor())
			{
				if (iter->IsObjectType(EGsGameObjectType::Player))
				{
					//bak1210 : 유저와 페어리는 통합관리되는 것이 맞지만 연출구조상 별도로 페어리를 스폰하기떄문에
					// 유저만 관리하는 로직을 호출한다.
					iter->UGsGameObjectBase::SetGameObjectActorVisible(true);
				}
				else
				{
					iter->SetGameObjectActorVisible(true);
				}
				
			}
		}
	}
}

// prop visible 갱신
// sand bag 갱신
void UGsGameObjectManager::UpdateServerSpawnVisible()
{
	if (DefGameIdMap* findValue = _mapTypeSpawns.Find(EGsGameObjectType::PropInteraction))
	{
		for (auto iter : *findValue)
		{
			if (iter.Value != nullptr)
			{
				iter.Value->UpdateVisible();
			}
		}
	}

	if (DefGameIdMap* findValue = _mapTypeSpawns.Find(EGsGameObjectType::Sandbag))
	{
		for (auto iter : *findValue)
		{
			if (iter.Value != nullptr)
			{
				iter.Value->UpdateVisible();
			}
		}
	}
}

// 퀘스트 완료
void UGsGameObjectManager::OnQuestMainCompleted(const IGsMessageParam*)
{
	UpdateServerSpawnVisible();
}
// 퀘스트 수락
void UGsGameObjectManager::OnQuestMainAccepted(const IGsMessageParam*)
{
	UpdateServerSpawnVisible();
}

void UGsGameObjectManager::OnPlaySequence(const IGsMessageParam*)
{
	for (UGsGameObjectBase* iter : _listSpawns)
	{
		if (iter)
		{
			iter->SetGameObjectActorVisible(false);
		}
	}
}

void UGsGameObjectManager::OnPlaySequenceEnd(const IGsMessageParam*)
{
	for (UGsGameObjectBase* iter : _listSpawns)
	{
		if (iter)
		{
			iter->SetGameObjectActorVisible(true);
		}
	}
}

// 스캔 정보 클리어
void UGsGameObjectManager::ClearScanInfo()
{
	if (_mapScanMemberInfo.Num() == 0)
	{
		return;
	}

	for (auto iter: _mapScanMemberInfo)
	{
		if (iter.Value == nullptr)
		{
			continue;
		}

		iter.Value->Finalize();
		delete iter.Value;
	}

	_mapScanMemberInfo.Empty();
}
// 스캔 공격 정보 클리어
void UGsGameObjectManager::ClearScanAttackInfo()
{
	if (_mapScanAttackInfo.Num() == 0)
	{
		return;
	}

	for (auto iter: _mapScanAttackInfo)
	{
		if (iter.Value == nullptr)
		{
			continue;
		}
		iter.Value->Finalize();
		delete iter.Value;
	}

	_mapScanAttackInfo.Empty();
}
// 공격자 id 추가
void UGsGameObjectManager::AddScanAttackerId(int64 In_gameId)
{
	FGsAttackInfoScan* findAttackInfo = _mapScanAttackInfo.FindRef(In_gameId);

	// 기존에 있다면 시간만 갱신
	if (findAttackInfo != nullptr)
	{
		findAttackInfo->StartAttack();
		return;
	}
	// 없으면 만들고 ui 갱신
	else
	{
		findAttackInfo = new FGsAttackInfoScan(In_gameId);
		findAttackInfo->CallbackTimeEnd = [this](int64 In_callBackId)
		{
			AddRemoveScanAttackInfo(In_callBackId);
		};
		_mapScanAttackInfo.Add(In_gameId, findAttackInfo);
	}

	// 스캔정보 갱신
	if (FGsScanMemberInfo* findScanMemberInfo = FindScanMemberInfo(In_gameId))
	{
		findScanMemberInfo->AddDirtyInfo(EGsScanMemberInfoType::Attacker);
	}
	
}
// 공격자 id 빼기
void UGsGameObjectManager::RemoveScanAttackerId(int64 In_gameId)
{
	FGsAttackInfoScan* findAttackInfo =
		_mapScanAttackInfo.FindRef(In_gameId);
	if(findAttackInfo == nullptr)
	{
		return;
	}
	findAttackInfo->Finalize();
	delete findAttackInfo;

	_mapScanAttackInfo.Remove(In_gameId);

	// 스캔정보 갱신
	if (FGsScanMemberInfo* findScanMemberInfo = FindScanMemberInfo(In_gameId))
	{
		findScanMemberInfo->AddDirtyInfo(EGsScanMemberInfoType::Attacker);
	}
}

// 공격자인지
bool UGsGameObjectManager::GetIsAttackerId(int64 In_gameId)
{
	return _mapScanAttackInfo.Contains(In_gameId);
}

// 스캔타이머 업데이트
void UGsGameObjectManager::UpdateScanTimmer()
{
	if (_mapScanMemberInfo.Num() == 0)
	{
		return;
	}

	// 로컬 오브젝트
	UGsGameObjectBase* localPlayerObjBase = FindObject(EGsGameObjectType::LocalPlayer);
	if (localPlayerObjBase == nullptr)
	{
		return;
	}

	// 로컬 위치
	FVector2D localPos = FVector2D(localPlayerObjBase->GetLocation());

	for (auto iter : _mapScanMemberInfo)
	{
		if (nullptr == iter.Value)
		{
			continue;
		}
		// 딤드 체크
		// 1. 디스폰인가
		// 2. 스캔 거리 밖인가
		bool isDimmed = false;
		UGsGameObjectBase* findObject = FindObject(EGsGameObjectType::Creature, iter.Key);
		// 스폰된게 없음
		if (findObject == nullptr)
		{
			isDimmed = true;
			// null이 아니면 타겟을 null로 처리한다
			if (iter.Value->GetTargetObject() != nullptr)
			{				
				iter.Value->SetTargetObject(nullptr);
			}
		}
		else
		{
			// 기존에 디스폰이었다가 다시 스폰되었으면 object 세팅
			if (iter.Value->GetTargetObject() == nullptr)
			{
				iter.Value->SetTargetObject(findObject);
				iter.Value->SetBillboardScanNumber();
			}

			FVector2D targetObjectPos = FVector2D(findObject->GetLocation());
			float distSquared = (localPos - targetObjectPos).SizeSquared();
			// 거리 밖
			if (distSquared > _scanDistSquared)
			{
				isDimmed = true;
			}
		}

		// 기존과 딤드 상태가 다르면
		if (isDimmed != iter.Value->GetIsDimmed())
		{
			// 상태 변경
			iter.Value->SetIsDimmed(isDimmed);
			iter.Value->AddDirtyInfo(EGsScanMemberInfoType::Dimmed);
		}
	}
}
// 자동 스캔 타이머
void UGsGameObjectManager::UpdateAutoScanTimmer()
{
	FGsGameFlowGame* gameFlow = GMode()->GetGameFlow();
	if (nullptr == gameFlow)
	{
		return;
	}

	FGsContentsManagerGame* contents = gameFlow->GetContentsManagerGame();
	if (nullptr == contents)
	{
		return;
	}

	FGsGameStateHud* hud = contents->GetContentsStateObject<FGsGameStateHud>(FGsContentsMode::InGame::ContentsHud);
	if (nullptr == hud)
	{
		return;
	}

	FGsScanHandler* scanHandler = hud->GetScanHandler();
	if (scanHandler == nullptr)
	{
		return;
	}


	scanHandler->StartScan(false);
}
// 자동 스캔 시작
void UGsGameObjectManager::StartAutoScan()
{
	// 기존 타이머 종료
	EndAutoScan();

	// 갱신 시간
	float updateSecond = 0;
	// 갱신 타임
	if (_scanTimmerIndex == 0)
	{
		updateSecond =
			GData()->GetGlobalData()->_scanTimmerSecond_1;
	}
	else if (_scanTimmerIndex == 1)
	{
		updateSecond =
			GData()->GetGlobalData()->_scanTimmerSecond_2;
	}
	else
	{
		updateSecond =
			GData()->GetGlobalData()->_scanTimmerSecond_3;
	}
		 

	// 스캔 타이머 등록
	GetWorld()->GetTimerManager().SetTimer(_timerHandleAutoScan,
		FTimerDelegate::CreateUObject(this, &UGsGameObjectManager::UpdateAutoScanTimmer),
		updateSecond, true);
}
// 자동 스캔 끝
void UGsGameObjectManager::EndAutoScan()
{
	if (_timerHandleAutoScan.IsValid() == false)
	{
		return;
	}

	// 타이머 클리어
	if (UWorld* world = GetWorld())
	{
		world->GetTimerManager().ClearTimer(_timerHandleAutoScan);
		_timerHandleAutoScan.Invalidate();
	}
}

// 스캔 멤버 정보 삭제
bool UGsGameObjectManager::RemoveScanMemberInfo(int64 In_gameId)
{
	FGsScanMemberInfo* findInfo = _mapScanMemberInfo.FindRef(In_gameId);
	
	// 삭제
	if (findInfo != nullptr)
	{
		findInfo->Finalize();
		delete findInfo;
		_mapScanMemberInfo.Remove(In_gameId);

		return true;
	}
	
	return false;
}

// 디스폰시(스캔)
void UGsGameObjectManager::OnDespawnScan(int64 In_gameId)
{
	// 공격자 정보에서 삭제
	RemoveScanAttackerId(In_gameId);


	// 스캔 정보에서 없으면
	FGsScanMemberInfo* findInfo = _mapScanMemberInfo.FindRef(In_gameId);
	if (findInfo == nullptr)
	{
		return;
	}

	// 상태 변경
	findInfo->SetTargetObject(nullptr);
	findInfo->SetIsDimmed(true);
	findInfo->AddDirtyInfo(EGsScanMemberInfoType::Dimmed);
}

// 스캔 멤버 인포 찾기
FGsScanMemberInfo* UGsGameObjectManager::FindScanMemberInfo(int64 In_gameId)
{
	return _mapScanMemberInfo.FindRef(In_gameId);
}

// 자동 스캔 세팅
void UGsGameObjectManager::SetIsAutoScan(bool In_val)
{
	_isAutoScan = In_val;

	if (_isAutoScan == true)
	{
		StartAutoScan();
	}
	else
	{
		EndAutoScan();
	}
}


bool UGsGameObjectManager::IsLocalPlayerDead()
{
	UGsGameObjectBase* localPlayer = FindObject(EGsGameObjectType::LocalPlayer);
	UGsGameObjectLocalPlayer* castLocalPlayer = Cast<UGsGameObjectLocalPlayer>(localPlayer);
	if (castLocalPlayer)
	{
		return castLocalPlayer->IsZeroHP();
	}

	return false;
}

bool UGsGameObjectManager::IsAllRemotePlayerDead()
{
	bool isAllDead = true;
	TArray<UGsGameObjectBase*> objectArray = FindObjectArray(EGsGameObjectType::RemotePlayer);
	for (UGsGameObjectBase* remotePlayer : objectArray)
	{
		UGsGameObjectRemotePlayer* castRemotePlayer = Cast<UGsGameObjectRemotePlayer>(remotePlayer);
		if (castRemotePlayer
		&& false == castRemotePlayer->IsZeroHP())
		{
			isAllDead = false;
			break;
		}
	}

	return isAllDead;
}

void UGsGameObjectManager::InitSphereTree(const FVector& levelCenter, float levelRadius)
{
	if (auto gData = GData()->GetGlobalData())
	{
		_sphereNodeHandler->Init(levelCenter, levelRadius, gData->_MaxSphereCount, gData->_RootNodeMaxSize, gData->_LeapNodeMaxSize, gData->_NodeGravyValue);
	}
}

void UGsGameObjectManager::UpdateInvaderNumViaAllInvasionProps()
{
	auto invadeProps = GSGameObject()->FindObjectArray(EGsGameObjectType::PropInteractInvasion);

	//auto invadeProps = listPropObject.FilterByPredicate([](auto e) {
	//		if (auto propObject = Cast<UGsGameObjectPropInteraction>(e))
	//		{
	//			const auto propData = propObject->GetPropTableData();
	//			return propData && propData->propType == PropType::INVADE_PORTAL;
	//		}
	//		return false;
	//	});

	for (auto eObj : invadeProps)
	{
		if (auto invastionActor = Cast<AGsActorInvasionPropTouch>(eObj->GetActor()))
		{
			invastionActor->UpdateInvaderInfoPlate();
		}
	}
}

// 스캔 타이머 인덱스 세팅
void UGsGameObjectManager::SetScanTimmerIndex(int In_index)
{
	_scanTimmerIndex = In_index;

	// timmer reset
	if (_isAutoScan == true)
	{
		StartAutoScan();
	}	
}

void UGsGameObjectManager::StartCounterAttackShow(int64 In_targetGameId)
{
	if (_counterAttackInfo == nullptr)
	{
		return;
	}
	_counterAttackInfo->StartCounterAttackShow(In_targetGameId);
}

void UGsGameObjectManager::OnDespawnCounterAttack(int64 In_gameId)
{
	StopCounterAttackShow(In_gameId);
}
void UGsGameObjectManager::StopCounterAttackShow(int64 In_gameId)
{
	if (_counterAttackInfo == nullptr)
	{
		return;
	}

	if (false == _counterAttackInfo->GetIsCounterAttackShow() ||
		In_gameId != _counterAttackInfo->GetTargetGameId())
	{
		return;
	}

	_counterAttackInfo->StopCountAttackShow();
}