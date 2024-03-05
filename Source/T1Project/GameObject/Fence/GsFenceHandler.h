// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Map/Fence/GsSchemaFenceElement.h"
#include "Map/Fence/GsSchemaFenceData.h"

#include "Message/GsMessageGameObject.h"
#include "Message/GsMessageContents.h"
#include "Message/GsMessageInvasion.h"

#include "Shared/Client/SharedEnums/SharedCommonEnum.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"

#include "GameObject/GsGameObjectHandler.h"
#include "DataSchema/Sound/GsSchemaSoundResData.h"
#include "GsFence.h"
#include "GsNoCampZoneFence.h"
#include "Management/ScopeGame/GsGameObjectManager.h"

/**
 * Object 타겟 Base클래스
 * 현재는 가장 심플하게 구현해둔다.
 */
class T1PROJECT_API FGsFenceHandler : public IGsGameObjectHandler
{
private:
	TArray<TPair<MessageContentQuest, FDelegateHandle>> _messageContentQuestList;
	TArray<TPair<MessageGameObject, FDelegateHandle>> _messageGameObjectList;
	TArray<TPair<MessageInvasion, FDelegateHandle>> _messageInvasionList;

private:
	TWeakObjectPtr<class UGsGameObjectLocalPlayer> _player;
	TMap<TerritoryType, TArray<TSharedPtr<struct FGsFence>>> _territoryMap;
	TArray<TWeakPtr<struct FGsFence>> _nonPVPTerritoryArray;
	TArray<TSharedPtr<struct FGsNoCampZoneFence>> _noCampZoneTerritoryArray;
	TArray<TWeakPtr<struct FGsTownChaosFence>> _townChaosTerritoryArray;
	TArray<TWeakPtr<struct FGsFence>> _townTerritoryArray;

	//enter
private:
	TSet<int64> _enteredServerTerritoryListForSend;
	TSet<int64> _enteredServerTerritoryListForWaitAck;
	TSet<int64> _leftServerTerritoryListForSend;
	TSet<int64> _leftServerTerritoryListForWaitAck;
	TArray<TWeakPtr<struct FGsFence>> _enteredServerTerritoryList;

private:
	TArray<TWeakPtr<struct FGsFence>> _enteredClientTerritoryList;
	TArray<TWeakPtr<struct FGsFence>> _leftClientTerritoryList;

private:
	float _updateRate = 0.2f;
	float _currentTime = 0.0f;
	bool _active = false;

private:
	FVector _playerLocation;

public:
	FGsFenceHandler() : IGsGameObjectHandler(GameObjectHandlerType::FENCE) {};
	virtual ~FGsFenceHandler() = default;

public:
	virtual void Initialize(class UGsGameObjectBase* inPlayer) override;
	virtual void Update(float inDelta) override;
	virtual void Finalize() override;
	void BindMessage();
	void UnbindMessage();
	void SetPlayer(class UGsGameObjectBase* inPlayer);

public:
	void Reset();
	void UpdateTerritory();

public:
	void UpdateEnterFence();
	void UpdateLeftFence();

private:
	void NotifyServerEnteredFence();
	void NotifyServerLeftFence();

private:
	void InitializeTerritory();
	void InitServerFence(const struct FGsSchemaMapTerritoryData* inMapTerritoryData, const TArray<FGsSchemaFenceElement>& inFenceArray);
	void InitClientFence(const struct FGsSchemaMapTerritoryData* inMapTerritoryData, const TArray<FGsSchemaFenceElement>& inFenceArray);
	void InitNoCampZoneFence(const struct FGsSchemaMapTerritoryData* inMapTerritoryData, const TArray<FGsSchemaFenceElement>& inFenceArray);

public:
	bool TryGetAllOverlapedServerFence(TArray<TSharedPtr<struct FGsFence>>& outFenceList, const FVector& inLocation);	
	bool TryGetAllOverlapedFence(TArray<TSharedPtr<struct FGsFence>>& outFenceList, const FVector& inLocation);
	bool IsInNonPVPFence(const FVector& inLocation);
	bool IsInTownFence(const FVector& inLocation);
	bool TryGetAllFence(TArray<TSharedPtr<struct FGsFence>>& outFenceList, TerritoryType inType);
	bool TryGetNoCampZoneFence(TArray<TSharedPtr<struct FGsNoCampZoneFence>>& outNoCampZoneFenceList);
	bool TryGetAllOverlapedClientFence(TArray<TSharedPtr<struct FGsFence>>& outFenceList, const FVector& inLocation);
	TWeakPtr<struct FGsFence> GetEnteredTerritory(int64 inTerritoryId);
	TWeakPtr<struct FGsFence> GetTerritory(int64 inTerritoryId);
	bool IsEnteredTerritory(int64 inTerritoryId);
	bool IsEnteredTerritoryType(TerritoryType inType);
	//플레이어가 안전지대에 있는지 확인하는 함수
	bool IsPlayerInNonPVPFence();
	bool IsPlayerInTownFence();
	//플레이어가 들어가 있는 서버 영역 중 프리오리티를 비교하여 펜스 타입을 얻어오는 함수
	bool TryGetPlayerTopPriorityServerFence(OUT FGsFence*& outFence);
	bool TryGetTopPriorityServerFence(OUT FGsFence*& outFence, const FVector& inLocation);
	bool AddEnteredTerritory(int64 inTerritoryId);
	bool RemoveEnterTerritory(int64 inTerritoryId);
	void SetActive(bool inActive);
	TArray<TWeakPtr<struct FGsFence>> GetEnteredFenceArray() { return _enteredServerTerritoryList; }

	//client fence
private:	
	bool UpdateClientFenceEnter(TWeakPtr<struct FGsFence> inFence);		
	bool UpdatesClientFenceLeft(TWeakPtr<struct FGsFence> inFence);
	bool ForceLeftClientFence(TWeakPtr<struct FGsFence> inFence);
	bool UpdatesClientFenceLeft_Internal(TWeakPtr<struct FGsFence> inFence);

	//sever fence
private:
	bool UpdatesServerFenceEnter(const struct FGsFence* inFence);
	bool UpdatesServerFenceLeft(const struct FGsFence* inFence);

public:
	//방어자가 마을에 있다가 무법지대에 있다면 타운을 나가고 무법지대로 변경해준다. 
	void OnUpdatePlayerChaosState(const struct IGsMessageParam* inParam);

private:		
	void OnQuestAccepted(const struct IGsMessageParam*);
	void OnQuestComplete(const struct IGsMessageParam*);

	//sound
public:
	bool TryGetFirstPrioritySoundFence(struct FGsFence*& outFence, const FVector& inLocation);

public:
	static bool IsClientFence(TerritoryType inTerritoryType);
	static bool IsSafeFence(struct FGsFence* inFence);
	static bool IsServerFence(TerritoryType inTerritoryType);
	static bool IsTown(TerritoryType inTerritoryType);

public:
	bool CanCreateCampZone(const FVector& inLocation);

public:
	void ShowState();
	static int32 GetSoundPriority(TerritoryType inTerritoryType);
	
	FString ToString();
};

#define GFence() ((nullptr != GSGameObject()->FindObject(EGsGameObjectType::LocalPlayer)) ? (Cast<UGsGameObjectLocalPlayer>(GSGameObject()->FindObject(EGsGameObjectType::LocalPlayer)))->GetFenceHandler() : nullptr)
