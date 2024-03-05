#pragma once

/**
* file		FGsTitleTextHelper.h
* @brief	Title Text Helper Class
* @author	PKT
* @date		2022/01/21
**/

#include "CoreMinimal.h"


struct T1PROJECT_API FGsTitleTextHelper
{
	static bool FindText(const FTextKey& InKey, OUT FText& OutText);
};