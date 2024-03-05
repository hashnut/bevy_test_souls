#include "GsSortCost.h"

#include "DataSchema/Item/GsSchemaItemCommon.h"
#include "DataSchema/Item/GsSchemaItemExpiredTime.h"
#include "GsCostPackage.h"
#include "Item/GsItem.h"
#include "Management/GsScopeGame.h"
#include "Management/GsScopeHolder.h"
#include "Management/ScopeGame/GsValidatorChainManager.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "T1Project.h"

FGsComparerItemIdDisposalEfficiency::FGsComparerItemIdDisposalEfficiency(ECachedValueItemDelegateType InCachedValueItemDelegateType, bool InTradableFirst)
	: _cachedValueItemDelegateType(InCachedValueItemDelegateType)
	, _tradableFirst(InTradableFirst)
{
}

FGsComparerItemIdDisposalEfficiency::FGsComparerItemIdDisposalEfficiency(ECachedValueItemDelegateType InCachedValueItemDelegateType)
	: FGsComparerItemIdDisposalEfficiency(InCachedValueItemDelegateType, false)
{
}

bool FGsComparerItemIdDisposalEfficiency::operator()(const TSharedRef<FGsCostElement::FGsCostElementBase>& InItemA, const TSharedRef<FGsCostElement::FGsCostElementBase>& InItemB) const
{
	return (*this)(StaticCastSharedRef<FGsCostElement::FGsCostElementItemBase>(InItemA)->GetItemId(), StaticCastSharedRef<FGsCostElement::FGsCostElementItemBase>(InItemB)->GetItemId());
}

bool FGsComparerItemIdDisposalEfficiency::operator()(const TWeakPtr<FGsItem>& InItemA, const TWeakPtr<FGsItem>& InItemB) const
{
	const bool IsValidA = InItemA.IsValid();
	const bool IsValidB = InItemB.IsValid();
	if (!IsValidA || !IsValidB)
	{
		GSLOG(Warning, TEXT("Invalid parameters. ItemA: %d, ItemB: %d"), IsValidA, IsValidB);
		return IsValidA ? false : true;
	}

	return (*this)(InItemA.Pin()->GetTID(), InItemB.Pin()->GetTID());
}

bool FGsComparerItemIdDisposalEfficiency::operator()(const ItemId& InItemIdA, const ItemId& InItemIdB) const
{
	// 유효한 아이템을 앞으로
	const FGsSchemaItemCommon* ItemCommonA = FGsItem::GetTableDataByID(InItemIdA);
	const FGsSchemaItemCommon* ItemCommonB = FGsItem::GetTableDataByID(InItemIdB);

	const bool IsValidA = nullptr != ItemCommonA;
	const bool IsValidB = nullptr != ItemCommonB;
	if (!IsValidA || !IsValidB)
	{
		GSLOG(Warning, TEXT("Invalid parameters. ItemA: %d, ItemB: %d"), IsValidA, IsValidB);
		return IsValidA ? false : true;
	}

	{
		// 보유중인 아이템을 앞으로
		FGsValidatorChainManager* ValidatorChainManager = GSValidatorChain();
		if (nullptr == ValidatorChainManager)
		{
			GSLOG(Error, TEXT("nullptr == ValidatorChainManager"));
			return 0;
		}

		const bool HasItemA = 0 < ValidatorChainManager->GetValue(_cachedValueItemDelegateType, InItemIdA);
		const bool HasItemB = 0 < ValidatorChainManager->GetValue(_cachedValueItemDelegateType, InItemIdB);
		if (HasItemA != HasItemB)
		{
			return HasItemA;
		}
	}

	const UGsTableItemExpiredTime* ItemExpiredTimeTable = Cast<UGsTableItemExpiredTime>(FGsSchemaItemExpiredTime::GetStaticTable());
	if (nullptr == ItemExpiredTimeTable)
	{
		return false;
	}

	const FGsSchemaItemExpiredTime* ItemExpiredTimeA = nullptr;
	const FGsSchemaItemExpiredTime* ItemExpiredTimeB = nullptr;
	FDateTime ExpiredTimeA = FDateTime::MinValue();
	FDateTime ExpiredTimeB = FDateTime::MinValue();
	const bool HasRemainTimeA = (0 != ItemCommonA->expiredTimeId) && ItemExpiredTimeTable->FindRowById(ItemCommonA->expiredTimeId, ItemExpiredTimeA) && !ItemExpiredTimeA->endTime.IsEmpty() && FDateTime::Parse(ItemExpiredTimeA->endTime, ExpiredTimeA);
	const bool HasRemainTimeB = (0 != ItemCommonB->expiredTimeId) && ItemExpiredTimeTable->FindRowById(ItemCommonB->expiredTimeId, ItemExpiredTimeB) && !ItemExpiredTimeB->endTime.IsEmpty() && FDateTime::Parse(ItemExpiredTimeB->endTime, ExpiredTimeB);

	// 유효기간 있는 아이템을 앞으로
	if (HasRemainTimeA != HasRemainTimeB)
	{
		return HasRemainTimeA;
	}

	// 유효기간 짧은 아이템을 앞으로
	if (ExpiredTimeA != ExpiredTimeB)
	{
		return ExpiredTimeA < ExpiredTimeB;
	}

	const bool TradableA = ItemCommonA->tradable;
	const bool TradableB = ItemCommonB->tradable;
	if (TradableA != TradableB)
	{
		// [U1] | ejrrb10 | 제작 재료 대체 아이템 정렬 시에는 거래 가능 아이템을 먼저 보여주기 (CHR-21150)
		if (_tradableFirst)
		{
			return TradableA;
		}
		// 일반적인 경우에는 거래 불가 아이템을 앞으로
		else
		{
			return !TradableA;
		}
	}

	// 아이템 아이디 작은 아이템을 앞으로
	return InItemIdA < InItemIdB;
}

FGsComparerItemIdDisposalEfficiencyTradableFirst::FGsComparerItemIdDisposalEfficiencyTradableFirst(ECachedValueItemDelegateType InCachedValueItemDelegateType)
	: Super(InCachedValueItemDelegateType, true)
{
}

FGsComparerItemIdLevelForDisposalEfficiency::FGsComparerItemIdLevelForDisposalEfficiency(ECachedValueItemDelegateType InCachedValueItemDelegateType)
	: _cachedValueItemDelegateType(InCachedValueItemDelegateType)
{
}

bool FGsComparerItemIdLevelForDisposalEfficiency::operator()(const TSharedRef<FGsCostElement::FGsCostElementBase>& InItemA, const TSharedRef<FGsCostElement::FGsCostElementBase>& InItemB) const
{
	const TSharedRef<FGsCostElement::FGsCostElementItemLevel> ItemA = StaticCastSharedRef<FGsCostElement::FGsCostElementItemLevel>(InItemA);
	const TSharedRef<FGsCostElement::FGsCostElementItemLevel> ItemB = StaticCastSharedRef<FGsCostElement::FGsCostElementItemLevel>(InItemB);

	ItemId InItemIdA = ItemA->GetItemId();
	ItemId InItemIdB = ItemB->GetItemId();

	Level itemLevelA = ItemA->GetItemLevel();
	Level itemLevelB = ItemB->GetItemLevel();

	// 유효한 아이템을 앞으로
	const FGsSchemaItemCommon* ItemCommonA = FGsItem::GetTableDataByID(InItemIdA);
	const FGsSchemaItemCommon* ItemCommonB = FGsItem::GetTableDataByID(InItemIdB);

	const bool IsValidA = nullptr != ItemCommonA;
	const bool IsValidB = nullptr != ItemCommonB;
	if (!IsValidA || !IsValidB)
	{
		GSLOG(Warning, TEXT("Invalid parameters. ItemA: %d, ItemB: %d"), IsValidA, IsValidB);
		return IsValidA ? false : true;
	}

	{
		// 보유중인 아이템을 앞으로
		FGsValidatorChainManager* ValidatorChainManager = GSValidatorChain();
		if (nullptr == ValidatorChainManager)
		{
			GSLOG(Error, TEXT("nullptr == ValidatorChainManager"));
			return 0;
		}

		const bool HasItemA = 0 < ValidatorChainManager->GetValue(_cachedValueItemDelegateType, InItemIdA, itemLevelA);
		const bool HasItemB = 0 < ValidatorChainManager->GetValue(_cachedValueItemDelegateType, InItemIdB, itemLevelB);
		if (HasItemA != HasItemB)
		{
			return HasItemA;
		}
	}

	const UGsTableItemExpiredTime* ItemExpiredTimeTable = Cast<UGsTableItemExpiredTime>(FGsSchemaItemExpiredTime::GetStaticTable());
	if (nullptr == ItemExpiredTimeTable)
	{
		return false;
	}

	const FGsSchemaItemExpiredTime* ItemExpiredTimeA = nullptr;
	const FGsSchemaItemExpiredTime* ItemExpiredTimeB = nullptr;
	FDateTime ExpiredTimeA = FDateTime::MinValue();
	FDateTime ExpiredTimeB = FDateTime::MinValue();
	const bool HasRemainTimeA = (0 != ItemCommonA->expiredTimeId) && ItemExpiredTimeTable->FindRowById(ItemCommonA->expiredTimeId, ItemExpiredTimeA) && !ItemExpiredTimeA->endTime.IsEmpty() && FDateTime::Parse(ItemExpiredTimeA->endTime, ExpiredTimeA);
	const bool HasRemainTimeB = (0 != ItemCommonB->expiredTimeId) && ItemExpiredTimeTable->FindRowById(ItemCommonB->expiredTimeId, ItemExpiredTimeB) && !ItemExpiredTimeB->endTime.IsEmpty() && FDateTime::Parse(ItemExpiredTimeB->endTime, ExpiredTimeB);

	// 유효기간 있는 아이템을 앞으로
	if (HasRemainTimeA != HasRemainTimeB)
	{
		return HasRemainTimeA;
	}

	// 유효기간 짧은 아이템을 앞으로
	if (ExpiredTimeA != ExpiredTimeB)
	{
		return ExpiredTimeA < ExpiredTimeB;
	}

	const bool TradableA = ItemCommonA->tradable;
	const bool TradableB = ItemCommonB->tradable;
	if (TradableA != TradableB)
	{
		return !TradableA;
	}

	// 아이템 아이디 작은 아이템을 앞으로
	return InItemIdA < InItemIdB;
}
