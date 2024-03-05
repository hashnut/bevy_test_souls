// Fill out your copyright notice in the Description page of Project Settings.


#include "GsTerritoryEditorUtil.h"

//cygwin
//#include <sys/resource.h>
//#include <stdio.h>
//#include <stdlib.h>
//#include <errno.h>
//#include <unistd.h>

//UE4
#include "Editor/UnrealEd/Classes/Editor/EditorEngine.h"
#include "Editor/UnrealEd/Public/LevelEditorViewport.h"
#include "Editor/UnrealEd/Public/Editor.h"
#include "Editor/UnrealEd/Classes/Editor/EditorEngine.h"
#include "Editor/UnrealEd/Public/EditorViewportClient.h"
#include "Editor/UnrealEd/Classes/Editor/UnrealEdEngine.h"
#include "Editor/UnrealEd/Public/UnrealEdGlobals.h"
#include "Editor/PropertyEditor/Public/DetailCategoryBuilder.h"
#include "Editor/PropertyEditor/Public/PropertyEditorModule.h"
#include "Runtime/Core/Public/Modules/ModuleManager.h"
#include "Runtime/Engine/Public/SceneView.h"
#include "Runtime/Engine/Classes/Engine/GameViewportClient.h"
#include "Runtime/UMG/Public/Blueprint/WidgetLayoutLibrary.h"
#include "Runtime/Engine/Public/DrawDebugHelpers.h"
#include "Runtime/Engine/Public/Engine.h"
#include "Runtime/Engine/Public/UnrealClient.h"
#include "Runtime/Engine/Classes/Kismet/KismetMathLibrary.h"
#include "Widgets/Notifications/SNotificationList.h"
#include "Framework/Notifications/NotificationManager.h"

//Territory editor
#include "Define/GsTerritoryZoneShapeDefine.h"
#include "Define/GsTerritorySpawnDefine.h"
#include "GsTerritoryEditorSetting.h"
#include "TerritoryEditor.h"


bool FGsTerritoryEditorUtil::TryGetLocationOfMouse(UWorld* in_world, FVector& out_worldLocation, FVector& out_faceDirection)
{
	UE_LOG(LogTerritoryEditor, Log, TEXT("FGsTerritoryEditorUtil::TryGetLocationOfMouse"));

	const FVector2D& mouseLocation = UWidgetLayoutLibrary::GetMousePositionOnViewport(in_world);
	FLevelEditorViewportClient* client = (FLevelEditorViewportClient*)(GEditor->GetActiveViewport()->GetClient());
	if (client)
	{
		FVector2D screenLocation(mouseLocation);
		FSceneViewFamilyContext viewFamily(FSceneViewFamily::ConstructionValues(
			client->Viewport,
			client->GetScene(),
			client->EngineShowFlags));

		FSceneView* view = client->CalcSceneView(&viewFamily);
		if (view)
		{
			view->DeprojectFVector2D(screenLocation
				, out_worldLocation
				, out_faceDirection);

			return true;
		}
	}

	return false;
}

bool FGsTerritoryEditorUtil::TryGetViewportCameraLocation(FVector& out_cameraLocation)
{
	UE_LOG(LogTerritoryEditor, Log, TEXT("FGsTerritoryEditorUtil::TryGetViewportCameraLocation"));

	if (GEditor)
	{
		if (GEditor->GetActiveViewport())
		{
			FLevelEditorViewportClient* client = (FLevelEditorViewportClient*)GEditor->GetActiveViewport()->GetClient();
			if (client)
			{
				out_cameraLocation = client->GetViewLocation();

				return true;
			}
		}
	}

	return false;
}

bool FGsTerritoryEditorUtil::TryGetCollisionLocation(UWorld* in_world, const FVector& in_start, const FVector& in_end, FVector& out_location)
{
	UE_LOG(LogTerritoryEditor, Log, TEXT("FGsTerritoryEditorUtil::TryGetCollisionLocation"));

	FHitResult hit;
	FCollisionObjectQueryParams objectQueryParams;
	objectQueryParams.AddObjectTypesToQuery(ECC_GameTraceChannel5);	

	//DrawDebugLine(in_world, in_start, in_end, FColor::Yellow, false, 10.0f);	

	if (in_world->LineTraceSingleByObjectType(hit, in_start, in_end, objectQueryParams))
	{
		out_location = hit.Location;

		//DrawDebugSphere(in_world, out_location, 100, 12, FColor::Blue, false, 10.0f);

		return true;
	}

	return false;
}

bool FGsTerritoryEditorUtil::TryGetLocationOfCollisionFromRightClick(UWorld* in_world, FVector& out_location)
{
	if (nullptr == in_world)
	{
		return false;
	}

	if (nullptr == GEditor)
	{
		return false;
	}

	UE_LOG(LogTerritoryEditor, Log, TEXT("FGsTerritoryEditorUtil::TryGetLocationOfCollisionFromRightClick"));

	FVector cameraLocation;
	if (TryGetViewportCameraLocation(cameraLocation))
	{
		FVector collisionLocation;
		const FVector& direction = (GEditor->ClickLocation - cameraLocation).GetSafeNormal();
		const FVector& endLocation = GEditor->ClickLocation + (direction * 10000.0f);

		if (TryGetCollisionLocation(in_world, cameraLocation, endLocation, collisionLocation))
		{
			out_location = collisionLocation;

			UE_LOG(LogTerritoryEditor, Log, TEXT("Collision success - mouse location : %s"), *collisionLocation.ToString());

			return true;
		}
	}

	UE_LOG(LogTerritoryEditor, Log, TEXT("Collision fail - mouse location : %s"), *cameraLocation.ToString());

	return false;
}

void FGsTerritoryEditorUtil::OpenWarningDialog(const FString& in_title, const FString& in_log)
{
	UE_LOG(LogTerritoryEditor, Log, TEXT("FGsTerritoryEditorUtil::OpenWarningDialog"));
	UE_LOG(LogTerritoryEditor, Error, TEXT("Worng batched - %s"), *in_log);

	FText title = FText::FromString(in_title);

	FMessageDialog::Open(EAppMsgType::Ok,
		FText::FromString(in_log),
		&title);
}

void FGsTerritoryEditorUtil::ShowWarningNotification(const FString& inNotifyText, const float inDuration)
{
	UE_LOG(LogTerritoryEditor, Log, TEXT("FGsTerritoryEditorUtil::ShowWarningNotification"));

	FNotificationInfo notify = FNotificationInfo(FText::FromString(inNotifyText));	

	notify.FadeInDuration = inDuration;
	notify.FadeOutDuration = inDuration + 0.5;
	notify.ExpireDuration = inDuration + 1;

	TSharedPtr<SNotificationItem> item = FSlateNotificationManager::Get().AddNotification(notify);
	if (item.IsValid())
	{
		item->SetCompletionState(SNotificationItem::ECompletionState::CS_Fail);
	}
}

FRotator FGsTerritoryEditorUtil::GetFaceRotationForEditorCamera(const FVector& in_actor)
{
	UE_LOG(LogTerritoryEditor, Log, TEXT("FGsTerritoryEditorUtil::GetFaceRotationForEditorCamera"));

	FLevelEditorViewportClient* Client = (FLevelEditorViewportClient*)(GEditor->GetActiveViewport()->GetClient());
	if (Client)
	{
		return UKismetMathLibrary::FindLookAtRotation(in_actor, Client->GetViewLocation());
	}

	return FRotator::ZeroRotator;
}

void FGsTerritoryEditorUtil::RefreshForceDetailPanel()
{
	UE_LOG(LogTerritoryEditor, Log, TEXT("FGsTerritoryEditorUtil::RefreshForceDetailPanel"));

	FPropertyEditorModule& propertyModule = FModuleManager::GetModuleChecked<FPropertyEditorModule>(TEXT("PropertyEditor"));
	TSharedPtr<IDetailsView> detailPanel = propertyModule.FindDetailView(FName(TEXT("LevelEditorSelectionDetails")));
	if (detailPanel.IsValid())
	{
		detailPanel.Get()->ForceRefresh();
	}
}

bool FGsTerritoryEditorUtil::IsEditorLevel(ULevel* inLevel)
{
	UE_LOG(LogTerritoryEditor, Log, TEXT("FGsTerritoryEditorUtil::IsEditorLevel"));

	FString name = inLevel->GetOuter()->GetName();
	TArray<FString> splitArray;

	FString right;
	FString left;

	while (name.Split("_", &left, &right))
	{
		name = right;

		splitArray.Add(left);
	}

	splitArray.Add(name);

	if (splitArray.Num() != 2)
	{
		return false;
	}

	if (false == splitArray[0].Equals("World"))
	{
		return false;
	}

	if (false == splitArray[1].IsNumeric())
	{
		return false;
	}

	return true;
}

void FGsTerritoryEditorUtil::PrintRamUsage()
{
	/*struct rusage r_usage;
	int ret = getrusage(RUSAGE_SELF, &r_usage);
	if (ret == 0)
	{
		UE_LOG(LogTerritoryEditor, Log, TEXT("FGsTerritoryEditorUtil::PrintRamUsage - %lld mbyte"), r_usage.ru_maxrss / 1024);
	}		*/
}
