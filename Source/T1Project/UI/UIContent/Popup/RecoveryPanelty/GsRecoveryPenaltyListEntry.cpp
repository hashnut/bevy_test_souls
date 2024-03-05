// Fill out your copyright notice in the Description page of Project Settings.


#include "GsRecoveryPenaltyListEntry.h"
#include "WidgetSwitcher.h"
#include "TextBlock.h"
#include "Image.h"
#include "Widget.h"
#include "UI/UIContent/Common/Icon/GsUIIconItemInventory.h"
#include "UI/UIControlLib/Define/GsIconDefine.h"
#include "UI/UIControlLib/Control/GsDynamicIconSlotHelper.h"
#include "UI/UIControlLib/ContentWidget/GsHorizontalBoxIconSelector.h"
#include "UI/UIControlLib/ContentWidget/GsButton.h"
#include "UI/UILib/Define/GsUIDefine.h"
#include "UTIL/GsTableUtil.h"
#include "Item/GsItemManager.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "Management/ScopeGlobal/GsGameDataManager.h"
#include "GsUIPopupRecoveryPenalty.h"
#include "Misc/Timespan.h"
#include "DataSchema/User/Level/GsSchemaLevelRecoveryExpTable.h"
#include "UTIL/GsTableUtil.h"

const FTextKey REMAIN_RECOVERY_TIME_KEY = TEXT("RemainRecoveryTime");

void UGsRecoveryPenaltyListEntry::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	_btnSlot->OnClicked.AddDynamic(this, &UGsRecoveryPenaltyListEntry::OnSelectPenalty);
}

void UGsRecoveryPenaltyListEntry::NativeConstruct()
{
	Super::NativeConstruct();

	FText::FindText(RESURRECT_TEXT_KEY, REMAIN_RECOVERY_TIME_KEY, _timeFormat);
	FText::FindText(RESURRECT_TEXT_KEY, LOSS_EXP_AMOUNT_TEXT_KEY, _lossExpAmountFormat);
}

void UGsRecoveryPenaltyListEntry::NativeDestruct()
{
	_iconSelector->ClearChildren();
	_iconSelector->RemoveAllChildren();

	Super::NativeDestruct();
}

void UGsRecoveryPenaltyListEntry::NativeOnListItemObjectSet(UObject* inListItemObject)
{
	UGsRecoveryPenaltyListEntryParam* param = Cast<UGsRecoveryPenaltyListEntryParam>(inListItemObject);
	if (nullptr == param)
	{
		return;
	}

	_expPenalty = param->_expPenalty;
	_itemPenalty = param->_itemPenalty;
	_parents = param->_parents;

	InvalidIcon();
	InvalidDetailInfo();
	InvalidCurrency();
	InvalidTime();
	InvalidSelect();
}

void UGsRecoveryPenaltyListEntry::InvalidIcon()
{
	switch (_parents.Get()->GetPenaltyType())
	{
	case EGsPenaltyType::Exp:
	{
		_iconSelector->SetVisibility(ESlateVisibility::Hidden);
		_expImg->SetVisibility(ESlateVisibility::Visible);
	}
	break;
	case EGsPenaltyType::Item:
	{
		_iconSelector->SetVisibility(ESlateVisibility::Visible);
		_expImg->SetVisibility(ESlateVisibility::Hidden);
		_iconSelector->RemoveAllChildren();

		//create item icon			
		_uiIcon = Cast<UGsUIIconItemInventory>(_iconSelector->CreateIcon());
		_iconSelector->AddChild(_uiIcon);
		if (_uiIcon)
		{
			_uiIcon->SetNotOwnItem(_itemPenalty.mDeathPenaltyItem.mTemplateId, 1);
			_uiIcon->SetItemEnchantLevel(_itemPenalty.mDeathPenaltyItem.mLevel);
			_uiIcon->SetSlotEquipedImage(false);
			_uiIcon->SetPresetImage(false);
			_uiIcon->SetItemNameVisibility(false);
			_uiIcon->OnFinishedLongPressEvent.RemoveAll(this);
			_uiIcon->OnFinishedLongPressEvent.AddUObject(this, &UGsRecoveryPenaltyListEntry::OnLongPressIcon);
		}
	}
	break;
	}
}

void UGsRecoveryPenaltyListEntry::InvalidCurrency()
{
	if (false == _parents.IsValid())
	{
		return;
	}

	EGsPenaltyCurrencyType currency = UGsUIPopupRecoveryPenalty::GetCurrencyType(_parents->GetCurrency());
	if (_condencyIconSwitcher)
	{
		_condencyIconSwitcher->SetActiveWidgetIndex((int32)currency);
	}

	InvalidRecoveryCost();
}

void UGsRecoveryPenaltyListEntry::InvalidRecoveryCost()
{
	if (nullptr == _costText)
	{
		return;
	}

	int32 cost = 0;

	switch (_parents->GetPenaltyType())
	{
	case EGsPenaltyType::Exp:
	{
		cost = GetCost(_expPenalty, _parents->GetCurrency());
	}
	break;
	case EGsPenaltyType::Item:
	{
		cost = GetCost(_itemPenalty, _parents->GetCurrency());
	}
	break;
	}

	_costText->SetText(FText::AsNumber(cost));
}

void UGsRecoveryPenaltyListEntry::InvalidSelect()
{
	switch (_parents->GetPenaltyType())
	{
	case EGsPenaltyType::Exp:
	{
		const DeathPenaltyExp* find = _parents->GetExpPenalty().FindByPredicate([=](const DeathPenaltyExp& penalty)->bool {
			return _expPenalty.mDeathPenaltyId == penalty.mDeathPenaltyId;
			});

		if (find)
		{
			_selectImg->SetVisibility(ESlateVisibility::Visible);
		}
		else
		{
			_selectImg->SetVisibility(ESlateVisibility::Hidden);
		}
	}
	break;
	case EGsPenaltyType::Item:
	{
		const DeathPenaltyItem* find = _parents->GetItemPenalty().FindByPredicate([=](const DeathPenaltyItem& penalty)->bool {
			return _itemPenalty.mDeathPenaltyId == penalty.mDeathPenaltyId;
			});

		if (find)
		{
			_selectImg->SetVisibility(ESlateVisibility::Visible);
		}
		else
		{
			_selectImg->SetVisibility(ESlateVisibility::Hidden);
		}
	}
	break;
	}
}

int32 UGsRecoveryPenaltyListEntry::GetCost(const DeathPenaltyItem& inPenalty, const CurrencyType inCurrency)
{
	int32 cost = 0;
	ItemId  itemId = inPenalty.mDeathPenaltyItem.mTemplateId;
	const FGsSchemaItemCommon* item = UGsTableUtil::FindRowById<UGsTableItemCommon, FGsSchemaItemCommon>(itemId);
	if (item)
	{
		const FGsSchemaItemEquipDetail* equipItem = UGsTableUtil::FindRowById<UGsTableItemEquipDetail, FGsSchemaItemEquipDetail>(item->detailId);
		if (equipItem)
		{
			switch (inCurrency)
			{
			case CurrencyType::DIA:
			{
				cost = equipItem->diaRecoveryCost;
			}
			break;
			case CurrencyType::GOLD:
			{
				cost = equipItem->goldRecoveryCost;
			}
			break;
			}
		}
	}

	return cost;
}

int32 UGsRecoveryPenaltyListEntry::GetCost(const DeathPenaltyExp& inPenalty, const CurrencyType inCurrency)
{
	int32 cost = 0;
	int32 level = inPenalty.mLevel;
	const FGsSchemaLevelRecoveryExpTable* levelData = UGsTableUtil::FindRowById<UGsTableLevelRecoveryExpTable, FGsSchemaLevelRecoveryExpTable>(level);
	if (levelData)
	{
		switch (inCurrency)
		{
		case CurrencyType::DIA:
		{
			cost = levelData->expRecoveryDiaCost;
		}
		break;
		case CurrencyType::GOLD:
		{
			cost = levelData->expRecoveryGoldCost;
		}
		break;
		case CurrencyType::DEATH_EXP_RECOVERY:
		{
			cost = levelData->expRecoveryItemCost;
		}
		break;
		}
	}

	return cost;
}

void UGsRecoveryPenaltyListEntry::OnSelectPenalty()
{
	int32 index = INDEX_NONE;
	EGsPenaltyType penaltyType = _parents->GetPenaltyType();
	switch (penaltyType)
	{
	case EGsPenaltyType::Exp:
	{
		int32 num = _parents->GetExpPenalty().Num();
		for (int32 i = 0; i < num; ++i)
		{
			TArray<DeathPenaltyExp>& expList = _parents->GetExpPenalty();
			if (expList[i].mDeathPenaltyId == _expPenalty.mDeathPenaltyId)
			{
				index = i;
				break;
			}
		}

		if (INDEX_NONE != index)
		{
			_parents->GetExpPenalty().RemoveAt(index);
		}
		else
		{
			_parents->GetExpPenalty().Add(_expPenalty);

#if PENALTY_DEBUG
			int32 expCount = _parents->GetExpPenalty().Num();
			GSLOG(Log, TEXT("OnSelectPenalty - GetExpPenalty - count : %d"), expCount);
#endif
		}
	}
	break;
	case EGsPenaltyType::Item:
	{
		int32 num = _parents->GetItemPenalty().Num();
		for (int32 i = 0; i < num; ++i)
		{
			TArray<DeathPenaltyItem>& itemList = _parents->GetItemPenalty();
			if (itemList[i].mDeathPenaltyId == _itemPenalty.mDeathPenaltyId)
			{
				index = i;
				break;
			}
		}

		if (INDEX_NONE != index)
		{
			_parents->GetItemPenalty().RemoveAt(index);
		}
		else
		{
			_parents->GetItemPenalty().Add(_itemPenalty);

#if PENALTY_DEBUG
			int32 itemCount = _parents->GetItemPenalty().Num();
			GSLOG(Log, TEXT("OnSelectPenalty - GetItemPenalty - count : %d"), itemCount);
#endif
		}
	}
	break;
	}

	_parents->InvalidRestoreButton();
	InvalidSelect();
}

void UGsRecoveryPenaltyListEntry::OnLongPressIcon(UGsUIIconItem& InIcon)
{
	UGsUIIconItem::DefaultItemDetailOpener(InIcon, true);
}

void UGsRecoveryPenaltyListEntry::InvalidTime()
{
	EGsPenaltyType penaltyType = _parents->GetPenaltyType();	
	switch (penaltyType)
	{
	case EGsPenaltyType::Exp:
	{
		_timeSpan = FGsTimeSyncUtil::ConvertToDateTime(_expPenalty.mExpireTime) - FGsTimeSyncUtil::GetServerNowDateTime();
	}
	break;
	case EGsPenaltyType::Item:
	{
		_timeSpan = FGsTimeSyncUtil::ConvertToDateTime(_itemPenalty.mExpireTime) - FGsTimeSyncUtil::GetServerNowDateTime();
	}
	break;
	}

	_remainTimeText = FText::Format(_timeFormat, 0, 0);
	if (_timeSpan.GetTotalSeconds() > 0)
	{
		_remainTimeText = FText::Format(_timeFormat, _timeSpan.GetHours(), _timeSpan.GetMinutes());
	}	

#if PENALTY_DEBUG		
	switch (penaltyType)
	{
	case EGsPenaltyType::Exp:
	{
		FDateTime currentTime = FGsTimeSyncUtil::ConvertToDateTime(_expPenalty.mExpireTime);
		FString debugTime = FString::Printf(TEXT("%s\tt:%.0lf(%s)")
			, *_remainTimeText.ToString()
			, _timeSpan.GetTotalSeconds()
			, *currentTime.ToString());
		_timeText->SetText(FText::FromString(debugTime));
	}
	break;
	case EGsPenaltyType::Item:
	{
		FDateTime currentTime = FGsTimeSyncUtil::ConvertToDateTime(_itemPenalty.mExpireTime);
		FString debugTime = FString::Printf(TEXT("%s\tt:%.0lf(%s)")
			, *_remainTimeText.ToString()
			, _timeSpan.GetTotalSeconds()
			, *currentTime.ToString());
		_timeText->SetText(FText::FromString(debugTime));
	}
	break;
	}
#else
	_timeText->SetText(_remainTimeText);
#endif	

	if (_timeSpan.GetTotalSeconds() <= 0)
	{
		switch (penaltyType)
		{
		case EGsPenaltyType::Exp:
		{
			GGameData()->RemoveDeathPenaly(_expPenalty.mDeathPenaltyId);
		}
		break;
		case EGsPenaltyType::Item:
		{
			GGameData()->RemoveDeathPenaly(_itemPenalty.mDeathPenaltyId);
		}
		break;
		}

		GMessage()->GetUI().SendMessage(MessageUI::INVALID_PENALTY, nullptr);
		return;
	}
}

void UGsRecoveryPenaltyListEntry::InvalidDetailInfo()
{
	switch (_parents->GetPenaltyType())
	{
	case EGsPenaltyType::Exp:
	{
		float percent = _expPenalty.mCurrentExp != 0 ? (((float)_expPenalty.mExp / (float)_expPenalty.mCurrentExp) * 100.0f) : 0;
		_penaltyText->SetText(FText::Format(_lossExpAmountFormat, _expPenalty.mExp, percent));
	}
	break;
	case EGsPenaltyType::Item:
	{
		const FGsSchemaItemCommon* find = UGsTableUtil::FindRowById<UGsTableItemCommon, FGsSchemaItemCommon>(_itemPenalty.mDeathPenaltyItem.mTemplateId);
		if (nullptr == find)
		{
			return;
		}

		_penaltyText->SetText(FText::FromString(find->name.ToString()));
	}
	break;
	}
}
