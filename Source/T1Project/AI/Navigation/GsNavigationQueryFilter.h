#pragma once

#include "CoreMinimal.h"
#include "Runtime/NavigationSystem/Public/NavFilters/NavigationQueryFilter.h"
#include "GsNavigationQueryFilter.generated.h"

// https://forums.unrealengine.com/t/tip-how-to-increase-pathfinding-distance/484658

class ANavigationData;

UCLASS()
class UGsNavigationQueryFilter : public UNavigationQueryFilter
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = Filter)
	int maxSearchNodes = 8192;

protected:
	virtual void InitializeFilter(const ANavigationData& NavData, const UObject* Querier, FNavigationQueryFilter& Filter) const override;
};