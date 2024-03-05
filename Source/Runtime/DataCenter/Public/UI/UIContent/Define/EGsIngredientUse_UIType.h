#pragma once

#include "EGsIngredientUse_UIType.generated.h"


UENUM(BlueprintType)
enum class EGsIngredientUse_UIType : uint8
{
	FairyUIType = 0,
	MonsterKnowledgeUIType,
	GuildCampUIType,
	ItemEnchantUIType,
	ItemMultiDecomposeUIType,
	DiscountCouponUIType,
	ChangeCostumeUIType UMETA(Tooltip = "Exchange Costume Ticket"),
	
	ContentsType_Max,
};