#pragma once

#include "CoreMinimal.h"

// continental space crack sort info
class FGsContinentalSpaceCrackMapInfo
{
public:
	bool _isDecoLine = false;
	bool _isSpawned = false;
	int _mapId = 0;
	FText _mapName;
	int _minTimeMinute;
	int _maxTimeMinute;
};