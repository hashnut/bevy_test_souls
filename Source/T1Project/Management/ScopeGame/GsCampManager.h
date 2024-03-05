// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GsManager.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "GameObject/Define/GsGameObjectDefine.h"
#include "Message/GsMessageGameObject.h"
#include "DataSchema/GameObject/Camp/GsSchemaCampData.h"

class AActor;
class AGsCamp;
class AGsCompleteCamp;
class AGsCampStaticMesh;
class AGsCharacterBase;
class AGsCampNavModifier;
class UStaticMesh;
class UGsGameObjectBase;
class UGsGameObjectCamp;
class UGsGameObjectLocalPlayer;
struct FGsSchemaItemEffectSpawnCamp;
/**
 * 
 */
class T1PROJECT_API FGsCampManager final : public IGsManager
{
private:
	TWeakObjectPtr<UGsGameObjectLocalPlayer> _localPlayerPtr;

	// CampGameID , CampObject
	TMap<int64, TWeakObjectPtr<UGsGameObjectCamp>> _spawnCampObjectMap;

	// CampGameID , InteractionUserCount
	TMap<int64 , int32> _interactingCampObjectMap;

private:
	UPROPERTY()
	AGsCampStaticMesh* _clientSpawnCampActor = nullptr;

private:
	const FGsSchemaItemEffectSpawnCamp* _campEffectData = nullptr;

private:
	// 현재 가입된 기사단이 있다면 이미 설치된 수호탑이 있는지에 대한 flag
	bool _isAllowedBuildCamp = true; 

private:
	bool _isOkBuildCampArea = false;
	bool _isAllowObjectDist = true;
	uint64 _campConsumItemDBID = 0;
	FVector _targetCampPos;
	TArray<FVector> _serverSpawnPropObjectPosList;

private:
	MsgGameObjHandle _gameObjectMsgDelegate;

	// TestCode
public:
	bool _testAngleFlag = false;

private:
	TArray<const UGsGameObjectBase*> _findObjectList;

public:
	//IGsManager
	virtual void Initialize() override;
	virtual void Finalize() override;

public:
	void ClientSpawnCampActor(const int32 InCampDataID);
	AActor* SpawnNavModifierActor(AActor* InTargetActor);
	AGsCharacterBase* LoadCompleteCampActor(const int64 InGameID, const bool InIsNew);
	AGsCharacterBase* LoadConstructCampActor(const int64 InGameID);
	void SetConstructPos(const int64 InGameID);
	float GetConstructPos(const int64 InGameID);

public:
	void OnSpawnCampAck(const int64 InGameID);

public:
	void DespawnCamp(const int64 InGameID);
	void DespawnClientCampActor();
	void DespawnNavModifierCamp(AActor* InActor);
	void ResetAllData();

public:
	void TryUseCampConsumItem(const uint64 InItemDBID);
	void SendBuildCamp();
	void SpawnBuildCamp(UGsGameObjectCamp* spawnCampObj, const bool InIsAlive);
	const bool IsOkBuildArea();
	void OnBuildCamp();
	void CancelBuildCamp();
	void OpenCampPlacementMenuUI();
	void CloseCampPlacementMenuUI();

public:
	void AddInteractionUser(const int64 InCampObjectGameID);
	void RemoveInteractionUser(const int64 InCampObjectGameID);

private:
	AActor* CreateBPClass(AActor* InLocalPlayerActor, UClass* InClass, const FVector& InSpawnLocation, const FRotator& InSpawnRotation, FName InName = NAME_None);

private:
	void OnLocalPlayerSpawnComplete(const struct IGsMessageParam* InParam);

private:
	bool FindLocalPlayerData();
	UGsGameObjectCamp* FindCampObject(const int64 InGameID);
	void FindSearchObject();

private:
	void RemoveCampObjectMap(const int64 InGameID);

public:
	const FGsSchemaCampData* FindCampData(const int64 InGameID);

public:
	void ShowCampErrorPopupUI(FString InErrorMsg);

public:
	void SetIsOkBuildCampArea(const bool IsOk) { _isOkBuildCampArea = IsOk; }
	void SetIsAllowedBuildCamp(const bool InIsOk) { _isAllowedBuildCamp = InIsOk; }

public:
	bool IsClientSpawned();
	bool IsAllowMap();
	bool IsOkBuildCampArea() { return _isOkBuildCampArea; }
	bool IsAllowedBuildCamp() { return _isAllowedBuildCamp; }
	int32 GetInteractionRange(const int64 InGameID);
	UGsGameObjectLocalPlayer* GetLocalPlayerObject();
	const FGsSchemaItemEffectSpawnCamp* FindCampEffectData(const uint64 InItemDBID);
	FVector GetTargetCampPos() { return _targetCampPos; }

public:
	void GetCampName(const int64 InGameID, FText& outText);
	int32 GetCampInteractionRange(const struct FGsSchemaNpcData* InNpcData, class UGsGameObjectBase* InTarget);

private:
	const bool IsAllowedEnemyCampDist(const UGsGameObjectBase* InTargetObject, const float InDiaMeter);
	const bool IsAllowedOtherCampDist(const UGsGameObjectBase* InTargetObject, const float InDiaMeter);
	void ObjectDistanceCheck();

public:
	void DestroyCamp_Test(const uint32 InCampID);

public:
	template<typename T1, typename... T2>
	void FindObjectList(T1 InType, T2... InTypes);
	void FindObjectList(); // 후처리용 함수
	void FindByObjectType(EGsGameObjectType InType);

};

#define GSCamp() UGsScopeHolder::GetGameManagerFType<FGsCampManager>(UGsScopeGame::EManagerType::Camp)

template<typename T1, typename... T2>
void FGsCampManager::FindObjectList(T1 InType, T2... InTypes)
{
	FindByObjectType(InType);
	FindObjectList(InTypes...);
}