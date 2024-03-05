#include "GsCurrencyHelper.h"

#include "DataSchema/GsSchemaCashCurrency.h"
#include "DataSchema/GsSchemaCurrencyData.h"
#include "DataSchema/Item/GsSchemaItemCommon.h"
#include "DataSchema/SupplyInfo/GsSchemaSupplyInfoConstraint.h"
#include "DataSchema/Tax/GsSchemaTaxConfig.h"
#include "Data/GsDataContainManager.h"
#include "GsCostPackage.h"
#include "Item/GsItemManager.h"
#include "Management/ScopeGame/GsSupplyManager.h"
#include "Management/ScopeGame/GsUtilizingContentsManager.h"
#include "Management/ScopeGlobal/GsGameDataManager.h"
#include "Management/ScopeGlobal/GsGuildManager.h"
#include "Management/ScopeGlobal/GsHiveManager.h"
#include "Management/ScopeGlobal/GsUIManager.h"
#include "Management/ScopeGame/GsSiegeWarfareManager.h"
#include "T1Project.h"
#include "UI/UIContent/Helper/GsUIHelper.h"
#include "UI/UIContent/Popup/GsUIPopupCommonPayLack.h"
#include "GsCostPackageMultiple.h"
#include "Shared/Shared/SharedInclude/SharedFuncs.h"

FSoftObjectPath FGsCurrencyHelper::GetCashCurrencyIconPath()
{
	if (const UGsGlobalConstant* GlobalConstant = GData()->GetGlobalData())
	{
		const UGsTableCashCurrency* CashCurrencyTable = Cast<UGsTableCashCurrency>(FGsSchemaCashCurrency::GetStaticTable());
		const FGsSchemaCashCurrency* CashCurrency = nullptr;
		if (CashCurrencyTable->FindRowById(GlobalConstant->_defaultCurrencyCode, CashCurrency))
		{
			return CashCurrency->iconPath;
		}
	}

	return FSoftObjectPath();
}

Currency FGsCurrencyHelper::GetCurrencyAmount(CurrencyType InType)
{
	Currency myAmount = 0;

	FGsGameDataManager* dataMgr = GGameData();
	switch (InType)
	{
	case CurrencyType::GUILD_ASSET:
	{
		myAmount = GGuild()->GetGuildAssetAmount();
		break;
	}
	case CurrencyType::DIA:
	case CurrencyType::FREE_DIA:
	{
		myAmount = !GHive()->IsDisplayFreeDiaCountry() ? 
			dataMgr->GetCurrency(CurrencyType::DIA) + dataMgr->GetCurrency(CurrencyType::FREE_DIA) : dataMgr->GetCurrency(InType);		

		break;
	}
	default:
		myAmount = dataMgr->GetCurrency(InType);
		break;
	}

	return myAmount;
}

Currency FGsCurrencyHelper::GetItemCurrencyAmount(const int32 InItemTid, bool bCheckLock)
{
	if (0 >= InItemTid)
		return 0;

	ItemAmount ownedItemCount = GItem()->GetAllItemCountByTID(InItemTid, bCheckLock);
	return ownedItemCount;
}

Currency FGsCurrencyHelper::GetCurrencyMaxAmount(CurrencyType InType)
{
	if (CurrencyType::DIA == InType ||
		CurrencyType::FREE_DIA == InType ||
		CurrencyType::MILEAGE == InType ||
		CurrencyType::CASH_MILEAGE == InType)
	{
		return MAX_GEM;
	}

	return MAX_MONEY;
}

const FGsSchemaCurrencyData* FGsCurrencyHelper::GetCurrencyData(const CurrencyType InType)
{
	const UGsTableCurrencyData* DataTable = Cast<UGsTableCurrencyData>(FGsSchemaCurrencyData::GetStaticTable());
	if (nullptr == DataTable)
	{
		return nullptr;
	}

	const FGsSchemaCurrencyData* Row = nullptr;
	if (!DataTable->FindRowById(InType, Row))
	{
		GSLOG(Warning, TEXT("!DataTable->FindRowById(%d, Row)"), static_cast<uint8>(InType));
	}

	return Row;
}

FText FGsCurrencyHelper::GetCurrencyLocalizedText(const CurrencyType InType)
{
	if (const FGsSchemaCurrencyData* data = GetCurrencyData(InType))
	{
		return data->nameText;
	}

	GSLOG(Error, TEXT("[Currency] nullptr == data. InType: %d"), static_cast<uint8>(InType));
	return FText::FromString(CurrencyTypeEnumToDisplayKey(InType));
}

FSoftObjectPath FGsCurrencyHelper::GetCurrencyIconPath(const CurrencyType InType)
{
	if (CurrencyType::CASH == InType)
	{
		return FGsCurrencyHelper::GetCashCurrencyIconPath();
	}

	if (const FGsSchemaCurrencyData* data = GetCurrencyData(InType))
	{
		return data->iconPath;
	}

	return FSoftObjectPath();
}

void FGsCurrencyHelper::SetGuildAsset(CurrencyType InType, Currency InAmount, bool bSendMessage)
{
	// 개선: 그냥 GameDataManager쪽으로 통합하여 사용할지 고민
	if (CurrencyType::GUILD_ASSET == InType)
	{
		// 안에서 갱신 메세지 보냄
		GGuild()->SetGuildAsset(InAmount, bSendMessage);
	}
}

bool FGsCurrencyHelper::CheckCurrency(CurrencyType InType, Currency InRequireAmount, bool bInShowTicker)
{
	if (CurrencyType::CASH == InType)
	{
		return true;
	}

	Currency myAmount = GetCurrencyAmount(InType);
	if (InRequireAmount <= myAmount)
	{
		return true;
	}
	
	if (bInShowTicker)
	{
		if (const FGsSchemaCurrencyData* data = GetCurrencyData(InType))
		{

			// TEXT: {0}이/가 부족합니다.
			FText textFormat;
			if (FText::FindText(TEXT("UICommonText"), TEXT("Alert_Shortage"), textFormat))
			{
				FText textMsg = FText::Format(textFormat, data->nameText);
				FGsUIHelper::TraySectionMessageTicker(EGsNotiSection::EXTSYSTEM, textMsg);
			}
		}
	}

	return false;
}

bool FGsCurrencyHelper::CheckCurrency(uint32 InItemTid, Currency InRequireAmount, bool bInShowTicker)
{
	Currency myAmount = GetItemCurrencyAmount(InItemTid);
	if (InRequireAmount <= myAmount)
	{
		return true;
	}

	if (bInShowTicker)
	{
		if (const FGsSchemaItemCommon* tableData = GItem()->GetItemTableDataByTID(InItemTid))
		{
			// TEXT: {0}이/가 부족합니다.
			FText textFormat;
			if (FText::FindText(TEXT("UICommonText"), TEXT("Alert_Shortage"), textFormat))
			{
				FText textMsg = FText::Format(textFormat, tableData->name);
				FGsUIHelper::TraySectionMessageTicker(EGsNotiSection::EXTSYSTEM, textMsg);
			}
		}
	}

	return false;
}

FText FGsCurrencyHelper::MakeSupplyHyperLink(const CurrencyType InType)
{
	const UGsSupplyManager* SupplyManager = GSSupply();
	if (nullptr == SupplyManager)
	{
		return FText::GetEmpty();
	}

	return SupplyManager->MakeRichTextCurrencyType(InType);
}

FText FGsCurrencyHelper::MakeUtilizeHyperLink(const CurrencyType InType)
{
	const FGsUtilizingContentsManager* DemandManager = GSUtilizingContents();
	if (nullptr == DemandManager)
	{
		return FText::GetEmpty();
	}

	return DemandManager->MakeRichText(InType);
}

bool FGsCurrencyHelper::TryToNavigate(const CurrencyType InType)
{
	const UGsSupplyManager* supplyMgr = GSSupply();
	if (nullptr == supplyMgr)
	{
		return false;
	}

	const FGsSchemaCurrencyData* currencyData = GetCurrencyData(InType);
	if (nullptr == currencyData)
	{
		return false;
	}

	const FGsSchemaSupplyInfoConstraint* supplyInfo = currencyData->supplyInfoConstraint.GetRow();
	if (nullptr == supplyInfo)
	{
		return false;
	}

	// 현재는 BMShop으로만 처리 가능
	if (0 < supplyInfo->bmshopTabId)
	{
		if (UGsSupplyManager::EGsNavigationStatus::Movable ==
			supplyMgr->GetNavigationStatus(EGsUnlockContentsType::BMSHOP, InType))
		{
			return supplyMgr->TryToNavigateCurrencyType(EGsUnlockContentsType::BMSHOP, InType);
		}
	}

	return false;
}

bool FGsCurrencyHelper::TryToNavigate(const ItemId InItemId)
{
	if (const UGsSupplyManager* supplyMgr = GSSupply())
	{
		if (UGsSupplyManager::EGsNavigationStatus::Movable ==
			supplyMgr->GetNavigationStatus(EGsUnlockContentsType::BMSHOP, InItemId))
		{
			return supplyMgr->TryToNavigateItemId(EGsUnlockContentsType::BMSHOP, InItemId);
		}
	}

	return false;
}

bool FGsCurrencyHelper::CanTryToNavigate(const CurrencyType InType)
{
	if (const UGsSupplyManager* supplyMgr = GSSupply())
	{
		return UGsSupplyManager::EGsNavigationStatus::Movable ==
			supplyMgr->GetNavigationStatus(EGsUnlockContentsType::BMSHOP, InType);
	}

	return false;
}

bool FGsCurrencyHelper::CanTryToNavigate(const ItemId InItemId)
{
	if (const UGsSupplyManager* supplyMgr = GSSupply())
	{
		return UGsSupplyManager::EGsNavigationStatus::Movable ==
			supplyMgr->GetNavigationStatus(EGsUnlockContentsType::BMSHOP, InItemId);
	}

	return false;
}

void FGsCurrencyHelper::OpenLackCurrencyPopup(CurrencyType InType, Currency InLackAmount, 
	bool bInUseLink, TFunction<void(bool)> InCallback)
{
	TWeakObjectPtr<UGsUIWidgetBase> widget = GUI()->OpenAndGetWidget(TEXT("PopupCommonPayLack"));
	if (widget.IsValid())
	{
		if (UGsUIPopupCommonPayLack* popup = Cast<UGsUIPopupCommonPayLack>(widget.Get()))
		{
			popup->SetData(InType, InLackAmount, bInUseLink, InCallback);
		}
	}
}

void FGsCurrencyHelper::OpenLackItemPopup(ItemId InItemId, ItemAmount InLackAmount, 
	bool bInUseLink, TFunction<void(bool)> InCallback)
{
	TWeakObjectPtr<UGsUIWidgetBase> widget = GUI()->OpenAndGetWidget(TEXT("PopupCommonPayLack"));
	if (widget.IsValid())
	{
		if (UGsUIPopupCommonPayLack* popup = Cast<UGsUIPopupCommonPayLack>(widget.Get()))
		{
			popup->SetItemData(InItemId, InLackAmount, bInUseLink, InCallback);
		}
	}
}

bool FGsCurrencyHelper::OpenLackCostElementPopup(const FGsCostElement& InCostElement, bool bInUseLink /*= true*/, TFunction<void(bool)> InCallback /*= nullptr*/)
{
	if (CostType::CURRENCY == InCostElement._costType)
	{
		FGsCurrencyHelper::OpenLackCurrencyPopup(InCostElement.GetCurrencyType(), InCostElement.GetCostLackAmount(), bInUseLink, InCallback);
		return true;
	}
	else if (CostType::ITEM == InCostElement._costType)
	{
		FGsCurrencyHelper::OpenLackItemPopup(InCostElement.GetItemId(), InCostElement.GetCostLackAmount(), bInUseLink, InCallback);
		return true;
	}

	GSLOG(Error, TEXT("InCostElement._costType has not valid value."));
	return false;
}

bool FGsCurrencyHelper::OpenLackCostPackagePopup(const FGsCostPackage& InCostPackage, bool bInUseLink /*= true*/, TFunction<void(bool)> InCallback /*= nullptr*/)
{
	const FGsCostElement* CostElements = InCostPackage.GetFirstCostElementNotEnough();
	if (nullptr == CostElements)
	{
		GSLOG(Warning, TEXT("Could not find any not enough cost element. nullptr == CostElements"));
		return false;
	}

	return FGsCurrencyHelper::OpenLackCostElementPopup(*CostElements, bInUseLink, InCallback);
}

bool FGsCurrencyHelper::OpenLackCostPackageMultiplePopup(const FGsCostPackageMultiple& InCostPackageMultiple, bool bInUseLink /*= true*/, TFunction<void(bool)> InCallback /*= nullptr*/)
{
	const FGsCostElement* CostElements = InCostPackageMultiple.GetFirstCostElementNotEnough();
	if (nullptr == CostElements)
	{
		GSLOG(Warning, TEXT("Could not find any not enough cost element. nullptr == CostElements"));
		return false;
	}

	return FGsCurrencyHelper::OpenLackCostElementPopup(*CostElements, bInUseLink, InCallback);
}

const FGsSchemaTaxConfig* FGsCurrencyHelper::GetTaxConfig()
{
	// 1개 열밖에 없음.
	const FGsSchemaTaxConfig* row = nullptr;
	if (const UGsTable* table = FGsSchemaTaxConfig::GetStaticTable())
	{
		if (table->FindRow(TEXT("0"), row))
		{
			return row;
		}
	}

#if WITH_EDITOR
	GSLOG(Error, TEXT("[Guild] Fail to get FGsSchemaTaxConfig."));
#endif

	return nullptr;
}

int32 FGsCurrencyHelper::GetTotalTaxPercent(CurrencyType InType)
{
	if (FGsSiegeWarfareManager* siegeWarfareMgr = GSSiegeWarfare())
	{
		if (FGsTaxDataHandler* taxHandler = siegeWarfareMgr->GetTaxHandler())
		{
			return taxHandler->GetTotalTaxPercent(InType);
		}
	}

	return 0;
}

Currency FGsCurrencyHelper::GetTaxIncludedPrice(CurrencyType InType, Currency InAmount)
{
	int32 totalTaxPercent = GetTotalTaxPercent(InType);
	if (0 == totalTaxPercent)
	{
		return InAmount;
	}

	return SharedFunc::GetTaxIncludedPrice(totalTaxPercent, InAmount);
}

FText FGsCurrencyHelper::GetTextTotalTax(CurrencyType InType)
{
	// TEXT: {0}%
	FText textFormat;
	if (FText::FindText(TEXT("UICommonText"), TEXT("NPercent"), textFormat))
	{
		return FText::Format(textFormat, GetTotalTaxPercent(InType));
	}

	return FText::GetEmpty();
}

FText FGsCurrencyHelper::GetTextTaxInfo(CurrencyType InType)
{
	FGsTaxDataHandler* taxHandler = nullptr;
	if (FGsSiegeWarfareManager* siegeWarfareMgr = GSSiegeWarfare())
	{
		taxHandler = siegeWarfareMgr->GetTaxHandler();
	}

	if (nullptr == taxHandler)
	{
		return FText::GetEmpty();
	}

	int32 systemTexPercent = taxHandler->GetTaxPercent(InType, true);
	int32 defaultTexPercent = taxHandler->GetTaxPercent(InType, false);
	int32 castleTexPercent = taxHandler->GetCastleTaxPercent();

	FString strTemp;
	if (0 < systemTexPercent)
	{
		// TEXT: 기본세율 {0}%
		FText textFormat;
		if (FText::FindText(TEXT("SiegeWarfareText"), TEXT("UI_TaxRateInfo_SystemTaxRate"), textFormat))
		{
			FText textTemp = FText::Format(textFormat, systemTexPercent);
			strTemp.Append(textTemp.ToString());
		}
	}

	if (0 < defaultTexPercent)
	{
		if (false == strTemp.IsEmpty())
		{
			strTemp.Append(TEXT(" + "));
		}

		// TEXT: 전쟁세율 {0}%
		FText textFormat;
		if (FText::FindText(TEXT("SiegeWarfareText"), TEXT("UI_TaxRateInfo_DefaultTaxRate"), textFormat))
		{
			FText textTemp = FText::Format(textFormat, defaultTexPercent);
			strTemp.Append(textTemp.ToString());
		}
	}

	if (0 < castleTexPercent)
	{
		if (false == strTemp.IsEmpty())
		{
			strTemp.Append(TEXT(" + "));
		}

		// TEXT: 성세율 {0}%
		FText textFormat;
		if (FText::FindText(TEXT("SiegeWarfareText"), TEXT("UI_TaxRateInfo_CastleTaxRate"), textFormat))
		{
			FText textTemp = FText::Format(textFormat, castleTexPercent);
			strTemp.Append(textTemp.ToString());
		}
	}

	if (strTemp.IsEmpty())
	{
		return FText::GetEmpty();
	}

	FString strOut = FString::Format(TEXT("({0})"), { *strTemp });
	return FText::FromString(strOut);
}

bool FGsCurrencyHelper::IsTaxNpcType(NpcFunctionType InType)
{
	if (const FGsSchemaTaxConfig* taxConfig = GetTaxConfig())
	{
		return taxConfig->taxNpcList.Contains(InType);
	}

	return false;
}
