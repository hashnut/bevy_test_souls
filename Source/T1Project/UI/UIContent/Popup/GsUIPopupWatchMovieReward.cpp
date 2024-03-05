// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUIPopupWatchMovieReward.h"
#include "UI/UIControlLib/ContentWidget/GsButton.h"
#include "Sequence/GsSequenceManager.h"
#include "Sequence/GsSequencePlayer.h"
#include "Reward/GsRewardHelper.h"
#include "UI/UIContent/Common/GsUICurrencySlot.h"
#include "UMG/Public/Components/TextBlock.h"
#include "UI/UIContent/Helper/GsUIStringHelper.h"
#include "UMG/Public/Components/WidgetSwitcher.h"
#include "UI/UIControlLib/ContentWidget/GsCheckBox.h"
#include "Option/GsServerOption.h"


void UGsUIPopupWatchMovieReward::NativeDestruct()
{
	_buttonCallback = nullptr;

	Super::NativeDestruct();
}

void UGsUIPopupWatchMovieReward::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	_btnOk->OnClicked.AddDynamic(this, &UGsUIPopupWatchMovieReward::OnClickOk);
	_btnCancel->OnClicked.AddDynamic(this, &UGsUIPopupWatchMovieReward::OnClickCancel);

	_checkBoxEditable->OnCheckStateChangedEx.AddDynamic(this, &UGsUIPopupWatchMovieReward::OnChangeCheckBox);
}

void UGsUIPopupWatchMovieReward::OnInputOk()
{
	OnClickOk();
}

void UGsUIPopupWatchMovieReward::OnInputCancel()
{
	OnClickClose();
}

void UGsUIPopupWatchMovieReward::OnClickOk()
{
	if (nullptr != _buttonCallback)
	{
		_buttonCallback(true);
	}

	Close();
}

void UGsUIPopupWatchMovieReward::OnClickCancel()
{
	OnClickClose();
}

void UGsUIPopupWatchMovieReward::OnClickClose()
{
	if (nullptr != _buttonCallback)
	{
		_buttonCallback(false);
	}

	Close();
}

void UGsUIPopupWatchMovieReward::SetData(int32 inSequenceResId, TFunction<void(bool)> InCallback)
{
	_buttonCallback = InCallback;

	const FGsSchemaSequenceResData*  resData = GSequencePlayer()->GetSequenceResData(inSequenceResId);
	if (nullptr == resData)
	{
		OnClickOk();
		return;
	}

	const FGsSchemaRewardData* rewardData = resData->viewRewardId.GetRow();
	if (nullptr == rewardData)
	{
		OnClickOk();
		return;
	}

	FGsRewardUnboxingData data;
	FGsRewardHelper::UnboxingRewardData(rewardData->id, data);

	if (0 < data.GetCommonExp())
	{
		_switcherRewardType->SetActiveWidgetIndex(0);

		FText txt;
		FGsUIStringHelper::GetAmountText(data.GetCommonExp(), MAX_uint64, txt);
		_expTextBlock->SetText(txt);
	}
	else
	{
		_switcherRewardType->SetActiveWidgetIndex(1);

		_currencySlot->SetData(data._currencyType, data.GetCommonCurrencyAmount());
	}
	
	if (_checkBoxEditable)
	{
		_checkBoxEditable->SetIsSelected(false);
	}
}

void UGsUIPopupWatchMovieReward::OnChangeCheckBox(int32 IndexInGroup, bool bIsChecked)
{
	GServerOption()->SetIsIgnoreCinematicRewardPopup(bIsChecked);
}