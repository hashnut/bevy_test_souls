#include "GsDrawDebugHelpers.h"

#include "Kismet/GameplayStatics.h"

#include "Management/ScopeGlobal/GsLevelManager.h"

#include "Management/ScopeGame/GsGameObjectManager.h"
#include "Management/ScopeGame/GsSummonManager.h"

#include "GameObject/ObjectClass/GsGameObjectLocalPlayer.h"

#include "ActorEx/GsCharacterLocalPlayer.h"

#include "GameFlow/GameContents/ContentsGame/Hud/Summon/GsSummonHandler.h"

#include "GameFramework/PlayerController.h"

#include "Engine/Classes/Engine/LocalPlayer.h"
#include "Engine/Classes/Engine/GameViewportClient.h"
#include "Engine/Public/SceneView.h"
#include "Engine/Public/UnrealClient.h"

#include "T1Project.h"

void FGsDrawDebugHelpers::DrawDebugFence(UWorld* inWorld, const TArray<FVector>& inFenceList, float inHeight)
{	
	int32 num = inFenceList.Num();
	int32 last = inFenceList.Num() - 1;

	int32 currentIndex;
	int32 nextIndex;

	FVector face;
	FVector start;
	FVector end;
	FVector center;
	FVector2D extension;
	float height = inHeight;

	for (int32 i = 0; i < inFenceList.Num(); ++i)
	{		
		if (i == last)
		{			
			currentIndex = last;
			nextIndex = 0;
		}
		else
		{
			currentIndex = i;
			nextIndex = i + 1;
		}

		start = inFenceList[currentIndex];
		end = inFenceList[nextIndex];
		center = (start + end) / 2;

		extension.X = FVector2D(start - end).Size() / 2;
		extension.Y = height;
		face = FVector::CrossProduct(FVector::UpVector, (start - end).GetSafeNormal());
		
		

		DrawDebugSolidPlane(inWorld, FPlane(center, face), center, extension, FColor::Green, true);
	}	

	for (int32 i = 0; i < inFenceList.Num(); ++i)
	{
		start = inFenceList[i];

		DrawDebugSolidBox(inWorld, start + FVector(0, 0, height / 2), FVector(50, 50, height), FColor::Purple, true);
	}

	for (int32 i = 0; i < inFenceList.Num(); ++i)
	{
		start = inFenceList[i];

		DrawDebugString(inWorld, start, FString::FromInt(i), nullptr, FColor::Black, 100.0f);
	}
}
void FGsDrawDebugHelpers::DrawDebugArrow(const FVector2D& In_touchPos)
{
	UWorld* world = GSGameObject()->GetWorld();
	if (world == nullptr)
	{
		return;
	}


	GSLOG(Log, TEXT("In_touchPos: %s"), *(In_touchPos.ToString()));
	

	if (APlayerController* playerController = world->GetFirstPlayerController())
	{
		FVector WorldOrigin;
		FVector WorldDirection;
		if (UGameplayStatics::DeprojectScreenToWorld(playerController, In_touchPos, WorldOrigin, WorldDirection) == true)
		{
			
			DrawDebugDirectionalArrow(world, WorldOrigin, WorldOrigin + WorldDirection * playerController->HitResultTraceDistance, 20.f, FColor::Red, false, 10.f);
		}


		ULocalPlayer* const LP = playerController ? playerController->GetLocalPlayer() : nullptr;
		if (LP && LP->ViewportClient)
		{
			// get the projection data
			FSceneViewProjectionData ProjectionData;
			if (LP->GetProjectionData(LP->ViewportClient->Viewport, eSSP_FULL, /*out*/ ProjectionData))
			{
				FMatrix const InvViewProjMatrix = ProjectionData.ComputeViewProjectionMatrix().InverseFast();
				
				//FSceneView::DeprojectScreenToWorld(ScreenPosition, ProjectionData.GetConstrainedViewRect(), InvViewProjMatrix, /*out*/ WorldPosition, /*out*/ WorldDirection);
				
				FVector2D ScreenPos = In_touchPos;
				const FIntRect& ViewRect = ProjectionData.GetConstrainedViewRect();


				float PixelX = FMath::TruncToFloat(ScreenPos.X);
				float PixelY = FMath::TruncToFloat(ScreenPos.Y);

				// Get the eye position and direction of the mouse cursor in two stages (inverse transform projection, then inverse transform view).
				// This avoids the numerical instability that occurs when a view matrix with large translation is composed with a projection matrix

				// Get the pixel coordinates into 0..1 normalized coordinates within the constrained view rectangle
				const float NormalizedX = (PixelX - ViewRect.Min.X) / ((float)ViewRect.Width());
				const float NormalizedY = (PixelY - ViewRect.Min.Y) / ((float)ViewRect.Height());

				// Get the pixel coordinates into -1..1 projection space
				const float ScreenSpaceX = (NormalizedX - 0.5f) * 2.0f;
				const float ScreenSpaceY = ((1.0f - NormalizedY) - 0.5f) * 2.0f;

				// The start of the ray trace is defined to be at mousex,mousey,1 in projection space (z=1 is near, z=0 is far - this gives us better precision)
				// To get the direction of the ray trace we need to use any z between the near and the far plane, so let's use (mousex, mousey, 0.5)
				const FVector4 RayStartProjectionSpace = FVector4(ScreenSpaceX, ScreenSpaceY, 1.0f, 1.0f);
				const FVector4 RayEndProjectionSpace = FVector4(ScreenSpaceX, ScreenSpaceY, 0.5f, 1.0f);

				// Projection (changing the W coordinate) is not handled by the FMatrix transforms that work with vectors, so multiplications
				// by the projection matrix should use homogeneous coordinates (i.e. FPlane).
				const FVector4 HGRayStartWorldSpace = InvViewProjMatrix.TransformFVector4(RayStartProjectionSpace);
				const FVector4 HGRayEndWorldSpace = InvViewProjMatrix.TransformFVector4(RayEndProjectionSpace);
				FVector RayStartWorldSpace(HGRayStartWorldSpace.X, HGRayStartWorldSpace.Y, HGRayStartWorldSpace.Z);
				FVector RayEndWorldSpace(HGRayEndWorldSpace.X, HGRayEndWorldSpace.Y, HGRayEndWorldSpace.Z);
				// divide vectors by W to undo any projection and get the 3-space coordinate
				if (HGRayStartWorldSpace.W != 0.0f)
				{
					RayStartWorldSpace /= HGRayStartWorldSpace.W;
				}
				if (HGRayEndWorldSpace.W != 0.0f)
				{
					RayEndWorldSpace /= HGRayEndWorldSpace.W;
				}
				const FVector RayDirWorldSpace = (RayEndWorldSpace - RayStartWorldSpace).GetSafeNormal();


				GSLOG(Log, TEXT("RayStartWorldSpace: %s"), *(RayStartWorldSpace.ToString()));
				GSLOG(Log, TEXT("RayDirWorldSpace: %s"), *(RayDirWorldSpace.ToString()));

			}
		}
	}
}