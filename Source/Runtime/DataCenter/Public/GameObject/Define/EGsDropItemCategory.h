#pragma once

#include "EGsDropItemCategory.generated.h"

UENUM(BlueprintType)
enum class EGsDropItemCategory : uint8
{	
	Weapon,	
	Armor,	
	Accessory,
	Consumable,
	Max
};
