// Fill out your copyright notice in the Description page of Project Settings.
#include "GsTerritorySpotActionUtility.h"

//ue4
#include "Editor/Blutility/Public/EditorUtilityLibrary.h"

//territory editor
#include "TerritoryEditor.h"
#include "Editor/TerritoryEditor/GsTerritoryUniqueIDManager.h"
#include "Util/GsTerritoryLevelUtil.h"
#include "Util/GsTerritoryEditorUtil.h"
#include "GsTerritoryUniqueIDController.h"
#include "Actor/Spot/GsTerritorySpot.h"
#include "GsTerritoryEditorSetting.h"
#include "Actor/Spot/GsTerritoryWarpSpot.h"

const FString INVALID_SPOT_SELECT_ErrMsg = TEXT("Select only one spot");
const FString CANT_CHANGE_SAME_SPOT_TYPE_ErrMsg = TEXT("Can't chagne same spot type");

void UGsTerritorySpotActionUtility::Delete()
{
	const TArray<AActor*>& selectArray = UEditorUtilityLibrary::GetSelectionSet();

	for (AActor* actor : selectArray)
	{
		if (actor)
		{
			if (AGsTerritorySpot* spot = Cast<AGsTerritorySpot>(actor))
			{
				spot->DestroyManually();
			}
		}
	}
}

void UGsTerritorySpotActionUtility::ModifyIndex(int64 inIndex)
{
	if (inIndex > (int64)TERRITORY_INSTANCE_ID_MAX)
	{
#if SPOT_ACTION_UTILITY
		FGsTerritoryEditorUtil::OpenWarningDialog(TERRITORY_EDIOTR_TITLE, INVALID_TERRITORY_SPOT_INDEX_RANGE);
#endif

		return;
	}

	if (inIndex < 1)
	{
#if SPOT_ACTION_UTILITY
		FGsTerritoryEditorUtil::OpenWarningDialog(TERRITORY_EDIOTR_TITLE, INVALID_TERRITORY_SPOT_INDEX_RANGE);
#endif

		return;
	}

	FTerritoryEditor* instance = FTerritoryEditor::GetInstance();
	if (nullptr == instance)
	{
#if SPOT_ACTION_UTILITY
		UE_LOG(LogTerritoryEditor, Error, TEXT("Territory editor instance is  null"));
#endif

		return;
	}

	const TArray<AActor*>& selectArray = UEditorUtilityLibrary::GetSelectionSet();

	if (selectArray.Num() != 1)
	{
#if SPOT_ACTION_UTILITY
		FGsTerritoryEditorUtil::OpenWarningDialog(TERRITORY_EDIOTR_TITLE, ONLY_ONE_TERRITORY_SPOT_SELECT);
#endif

		return;
	}

	AActor* select = selectArray[0];
	if (nullptr == select)
	{	
#if SPOT_ACTION_UTILITY
		FGsTerritoryEditorUtil::OpenWarningDialog(TERRITORY_EDIOTR_TITLE, NON_TERRITORY_SPOT_SELECT);
#endif

		return;
	}

	AGsTerritorySpot* spot = Cast<AGsTerritorySpot>(select);
	if (nullptr == spot)
	{
#if SPOT_ACTION_UTILITY
		FGsTerritoryEditorUtil::OpenWarningDialog(TERRITORY_EDIOTR_TITLE, NON_TERRITORY_SPOT_SELECT);
#endif

		return;
	}

	ULevel* level = spot->GetLevel();

	//remove unique id		
	if (nullptr == instance->GetUniqueIDManager())
	{
#if SPOT_ACTION_UTILITY
		FGsTerritoryEditorUtil::OpenWarningDialog(TERRITORY_EDIOTR_TITLE, UNIQUE_ID_GENERATE_INVALID);
#endif

		return;
	}

	uint32 indexID = (uint32)inIndex;
	
	//Check can generate unique id
	if (false == instance->GetUniqueIDManager()->IsCanGenerateUniqueID(level, spot->GetTerritoryObjType(), indexID))
	{
#if SPOT_ACTION_UTILITY
		FGsTerritoryEditorUtil::OpenWarningDialog(TERRITORY_EDIOTR_TITLE, UNIQUE_ID_GENERATE_INVALID);
#endif

		return;
	}

	//remove unique id
	instance->GetUniqueIDManager()->Remove(level, spot->GetTerritoryObjType(), spot->_instanceID);

	int64 uniqueID;
	int64 instanceID;	

	//add unique id
	instance->GetUniqueIDManager()->TryGenerateUniqueID(level, spot->GetTerritoryObjType(), indexID, uniqueID, instanceID);

	//int64 uniqueID = instance->_uniqueIDManager->Add(level, spot->GetTerritoryObjType(), spot->GetTerritoryUniqueID());

	spot->SetTerritoryUniqueID(uniqueID, instanceID);			

	FString levelName = UGsTerritoryLevelUtil::GetLevelName(level);
	FString persistantLevelName;
	FString levelKey;
	
	if (UGsTerritoryLevelUtil::IsValidServerLevelName(levelName, persistantLevelName, levelKey))
	{
		FString newName = FGsTerritoryUniqueIDManager::CombineTerritoryActorName(levelKey, spot->GetTerritoryObjType(), spot->_instanceID);		

		spot->SetActorLabel(newName);
	}
	else
	{
		FString newName = FGsTerritoryUniqueIDManager::CombineInvalidTerrritoryActorName(spot->GetTerritoryObjType());

#if SPOT_ACTION_UTILITY
		FString log = FString::Printf(TEXT("Invalid territory actor created - name : %s"), *newName);
		FGsTerritoryEditorUtil::ShowWarningNotification(log);
#endif

		spot->SetActorLabel(newName);
	}	

	spot->InvalidSpotIndex();
	spot->InvalidWidgetText();
	spot->Modify();	
}

void UGsTerritorySpotActionUtility::ModifySpotName(FText inName)
{	
	const TArray<AActor*>& selectArray = UEditorUtilityLibrary::GetSelectionSet();
	if (selectArray.Num() != 1)
	{
#if SPOT_ACTION_UTILITY
		FGsTerritoryEditorUtil::OpenWarningDialog(TERRITORY_EDIOTR_TITLE, ONLY_ONE_TERRITORY_SPOT_SELECT);
#endif

		return;
	}

	AActor* select = selectArray[0];
	if (nullptr == select)
	{
#if SPOT_ACTION_UTILITY
		FGsTerritoryEditorUtil::OpenWarningDialog(TERRITORY_EDIOTR_TITLE, NON_TERRITORY_SPOT_SELECT);
#endif

		return;
	}

	AGsTerritorySpot* spot = Cast<AGsTerritorySpot>(select);
	if (nullptr == spot)
	{
#if SPOT_ACTION_UTILITY
		FGsTerritoryEditorUtil::OpenWarningDialog(TERRITORY_EDIOTR_TITLE, NON_TERRITORY_SPOT_SELECT);
#endif

		return;
	}

	spot->_name = inName;

	spot->InvalidWidgetText();
	spot->Modify();
}

void UGsTerritorySpotActionUtility::ChangeToNormalSpot(FText inName, float inRandomSpawnRange /*= 100*/)
{
	UGsTerritoryEditorSetting* settings = GetMutableDefault<UGsTerritoryEditorSetting>();
	if (nullptr == settings)
	{
		UE_LOG(LogTemp, Error, TEXT("Get TerritoryEditorSetting fail"));
		return;
	}

	UWorld* world = GEditor->GetEditorWorldContext().World();
	if (nullptr == world)
	{
		UE_LOG(LogTemp, Error, TEXT("Get world fail"));
		return;
	}

	//check is normal spot
	AGsTerritorySpot* sourceSpot = GetSelectedSpot();
	if (nullptr == sourceSpot)
	{
		FGsTerritoryEditorUtil::OpenWarningDialog(TERRITORY_EDIOTR_TITLE, INVALID_SPOT_SELECT_ErrMsg);
		return;
	}

	if (sourceSpot->IsA(AGsTerritoryNormalSpot::StaticClass()))
	{
		//can't change to same type
		FGsTerritoryEditorUtil::OpenWarningDialog(TERRITORY_EDIOTR_TITLE, CANT_CHANGE_SAME_SPOT_TYPE_ErrMsg);
		return;
	}

	//get instance id
	uint32 sourceIndex = sourceSpot->_index;
	float sourceRadius = sourceSpot->_radius;
	FText sourceName = sourceSpot->_name;
	FTransform sourceTransform = sourceSpot->GetTransform();

	if (inRandomSpawnRange > 0)
	{
		sourceRadius = inRandomSpawnRange;
	}

	if (false == inName.IsEmpty())
	{
		sourceName = inName;
	}

	//delete source spot
	sourceSpot->DestroyManually();
	
	//create new spot
	if (settings->_normalSpotClass)
	{
		if (AActor* actor = GEditor->AddActor(world->GetCurrentLevel(), settings->_normalSpotClass, sourceTransform))
		{
			if (AGsTerritoryNormalSpot* destSpot = Cast<AGsTerritoryNormalSpot>(actor))
			{				
				ULevel* level = destSpot->GetLevel();
				int64 tempInstanceId = destSpot->GetTerritoryInstanceId();
				//remove unique id
				FTerritoryEditor::GetInstance()->GetUniqueIDManager()->Remove(level, destSpot->GetTerritoryObjType(), tempInstanceId);

				int64 destUniqueID;
				int64 dsetInstanceID;				
				//add unique id
				FTerritoryEditor::GetInstance()->GetUniqueIDManager()->TryGenerateUniqueID(level, destSpot->GetTerritoryObjType(), sourceIndex, destUniqueID, dsetInstanceID);
				destSpot->SetTerritoryUniqueID(destUniqueID, dsetInstanceID);

				FString levelName = UGsTerritoryLevelUtil::GetLevelName(level);
				FString persistantLevelName;
				FString levelKey;
				FString destName;
				if (UGsTerritoryLevelUtil::IsValidServerLevelName(levelName, persistantLevelName, levelKey))
				{
					destName = FGsTerritoryUniqueIDManager::CombineTerritoryActorName(levelKey, destSpot->GetTerritoryObjType(), destSpot->_instanceID);
					destSpot->SetActorLabel(destName);
				}
				else
				{
					destName = FGsTerritoryUniqueIDManager::CombineInvalidTerrritoryActorName(destSpot->GetTerritoryObjType());

#if SPOT_ACTION_UTILITY
					FString log = FString::Printf(TEXT("Invalid territory actor created - name : %s"), *destName);
					FGsTerritoryEditorUtil::ShowWarningNotification(log);
#endif
					destSpot->SetActorLabel(destName);
				}
								
				destSpot->_radius = sourceRadius;
				destSpot->_name = sourceName;

				destSpot->InvalidShape();
				destSpot->InvalidSpotIndex();
				destSpot->InvalidWidgetText();
				destSpot->Modify();
			}
		}
	}
}

void UGsTerritorySpotActionUtility::ChangeToSpawnSpot(FText inName, IffTeamType inTeamType /*= IffTeamType::NEUTRAL*/, float inRandomSpawnRange /*= 100*/)
{
	UGsTerritoryEditorSetting* settings = GetMutableDefault<UGsTerritoryEditorSetting>();
	if (nullptr == settings)
	{
		UE_LOG(LogTemp, Error, TEXT("Get TerritoryEditorSetting fail"));
		return;
	}

	UWorld* world = GEditor->GetEditorWorldContext().World();
	if (nullptr == world)
	{
		UE_LOG(LogTemp, Error, TEXT("Get world fail"));
		return;
	}

	//check is same spot
	AGsTerritorySpot* sourceSpot = GetSelectedSpot();
	if (nullptr == sourceSpot)
	{
		FGsTerritoryEditorUtil::OpenWarningDialog(TERRITORY_EDIOTR_TITLE, INVALID_SPOT_SELECT_ErrMsg);
		return;
	}

	if (sourceSpot->IsA(AGsTerritorySpawnSpot::StaticClass()))
	{
		//can't change to same type
		FGsTerritoryEditorUtil::OpenWarningDialog(TERRITORY_EDIOTR_TITLE, CANT_CHANGE_SAME_SPOT_TYPE_ErrMsg);
		return;
	}

	//get instance id
	uint32 sourceIndex = sourceSpot->_index;
	float sourceRadius = sourceSpot->_radius;
	FText sourceName = sourceSpot->_name;
	FTransform sourceTransform = sourceSpot->GetTransform();

	if (inRandomSpawnRange > 0)
	{
		sourceRadius = inRandomSpawnRange;
	}

	if (false == inName.IsEmpty())
	{
		sourceName = inName;
	}

	//delete source spot
	sourceSpot->DestroyManually();

	//create new spot
	if (settings->_spawnSpotClass)
	{
		if (AActor* actor = GEditor->AddActor(world->GetCurrentLevel(), settings->_spawnSpotClass, sourceTransform))
		{
			if (AGsTerritorySpawnSpot* destSpot = Cast<AGsTerritorySpawnSpot>(actor))
			{
				ULevel* level = destSpot->GetLevel();
				int64 tempInstanceId = destSpot->GetTerritoryInstanceId();
				//remove unique id
				FTerritoryEditor::GetInstance()->GetUniqueIDManager()->Remove(level, destSpot->GetTerritoryObjType(), tempInstanceId);

				int64 destUniqueID;
				int64 dsetInstanceID;
				//add unique id
				FTerritoryEditor::GetInstance()->GetUniqueIDManager()->TryGenerateUniqueID(level, destSpot->GetTerritoryObjType(), sourceIndex, destUniqueID, dsetInstanceID);
				destSpot->SetTerritoryUniqueID(destUniqueID, dsetInstanceID);

				FString levelName = UGsTerritoryLevelUtil::GetLevelName(level);
				FString persistantLevelName;
				FString levelKey;
				FString destName;
				if (UGsTerritoryLevelUtil::IsValidServerLevelName(levelName, persistantLevelName, levelKey))
				{
					destName = FGsTerritoryUniqueIDManager::CombineTerritoryActorName(levelKey, destSpot->GetTerritoryObjType(), destSpot->_instanceID);
					destSpot->SetActorLabel(destName);
				}
				else
				{
					destName = FGsTerritoryUniqueIDManager::CombineInvalidTerrritoryActorName(destSpot->GetTerritoryObjType());

#if SPOT_ACTION_UTILITY
					FString log = FString::Printf(TEXT("Invalid territory actor created - name : %s"), *destName);
					FGsTerritoryEditorUtil::ShowWarningNotification(log);
#endif
					destSpot->SetActorLabel(destName);
				}

				destSpot->_radius = sourceRadius;
				destSpot->_name = sourceName;
				destSpot->_teamType = inTeamType;				

				destSpot->InvalidShape();
				destSpot->InvalidSpotIndex();
				destSpot->InvalidWidgetText();
				destSpot->Modify();
			}
		}
	}
}

void UGsTerritorySpotActionUtility::ChangeToResurrectSpot(FText inName, IffTeamType inTeamType /*= IffTeamType::NEUTRAL*/, float inRandomSpawnRange /*= 100*/)
{
	UGsTerritoryEditorSetting* settings = GetMutableDefault<UGsTerritoryEditorSetting>();
	if (nullptr == settings)
	{
		UE_LOG(LogTemp, Error, TEXT("Get TerritoryEditorSetting fail"));
		return;
	}

	UWorld* world = GEditor->GetEditorWorldContext().World();
	if (nullptr == world)
	{
		UE_LOG(LogTemp, Error, TEXT("Get world fail"));
		return;
	}

	//check is normal spot
	AGsTerritorySpot* sourceSpot = GetSelectedSpot();
	if (nullptr == sourceSpot)
	{
		FGsTerritoryEditorUtil::OpenWarningDialog(TERRITORY_EDIOTR_TITLE, INVALID_SPOT_SELECT_ErrMsg);
		return;
	}

	if (sourceSpot->IsA(AGsTerritoryResurrectionSpot::StaticClass()))
	{
		//can't change to same type
		FGsTerritoryEditorUtil::OpenWarningDialog(TERRITORY_EDIOTR_TITLE, CANT_CHANGE_SAME_SPOT_TYPE_ErrMsg);
		return;
	}

	//get instance id
	uint32 sourceIndex = sourceSpot->_index;
	float sourceRadius = sourceSpot->_radius;
	FText sourceName = sourceSpot->_name;
	FTransform sourceTransform = sourceSpot->GetTransform();

	if (inRandomSpawnRange > 0)
	{
		sourceRadius = inRandomSpawnRange;
	}

	if (false == inName.IsEmpty())
	{
		sourceName = inName;
	}

	//delete source spot
	sourceSpot->DestroyManually();

	//create new spot
	if (settings->_ressurectionSpotClass)
	{
		if (AActor* actor = GEditor->AddActor(world->GetCurrentLevel(), settings->_ressurectionSpotClass, sourceTransform))
		{
			if (AGsTerritoryResurrectionSpot* destSpot = Cast<AGsTerritoryResurrectionSpot>(actor))
			{
				ULevel* level = destSpot->GetLevel();
				int64 tempInstanceId = destSpot->GetTerritoryInstanceId();
				//remove unique id
				FTerritoryEditor::GetInstance()->GetUniqueIDManager()->Remove(level, destSpot->GetTerritoryObjType(), tempInstanceId);

				int64 destUniqueID;
				int64 dsetInstanceID;
				//add unique id
				FTerritoryEditor::GetInstance()->GetUniqueIDManager()->TryGenerateUniqueID(level, destSpot->GetTerritoryObjType(), sourceIndex, destUniqueID, dsetInstanceID);
				destSpot->SetTerritoryUniqueID(destUniqueID, dsetInstanceID);

				FString levelName = UGsTerritoryLevelUtil::GetLevelName(level);
				FString persistantLevelName;
				FString levelKey;
				FString destName;
				if (UGsTerritoryLevelUtil::IsValidServerLevelName(levelName, persistantLevelName, levelKey))
				{
					destName = FGsTerritoryUniqueIDManager::CombineTerritoryActorName(levelKey, destSpot->GetTerritoryObjType(), destSpot->_instanceID);
					destSpot->SetActorLabel(destName);
				}
				else
				{
					destName = FGsTerritoryUniqueIDManager::CombineInvalidTerrritoryActorName(destSpot->GetTerritoryObjType());

#if SPOT_ACTION_UTILITY
					FString log = FString::Printf(TEXT("Invalid territory actor created - name : %s"), *destName);
					FGsTerritoryEditorUtil::ShowWarningNotification(log);
#endif
					destSpot->SetActorLabel(destName);
				}

				destSpot->_radius = sourceRadius;
				destSpot->_name = sourceName;
				destSpot->_teamType = inTeamType;

				destSpot->InvalidShape();
				destSpot->InvalidSpotIndex();
				destSpot->InvalidWidgetText();
				destSpot->Modify();
			}
		}
	}
}

void UGsTerritorySpotActionUtility::ChangeToTeleportSpot(FText inName, IffTeamType inTeamType /*= IffTeamType::NEUTRAL*/, float inRandomSpawnRange /*= 100*/)
{
	UGsTerritoryEditorSetting* settings = GetMutableDefault<UGsTerritoryEditorSetting>();
	if (nullptr == settings)
	{
		UE_LOG(LogTemp, Error, TEXT("Get TerritoryEditorSetting fail"));
		return;
	}

	UWorld* world = GEditor->GetEditorWorldContext().World();
	if (nullptr == world)
	{
		UE_LOG(LogTemp, Error, TEXT("Get world fail"));
		return;
	}

	//check is normal spot
	AGsTerritorySpot* sourceSpot = GetSelectedSpot();
	if (nullptr == sourceSpot)
	{
		FGsTerritoryEditorUtil::OpenWarningDialog(TERRITORY_EDIOTR_TITLE, INVALID_SPOT_SELECT_ErrMsg);
		return;
	}

	if (sourceSpot->IsA(AGsTerritoryTeleportSpot::StaticClass()))
	{
		//can't change to same type
		FGsTerritoryEditorUtil::OpenWarningDialog(TERRITORY_EDIOTR_TITLE, CANT_CHANGE_SAME_SPOT_TYPE_ErrMsg);
		return;
	}

	//get instance id
	uint32 sourceIndex = sourceSpot->_index;
	float sourceRadius = sourceSpot->_radius;
	FText sourceName = sourceSpot->_name;
	FTransform sourceTransform = sourceSpot->GetTransform();

	if (inRandomSpawnRange > 0)
	{
		sourceRadius = inRandomSpawnRange;
	}

	if (false == inName.IsEmpty())
	{
		sourceName = inName;
	}

	//delete source spot
	sourceSpot->DestroyManually();

	//create new spot
	if (settings->_teleportSpotClass)
	{
		if (AActor* actor = GEditor->AddActor(world->GetCurrentLevel(), settings->_teleportSpotClass, sourceTransform))
		{
			if (AGsTerritoryTeleportSpot* destSpot = Cast<AGsTerritoryTeleportSpot>(actor))
			{
				ULevel* level = destSpot->GetLevel();
				int64 tempInstanceId = destSpot->GetTerritoryInstanceId();
				//remove unique id
				FTerritoryEditor::GetInstance()->GetUniqueIDManager()->Remove(level, destSpot->GetTerritoryObjType(), tempInstanceId);

				int64 destUniqueID;
				int64 dsetInstanceID;
				//add unique id
				FTerritoryEditor::GetInstance()->GetUniqueIDManager()->TryGenerateUniqueID(level, destSpot->GetTerritoryObjType(), sourceIndex, destUniqueID, dsetInstanceID);
				destSpot->SetTerritoryUniqueID(destUniqueID, dsetInstanceID);

				FString levelName = UGsTerritoryLevelUtil::GetLevelName(level);
				FString persistantLevelName;
				FString levelKey;
				FString destName;
				if (UGsTerritoryLevelUtil::IsValidServerLevelName(levelName, persistantLevelName, levelKey))
				{
					destName = FGsTerritoryUniqueIDManager::CombineTerritoryActorName(levelKey, destSpot->GetTerritoryObjType(), destSpot->_instanceID);
					destSpot->SetActorLabel(destName);
				}
				else
				{
					destName = FGsTerritoryUniqueIDManager::CombineInvalidTerrritoryActorName(destSpot->GetTerritoryObjType());

#if SPOT_ACTION_UTILITY
					FString log = FString::Printf(TEXT("Invalid territory actor created - name : %s"), *destName);
					FGsTerritoryEditorUtil::ShowWarningNotification(log);
#endif
					destSpot->SetActorLabel(destName);
				}

				destSpot->_radius = sourceRadius;
				destSpot->_name = sourceName;
				destSpot->_teamType = inTeamType;

				destSpot->InvalidShape();
				destSpot->InvalidSpotIndex();
				destSpot->InvalidWidgetText();
				destSpot->Modify();
			}
		}
	}
}

void UGsTerritorySpotActionUtility::ChangeToWarp(FGsTerritoryMapWarp inInput, WarpType inWarpType, float inRadius /*= 100.0f*/, int32 inShapeID /*= 8282*/)
{
	UGsTerritoryEditorSetting* settings = GetMutableDefault<UGsTerritoryEditorSetting>();
	if (nullptr == settings)
	{
		UE_LOG(LogTemp, Error, TEXT("Get TerritoryEditorSetting fail"));
		return;
	}

	UWorld* world = GEditor->GetEditorWorldContext().World();
	if (nullptr == world)
	{
		UE_LOG(LogTemp, Error, TEXT("Get world fail"));
		return;
	}

	//check is normal spot
	AGsTerritorySpot* sourceSpot = GetSelectedSpot();
	if (nullptr == sourceSpot)
	{
		FGsTerritoryEditorUtil::OpenWarningDialog(TERRITORY_EDIOTR_TITLE, INVALID_SPOT_SELECT_ErrMsg);
		return;
	}

	if (sourceSpot->IsA(AGsTerritoryWarpSpot::StaticClass()))
	{
		//can't change to same type
		FGsTerritoryEditorUtil::OpenWarningDialog(TERRITORY_EDIOTR_TITLE, CANT_CHANGE_SAME_SPOT_TYPE_ErrMsg);
		return;
	}

	AGsTerritoryWarpSpot* destWarpSpot = nullptr;
	AGsTerritoryWarp* destWarp = nullptr;

	//get instance id
	uint32 sourceIndex = sourceSpot->_index;
	float sourceRadius = sourceSpot->_radius;
	FText sourceName = sourceSpot->_name;
	FTransform sourceTransform = sourceSpot->GetTransform();

	if (inRadius > 0)
	{
		sourceRadius = inRadius;
	}

	//delete source spot
	sourceSpot->DestroyManually();

	//create new spot
	if (settings->_warpSpotClass)
	{
		if (AActor* actor = GEditor->AddActor(world->GetCurrentLevel(), settings->_warpSpotClass, sourceTransform))
		{
			if (AGsTerritoryWarpSpot* destSpot = Cast<AGsTerritoryWarpSpot>(actor))
			{
				ULevel* level = destSpot->GetLevel();
				int64 tempInstanceId = destSpot->GetTerritoryInstanceId();
				//remove unique id
				FTerritoryEditor::GetInstance()->GetUniqueIDManager()->Remove(level, destSpot->GetTerritoryObjType(), tempInstanceId);

				int64 destUniqueID;
				int64 dsetInstanceID;
				//add unique id
				FTerritoryEditor::GetInstance()->GetUniqueIDManager()->TryGenerateUniqueID(level, destSpot->GetTerritoryObjType(), sourceIndex, destUniqueID, dsetInstanceID);
				destSpot->SetTerritoryUniqueID(destUniqueID, dsetInstanceID);

				FString levelName = UGsTerritoryLevelUtil::GetLevelName(level);
				FString persistantLevelName;
				FString levelKey;
				FString destName;
				if (UGsTerritoryLevelUtil::IsValidServerLevelName(levelName, persistantLevelName, levelKey))
				{
					destName = FGsTerritoryUniqueIDManager::CombineTerritoryActorName(levelKey, destSpot->GetTerritoryObjType(), destSpot->_instanceID);
					destSpot->SetActorLabel(destName);
				}
				else
				{
					destName = FGsTerritoryUniqueIDManager::CombineInvalidTerrritoryActorName(destSpot->GetTerritoryObjType());

#if SPOT_ACTION_UTILITY
					FString log = FString::Printf(TEXT("Invalid territory actor created - name : %s"), *destName);
					FGsTerritoryEditorUtil::ShowWarningNotification(log);
#endif
					destSpot->SetActorLabel(destName);
				}

				destSpot->_radius = sourceRadius;
				destSpot->_name = sourceName;				

				destSpot->InvalidShape();
				destSpot->InvalidSpotIndex();
				destSpot->InvalidWidgetText();
				destSpot->Modify();

				destWarpSpot = destSpot;
			}
		}
	}

	if (settings->_warpClass)
	{
		if (AActor* actor = GEditor->AddActor(world->GetCurrentLevel(), settings->_warpClass, sourceTransform))
		{
			if (AGsTerritoryWarp* warp = Cast<AGsTerritoryWarp>(actor))
			{
				warp->_destMapID = inInput.DestMapID;
				warp->_destSpotIndex = inInput.SpotIndex;
				warp->_warpType = inWarpType;
				warp->_shapeID = inShapeID;
				destWarp = warp;
			}
		}
	}

	if (destWarpSpot)
	{
		destWarpSpot->SetWarp(destWarp);
	}
}


AGsTerritorySpot* UGsTerritorySpotActionUtility::GetSelectedSpot()
{
	TArray<AActor*> selectArray = UEditorUtilityLibrary::GetSelectionSet();
	if (selectArray.Num() == 1)
	{
		if (AGsTerritorySpot * spot = Cast<AGsTerritorySpot>(selectArray[0]))
		{
			return spot;
		}
	}

	FString log = TEXT("영역이 없습니다.");
	FGsTerritoryEditorUtil::OpenWarningDialog(TERRITORY_EDIOTR_TITLE, log);

	return nullptr;
}