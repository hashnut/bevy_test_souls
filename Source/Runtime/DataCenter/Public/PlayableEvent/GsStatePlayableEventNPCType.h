#pragma once
#include "CoreMinimal.h"
#include "GsStatePlayableEventNPCType.generated.h"

UENUM(BlueprintType)
enum class EGsStatePlayableEventNPC : uint8
{
	None = 0,
	Idle,				// use: lucifer(after skill)
	Spawn,				// use: demon_spear, demon_axe( -> Attack looping)
	CastingLooping,		// use: temir, traiq
	AttackLooping,		// use: temir, traiq(after action), demon_spear, demon_axe
	Action,				// use: temir, traiq(-> attack looping)
	Skill,				// use: lucifer(->idle)
	Damaged,			// use: demon_spear, demon_axe
};