
#pragma once

/**
* file		GsDungeonGroupParty.h
* @brief	Dungeon Party Class
* @author	PKT
* @date		2022/07/19
**/

#include "CoreMinimal.h"
#include "IDelegateInstance.h"
#include "GsDungeonGroupBase.h"
#include "../Currency/GsCostPackage.h"
#include "../Message/GsMessageItem.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "Shared/Client/SharedEnums/SharedPartyDungeonEnum.h"


class FGsDungeonGroupParty : public FGsDungeonGroupBase
{
protected:	
	// 2022/05/23 PKT - 마지막에 클리어한 난이도
	DungeonDifficulty _lastClearDifficulty = DungeonDifficulty::NONE;

	// 2022/05/23 PKT - 보상 혜택의 권한이 주어지는 Item
	TSharedRef<const FGsCostPackage> _costDungeonKey;

	int32 _defaultRewardCount = 0;

public:
	explicit FGsDungeonGroupParty(const struct FGsSchemaDungeonGroupData* InSchemaData, int32 InDefaultRewardCount);

protected:
	virtual void AddItem(struct FGsItemMessageParamBase& InParam) override;
	virtual void UpdateItem(struct FGsItemMessageParamBase& InParam) override;
	virtual void RemoveItem(struct FGsItemMessageParamBase& InParam) override;
	
	virtual bool IsDungeonUnLockContentsDelegate(const FGsDungeonData* InTarget, bool InShowTicker = false) override;

	void CheckedCostItem(const ItemId InItemId);

public:	
	void SetClearDifficulty(DungeonDifficulty InType);
	void SetHaveWeekRewardCount(int32 InCount);

	// 2022/06/13 PKT - 입장 가능한 난이도
	DungeonDifficulty DifficultyToEnter() const;
	DungeonDifficulty CurrentDifficulty() const;

	// 2022/05/25 PKT - 던전 보상 받을 수 있는 아이템 갯수
	CurrencyDelta GetDungeonKeyAmount() const;

	// 2022/08/16 PKT - 기본 주간 보상을 받을 수 있는 현재 갯수
	int32 GetDefaultHaveRewardCount() const;

	/************************************************************************/
	/* Find                                                                 */
	/************************************************************************/
	const class FGsDungeonData* FindDungeonByDifficulty(DungeonDifficulty InType) const;
};

