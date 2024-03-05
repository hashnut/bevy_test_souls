#pragma once

#include "EBMShopProductTagType.generated.h"

UENUM(BlueprintType)
enum class EBMShopProductTagType : uint8
{
	Tag_None = 0,
	Tag_New,
	Tag_Event,
	Tag_Hot,
	Tag_GuildGift,
};