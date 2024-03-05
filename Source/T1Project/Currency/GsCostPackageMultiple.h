#pragma once

#include "GsCostPackage.h"
#include "Management/ScopeGame/GsValidatorChainManager.h"
#include "Shared/Client/SharedEnums/SharedCommonEnum.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"

class FGsCostElement;

/**
 * FGsCostPackage 는 단일 비용이고, 이 클래스를 통해 복수 비용을 처리함.
*/
class FGsCostPackageMultiple final
{

private:
	TSharedRef<const FGsCostPackage> _costPackage;

	int32 _multiple = 1;

	/************************************************************************/
	/* 인스턴싱																*/
	/************************************************************************/

public:
	// TODO: 풀러에서 관리하도록 개선 필요
	FGsCostPackageMultiple(TSharedRef<const FGsCostPackage> InCostPackage, const int32 InMultiple);

	FGsCostPackageMultiple(FGsCostPackageMultiple&& Rhs);

	FGsCostPackageMultiple(const FGsCostPackageMultiple& Rhs);

private:
	void CheckValidation() const;

	/************************************************************************/
	/* setters																*/
	/************************************************************************/

public:
	void SetMultiple(const int32 InMultiple);

	/************************************************************************/
	/* getters																*/
	/************************************************************************/

public:
	int32 GetMultiple() const;

	ECachedValueItemDelegateType GetCachedValueItemDelegateType() const;

	/**
	 * 현재 유저 소유 재화로 가능한 최대 갯수. 잠김, 장착 등 즉시 처분 불가능한 아이템은 제외함.
	 */
	int32 GetPlayerCraftMaxCount() const;

	/************************************************************************/
	/* single getters                                                       */
	/************************************************************************/

	CurrencyDelta GetAmountSingle(const CostType InCostType, const CurrencyType InCurrencyType, const ItemId InItemId) const;

	CurrencyDelta GetAmountSingle(const ItemId InItemId) const;

	CurrencyDelta GetAmountSingle(const CurrencyType InCurrencyType) const;

	CurrencyDelta GetFirstAmountSingle(const ItemId InItemId) const;

	CurrencyDelta GetFirstAmountSingle(const CurrencyType InCurrencyType) const;

	FString GetFirstAmountStringSingle(const CurrencyType InCurrencyType) const;

	/************************************************************************/
	/* multiple getters                                                     */
	/************************************************************************/

	CurrencyDelta GetAmountMultiple(const CostType InCostType, const CurrencyType InCurrencyType, const ItemId InItemId) const;

	CurrencyDelta GetAmountMultiple(const ItemId InItemId) const;

	CurrencyDelta GetAmountMultiple(const CurrencyType InCurrencyType) const;

	CurrencyDelta GetFirstAmountMultiple(const ItemId InItemId) const;

	CurrencyDelta GetFirstAmountMultiple(const CurrencyType InCurrencyType) const;

	/************************************************************************/
	/* cost elements														*/
	/************************************************************************/

public:
	const FGsCostElement* GetSingleCostElementAt(const int32 InIndex) const;

	int32 GetCostElementsCount() const;

	/************************************************************************/
	/* validator															*/
	/************************************************************************/

public:
	/**
	 * 모든 비용이 충분한지 여부
	 */
	bool PlayerHasEnoughMultiple() const;

	const FGsCostElement* GetFirstCostElementNotEnough(const CostType InCostType = CostType::MAX) const;

};