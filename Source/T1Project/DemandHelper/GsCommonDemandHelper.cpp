#include "GsCommonDemandHelper.h"

#include "DataSchema/DemandInfo/GsSchemaCurrencyDemandInfoSet.h"
#include "DataSchema/DemandInfo/GsSchemaItemDemandInfoSet.h"
#include "DataSchema/GsSchemaCurrencyData.h"
#include "DataSchema/GsSchemaEnums.h"
#include "DataSchema/Item/GsSchemaItemCommon.h"
#include "GsBaseDemandHelper.h"
#include "GsSchemaRowBase.h"
#include "Management/GsMessageHolder.h"
#include "Management/GsScopeGame.h"
#include "Management/GsScopeHolder.h"
#include "Management/ScopeGame/GsSupplyManager.h"
#include "Management/ScopeGame/GsUnlockManager.h"
#include "Message/GsMessageContents.h"
#include "Message/MessageParam/GsCraftMessageParam.h"
#include "Shared/Client/SharedEnums/SharedCommonEnum.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "T1Project.h"
#include "Unlock/GsUnlockDefine.h"

FGsCommonDemandHelper::FGsCommonDemandHelper(const EGsUnlockContentsType InContentsType)
	: Super(InContentsType)
{
}

UGsSupplyManager::EGsNavigationStatus FGsCommonDemandHelper::GetNavigationStatus(const FGsSchemaCurrencyDemandInfoSet& InDemandInfo) const
{
	if (!InDemandInfo.contentsTypeSet.Contains(_contentsType))
	{
		return UGsSupplyManager::EGsNavigationStatus::None;
	}

	return FGsBaseDemandHelper::GetNavigationStatusStatic(_contentsType, InDemandInfo.currencyType);
}

bool FGsCommonDemandHelper::TryToNavigate(const FGsSchemaCurrencyDemandInfoSet& InCurrencyDemandInfoSet, OUT FText* OutFailMessage /*= nullptr*/) const
{
	return FGsCommonDemandHelper::TryToNavigate(_contentsType, OutFailMessage);
}

UGsSupplyManager::EGsNavigationStatus FGsCommonDemandHelper::GetNavigationStatus(const FGsSchemaItemDemandInfoSet& InDemandInfo) const
{
	if (!InDemandInfo.contentsTypeSet.Contains(_contentsType))
	{
		return UGsSupplyManager::EGsNavigationStatus::None;
	}

	return FGsBaseDemandHelper::GetNavigationStatusStatic(_contentsType, InDemandInfo.itemId);
}

bool FGsCommonDemandHelper::TryToNavigate(const FGsSchemaItemDemandInfoSet& InItemDemandInfoSet, OUT FText* OutFailMessage /*= nullptr*/) const
{
	return FGsCommonDemandHelper::TryToNavigate(_contentsType, OutFailMessage);
}

bool FGsCommonDemandHelper::TryToNavigate(const EGsUnlockContentsType InContentsType, OUT FText* OutFailMessage)
{
	FGsUnlockManager* UnlockManager = GSUnlock();
	if (nullptr == UnlockManager || !UnlockManager->IsContentsUnlock(InContentsType))
	{
		if (nullptr != OutFailMessage)
		{
			UnlockManager->GetUnLockMessage(InContentsType, *OutFailMessage);
		}
		return false;
	}

	// TODO: 컨텐츠오프너로 이전 필요
	switch (InContentsType)
	{
	case EGsUnlockContentsType::BMSHOP:
	{
		GMessage()->GetContents().SendMessage(MessageContents::ACTIVE_BMSHOP);
		return true;
	}

	case EGsUnlockContentsType::SKILL:
	{
		GMessage()->GetContents().SendMessage(MessageContents::ACTIVE_WINDOW_SKILL);
		return true;
	}

	case EGsUnlockContentsType::CRAFT:
	{
		FGsCraftMessageParamOpenCraftWith(INVALID_CRAFT_ID).Send();
		return true;
	}

	case EGsUnlockContentsType::COSTUME:
	{
		GMessage()->GetContents().SendMessage(MessageContents::ACTIVE_WINDOW_COSTUME);
		return true;
	}

	case EGsUnlockContentsType::FAIRY:
	{
		GMessage()->GetContents().SendMessage(MessageContents::ACTIVE_WINDOW_FAIRY);
		return true;
	}

	case EGsUnlockContentsType::MONSTER_KNOWLEDGE_COLLECTION:
	{
		GMessage()->GetContents().SendMessage(MessageContents::ACTIVE_WINDOW_MONSTER_COLLECTION);
		return true;
	}

	case EGsUnlockContentsType::WORLDMAP:
	{
		GMessage()->GetContents().SendMessage(MessageContents::ACTIVE_WINDOW_REGIONMAP);
		return true;
	}

	case EGsUnlockContentsType::SEAL:
	{
		GMessage()->GetContents().SendMessage(MessageContents::ACTIVE_WINDOW_SEAL);
		return true;
	}
	}

	return false;
}
