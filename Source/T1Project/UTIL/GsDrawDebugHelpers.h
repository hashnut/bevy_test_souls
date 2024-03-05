#pragma once

#include "DrawDebugHelpers.h"

class FGsDrawDebugHelpers
{
public:
	static void DrawDebugFence(UWorld* inWorld, const TArray<FVector>& inFenceList, float inHeight = 1000);

	static void DrawDebugArrow(const FVector2D& In_touchPos);
};