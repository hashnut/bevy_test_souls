#include "GsContentsEventTeleportCost.h"

#include "DataSchema/ContentsEvent/GsSchemaContentsEventTeleportCost.h"
#include "Management/GsScopeGame.h"
#include "Management/GsScopeHolder.h"
#include "Management/ScopeGame/GsContentsEventManager.h"
#include "Shared/Client/SharedEnums/SharedCommonEnum.h"
#include "Shared/Client/SharedEnums/SharedEventEnum.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "Currency/GsCostOption.h"

constexpr Currency ZERO_CURRENCY = 0;

TSharedPtr<FGsCostOption> FGsContentsEventTeleportCost::MakeCostOption() const
{
	TSharedPtr<FGsCostOption> CostOption;

	// 현재는 모두 골드만 할인 가능함.
	if (ContentsEventTeleportCost::VALUE == _tableData->goldDiscountType)
	{
		// 할인 객체 생성
		TSharedPtr<FGsCostOptionClamp> CostOptionValueAmount = MakeShared<FGsCostOptionClamp>(ZERO_CURRENCY, StaticCast<Currency>(_tableData->goldDiscountValue));

		// 할인 대상 정보 입력
		CostOptionValueAmount->AddCostElementFilter(MakeShared<FGsCostElementFilterCurrencyType>(CurrencyType::GOLD));

		CostOption = CostOptionValueAmount;
	}
	else if (ContentsEventTeleportCost::RATE == _tableData->goldDiscountType)
	{
		// 할인 만분율 객체 생성
		TSharedPtr<FGsCostOptionDiscount10kRatio> CostOptionRateAmount = MakeShared<FGsCostOptionDiscount10kRatio>(StaticCast<Currency>(_tableData->goldDiscountValue));

		// 할인 대상 정보 입력
		CostOptionRateAmount->AddCostElementFilter(MakeShared<FGsCostElementFilterCurrencyType>(CurrencyType::GOLD));

		CostOption = CostOptionRateAmount;
	}

	return CostOption;
}

TSharedPtr<FGsCostOption> FGsContentsEventTeleportCost::CreateActivatedTeleportCostOption()
{
	const UGsContentsEventManager* ContentsEventManager = GSContentsEvent();
	if (nullptr != ContentsEventManager)
	{
		TWeakPtr<const FGsContentsEventTeleportCost> ContentsEventTeleportCost = ContentsEventManager->GetUniqueActivatedContentsEvent<FGsContentsEventTeleportCost>();
		if (ContentsEventTeleportCost.IsValid())
		{
			TSharedPtr<FGsCostOption> CostOption = ContentsEventTeleportCost.Pin()->MakeCostOption();
			if (CostOption.IsValid())
			{
				return CostOption;
			}
		}
	}
	return nullptr;
}
