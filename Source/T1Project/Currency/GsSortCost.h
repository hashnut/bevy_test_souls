#pragma once

#include "GsCostPackage.h"
#include "Item/GsItem.h"
#include "Management/ScopeGame/GsValidatorChainManager.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"

/************************************************************************/
/* 비용 정렬 모음                                                       */
/************************************************************************/

/**
 * 아이템ID로 처분 효율 순서로 정렬
 */
struct T1PROJECT_API FGsComparerItemIdDisposalEfficiency
{
private:
	friend struct TDereferenceWrapper<TSharedRef<FGsCostElement::FGsCostElementBase>, FGsComparerItemIdDisposalEfficiency>;
	friend struct TDereferenceWrapper<TWeakPtr<FGsItem>, FGsComparerItemIdDisposalEfficiency>;
	friend struct TDereferenceWrapper<ItemId, FGsComparerItemIdDisposalEfficiency>;

private:
	const ECachedValueItemDelegateType _cachedValueItemDelegateType;

	/** 거래 가능한 아이템을 상위 우선 순위로 설정함.
	 *  다른 모든 정렬 규칙은 그대로하고, 거래 가/불가 조건만 반대로 적용할 때 사용함.
	 *  제작 재료인 경우, 정렬 규칙 추가 용도로 사용.
	 */
	const bool _tradableFirst;

protected:
	explicit FGsComparerItemIdDisposalEfficiency(ECachedValueItemDelegateType InCachedValueItemDelegateType, bool InTradableFirst);

public:
	explicit FGsComparerItemIdDisposalEfficiency(ECachedValueItemDelegateType InCachedValueItemDelegateType);

protected:
	bool operator()(const TSharedRef<FGsCostElement::FGsCostElementBase>& InItemA, const TSharedRef<FGsCostElement::FGsCostElementBase>& InItemB) const;
	bool operator()(const TWeakPtr<FGsItem>& InItemA, const TWeakPtr<FGsItem>& InItemB) const;
	bool operator()(const ItemId& InItemIdA, const ItemId& InItemIdB) const;
};

/**
 * ID로 처분 효율 순서로 정렬
 * (모든 조건이 FGsComparerItemIdDisposalEfficiency 와 동일하지만, 거래 가능한 아이템을 우선순위 높임.)
 */
struct T1PROJECT_API FGsComparerItemIdDisposalEfficiencyTradableFirst final : public FGsComparerItemIdDisposalEfficiency
{
private:
	using Super = FGsComparerItemIdDisposalEfficiency;
	friend struct TDereferenceWrapper<TSharedRef<FGsCostElement::FGsCostElementBase>, FGsComparerItemIdDisposalEfficiencyTradableFirst>;
	friend struct TDereferenceWrapper<TWeakPtr<FGsItem>, FGsComparerItemIdDisposalEfficiencyTradableFirst>;
	friend struct TDereferenceWrapper<ItemId, FGsComparerItemIdDisposalEfficiencyTradableFirst>;

public:
	explicit FGsComparerItemIdDisposalEfficiencyTradableFirst(ECachedValueItemDelegateType InCachedValueItemDelegateType);
};

/**
 * 아이템ID 와 아이템 레벨로 처분 효율 순서로 정렬
 */
struct T1PROJECT_API FGsComparerItemIdLevelForDisposalEfficiency final
{
private:
	friend struct TDereferenceWrapper<TSharedRef<FGsCostElement::FGsCostElementBase>, FGsComparerItemIdLevelForDisposalEfficiency>;

private:
	const ECachedValueItemDelegateType _cachedValueItemDelegateType;

public:
	explicit FGsComparerItemIdLevelForDisposalEfficiency(ECachedValueItemDelegateType InCachedValueItemDelegateType);

private:
	bool operator()(const TSharedRef<FGsCostElement::FGsCostElementBase>& InItemA, const TSharedRef<FGsCostElement::FGsCostElementBase>& InItemB) const;
};
