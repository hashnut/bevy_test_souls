// Fill out your copyright notice in the Description page of Project Settings.


#include "GsTerritorySpawnActorActionUtility.h"

//UE4
#include "Editor/UnrealEd/Classes/Editor/UnrealEdEngine.h"
#include "Editor/UnrealEd/Public/UnrealEdGlobals.h"
#include "Editor/UnrealEd/Classes/Editor/EditorEngine.h"
#include "Editor/UnrealEd/Public/Editor.h"
#include "Runtime/Engine/Classes/Engine/Selection.h"
#include "UTIL/GsText.h"
#include "Editor/Blutility/Public/EditorUtilityLibrary.h"

//Territory editor
#include "Actor/Spawn/GsTerritoryActor.h"
#include "Actor/Spawn/Zone/GsTerritoryZone.h"
#include "Actor/Spawn/SpawnActor/GsTerritorySpawnActor.h"
#include "Actor/Spawn/SpawnActor/GsTerritoryNpc.h"
#include "TerritoryEditor.h"
#include "Util/GsTerritoryEditorUtil.h"
#include "TerritoryEditor.h"
#include "GsTerritoryUniqueIDManager.h"

const FString SELECT_FAIL = TEXT("선택된 대상이 없습니다.");
const FString SELECT_ONLY_ONE_ZONE = TEXT("하나의 Zone만 선택해 주세요");

void UGsTerritorySpawnActorActionUtility::RemoveTerritoryActor()
{
	AGsTerritorySpawnActor* actor = GetSelectedTerritoryActor();
	if (nullptr == actor)
	{
		FGsTerritoryEditorUtil::OpenWarningDialog(TERRITORY_EDIOTR_TITLE, *DELETE_FAIL);
		return;
	}

	actor->DestroyManually();

	FGsTerritoryEditorUtil::RefreshForceDetailPanel();
}

AGsTerritorySpawnActor* UGsTerritorySpawnActorActionUtility::GetSelectedTerritoryActor()
{
	AGsTerritorySpawnActor* selectedActor = FGsTerritoryEditorUtil::GetMouseSelectedObject<AGsTerritorySpawnActor>();
	if (selectedActor)
	{
		return selectedActor;
	}

	UE_LOG(LogTerritoryEditor, Error, TEXT("Territory actor aciton's select is null"));

	return nullptr;
}

void UGsTerritorySpawnActorActionUtility::SelectZone()
{
	if (nullptr == GEditor)
	{
		return;
	}

	AGsTerritorySpawnActor* selectedActor = FGsTerritoryEditorUtil::GetMouseSelectedObject<AGsTerritorySpawnActor>();
	if (nullptr == selectedActor)
	{
		FGsTerritoryEditorUtil::OpenWarningDialog(TERRITORY_EDIOTR_TITLE, *SELECT_FAIL);
		return;
	}

	GEditor->SelectNone(true, true);
	GEditor->SelectActor(selectedActor->_zone, true, true);
}

void UGsTerritorySpawnActorActionUtility::Copy()
{
	UWorld* world = GEditor->GetEditorWorldContext().World();

	if (false == IsSelectTerritoryChildActor())
	{
		FGsTerritoryEditorUtil::OpenWarningDialog(TERRITORY_EDIOTR_TITLE, SELECT_ONLY_ONE_ZONE);

		return;
	}

	FTerritoryEditor* instance = FTerritoryEditor::GetInstance();
	if (nullptr == instance)
	{
		FGsTerritoryEditorUtil::OpenWarningDialog(TERRITORY_EDIOTR_TITLE, *INSTANCE_MISSING);
		return;
	}

	const TArray<AActor*>& selectArray = UEditorUtilityLibrary::GetSelectionSet();
	AGsTerritorySpawnActor* child;

	for (AActor* actor : selectArray)
	{
		child = Cast<AGsTerritorySpawnActor>(actor);

		if (nullptr == child)
		{
			continue;
		}
		
		if (child->_zone)
		{
			FGsTerritorySpawnInfo data = child->_spawnInfo;
			data.UniqueID = -1;

			if (false == instance->GetUniqueIDManager()->IsCanGenerateUniqueID(world->GetCurrentLevel(), child->GetTerritoryObjType()))
			{
				FString objectType = FGsTextUtil::GetStringFromEnum(TEXT("EGsTerritoryObjType"), child->GetTerritoryObjType());
				FString log = objectType  + TEXT("의 Unique ID를 생성할 수 없습니다.");

				FGsTerritoryEditorUtil::OpenWarningDialog(TERRITORY_EDIOTR_TITLE, *log);
				return;
			}

			child->_zone->AddTerritorySpawnActor(data, child->GetActorLocation());
		}
	}
}

void UGsTerritorySpawnActorActionUtility::SetSpawnLevel(int32 inLevel)
{
	TArray<AActor*> selectArray = UEditorUtilityLibrary::GetSelectionSet();
	for(AActor* actor : selectArray)
	{
		if (AGsTerritorySpawnActor* spawnActor = Cast<AGsTerritorySpawnActor>(actor))
		{
			spawnActor->SetSpawnLevel(inLevel);
		}
	}	
}

bool UGsTerritorySpawnActorActionUtility::IsSelectOnlyOneActor()
{
	const TArray<AActor*>& selectArray = UEditorUtilityLibrary::GetSelectionSet();

	if (selectArray.Num() == 1)
	{
		if (AGsTerritorySpawnActor* actor = Cast<AGsTerritorySpawnActor>(selectArray[0]))
		{
			return true;
		}
	}

	return false;
}

bool UGsTerritorySpawnActorActionUtility::IsSelectTerritoryChildActor()
{
	const TArray<AActor*>& selectArray = UEditorUtilityLibrary::GetSelectionSet();

	if (selectArray.Num() > 0)
	{
		for (AActor* actor : selectArray)
		{
			if (AGsTerritorySpawnActor* zone = Cast<AGsTerritorySpawnActor>(actor))
			{
				if (nullptr == zone)
				{
					FString log = TEXT("none zone select");

					UE_LOG(LogTerritoryEditor, Error, TEXT("%s"), *log);

					return false;
				}
			}
		}

		return true;
	}

	return false;
}