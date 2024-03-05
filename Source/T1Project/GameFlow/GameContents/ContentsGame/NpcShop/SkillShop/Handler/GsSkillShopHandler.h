// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFlow/GameContents/ContentsGame/NpcShop/GsNpcShopBase.h"


struct FGsSchemaNpcData;
/**
 * 스킬북 상점 로직 수행 Class
 */
class T1PROJECT_API FGsSkillShopHandler : public FGsNpcShopBase
{
	using Super = FGsNpcShopBase;

public:
	FGsSkillShopHandler() = default;
	virtual ~FGsSkillShopHandler() = default;

public:
	bool TryBuyItem() override;
	bool TrySellItem() override;
};