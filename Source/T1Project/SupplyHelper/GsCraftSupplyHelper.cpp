#include "GsCraftSupplyHelper.h"

#include "DataSchema/GsSchemaEnums.h"
#include "DataSchema/SupplyInfo/GsSchemaCraftSupplyInfo.h"
#include "DataSchema/SupplyInfo/GsSchemaCurrencySupplyInfoSet.h"
#include "DataSchema/SupplyInfo/GsSchemaItemSupplyInfoSet.h"
#include "DataSchema/SupplyInfo/GsSchemaSkillSupplyInfoSet.h"
#include "GsBaseSupplyHelper.h"
#include "Management/GsMessageHolder.h"
#include "Management/GsScopeGame.h"
#include "Management/GsScopeHolder.h"
#include "Management/ScopeGame/GsCraftManager.h"
#include "Management/ScopeGame/GsSupplyManager.h"
#include "Management/ScopeGame/GsUnlockManager.h"
#include "Message/GsMessageContents.h"
#include "Message/MessageParam/GsCraftMessageParam.h"
#include "Shared/Client/SharedEnums/SharedCommonEnum.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "Unlock/GsUnlockDefine.h"

FGsCraftSupplyHelper::FGsCraftSupplyHelper()
	: FGsBaseSupplyHelper(EGsUnlockContentsType::CRAFT)
{
}

FText FGsCraftSupplyHelper::GetTitleText() const
{
	return NSLOCTEXT("ItemSupplyInfoText", "SupplyInfo_Craft", "제작");
}

UGsSupplyManager::EGsNavigationStatus FGsCraftSupplyHelper::GetNavigationStatus(const FGsSchemaCurrencySupplyInfoSet& InSupplyInfoSet) const
{
	return FGsCraftSupplyHelper::GetNavigationStatus(InSupplyInfoSet.craftIdSet);
}

bool FGsCraftSupplyHelper::TryToNavigate(const FGsSchemaCurrencySupplyInfoSet& InSupplyInfoSet, OUT FText* OutFailMessage /*= nullptr*/) const
{
	return FGsCraftSupplyHelper::TryToNavigate(InSupplyInfoSet.craftIdSet, OutFailMessage);
}

UGsSupplyManager::EGsNavigationStatus FGsCraftSupplyHelper::GetNavigationStatus(const FGsSchemaItemSupplyInfoSet& InSupplyInfoSet) const
{
	return FGsCraftSupplyHelper::GetNavigationStatus(InSupplyInfoSet.craftIdSet);
}

bool FGsCraftSupplyHelper::TryToNavigate(const FGsSchemaItemSupplyInfoSet& InSupplyInfoSet, OUT FText* OutFailMessage /*= nullptr*/) const
{
	return FGsCraftSupplyHelper::TryToNavigate(InSupplyInfoSet.craftIdSet, OutFailMessage);
}

UGsSupplyManager::EGsNavigationStatus FGsCraftSupplyHelper::GetNavigationStatus(const FGsSchemaSkillSupplyInfoSet& InSupplyInfoSet) const
{
	return FGsCraftSupplyHelper::GetNavigationStatus(InSupplyInfoSet.craftIdSet);
}

bool FGsCraftSupplyHelper::TryToNavigate(const FGsSchemaSkillSupplyInfoSet& InSupplyInfoSet, OUT FText* OutFailMessage /*= nullptr*/) const
{
	return FGsCraftSupplyHelper::TryToNavigate(InSupplyInfoSet.craftIdSet, OutFailMessage);
}

static bool SortBestCraftFirst(const int32& InCraftIdA, const int32& InCraftIdB)
{
	const UGsCraftManager* CraftManager = GSCraft();
	if (nullptr == CraftManager)
	{
		return false;
	}

	const bool CanDisplayA = CraftManager->CanDisplay(InCraftIdA);
	const bool CanDisplayB = CraftManager->CanDisplay(InCraftIdB);
	if (CanDisplayA != CanDisplayB)
	{
		return CanDisplayA;
	}

	return false;
}

CraftId FGsCraftSupplyHelper::FindBestCraftId(const TSet<int32>& InCraftIds)
{
	if (0 == InCraftIds.Num())
	{
		return INVALID_CRAFT_ID;
	}

	TArray<int32> CraftIds = InCraftIds.Array();
	CraftIds.StableSort(&SortBestCraftFirst);

	return StaticCast<CraftId>(CraftIds[0]);
}

UGsSupplyManager::EGsNavigationStatus FGsCraftSupplyHelper::GetNavigationStatus(const TSet<int32>& InCraftIds)
{
	const UGsCraftManager* CraftManager = GSCraft();
	if (nullptr == CraftManager)
	{
		return UGsSupplyManager::EGsNavigationStatus::None;
	}

	const CraftId BestCraftId = FGsCraftSupplyHelper::FindBestCraftId(InCraftIds);
	if (INVALID_CRAFT_ID == BestCraftId)
	{
		return UGsSupplyManager::EGsNavigationStatus::None;
	}

	return CraftManager->CanDisplay(BestCraftId) ? UGsSupplyManager::EGsNavigationStatus::Movable : UGsSupplyManager::EGsNavigationStatus::None;
}

bool FGsCraftSupplyHelper::TryToNavigate(const TSet<int32>& InCraftIds, OUT FText* OutFailMessage /*= nullptr*/)
{
	const UGsCraftManager* CraftManager = GSCraft();
	if (nullptr == CraftManager)
	{
		return false;
	}

	const CraftId BestCraftId = FGsCraftSupplyHelper::FindBestCraftId(InCraftIds);
	if (INVALID_CRAFT_ID == BestCraftId)
	{
		if (nullptr != OutFailMessage)
		{
			*OutFailMessage = NSLOCTEXT("CraftText", "ItIsNotACrafTargetItem", "제작할 수 없는 아이템입니다.");
		}
		return false;
	}

	return CraftManager->TryToNavigate(BestCraftId, OutFailMessage);
}
