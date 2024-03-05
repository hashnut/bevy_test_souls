#pragma once

/**
* file		GsTreeViewElementData.h
* @brief	GsTreeView Data - 'UGsUITreeView'와 함께 동작.
* @author	PKT
* @date		2021/11/23
**/

#include "CoreMinimal.h"

struct T1PROJECT_API FGsTreeViewElementData
{
	FText name;
	const void* data = nullptr;
};

struct T1PROJECT_API FGsTreeViewData
{
	FGsTreeViewElementData baseData;
	TArray<FGsTreeViewElementData> childElements;
};