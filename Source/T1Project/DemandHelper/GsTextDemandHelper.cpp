#include "GsTextDemandHelper.h"

#include "Currency/GsCurrencyHelper.h"
#include "DataSchema/DemandInfo/GsSchemaCurrencyDemandInfoSet.h"
#include "DataSchema/DemandInfo/GsSchemaDemandInfoConstraint.h"
#include "DataSchema/DemandInfo/GsSchemaItemDemandInfoSet.h"
#include "DataSchema/GsSchemaCurrencyData.h"
#include "DataSchema/Item/GsSchemaItemCommon.h"
#include "Management/ScopeGame/GsSupplyManager.h"
#include "Management/ScopeGame/GsUtilizingContentsManager.h"
#include "Shared/Client/SharedEnums/SharedCommonEnum.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "T1Project.h"
#include "Unlock/GsUnlockDefine.h"

FGsTextDemandHelper::FGsTextDemandHelper()
	: FGsBaseDemandHelper(EGsUnlockContentsType::NONE)
{
}

FText FGsTextDemandHelper::GetTitleText(const CostType InCostType, const CurrencyType InCurrencyType, const ItemId InItemId) const
{
	FText Text;

	if (CostType::CURRENCY == InCostType)
	{
		const FGsSchemaCurrencyData* Data = FGsCurrencyHelper::GetCurrencyData(InCurrencyType);
		if (nullptr != Data)
		{
			Text = Data->usedfor;
		}
	}
	else if (CostType::ITEM == InCostType)
	{
		const UGsTableItemCommon* Data = Cast<UGsTableItemCommon>(FGsSchemaItemCommon::GetStaticTable());
		if (nullptr != Data)
		{
			const FGsSchemaItemCommon* ItemCommon = nullptr;
			if (Data->FindRowById(InItemId, ItemCommon) && nullptr != ItemCommon)
			{
				const FGsSchemaDemandInfoConstraintRow& DemandInfoConstraintRow = ItemCommon->demandInfoConstraint;
				if (!DemandInfoConstraintRow.IsNull())
				{
					const FGsSchemaDemandInfoConstraint* DemandInfoConstraint = DemandInfoConstraintRow.GetRow();
					if (nullptr != DemandInfoConstraint)
					{
						Text = FText::Join(FGsUtilizingContentsManager::HYPERLINK_DELIMETER, DemandInfoConstraint->extraTexts);
					}
				}
			}
		}
	}

	return Text;
}

UGsSupplyManager::EGsNavigationStatus FGsTextDemandHelper::GetNavigationStatus(const FGsSchemaCurrencyDemandInfoSet& InDemandInfo) const
{
	return GetTitleText(CostType::CURRENCY, InDemandInfo.currencyType, INVALID_ITEM_ID).IsEmpty() ? UGsSupplyManager::EGsNavigationStatus::None : UGsSupplyManager::EGsNavigationStatus::Displayable;
}

bool FGsTextDemandHelper::TryToNavigate(const FGsSchemaCurrencyDemandInfoSet& InCurrencyDemandInfoSet, OUT FText* OutFailMessage /*= nullptr*/) const
{
	GSLOG(Error, TEXT("FGsTextDemandHelper::TryToNavigate()"));
	return false;
}

UGsSupplyManager::EGsNavigationStatus FGsTextDemandHelper::GetNavigationStatus(const FGsSchemaItemDemandInfoSet& InDemandInfo) const
{
	return GetTitleText(CostType::ITEM, CurrencyType::NONE, InDemandInfo.itemId).IsEmpty() ? UGsSupplyManager::EGsNavigationStatus::None : UGsSupplyManager::EGsNavigationStatus::Displayable;
}

bool FGsTextDemandHelper::TryToNavigate(const FGsSchemaItemDemandInfoSet& InItemDemandInfoSet, OUT FText* OutFailMessage /*= nullptr*/) const
{
	GSLOG(Error, TEXT("FGsTextDemandHelper::TryToNavigate()"));
	return false;
}
