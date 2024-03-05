
#include "GsUIPopupPKBookTryToActionBase.h"
#include "../PKBook/GsPKBookData.h"
#include "../Management/ScopeGame/GsPKBookManager.h"
#include "../Management/ScopeGlobal/GsUIManager.h"
#include "../Management/ScopeGlobal/GsGameDataManager.h"

#include "../Currency/GsCurrencyHelper.h"

#include "TextBlock.h"
#include "../Common/GsUICurrencySlot.h"
#include "GsUIPopupCommonPayLack.h"
#include "UI/UIContent/Define/EGsUIColorType.h"
#include "../UI/UIControlLib/ContentWidget/GsButton.h"
#include "../Net/GsNetSendServiceWorld.h"
#include "../T1Project.h"



void UGsUIPopupPKBookTryToActionBase::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	_btnClose->OnClicked.AddDynamic(this, &UGsUIPopupPKBookTryToActionBase::OnClickedClose);
	_btnOK->OnClicked.AddDynamic(this, &UGsUIPopupPKBookTryToActionBase::OnClickedOK);
}

void UGsUIPopupPKBookTryToActionBase::BeginDestroy()
{
	if (_btnClose)
	{
		_btnClose->OnClicked.RemoveDynamic(this, &UGsUIPopupPKBookTryToActionBase::OnClickedClose);
		_btnClose = nullptr;
	}

	if (_btnOK)
	{
		_btnOK->OnClicked.RemoveDynamic(this, &UGsUIPopupPKBookTryToActionBase::OnClickedOK);
		_btnOK = nullptr;
	}

	Super::BeginDestroy();
}

void UGsUIPopupPKBookTryToActionBase::TryToAction(PKRecordId InRecordId)
{
	Close();
}

void UGsUIPopupPKBookTryToActionBase::OnClickedClose()
{
	Close();
}

void UGsUIPopupPKBookTryToActionBase::OnClickedOK()
{
	Currency amountHave = FGsCurrencyHelper::GetCurrencyAmount(_currencyType);

	if (_amountCost > amountHave)
	{
		UGsUIManager* uiManager = GUI();
		if (uiManager)
		{
			uiManager->CloseByKeyName(TEXT("PopupPKBookTryToTease"), true);
		}

		// 2022/09/27 PKT - 재화 부족 팝업 띄움.
		const uint64 needCost = _amountCost - amountHave;
		FGsCurrencyHelper::OpenLackCurrencyPopup(_currencyType, needCost);
	}
	else
	{
		TryToAction(_recordId);
	}
}

void UGsUIPopupPKBookTryToActionBase::SetData(PKRecordId InRecordId, const struct FGsPKRecordingContributorData* InData)
{
	_recordId = INVALID_PK_RECORD_ID;
	_currencyType = CurrencyType::NONE;
	_amountCost = 0;

	if (INVALID_PK_RECORD_ID == InRecordId || nullptr == InData)
	{
		GSLOG(Error, TEXT("INVALID_PK_RECORD_ID == InRecordId || nullptr == InData"));
		return;
	}

	FGsPKBookManager* PKBookManager = GsPKBook();
	if (nullptr == PKBookManager)
	{
		GSLOG(Error, TEXT("nullptr == PKBookManager"));
		return;
	}

	_recordId = InRecordId;

	// 2022/09/28 PKT - Action Cost
	PKBookManager->ActionCostInfo(InData->_reason, _currencyType, _amountCost);

	// 2022/09/27 PKT - 소유 금액
	Currency amountHave = FGsCurrencyHelper::GetCurrencyAmount(_currencyType);
	_currencySlotHave->SetData(_currencyType, amountHave);

	// 2022/09/27 PKT - 지불 금액
	_currencySlotPay->SetData(_currencyType, _amountCost);
	// 2022/10/21 PKT - 지불금액이 충분한가?
	_currencySlotPay->SetAmountTextColor((_amountCost <= amountHave) ? EGsUIColorType::DEFAULT_ENOUGH : EGsUIColorType::DEFAULT_LACK);
}

void UGsUIPopupPKBookTryToActionBase::OnInputCancel()
{
	OnClickedClose();
}