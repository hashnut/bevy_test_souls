#pragma once

/**
* file		GsItemCollectionFilterBase.h
* @brief	Item Collection 에서 사용 하게 될 전용 필터
* @author	PKT
* @date		2021/07/15
* */
#include "CoreMinimal.h"

/**
 * 2021/09/30 PKT - Item Collection 상속 구조로 변경 됨에 따라 Template 로 바꿈
 */
template<typename T>
class FGsItemCollectionFilterBase
{
	/************************************************************************/
	/* using                                                                */
	/************************************************************************/
public:
	using DelegateFunc = TFunction<bool(const T*)>;

	DelegateFunc Condition = nullptr;

public:
	void Execute(const TArray<const T*>& InList, TArray<const T*>& outList)
	{
		for (const T* itemCollection : InList)
		{
			if (nullptr == Condition || true == Condition(itemCollection))
			{
				outList.Emplace(itemCollection);
			}
		}
	}
};
