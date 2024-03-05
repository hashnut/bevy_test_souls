#include "GsBMShopDemandHelper.h"

#include "Classes/GsMessageHandlerOneParam.h"
#include "Currency/GsCurrencyHelper.h"
#include "DataSchema/DemandInfo/GsSchemaCurrencyDemandInfoSet.h"
#include "DataSchema/DemandInfo/GsSchemaDemandInfoConstraint.h"
#include "DataSchema/DemandInfo/GsSchemaItemDemandInfoSet.h"
#include "DataSchema/GsSchemaCurrencyData.h"
#include "DataSchema/Item/GsSchemaItemCommon.h"
#include "GsBaseDemandHelper.h"
#include "Management/GsMessageHolder.h"
#include "Management/ScopeGlobal/GsBMShopManager.h"
#include "Management/ScopeGame/GsSupplyManager.h"
#include "Management/ScopeGame/GsUnlockManager.h"
#include "Message/GsMessageContents.h"
#include "Message/MessageParam/GsBMShopMessageParam.h"
#include "Shared/Client/SharedEnums/SharedCommonEnum.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "T1Project.h"
#include "Unlock/GsUnlockDefine.h"

FGsBMShopDemandHelper::FGsBMShopDemandHelper()
	: Super(EGsUnlockContentsType::BMSHOP)
{
}

UGsSupplyManager::EGsNavigationStatus FGsBMShopDemandHelper::GetNavigationStatus(const FGsSchemaCurrencyDemandInfoSet& InDemandInfo) const
{
	int32 BMShopTabId = 0;
	if (!FGsBMShopDemandHelper::GetBMShopTabId(InDemandInfo.currencyType, BMShopTabId))
	{
		return UGsSupplyManager::EGsNavigationStatus::None;
	}

	if (!FGsBMShopDemandHelper::CanDisplay(BMShopTabId))
	{
		return UGsSupplyManager::EGsNavigationStatus::None;
	}

	return UGsSupplyManager::EGsNavigationStatus::Movable;
}

bool FGsBMShopDemandHelper::TryToNavigate(const FGsSchemaCurrencyDemandInfoSet& InDemandInfoSet, OUT FText* OutFailMessage /*= nullptr*/) const
{
	int32 BMShopTabId = 0;
	if (!FGsBMShopDemandHelper::GetBMShopTabId(InDemandInfoSet.currencyType, BMShopTabId))
	{
		return false;
	}

	return FGsBMShopDemandHelper::TryToNavigate(BMShopTabId, OutFailMessage);
}

UGsSupplyManager::EGsNavigationStatus FGsBMShopDemandHelper::GetNavigationStatus(const FGsSchemaItemDemandInfoSet& InDemandInfo) const
{
	int32 BMShopTabId = 0;
	if (!FGsBMShopDemandHelper::GetBMShopTabId(InDemandInfo.itemId, BMShopTabId))
	{
		return UGsSupplyManager::EGsNavigationStatus::None;
	}

	if (!FGsBMShopDemandHelper::CanDisplay(BMShopTabId))
	{
		return UGsSupplyManager::EGsNavigationStatus::None;
	}

	return UGsSupplyManager::EGsNavigationStatus::Movable;
}

bool FGsBMShopDemandHelper::TryToNavigate(const FGsSchemaItemDemandInfoSet& InDemandInfoSet, OUT FText* OutFailMessage /*= nullptr*/) const
{
	int32 BMShopTabId = 0;
	if (!FGsBMShopDemandHelper::GetBMShopTabId(InDemandInfoSet.itemId, BMShopTabId))
	{
		return false;
	}

	return FGsBMShopDemandHelper::TryToNavigate(BMShopTabId, OutFailMessage);
}

bool FGsBMShopDemandHelper::CanDisplay(const int32& InBmshopTabId)
{
	if (0 == InBmshopTabId)
	{
		GSLOG(Warning, TEXT("0 == InBmshopTabId"));
		return false;
	}

	// 현재 해당 탭이 유효한지 체크.
	FGsBMShopManager* BMShopManager = GBMShop();
	if (nullptr == BMShopManager)
	{
		return false;
	}

	return BMShopManager->IsExistTabID(InBmshopTabId);
}

bool FGsBMShopDemandHelper::TryToNavigate(const int32& InBmshopTabId, OUT FText* OutFailMessage)
{
	if (!FGsBMShopDemandHelper::CanDisplay(InBmshopTabId))
	{
		if (nullptr != OutFailMessage)
		{
			*OutFailMessage = NSLOCTEXT("BMShopDemandHelper", "InvalidBmshopTabId", "상점으로 이동할 수 없습니다.");
		}
		return false;
	}

	FGsUnlockManager* UnlockManager = GSUnlock();
	FGsMessageHolder* MessageHolder = GMessage();
	if (nullptr == UnlockManager || nullptr == MessageHolder)
	{
		GSLOG(Warning, TEXT("nullptr == UnlockManager || nullptr == MessageHolder"));
		return false;
	}

	// 잠김체크
	if (!UnlockManager->IsContentsUnlock(EGsUnlockContentsType::BMSHOP))
	{
		if (nullptr != OutFailMessage)
		{
			UnlockManager->GetUnLockMessage(EGsUnlockContentsType::BMSHOP, *OutFailMessage);
		}
		return false;
	}

	// R2: int8에 숫자가 잘려서 int32로 받음
	MakeShared<FGsBMShopOpenTab>(InBmshopTabId)->Send();
	return true;
}

bool FGsBMShopDemandHelper::GetBMShopTabId(const CurrencyType InCurrencyType, OUT int32& OutBMShopTabId)
{
	const FGsSchemaCurrencyData* CurrencyData = FGsCurrencyHelper::GetCurrencyData(InCurrencyType);
	if (nullptr == CurrencyData)
	{
		return false;
	}

	if (!FGsBMShopDemandHelper::GetBMShopTabId(CurrencyData->demandInfoConstraint.GetRow(), OutBMShopTabId))
	{
		return false;
	}

	return true;
}

bool FGsBMShopDemandHelper::GetBMShopTabId(const ItemId InItemId, OUT int32& OutBMShopTabId)
{
	const UGsTableItemCommon* ItemCommonTable = Cast<UGsTableItemCommon>(FGsSchemaItemCommon::GetStaticTable());
	if (nullptr == ItemCommonTable)
	{
		GSLOG(Error, TEXT("nullptr == ItemCommonTable"));
		return false;
	}

	const FGsSchemaItemCommon* ItemCommon = nullptr;
	if (!ItemCommonTable->FindRowById(InItemId, ItemCommon))
	{
		GSLOG(Error, TEXT("!ItemCommonTable->FindRowById(%d, ItemCommon)"), InItemId);
		return false;
	}

	if (!FGsBMShopDemandHelper::GetBMShopTabId(ItemCommon->demandInfoConstraint.GetRow(), OutBMShopTabId))
	{
		return false;
	}

	return true;
}

bool FGsBMShopDemandHelper::GetBMShopTabId(const FGsSchemaDemandInfoConstraint* InDemandInfoSet, OUT int32& OutBMShopTabId)
{
	if (nullptr == InDemandInfoSet || 0 == InDemandInfoSet->bmshopTabId)
	{
		return false;
	}

	OutBMShopTabId = InDemandInfoSet->bmshopTabId;
	return true;
}
