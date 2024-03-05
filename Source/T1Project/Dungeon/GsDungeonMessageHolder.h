#pragma once

#include "CoreMinimal.h"


class T1PROJECT_API FGsDungeonMessageHolder
{
private:	
	const static int32 HOLD_INDEX = 1;
	uint32* _holder = nullptr;

public:
	FGsDungeonMessageHolder(uint32& InHolder) : _holder(&InHolder)
	{
		if (_holder) (*_holder) += HOLD_INDEX;
	}
	~FGsDungeonMessageHolder()
	{
		if (_holder && 0 < (*_holder))
		{
			(*_holder) -= HOLD_INDEX;
		}

		_holder -= HOLD_INDEX;
	}

	bool IsHolder() const
	{
		if (_holder)
		{
			return (0 >= (*_holder));
		}

		return false;
	}
};