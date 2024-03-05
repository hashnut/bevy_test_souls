#include "GsBMShopSupplyHelper.h"

#include "Currency/GsCurrencyHelper.h"
#include "DataSchema/GsSchemaCurrencyData.h"
#include "DataSchema/GsSchemaEnums.h"
#include "DataSchema/Item/GsSchemaItemCommon.h"
#include "DataSchema/SupplyInfo/GsSchemaCurrencySupplyInfoSet.h"
#include "DataSchema/SupplyInfo/GsSchemaItemSupplyInfoSet.h"
#include "DataSchema/SupplyInfo/GsSchemaSkillSupplyInfoSet.h"
#include "DataSchema/SupplyInfo/GsSchemaSupplyInfoConstraint.h"
#include "GsBaseSupplyHelper.h"
#include "Item/GsItemManager.h"
#include "Management/GsMessageHolder.h"
#include "Management/GsScopeGame.h"
#include "Management/GsScopeHolder.h"
#include "Management/ScopeGame/GsSupplyManager.h"
#include "Management/ScopeGame/GsUnlockManager.h"
#include "Management/ScopeGlobal/GsBMShopManager.h"
#include "Message/GsMessageContents.h"
#include "Message/MessageParam/GsBMShopMessageParam.h"
#include "Shared/Client/SharedEnums/SharedCommonEnum.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "T1Project.h"
#include "Unlock/GsUnlockDefine.h"

FGsBMShopSupplyHelper::FGsBMShopSupplyHelper()
	: FGsBaseSupplyHelper(EGsUnlockContentsType::BMSHOP)
{
}

FText FGsBMShopSupplyHelper::GetTitleText() const
{
	return NSLOCTEXT("ItemSupplyInfoText", "SupplyInfo_BMShop", "상점");
}

UGsSupplyManager::EGsNavigationStatus FGsBMShopSupplyHelper::GetNavigationStatus(const FGsSchemaCurrencySupplyInfoSet& InSupplyInfoSet) const
{
	int32 BMShopTabId = 0;
	if (!FGsBMShopSupplyHelper::GetBMShopTabIdCurrencyType(InSupplyInfoSet.currencyType, BMShopTabId))
	{
		return UGsSupplyManager::EGsNavigationStatus::None;
	}

	if (!FGsBMShopSupplyHelper::CanDisplay(BMShopTabId))
	{
		return UGsSupplyManager::EGsNavigationStatus::None;
	}

	return UGsSupplyManager::EGsNavigationStatus::Movable;
}

bool FGsBMShopSupplyHelper::TryToNavigate(const FGsSchemaCurrencySupplyInfoSet& InSupplyInfoSet, OUT FText* OutFailMessage /*= nullptr*/) const
{
	int32 BMShopTabId = 0;
	if (!FGsBMShopSupplyHelper::GetBMShopTabIdCurrencyType(InSupplyInfoSet.currencyType, BMShopTabId))
	{
		return false;
	}

	return FGsBMShopSupplyHelper::TryToNavigate(BMShopTabId, OutFailMessage);
}

UGsSupplyManager::EGsNavigationStatus FGsBMShopSupplyHelper::GetNavigationStatus(const FGsSchemaItemSupplyInfoSet& InSupplyInfoSet) const
{
	int32 BMShopTabId = 0;
	if (!FGsBMShopSupplyHelper::GetBMShopTabIdItemId(InSupplyInfoSet.itemId, BMShopTabId))
	{
		return UGsSupplyManager::EGsNavigationStatus::None;
	}

	if (!FGsBMShopSupplyHelper::CanDisplay(BMShopTabId))
	{
		return UGsSupplyManager::EGsNavigationStatus::None;
	}

	return UGsSupplyManager::EGsNavigationStatus::Movable;
}

bool FGsBMShopSupplyHelper::TryToNavigate(const FGsSchemaItemSupplyInfoSet& InSupplyInfoSet, OUT FText* OutFailMessage /*= nullptr*/) const
{
	int32 BMShopTabId = 0;
	if (!FGsBMShopSupplyHelper::GetBMShopTabIdItemId(InSupplyInfoSet.itemId, BMShopTabId))
	{
		return false;
	}

	return FGsBMShopSupplyHelper::TryToNavigate(BMShopTabId, OutFailMessage);
}

UGsSupplyManager::EGsNavigationStatus FGsBMShopSupplyHelper::GetNavigationStatus(const FGsSchemaSkillSupplyInfoSet& InSupplyInfoSet) const
{
	TArray<int32> BMShopTabIds;

	if (FGsBMShopSupplyHelper::GetAllBMShopTabIdsBySkillId(InSupplyInfoSet.skillId, BMShopTabIds))
	{
		for (const int32 BMShopTabId : BMShopTabIds)
		{
			if (FGsBMShopSupplyHelper::CanDisplay(BMShopTabId))
			{
				return UGsSupplyManager::EGsNavigationStatus::Movable;
			}
		}
	}

	return UGsSupplyManager::EGsNavigationStatus::None;
}

bool FGsBMShopSupplyHelper::TryToNavigate(const FGsSchemaSkillSupplyInfoSet& InSupplyInfoSet, OUT FText* OutFailMessage /*= nullptr*/) const
{
	TArray<int32> BMShopTabIds;
	if (!FGsBMShopSupplyHelper::GetAllBMShopTabIdsBySkillId(InSupplyInfoSet.skillId, BMShopTabIds))
	{
		*OutFailMessage = NSLOCTEXT("BMShopSupplyHelper", "InvalidBmshopTabId", "상점으로 이동할 수 없습니다.");
		return false;
	}

	// 가능한 BMShop 이 있다면 이동하고, 없다면 실패.
	for (const int32 BMShopTabId : BMShopTabIds)
	{
		if (FGsBMShopSupplyHelper::TryToNavigate(BMShopTabId, OutFailMessage))
		{
			return true;
		}
	}

	return false;
}

bool FGsBMShopSupplyHelper::CanDisplay(const int32& InBmshopTabId)
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

bool FGsBMShopSupplyHelper::TryToNavigate(const int32& InBmshopTabId, OUT FText* OutFailMessage)
{
	if (!FGsBMShopSupplyHelper::CanDisplay(InBmshopTabId))
	{
		// GSLOG(Error, TEXT("FGsBMShopSupplyHelper::CanDisplay(%d)"), InBmshopTabId);
		if (nullptr != OutFailMessage)
		{
			*OutFailMessage = NSLOCTEXT("BMShopSupplyHelper", "InvalidBmshopTabId", "상점으로 이동할 수 없습니다.");
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
	//const BMStoreTabId CurrentBMStoreTabId = InSupplyInfo.referenceIds[0];
	MakeShared<FGsBMShopOpenTab>(InBmshopTabId)->Send();
	return true;
}

bool FGsBMShopSupplyHelper::GetBMShopTabIdCurrencyType(const CurrencyType InCurrencyType, OUT int32& OutBMShopTabId)
{
	const FGsSchemaCurrencyData* CurrencyData = FGsCurrencyHelper::GetCurrencyData(InCurrencyType);
	if (nullptr == CurrencyData)
	{
		return false;
	}

	if (!FGsBMShopSupplyHelper::GetBMShopTabIdSupplyInfoConstraint(CurrencyData->supplyInfoConstraint.GetRow(), OutBMShopTabId))
	{
		return false;
	}

	return true;
}

bool FGsBMShopSupplyHelper::GetBMShopTabIdItemId(const ItemId InItemId, OUT int32& OutBMShopTabId)
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

	if (!FGsBMShopSupplyHelper::GetBMShopTabIdSupplyInfoConstraint(ItemCommon->supplyInfoConstraint.GetRow(), OutBMShopTabId))
	{
		return false;
	}

	return true;
}

bool FGsBMShopSupplyHelper::GetAllBMShopTabIdsBySkillId(const SkillId InSkillId, TArray<int32>& OutBMShopTabIds)
{
	const UGsItemManager* ItemManager = GItem();
	if (nullptr == ItemManager)
	{
		GSLOG(Error, TEXT("nullptr == ItemManager"));
		return false;
	}

	const UGsTableSkillSupplyInfoSet* Data = Cast<UGsTableSkillSupplyInfoSet>(FGsSchemaSkillSupplyInfoSet::GetStaticTable());
	if (nullptr == Data)
	{
		GSLOG(Error, TEXT("nullptr == Data"));
		return false;
	}

	// 스킬ID에 해당하는 공급처세트를 조회.
	const FGsSchemaSkillSupplyInfoSet* SupplyInfoSet = nullptr;
	if (!Data->FindRowById(InSkillId, SupplyInfoSet))
	{
		GSLOG(Warning, TEXT("!Data->FindRowById(%d, SupplyInfoSet)"), InSkillId);
		return false;
	}

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

	// 수집한 모든 공급처제한정보에서 "BMShopTabId" 값들만 수집.
	for (const FGsSchemaSupplyInfoConstraint* SupplyInfoConstraint : SupplyInfoConstraints)
	{
		if (nullptr != SupplyInfoConstraint && !OutBMShopTabIds.Contains(SupplyInfoConstraint->bmshopTabId))
		{
			OutBMShopTabIds.Add(SupplyInfoConstraint->bmshopTabId);
		}
	}

	return 0 < OutBMShopTabIds.Num();
}

bool FGsBMShopSupplyHelper::GetBMShopTabIdSupplyInfoConstraint(const FGsSchemaSupplyInfoConstraint* InSupplyInfoSet, OUT int32& OutBMShopTabId)
{
	if (nullptr == InSupplyInfoSet || 0 == InSupplyInfoSet->bmshopTabId)
	{
		return false;
	}

	OutBMShopTabId = InSupplyInfoSet->bmshopTabId;
	return true;
}
