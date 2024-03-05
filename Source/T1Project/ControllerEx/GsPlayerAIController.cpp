// Fill out your copyright notice in the Description page of Project Settings.


#include "GsPlayerAIController.h"
#include "VisualLogger/VisualLogger.h"
#include "Runtime/Engine/Classes/Engine/World.h"
#include "Runtime/Engine/Classes/GameFramework/Character.h"
#include "Runtime/Engine/Classes/GameFramework/SpringArmComponent.h"
#include "Runtime/Engine/Classes/GameFramework/Pawn.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Runtime/Engine/Classes/Camera/PlayerCameraManager.h"
#include "Runtime/Engine/Classes/Camera/CameraComponent.h"
#include "Runtime/GameplayTasks/Classes/GameplayTasksComponent.h"
#include "Runtime/Engine/Public/VisualLogger/VisualLoggerTypes.h"
#include "Runtime/AIModule/Classes/BrainComponent.h"
#include "GameFramework/PlayerInput.h"

void AGsPlayerAIController::OnPossess(APawn* in_pawn)
{
	//플레이어가 가진 플레이어 컨트롤러를 해제하지 말아야한다.

	//AAIController::OnPossess
	{
		// don't even try possessing pending-kill pawns
		if (in_pawn != nullptr && in_pawn->IsPendingKill())
		{
			return;
		}

		//Controller::OnPossess
		{
			const bool bNewPawn = GetPawn() != in_pawn;

			if (in_pawn != NULL)
			{
				/*if (bNewPawn && GetPawn())
				{
					UnPossess();
				}*/

				/*if (in_owenr->Controller != NULL)
				{
					InPawn->Controller->UnPossess();
				}*/

				//in_owenr->PossessedBy(this);

				SetPawn(in_pawn);

				// update rotation to match possessed pawn's rotation
				SetControlRotation(GetPawn()->GetActorRotation());

				GetPawn()->Restart();
			}

			if (bNewPawn)
			{
				OnNewPawn.Broadcast(GetPawn());
			}
		}

		if (GetPawn() == nullptr || in_pawn == nullptr)
		{
			return;
		}

		// not calling UpdateNavigationComponents() anymore. The PathFollowingComponent 
		// is now observing newly possessed pawns (via OnNewPawn)

		if (GetPathFollowingComponent())
		{
			GetPathFollowingComponent()->Initialize();
		}

		if (bWantsPlayerState)
		{
			ChangeState(NAME_Playing);
		}

		// a Pawn controlled by AI _requires_ a GameplayTasksComponent, so if Pawn 
		// doesn't have one we need to create it
		if (CachedGameplayTasksComponent == nullptr)
		{
			UGameplayTasksComponent* GTComp = GetPawn()->FindComponentByClass<UGameplayTasksComponent>();

			if (GTComp == nullptr)
			{
				GTComp = NewObject<UGameplayTasksComponent>(GetPawn(), TEXT("GameplayTasksComponent"));
				GTComp->RegisterComponent();
			}

			CachedGameplayTasksComponent = GTComp;
		}

		if (CachedGameplayTasksComponent && !CachedGameplayTasksComponent->OnClaimedResourcesChange.Contains(this, GET_FUNCTION_NAME_CHECKED(AAIController, OnGameplayTaskResourcesClaimed)))
		{
			CachedGameplayTasksComponent->OnClaimedResourcesChange.AddDynamic(this, &AAIController::OnGameplayTaskResourcesClaimed);

			REDIRECT_OBJECT_TO_VLOG(CachedGameplayTasksComponent, this);
		}
	}	
}

void AGsPlayerAIController::OnUnPossess()
{
	//플레이어가 가진 플레이어 컨트롤러를 해제하지 말아야한다.

	//AAIController::OnUnpossess
	{
		APawn* CurrentPawn = GetPawn();

		//AController::OnUnpossess
		{
			if (GetPawn() != NULL)
			{
				//Pawn->UnPossessed();
				SetPawn(NULL);
			}
		}	

		if (GetPathFollowingComponent())
		{
			GetPathFollowingComponent()->Cleanup();
		}

		if (bStopAILogicOnUnposses)
		{
			if (BrainComponent)
			{
				BrainComponent->Cleanup();
			}
		}

		if (CachedGameplayTasksComponent && (CachedGameplayTasksComponent->GetOwner() == CurrentPawn))
		{
			CachedGameplayTasksComponent->OnClaimedResourcesChange.RemoveDynamic(this, &AAIController::OnGameplayTaskResourcesClaimed);
			CachedGameplayTasksComponent = nullptr;
		}
	}
}

