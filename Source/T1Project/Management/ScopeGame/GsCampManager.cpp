// Fill out your copyright notice in the Description page of Project Settings.


#include "Management/ScopeGame/GsCampManager.h"
#include "Management/ScopeGame/GsGameObjectManager.h"
#include "Management/ScopeGlobal/GsUIManager.h"
#include "Management/ScopeGlobal/GsLevelManager.h"
#include "Management/ScopeGlobal/GsGuildManager.h"
#include "Management/ScopeGlobal/GsGameFlowManager.h"
#include "Management/ScopeGlobal/GsGameDataManager.h"
#include "Management/GsMessageHolder.h"
#include "Components/CapsuleComponent.h"
#include "Data/GsDataContainManager.h"
#include "Data/GsGlobalConstant.h"
#include "DataSchema/GameObject/GsSchemaShapeData.h"
#include "DataSchema/GameObject/Npc/GsSchemaNpcData.h"
#include "DataSchema/Map/Spawn/GsSchemaSpawnElement.h"
#include "DataSchema/Item/GsSchemaItemEffectSpawnCamp.h"
#include "DataSchema/GameObject/Camp/GsSchemaConstructCampInfo.h"
#include "GameObject/Data/GsGameObjectDataLocalPlayer.h"
#include "GameObject/ObjectClass/GsGameObjectCamp.h"
#include "GameObject/ObjectClass/GsGameObjectLocalPlayer.h"
#include "GameObject/State/GsGameObjectStateManager.h"
#include "Animation/GsCompleteCampAnimInstance.h"
#include "GameFlow/GsGameFlowGame.h"
#include "GameFlow/Stage/GsStageMode.h"
#include "GameFlow/Stage/StageGame/GsStageManagerGame.h"
#include "Guild/GsGuildData.h"
#include "ActorEx/GsCharacterBase.h"
#include "ActorEx/GsCamp.h"
#include "ActorEx/GsCompleteCamp.h"
#include "ActorEx/GsCampBase.h"
#include "ActorEx/GsCampNavModifier.h"
#include "ActorEx/GsCampStaticMesh.h"
#include "UTIL/GsLevelUtil.h"
#include "UTIL/GsDir.h"
#include "Item/GsItemManager.h"
#include "Item/GsItemConsumable.h"
#include "Item/GsItemIngredient.h"
#include "Net/GsNetSendServiceItem.h"
#include "Net/GsNetSendServiceWorld.h"
#include "T1Project.h"
#include "UI/UIContent/Popup/GsUIPopupCampPlacementMenu.h"
#include "UI/UIContent/Helper/GsUIHelper.h"
#include "UI/UIContent/Popup/GsUIPopupOK.h"
#include "Engine/StaticMesh.h"
#include "Shared/Client/SharedEnums/SharedCreatureEnum.h"
#include "DataSchema/Map/Spot/GsSchemaSpotInfo.h"



void FGsCampManager::Initialize()
{
	FindLocalPlayerData();

	FGsMessageHolder* msg = GMessage();
	if (nullptr != msg)
	{
		_gameObjectMsgDelegate = msg->GetGameObject().AddRaw(MessageGameObject::LOCAL_SPAWN_ME_COMPLETE, this, &FGsCampManager::OnLocalPlayerSpawnComplete);
	}
}

void FGsCampManager::Finalize()
{
	FGsMessageHolder* msg = GMessage();
	if (nullptr != msg)
	{
		msg->GetGameObject().Remove(_gameObjectMsgDelegate);
	}

	ResetAllData();
}

void FGsCampManager::ResetAllData()
{
	_testAngleFlag = false;
	_campConsumItemDBID = 0;
	_isOkBuildCampArea = false;
	_campEffectData = nullptr;
	_clientSpawnCampActor = nullptr;
	_localPlayerPtr.Reset();
	_findObjectList.Empty();
	_spawnCampObjectMap.Empty();
	_interactingCampObjectMap.Empty();

	DespawnClientCampActor();

	_serverSpawnPropObjectPosList.Empty();
}

void FGsCampManager::OnLocalPlayerSpawnComplete(const struct IGsMessageParam* InParam)
{
	_serverSpawnPropObjectPosList.Empty();

	TArray<FGsSchemaSpawnElement> spawnElementList;
	int32 levelKey = GLevel()->GetCurrentLevelId();
	UGsLevelUtil::TryGetAllSpawnElement(levelKey, CreatureSpawnType::PROP, spawnElementList);
	for (FGsSchemaSpawnElement iter : spawnElementList)
	{
		if(CreatureSpawnType::PROP != iter.SpawnType)
			continue;

		_serverSpawnPropObjectPosList.Emplace(iter.Pos);		
	}

	TArray<const FGsSchemaSpotInfo*> invadeSpotElement;
	UGsLevelUtil::TryGetAllInvadePortalSpotArray(levelKey, invadeSpotElement);
	for (const FGsSchemaSpotInfo* iter : invadeSpotElement)
	{
		_serverSpawnPropObjectPosList.Emplace(iter->pos);
	}
}

void FGsCampManager::ClientSpawnCampActor(const int32 InCampDataID)
{
	if (IsClientSpawned())
	{
		DespawnClientCampActor();
	}
	_isOkBuildCampArea = false;

	if (false == _localPlayerPtr.IsValid())
	{
		if (false == FindLocalPlayerData())
			return;
	}

	const UGsTableCampData* table = Cast<UGsTableCampData>(FGsSchemaCampData::GetStaticTable());
	if (nullptr == table)
	{
		GSLOG(Error, TEXT("[FGsCampManager::ClientSpawnCampActor] table is nullptr !! "));
		return;
	}

	const FGsSchemaCampData* data;
	if (false == table->FindRowById(InCampDataID, data))
	{
		GSLOG(Error, TEXT("[FGsCampManager::ClientSpawnCampActor] table->FindRowById() is empty !! "));
		return;
	}
	
	uint16 currCampLevel = GGuild()->GetCampLevel();
	if (0 == currCampLevel)
	{
		return;
	}

	const FGsSchemaNpcData* npcData = data->npcDataIdList[currCampLevel - 1].GetRow();
	if(nullptr == npcData)
		return;

	const FGsSchemaShapeData* shapeData = npcData->shapeId.GetRow();
	if (nullptr == shapeData)
		return;

	FString staticmeshPath = shapeData->staticMeshPath.ToString();
	UStaticMesh* staticmesh = LoadObject<UStaticMesh>(nullptr, *staticmeshPath);
	if (nullptr == staticmesh)
	{
		GSLOG(Error, TEXT("[FGsCampManager::ClientSpawnCampActor] StaticMesh is nullptr !!"));
		return;
	}
	
	UClass* bpClass = nullptr;
	FString path = FString::Printf(TEXT("%s_C"), *data->campStaticMeshPath.ToString());
	bpClass = LoadObject<UClass>(nullptr, *path);
	if (nullptr == bpClass)
	{
		GSLOG(Error, TEXT("[FGsCampManager::ClientSpawnCampObject] LoadObject Failed / campStaticMeshPath bpClass is nullptr !!"));
		return;
	}

	AActor* localPlayerActor = _localPlayerPtr->GetActor();
	AActor* createCampActor = CreateBPClass(localPlayerActor, bpClass, localPlayerActor->GetActorLocation(), localPlayerActor->GetActorRotation());
	if (nullptr == createCampActor)
		return;

	_clientSpawnCampActor = Cast<AGsCampStaticMesh>(createCampActor);
	_clientSpawnCampActor->SetCampStaticMesh(staticmesh, data->staticmeshMaterial);
}

AActor* FGsCampManager::SpawnNavModifierActor(AActor* InTargetActor)
{
	if (nullptr == InTargetActor)
		return nullptr;

	if (false == _localPlayerPtr.IsValid())
	{
		if (false == FindLocalPlayerData())
			return nullptr;
	}

	FString bpPath = "";
	if (GData())
	{
		bpPath = GData()->GetGlobalData()->_navModifierBPclassPath.ToString();
	}
	UClass* bpClass = nullptr;
	FString path = FString::Printf(TEXT("%s_C"), *bpPath);
	bpClass = LoadObject<UClass>(nullptr, *path);
	if (nullptr == bpClass)
	{
		GSLOG(Error, TEXT("[FGsCampManager::SpawnNavModifierActor] LoadObject Failed / bpClass is nullptr !!"));
		return nullptr;
	}

	AActor* localPlayerActor = _localPlayerPtr->GetActor();
	AActor* createActor = CreateBPClass(localPlayerActor, bpClass, InTargetActor->GetActorLocation() + FVector(0.f, 0.f, 100.f), InTargetActor->GetActorRotation());
	if (nullptr == createActor)
		return nullptr;

	return createActor;
}

bool FGsCampManager::IsClientSpawned()
{
	return nullptr != _clientSpawnCampActor;
}

void FGsCampManager::SetConstructPos(const int64 InGameID)
{
	const FGsSchemaCampData* campData = FindCampData(InGameID);
	if (nullptr == campData)
		return;

	UGsGameObjectCamp* campObject = FindCampObject(InGameID);
	if (nullptr == campObject)
		return;

	if (false == _localPlayerPtr.IsValid())
	{
		if (false == FindLocalPlayerData())
			return;
	}

	AGsCamp* campActor = campObject->GetConstructCampActor();
	if (nullptr == campActor)
		return;

	FVector Location = campObject->GetActor()->GetActorLocation();
	float resultZ = campActor->GetActorLocation().Z;
	FHitResult hit;
	FCollisionQueryParams CollisionParams;
	const bool bHit = campObject->GetWorld()->LineTraceSingleByChannel(hit, campActor->GetActorLocation(),
		Location - FVector(0.f, 0.f, 10000.f), ECC_Visibility, CollisionParams);
	if (bHit)
	{
		resultZ = hit.Location.Z;
	}
	campActor->SetActorLocation(FVector(campActor->GetActorLocation().X, campActor->GetActorLocation().Y, resultZ));
}

float FGsCampManager::GetConstructPos(const int64 InGameID)
{
	UGsGameObjectCamp* campObject = FindCampObject(InGameID);
	if (nullptr == campObject)
		return 0.f;

	AGsCamp* campActor = campObject->GetConstructCampActor();
	if (nullptr == campActor)
		return 0.f;

	FVector Location = campObject->GetActor()->GetActorLocation();
	float resultZ = campActor->GetActorLocation().Z;
	FHitResult hit;
	FCollisionQueryParams CollisionParams;
	const bool bHit = campObject->GetWorld()->LineTraceSingleByChannel(hit, campActor->GetActorLocation(),
		Location - FVector(0.f, 0.f, 10000.f), ECC_Visibility, CollisionParams);
	if (bHit)
	{
		resultZ = hit.Location.Z;
	}

	return resultZ;
}

AGsCharacterBase* FGsCampManager::LoadConstructCampActor(const int64 InGameID)
{
	const FGsSchemaCampData* campData = FindCampData(InGameID);
	if (nullptr == campData)
		return nullptr;

	UGsGameObjectCamp* campObject = FindCampObject(InGameID);
	if (nullptr == campObject)
		return nullptr;

	if (false == _localPlayerPtr.IsValid())
	{
		if (false == FindLocalPlayerData())
			return nullptr;
	}

	uint16 curBuildingLevel = campObject->GetBuildingLevel();
	if (0 >= curBuildingLevel || 0 >= campData->npcDataIdList.Num() || false == campData->npcDataIdList.IsValidIndex(curBuildingLevel - 1))
	{
		GSLOG(Error, TEXT("[FGsCampManager::LoadConstructCampActor] 0 >= curBuildingLevel || 0 >= data->npcDataIdList.Num() || false == data->npcDataIdList.IsValidIndex(curBuildingLevel - 1) "));
		return nullptr;
	}

	const FGsSchemaNpcData* npcData = campData->npcDataIdList[curBuildingLevel - 1].GetRow();
	if (nullptr == npcData)
		return nullptr;

	const FGsSchemaShapeData* shapeData = npcData->shapeId.GetRow();
	if (nullptr == shapeData)
		return nullptr;

	UClass* bpClass = nullptr;
	FString path = FString::Printf(TEXT("%s_C"), *shapeData->bPClass.ToString());
	bpClass = LoadObject<UClass>(nullptr, *path);
	if (nullptr == bpClass)
	{
		GSLOG(Error, TEXT("[FGsCampManager::LoadConstructCampActor] LoadObject Failed / bpClass is nullptr !!"));
		return nullptr;
	}
	FVector Location = campObject->GetSpawnPos();
	/*
	FVector Loca2;
	FHitResult hit;
	if (UGsLevelUtil::TryLineTraceToLand(hit, campObject->GetWorld(), Location))
	{
		Loca2 = hit.Location;
	}
	GSLOG(Error, TEXT("TryLineTraceToLand : %s "), *Loca2.ToString());
	*/

	AActor* createCampActor = CreateBPClass(_localPlayerPtr->GetActor(), bpClass, Location, _localPlayerPtr->GetActor()->GetActorRotation());
	if (nullptr == createCampActor)
		return nullptr;

	AGsCharacterBase* charBaseActor = Cast<AGsCharacterBase>(createCampActor);
	if (nullptr == charBaseActor)
	{
		GSLOG(Error, TEXT("[FGsCampManager::LoadConstructCampActor] AGsCharacterBase cast is nullptr !! - shapeData->id : %d"), shapeData->id);
		return nullptr;
	}

	FGsSchemaConstructCampInfo constructionCampInfo = campData->constructionInfo;
	if (nullptr != constructionCampInfo.creatureCommonInfoId.GetRow())
	{
		campObject->SetCreatureCommonInfoData(constructionCampInfo.creatureCommonInfoId.GetRow());
	}

	charBaseActor->ActiveGravity(false);
	charBaseActor->GetCapsuleComponent()->SetEnableGravity(false);

	//campObject->InitializeActor(createCampActor);
	campObject->ActorSpawned(createCampActor);

	float resultZ = createCampActor->GetActorLocation().Z;
	FHitResult hit;
	FCollisionQueryParams CollisionParams;
	const bool bHit = campObject->GetWorld()->LineTraceSingleByChannel(hit, createCampActor->GetActorLocation(),
		Location - FVector(0.f, 0.f, 10000.f), ECC_Visibility, CollisionParams);
	if (bHit)
	{		
		resultZ = hit.Location.Z;	
	}

	AGsCampBase* campBase = Cast<AGsCampBase>(charBaseActor);
	FVector originVec;
	FVector extendVec;
	createCampActor->GetActorBounds(true, originVec, extendVec);
	campBase->SetNavigationAddOffsetValue(FVector(0.f, 0.f, extendVec.Z));

	_targetCampPos = FVector(createCampActor->GetActorLocation().X, createCampActor->GetActorLocation().Y, resultZ);

	//createCampActor->SetActorLocation(FVector(createCampActor->GetActorLocation().X, createCampActor->GetActorLocation().Y, resultZ));
	return charBaseActor;
}

AGsCharacterBase* FGsCampManager::LoadCompleteCampActor(const int64 InGameID, const bool InIsNew)
{
	const FGsSchemaCampData* campData = FindCampData(InGameID);
	if (nullptr == campData)
		return nullptr;

	UGsGameObjectCamp* campObject = FindCampObject(InGameID);
	if(nullptr == campObject)
		return nullptr;

	if (false == _localPlayerPtr.IsValid())
	{
		if (false == FindLocalPlayerData())
			return nullptr;
	}

	uint16 curBuildingLevel = campObject->GetBuildingLevel();
	if (0 >= curBuildingLevel || 0 >= campData->completeCampShapeId.Num() || false == campData->completeCampShapeId.IsValidIndex(curBuildingLevel - 1))
	{
		GSLOG(Error, TEXT("[FGsCampManager::LoadCompleteCampActor] 0 >= curBuildingLevel || 0 >= data->completeCampShapeId.Num() || false == data->completeCampShapeId.IsValidIndex(curBuildingLevel - 1) "));
		return nullptr;
	}

	const FGsSchemaShapeData* shapeData = campData->completeCampShapeId[curBuildingLevel - 1].GetRow();
	if (nullptr == shapeData)
		return nullptr;

	UClass* bpClass = nullptr;
	FString path = FString::Printf(TEXT("%s_C"), *shapeData->bPClass.ToString());
	bpClass = LoadObject<UClass>(nullptr, *path);
	if (nullptr == bpClass)
	{
		GSLOG(Error, TEXT("[FGsCampManager::LoadCompleteCampActor] LoadObject Failed / bpClass is nullptr !!"));
		return nullptr;
	}
	FVector Location = campObject->GetSpawnPos();
	AActor* createCompleteCampActor = CreateBPClass(_localPlayerPtr->GetActor(), bpClass, Location, _localPlayerPtr->GetActor()->GetActorRotation());
	if (nullptr == createCompleteCampActor)
		return nullptr;

	AGsCharacterBase* charBaseActor = Cast<AGsCharacterBase>(createCompleteCampActor);
	if (nullptr == charBaseActor)
	{
		GSLOG(Error, TEXT("[FGsCampManager::LoadCompleteCampActor] AGsCharacterBase cast is nullptr !! - shapeData->id : %d"), shapeData->id);
		return nullptr;
	}

	campObject->RevertCreatureCommonInfoData();
	campObject->InitializeActor(createCompleteCampActor);

	float resultZ = createCompleteCampActor->GetActorLocation().Z;
	FHitResult hit;
	FCollisionQueryParams CollisionParams;
	const bool bHit = campObject->GetWorld()->LineTraceSingleByChannel(hit, createCompleteCampActor->GetActorLocation(),
		Location - FVector(0.f, 0.f, 10000.f), ECC_Visibility, CollisionParams);
	if (bHit)
	{
		resultZ = hit.Location.Z;
	}

	AGsCampBase* campBase = Cast<AGsCampBase>(charBaseActor);
	FVector originVec;
	FVector extendVec;
	createCompleteCampActor->GetActorBounds(true, originVec, extendVec);
	campBase->SetNavigationAddOffsetValue(FVector(0.f, 0.f, extendVec.Z));

	_targetCampPos = FVector(createCompleteCampActor->GetActorLocation().X, createCompleteCampActor->GetActorLocation().Y, resultZ);

	return charBaseActor;
}

int32 FGsCampManager::GetCampInteractionRange(const struct FGsSchemaNpcData* InNpcData, class UGsGameObjectBase* InTarget)
{
	UGsGameObjectCamp* campObject = FindCampObject(InTarget->GetGameId());
	if (nullptr == campObject)
		return 0;

	if (campObject->IsCompleteBuildingCamp())
	{
		return InNpcData->interactionRange;
	}
	else
	{
		const FGsSchemaCampData* campData = FindCampData(InTarget->GetGameId());
		if (nullptr == campData)
			return 0;

		FGsSchemaConstructCampInfo constructionCampInfo = campData->constructionInfo;
		return 0 >= constructionCampInfo.interactionRange ?
			InNpcData->interactionRange : constructionCampInfo.interactionRange;
	}
}

AActor* FGsCampManager::CreateBPClass(AActor* InLocalPlayerActor, UClass* InClass, const FVector& InSpawnLocation, const FRotator& InSpawnRotation, FName InName)
{
	if(nullptr == InLocalPlayerActor || nullptr == InLocalPlayerActor->GetWorld())
		return nullptr;

	if (nullptr == InClass)
		return nullptr;

	FActorSpawnParameters spawnParam = FActorSpawnParameters();
	spawnParam.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	if (InName != NAME_None)
	{
		spawnParam.Name = InName;
	}

	return InLocalPlayerActor->GetWorld()->SpawnActor(InClass, &InSpawnLocation, &InSpawnRotation, spawnParam);
}

const FGsSchemaCampData* FGsCampManager::FindCampData(const int64 InGameID)
{
	UGsGameObjectCamp* campObject = FindCampObject(InGameID);
	if (nullptr == campObject)
		return nullptr;

	const UGsTableCampData* table = Cast<UGsTableCampData>(FGsSchemaCampData::GetStaticTable());
	if (nullptr == table)
	{
		GSLOG(Error, TEXT("[FGsCampManager::FindCampData] table is nullptr !! "));
		return nullptr;
	}

	const FGsSchemaCampData* data;
	if (false == table->FindRowById(campObject->GetCampID(), data))
	{
		GSLOG(Error, TEXT("[FGsCampManager::FindCampData] table->FindRowById() is empty !! "));
		return nullptr;
	}
	return data;
}

UGsGameObjectCamp* FGsCampManager::FindCampObject(const int64 InGameID)
{
	if (_spawnCampObjectMap.Contains(InGameID))
	{
		TWeakObjectPtr<UGsGameObjectCamp> campObject = _spawnCampObjectMap[InGameID];
		if (campObject.IsValid())
		{
			return campObject.Get();
		}
	}

	return nullptr;
}

void FGsCampManager::RemoveCampObjectMap(const int64 InGameID)
{
	if (_spawnCampObjectMap.Contains(InGameID))
	{
		_spawnCampObjectMap.Remove(InGameID);
	}
}

bool FGsCampManager::FindLocalPlayerData()
{
	if (nullptr == GSGameObject())
	{
		_localPlayerPtr.Reset();
		return false;
	}

	UGsGameObjectBase* findLocalPlayer = GSGameObject()->FindObject(EGsGameObjectType::LocalPlayer);
	if (nullptr == findLocalPlayer || nullptr == findLocalPlayer->GetActor())
	{
		return false;
	}

	UGsGameObjectLocalPlayer* localPlayer = findLocalPlayer->CastGameObject<UGsGameObjectLocalPlayer>();
	if (nullptr == localPlayer)
	{
		return false;
	}
	_localPlayerPtr = localPlayer;

	return true;
}

void FGsCampManager::DespawnCamp(const int64 InGameID)
{
	RemoveCampObjectMap(InGameID);
}

void FGsCampManager::DespawnClientCampActor()
{
	if (nullptr != _clientSpawnCampActor)
	{
		_clientSpawnCampActor->DestoryClientSpawn();
		_clientSpawnCampActor = nullptr;
	}

	_testAngleFlag = false;
}

void FGsCampManager::DespawnNavModifierCamp(AActor* InActor)
{
	if (nullptr != InActor)
	{
		InActor->Destroy();
		InActor = nullptr;
	}
}

void FGsCampManager::TryUseCampConsumItem(const uint64 InItemDBID)
{
	_campEffectData = nullptr;
	_campConsumItemDBID = 0;

	TWeakPtr<FGsItem> findItem = GItem()->FindItem(InItemDBID,ItemType::INGREDIENT);
	if (findItem.IsValid())
	{
		FGsItemIngredient* ingredientItem = findItem.Pin()->GetCastItem<FGsItemIngredient>();
		if (nullptr == ingredientItem)
			return;

		TArray<int32> effectIdList;
		ingredientItem->FindEffectIdList(effectIdList);
		if (0 >= effectIdList.Num())
			return;

		int32 id = effectIdList[0];
		const UGsTableItemEffectSpawnCamp* table = Cast<UGsTableItemEffectSpawnCamp>(FGsSchemaItemEffectSpawnCamp::GetStaticTable());
		if (nullptr == table)
		{
			GSLOG(Error, TEXT("[FGsCampManager::FGsSchemaItemEffectSpawnCamp] table is nullptr !! "));
			return;
		}
		const FGsSchemaItemEffectSpawnCamp* row = nullptr;
		if (false == table->FindRowById(id, row))
		{
			return;
		}
		if (false == _localPlayerPtr.IsValid())
		{
			if (false == FindLocalPlayerData())
				return;
		}
		_campEffectData = row;
		_campConsumItemDBID = InItemDBID;

		ClientSpawnCampActor(row->campId);
		OpenCampPlacementMenuUI();
	}
}

const FGsSchemaItemEffectSpawnCamp* FGsCampManager::FindCampEffectData(const uint64 InItemDBID)
{
	TWeakPtr<FGsItem> findItem = GItem()->FindItem(InItemDBID, ItemType::INGREDIENT);
	if (findItem.IsValid())
	{
		FGsItemIngredient* ingredientItem = findItem.Pin()->GetCastItem<FGsItemIngredient>();
		if (nullptr == ingredientItem)
			return nullptr;

		TArray<int32> effectIdList;
		ingredientItem->FindEffectIdList(effectIdList);
		if (0 >= effectIdList.Num())
			return nullptr;

		int32 id = effectIdList[0];
		const UGsTableItemEffectSpawnCamp* table = Cast<UGsTableItemEffectSpawnCamp>(FGsSchemaItemEffectSpawnCamp::GetStaticTable());
		if (nullptr == table)
		{
			GSLOG(Error, TEXT("[FGsCampManager::FindCampEffectData] table is nullptr !! - InItemDBID : %llu") , InItemDBID);
			return nullptr;
		}
		const FGsSchemaItemEffectSpawnCamp* row = nullptr;
		if (false == table->FindRowById(id, row))
		{
			GSLOG(Error, TEXT("[FGsCampManager::FindCampEffectData] FindRowById is nullptr !! - InItemDBID : %llu "), InItemDBID);
			return nullptr;
		}
		return row;
	}

	return nullptr;
}

void FGsCampManager::SendBuildCamp()
{
	if (false == _localPlayerPtr.IsValid())
	{
		if (false == FindLocalPlayerData())
			return;
	}
	if (nullptr == _clientSpawnCampActor)
	{
		return;
	}

	FVector finalLocation;
	FHitResult hit;
	FCollisionQueryParams CollisionParams;
	const bool bHit = _clientSpawnCampActor->GetWorld()->LineTraceSingleByChannel(hit, _clientSpawnCampActor->GetActorLocation(),
		_clientSpawnCampActor->GetActorLocation() - FVector(0.f, 0.f, 10000.f), ECC_Visibility, CollisionParams);
	if (bHit)
	{
		finalLocation = hit.Location;
	}

	CampPosition posInfo;
	posInfo.mDir = FGsDir::dirToAngle(_clientSpawnCampActor->GetActorForwardVector());
	posInfo.mX = finalLocation.X;
	posInfo.mY = finalLocation.Y;
	posInfo.mZ = finalLocation.Z;

	//GSLOG(Error, TEXT("[FGsCampManager::SendBuildCamp] - posInfo.mX : %f"), posInfo.mX);
	//GSLOG(Error, TEXT("[FGsCampManager::SendBuildCamp] - posInfo.mY : %f"), posInfo.mY);
	//GSLOG(Error, TEXT("[FGsCampManager::SendBuildCamp] - posInfo.mZ : %f"), posInfo.mZ);

	DespawnClientCampActor();

	// 서버에서는 z 축을 그대로 사용하지 않음( 별도의 보정값을 사용하는듯? )
	// x y 축만 클라에서 받은 그대로 사용함
	FGsNetSendServiceItem::SendUseCampConsumItem(_campConsumItemDBID, posInfo);
}

void FGsCampManager::SpawnBuildCamp(UGsGameObjectCamp* spawnCampObj, const bool InIsAlive)
{
	if (false == _spawnCampObjectMap.Contains(spawnCampObj->GetGameId()))
	{
		_spawnCampObjectMap.Emplace(spawnCampObj->GetGameId(), spawnCampObj);
	}
	if (InIsAlive)
	{
		if (false == _localPlayerPtr.IsValid())
		{
			if (false == FindLocalPlayerData())
				return;
		}
		
		spawnCampObj->CompleteSpawn();

		//GSLOG(Error, TEXT("[FGsCampManager::SpawnBuildCamp] - spawnPos : %s"), *_campObjectPtr->GetActor()->GetActorLocation().ToString());
		/*
		FVector Location;
		FHitResult hit;
		if (UGsLevelUtil::TryLineTraceToLand(hit, _localPlayerPtr->GetWorld(), _campObjectPtr->GetActor()->GetActorLocation()))
		{
			Location = hit.Location;
		}
		*/
		//FVector finalPos = spawnCampObj->GetActor()->GetActorLocation() + FVector(0.f, 0.f, 50.f);

		/*
		FVector Pos = spawnCampObj->GetActor()->GetActorLocation();
		float resultZ = Pos.Z;
		FHitResult hit;
		FCollisionQueryParams CollisionParams;
		const bool bHit = spawnCampObj->GetWorld()->LineTraceSingleByChannel(hit, Pos,
			Pos - FVector(0.f, 0.f, 10000.f), ECC_Visibility, CollisionParams);
		if (bHit)
		{
			FVector originVec;
			FVector extendVec;
			spawnCampObj->GetActor()->GetActorBounds(true, originVec, extendVec);
			resultZ = hit.Location.Z + ((extendVec.Z / 2.f) - 50.f);
		}
		spawnCampObj->GetActor()->SetActorLocation(FVector(Pos.X, Pos.Y, resultZ));
		*/
	}
	else // 파괴된 상태, 애초에 spawn 패킷이 오지 않을듯함
	{
	}
}

void FGsCampManager::OpenCampPlacementMenuUI()
{
	UGsUIManager* uiManager = GUI();
	if (nullptr == uiManager)
		return;

	if (true == uiManager->IsActiveWidget(TEXT("PopupCampPlacement")))
		return;

	uiManager->OpenAndGetWidget(TEXT("PopupCampPlacement"));
}

void FGsCampManager::CloseCampPlacementMenuUI()
{
	UGsUIManager* uiManager = GUI();
	if (nullptr == uiManager)
		return;

	if (false == uiManager->IsActiveWidget(TEXT("PopupCampPlacement")))
		return;

	uiManager->CloseByKeyName(TEXT("PopupCampPlacement"));
}

void FGsCampManager::OnBuildCamp()
{
	SendBuildCamp();

	UGsUIManager* uiManager = GUI();
	if (nullptr == uiManager)
		return;

	uiManager->CloseByKeyName(TEXT("PopupCampPlacement"));
}

void FGsCampManager::CancelBuildCamp()
{
	DespawnClientCampActor();

	UGsUIManager* uiManager = GUI();
	if (nullptr == uiManager)
		return;
	
	uiManager->CloseByKeyName(TEXT("PopupCampPlacement"));
}

void FGsCampManager::OnSpawnCampAck(const int64 InGameID)
{
	UGsGameObjectCamp* campObject = FindCampObject(InGameID);
	if (nullptr != campObject)
	{
		//campObject->SetConstructAffectNavigation(true);
		campObject->OnClickInteract(false);
	}

	FText findText;
	FText::FindText(TEXT("GuildText"), TEXT("Ticker_TowerConstructionStart"), findText);
	FGsUIHelper::TraySectionMessageTicker(EGsNotiSection::EXTSYSTEM, findText, TrayCondition::TC_Close);
}

void FGsCampManager::GetCampName(const int64 InGameID, FText& outText)
{
	UGsGameObjectCamp* campObject = FindCampObject(InGameID);
	if (nullptr == campObject)
		return;

	const FGsSchemaCampData* campData = FindCampData(InGameID);
	if (nullptr == campData)
		return;

	uint16 level = campObject->GetBuildingLevel();
	if (0 >= level || 0 >= campData->npcDataIdList.Num() || false == campData->npcDataIdList.IsValidIndex(level - 1))
	{
		GSLOG(Error, TEXT("[FGsCampManager::GetCampName] 0 >= curBuildingLevel || 0 >= data->npcDataIdList.Num() || false == data->npcDataIdList.IsValidIndex(curBuildingLevel - 1) "));
		return;
	}

	const FGsSchemaNpcData* npcData = campData->npcDataIdList[level - 1].GetRow();
	if (nullptr == npcData)
		return;

	outText = npcData->nameText;
}

int32 FGsCampManager::GetInteractionRange(const int64 InGameID)
{
	UGsGameObjectCamp* campObject = FindCampObject(InGameID);
	if (nullptr == campObject)
		return 0;

	const FGsSchemaCampData* campData = FindCampData(InGameID);
	if (nullptr == campData)
		return 0;

	uint16 level = campObject->GetBuildingLevel();
	if (0 >= level || 0 >= campData->npcDataIdList.Num() || false == campData->npcDataIdList.IsValidIndex(level - 1))
		return 0;	

	const FGsSchemaNpcData* npcData = campData->npcDataIdList[level - 1].GetRow();
	if (nullptr == npcData)
		return 0;

	return npcData->interactionRange;
}

UGsGameObjectLocalPlayer* FGsCampManager::GetLocalPlayerObject()
{
	if (false == _localPlayerPtr.IsValid())
	{
		if (false == FindLocalPlayerData())
			return nullptr;
	}

	return _localPlayerPtr.Get();
}

// 이 함수는 네비메시 체크 하지 않음
const bool FGsCampManager::IsOkBuildArea()
{
	if (nullptr == _clientSpawnCampActor)
		return false;

	if (nullptr == _campEffectData)
	{
		FString temp = "_campEffectData is nullptr";
		FGsUIHelper::TrayMessageTicker(FText::FromString(temp));
		return false;
	}

	FindSearchObject();
	if (false == IsAllowMap() || false == _isAllowObjectDist)
	{
#if WITH_EDITOR
		GSLOG(Warning, TEXT("[Reason] - is not allow IsAllowMap or OtherObject Distance"));
#endif

		FText findText;
		FText::FindText(TEXT("GuildText"), TEXT("Alert_CannotBuilding"), findText);
		FGsUIHelper::TrayMessageTicker(findText);
		return false;
	}

	return true;
}

bool FGsCampManager::IsAllowMap()
{
	// 인터서버에서 설치 불가능
	if (GGameData()->IsInvadeWorld())
	{
		return false;
	}

	// 안전지대 및 마을 안에서는 설치 불가능
	if (FGsGameFlowGame* gameFlow = GMode()->GetGameFlow())
	{
		if (FGsStageManagerGame* stage = gameFlow->GetStageManagerGame())
		{
			if (FGsStageMode::SAFETY_ZONE == stage->GetCurrentStageMode()
				|| FGsStageMode::TOWN == stage->GetCurrentStageMode()
				|| FGsStageMode::ARENA == stage->GetCurrentStageMode()
				|| FGsStageMode::ARENA_READY == stage->GetCurrentStageMode())
			{
				//FText findText;
				//FText::FindText(TEXT("GuildText"), TEXT("CAMP_ERROR_SAFE_TYPE_TERRITORY"), findText);
				//FGsUIHelper::TrayMessageTicker(findText);
				return false;
			}
		}
	}

	int32 levelKey = GLevel()->GetCurrentLevelId();
	const int32* findID = _campEffectData->allowedMapIdList.FindByPredicate([&levelKey](int32 InMapID)
		{
			return levelKey == InMapID;
		});

	if (nullptr == findID)
	{
		return false;
	}
	return true;
}

const bool FGsCampManager::IsAllowedEnemyCampDist(const UGsGameObjectBase* InTargetObject , const float InDiaMeter)
{
	int32 enemyCampDistance = _campEffectData->enemyCampDistance;
	float getDistance = _clientSpawnCampActor->GetDistanceTo(InTargetObject->GetActor());
	if (enemyCampDistance >= getDistance)
	{
		_isAllowObjectDist = false;
	}
	return _isAllowObjectDist;
}

const bool FGsCampManager::IsAllowedOtherCampDist(const UGsGameObjectBase* InTargetObject, const float InDiaMeter)
{
	int32 otherCampDistance = _campEffectData->otherCampDistance;
	float getDistance = _clientSpawnCampActor->GetDistanceTo(InTargetObject->GetActor());
	if (otherCampDistance >= getDistance)
	{
		_isAllowObjectDist = false;
	}
	return _isAllowObjectDist;
}

void FGsCampManager::ObjectDistanceCheck()
{
	for (const UGsGameObjectBase* iter : _findObjectList)
	{
		float diaMeter = _clientSpawnCampActor->GetCampDiameter();
		if (EGsGameObjectType::Camp == iter->GetObjectType())
		{
			if (_localPlayerPtr.IsValid())
			{
				if (true == _localPlayerPtr->IsEnemy(const_cast<UGsGameObjectBase*>(iter)))
				{
					_isAllowObjectDist = IsAllowedEnemyCampDist(iter, diaMeter);
				}
				else
				{
					_isAllowObjectDist = IsAllowedOtherCampDist(iter, diaMeter);
				}
			}
		}
		else
		{
			// 시공의틈새 워프쪽만 dist 더 넓게 체크(기획팀 니즈)
			if (EGsGameObjectType::PropInteraction == iter->GetObjectType())
			{
				diaMeter *= 1.1f;
			}
			float getDistance = _clientSpawnCampActor->GetDistanceTo(iter->GetActor());
			if (diaMeter >= getDistance)
			{
				_isAllowObjectDist = false;
			}
		}
	}

	// 서버에서 동적으로 스폰시키는 Prop 위치도 체크해야함
	if (_isAllowObjectDist)
	{
		for (FVector iter : _serverSpawnPropObjectPosList)
		{
			float diaMeter = _clientSpawnCampActor->GetCampDiameter();
			float getDistance = FVector::Dist2D(_clientSpawnCampActor->GetActorLocation(), iter);
			if ((diaMeter *= 1.1f) >= getDistance)
			{
				_isAllowObjectDist = false;
				//GSLOG(Error, TEXT("_serverSpawnPropObjectPosList - iter pos : %s"), *iter.ToString());
				//GSLOG(Error, TEXT("_serverSpawnPropObjectPosList - getDistance : %f"), getDistance);
			}
		}
	}
}

void FGsCampManager::FindSearchObject()
{
	_isAllowObjectDist = true;
	_findObjectList.Empty();
	FindObjectList(EGsGameObjectType::Camp, EGsGameObjectType::DropObject, EGsGameObjectType::NonPlayer, EGsGameObjectType::RemotePlayer,
		EGsGameObjectType::Sandbag, EGsGameObjectType::Warp, EGsGameObjectType::PropInteraction, EGsGameObjectType::PropObject, EGsGameObjectType::PropInteractInvasion);
}

void FGsCampManager::FindObjectList()
{
	ObjectDistanceCheck();
}

void FGsCampManager::FindByObjectType(EGsGameObjectType InType)
{
	TArray<UGsGameObjectBase*> findObjectList = GSGameObject()->FindObjectArray(InType);
	for (const UGsGameObjectBase* iter : findObjectList)
	{
		if(nullptr == iter || nullptr == iter->GetActor())
			continue;

		_findObjectList.Emplace(iter);
	}
}

void FGsCampManager::AddInteractionUser(const int64 InCampObjectGameID)
{
	if (_interactingCampObjectMap.Contains(InCampObjectGameID))
	{
		++_interactingCampObjectMap[InCampObjectGameID];
	}
	else
	{
		_interactingCampObjectMap.Emplace(InCampObjectGameID, 1);
	}

	UGsGameObjectCamp* campObject = FindCampObject(InCampObjectGameID);
	if(nullptr != campObject)
	{
		campObject->StartInteractionEffect();
	}
}

void FGsCampManager::RemoveInteractionUser(const int64 InCampObjectGameID)
{
	if (_interactingCampObjectMap.Contains(InCampObjectGameID))
	{
		UGsGameObjectCamp* campObject = FindCampObject(InCampObjectGameID);
		if (nullptr == campObject)
		{
			_interactingCampObjectMap.Remove(InCampObjectGameID);
			return;
		}

		--_interactingCampObjectMap[InCampObjectGameID];
		if (0 >= _interactingCampObjectMap[InCampObjectGameID])
		{
			// All Cancel Interaction CampObject
			campObject->StopInteractionEffect();
			_interactingCampObjectMap.Remove(InCampObjectGameID);
		}
	}
}

void FGsCampManager::DestroyCamp_Test(const uint32 InCampID)
{
	FGsNetSendServiceWorld::SendCancelCamp(InCampID);
}

void FGsCampManager::ShowCampErrorPopupUI(FString InErrorMsg)
{
	if (UGsUIManager* uiMgr = GUI())
	{
		TWeakObjectPtr<UGsUIWidgetBase> widget = uiMgr->OpenAndGetWidget(TEXT("PopupOK"));
		if (widget.IsValid())
		{
			UGsUIPopupOK* popup = Cast<UGsUIPopupOK>(widget.Get());
			if (popup)
			{
				popup->SetData(FText::FromString(InErrorMsg));
			}
		}
	}
}