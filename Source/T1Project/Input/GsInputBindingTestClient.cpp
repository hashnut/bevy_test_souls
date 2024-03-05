// Fill out your copyright notice in the Description page of Project Settings.


#include "GsInputBindingTestClient.h"
#include "Engine/Classes/GameFramework/PlayerController.h"
#include "GameObject/Movement/LocalPlayer/GsMovementStateInput.h"
#include "GameObject/State/GsGameObjectStateManager.h"
#include "GameObject/ObjectClass/GsGameObjectLocalPlayer.h"
#include "GameObject/Camera/GsCameraModeHandler.h"

#include "ActorEx/GsCharacterLocalPlayer.h"
#include "Management/ScopeGame/GsCameraModeManager.h"

void UGsInputBindingTestClient::Initialize(class UGsGameObjectLocalPlayer* target)
{
	Super::Initialize(target);

	_local = target;
}

void UGsInputBindingTestClient::Update(float Delta)
{
	//if (_owner)
	//{
	//	if (_axisValue.Size() > 0.f || _prevAxisValue != _axisValue)
	//	{
	//		// 추후 Movement 클래스에서 LocalClient용 클래스를 제작해서 구현
	//		// @see : FGsMovementState
	//		FVector2D scale(_axisValue.Y, _axisValue.X);
	//		FVector dir = _local->GetLocalCharacter()->Controller->GetControlRotation().RotateVector(
	//			FVector(scale.X, scale.Y, 0.f).GetClampedToMaxSize(1.0f));
	//		float scaleSize = scale.Size();
	//		scaleSize = (scaleSize > 0.f ? (scaleSize < FGsMovementStateInput::CacheChangeScaleRateRunWalk ?
	//			FGsMovementStateInput::CacheChangeScaleRateRunWalk : 1.f) : 0.f);
	//		if (scale.SizeSquared() > 0.f)
	//		{
	//			_local->GetBaseFSM()->ProcessEvent(EGsStateBase::Run);
	//		}
	//		else
	//		{
	//			_local->GetBaseFSM()->ProcessEvent(EGsStateBase::Idle);
	//		}
	//		_local->GetLocalCharacter()->AddMovementInput(dir, scaleSize);

	//		if (FGsCameraModeHandler* cameraModehandler = _owner->GetCameraModeHandler())
	//		{
	//			cameraModehandler->GetState()->SetAxisValue(_axisValue);
	//		}

	//		// 이전에 값이 있고
	//		// 새로 들어온값이 없으면
	//		// 조이스틱 땐 순간임
	//		if (_prevAxisValue.Size() > 0.f &&
	//			_axisValue.Size() == 0.0f)
	//		{
	//			OnReleaseJoystick();
	//		}

	//		// 이전에 값이 없었고
	//		// 새로 들어온 값이 있으면
	//		// 조이스틱 누르는 순간임
	//		if (_prevAxisValue.Size() == 0.0f &&
	//			_axisValue.Size() > 0.f)
	//		{
	//			OnPressJoystick();
	//		}


	//		_prevAxisValue = _axisValue;
	//	}
	//}
}
