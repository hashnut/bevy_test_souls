
#pragma once

/**
* file		GsItemCollectionFilter.h
* @brief	Item Collection 에서 사용 하게 될 전용 필터
* @author	PKT
* @date		2022/12/30
* */

#include "CoreMinimal.h"

template<typename T>
class FGsItemCollectionFilter
{
	/************************************************************************/
	/* using                                                                */
	/************************************************************************/
public:
	using DelegateFunc = TFunction<bool(const T*)>;

	DelegateFunc Condition = nullptr;

public:
	bool Execute(const T* InData)
	{
		if (InData && nullptr == Condition || true == Condition(InData) )
		{	
			return true;
		}

		return false;
	}
};
