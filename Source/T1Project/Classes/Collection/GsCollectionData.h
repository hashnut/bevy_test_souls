// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "Shared/Client/SharedEnums/SharedPassivityEnum.h"

/**
 * 콜렉션 데이터는 이 인터페이스를 상속
 */
class T1PROJECT_API IGsCollectionData
{
public:
	virtual PassivityOwnerType GetPassivityOwnerType() const = 0;
	virtual int32 GetPassivityCount() const = 0;
	virtual PassivityId GetPassivityId(int32 InIndex) const = 0;
};
