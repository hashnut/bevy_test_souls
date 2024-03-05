// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFlow/GameContents/ContentsGame/NpcShop/GsNpcShopBase.h"


struct FGsSchemaNpcData;
/**
 * 장비 상점 로직 수행 Class
 */
class T1PROJECT_API FGsEquipShopHandler : public FGsNpcShopBase
{
	using Super = FGsNpcShopBase;

public:
	FGsEquipShopHandler() = default;
	virtual ~FGsEquipShopHandler() = default;
	
public:
	bool TryBuyItem() override;
	bool TrySellItem() override;
};