#include "GsBaseDemandHelper.h"

#include "DataSchema/GsSchemaCurrencyData.h"
#include "DataSchema/GsSchemaEnums.h"
#include "DataSchema/Item/GsSchemaItemCommon.h"
#include "Management/GsScopeHolder.h"
#include "Management/ScopeGame/GsSupplyManager.h"
#include "Management/ScopeGame/GsUnlockManager.h"
#include "Shared/Client/SharedEnums/SharedCommonEnum.h"
#include "Unlock/GsUnlockDefine.h"

FGsBaseDemandHelper::FGsBaseDemandHelper(const EGsUnlockContentsType InContentsType)
	: _contentsType(InContentsType)
{
}

FGsBaseDemandHelper::FGsBaseDemandHelper()
	: FGsBaseDemandHelper(EGsUnlockContentsType::NONE)
{
}

FGsBaseDemandHelper::~FGsBaseDemandHelper()
{
}

FText FGsBaseDemandHelper::GetTitleText(const CostType InCostType, const CurrencyType InCurrencyType, const ItemId InItemId) const
{
	FText TitleText;
	const FGsUnlockManager* UnlockManager = GSUnlock();
	if (nullptr != UnlockManager)
	{
		UnlockManager->GetContentsName(_contentsType, TitleText);
	}
	return TitleText;
}

UGsSupplyManager::EGsNavigationStatus FGsBaseDemandHelper::GetNavigationStatus(const FGsSchemaCurrencyDemandInfoSet& InDemandInfo) const
{
	return UGsSupplyManager::EGsNavigationStatus::None;
}

bool FGsBaseDemandHelper::TryToNavigate(const FGsSchemaCurrencyDemandInfoSet& InCurrencyDemandInfoSet, OUT FText* OutFailMessage /*= nullptr*/) const
{
	return false;
}

UGsSupplyManager::EGsNavigationStatus FGsBaseDemandHelper::GetNavigationStatus(const FGsSchemaItemDemandInfoSet& InDemandInfo) const
{
	return UGsSupplyManager::EGsNavigationStatus::None;
}

bool FGsBaseDemandHelper::TryToNavigate(const FGsSchemaItemDemandInfoSet& InItemDemandInfoSet, OUT FText* OutFailMessage /*= nullptr*/) const
{
	return false;
}

UGsSupplyManager::EGsNavigationStatus FGsBaseDemandHelper::GetNavigationStatusStatic(const EGsUnlockContentsType InContentsType, const CurrencyType InCurrencyType)
{
	// 언락 상태 체크
	{
		FGsUnlockManager* UnlockManager = GSUnlock();
		if (nullptr == UnlockManager)
		{
			GSLOG(Error, TEXT("nullptr == UnlockManager"));
			return UGsSupplyManager::EGsNavigationStatus::None;
		}

		if (!UnlockManager->IsContentsUnlock(InContentsType))
		{
			// 잠금 상태라도, 링크를 클릭하여 잠김 상태 출력을 위해 Movable 로 반환.
			return UGsSupplyManager::EGsNavigationStatus::Movable;
		}
	}

	// extra 테이블로 체크
	//{
	//	const UGsTableCurrencyData* CurrencyDataTable = Cast<const UGsTableCurrencyData>(FGsSchemaCurrencyData::GetStaticTable());
	//	if (nullptr == CurrencyDataTable)
	//	{
	//		GSLOG(Error, TEXT("nullptr == CurrencyDataTable"));
	//		return UGsSupplyManager::EGsNavigationStatus::None;
	//	}

	//	const FGsSchemaCurrencyData* CurrencyData = nullptr;
	//	if (!CurrencyDataTable->FindRowById(InCurrencyType, CurrencyData) || nullptr == CurrencyData || CurrencyData->demandInfoConstraint.IsNull())
	//	{
	//		// extra 가 지정되어 있지 않다면, 항상 가능한 것으로 간주
	//		return UGsSupplyManager::EGsNavigationStatus::Movable;
	//	}

	//	return FGsBaseDemandHelper::GetNavigationStatusStatic(InContentsType, *CurrencyData->demandInfoConstraint.GetRow());
	//}

	return UGsSupplyManager::EGsNavigationStatus::Movable;
}

UGsSupplyManager::EGsNavigationStatus FGsBaseDemandHelper::GetNavigationStatusStatic(const EGsUnlockContentsType InContentsType, const ItemId InItemId)
{
	// 언락 상태 체크
	//{
	//	FGsUnlockManager* UnlockManager = GSUnlock();
	//	if (nullptr == UnlockManager)
	//	{
	//		GSLOG(Error, TEXT("nullptr == UnlockManager"));
	//		return UGsSupplyManager::EGsNavigationStatus::None;
	//	}

	//	if (!UnlockManager->IsContentsUnlock(InContentsType))
	//	{
	//		// 잠금 상태라도, 링크를 클릭하여 잠김 상태 출력을 위해 Movable 로 반환.
	//		return UGsSupplyManager::EGsNavigationStatus::Movable;
	//	}
	//}

	// extra 테이블로 체크
	//{
	//	const UGsTableItemCommon* ItemCommonTable = Cast<const UGsTableItemCommon>(FGsSchemaItemCommon::GetStaticTable());
	//	if (nullptr == ItemCommonTable)
	//	{
	//		GSLOG(Error, TEXT("nullptr == ItemCommonTable"));
	//		return UGsSupplyManager::EGsNavigationStatus::None;
	//	}

	//	const FGsSchemaItemCommon* ItemCommon = nullptr;
	//	if (!ItemCommonTable->FindRowById(InItemId, ItemCommon) || nullptr == ItemCommon || ItemCommon->demandInfoConstraint.IsNull())
	//	{
	//		// extra 가 지정되어 있지 않다면, 항상 가능한 것으로 간주
	//		return UGsSupplyManager::EGsNavigationStatus::Movable;
	//	}

	//	return FGsBaseDemandHelper::GetNavigationStatusStatic(InContentsType, *ItemCommon->demandInfoConstraint.GetRow());
	//}

	return UGsSupplyManager::EGsNavigationStatus::Movable;
}

//UGsSupplyManager::EGsNavigationStatus FGsBaseDemandHelper::GetNavigationStatusStatic(const EGsUnlockContentsType InContentsType, const FGsSchemaDemandInfoConstraint& InDemandInfoConstraint)
//{
//	if (EGsContentsShortcutMethod::Exclusive == InDemandInfoConstraint.showContentsTypesMethod)
//	{
//		// 배타적 리스트인 경우에는, 리스트에 포함된 경우에만 유효함.
//		return InDemandInfoConstraint.showContentsTypes.Contains(InContentsType) ? UGsSupplyManager::EGsNavigationStatus::None : UGsSupplyManager::EGsNavigationStatus::Movable;
//	}
//	else if (EGsContentsShortcutMethod::Inclusive == InDemandInfoConstraint.showContentsTypesMethod)
//	{
//		// 포괄적 리스트인 경우에는, 리스트에 무관하게 유효함.
//		return UGsSupplyManager::EGsNavigationStatus::Movable;
//	}
//
//	// 그 외, 알 수 없음
//	return UGsSupplyManager::EGsNavigationStatus::None;
//}
