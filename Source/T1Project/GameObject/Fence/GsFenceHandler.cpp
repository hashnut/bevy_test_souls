// Fill out your copyright notice in the Description page of Project Settings.


#include "GsFenceHandler.h"

#include "Data/GsGlobalConstant.h"
#include "Data/GsDataContainManager.h"

#include "Map/Fence/GsSchemaFenceElement.h"
#include "Map/Fence/GsSchemaFenceData.h"
#include "Map/GsSchemaMapTerritoryInfo.h"
#include "Map/GsSchemaMapTerritoryData.h"

#include "Management/ScopeGlobal/GsLevelManager.h"
#include "Management/ScopeGlobal/GsSoundManager.h"
#include "Management/ScopeGame/GsGameObjectManager.h"
#include "Management/ScopeGlobal/GsQuestManager.h"
#include "management/GsMessageHolder.h"
#include "Management/ScopeGlobal/GsSoundManager.h"

#include "Message/MessageParam/GsQuestMessageParam.h"
#include "Message/GsMessageGameObject.h"
#include "Message/MessageParam/GsMessageParam.h"
#include "Message/MessageParam/GsGameObjectMessageParam.h"

#include "T1Project.h"

#include "GameObject/ObjectClass/GsGameObjectLocalPlayer.h"

#include "Net/GsNetSendServiceWorld.h"
#include "GsTableManager.h"
#include "Shared/Client/SharedEnums/SharedCommonEnum.h"

#include "UTIL/GsLevelUtil.h"
#include "UTIL/GsSoundUtil.h"
#include "UTIL/GsText.h"
#include "UTIL/GsTableUtil.h"


#include "Sound/GsSchemaSoundResData.h"

#include "Level/Territory/GsTerritoryZoneDefine.h"
#include "DataSchema/Map/GsSchemaMapTerritorySpaceCrack.h"
#include "GsSpaceCrackFence.h"
#include "GsServerFence.h"
#include "GsSoundFence.h"
#include "GsEnvFence.h"
#include "GsFence.h"
#include "GsTownChaosFence.h"
#include "GameObject/Fence/GsServerFence.h"
#include "Message/MessageParam/GsInvasionMessageParam.h"
#include "GsNoCampZoneFence.h"
#include "GsTownFence.h"

void FGsFenceHandler::Initialize(UGsGameObjectBase* inPlayer)
{
	SetPlayer(inPlayer);
	InitializeTerritory();

	_currentTime = 0;

	if (const UGsGlobalConstant* globalData = GData()->GetGlobalData())
	{
		_updateRate = globalData->_clientUpdateRate;
	}

	BindMessage();
}

void FGsFenceHandler::Finalize()
{
	for (TWeakPtr<struct FGsFence> fence : _enteredClientTerritoryList)
	{
		ForceLeftClientFence(fence);
	}

	_player = nullptr;

	UnbindMessage();

	Reset();

	_nonPVPTerritoryArray.Empty();
	_townChaosTerritoryArray.Empty();
	_noCampZoneTerritoryArray.Empty();
	_townTerritoryArray.Empty();

	for (TPair<TerritoryType, TArray<TSharedPtr<FGsFence>>>& pair : _territoryMap)
	{
		for (TSharedPtr<FGsFence> fence : pair.Value)
		{
			fence->Clear();
		}

		pair.Value.Empty();
	}
	_territoryMap.Empty();
}

void FGsFenceHandler::Update(float inDelta)
{
	if (false == _active
		|| false == _player.IsValid()
		|| nullptr == _player->GetActor())
	{
		return;
	}

	_currentTime += inDelta;
	if (_currentTime > _updateRate)
	{
		_currentTime = 0;

		//send packet delay 0.25 seconds
		NotifyServerEnteredFence();
		NotifyServerLeftFence();
		UpdateTerritory();
	}
}


void FGsFenceHandler::BindMessage()
{
	FGsMessageHolder* messageHolder = GMessage();
	if (nullptr == messageHolder)
	{
		return;
	}

	MQuest& qusetMessage = messageHolder->GetQuest();
	_messageContentQuestList.Empty();
	_messageContentQuestList.Emplace(qusetMessage.AddRaw(MessageContentQuest::QUEST_MAIN_ACCEPTED, this, &FGsFenceHandler::OnQuestAccepted));
	_messageContentQuestList.Emplace(qusetMessage.AddRaw(MessageContentQuest::QUEST_MAIN_COMPLETED, this, &FGsFenceHandler::OnQuestComplete));
	_messageContentQuestList.Emplace(qusetMessage.AddRaw(MessageContentQuest::QUEST_SUB_ACCEPTED, this, &FGsFenceHandler::OnQuestAccepted));
	_messageContentQuestList.Emplace(qusetMessage.AddRaw(MessageContentQuest::QUEST_SUB_COMPLETED, this, &FGsFenceHandler::OnQuestComplete));

	MInvasion& invasionMessage = messageHolder->GetInvasion();
	_messageInvasionList.Empty();
	_messageInvasionList.Emplace(invasionMessage.AddRaw(MessageInvasion::INVALID_TOWN_CHAOS_FENCE, this, &FGsFenceHandler::OnUpdatePlayerChaosState));
}

void FGsFenceHandler::UnbindMessage()
{
	FGsMessageHolder* messageHolder = GMessage();
	if (nullptr == messageHolder)
	{
		return;
	}

	MQuest& qusetMessage = GMessage()->GetQuest();
	for (TPair<MessageContentQuest, FDelegateHandle> contentQuestPair : _messageContentQuestList)
	{
		qusetMessage.Remove(contentQuestPair);
	}
	_messageContentQuestList.Empty();

	MInvasion& invasionMessage = GMessage()->GetInvasion();
	for (TPair<MessageInvasion, FDelegateHandle> invasionPair : _messageInvasionList)
	{
		invasionMessage.Remove(invasionPair);
	}
	_messageInvasionList.Empty();
}

void FGsFenceHandler::SetPlayer(UGsGameObjectBase * inPlayer)
{
	if (nullptr == inPlayer)
	{
		return;
	}

	_player = inPlayer->CastGameObject<UGsGameObjectLocalPlayer>();
}

void FGsFenceHandler::Reset()
{
	_enteredServerTerritoryListForSend.Empty();
	_enteredServerTerritoryListForWaitAck.Empty();

	_leftServerTerritoryListForSend.Empty();
	_leftServerTerritoryListForWaitAck.Empty();

	_enteredServerTerritoryList.Empty();
	_enteredClientTerritoryList.Empty();
}

void FGsFenceHandler::UpdateEnterFence()
{
	for (TPair<TerritoryType, TArray<TSharedPtr<FGsFence>>> pair : _territoryMap)
	{
		for (TSharedPtr<FGsFence> territory : pair.Value)
		{
			if (false == territory.IsValid())
			{
				continue;
			}

			if (IsClientFence(pair.Key))
			{
				UpdateClientFenceEnter(territory);
			}
			else
			{
				UpdatesServerFenceEnter(territory.Get());
			}
		}
	}
}

void FGsFenceHandler::UpdateLeftFence()
{
	for (TWeakPtr<FGsFence> territory : _enteredServerTerritoryList)
	{
		if (false == territory.IsValid()
			|| false == territory.Pin()->IsValid())
		{
			continue;
		}

		UpdatesServerFenceLeft(territory.Pin().Get());
	}

	for (TWeakPtr<FGsFence> territory : _enteredClientTerritoryList)
	{
		if (false == territory.IsValid()
			|| false == territory.Pin()->IsValid())
		{
			continue;
		}

		UpdatesClientFenceLeft(territory.Pin());
	}

	for (TWeakPtr<FGsFence> territory : _leftClientTerritoryList)
	{
		if (false == territory.IsValid()
			|| false == territory.Pin()->IsValid())
		{
			continue;
		}

		_enteredClientTerritoryList.Remove(territory);
	}

	_leftClientTerritoryList.Empty();
}

void FGsFenceHandler::UpdateTerritory()
{
	if(false == _player.IsValid())
	{
		return;
	}

	AActor* actor = _player->GetActor();
	if (nullptr == actor)
	{
		return;
	}

	_playerLocation = actor->GetActorLocation();

	UpdateEnterFence();
	UpdateLeftFence();
}

void FGsFenceHandler::NotifyServerEnteredFence()
{
	for (int64 territoryId : _enteredServerTerritoryListForSend)
	{
		if (_enteredServerTerritoryListForWaitAck.Contains(territoryId))
		{
			GSLOG(Error, TEXT("Already send territory enter packet - territory id : %lld"), territoryId);
			continue;
		}

		_enteredServerTerritoryListForWaitAck.Add(territoryId);
		FGsNetSendServiceWorld::SendEnterTerritory(territoryId, _playerLocation);
	}

	_enteredServerTerritoryListForSend.Empty();
}

void FGsFenceHandler::NotifyServerLeftFence()
{
	for (int64 territoryId : _leftServerTerritoryListForSend)
	{
		if (_leftServerTerritoryListForWaitAck.Contains(territoryId))
		{
			GSLOG(Error, TEXT("Already send territory left packet - territory id : %lld"), territoryId);
			continue;
		}

		_leftServerTerritoryListForWaitAck.Add(territoryId);
		FGsNetSendServiceWorld::SendLeftTerritory(territoryId, _playerLocation);
	}

	_leftServerTerritoryListForSend.Empty();
}

void FGsFenceHandler::InitializeTerritory()
{
	_nonPVPTerritoryArray.Empty();
	_townChaosTerritoryArray.Empty();
	_noCampZoneTerritoryArray.Empty();
	_townTerritoryArray.Empty();

	//initialize fence map
	TArray<TSharedPtr<FGsFence>> temp;
	int32 num = (int32)TerritoryType::MAX;
	for (int32 i = 0; i < num; ++i)
	{
		TerritoryType territroyType = (TerritoryType)i;
		if (TerritoryType::MAX == territroyType
			|| TerritoryType::NONE == territroyType
			|| TerritoryType::NO_CAMP_ZONE == territroyType)
		{
			continue;
		}

		_territoryMap.Add((TerritoryType)i, temp);
	}

	int32 levelKey = GLevel()->GetCurrentLevelId();
	FString levelKeyString = GLevel()->GetLevelIdString();
	FString tableName = UGsTableUtil::GetCustomTerritoryDataTableName<UGsTableMapTerritoryData>(levelKeyString);
	const UGsTable* mapTerritoryTable = UGsTableManager::GetInstance().GetTable(FGsSchemaMapTerritoryData::StaticStruct(), tableName);
	if (nullptr == mapTerritoryTable)
	{
		return;
	}

	const FGsSchemaMapTerritoryData* mapTerritoryData;
	mapTerritoryTable->FindRow<FGsSchemaMapTerritoryData>(*levelKeyString, mapTerritoryData);
	if (nullptr == mapTerritoryTable)
	{
		GSLOG(Error, TEXT("Missing safetyZone - id : %d"), levelKey);
		return;
	}

	FString fenceTableName = UGsTableUtil::GetCustomTerritoryDataTableName<UGsTableFenceData>(FString::FromInt(levelKey));
	const UGsTable* fenceTable = UGsTableManager::GetInstance().GetTable(FGsSchemaFenceData::StaticStruct(), fenceTableName);
	if (nullptr == fenceTable)
	{
		return;
	}

	const FGsSchemaFenceData* fenceData = nullptr;
	fenceTable->FindRow<FGsSchemaFenceData>(*levelKeyString, fenceData);
	if (nullptr == fenceData)
	{
		return;
	}

	const TArray<FGsSchemaFenceElement>& fenceArray = fenceData->FenceList;

	InitServerFence(mapTerritoryData, fenceArray);
	InitClientFence(mapTerritoryData, fenceArray);
	InitNoCampZoneFence(mapTerritoryData, fenceArray);
}

void FGsFenceHandler::InitServerFence(const struct FGsSchemaMapTerritoryData* inMapTerritoryData, const TArray<FGsSchemaFenceElement>& inFenceArray)
{
	for (const FGsSchemaMapTerritoryInfo& territoryInfo : inMapTerritoryData->territoryList)
	{
		if (IsClientFence(territoryInfo.territoryType))
		{
			GSLOG(Error, TEXT("Territory type is max - id : %lld"), territoryInfo.territoryId);
			continue;
		}

		const FGsSchemaFenceElement* findFence = inFenceArray.FindByPredicate([&](const FGsSchemaFenceElement& fence)->bool {
			return fence.FenceId == territoryInfo.fenceId;
			});
		if (nullptr == findFence)
		{
			GSLOG(Error, TEXT("Missing fence - id : %lld"), territoryInfo.fenceId);
			continue;
		}

		TSharedPtr<FGsFence> fence;
		switch (territoryInfo.territoryType)
		{
		case TerritoryType::TOWN_CHAOS:
		{
			fence = MakeShareable(new FGsTownChaosFence());			
			_townChaosTerritoryArray.Add(StaticCastSharedPtr<FGsTownChaosFence>(fence));
		}
		break;
		case TerritoryType::TOWN:
		{
			fence = MakeShareable(new FGsTownFence());
			_townTerritoryArray.Add(fence);
			_nonPVPTerritoryArray.Add(fence);
		}
		break;
		case TerritoryType::ARENA_READY:
		case TerritoryType::ARRIVAL:
		case TerritoryType::SAFE_ZONE:
		case TerritoryType::NON_PK:
		{
			fence = MakeShareable(new FGsServerFence());
			_nonPVPTerritoryArray.Add(fence);
		}
		break;
		case TerritoryType::ARENA:
		{
			fence = MakeShareable(new FGsServerFence());
		}
		break;
		}

		if (false == fence.IsValid())
		{
			continue;
		}

		FGsServerFence* serverFence = static_cast<FGsServerFence*>(fence.Get());
		if (nullptr == serverFence)
		{
			continue;
		}

		serverFence->Initialize(&territoryInfo, findFence);

		if (false == _territoryMap.Contains(fence->GetTerritoryType()))
		{
			GSLOG(Error, TEXT("Territory type is missing - id : %lld"), fence->GetTerritoryID());
			continue;
		}

		_territoryMap[fence->GetTerritoryType()].Add(fence);
	}
}

void FGsFenceHandler::InitClientFence(const struct FGsSchemaMapTerritoryData* inMapTerritoryData, const TArray<FGsSchemaFenceElement>& inFenceArray)
{
	//sound
	for (const FGsSchemaMapTerritorySound& soundInfo : inMapTerritoryData->soundTerritoryList)
	{
		const FGsSchemaFenceElement* findFence = inFenceArray.FindByPredicate([&](const FGsSchemaFenceElement& fence)->bool {
			return fence.FenceId == soundInfo.fenceId;
			});
		if (nullptr == findFence)
		{
			GSLOG(Error, TEXT("Missing fence - id : %lld"), soundInfo.fenceId);
			continue;
		}

		FGsSoundFence* soundFence = new FGsSoundFence();
		if (nullptr == soundFence)
		{
			continue;
		}

		soundFence->InitSoundFence(&soundInfo, findFence);

		TSharedPtr<FGsFence> fence = MakeShareable(soundFence);
		if (false == fence.IsValid())
		{
			continue;
		}

		if (false == _territoryMap.Contains(fence->GetTerritoryType()))
		{
			GSLOG(Error, TEXT("Territory type is missing - id : %lld"), fence->GetTerritoryID());
			continue;
		}

		_territoryMap[fence->GetTerritoryType()].Add(fence);
	}

	//env
	for (const FGsSchemaMapTerritoryEnv& envInfo : inMapTerritoryData->envTerritoryList)
	{
		const FGsSchemaFenceElement* findFence = inFenceArray.FindByPredicate([&](const FGsSchemaFenceElement& fence)->bool {
			return fence.FenceId == envInfo.fenceId;
			});
		if (nullptr == findFence)
		{
			GSLOG(Error, TEXT("Missing fence - id : %lld"), envInfo.fenceId);
			continue;
		}

		FGsEnvFence* envFence = new FGsEnvFence();
		if (nullptr == envFence)
		{
			continue;
		}

		envFence->InitEnvFence(&envInfo, findFence);

		TSharedPtr<FGsFence> fence = MakeShareable(envFence);
		if (false == fence.IsValid())
		{
			continue;
		}

		if (false == _territoryMap.Contains(fence->GetTerritoryType()))
		{
			GSLOG(Error, TEXT("Territory type is missing - id : %lld"), fence->GetTerritoryID());
			continue;
		}

		_territoryMap[fence->GetTerritoryType()].Add(fence);
	}

	//spacecrack
	for (const FGsSchemaMapTerritorySpaceCrack& spaceCrackInfo : inMapTerritoryData->spaceCrackZoneList)
	{
		const FGsSchemaFenceElement* findFence = inFenceArray.FindByPredicate([&](const FGsSchemaFenceElement& fence)->bool {
			return fence.FenceId == spaceCrackInfo.fenceId;
			});
		if (nullptr == findFence)
		{
			GSLOG(Error, TEXT("Missing fence - id : %lld"), spaceCrackInfo.fenceId);
			continue;
		}

		FGsSpaceCrackFence* spaceCrackFence = new FGsSpaceCrackFence();
		if (nullptr == spaceCrackFence)
		{
			continue;
		}

		spaceCrackFence->InitSpaceCrack(&spaceCrackInfo, findFence);

		TSharedPtr<FGsFence> fence = MakeShareable(spaceCrackFence);
		if (false == fence.IsValid())
		{
			continue;
		}

		if (false == _territoryMap.Contains(fence->GetTerritoryType()))
		{
			GSLOG(Error, TEXT("Territory type is missing - id : %lld"), fence->GetTerritoryID());
			continue;
		}

		_territoryMap[fence->GetTerritoryType()].Add(fence);
	}
}

void FGsFenceHandler::InitNoCampZoneFence(const struct FGsSchemaMapTerritoryData* inMapTerritoryData, const TArray<FGsSchemaFenceElement>& inFenceArray)
{
	for (const FGsSchemaMapTerritoryInfo& info : inMapTerritoryData->noCampZoneList)
	{
		const FGsSchemaFenceElement* findFence = inFenceArray.FindByPredicate([&](const FGsSchemaFenceElement& fence)->bool {
			return fence.FenceId == info.fenceId;
			});
		if (nullptr == findFence)
		{
			GSLOG(Error, TEXT("Missing fence - id : %lld"), info.fenceId);
			continue;
		}

		TSharedPtr<FGsNoCampZoneFence> fence = MakeShareable(new FGsNoCampZoneFence());
		if (false == fence.IsValid())
		{
			continue;
		}

		fence.Get()->Initialize(&info, findFence);

		_noCampZoneTerritoryArray.Emplace(fence);
	}
}

bool FGsFenceHandler::TryGetAllOverlapedServerFence(TArray<TSharedPtr<struct FGsFence>>& outFenceList, const FVector& inLocation)
{
	for (TPair<TerritoryType, TArray<TSharedPtr<FGsFence>>> pair : _territoryMap)
	{
		for (TSharedPtr<FGsFence> fence : pair.Value)
		{
			if (false == fence.IsValid()
				|| false == fence->IsValid())
			{
				continue;
			}

			if (false == IsServerFence(fence->GetTerritoryType()))
			{
				continue;
			}

			if (false == fence->IsInFence(inLocation))
			{
				continue;
			}

			outFenceList.Add(fence);
		}
	}

	return (outFenceList.Num() > 0);
}

bool FGsFenceHandler::TryGetFirstPrioritySoundFence(FGsFence*& outFence, const FVector& inLocation)
{
	TArray<TSharedPtr<FGsFence>> fenceArray;
	if (false == TryGetAllOverlapedFence(fenceArray, inLocation))
	{
		return false;
	}

	TArray<TSharedPtr<FGsFence>> soundFenceArray;
	for (TSharedPtr<FGsFence> fence : fenceArray)
	{
		//1. 사운드 재생이 가능한지 체크, 서버 펜스 모두 사운드 플레이 가능, 클라이언트 펜스 중에는 사운드 펜스만 가능 
		//2. 퀘스트 조건등이 있다면 설정, 서버 펜스 모두 조건이 없음, 클라이언트 펜스는 조건 존재 
		if (fence->IsCanSoundPlay()
			&& fence->IsActive())
		{
			soundFenceArray.Add(fence);
		}
	}

	if (0 == soundFenceArray.Num())
	{
		return false;
	}

	soundFenceArray.Sort([](TSharedPtr<FGsFence> inSource, TSharedPtr<FGsFence> inDest)->bool {
		return FGsFenceHandler::GetSoundPriority(inSource->GetTerritoryType()) > FGsFenceHandler::GetSoundPriority(inDest->GetTerritoryType());
		});

	outFence = soundFenceArray[0].Get();

	return true;
}

bool FGsFenceHandler::TryGetAllOverlapedFence(TArray<TSharedPtr<FGsFence>>& outFenceList, const FVector& inLocation)
{
	for (TPair<TerritoryType, TArray<TSharedPtr<FGsFence>>> pair : _territoryMap)
	{
		for (TSharedPtr<FGsFence> fence : pair.Value)
		{
			if (false == fence.IsValid()
				|| false == fence->IsValid())
			{
				continue;
			}

			if (false == fence->IsInFence(inLocation))
			{
				continue;
			}

			outFenceList.Add(fence);
		}
	}

	return (outFenceList.Num() > 0);
}

bool FGsFenceHandler::IsInNonPVPFence(const FVector& inLocation)
{
	for (TWeakPtr<struct FGsFence> fence : _nonPVPTerritoryArray)
	{
		if (false == fence.IsValid()
			|| false == fence.Pin()->IsValid())
		{
			continue;
		}

		if (false == fence.Pin()->IsInFence(inLocation))
		{
			continue;
		}

		return true;
	}

	for (TWeakPtr<struct FGsTownChaosFence> townChaos : _townChaosTerritoryArray)
	{
		if (false == townChaos.IsValid()
			|| false == townChaos.Pin()->IsValid())
		{
			continue;
		}

		if (false == townChaos.Pin()->IsInFence(inLocation))
		{
			continue;
		}

		if (townChaos.Pin()->IsChaos())
		{
			continue;
		}

		return true;
	}

	return false;
}

bool FGsFenceHandler::IsInTownFence(const FVector& inLocation)
{
	for (TWeakPtr<struct FGsFence> fence : _townTerritoryArray)
	{
		if (false == fence.IsValid()
			|| false == fence.Pin()->IsValid())
		{
			continue;
		}

		if (false == fence.Pin()->IsInFence(inLocation))
		{
			continue;
		}

		return true;
	}

	for (TWeakPtr<struct FGsTownChaosFence> townChaos : _townChaosTerritoryArray)
	{
		if (false == townChaos.IsValid()
			|| false == townChaos.Pin()->IsValid())
		{
			continue;
		}

		if (false == townChaos.Pin()->IsInFence(inLocation))
		{
			continue;
		}

		if (townChaos.Pin()->IsChaos())
		{
			continue;
		}

		return true;
	}

	return false;
}

bool FGsFenceHandler::TryGetAllFence(TArray<TSharedPtr<FGsFence>>& outFenceList, TerritoryType inType)
{
	if (_territoryMap.Contains(inType))
	{
		outFenceList = _territoryMap[inType];
		return true;
	}

	return false;
}

bool FGsFenceHandler::TryGetNoCampZoneFence(TArray<TSharedPtr<struct FGsNoCampZoneFence>>& outNoCampZoneFenceList)
{
	outNoCampZoneFenceList = _noCampZoneTerritoryArray;

	return true;
}

bool FGsFenceHandler::TryGetAllOverlapedClientFence(TArray<TSharedPtr<FGsFence>>& outFenceList, const FVector& inLocation)
{
	TArray<TSharedPtr<FGsFence>> fenceArray;
	TryGetAllOverlapedFence(fenceArray, inLocation);

	for (TSharedPtr<FGsFence> fence : fenceArray)
	{
		if (false == IsClientFence(fence->GetTerritoryType()))
		{
			continue;
		}

		outFenceList.Add(fence);
	}

	return (outFenceList.Num() > 0);
}

TWeakPtr<FGsFence> FGsFenceHandler::GetEnteredTerritory(int64 inTerritoryId)
{
	TWeakPtr<FGsFence>* find = _enteredServerTerritoryList.FindByPredicate([&](const TWeakPtr<FGsFence> fence)->bool {

		if (fence.IsValid()
			&& fence.Pin()->IsValid())
		{
			return fence.Pin()->GetTerritoryID() == inTerritoryId;
		}

		return false;
		});

	if (find
		&& find->IsValid())
	{
		return (*find);
	}

	return nullptr;
}

TWeakPtr<FGsFence> FGsFenceHandler::GetTerritory(int64 inTerritoryId)
{
	int32 num = 0;
	for (const TPair<TerritoryType, TArray<TSharedPtr<FGsFence>>>& pair : _territoryMap)
	{
		num = pair.Value.Num();
		for (int32 i = 0; i < num; ++i)
		{
			if (false == pair.Value[i].IsValid() ||
				false == pair.Value[i]->IsValid())
			{
				continue;
			}

			if (pair.Value[i]->GetTerritoryID() == inTerritoryId)
			{
				return pair.Value[i];
			}
		}
	}

	return nullptr;
}

bool FGsFenceHandler::IsEnteredTerritory(int64 inTerritoryId)
{
	return(nullptr != GetEnteredTerritory(inTerritoryId));
}

bool FGsFenceHandler::AddEnteredTerritory(int64 inTerritoryId)
{
	if (IsEnteredTerritory(inTerritoryId))
	{
		return false;
	}

	TWeakPtr<FGsFence> find = GetTerritory(inTerritoryId);
	if (false == find.IsValid()
		|| false == find.Pin()->IsValid())
	{
		return false;
	}

#if TERRITORY_DEBUG
	FString territoryType = FGsTextUtil::GetStringFromEnum(TEXT("TerritoryType"), find.Pin().Get()->GetTerritoryType());
	GSLOG(Log, TEXT("recieve enter territory - teritory id : %lld\ttype : %s"), find.Pin().Get()->GetTerritoryID(), *territoryType);
#endif		

	_enteredServerTerritoryListForWaitAck.Remove(find.Pin()->GetTerritoryID());
	_enteredServerTerritoryList.Add(find);

	GMessage()->GetGameObject().SendMessage(MessageGameObject::TERRITORY_ENTERED, find.Pin().Get());

	return true;
}

bool FGsFenceHandler::RemoveEnterTerritory(int64 inTerritoryId)
{
	TWeakPtr<FGsFence>* find = _enteredServerTerritoryList.FindByPredicate([&](const TWeakPtr<FGsFence> fence)->bool {

		if (fence.IsValid()
			&& fence.Pin()->IsValid())
		{
			return fence.Pin()->GetTerritoryID() == inTerritoryId;
		}

		return false;

		});

	if (nullptr == find)
	{
		return false;
	}

	TWeakPtr<FGsFence> territory = (*find);
	if (false == territory.IsValid()
		|| false == territory.Pin()->IsValid())
	{
		return false;
	}

	FGsFence* fence = territory.Pin().Get();
#if TERRITORY_DEBUG
	FString territoryType = FGsTextUtil::GetStringFromEnum(TEXT("TerritoryType"), fence->GetTerritoryType());
	GSLOG(Log, TEXT("recieve remove territory - teritory id : %lld\ttype : %s"), fence->GetTerritoryID(), *territoryType);
#endif		

	_leftServerTerritoryListForWaitAck.Remove(fence->GetTerritoryID());
	_enteredServerTerritoryList.Remove(territory);

	fence->Leave();

	GMessage()->GetGameObject().SendMessage(MessageGameObject::TERRITORY_LEFT, fence);

	return true;
}

void FGsFenceHandler::SetActive(bool inActive)
{
	_active = inActive;
}

//Stage에 영향을 미칠 수 있는 서버 펜스
bool FGsFenceHandler::IsServerFence(TerritoryType inTerritoryType)
{
	switch (inTerritoryType)
	{
	case TerritoryType::TOWN:
	case TerritoryType::TOWN_CHAOS:
	case TerritoryType::SAFE_ZONE:
	case TerritoryType::ARENA:
	case TerritoryType::ARENA_READY:
	case TerritoryType::ARRIVAL:
	case TerritoryType::NON_PK:
	{
		return true;
	}
	break;
	}

	return false;
}

// 포괄적 의미를 마을 타입을 정의 한다.
bool FGsFenceHandler::IsTown(TerritoryType inTerritoryType)
{
	if (inTerritoryType == TerritoryType::TOWN ||
		inTerritoryType == TerritoryType::TOWN_CHAOS)
	{
		return true;
	}
	return false;
}

bool FGsFenceHandler::CanCreateCampZone(const FVector& inLocation)
{
	FGsFence* fence = nullptr;
	/*
	if (TryGetPlayerTopPriorityServerFence(fence))
	{
		if (nullptr != fence)
		{
			if (TerritoryType::TOWN_CHAOS == fence->GetTerritoryType() || TerritoryType::TOWN == fence->GetTerritoryType()
				|| TerritoryType::SAFE_ZONE == fence->GetTerritoryType() || TerritoryType::ARENA == fence->GetTerritoryType()
				|| TerritoryType::ARENA_READY == fence->GetTerritoryType())
			{
				return false;
			}
		}
	}
	*/
	if (TryGetTopPriorityServerFence(fence, inLocation))
	{
		if (nullptr != fence)
		{
			if (TerritoryType::TOWN_CHAOS == fence->GetTerritoryType() || TerritoryType::TOWN == fence->GetTerritoryType()
				|| TerritoryType::SAFE_ZONE == fence->GetTerritoryType() || TerritoryType::ARENA == fence->GetTerritoryType()
				|| TerritoryType::ARENA_READY == fence->GetTerritoryType())
			{
				return false;
			}
		}
	}

	for (TSharedPtr<struct FGsNoCampZoneFence > noCampZone : _noCampZoneTerritoryArray)
	{
		if (false == noCampZone.IsValid())
		{
			continue;
		}

		if (noCampZone.Get()->IsInFence(inLocation))
		{
			return false;
		}
	}

	return true;
}

void FGsFenceHandler::ShowState()
{
	GSLOG(Log, TEXT("_enteredTerritoryListForSend - num : %d"), _enteredServerTerritoryListForSend.Num());
	GSLOG(Log, TEXT("_enteredTerritoryListForWaitAck - num : %d"), _enteredServerTerritoryListForWaitAck.Num());
	GSLOG(Log, TEXT("_enteredTerritoryList - num : %d"), _enteredServerTerritoryList.Num());
	GSLOG(Log, TEXT("_leftTerritoryListForSend - num : %d"), _leftServerTerritoryListForSend.Num());
	GSLOG(Log, TEXT("_leftTerritoryListForWaitAck - num : %d"), _leftServerTerritoryListForWaitAck.Num());
}

int32 FGsFenceHandler::GetSoundPriority(TerritoryType inTerritoryType)
{
	switch (inTerritoryType)
	{
	case TerritoryType::SOUND:
	{
		return 4;
	}
	break;
	case TerritoryType::ARENA:
	{
		return 3;
	}
	break;
	case TerritoryType::ARENA_READY:
	{
		return 2;
	}
	break;
	case TerritoryType::TOWN:
	case TerritoryType::TOWN_CHAOS:
	case TerritoryType::SAFE_ZONE:
	case TerritoryType::ARRIVAL:
	{
		return 1;
	}
	break;
	}

	return 0;
}

FString FGsFenceHandler::ToString()
{
	FString log;
	log += TEXT("====================== print fence start ==========================\n");

	log += TEXT("_enteredServerTerritoryListForSend : ");
	for (int64 id : _enteredServerTerritoryListForSend)
	{
		log += FString::Printf(TEXT("%lld,\t"), id);
	}
	log += TEXT("\n");

	log += TEXT("_enteredServerTerritoryListForWaitAck : ");
	for (int64 id : _enteredServerTerritoryListForWaitAck)
	{
		log += FString::Printf(TEXT("%lld,\t"), id);
	}
	log += TEXT("\n");

	log += TEXT("_leftServerTerritoryListForSend : ");
	for (int64 id : _leftServerTerritoryListForSend)
	{
		log += FString::Printf(TEXT("%lld,\t"), id);
	}
	log += TEXT("\n");

	log += TEXT("_leftServerTerritoryListForWaitAck : ");
	for (int64 id : _leftServerTerritoryListForWaitAck)
	{
		log += FString::Printf(TEXT("%lld,\t"), id);
	}
	log += TEXT("\n");

	log += TEXT("_enteredServerTerritoryList : ");
	for (TWeakPtr<FGsFence> fence : _enteredServerTerritoryList)
	{
		log += FString::Printf(TEXT("%lld,\t"), fence.Pin()->GetFenceID());
	}
	log += TEXT("\n");

	log += TEXT("_enteredClientTerritoryList : ");
	for (TWeakPtr<FGsFence> fence : _enteredClientTerritoryList)
	{
		log += FString::Printf(TEXT("%lld,\t"), fence.Pin()->GetFenceID());
	}
	log += TEXT("\n");

	log += TEXT("_leftClientTerritoryList : ");
	for (TWeakPtr<FGsFence> fence : _leftClientTerritoryList)
	{
		log += FString::Printf(TEXT("%lld,\t"), fence.Pin()->GetFenceID());
	}
	log += TEXT("\n");

	log += TEXT("====================== print fence finish ==========================\n");

	return log;
}

bool FGsFenceHandler::IsEnteredTerritoryType(TerritoryType inType)
{
	for (TWeakPtr<FGsFence> territory : _enteredServerTerritoryList)
	{
		if (territory.IsValid()
			&& territory.Pin()->IsValid())
		{
			if (inType == territory.Pin()->GetTerritoryType())
			{
				return true;
			}
		}
	}

	return false;
}

bool FGsFenceHandler::IsPlayerInNonPVPFence()
{
	UGsGameObjectBase* find = GSGameObject()->FindObject(EGsGameObjectType::LocalPlayer);
	if (nullptr == find)
	{
		return false;
	}

	UGsGameObjectLocalPlayer* player = find->CastGameObject<UGsGameObjectLocalPlayer>();
	if (nullptr == player)
	{
		return false;
	}

	AGsCharacterBase* playerCharacter = player->GetCharacter();
	if (nullptr == playerCharacter)
	{
		return false;
	}

	return IsInNonPVPFence(playerCharacter->GetActorLocation());
}

bool FGsFenceHandler::IsPlayerInTownFence()
{
	FGsFence* fence = nullptr;
	if (TryGetPlayerTopPriorityServerFence(fence))
	{
		switch (fence->GetTerritoryType())
		{
		case TerritoryType::TOWN:
		{
			return true;
		}
		break;
		case TerritoryType::TOWN_CHAOS:
		{
			FGsTownChaosFence* townChaos = static_cast<FGsTownChaosFence*>(fence);
			if (townChaos)
			{
				if (false == townChaos->IsChaos())
				{
					return true;
				}
			}
		}
		}
	}

	return false;
}

bool FGsFenceHandler::TryGetPlayerTopPriorityServerFence(OUT FGsFence*& outFence)
{
	UGsGameObjectBase* find = GSGameObject()->FindObject(EGsGameObjectType::LocalPlayer);
	if (nullptr == find)
	{
		return false;
	}

	UGsGameObjectLocalPlayer* player = find->CastGameObject<UGsGameObjectLocalPlayer>();
	if (nullptr == player)
	{
		return false;
	}

	AGsCharacterBase* playerCharacter = player->GetCharacter();
	if (nullptr == playerCharacter)
	{
		return false;
	}

	return TryGetTopPriorityServerFence(outFence, playerCharacter->GetActorLocation());
}

bool FGsFenceHandler::TryGetTopPriorityServerFence(OUT FGsFence*& outFence, const FVector& inLocation)
{
	TArray<TSharedPtr<FGsFence>> fenceList;
	if (false == TryGetAllOverlapedServerFence(fenceList, inLocation))
	{
		return false;
	}

	if (0 == fenceList.Num())
	{
		return false;
	}

	fenceList.Sort([](TSharedPtr<FGsFence> inSource, TSharedPtr<FGsFence> inDest)->bool {
		return SharedFunc::GetTerritoryPriority(inSource->GetTerritoryType()) > SharedFunc::GetTerritoryPriority(inDest->GetTerritoryType());
		});

	outFence = fenceList[0].Get();

	return true;
}

//client 전용 펜스에 플레이엉가 들어와 있는가?
bool FGsFenceHandler::UpdateClientFenceEnter(TWeakPtr<struct FGsFence> inFence)
{
	if (false == inFence.IsValid())
	{
		return false;
	}

	if (false == inFence.Pin()->IsInFence(_playerLocation))
	{
		return false;
	}

	if (_enteredClientTerritoryList.Contains(inFence))
	{
		return false;
	}

	if (false == inFence.Pin()->IsActive())
	{
		return false;
	}

	_enteredClientTerritoryList.Emplace(inFence);
	if (inFence.Pin()->Enter())
	{
#if TERRITORY_DEBUG
		FString territoryType = FGsTextUtil::GetStringFromEnum(TEXT("TerritoryType"), inFence.Pin()->GetTerritoryType());
		GSLOG(Log, TEXT("enter territory - teritory id : %lld\ttype : %s"), inFence.Pin()->GetTerritoryID(), *territoryType);
#endif	
	}

	return false;
}

/*
* 플레이어가 client fence를 떠났는가?
*/
bool FGsFenceHandler::UpdatesClientFenceLeft(TWeakPtr<struct FGsFence> inFence)
{
	if (false == inFence.IsValid())
	{
		return false;
	}

	if (inFence.Pin()->IsActive()
		&& inFence.Pin()->IsInFence(_playerLocation))
	{
		return false;
	}

	return UpdatesClientFenceLeft_Internal(inFence);
}

bool FGsFenceHandler::ForceLeftClientFence(TWeakPtr<struct FGsFence> inFence)
{
	return UpdatesClientFenceLeft_Internal(inFence);
}

bool FGsFenceHandler::UpdatesClientFenceLeft_Internal(TWeakPtr<struct FGsFence> inFence)
{
	if (false == inFence.IsValid())
	{
		return false;
	}

	if (false == _enteredClientTerritoryList.Contains(inFence))
	{
		return false;
	}

	_leftClientTerritoryList.Add(inFence);

	if (inFence.Pin()->Leave())
	{
#if TERRITORY_DEBUG
		FString territoryType = FGsTextUtil::GetStringFromEnum(TEXT("TerritoryType"), inFence.Pin()->GetTerritoryType());
		GSLOG(Log, TEXT("send left territory - teritory id : %lld\ttype : %s"), inFence.Pin()->GetTerritoryID(), *territoryType);
#endif	
	}

	return true;
}

//플레이어가 server fence에 들어 왔는지 확인
bool FGsFenceHandler::UpdatesServerFenceEnter(const struct FGsFence* inFence)
{
	//if wait for send packet, don't check
	if (_enteredServerTerritoryListForSend.Contains(inFence->GetTerritoryID()))
	{
		return false;
	}

	//if wait for ack enter packet, don't check
	if (_enteredServerTerritoryListForWaitAck.Contains(inFence->GetTerritoryID()))
	{
		return false;
	}

	//only none state territory check
	if (IsEnteredTerritory(inFence->GetTerritoryID()))
	{
		return false;
	}

	if (false == inFence->IsInFence(_playerLocation))
	{
		return false;
	}

	_enteredServerTerritoryListForSend.Emplace(inFence->GetTerritoryID());

#if TERRITORY_DEBUG
	FString territoryType = FGsTextUtil::GetStringFromEnum(TEXT("TerritoryType"), inFence->GetTerritoryType());
	GSLOG(Log, TEXT("send add territory - teritory id : %lld\ttype : %s"), inFence->GetTerritoryID(), *territoryType);
#endif	

	return true;
}

//플레이어가 server fence에 들어 왔는지 확인
bool FGsFenceHandler::UpdatesServerFenceLeft(const struct FGsFence* inFence)
{
	if (_leftServerTerritoryListForSend.Contains(inFence->GetTerritoryID()))
	{
		return false;
	}

	if (_leftServerTerritoryListForWaitAck.Contains(inFence->GetTerritoryID()))
	{
		return false;
	}

	if (inFence->IsInFence(_playerLocation))
	{
		return false;
	}

	_leftServerTerritoryListForSend.Add(inFence->GetTerritoryID());

#if TERRITORY_DEBUG
	FString territoryType = FGsTextUtil::GetStringFromEnum(TEXT("TerritoryType"), inFence->GetTerritoryType());
	GSLOG(Log, TEXT("left territory - teritory id : %lld\ttype : %s"), inFence->GetTerritoryID(), *territoryType);
#endif	

	return true;
}

void FGsFenceHandler::OnUpdatePlayerChaosState(const struct IGsMessageParam* inParam)
{
	//플레이어가 마을 무법지대에 에있는데 스테이지가 마을 스테이지이다.
	//하지만 서버에서 무법 지대에 있다고 한다면
	//마을 스테이지에서 떠나게 한다. 
	//마을 무법 지대를 무법 지대 상태로 변경하고 강제 업데이트

	//플레이어가 마을 무법지대에 에있는데 스테이지가 무법 스테이지이다.
	//하지만 서버에서 마을 지대에 있다고 한다면
	//무법 스테이지에서 떠나게 한다. 
	//마을 무법 지대를 무법 지대 상태로 변경하고 강제 업데이트

	const FGsInvasionChaosParam* param = inParam->Cast<const FGsInvasionChaosParam>();
	if (nullptr == param)
	{
		return;
	}

	bool isChaos = param->_isChaos;

	AActor* actor = _player->GetActor();
	if (nullptr == actor)
	{
		return;
	}

	FVector location = actor->GetActorLocation();

	FGsFence* fence = nullptr;
	if (false == TryGetPlayerTopPriorityServerFence(fence))
	{
		return;
	}

	//이미 플레이어는 펜스 안에 있으므로 펜스는 나갈 필요가 없다. 
	//하지만 마을 무법지대 펜스는 카오스 상태에 따라 동작이 다르니
	//이전 상태를 나가고 상태 변경 후 다시 들어가고 메시지를 보내서 UI와 Stage를 갱신한다. 
	if (TerritoryType::TOWN_CHAOS != fence->GetTerritoryType())
	{
		return;
	}

	if (FGsTownChaosFence* townChaos = static_cast<FGsTownChaosFence*>(fence))
	{
		townChaos->Leave();
		townChaos->SetChaos(isChaos);
		GMessage()->GetGameObject().SendMessage(MessageGameObject::TERRITORY_ENTERED, townChaos);

		return;
	}
}

void FGsFenceHandler::OnQuestAccepted(const struct IGsMessageParam* inParam)
{
	//퀘스트 수락은 Fence 안에 있고 퀘스트가 맞으면 Fence 안에 있다고 판단
	//UpdateTerritory를 통해서 업데이트 한다.
	UpdateTerritory();
}

void FGsFenceHandler::OnQuestComplete(const struct IGsMessageParam* inParam)
{
	//퀘스트 수락은 Fence 안에 있고 퀘스트가 맞으면 Fence 안에 있다고 판단
	//UpdateTerritory를 통해서 업데이트 한다.
	UpdateTerritory();
}

bool FGsFenceHandler::IsClientFence(TerritoryType inType)
{
	switch (inType)
	{
	case TerritoryType::SOUND:
	case TerritoryType::Env:
	case TerritoryType::SPACE_CRACK:
		return true;
	default:
		break;
	}

	return false;
}

bool FGsFenceHandler::IsSafeFence(struct FGsFence* inFence)
{
	switch (inFence->GetTerritoryType())
	{
	case TerritoryType::TOWN:
	case TerritoryType::SAFE_ZONE:
	case TerritoryType::ARENA_READY:
	case TerritoryType::ARRIVAL:
	{
		return true;
	}
	break;
	case TerritoryType::ARENA:
	{
		return false;
	}
	break;
	case TerritoryType::TOWN_CHAOS:
	{
		FGsTownChaosFence* townChaos = static_cast<FGsTownChaosFence*>(inFence);
		if (townChaos)
		{
			return !townChaos->IsChaos();
		}
	}
	break;
	}

	return false;
}
