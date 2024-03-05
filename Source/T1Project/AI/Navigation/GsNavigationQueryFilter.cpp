#include "GsNavigationQueryFilter.h"

#include "Runtime/NavigationSystem/Public/NavigationData.h"
#include "Runtime/Engine/Public/AI/Navigation/NavQueryFilter.h"

void UGsNavigationQueryFilter::InitializeFilter(const ANavigationData& NavData, const UObject* Querier, FNavigationQueryFilter& Filter) const
{
	Super::InitializeFilter(NavData, Querier, Filter);
	Filter.SetMaxSearchNodes(maxSearchNodes);
}