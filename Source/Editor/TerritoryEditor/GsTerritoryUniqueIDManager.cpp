#include "GsTerritoryUniqueIDManager.h"

//UE4
#include "Editor.h"
#include "Misc/MessageDialog.h"
#include "Engine/DataTable.h"
#include "WorldBrowserModule.h"
#include "Engine/WorldComposition.h"
#include "Kismet/GameplayStatics.h"

//T1 Project
#include "RSQLEditor/Public/GsTableManagerEditor.h"
#include "Runtime/Public/GsTable.h"
#include "UTIL/GsText.h"

//Territory editor
#include "Actor/Spawn/GsTerritoryActor.h"
#include "GsTerritoryEditorSetting.h"
#include "Util/GsTerritoryLevelUtil.h"
#include "Util/GsTerritoryEditorUtil.h"
#include "TerritoryEditor.h"

FGsTerritoryUniqueIDManager::~FGsTerritoryUniqueIDManager()
{
#if TERRITORY_EDITOR_DEBUG
	UE_LOG(LogTerritoryEditor, Log, TEXT("FGsTerritoryUniqueIDManager::~FGsTerritoryUniqueIDManager"));
#endif

	Clear();
}

void FGsTerritoryUniqueIDManager::Initialize()
{
#if TERRITORY_EDITOR_DEBUG
	UE_LOG(LogTerritoryEditor, Log, TEXT("FGsTerritoryUniqueIDManager::Initialize"));
#endif

	if(_exportDataHandle.IsValid())
	{
		FEditorDelegates::PostSaveWorld.Remove(_exportDataHandle);
	}	
	_exportDataHandle = FEditorDelegates::PostSaveWorld.AddRaw(this, &FGsTerritoryUniqueIDManager::OnPostSaveUniqueTable);

	_isGenerateUniqueIDValid = false;
}

void FGsTerritoryUniqueIDManager::Deinitialize()
{
	UE_LOG(LogTerritoryEditor, Log, TEXT("FGsTerritoryUniqueIDManager::Deinitialize"));

	if (_exportDataHandle.IsValid())
	{
		FEditorDelegates::PostSaveWorld.Remove(_exportDataHandle);
	}
}

void FGsTerritoryUniqueIDManager::Clear()
{
	TArray<FString> keyArray;
	_serverLevelInstanceIDController.GenerateKeyArray(keyArray);
	for (const FString& key : keyArray)
	{
		if (_serverLevelInstanceIDController.Contains(key))
		{
			if (_serverLevelInstanceIDController[key].IsValid())
			{
				_serverLevelInstanceIDController.Reset();
			}
		}
	}

	_serverLevelInstanceIDController.Empty();
}

void FGsTerritoryUniqueIDManager::OnCheckServerLevelLoaded()
{	
	UE_LOG(LogTerritoryEditor, Log, TEXT("FGsTerritoryUniqueIDManager::OnCheckServerLevelLoaded"));

	//check using world composition
	UWorld* world = GEditor->GetEditorWorldContext().World();
	if (nullptr == world)
	{
		return;
	}

	if (nullptr == world->WorldComposition)
	{
		return;
	}

	//Get current select world tile model
	ULevel* currentLevel = world->GetCurrentLevel();

	Clear();

	//check server level

	if (currentLevel)
	{
		const FString& path = currentLevel->GetPathName();
		if (UGsTerritoryLevelUtil::IsServerLevel(path))
		{
			InitServerLevelUniqueIDControllers(currentLevel);
			//Initialize infos
		}
	}

	_isGenerateUniqueIDValid = true;
}

bool FGsTerritoryUniqueIDManager::InitServerLevelUniqueIDControllers(ULevel* inLevel)
{
	UE_LOG(LogTerritoryEditor, Log, TEXT("FGsTerritoryUniqueIDManager::InitServerLevelUniqueIDControllers"));

	const FString& levelName = UGsTerritoryLevelUtil::GetLevelName(inLevel);
	if (levelName.IsEmpty())
	{
		return false;
	}

#if TERRITORY_EDITOR_DEBUG
	if (inLevel)
	{
		UE_LOG(LogTerritoryEditor, Log, TEXT("FGsTerritoryUniqueIDManager::InitServerLevelUniqueIDControllers - init unique id manager : %s"), *levelName);
	}
#endif

	TSharedPtr<FGsTerritoryServerLevelInstanceIDController> controller = MakeShareable(new FGsTerritoryServerLevelInstanceIDController());
	if (false == controller.IsValid())
	{
		return false;
	}

	_serverLevelInstanceIDController.Emplace(levelName, controller);
	controller->Initialize(inLevel);

	return true;
}

void FGsTerritoryUniqueIDManager::OnPostSaveUniqueTable(uint32 in_saveFlag, UWorld* in_world, bool in_success)
{
#if TERRITORY_EDITOR_DEBUG
	UE_LOG(LogTerritoryEditor, Log, TEXT("FGsTerritoryUniqueIDManager::OnPostSaveUniqueTable"));
#endif

	if (in_success)
	{
		_isGenerateUniqueIDValid = true;
	}
}

bool FGsTerritoryUniqueIDManager::TryGenerateUniqueID(ULevel* inLevel, EGsTerritoryObjType inObjType, int64& outUniqueID, int64& outInstanceID)
{	
	outUniqueID = 1;
	outInstanceID = 1;	

	FGsTerritoryServerLevelInstanceIDController* controller = GetServerController(inLevel);
	if (nullptr == controller)
	{
		return false;
	}

	return controller->TryGenerateUniqueID(inObjType, outUniqueID, outInstanceID);
}

bool FGsTerritoryUniqueIDManager::TryGenerateUniqueID(ULevel* inLevel, EGsTerritoryObjType inObjType, uint32& inInstanceID, int64& outUniqueID, int64& outInstanceID)
{	
	outUniqueID = 1;
	outInstanceID = 1;	

	FGsTerritoryServerLevelInstanceIDController* controller = GetServerController(inLevel);
	if (nullptr == controller)
	{
		return false;
	}

	return controller->TryGenerateUniqueID(inObjType, inInstanceID, outUniqueID, outInstanceID);
}

void FGsTerritoryUniqueIDManager::AddUniqueIdForce(ULevel* inLevel, EGsTerritoryObjType inObjType, uint32& inInstanceID)
{
	FGsTerritoryServerLevelInstanceIDController* controller = GetServerController(inLevel);
	if (nullptr == controller)
	{
		return;
	}

	controller->AddForce(inObjType, inInstanceID);
}

bool FGsTerritoryUniqueIDManager::IsCanGenerateUniqueID(ULevel* inLevel, EGsTerritoryObjType inObjType)
{
	FGsTerritoryServerLevelInstanceIDController* controller = GetServerController(inLevel);
	if (nullptr == controller)
	{
		return false;
	}

	return controller->IsCanGenerateUniqueID(inObjType);
}

bool FGsTerritoryUniqueIDManager::IsCanGenerateUniqueID(ULevel* inLevel, EGsTerritoryObjType inObjType, uint32& inInstanceID, bool inIsForceCheck)
{
	FGsTerritoryServerLevelInstanceIDController* controller = GetServerController(inLevel);
	if (nullptr == controller)
	{
		return false;
	}

	return controller->IsCanGenerateUniqueID(inObjType, inInstanceID, inIsForceCheck);
}

bool FGsTerritoryUniqueIDManager::Remove(ULevel* inLevel, EGsTerritoryObjType inObjType, int64& inInstanceID)
{	
	FGsTerritoryServerLevelInstanceIDController* controller = GetServerController(inLevel);
	if (nullptr == controller)
	{
		return false;
	}

#if TERRITORY_EDITRO_UNIQUEID_DEBUG
	const FString& levelName = UGsTerritoryLevelUtil::GetLevelName(inLevel);
	UE_LOG(LogTerritoryEditor, Log, TEXT("FGsTerritoryUniqueIDManager::Remove - level : %s"), *levelName);
#endif

	return controller->Remove(inObjType, inInstanceID);
}

FString FGsTerritoryUniqueIDManager::CombineTerritoryActorName(const FString& inLevelKey, const EGsTerritoryObjType inObjectType, const int64& inInstanceID)
{				
	const FString& objectType = FGsTextUtil::GetStringFromEnum("EGsTerritoryObjType", inObjectType);
	return FString::Printf(TEXT("%s_%s_%07lld"), *inLevelKey, *objectType, inInstanceID);
}

FString FGsTerritoryUniqueIDManager::CombineInvalidTerrritoryActorName(const EGsTerritoryObjType inObjectType)
{
	const FString& objectType = FGsTextUtil::GetStringFromEnum("EGsTerritoryObjType", inObjectType);
	return FString::Printf(TEXT("Invalid_%s"), *objectType);
}

void FGsTerritoryUniqueIDManager::InvalidTerritoryActorName(FString inLevelKey, AGsTerritoryActor* inActor)
{
	if (nullptr == inActor)
	{
		return;
	}	

	const FString& label = CombineTerritoryActorName(inLevelKey, inActor->GetTerritoryObjType(), inActor->GetTerritoryInstanceId());
	inActor->SetActorLabel(label);
}

FGsTerritoryServerLevelInstanceIDController* FGsTerritoryUniqueIDManager::GetServerController(ULevel* inLevel)
{
	if (_isGenerateUniqueIDValid)
	{
		const FString& levelName = UGsTerritoryLevelUtil::GetLevelName(inLevel);
		if (_serverLevelInstanceIDController.Contains(levelName))
		{
			if (_serverLevelInstanceIDController[levelName].IsValid())
			{
				return _serverLevelInstanceIDController[levelName].Get();
			}			
		}
	}

	return nullptr;
}

void FGsTerritoryUniqueIDManager::RefreshWorldBrowser()
{
	UE_LOG(LogTerritoryEditor, Log, TEXT("FGsTerritoryUniqueIDManager::RefreshWorldBrowser"));
	if (UWorld * world = GEngine->GetWorld())
	{
		world->WorldComposition->Rescan();

		FWorldBrowserModule& worldBrowserModule = FModuleManager::LoadModuleChecked<FWorldBrowserModule>("WorldBrowser");

		worldBrowserModule.OnBrowseWorld.Broadcast(world);
	}
}

bool FGsTerritoryUniqueIDManager::SetUniqueID(AActor* inActor)
{	
	if (nullptr == inActor)
	{		
		return false;
	}

	UE_LOG(LogTerritoryEditor, Log, TEXT("FGsTerritoryUniqueIDManager::SetUniqueID - label : %s"), *inActor->GetActorLabel());

	AGsTerritoryActor* actor = Cast<AGsTerritoryActor>(inActor);
	if (nullptr == actor)
	{
		UE_LOG(LogTemp, Log, TEXT("TerritoryActor - actor is nulltpr - name : %s"), *actor->GetActorLabel());

		return false;
	}		

	//생성 후 삭제시 Zone 같은 경우 자식 액터가 생성되기 때문에 문제가 발생할 수 있다. 		
	//그리고 강제로 두번 복사해도 문제가 발생한다. 
	//따라서 생성은 허용하되 유니크 아이디를 관리하지 않는다.		

	const FString& typeText = FGsTextUtil::GetStringFromEnum(TEXT("EGsTerritoryObjType"), actor->GetTerritoryObjType());
	if (false == IsCanGenerateUniqueID(inActor->GetLevel(), actor->GetTerritoryObjType()))
	{
		const FString log = TEXT("Can't generate unique id");
		FGsTerritoryLevelManager::SetInvalidTerritoryActor(actor, log);
		return false;
	}

	if (false == UGsTerritoryLevelUtil::IsServerLevel(inActor->GetLevel()->GetPathName()))
	{
		const FString& log = TEXT("Actor is not in server level");
		FGsTerritoryLevelManager::SetInvalidTerritoryActor(actor, log);
		return false;
	}

	//복사한 액터라면 Unique id가 INDEX_NONE이 아닐 것 이다. 
	//복사한 액터의 원본 데이터를 찾아 복사시 사용한다.
	FString invalidReason;
	if (false == actor->InvalidTerritoryUniqueID(invalidReason))
	{		
		FGsTerritoryLevelManager::SetInvalidTerritoryActor(actor, invalidReason);
		return false;
	}

	//if (actor->IsNeedToCopy())
	//{
	//	actor->Copy();
	//}		

	const FString& levelName = UGsTerritoryLevelUtil::GetLevelName(actor->GetLevel()->GetPathName());
	FString levelKey = TEXT("NonServerLevel");
	FString persistantLevelName;
	UGsTerritoryLevelUtil::IsValidServerLevelName(levelName, persistantLevelName, levelKey);
	const FString& newName = FGsTerritoryUniqueIDManager::CombineTerritoryActorName(levelKey, actor->GetTerritoryObjType(), actor->_instanceID);
	actor->SetActorLabel(newName);

	actor->InvalidWidgetText();

	return true;
}
