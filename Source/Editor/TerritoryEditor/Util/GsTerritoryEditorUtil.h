// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

//UE4
#include "CoreMinimal.h"
#include "Runtime/Engine/Classes/Engine/Selection.h"
#include "UObject/NoExportTypes.h"
/**
 * 
 */
const FString TERRITORY_EDIOTR_TITLE = "Territory editor";
const FString ONLY_ONE_TERRITORY_SPOT_SELECT = TEXT("Only one select spot ");
const FString NON_TERRITORY_SPOT_SELECT = TEXT("Object is not spot type");
const FString INVALID_TERRITORY_SPOT_INDEX_RANGE = TEXT("Index id is 1 ~ 1048575");

class UWorld;

class TERRITORYEDITOR_API FGsTerritoryEditorUtil
{
public:
	static bool TryGetLocationOfMouse(UWorld* in_world, FVector& out_worldLocation, FVector& out_faceDirection);
	static bool TryGetViewportCameraLocation(FVector& out_cameraLocation);
	static bool TryGetCollisionLocation(UWorld* in_world, const FVector& in_start, const FVector& in_end, FVector& out_location);	
	static bool TryGetLocationOfCollisionFromRightClick(UWorld* in_world, FVector& out_location);
	template<typename T = AActor>
	static T* GetMouseSelectedObject()
	{
		if (GEditor)
		{
			if (USelection* selection = GEditor->GetSelectedActors())
			{
				TArray<T*> selectArray;
				int32 num = selection->GetSelectedObjects<T>(selectArray);
				T* temp;

				for (int32 i = 0; i < num; ++i)
				{
					temp = selectArray[i];

					if (temp)
					{
						UE_LOG(LogTemp, Log, TEXT("Get selected object - select : %s"), *temp->GetActorLabel());

						return temp;							
					}
				}
			}
		}

		return nullptr;
	}

	template<typename T = AActor>
	static bool CheckSelectedObjectIsOnlyOne()
	{
		if (GEditor)
		{
			if (USelection* selection = GEditor->GetSelectedActors())
			{		
				TArray<T*> selectArray;
				return (1 ==  selection->GetSelectedObjects<T>(selectArray));		
			}
		}

		return false;
	}

	static void OpenWarningDialog(const FString& in_title, const FString& in_log);
	static void ShowWarningNotification(const FString& inNotifyText, const float inDuration = 3);
	static FRotator GetFaceRotationForEditorCamera(const FVector& in_actor);
	static void RefreshForceDetailPanel();	
	static bool IsEditorLevel(ULevel* inLevel);
	static void PrintRamUsage();
};
