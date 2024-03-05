// Fill out your copyright notice in the Description page of Project Settings.
#pragma once


#include "CoreMinimal.h"
#include "Classes/Container/GsAccessibleQueue.h"


struct FGsWidgetQueueData
{
	FName _key; // UIPath 키
	int32 _id; // FGsUIQueue::IsExist 에서 활용. 같은 목적의 UI인지 구분하기 위함.

	TFunction<void(class UGsUIWidgetBase*)> InitFunc;

	FGsWidgetQueueData() = default;

	explicit FGsWidgetQueueData(const FName& InKey, int32 InId, TFunction<void(class UGsUIWidgetBase*)> InInitFunc)
		: _key(InKey), _id(InId), InitFunc(InInitFunc)
	{
	}
};

class FGsWidgetQueue : public TAccessibleQueue<FGsWidgetQueueData>
{
public:
	bool IsExist(const FName& InKey, int32 InId) const
	{
		TNode* next = Tail->NextNode;
		while (nullptr != next)
		{
			if (next->Item._key == InKey)
			{
				if (next->Item._id == InId)
				{
					return true;
				}
			}

			next = next->NextNode;
		}

		return false;
	}
};
