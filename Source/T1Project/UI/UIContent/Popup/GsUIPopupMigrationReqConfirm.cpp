#include "GsUIPopupMigrationReqConfirm.h"
#include "UI/UIContent/Common/GsUICurrencySlot.h"
#include "UI/UIControlLib/ContentWidget/GsButton.h"
#include "../Management/ScopeGlobal/GsBMShopManager.h"
#include "../Currency/GsCurrencyHelper.h"

void UGsUIPopupMigrationReqConfirm::NativeOnInitialized()
{
	Super::NativeOnInitialized();
}

void UGsUIPopupMigrationReqConfirm::BeginDestroy()
{
	Super::BeginDestroy();
}

void UGsUIPopupMigrationReqConfirm::SetInit(InitParam* param, TFunction<void()> okCallback)
{
	_okCallback = okCallback;

	if (param)
	{
		if (param->type == ServerMigrateType::ACCOUNT_ONLY)
		{
			FText::FindText(TEXT("ServerMigrateText"), TEXT("ServerMigrate_Popup_Title_Account"), _textTitle);
			FText::FindText(TEXT("ServerMigrateText"), TEXT("ServerMigrate_Popup_Caution_Account"), _textDesc);

			if (FGsServerMigrateHandler* handler = (GBMShop()) ? GBMShop()->GetServerMigrateHandler() : nullptr)
			{
				FText textDesc;
				FText::FindText(TEXT("ServerMigrateText"), TEXT("ServerMigrate_UiNotice_Expired"), textDesc);
				_textRemainTime = FText::Format(textDesc, handler->GetTextRemainingTime());

				FText textFormat;
				FText::FindText(TEXT("ServerMigrateText"), TEXT("ServerMigrate_UiNotice_Buyable_Count"), textFormat);
				_textMigrateCount = FText::FromString(FString::Format(*textFormat.ToString(), 
					{ handler->CountInfo().Key, handler->CountInfo().Value }));

				const auto& currencyPair = handler->GetCurrencyInfo(ServerMigrateType::ACCOUNT_ONLY);
				auto currentCurrency = FGsCurrencyHelper::GetCurrencyAmount(currencyPair.mCurrencyType);

				_needCurrency->SetData(currencyPair.mCurrencyType, currencyPair.mCurrency);
				_currentCurrency->SetData(currencyPair.mCurrencyType, currentCurrency);
			}
		}
		else
		{

			FText::FindText(TEXT("ServerMigrateText"), TEXT("ServerMigrate_Popup_Title_Guild"), _textTitle);
			FText::FindText(TEXT("ServerMigrateText"), TEXT("ServerMigrate_Popup_Caution_Guild"), _textDesc);

			if (FGsServerMigrateHandler* handler = (GBMShop()) ? GBMShop()->GetServerMigrateHandler() : nullptr)
			{
				FText textDesc;
				FText::FindText(TEXT("ServerMigrateText"), TEXT("ServerMigrate_UiNotice_Expired"), textDesc);
				_textRemainTime = FText::Format(textDesc, handler->GetTextRemainingTime());

				FText textFormat;
				FText::FindText(TEXT("ServerMigrateText"), TEXT("ServerMigrate_UiNotice_Buyable_Count"), textFormat);
				_textMigrateCount = FText::FromString(FString::Format(*textFormat.ToString(),
					{ handler->CountInfo().Key, handler->CountInfo().Value }));

				const auto& currencyPair = handler->GetCurrencyInfo(ServerMigrateType::INCLUDE_GUILD_INFO);
				auto currentCurrency = FGsCurrencyHelper::GetCurrencyAmount(currencyPair.mCurrencyType);

				_needCurrency->SetData(currencyPair.mCurrencyType, currencyPair.mCurrency);
				_currentCurrency->SetData(currencyPair.mCurrencyType, currentCurrency);
			}
		}
	}
}

void UGsUIPopupMigrationReqConfirm::OnClickConfirm()
{
	if (_okCallback) _okCallback();

	Close();
}

void UGsUIPopupMigrationReqConfirm::OnClickCancel()
{
	Close();
}

void UGsUIPopupMigrationReqConfirm::OnInputCancel()
{
	OnClickCancel();
}

void UGsUIPopupMigrationReqConfirm::OnInputOk()
{
	OnClickConfirm();
}
