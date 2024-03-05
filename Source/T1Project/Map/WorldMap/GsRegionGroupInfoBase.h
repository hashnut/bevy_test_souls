#pragma once
#include "CoreMinimal.h"
/*
* region map group info base class
*/

class FGsRegionGroupInfoBase
{
	// member
public: 
	// group id
	int _groupId;
	// region ids
	TArray<int> _arrayRegionId;

	// destruct
public:
	virtual ~FGsRegionGroupInfoBase() = default;

	// virtual function
public:

	virtual void Finalize()
	{
		if (_arrayRegionId.Num() != 0)
		{
			_arrayRegionId.Empty();
		}
	}
};