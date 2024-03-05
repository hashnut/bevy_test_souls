#include "GsMovementStateStop.h"
#include "ActorEx/GsCharacterLocalPlayer.h"
#include "Runtime/Engine/Classes/GameFramework/PlayerController.h"
#include "Runtime/Engine/Classes/Components/CapsuleComponent.h"
#include "Util/GsDir.h"

//---------------------------------------------------------------------------------------------
// FGsMovementState
//---------------------------------------------------------------------------------------------
int16 FGsMovementStateStop::GetDirNetConverted() const
{
	_direction = _characterLocal->GetActorRotation().Vector();
	return FGsDir::RotatorToAngle(_characterLocal->GetActorRotation());
}

float FGsMovementStateStop::GetFallingSpeed() const
{
	return 0.0f;
}

const FVector& FGsMovementStateStop::GetDirection() const
{
	return FVector::ZeroVector;
}

const FVector& FGsMovementStateStop::GetLocation() const
{
	if (_characterLocal)
	{
		_location = _characterLocal->GetNavAgentLocation();
	}
	
	return  _location;
}

const FVector& FGsMovementStateStop::GetDestination() const
{
	if (_characterLocal)
	{
		_location = _characterLocal->GetNavAgentLocation();
	}
	return _location;
}


//-------------------------------------------------------------------------------------------------
// TGsState
//-------------------------------------------------------------------------------------------------
void FGsMovementStateStop::Enter()
{

}

void FGsMovementStateStop::Exit()
{

}

void FGsMovementStateStop::Update(float inDeltatime)
{

}