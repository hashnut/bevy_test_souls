
#include "GsGameObjectPropInvasionTreasure.h"
#include "GsGameObjectPropInteraction.h"
#include "Management/GsMessageHolder.h"
#include "Message/GsMessageGameObject.h"

void UGsGameObjectPropInvasionTreasure::CreateSubClass()
{
	Super::CreateSubClass();

	// invade treasure box make minimap icon
	if (_propData)
	{
		FGsGameObjectMessageParamMapIcon paramIcon(this, EGsMapIconType::InvasionTreasureBox);
		GMessage()->GetGameObject().SendMessage(MessageGameObject::MINIMAP_SPAWN_ICON, &paramIcon);
	}
}

void UGsGameObjectPropInvasionTreasure::ActorDespawn()
{
	// invade treasure box delete minimap icon
	if (_propData)
	{		
		FGsGameObjectMessageParamMapIcon paramIcon(GetGameId());
		GMessage()->GetGameObject().SendMessage(MessageGameObject::MINIMAP_DESPAWN_ICON, &paramIcon);
	}

	Super::ActorDespawn();
}

bool UGsGameObjectPropInvasionTreasure::IsForceHideInteractionButton()
{
	return _isCanActiveInteractionWidget == false;
}