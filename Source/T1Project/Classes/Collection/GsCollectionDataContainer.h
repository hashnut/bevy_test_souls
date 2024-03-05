// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 콜렉션 데이터를 가진 곳에서 상속
 */
class T1PROJECT_API IGsCollectionDataContainer
{
public:
	virtual const class IGsCollectionData* GetCollectionData() const = 0;
};
