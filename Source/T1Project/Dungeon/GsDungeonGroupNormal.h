
#pragma once

/**
* file		GsNormalDungeonGroup.h
* @brief	Dungeon Normal Class
* @author	PKT
* @date		2022/07/19
**/

#include "CoreMinimal.h"
#include "GsDungeonGroupBase.h"
#include "Currency/GsCostPackage.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"


class FGsDungeonGroupNormal : public FGsDungeonGroupBase
{
protected:
	// 2022/08/16 PKT - 시간 충전 용 아이템
	TSharedRef<const FGsCostPackage> _costChargeItem;

	// 2022/07/19 PKT - 기본 시간
	int32 _normalTime = 0;
	// 2022/07/19 PKT - 충전 시간
	int32 _chargeTime = 0;

public:
	explicit FGsDungeonGroupNormal(const struct FGsSchemaDungeonGroupData* InSchemaData);

	virtual void Initialized();
	virtual void Finalized();

protected:
	// 2022/08/18 PKT - Time Charge Item
	virtual void AddItem(struct FGsItemMessageParamBase& InParam) override;
	virtual void UpdateItem(struct FGsItemMessageParamBase& InParam) override;
	virtual void RemoveItem(struct FGsItemMessageParamBase& InParam) override;

	virtual bool IsDungeonUnLockContentsDelegate(const FGsDungeonData* InTarget, bool InShowTicker = false ) override;

	void CheckedCostItem(const ItemId InItemId);

public:
	void SetTime(int32 InNormalTime, int32 InChargeTime);	

	ItemAmount CostAmount() const;

	int32 GetNormalTime() const;

	int32 GetChargeTime() const;

	const FDateTime GetTime() const;

	bool IsTimeLimit() const;

	bool IsTimeOver() const;
};