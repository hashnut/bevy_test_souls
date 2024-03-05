// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUIPopupRecoveryPenaltyYesNo.h"
#include "Button.h"
#include "Net/MessageHandler/GsNetMessageHandlerWorld.h"
#include "Net/GsNetSendServiceWorld.h"
#include "Management/ScopeGlobal/GsUIManager.h"
#include "WidgetSwitcher.h"
#include "Currency/GsCurrencyHelper.h"
#include "Management/ScopeGlobal/GsGameDataManager.h"
#include "TextBlock.h"
#include "GsRecoveryPenaltyListEntry.h"
#include "UI/UIContent/Common/GsUICurrencySlot.h"
#include "UI/UIControlLib/ContentWidget/GsButton.h"

const FTextKey RECOVERY_POPUP_ASK_ITEM = TEXT("RecoveryPopup_Ask_Item");
const FTextKey RECOVERY_POPUP_ASK_EXP = TEXT("RecoveryPopup_Ask");
const FTextKey RECOVERY_COST_FREE_TEXT_KEY = TEXT("RecoveryCost_Free");

void UGsUIPopupRecoveryPenaltyYesNo::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	_cancelButton->OnClicked.AddDynamic(this, &UGsUIPopupRecoveryPenaltyYesNo::OnCancel);
	_recoveryButton->OnClicked.AddDynamic(this, &UGsUIPopupRecoveryPenaltyYesNo::OnClickRecovery);
}

void UGsUIPopupRecoveryPenaltyYesNo::NativeConstruct()
{
	Super::NativeConstruct();

	FText::FindText(RESURRECT_TEXT_KEY, RECOVERY_POPUP_ASK_ITEM, _itemRecoveryText);
	FText::FindText(RESURRECT_TEXT_KEY, RECOVERY_POPUP_ASK_EXP, _expRecoveryText);

	Clear();
}

void UGsUIPopupRecoveryPenaltyYesNo::NativeDestruct()
{
	Clear();

	Super::NativeDestruct();
}

void UGsUIPopupRecoveryPenaltyYesNo::Invalid()
{	
	InvalidCost();
}

/**
 * 무료 횟수가 있으면 무료로 복구해 준다. 그러나 무료 복구도 있고 지불 복구가 있다면 가장 비싼 것을 무료로 복구해준다. 
 */
void UGsUIPopupRecoveryPenaltyYesNo::InvalidCost()
{
	int32 totalCost = 0;	
	int32 freeCount = 0;
	bool isFree = false;
	_restoreList.Empty();

	if (_penaltyType == EGsPenaltyType::Item)
	{
		freeCount = GGameData()->GetUserData()->freeItemRestoreCount;	
		isFree = freeCount >= _itemPenalty.Num();

		for (int i = 0; i < _itemPenalty.Num(); ++i)
		{
			FGsRestoreInfo temp;
			temp.cost = UGsRecoveryPenaltyListEntry::GetCost(_itemPenalty[i], _currencyType);
			temp.penaltyId = _itemPenalty[i].mDeathPenaltyId;
			temp.currencyType = _currencyType;

			_restoreList.Add(temp);
		}

		_restoreList.Sort([=](const FGsRestoreInfo& source, const FGsRestoreInfo& dest)->bool {
			return source.cost > dest.cost;
		});

		for (int32 i = 0; i < _restoreList.Num(); ++i)
		{
			if (i < freeCount)
			{
				_restoreList[i].currencyType = CurrencyType::ITEM_DEATH_PENALTY_FREE;
				_restoreList[i].cost = 0;
			}			
			else
			{
				break;
			}
		}

		for (const FGsRestoreInfo& restore : _restoreList)
		{
			totalCost += restore.cost;
		}
	}
	else
	{
		freeCount = GGameData()->GetUserData()->freeExpRestoreCount;	
		isFree = freeCount >= _expPenalty.Num();

		for (int i = 0; i < _expPenalty.Num(); ++i)
		{
			FGsRestoreInfo temp;
			temp.cost = UGsRecoveryPenaltyListEntry::GetCost(_expPenalty[i], _currencyType);
			temp.penaltyId = _expPenalty[i].mDeathPenaltyId;
			temp.currencyType = _currencyType;

			_restoreList.Add(temp);
		}

		_restoreList.Sort([=](const FGsRestoreInfo& source, const FGsRestoreInfo& dest)->bool {
			return source.cost > dest.cost;
		});

		for (int32 i = 0; i < _restoreList.Num(); ++i)
		{
			if (i < freeCount)
			{
				_restoreList[i].currencyType = CurrencyType::EXP_DEATH_PENALTY_FREE;
				_restoreList[i].cost = 0;
			}
			else
			{
				break;
			}
		}

		for (const FGsRestoreInfo& restore : _restoreList)
		{
			totalCost += restore.cost;
		}
	}
	
	if (isFree)
	{
		FText findText;
		FText::FindText(RESURRECT_TEXT_KEY, RECOVERY_COST_FREE_TEXT_KEY, findText);		
		_costCurrencySlot->SetData(_currencyType, findText);
		_currentCost = 0;
	}
	else
	{
		_costCurrencySlot->SetData(_currencyType, totalCost);
		_currentCost = totalCost;
	}
	
	_costCurrencySlot->UpdateAmountTextColorLocalPlayer();
	_playerCurrencyAmount = GGameData()->GetCurrency(_currencyType);
	_currentCurrencySlot->SetData(_currencyType, _playerCurrencyAmount);
}

void UGsUIPopupRecoveryPenaltyYesNo::OnInputCancel()
{
	Close();
}

void UGsUIPopupRecoveryPenaltyYesNo::Clear()
{
	_parents.Reset();
	_expPenalty.Empty();
	_itemPenalty.Empty();
	_restoreList.Empty();

	_currentCost = 0;
	_playerCurrencyAmount = 0;
}

void UGsUIPopupRecoveryPenaltyYesNo::SetItemPenalty(UGsUIPopupRecoveryPenalty* inPerents, TArray<DeathPenaltyItem>& inList)
{
	_penaltyType = inPerents->GetPenaltyType();
	_currencyType = inPerents->GetCurrency();
	_parents = inPerents;
	_itemPenalty = inList;

	_penaltyText->SetText(_itemRecoveryText);
	_penaltyDetailText->SetVisibility(ESlateVisibility::Hidden);

	Invalid();
}

void UGsUIPopupRecoveryPenaltyYesNo::SetExpPenalty(UGsUIPopupRecoveryPenalty* inPerents, TArray<DeathPenaltyExp>& inList)
{
	_penaltyType = inPerents->GetPenaltyType();
	_currencyType = inPerents->GetCurrency();
	_parents = inPerents;
	_expPenalty = inList;

	_penaltyText->SetText(_expRecoveryText);
	_penaltyDetailText->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

	Invalid();
}

void UGsUIPopupRecoveryPenaltyYesNo::OnClickRecovery()
{
	if (false == FGsCurrencyHelper::CheckCurrency(_currencyType, _currentCost, false))
	{				
		OnCancel();
		FGsCurrencyHelper::OpenLackCurrencyPopup(_currencyType, _currentCost - _playerCurrencyAmount);
		return;
	}

	TArray<DeathPenaltyRestore> restoreList;	
	for (const FGsRestoreInfo& restore : _restoreList)
	{
		restoreList.Add(DeathPenaltyRestore(restore.penaltyId, restore.currencyType));
	}

	if (_parents.IsValid())
	{
		_parents->ClearRestorePenaltyList();
		_parents->Invalid();
	}
	
	FGsNetSendServiceWorld::SendReqDeathPenaltyRestore(restoreList);

	OnCancel();
}

void UGsUIPopupRecoveryPenaltyYesNo::OnCancel()
{
	//close
	GUI()->Close(this);
}
