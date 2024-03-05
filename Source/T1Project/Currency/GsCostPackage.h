#pragma once

#include "Runtime/Core/Public/Containers/Array.h"
#include "Runtime/Core/Public/Math/NumericLimits.h"
#include "Runtime/Core/Public/Templates/Tuple.h"

#include "Item/GsItemManager.h"
#include "Management/ScopeGame/GsValidatorChainManager.h"
#include "Shared/Client/SharedEnums/SharedCommonEnum.h"
#include "Shared/Client/SharedEnums/SharedItemEnum.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "T1Project.h"

class FGsBaseCachedValue;
class FGsCostPackage;
class FGsCostPackageMultiple;
class FGsItem;
struct FGsCostOption;
struct ItemDBIdAmountPair;
struct FGsCostOption;

constexpr Currency INVALID_DISPLAY_COST_AMOUNT = TNumericLimits<Currency>::Min();

/**
 * 비용은 여러 요소로 구성될 수 있는데, 이 클래스는 비용의 구성요소를 나타냄.
 * 하나의 비용 요소는 일반적으로 단 하나의 비용을 가지지만,
 * 대체 비용이 있을 경우, 대체 비용에 대한 정보들도 같이 포함함.
 */
class FGsCostElement final
{
	// for AddAmount()
	friend class FGsCostPackage;

public:
	enum class EGsCostElementType
	{
		CostElementCurrency,
		CostElementItem,
		CostElementItemCategorySub
	};

	/**
	 * FGsCostElementBase
	 */
	struct FGsCostElementBase
	{
		/**
		 * 대체아이템의 순서를 결정할 수 있는 우선순위. 낮은 숫자가 먼저 선택됨.
		 * 기본값은 최대값이고, 정렬 수행 시, stable sort 를 수행하면 원본 정렬 상태를 유지할 수 있음.
		 */
		mutable uint8 _priority = TNumericLimits<uint8>::Max();

	protected:
		FGsCostElementBase() = default;

		explicit FGsCostElementBase(const FGsCostElementBase& Rhs)
			: _priority(Rhs._priority)
		{
		}

	public:
		virtual EGsCostElementType GetCostElementType() const = 0;

		virtual Currency PlayerHasAmount() const = 0;
	};

	/**
	 * FGsCostElementCurrency
	 */
	struct FGsCostElementCurrency final : public FGsCostElementBase
	{
	private:
		using Super = FGsCostElementBase;

	public:
		const CurrencyType _costCurrencyType;

		explicit FGsCostElementCurrency(const FGsCostElementCurrency& Rhs)
			: Super(Rhs)
			, _costCurrencyType(Rhs._costCurrencyType)
		{
		}

		explicit FGsCostElementCurrency(const CurrencyType InCostCurrencyType)
			: _costCurrencyType(InCostCurrencyType)
		{
		}

		EGsCostElementType GetCostElementType() const final;

		Currency PlayerHasAmount() const final;
	};

	/**
	 * FGsCostElementItemBase
	 */
	struct FGsCostElementItemBase : public FGsCostElementBase
	{
	private:
		using Super = FGsCostElementBase;

	public:
		virtual void AddAllPassedItems(OUT TSet<TWeakPtr<FGsItem>>& OutItems) const = 0;

		virtual bool IsPassedItem(const FGsItem& InItem) const = 0;

		virtual ItemId GetItemId() const;

		virtual bool HasItemLevel() const;

		virtual Level GetItemLevel() const;
	};

	/**
	 * FGsCostElementItem
	 */
	struct FGsCostElementItem : public FGsCostElementItemBase
	{
	private:
		using Super = FGsCostElementItemBase;

	public:
		const ECachedValueItemDelegateType _cachedValueItemDelegateType;

		const ItemId _costItemId;

		explicit FGsCostElementItem(const FGsCostElementItem& Rhs)
			: Super(Rhs)
			, _cachedValueItemDelegateType(Rhs._cachedValueItemDelegateType)
			, _costItemId(Rhs._costItemId)
		{
		}

		FGsCostElementItem(const ECachedValueItemDelegateType InCachedValueItemDelegateType, const ItemId InItemId)
			: _cachedValueItemDelegateType(InCachedValueItemDelegateType)
			, _costItemId(InItemId)
		{
		}

		EGsCostElementType GetCostElementType() const final;

		Currency PlayerHasAmount() const override;

		void AddAllPassedItems(OUT TSet<TWeakPtr<FGsItem>>& OutItems) const override;

		bool IsPassedItem(const FGsItem& InItem) const override;

		ItemId GetItemId() const override;
	};

	/**
	 * FGsCostElementItemLevel
	 */
	struct FGsCostElementItemLevel final : public FGsCostElementItem
	{
	private:
		using Super = FGsCostElementItem;

	public:
		const Level _costItemLevel;

		explicit FGsCostElementItemLevel(const FGsCostElementItemLevel& Rhs)
			: Super(Rhs)
			, _costItemLevel(Rhs._costItemLevel)
		{
		}

		FGsCostElementItemLevel(const ECachedValueItemDelegateType InCachedValueItemDelegateType, const ItemId InItemId, const Level InItemLevel)
			: Super(InCachedValueItemDelegateType, InItemId)
			, _costItemLevel(InItemLevel)
		{
		}

		Currency PlayerHasAmount() const final;

		void AddAllPassedItems(OUT TSet<TWeakPtr<FGsItem>>& OutItems) const final;

		bool IsPassedItem(const FGsItem& InItem) const final;

		bool HasItemLevel() const final;

		Level GetItemLevel() const final;
	};

	/**
	 * FGsCostElementItemCategorySub
	 */
	struct FGsCostElementItemCategorySub final : public FGsCostElementItemBase
	{
	private:
		using Super = FGsCostElementItemBase;

	public:
		const ItemCategorySub _costItemCategorySub;

		explicit FGsCostElementItemCategorySub(const FGsCostElementItemCategorySub& Rhs)
			: Super(Rhs)
			, _costItemCategorySub(Rhs._costItemCategorySub)
		{
		}

		FGsCostElementItemCategorySub(const ItemCategorySub InItemCategorySub)
			: _costItemCategorySub(InItemCategorySub)
		{
		}

		EGsCostElementType GetCostElementType() const final;

		Currency PlayerHasAmount() const final;

		void AddAllPassedItems(OUT TSet<TWeakPtr<FGsItem>>& OutItems) const final;

		bool IsPassedItem(const FGsItem& InItem) const final;
	};

private:
	/**
	 * 다음 CurrencyType 은 PlayerHasEnough() 함수에서 항상 true 를 반환함.
	 * 단, PlayerHasAmount() 는 실제 값(예를 들면 0)을 그대로 반환함.
	 */
	static const TSet<CurrencyType> ALWAYS_ENOUGH_CURRENCY_TYPES;

public:
	const ECachedValueItemDelegateType _cachedValueItemDelegateType = ECachedValueItemDelegateType::DEFAULT;

	/** CostPackage::_jointCostCandidateElements 참고 */
	const bool _jointCostCandidateElements;

	const CostType _costType;

	// FText??
	const FString _costAmountString;

private:
	Currency _costAmount;

	Currency _displayCostAmount = INVALID_DISPLAY_COST_AMOUNT;

	/** 비용 요소는 보통 하나이지만, 대체 비용 처리를 위해 배열로 실제 비용 요소를 관리함. */
	TArray<TSharedRef<FGsCostElementBase>> _candidates;

	TOptional<TArray<TWeakPtr<FGsCostOption>>> _linkedCostOptions;

public:
	FGsCostElement(const FGsCostPackage& InCostPackage, const bool /*InIncludeItemGroup*/, const Currency InCostAmount, const Currency InDisplayCostAmount, const CurrencyType InCurrencyType);

	FGsCostElement(const FGsCostPackage& InCostPackage, const bool /*InIncludeItemGroup*/, const FString& InCostAmountString, const CurrencyType InCurrencyType);

	FGsCostElement(const FGsCostPackage& InCostPackage, const bool InIncludeItemGroup, const Currency InCostAmount, const Currency InDisplayCostAmount, const ItemId InCostItemId);

	FGsCostElement(const FGsCostPackage& InCostPackage, const bool InIncludeItemGroup, const Currency InCostAmount, const Currency InDisplayCostAmount, const ItemId InCostItemId, const Level InCostItemLevel);

	FGsCostElement(const FGsCostPackage& InCostPackage, const bool /*InIncludeItemGroup*/, const Currency InCostAmount, const Currency InDisplayCostAmount, const ItemCategorySub InItemCategorySub);

	/**
	 * 대체 가능한 아이템들은 하나의 배열로 묶어서 비용을 같이 계산할 수 있도록 별도 처리함
	 */
	FGsCostElement(const FGsCostPackage& InCostPackage, const bool InIncludeItemGroup, const Currency InCostAmount, const Currency InDisplayCostAmount, const TArray<TTuple<ItemId, Level>>& InItemIdMinLevels);

	FGsCostElement(const FGsCostPackage& InCostPackage, const bool InIncludeItemGroup, const Currency InCostAmount, const Currency InDisplayCostAmount, const TArray<ItemId>& InItemIds);

	virtual ~FGsCostElement() {}

	FGsCostElement(FGsCostElement&& Rhs);

	FGsCostElement(const FGsCostElement& Rhs) = delete;

	FGsCostElement& operator =(const FGsCostElement& InRhs) = delete;

public:
	const FText& GetCostName() const;

	Currency GetCostAmount(const bool InApplyCostOption = true) const;

	Currency GetDisplayCostAmount(const bool InApplyCostOption = true) const;

	/**
	 * 현재 소유 비용의 부족한 갯수
	 * 현재 소유 비용이 더 적을 경우, 몇개가 부족한 지 양수로 반환함.
	 * 현재 소유 비용이 더 많을 경우, 무조건 0을 반환함.
	 */
	Currency GetCostLackAmount(const bool InApplyCostOption = true) const;

	CurrencyType GetCurrencyType() const;

	ItemId GetItemId() const;

	Level GetItemLevel() const;

	/*
	 * 플레이어가 처분 가능한 재화/아이템의 최대 갯수.
	 * (대체비용 때문에 '최대' 라는 의미가 필요함. 대체비용이 없이 단일 비용인 경우, 그냥 현재 비용의 보유 갯수를 의미함.)
	 * 아이템의 경우, 잠김중, 장착중, IN창고 등 특정 상태의 아이템을 갯수에서 제외됨.(참고: FGsItemIdAmountCachedValue::UpdateValue)
	 * _jointCostCandidateElements 가 true 인 경우, 모든 대체비용들의 보유 갯수의 총합을 반환.
	 * _jointCostCandidateElements 가 false 인 경우, 모든 대체비용 중 보유 갯수가 가장 큰 비용의 보유 갯수를 반환.
	 */
	Currency PlayerHasAmountMax() const;

	/**
	 * 플레이어가 본 재화/아이템을 충분히 가지고 있는지 여부를 반환.
	 * 매개변수는 한번의 비용으로 간주하기 위해 기본값이 1이지만, 배수로 계산이 필요한 경우 배수를 넣어줌.
	 * 아이템의 경우, 잠김중, 장착중, IN창고 등 특정 상태의 아이템을 갯수에서 제외됨.(참고: FGsItemIdAmountCachedValue::UpdateValue)
	 */
	bool PlayerHasEnough(const int32 InMultiple = 1) const;

	/**
	 * 현재 플레이어가 첫번째 대체비용만으로 이 비용을 전부 지불 가능하면 true 반환.
	 * 두 개 이상의 대체비용을 합쳐야만 지불 가능할 경우, false 반환.
	 */
	bool PlayerHasEnoughJustFirstCandidate(const int32 InMultiple = 1) const;

	bool IsValid() const;

	void LinkCostOption(TSharedPtr<FGsCostOption> InCostOption);

private:
	bool CheckValidation() const;

	void AddAmount(const Currency InCostAmount, const Currency InDisplayCostAmount);

	/**
	 * 대체 비용 관련 기능
	 */

private:
	TArray<TSharedRef<FGsCostElementBase>> MakeCandidates(const TArray<TTuple<ItemId, Level>>& InItemIdMinLevels, const bool InUseCandidateItemGroup);

	TArray<TSharedRef<FGsCostElementBase>> MakeCandidates(const TArray<ItemId>& InItemIds, const bool InUseCandidateItemGroup);

	TSharedPtr<const FGsCostElementBase> GetFirstCandidate() const;

	Currency GetFirstCandidateAmount() const;

public:
	bool HasAnyInCandidate(const CurrencyType& InCurrencyType) const;

	bool HasAnyInCandidate(const TSet<CurrencyType>& InCurrencyTypes) const;

	bool HasAnyInCandidate(const ItemId& InItemId) const;

	bool HasAnyInCandidate(const ItemId& InItemId, const Level& InItemLevel) const;

	bool HasAnyInCandidate(const TSet<ItemId>& InItemIds) const;

	bool HasAnyInCandidate(const ItemCategorySub& InItemCategorySub) const;

	uint8 GetCandidateCount() const;

	TArray<TWeakPtr<const FGsCostElementCurrency>> GetAllCandidateCurrencies() const;

	TArray<TWeakPtr<const FGsCostElementItem>> GetAllCandidateItems() const;

	const FGsCostElementBase* GetCandidateAt(const uint8 InIndex) const;

	void MakeAllPassedCandidateItems(OUT TSet<TWeakPtr<FGsItem>>&OutItems) const;

	void SortCandidates(const EGsSortItemDisposal InSortItemDisposal = EGsSortItemDisposal::Efficiency);
	void SortCandidatesForCraft(const EGsSortItemDisposal InSortItemDisposal = EGsSortItemDisposal::Efficiency);
};


/**
 * 비용 정보 저장 및 관리 등
 * 구매 등에 돈(골드) 뿐 아니라, 다양한 기타 아이템(재료, 다이아 등) 을 필요로 할 수 있는데, 이러한 여러 비용 관련 정보들을 저장하고 관리할 때 사용.
*/
class FGsCostPackage final : FNoncopyable, public TSharedFromThis<FGsCostPackage>
{
	friend class FGsCostElement;

private:
	ECachedValueItemDelegateType _cachedValueItemDelegateType = ECachedValueItemDelegateType::DEFAULT;

	/** SetJointCostCandidateElements() 참고 */
	bool _jointCostCandidateElements = false;

	TArray<FGsCostElement> _allCostElements;

	TOptional<TArray<TSharedRef<FGsCostOption>>> _allCostOptions;

	/************************************************************************/
	/* 인스턴싱																*/
	/************************************************************************/

public:
	FGsCostPackage();

	explicit FGsCostPackage(const ECachedValueItemDelegateType InGetCachedValueDelegate);

	FGsCostPackage(FGsCostPackage&& Rhs);

	FGsCostPackage(const FGsCostPackage&) = delete;

	FGsCostPackage& operator =(const FGsCostPackage&) = delete;

public:
	static TSharedRef<FGsCostPackage> CreateSimple(const CostType InCostType, const CurrencyType InCurrencyType, const ItemId InItemId, const Currency InCostAmount);

	static TSharedRef<FGsCostPackage> CreateSimple(const CurrencyType InCurrencyType, const Currency InCostAmount);

	static TSharedRef<FGsCostPackage> CreateSimple(const ItemId InItemId, const Currency InCostAmount);

//private:
//	static TOptional<TArray<TSharedRef<FGsCostOption>>> CopyAllCostOptions(const TOptional<TArray<TSharedRef<FGsCostOption>>>& InAllCostOptions);

	/************************************************************************/
	/* Setters																*/
	/************************************************************************/

public:
	/**
	 * CostCaldidateElements 계산 시, 모든 가능한 요소를 합산 계산할 지 여부를 설정.
	 * 예를 들어, Candidate에 3개의 요소가 있고, 각 요소는 4개의 amount를 가진 상황에서, 필요 amount 가 10인 경우.
	 * 이 값이 참이면, Candidate 3개 요소의 총합인 12개를 처분할 수 있으므로, 비용 처리가 가능.
	 * 이 값이 거짓이면, Candidate 총합과 무관하게, 현재 활성화된 Candidate 요소의 갯수가 4 이므로, 비용 처리가 불가능.
	 */
	void SetJointCostCandidateElements(const bool InJointCostCandidateElements);

	/**
	 * 아이템을 하나만 입력해도, 항상 해당 아이템그룹의 모든 아이템을 대체아이템으로 입력할지 여부를 설정
	 */
	void SetCandidateByItemGroup(const bool InCandidateByItemGroup);

	FGsCostPackage& AddCost(const CostType InCostType, const CurrencyType InCurrencyType, const ItemId InCostItemId, const Currency InCostAmount, const Currency InDisplayCostAmount = INVALID_DISPLAY_COST_AMOUNT);

	FGsCostPackage& AddCost(const CurrencyType InCurrencyType, const Currency InCostAmount, const Currency InDisplayCostAmount = INVALID_DISPLAY_COST_AMOUNT);

	FGsCostPackage& AddCost(const CurrencyType InCurrencyType, const FString& InCostAmountString);

	FGsCostPackage& AddCost(const ItemId InItemId, const Currency InCostAmount, const bool InIncludeItemGroup = false, const Currency InDisplayCostAmount = INVALID_DISPLAY_COST_AMOUNT);

	FGsCostPackage& AddCost(const ItemId InItemId, const Level InItemLevel, const Currency InCostAmount, const bool InIncludeItemGroup = false, const Currency InDisplayCostAmount = INVALID_DISPLAY_COST_AMOUNT);

	FGsCostPackage& AddCost(const ItemCategorySub InItemCategorySub, const Currency InCostAmount, const Currency InDisplayCostAmount = INVALID_DISPLAY_COST_AMOUNT);

	FGsCostPackage& AddCostCandidates(const TArray<TTuple<ItemId, Level>>& InItemIdMinLevels, const Currency InCostAmount, const bool InIncludeItemGroup = false, const Currency InDisplayCostAmount = INVALID_DISPLAY_COST_AMOUNT);

	FGsCostPackage& AddCostCandidates(const TArray<ItemId>& InItemIds, const Currency InCostAmount, const bool InIncludeItemGroup = false, const Currency InDisplayCostAmount = INVALID_DISPLAY_COST_AMOUNT);

	FGsCostPackage& AddCostOption(TSharedRef<FGsCostOption> InCostOption);

private:
	template<typename... TArgs>
	FGsCostElement& AddCostInternal(const bool InIncludeItemGroup, const Currency InCostAmount, const Currency InDisplayCostAmount, TArgs&... InArgs)
	{
		FGsCostElement* CostElement = FindCostElement(InArgs...);
		if (nullptr == CostElement)
		{
			const int32 Index = _allCostElements.Emplace(*this, InIncludeItemGroup, InCostAmount, InDisplayCostAmount, InArgs...);
			LinkCostOptionIfPassedElement(_allCostElements[Index]);
			CostElement = &_allCostElements[Index];
		}
		else
		{
			CostElement->AddAmount(InCostAmount, InDisplayCostAmount);
		}
		return *CostElement;
	}

	/**
	 * 비용 옵션이 먼저 설정된 후에 비용이 추가된 경우, 이 함수가 호출되어 가용한 비용 옶션이 있다면 링크시킴.
	 */
	void LinkCostOptionIfPassedElement(FGsCostElement& InCostElement);

	/************************************************************************/
	/* Getters																*/
	/************************************************************************/

public:
	bool GetJointCostCandidateElements() const;

	ECachedValueItemDelegateType GetCachedValueItemDelegateType() const;

	/**
	 * 현재 유저 소유 재화로 가능한 최대 갯수. 잠김, 장착 등 즉시 처분 불가능한 아이템은 제외함.
	 */
	int32 GetPlayerCanUseMaxCount() const;

	Currency GetCostAmount(const CostType& InCostType, const CurrencyType& InCurrencyType, const ItemId& InItemId) const;

	Currency GetCostAmount(const ItemId& InItemId) const;

	Currency GetCostAmount(const ItemId& InItemId, const Level& InItemLevel) const;

	Currency GetCostAmount(const ItemCategorySub& InItemCategorySub) const;

	Currency GetCostAmount(const CurrencyType& InCurrencyType) const;

	Currency GetFirstCostAmount(const ItemId& InItemId) const;

	Currency GetFirstCostAmount(const ItemId& InItemId, const Level& InItemLevel) const;

	Currency GetFirstCostAmount(const ItemCategorySub& InItemCategorySub) const;

	Currency GetFirstCostAmount(const CurrencyType& InCurrencyType) const;

	FString GetFirstCostAmountString(const CurrencyType& InCurrencyType) const;

	Currency GetDisplayCostAmount(const CostType& InCostType, const CurrencyType& InCurrencyType, const ItemId& InItemId) const;

	Currency GetDisplayCostAmount(const ItemId& InItemId) const;

	Currency GetDisplayCostAmount(const ItemId& InItemId, const Level& InItemLevel) const;

	Currency GetDisplayCostAmount(const ItemCategorySub& InItemCategorySub) const;

	Currency GetDisplayCostAmount(const CurrencyType& InCurrencyType) const;

	/**
	 * 보유한 아이템 중에서 조건을 만족하는 모든 아이템 목록을 반환함.
	 * 기본적으로 인벤토리 내부 아이템들이 카운팅 대상이 되고, CostPackage 생성 파라미터에 따라서 창고 등을 포함할 수도 있다.
	 */
	TSet<TWeakPtr<FGsItem>> GetAllPassedItems() const;

private:
	template<CostType CostTypeValue, typename ... TArgs>
	Currency GetCostAmountInternal(const TArgs&... InArgs) const
	{
		const FGsCostElement* FoundCostElement = nullptr;
		for (const FGsCostElement& CostElement : _allCostElements)
		{
			if (CostElement._costType == CostTypeValue && CostElement.HasAnyInCandidate(InArgs ...))
			{
				FoundCostElement = &CostElement;
				break;
			}
		}
		return (nullptr == FoundCostElement) ? 0 : FoundCostElement->GetCostAmount();
	}
	
	template<CostType CostTypeValue, typename ... TArgs>
	Currency GetFirstCostAmountInternal(const TArgs&... InArgs) const
	{
		for (const FGsCostElement& CostElement : _allCostElements)
		{
			if (CostElement._costType == CostTypeValue && CostElement.HasAnyInCandidate(InArgs ...))
			{
				return CostElement.GetCostAmount();
			}
		}

		return 0;
	}

	template<CostType CostTypeValue, typename ... TArgs>
	Currency GetDisplayCostAmountInternal(const TArgs&... InArgs) const
	{
		const FGsCostElement* FoundCostElement = nullptr;
		for (const FGsCostElement& CostElement : _allCostElements)
		{
			if (CostElement._costType == CostTypeValue && CostElement.HasAnyInCandidate(InArgs ...))
			{
				FoundCostElement = &CostElement;
				break;
			}
		}
		return (nullptr == FoundCostElement) ? 0 : FoundCostElement->GetDisplayCostAmount();
	}

	/************************************************************************/
	/* cost element															*/
	/************************************************************************/

public:
	/**
	 * 모든 cost elements 들의 candidate 를 재정렬.
	 * 대체아이템 정렬이 필요할 때 사용.
	 */
	void SortAllElements(const EGsSortItemDisposal InSortItemDisposal = EGsSortItemDisposal::Efficiency);

	/** CostPackage 에 담긴 Element 중, 특정 element 에 해당하는 녀석만 Sort 진행 (대체 아이템 팝업 오픈 시 필요) */
	void SortElementAtIndex(const uint8 InIndex, const EGsSortItemDisposal InSortItemDisposal = EGsSortItemDisposal::Efficiency);

	TArray<FGsCostElement>::TConstIterator CreateConstIteratorCostElements() const;

	bool IsValidCostElementsIndex(int32 InIndex) const;

	int32 GetCostElementsCount() const;

	const FGsCostElement* GetCostElementAt(const int32 InIndex) const;

	const FGsCostElement* GetFirstCostElement() const;

	const FGsCostElement* FindFirstCostElementBy(const CostType InCostType) const;

	bool HasCostElement(const CurrencyType InCurrencyType) const;

	bool HasCostElement(const ItemId InItemId) const;

	/** 비용이 부족한 첫번째 비용엘리멘트를 반환. 없으면 nullptr 반환. 매개변수로 특정 CostType 제한 가능. */
	const FGsCostElement* GetFirstCostElementNotEnough(const CostType InCostType = CostType::MAX, const int32 InMultiple = 1) const;

private:
	template<CostType CostTypeValue, typename ... TArgs>
	FGsCostElement* FindCostElementInternal(const TArgs&... InArgs)
	{
		for (int32 i = _allCostElements.Num() - 1; i >= 0; i--)
		{
			FGsCostElement& CostElement = _allCostElements[i];
			if (CostTypeValue == CostElement._costType && CostElement.HasAnyInCandidate(InArgs ...))
			{
				return &CostElement;
			}
		}
		return nullptr;
	}

	FGsCostElement* FindCostElement(const CurrencyType& InCurrencyType);

	FGsCostElement* FindCostElement(const ItemId& InItemId);

	FGsCostElement* FindCostElement(const ItemId& InItemId, const Level& InItemLevel);

	/** InItemIds 중에서 하나라도 일치하는 비용 정보를 찾아서 반환 */
	FGsCostElement* FindCostElement(const TSet<ItemId>& InItemIds);

	FGsCostElement* FindCostElement(const ItemCategorySub& InItemCategorySub);

	/************************************************************************/
	/* validator															*/
	/************************************************************************/

public:
	/**
	 * 모든 비용이 충분한지 여부.
	 * 비용이 배수일 경우 매개변수를 이용.
	 */
	bool PlayerHasEnough(const int32 InMultiple = 1) const;

	/** 현재 비용의 모든 엘리멘트가 유효하지 않아도, 최소 하나 이상이 유효(갯수가 0 이상이고 타입이 None 등이 아닐 것)하다면, true 반환*/
	bool IsValidCostAtLeast() const;

	/************************************************************************/
	/* cost package multiple												*/
	/************************************************************************/

public:
	TSharedRef<FGsCostPackageMultiple> MakeCostPackageMultiple(const int32 InDefaultMultiple = 1) const;
};
