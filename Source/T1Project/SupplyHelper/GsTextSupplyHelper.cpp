#include "GsTextSupplyHelper.h"

#include "Currency/GsCurrencyHelper.h"
#include "DataSchema/GsSchemaCurrencyData.h"
#include "DataSchema/GsSchemaEnums.h"
#include "DataSchema/Item/GsSchemaItemCommon.h"
#include "DataSchema/SupplyInfo/GsSchemaCurrencySupplyInfoSet.h"
#include "DataSchema/SupplyInfo/GsSchemaItemSupplyInfoSet.h"
#include "DataSchema/SupplyInfo/GsSchemaSkillSupplyInfoSet.h"
#include "DataSchema/SupplyInfo/GsSchemaSupplyInfoConstraint.h"
#include "GsBaseSupplyHelper.h"
#include "Management/ScopeGame/GsSupplyManager.h"
#include "Management/ScopeGame/GsUtilizingContentsManager.h"
#include "Shared/Client/SharedEnums/SharedCommonEnum.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "T1Project.h"
#include "Unlock/GsUnlockDefine.h"
#include "../Item/GsItemManager.h"

FGsTextSupplyHelper::FGsTextSupplyHelper() :
	FGsBaseSupplyHelper(EGsUnlockContentsType::NONE)
{
}

FText FGsTextSupplyHelper::GetTitleTextCurrencyType(const CurrencyType InCurrencyType) const
{
	FText Text;

	const FGsSchemaCurrencyData* Data = FGsCurrencyHelper::GetCurrencyData(InCurrencyType);
	if (nullptr != Data)
	{
		Text = Data->acq;
	}

	return Text;
}

FText FGsTextSupplyHelper::GetTitleTextItemId(const ItemId InItemId) const
{
	FText Text;

	const UGsTableItemCommon* Data = Cast<UGsTableItemCommon>(FGsSchemaItemCommon::GetStaticTable());
	if (nullptr != Data)
	{
		const FGsSchemaItemCommon* ItemCommon = nullptr;
		if (Data->FindRowById(InItemId, ItemCommon) && nullptr != ItemCommon)
		{
			const FGsSchemaSupplyInfoConstraintRow& SupplyInfoConstraintRow = ItemCommon->supplyInfoConstraint;
			if (!SupplyInfoConstraintRow.IsNull())
			{
				const FGsSchemaSupplyInfoConstraint* SupplyInfoConstraint = SupplyInfoConstraintRow.GetRow();
				if (nullptr != SupplyInfoConstraint)
				{
					Text = FText::Join(FGsUtilizingContentsManager::HYPERLINK_DELIMETER, SupplyInfoConstraint->extraTexts);
				}
			}
		}
	}

	return Text;
}

FText FGsTextSupplyHelper::GetTitleTextSkillId(const SkillId InUserSkillSetId) const
{
	TArray<FText> Texts;

	const UGsItemManager* ItemManager = GItem();
	if (nullptr != ItemManager)
	{
		const UGsTableSkillSupplyInfoSet* Data = Cast<UGsTableSkillSupplyInfoSet>(FGsSchemaSkillSupplyInfoSet::GetStaticTable());
		if (nullptr != Data)
		{
			// 스킬ID에 해당하는 공급처세트를 조회.
			const FGsSchemaSkillSupplyInfoSet* SupplyInfoSet = nullptr;
			if (Data->FindRowById(InUserSkillSetId, SupplyInfoSet))
			{
				// 공급처세트에는 모든 스킬북아이템ID가 있으므로, 해당 아이템들에서 모든 공급처제한정보를 수집.
				TSet<const FGsSchemaSupplyInfoConstraint*> SupplyInfoConstraints;
				for (const int32 CurrentItemId : SupplyInfoSet->itemIds)
				{
					const FGsSchemaItemCommon* ItemCommon = ItemManager->GetItemTableDataByTID(CurrentItemId);
					if (nullptr != ItemCommon)
					{
						SupplyInfoConstraints.Add(ItemCommon->supplyInfoConstraint.GetRow());
					}
				}

				// 수집한 모든 공급처제한정보에서 "텍스트" 출력형 값들만 따로 수집.
				for (const FGsSchemaSupplyInfoConstraint* SupplyInfoConstraint : SupplyInfoConstraints)
				{
					if (nullptr != SupplyInfoConstraint)
					{
						for (const FText& ExtraText : SupplyInfoConstraint->extraTexts)
						{
							bool IsExistedText = false;
							for (const FText& ExistedExtraText : Texts)
							{
								if (0 == ExistedExtraText.CompareTo(ExtraText))
								{
									IsExistedText = true;
									break;
								}
							}

							if (!IsExistedText)
							{
								Texts.Add(ExtraText);
							}
						}
					}
				}
			}
		}
	}

	return FText::Join(FGsUtilizingContentsManager::HYPERLINK_DELIMETER, Texts);
}

FText FGsTextSupplyHelper::GetTitleText() const
{
	GSLOG(Warning, TEXT("return empty text"));
	return FText::GetEmpty();
}

UGsSupplyManager::EGsNavigationStatus FGsTextSupplyHelper::GetNavigationStatus(const FGsSchemaCurrencySupplyInfoSet& InSupplyInfoSet) const
{
	return GetTitleTextCurrencyType(InSupplyInfoSet.currencyType).IsEmpty() ? UGsSupplyManager::EGsNavigationStatus::None : UGsSupplyManager::EGsNavigationStatus::Displayable;
}

bool FGsTextSupplyHelper::TryToNavigate(const FGsSchemaCurrencySupplyInfoSet& InSupplyInfoSet, OUT FText* OutFailMessage /*= nullptr*/) const
{
	GSLOG(Error, TEXT("FGsTextSupplyHelper::TryToNavigate()"));
	return false;
}

UGsSupplyManager::EGsNavigationStatus FGsTextSupplyHelper::GetNavigationStatus(const FGsSchemaItemSupplyInfoSet& InSupplyInfoSet) const
{
	return GetTitleTextItemId(InSupplyInfoSet.itemId).IsEmpty() ? UGsSupplyManager::EGsNavigationStatus::None : UGsSupplyManager::EGsNavigationStatus::Displayable;
}

bool FGsTextSupplyHelper::TryToNavigate(const FGsSchemaItemSupplyInfoSet& InSupplyInfoSet, OUT FText* OutFailMessage /*= nullptr*/) const
{
	GSLOG(Error, TEXT("FGsTextSupplyHelper::TryToNavigate()"));
	return false;
}

UGsSupplyManager::EGsNavigationStatus FGsTextSupplyHelper::GetNavigationStatus(const FGsSchemaSkillSupplyInfoSet& InSupplyInfoSet) const
{
	return GetTitleTextSkillId(InSupplyInfoSet.skillId).IsEmpty() ? UGsSupplyManager::EGsNavigationStatus::None : UGsSupplyManager::EGsNavigationStatus::Displayable;
}

bool FGsTextSupplyHelper::TryToNavigate(const FGsSchemaSkillSupplyInfoSet& InSupplyInfoSet, OUT FText* OutFailMessage /*= nullptr*/) const
{
	GSLOG(Error, TEXT("FGsTextSupplyHelper::TryToNavigate()"));
	return false;
}
