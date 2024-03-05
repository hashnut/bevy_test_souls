// Fill out your copyright notice in the Description page of Project Settings.


#include "WidgetComponentEx.h"
//#include "T1Project/UTIL/GsEditorUtile.h"
#include "T1Project/UTIL/GsUIUtil.h"
#include "Editor/GsReregisterComponentInterface.h"

#define INVALID_WIDGET true

UWidgetComponentEx::UWidgetComponentEx()
{
	PrimaryComponentTick.bCanEverTick = true;

#if WITH_EDITOR
	bTickInEditor = true;
#endif
}

void UWidgetComponentEx::OnRegister()
{
	Super::OnRegister();

#if WITH_EDITOR
	if (GetOwner())
	{
		if (IGsReregisterComponentInterface* owner = Cast<IGsReregisterComponentInterface>(GetOwner()))
		{
			owner->ReregisterComponent();
		}		
	}	
#endif
}

void UWidgetComponentEx::TickComponent(float in_deltaTime, enum ELevelTick inTickType, FActorComponentTickFunction *in_ThisTickFunction)
{
	Super::TickComponent(in_deltaTime, inTickType, in_ThisTickFunction);

#if INVALID_WIDGET
	if (false == _isVisible)
	{
		SetInvisibleWidget();
		return;
	}

	//Check distance 
	if (_isDistanceCheck)
	{
		if (IsInViewDistance(GetWorld(), inTickType))
		{
			SetVisibleWidget();

			if (Space == EWidgetSpace::World)
			{
				if (_enableFaceToCamera)
				{
					FaceToCameraTick(in_deltaTime, inTickType);
				}
			}
		}
		else
		{
			SetInvisibleWidget();
		}
	}
	else
	{
		if (Space == EWidgetSpace::World)
		{
			if (_enableFaceToCamera)
			{
				FaceToCameraTick(in_deltaTime, inTickType);
			}
		}
	}
#endif

}

void UWidgetComponentEx::FaceToCameraTick(float in_deltaTime, enum ELevelTick in_tickType)
{
	if (ELevelTick::LEVELTICK_ViewportsOnly == in_tickType)
	{
#if WITH_EDITOR
		SetWorldRotation(UGsUIUtil::GetFaceRotationForEditorCamera(GetComponentLocation()));
#endif
	}
	else
	{
		FRotator rot;

		if (UGsUIUtil::TryGetRotationForFacePlayerCamera(GetWorld(), GetComponentLocation(), rot))
		{
			SetWorldRotation(rot);
		}
	}
}

bool UWidgetComponentEx::IsInViewDistance(UWorld* inWorld, enum ELevelTick inTickType)
{
	FVector location;

	if (ELevelTick::LEVELTICK_ViewportsOnly == inTickType)
	{
#if WITH_EDITOR
		if (UGsUIUtil::TryGetEditorCameraLocation(location))
		{
			return ((location - GetComponentLocation()).Size() < _viewDistance);
		}
#endif
	}
	else
	{
		if (UGsUIUtil::TryGetCameraLocation(inWorld, location))
		{
			return ((location - GetComponentLocation()).Size() < _viewDistance);
		}
	}

	return false;
}

void UWidgetComponentEx::SetVisibleWidget()
{
	if (GetUserWidgetObject())
	{
		if (GetUserWidgetObject()->GetVisibility() == ESlateVisibility::Hidden)
		{
			GetUserWidgetObject()->SetVisibility(ESlateVisibility::Visible);
		}
	}
}

void UWidgetComponentEx::SetInvisibleWidget()
{
	if (GetUserWidgetObject())
	{
		if (GetUserWidgetObject()->GetVisibility() != ESlateVisibility::Hidden)
		{
			GetUserWidgetObject()->SetVisibility(ESlateVisibility::Hidden);
		}
	}
}
