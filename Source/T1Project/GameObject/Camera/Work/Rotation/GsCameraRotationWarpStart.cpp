#include "GsCameraRotationWarpStart.h"

#include "ActorEx/GsCharacterLocalPlayer.h"

#include "GameObject/ObjectClass/GsGameObjectLocalPlayer.h"

#include "GameFramework/PlayerController.h"

void FGsCameraRotationWarpStart::SetRotation(const FRotator& In_rot)
{
	if (_local == nullptr)
	{
		return;
	}
	AGsCharacterLocalPlayer* localChar = _local->GetLocalCharacter();
	
	if (localChar->Controller == nullptr ||
		localChar->Controller->IsLocalPlayerController() == false)
	{
		return;
	}
	localChar->Controller->SetControlRotation(In_rot);
	
}