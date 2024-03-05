// Fill out your copyright notice in the Description page of Project Settings.


#include "GsDropObjectSpawnComponent.h"
#include "ActorEx/GsActorDropObject.h"

#include "UTIL/GsTableUtil.h"
#include "UTIL/GsItemUtil.h"

#include "GsTableManager.h"
#include "WidgetComponent.h"
#include "T1Project.h"
#include "DrawDebugHelpers.h"

#include "Item/GsSchemaItemCommon.h"
#include "GameObject/ObjectClass/GsGameObjectDropObject.h"
#include "Data/GsDataContainManager.h"

#include "Management/GsMessageHolder.h"
#include "Util/GsTimeUtil.h"


UGsDropObjectSpawnComponent::UGsDropObjectSpawnComponent()
{
	//PrimaryTick	
	PrimaryComponentTick.bCanEverTick = true;
}

void UGsDropObjectSpawnComponent::EndPlay(const EEndPlayReason::Type inReason)
{
	if (_timer)
	{
		_timer->Clear();
	}	

	Super::EndPlay(inReason);
}	

void UGsDropObjectSpawnComponent::Start(const FVector& inStart, const FVector& inDest)
{
	if (nullptr == GData()->GetGlobalData())
	{
		return;
	}

	_spawnCurve = GData()->GetGlobalData()->_dropObjectSpawnCurve;
	if (nullptr == _spawnCurve)
	{
		GSLOG(Error, TEXT("Drop item spawn curve is missing"));
	}

	float spawnDuration = GData()->GetGlobalData()->_dropObjectSpawnDuration;
	_height = GData()->GetGlobalData()->_dropObjectSpawnHeight;

	_destLocation = inDest;
	_startLocation = inStart;
	_direction = (inDest - inStart);
	_degree = _direction.Rotation().Yaw;	
	_timer = NewObject<UGsSimpleTimer>();		

	if (_timer)
	{		
		_timer->_tickDelegate.AddDynamic(this, &UGsDropObjectSpawnComponent::OnSpawn);
		_timer->_finishDelegate.AddDynamic(this, &UGsDropObjectSpawnComponent::OnFinishSpawn);

		_timer->SetMaxTime(spawnDuration);
		_timer->Start();
	}
}

void UGsDropObjectSpawnComponent::TickComponent(float inDelta, ELevelTick inTickType, FActorComponentTickFunction* inThisTickFunction)
{
	Super::TickComponent(inDelta, inTickType, inThisTickFunction);

	if (_timer)
	{
		if (false == _timer->IsFinish())
		{
			_timer->Tick(inDelta);
		}		
	}
}

void UGsDropObjectSpawnComponent::OnSpawn(float inDelta)
{
	if (_spawnCurve)
	{
		float heightRatio = _spawnCurve->GetFloatValue(_timer->GetCurrentTimeRatio());
		float height = _height * heightRatio;

		_direction.RotateAngleAxis(_degree, FVector::UpVector);
		FVector currentLocation = (_direction * _timer->GetCurrentTimeRatio()) + FVector(0, 0, height) + _startLocation;

		GetOwner()->SetActorLocation(currentLocation);

#if DROPOJBECT_EFFECT
		DrawDebugSphere(GetWorld(), currentLocation, 50.0f, 8, FColor::Blue, false, 3.0f);
#endif
	}
}

void UGsDropObjectSpawnComponent::OnFinishSpawn(const UObject* inTimer)
{
	if (GetOwner())
	{
		GetOwner()->SetActorLocation(_destLocation);

		if (AGsActorDropObject* dropItem = Cast<AGsActorDropObject>(GetOwner()))
		{
			dropItem->Active(true);			


			FDropObjectSpawnFinishMessage param = FDropObjectSpawnFinishMessage(dropItem);
			GMessage()->GetGameObject().SendMessage(MessageGameObject::DROPOBJECT_SPAWN_FINISH, &param);

			if (_timer)
			{
				_timer->Clear();
			}			

#if DROPOJBECT_EFFECT
			DrawDebugSphere(GetWorld(), _destLocation, 50.0f, 8, FColor::Yellow, false, 3.0f);
#endif
		}
	}

	Deactivate();
}