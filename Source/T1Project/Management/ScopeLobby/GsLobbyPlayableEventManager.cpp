#include "GsLobbyPlayableEventManager.h"

#include "Management/ScopeLobby/GsLobbyCharacterManager.h"

#include "Data/GsDataContainManager.h"
#include "Data/GsGlobalConstant.h"

#include "GameObject/ObjectClass/GsGameObjectPlayableEventNonPlayer.h"
#include "GameObject/ObjectClass/GsGameObjectPlayableEventLocalPlayer.h"
#include "GameObject/Define/GsGameObjectStruct.h"

#include "ActorEx/GsActorPlayableEventMonsterSpawn.h"
#include "ActorEx/GsActorPlayableEventBPSpawn.h"
#include "ActorEx/GsActorPlayableEventNPCAniPlay.h"
#include "ActorEx/GsActorPlayableEventCameraData.h"

#include "ActorComponentEx/GsPlayableEventNonPlayerComponent.h"
#include "ActorComponentEx/GsPlayableEventBPComponent.h"

#include "PlayableEvent/GsSchemaPlayableEventQuest.h"

#include "PlayableEvent/EGsPlayableEventQuestGoalType.h"

#include "DataSchema/GameObject/Npc/GsSchemaNpcData.h"
#include "DataSchema/GameObject/GsSchemaShapeData.h"

#include "UTIL/GsTableUtil.h"

#include "GsTable.h"

#include "AIModule/Classes/AIController.h"
#include "Engine/Classes/GameFramework/Controller.h"
#include "Kismet/GameplayStatics.h"

const FName PLAYABLE_EVENT_NONPLAYER_COMPONENT = TEXT("PlayableEventNonPlayerComponent");
const FName PLAYABLE_EVENT_BP_COMPONENT = TEXT("PlayableEventBPComponent");

void UGsLobbyPlayableEventManager::Initialize()
{
	// 이시점에 하면 모바일에서 테이블 접근 못함
	// 테이블을 Chunk0-Launcher요기로 옮기던지
	// 초기화 시점을 늦춰야함(요걸로 가자)
	//SortTable();
}
// 해제
void UGsLobbyPlayableEventManager::Finalize()
{
	DespawnAllMonster(true);
}
// 테이블 정렬
void UGsLobbyPlayableEventManager::SortTable()
{
	// 이미 세팅했으면 할 필요 없음
	if (_arrayPlayableEventQuestTable.Num() != 0)
	{
		return;
	}

	const UGsTable* tbl = FGsSchemaPlayableEventQuest::GetStaticTable();

	if (nullptr == tbl)
	{
		return;
	}

	tbl->GetAllRows<FGsSchemaPlayableEventQuest>(_arrayPlayableEventQuestTable);

	// id 순서대로 정렬
	_arrayPlayableEventQuestTable.Sort([](const FGsSchemaPlayableEventQuest& lhs, const FGsSchemaPlayableEventQuest& rhs)
		{
			return lhs.id < rhs.id;
		});

}

const FGsSchemaPlayableEventQuest* UGsLobbyPlayableEventManager::GetCurrentQuestTbl()
{
	// 데이터 인덱스를 넘어감
	if (_arrayPlayableEventQuestTable.Num() <= _progressQuestIndex)
	{
		return nullptr;
	}

	const FGsSchemaPlayableEventQuest* currentTbl = _arrayPlayableEventQuestTable[_progressQuestIndex];
	return currentTbl;
}

// 현재 퀘스트 데이터로 세팅
void UGsLobbyPlayableEventManager::SetNowQuestData(int In_index)
{
	_progressQuestIndex = In_index;

	const FGsSchemaPlayableEventQuest* currentTbl = GetCurrentQuestTbl();

	if (currentTbl == nullptr)
	{
		return;
	}
	// 현재 목표 타입
	EGsPlayableEventQuestGoalType currentGoalType = currentTbl->goalType;

	SetCurrentGoalType(currentGoalType);

	if (currentGoalType == EGsPlayableEventQuestGoalType::Move)
	{
		_currentTargetPos = currentTbl->movePos;
		_acceptanceDistance = currentTbl->acceptanceRadius;

		_squaredAcceptanceDistance = _acceptanceDistance * _acceptanceDistance;
	}
	else
	{
		_currentGoalKillCount = currentTbl->killCount;
		_currentProgressKillCount = 0;
	}

	MakeQuestTargetObject(currentTbl->id);
}

bool UGsLobbyPlayableEventManager::IsLastQuest()
{
	int tableCount = _arrayPlayableEventQuestTable.Num();
	// 데이터가 없으면 끝
	if (tableCount == 0)
	{
		return true;
	}
	// 인덱스가 마지막 꺼라면 끝
	// ex) 배열4개면 인덱스 3이 마지막
	if (tableCount == _progressQuestIndex + 1)
	{
		return true;
	}
	return false;
}

// 킬카운트 증가
bool UGsLobbyPlayableEventManager::AddKillCount(int In_tblId)
{
	const FGsSchemaPlayableEventQuest* currentTbl = GetCurrentQuestTbl();

	if (currentTbl == nullptr)
	{
		return false;
	}

	bool isAdd = false;
	// 킬타겟 id 가 없으면 무조건 증가
	if (currentTbl->killTargetId.Num() == 0)
	{
		isAdd = true;
	}
	else
	{
		if (currentTbl->killTargetId.Contains(In_tblId) == true)
		{			
			isAdd = true;
		}
	}

	if (isAdd == true)
	{
		++_currentProgressKillCount;
	}

	return isAdd;
}

bool UGsLobbyPlayableEventManager::IsCurrentKillQuestComplete()
{
	if (_currentGoalType == EGsPlayableEventQuestGoalType::Move)
	{
		return false;
	}

	return (_currentGoalKillCount <= _currentProgressKillCount) ? true : false;
}

// 모든 몬스터의 게임 오브젝트를 만든다
void UGsLobbyPlayableEventManager::MakeAllMonster()
{
	UWorld* world = GetWorld();

	// get spawn info actor
	TArray<AActor*> actors;
	UGameplayStatics::GetAllActorsOfClass(world, AGsActorPlayableEventMonsterSpawn::StaticClass(), actors);

	FString aiControllerClasspath = GData()->GetGlobalData()->_playableEventMonsterAiControllerClassPath.ToString() + TEXT("_C");
	TSubclassOf<AController> AIControllerClass = LoadClass<AAIController>(NULL, *aiControllerClasspath, NULL, LOAD_None, NULL);

	for (auto iter : actors)
	{
		UGsGameObjectPlayableEventNonPlayer* playableEventNonPlayer = MakeMonster(iter, AIControllerClass);
		if (nullptr == playableEventNonPlayer)
		{
			continue;
		}

		AGsActorPlayableEventMonsterSpawn* spawnInfo =
			Cast<AGsActorPlayableEventMonsterSpawn>(iter);

		if (spawnInfo == nullptr)
		{
			continue;
		}

		int spawnId = spawnInfo->_spawnId;
		_arrayPlayableEventNonPlayer.Add(playableEventNonPlayer);
		_mapPlayableEventNonPlayer.Add(spawnId, playableEventNonPlayer);
	}
}

void UGsLobbyPlayableEventManager::MakeQuestTargetObject(int In_questId)
{
	UWorld* world = GetWorld();

	// get spawn info actor
	TArray<AActor*> actors;
	UGameplayStatics::GetAllActorsOfClass(world, AGsActorPlayableEventMonsterSpawn::StaticClass(), actors);

	FString aiControllerClasspath = GData()->GetGlobalData()->_playableEventMonsterAiControllerClassPath.ToString() + TEXT("_C");
	TSubclassOf<AController> AIControllerClass = LoadClass<AAIController>(NULL, *aiControllerClasspath, NULL, LOAD_None, NULL);

	for (auto iter : actors)
	{
		AGsActorPlayableEventMonsterSpawn* spawnInfo =
			Cast<AGsActorPlayableEventMonsterSpawn>(iter);

		if (spawnInfo == nullptr ||
			spawnInfo->_activeQuestId != In_questId)
		{
			continue;
		}
		
		UGsGameObjectPlayableEventNonPlayer* playableEventNonPlayer = MakeMonster(iter, AIControllerClass);
		if (nullptr == playableEventNonPlayer)
		{
			continue;
		}

		int spawnId = spawnInfo->_spawnId;
		_arrayPlayableEventNonPlayer.Add(playableEventNonPlayer);
		_mapPlayableEventNonPlayer.Add(spawnId, playableEventNonPlayer);
	}

	TArray<AActor*> bpSpawnActors;
	UGameplayStatics::GetAllActorsOfClass(world, AGsActorPlayableEventBPSpawn::StaticClass(), bpSpawnActors);

	// spawn bp
	for (auto iter : bpSpawnActors)
	{

		AGsActorPlayableEventBPSpawn* spawnInfo =
			Cast<AGsActorPlayableEventBPSpawn>(iter);

		if (spawnInfo == nullptr ||
			spawnInfo->_activeQuestId != In_questId)
		{
			continue;
		}
		AActor* bpActor = MakeBP(iter);

		if (nullptr == bpActor)
		{
			continue;
		}

		int spawnId = spawnInfo->_spawnId;
		_arrayPlayableEventBP.Add(bpActor);
		_mapPlayableEventBP.Add(spawnId, bpActor);
	}

	// despawn bp
	for (auto iter : _arrayPlayableEventBP)
	{
		if (iter == nullptr)
		{
			continue;
		}

		if (UGsPlayableEventBPComponent* bpComponent = Cast<UGsPlayableEventBPComponent>(iter->GetComponentByClass(UGsPlayableEventBPComponent::StaticClass())))
		{
			if (bpComponent->_deactiveQuestId == In_questId)
			{
				AddDespawnListBP(bpComponent->_spawnId);
			}
		}
	}

}
void UGsLobbyPlayableEventManager::ClearAllMonsterSpawnTimmer()
{
	UWorld* world = GetWorld();
	if (nullptr == world)
	{
		return;
	}

	// get spawn info actor	
	TArray<AActor*> actors;
	UGameplayStatics::GetAllActorsOfClass(world, AGsActorPlayableEventMonsterSpawn::StaticClass(), actors);	

	AGsActorPlayableEventMonsterSpawn* spawnInfo = nullptr;
	for (auto iter : actors)
	{
		spawnInfo =	Cast<AGsActorPlayableEventMonsterSpawn>(iter);
		if (nullptr == spawnInfo)
		{
			continue;
		}

		spawnInfo->ClearTimmer();
	}
}
UGsGameObjectPlayableEventNonPlayer* UGsLobbyPlayableEventManager::MakeMonster(AActor* In_spawnActor,
	TSubclassOf<AController> In_aiControllClass)
{
	UWorld* world = GetWorld();
	if (In_spawnActor == nullptr ||
		world == nullptr)
	{
		return nullptr;
	}

	AGsActorPlayableEventMonsterSpawn* spawnInfo =
		Cast<AGsActorPlayableEventMonsterSpawn>(In_spawnActor);

	if (spawnInfo == nullptr)
	{
		return nullptr;
	}

	const FGsSchemaNpcData* npcTable = UGsTableUtil::FindRowById<UGsTableNpcData, FGsSchemaNpcData>(spawnInfo->_tableId);
	if (nullptr == npcTable ||
		true == npcTable->shapeId.IsNull())
	{
		return nullptr;
	}

	const FGsSchemaShapeData* shape = npcTable->shapeId.GetRow();
	if (nullptr == shape)
	{
		return nullptr;
	}

	UGsGameObjectPlayableEventNonPlayer* makeMonster =
		NewObject<UGsGameObjectPlayableEventNonPlayer>(this);

	FVector actorLocation = In_spawnActor->GetActorLocation();
	FRotator actorRotation = In_spawnActor->GetActorRotation();


	UClass* loadObjectC = GResource()->LoadSync<UClass>(shape->bPClass, true);

	FTransform actorTr = In_spawnActor->GetActorTransform();
	// pair FinishSpawning
	ACharacter* monsterChar =
		world->SpawnActorDeferred<ACharacter>(loadObjectC, actorTr, nullptr, nullptr, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);

	if (monsterChar == nullptr)
	{
		return nullptr;
	}

	monsterChar->AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
	monsterChar->AIControllerClass = In_aiControllClass;

	UGsPlayableEventNonPlayerComponent* playableEventNonPlayerComponent =
		NewObject<UGsPlayableEventNonPlayerComponent>(monsterChar, UGsPlayableEventNonPlayerComponent::StaticClass(),
			PLAYABLE_EVENT_NONPLAYER_COMPONENT);

	if (nullptr == playableEventNonPlayerComponent)
	{
		return nullptr;
	}

	playableEventNonPlayerComponent->_tableId = spawnInfo->_tableId;
	playableEventNonPlayerComponent->_useSkillId = spawnInfo->_useSkillId;

	monsterChar->AddOwnedComponent(playableEventNonPlayerComponent);
	playableEventNonPlayerComponent->RegisterComponent();

	// pair SpawnActorDeferred
	monsterChar->FinishSpawning(actorTr);


	int tableId = playableEventNonPlayerComponent->GetTableId();

	FGsSpawnParam spawnParam(EGsGameObjectType::NonPlayer, 0, tableId, actorLocation,
		actorRotation);

	makeMonster->ActorSpawned(monsterChar);

	makeMonster->Initialize(&spawnParam);


	playableEventNonPlayerComponent->SetGameObject(makeMonster);
	playableEventNonPlayerComponent->SetSpawnId(spawnInfo->_spawnId);

	makeMonster->SetSpawnActor(spawnInfo);

	makeMonster->MakeSpawnEffect();

	return makeMonster;
}
AActor* UGsLobbyPlayableEventManager::MakeBP(AActor* In_spawnActor)
{
	UWorld* world = GetWorld();
	if (In_spawnActor == nullptr ||
		world == nullptr)
	{
		return nullptr;
	}

	AGsActorPlayableEventBPSpawn* spawnInfo =
		Cast<AGsActorPlayableEventBPSpawn>(In_spawnActor);

	if (spawnInfo == nullptr)
	{
		return nullptr;
	}

	FVector actorLocation = In_spawnActor->GetActorLocation();
	FRotator actorRotation = In_spawnActor->GetActorRotation();


	UClass* loadObjectC = GResource()->LoadSync<UClass>(spawnInfo->_spawnTargetBP, true);

	FTransform actorTr = In_spawnActor->GetActorTransform();
	// pair FinishSpawning
	AActor* makeBP =
		world->SpawnActorDeferred<AActor>(loadObjectC, actorTr, nullptr, nullptr, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);

	if (makeBP == nullptr)
	{
		return nullptr;
	}

	UGsPlayableEventBPComponent* playableEventBPComponent =
		NewObject<UGsPlayableEventBPComponent>(makeBP, UGsPlayableEventBPComponent::StaticClass(),
			PLAYABLE_EVENT_BP_COMPONENT);

	if (nullptr == playableEventBPComponent)
	{
		return nullptr;
	}
	playableEventBPComponent->_deactiveQuestId = spawnInfo->_deactiveQuestId;
	playableEventBPComponent->_spawnId = spawnInfo->_spawnId;

	makeBP->AddOwnedComponent(playableEventBPComponent);
	playableEventBPComponent->RegisterComponent();

	// pair SpawnActorDeferred
	makeBP->FinishSpawning(actorTr);

	return makeBP;
}
void UGsLobbyPlayableEventManager::RequestMakeMonster(AGsActorPlayableEventMonsterSpawn* In_spawnActor)
{
	if (In_spawnActor == nullptr)
	{
		return;
	}

	FString aiControllerClasspath = GData()->GetGlobalData()->_playableEventMonsterAiControllerClassPath.ToString() + TEXT("_C");
	TSubclassOf<AController> AIControllerClass = LoadClass<AAIController>(NULL, *aiControllerClasspath, NULL, LOAD_None, NULL);

	UGsGameObjectPlayableEventNonPlayer* playableEventNonPlayer = MakeMonster(In_spawnActor, AIControllerClass);
	if (nullptr == playableEventNonPlayer)
	{
		return;
	}

	int spawnId = In_spawnActor->_spawnId;
	_arrayPlayableEventNonPlayer.Add(playableEventNonPlayer);
	_mapPlayableEventNonPlayer.Add(spawnId, playableEventNonPlayer);
}

// 제일 가까운 몬스터 찾기
UGsGameObjectBase* UGsLobbyPlayableEventManager::FindNearTarget()
{
	UGsGameObjectPlayableEventLocalPlayer* local = LSLobbyCharacter()->GetPlayableEventLocalPlayer();
	if (local == nullptr)
	{
		return nullptr;
	}


	FVector2D origin = local->GetLocation2D();

	UGsGameObjectBase* findObj = nullptr;

	// 타겟 검색
	if (_arrayPlayableEventNonPlayer.Num() > 0)
	{
		TArray<TPair<UGsGameObjectBase*, float>> arrFindTargets;
		for (UGsGameObjectBase* iter : _arrayPlayableEventNonPlayer)
		{
			AGsCharacterBase* monChar = iter->GetCharacter();

			if (monChar == nullptr)
			{
				continue;
			}

			UGsPlayableEventNonPlayerComponent* playableEventNonPlayerComponent =
				monChar->FindComponentByClass<UGsPlayableEventNonPlayerComponent>();

			if (playableEventNonPlayerComponent == nullptr)
			{
				continue;
			}


			if (playableEventNonPlayerComponent->IsAlive() == true)
			{
				arrFindTargets.Add(
					TPair<UGsGameObjectBase*, float>(iter, (iter->GetLocation2D() - origin).SizeSquared()));
			}

		}

		if (arrFindTargets.Num() > 0)
		{
			arrFindTargets.Sort([](const TPair<UGsGameObjectBase*, float>& lhs, const TPair<UGsGameObjectBase*, float>& rhs)
				{
					return (lhs.Value < rhs.Value);
				});
			findObj = arrFindTargets[0].Key;
		}
	}
	return findObj;
}

void UGsLobbyPlayableEventManager::DespawnObjectInternal(UGsGameObjectPlayableEventNonPlayer* In_obj)
{
	if (In_obj == nullptr)
	{
		return;
	}

	if (false == _arrayPlayableEventNonPlayer.Contains(In_obj))
	{
		return;
	}

	In_obj->Finalize();
	// 액터 소멸
	if (AActor* actor = In_obj->GetActor())
	{
		actor->Destroy();
		actor->MarkPendingKill();
	}

	In_obj->ClearActor();

	if (In_obj->IsRooted())
	{
		In_obj->RemoveFromRoot();
	}

	In_obj->MarkPendingKill();
}
void UGsLobbyPlayableEventManager::DespawnBPObjectInternal(AActor* In_actor)
{
	if (In_actor == nullptr)
	{
		return;
	}

	if (false == _arrayPlayableEventBP.Contains(In_actor))
	{
		return;
	}

	
	// 액터 소멸	
	In_actor->Destroy();
	In_actor->MarkPendingKill();
}
void UGsLobbyPlayableEventManager::DespawnObject(UGsGameObjectPlayableEventNonPlayer* In_obj)
{
	DespawnObjectInternal(In_obj);
	_arrayPlayableEventNonPlayer.Remove(In_obj);
}
void UGsLobbyPlayableEventManager::DespawnBPObject(AActor* In_actor)
{
	DespawnBPObjectInternal(In_actor);
	_arrayPlayableEventBP.Remove(In_actor);
}

// 모든 몬스터 디스폰
void UGsLobbyPlayableEventManager::DespawnAllMonster(bool In_directDespawn)
{
	// 모든 몬스터 삭제 리스트에 넣기
	for (auto iter : _mapPlayableEventNonPlayer)
	{
		AddDespawnList(iter.Key);
	}

	for (auto iter: _mapPlayableEventBP)
	{
		AddDespawnListBP(iter.Key);
	}

	if (In_directDespawn == true)
	{
		DespawnList();
	}

	ClearAllMonsterSpawnTimmer();
}

void UGsLobbyPlayableEventManager::Update(float inTick)
{
	DespawnList();
}
// 리스트에 저장된 몬스터 삭제
void UGsLobbyPlayableEventManager::DespawnList()
{
	// 삭제할꺼 있으면 삭제
	if (_arrayDespawn.Num() != 0)
	{
		for (int id : _arrayDespawn)
		{
			UGsGameObjectPlayableEventNonPlayer** findObj =
				_mapPlayableEventNonPlayer.Find(id);

			if (findObj == nullptr)
			{
				continue;
			}

			DespawnObject(*findObj);
			_mapPlayableEventNonPlayer.Remove(id);
		}

		_arrayDespawn.Empty();
	}

	
	if (_arrayDespawnBP.Num() != 0)
	{
		for (int id : _arrayDespawnBP)
		{
			AActor** findObj =
				_mapPlayableEventBP.Find(id);

			if (findObj == nullptr)
			{
				continue;
			}

			DespawnBPObject(*findObj);
			_mapPlayableEventBP.Remove(id);
		}

		_arrayDespawnBP.Empty();
	}


}
// 디스폰 리스테에 추가
void UGsLobbyPlayableEventManager::AddDespawnList(int In_spawnId)
{
	if (true == _arrayDespawn.Contains(In_spawnId))
	{
		return;
	}

	_arrayDespawn.Add(In_spawnId);
}
void UGsLobbyPlayableEventManager::AddDespawnListBP(int In_spawnId)
{
	if (true == _arrayDespawnBP.Contains(In_spawnId))
	{
		return;
	}

	_arrayDespawnBP.Add(In_spawnId);
}

void UGsLobbyPlayableEventManager::CollectArrangedNPCAniPlayBP()
{
	UWorld* world = GetWorld();
	if (nullptr == world)
	{
		return;
	}

	TArray<AActor*> bpNpcAniPlay;
	UGameplayStatics::GetAllActorsOfClass(world, AGsActorPlayableEventNPCAniPlay::StaticClass(), bpNpcAniPlay);

	for (auto iter : bpNpcAniPlay)
	{
		AGsActorPlayableEventNPCAniPlay* aniPlay =
			Cast<AGsActorPlayableEventNPCAniPlay>(iter);

		if (aniPlay == nullptr)
		{
			continue;
		}

		int spawnId = aniPlay->_spawnId;
		_arrayPlayableEventNPCAniPlay.Add(aniPlay);
		_mapPlayableEventNPCAniPlay.Add(spawnId, aniPlay);
	}
}

AGsActorPlayableEventNPCAniPlay* UGsLobbyPlayableEventManager::FindAniPlayBP(int In_spawnId)
{
	AGsActorPlayableEventNPCAniPlay** find = _mapPlayableEventNPCAniPlay.Find(In_spawnId);

	if (find == nullptr)
	{
		return nullptr;
	}

	return *find;
}

void UGsLobbyPlayableEventManager::CollectArrangedCameraData()
{
	UWorld* world = GetWorld();
	if (nullptr == world)
	{
		return;
	}

	TArray<AActor*> cameraDatas;
	UGameplayStatics::GetAllActorsOfClass(world, AGsActorPlayableEventCameraData::StaticClass(), cameraDatas);

	for (auto iter : cameraDatas)
	{
		AGsActorPlayableEventCameraData* camData =
			Cast<AGsActorPlayableEventCameraData>(iter);

		if (camData == nullptr)
		{
			continue;
		}

		_arrCamData.Add(camData);
	}

	_arrCamData.Sort([](const AGsActorPlayableEventCameraData& lhs, const AGsActorPlayableEventCameraData& rhs)
		{
			return (int)lhs._isStartData > (int)rhs._isStartData;
		});
}

AGsActorPlayableEventCameraData* UGsLobbyPlayableEventManager::FindCamData(bool In_isFirstCamera)
{
	int index = (In_isFirstCamera == true) ? 0 : 1;
	if (_arrCamData.Num() -1  < index)
	{
		return nullptr;
	}	
	return _arrCamData[index];
}

bool UGsLobbyPlayableEventManager::GetAllIsTestEnterVal()
{
	bool constVal = GData()->GetGlobalData()->_testPlayableEventEnter;
	return constVal || _isTestPlayableEventEnter;
}

void UGsLobbyPlayableEventManager::SetTestCheckPoint(int In_type)
{
	EGsPlayableEventServerCheckPointType newType =
		(EGsPlayableEventServerCheckPointType)In_type;

	_testCheckPointType = newType;
}