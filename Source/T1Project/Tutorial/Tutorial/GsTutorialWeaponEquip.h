// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Tutorial/Tutorial/GsTutorialBase.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"

/**
 * 무기 장착. 직전에 유저가 선택해서 받은 아이템에 맞춰 진행
 */
class T1PROJECT_API FGsTutorialWeaponEquip final : public FGsTutorialBase
{
public:
	using Super = FGsTutorialBase;

protected:
	TArray<ItemId> _targetItemList;
	ItemId _itemId = INVALID_ITEM_ID;

public:
	FGsTutorialWeaponEquip() : Super()
	{
	}
	virtual ~FGsTutorialWeaponEquip() {};

public:
	virtual bool OnPrevStart() override;

protected:
	virtual bool StartActionInternal() override;

private:
	bool SetTargetItemList();
};
