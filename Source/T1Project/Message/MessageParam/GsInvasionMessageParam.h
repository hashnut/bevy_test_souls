// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameObject/Define/GsGameObjectDefine.h"
#include "Shared/Client/SharedEnums/SharedCommonEnum.h"

class FGsItem;

/**
 * Inventory Message 사용될 구조체 정의
 */

 using FGsInvasionMessageParamBase = IGsMessageParam;

struct FGsInvasionChaosParam : public FGsInvasionMessageParamBase
{
public:
	bool _isChaos;

public:
	FGsInvasionChaosParam() = default;
	virtual ~FGsInvasionChaosParam() = default;	
	explicit FGsInvasionChaosParam(bool inChaos)
		: _isChaos(inChaos)
	{
	}
};
