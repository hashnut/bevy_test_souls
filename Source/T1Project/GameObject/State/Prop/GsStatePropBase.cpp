
#include "GsStatePropBase.h"
//#include "GsStateNonPlayerHeader.h"
#include "GsStatePropIdle.h"
#include "GsStatePropSpawn.h"
#include "GsStatePropAction.h"
#include "GsStatePropFinish.h"
#include "Animation/GsPropAnimInstance.h"

IGsStateBase* FGsStateBaseSingleProp::GetState(uint8 StateID) const
{
	switch (static_cast<EGsStateProp>(StateID))
	{
	case EGsStateProp::Spawn:
		return FGsStatePropSpawn::GetInstance();
	case EGsStateProp::Idle:
		return FGsStatePropIdle::GetInstance();	
	case EGsStateProp::Action:
		return FGsStatePropAction::GetInstance();
	case EGsStateProp::Finish:
		return FGsStatePropFinish::GetInstance();
	default:
		GSLOG(Error, TEXT("State Type undefined %d"), StateID);
		return nullptr;
	}
	return nullptr;
}