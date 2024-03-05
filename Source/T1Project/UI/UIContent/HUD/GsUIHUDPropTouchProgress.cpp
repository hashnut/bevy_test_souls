// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUIHUDPropTouchProgress.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "Management/ScopeGame/GsGameObjectManager.h"
#include "GameObject/Interaction/GsInteractionHandlerLocalPlayer.h"
#include "GameObject/ObjectClass/GsGameObjectLocalPlayer.h"

void UGsUIHUDPropTouchProgress::SetName(const FText& inName)
{
	if (_nameText)
	{
		_nameText->SetText(inName);
	}
}

void UGsUIHUDPropTouchProgress::SetTime(float inElapsed)
{
	if (_elapsedTimeText)
	{
		FString value = FString::Printf(TEXT("%.1fs"), inElapsed);		
		_elapsedTimeText->SetText(FText::FromString(value));
	}
}

void UGsUIHUDPropTouchProgress::SetRatio(float inRatio)
{
	if (_interactionProgressbar)
	{
		_interactionProgressbar->SetPercent(inRatio);
	}	
}

void UGsUIHUDPropTouchProgress::SetHUDMode(EGsUIHUDMode inType)
{

}

void UGsUIHUDPropTouchProgress::InvalidateAllInternal()
{
	UGsGameObjectBase* owner = GSGameObject()->FindObject(EGsGameObjectType::LocalPlayer);
	if (owner)
	{
		UGsGameObjectLocalPlayer* local = owner->CastGameObject<UGsGameObjectLocalPlayer>();
		if (local)
		{
			FGsInteractionHandlerLocalPlayer* interactionHandler = local->GetCastInteraction<FGsInteractionHandlerLocalPlayer>();
			if (interactionHandler)
			{
				if (nullptr == interactionHandler->GetInteractionTarget())
				{
					SetVisibility(ESlateVisibility::Hidden);
				}
				else
				{
					SetVisibility(ESlateVisibility::Visible);
				}				
			}
		}			
	}	
}
