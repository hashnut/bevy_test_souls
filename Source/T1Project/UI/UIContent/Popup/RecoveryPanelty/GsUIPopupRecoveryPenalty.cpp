// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUIPopupRecoveryPenalty.h"
#include "Button.h"
#include "UI/UIControlLib/Control/GsToggleGroup.h"
#include "UI/UIControlLib/ContentWidget/GsSwitcherButton.h"
#include "UI/UIControlLib/ContentWidget/GsButton.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "GsRecoveryPenaltyListEntry.h"
#include "ListView.h"
#include "Management/ScopeGlobal/GsGameDataManager.h"
#include "Management/ScopeGame/GsEventProgressManager.h"
#include "TextBlock.h"
#include "Management/ScopeGlobal/GsUIManager.h"
#include "Data/GsDataContainManager.h"
#include "WidgetSwitcher.h"
#include "UI/UIContent/Helper/GsUIColorHelper.h"
#include "UI/UIContent/Define/GsUIContentDefine.h"
#include "UI/UIContent/Helper/GsUIHelper.h"
#include "Management/GsMessageHolder.h"
#include "GsUIPopupRecoveryPenaltyYesNo.h"
#include "Resurrect/GsSchemaResurrectConfig.h"
#include "UTIL/GsTableUtil.h"
#include "EventProgress/GsEventProgressDefine.h"
#include "UI/UIContent/Define/EGsUIColorType.h"
#include "Item/GsItemManager.h"
#include "Components/HorizontalBox.h"
#include "Shared/Client/SharedEnums/SharedCommonEnum.h"

const FName POPUP_RECOVERY_PENALTY_YESNO = TEXT("PopupRecoveryPenaltyYesNo");
const FTextKey DAILTY_FREE_RECOVERY_AMOUNT_TEXT_KEY = TEXT("DailyFreeRecoveryAmount");
const FName RESURRECT_CONFIG_KEY = TEXT("NewRow");

void UGsUIPopupRecoveryPenalty::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	_toggleGroup.AddToggleWidget(_expSwitcherButton);
	_toggleGroup.AddToggleWidget(_equipItemSwitcherButton);
	_toggleGroup.OnSelectChanged.BindUObject(this, &UGsUIPopupRecoveryPenalty::OnClickTab);

	//close button
	_closeButton->OnClicked.AddDynamic(this, &UGsUIPopupRecoveryPenalty::OnClose);
	_changeCurrencyButton->OnClicked.AddDynamic(this, &UGsUIPopupRecoveryPenalty::OnChangeCurrency);
	_recoveryButton->OnClicked.AddDynamic(this, &UGsUIPopupRecoveryPenalty::OnRecovery);

	const FGsSchemaResurrectConfig* find = UGsTableUtil::FindRow<FGsSchemaResurrectConfig>(RESURRECT_CONFIG_KEY);
	if (find)
	{
		_maxExpPenaltyCount = find->maxExpPenaltyCount;
		_maxItemPenaltyCount = find->maxItemPenaltyCount;
	}	
	else
	{
		_maxExpPenaltyCount = 0;
		_maxItemPenaltyCount = 0;
	}	
}

void UGsUIPopupRecoveryPenalty::NativeConstruct()
{
	Super::NativeConstruct();
	
	FText::FindText(RESURRECT_TEXT_KEY, DAILTY_FREE_RECOVERY_AMOUNT_TEXT_KEY, _countFormat);
	FText::FindText(TEXT("ItemUIText"), TEXT("ItemUi_Inventory_Full"), _inventoryFullText);

	_msgUIHandle = GMessage()->GetUI().AddUObject(MessageUI::INVALID_PENALTY, this, &UGsUIPopupRecoveryPenalty::Invalid);

	_toggleGroup.SetSelectedIndex(0, true);		
	_selectedTabIndex = 0;	
	_penaltyType = EGsPenaltyType::Exp;
	_paymentCurrentType = EGsPenaltyCurrencyType::Gold;
	_currencyIndex = (int32)EGsPenaltyCurrencyType::Gold;

	Invalid();

	_time = 0;

	_penaltyListView->SetScrollbarVisibility(ESlateVisibility::Hidden);
}

void UGsUIPopupRecoveryPenalty::NativeDestruct()
{
	GMessage()->GetUI().Remove(_msgUIHandle);

	ClearRestorePenaltyList();

	if (FGsEventProgressManager* eventProgressManager = GSEventProgress())
	{
		eventProgressManager->OnReqFinishEvent(EGsEventProgressType::UI_POPUP_RECOVERY_PENALTY);
	}

	Super::NativeDestruct();
}

void UGsUIPopupRecoveryPenalty::NativeTick(const FGeometry & inMyGeometry, float inDeltaTime)
{
	Super::NativeTick(inMyGeometry, inDeltaTime);

	if (_time > _interval)
	{
		_time = 0;		
		InvalidLeftTime();
	}

	_time += inDeltaTime;
}

void UGsUIPopupRecoveryPenalty::Invalid(const IGsMessageParam* InParam)
{
	InvalidExpPenaltyCount();
	InvalidItemCount();
	InvalidCurrency();

	if (EGsPenaltyType::Exp == _penaltyType)
	{
		InvalidExpEntryList();		
	}
	else
	{		
		InvalidItemEntryList();
	}
	
	InvalidHolyWater();
	InvalidFreeRestoreCount();
	InvalidRestoreButton();
}

void UGsUIPopupRecoveryPenalty::InvalidRestoreButton()
{
	if (EGsPenaltyType::Exp == _penaltyType)
	{
		if (0 == _selectedExpPenalty.Num())
		{
			_recoveryButton->SetIsEnabled(false);
			return;
		}
	}
	else
	{
		if (0 == _selectedItemPenalty.Num())
		{
			_recoveryButton->SetIsEnabled(false);
			return;
		}
	}

	_recoveryButton->SetIsEnabled(true);
}

void UGsUIPopupRecoveryPenalty::InvalidExpEntryList()
{
	//get exp penalty data list
	const TArray<DeathPenaltyExp>& paneltyList = GGameData()->GetUserData()->expPenalty;

	//set penalty data 
	TArray<UGsRecoveryPenaltyListEntryParam*> entryList;
	for (const DeathPenaltyExp& panelty : paneltyList)
	{
		UGsRecoveryPenaltyListEntryParam* param = NewObject<UGsRecoveryPenaltyListEntryParam>();
		param->Set(this, panelty);
		entryList.Add(param);				
	}

	entryList.Sort([](const UGsRecoveryPenaltyListEntryParam& source, const UGsRecoveryPenaltyListEntryParam& dest)->bool {
		return source._expPenalty.mExpireTime < dest._expPenalty.mExpireTime;
		});

	_penaltyListView->ClearListItems();
	_penaltyListView->SetListItems(entryList);		

	if (paneltyList.Num() > 0)
	{
		_noPaneltySwitcher->SetVisibility(ESlateVisibility::Hidden);
	}
	else
	{
		_noPaneltySwitcher->SetVisibility(ESlateVisibility::HitTestInvisible);
		_noPaneltySwitcher->SetActiveWidgetIndex(0);
	}
}

void UGsUIPopupRecoveryPenalty::InvalidItemEntryList()
{
	//get exp penalty data list
	const TArray<DeathPenaltyItem>& paneltyList = GGameData()->GetUserData()->itemPenalty;

	//set penalty data 
	TArray<UGsRecoveryPenaltyListEntryParam*> entryList;
	for (const DeathPenaltyItem& panelty : paneltyList)
	{
		UGsRecoveryPenaltyListEntryParam* param = NewObject<UGsRecoveryPenaltyListEntryParam>();
		param->Set(this, panelty);
		entryList.Add(param);
	}

	entryList.Sort([](const UGsRecoveryPenaltyListEntryParam& source, const UGsRecoveryPenaltyListEntryParam& dest)->bool {
		return source._itemPenalty.mExpireTime < dest._itemPenalty.mExpireTime;
		});

	_penaltyListView->ClearListItems();
	_penaltyListView->SetListItems(entryList);

	if (paneltyList.Num() > 0)
	{
		_noPaneltySwitcher->SetVisibility(ESlateVisibility::Hidden);
	}
	else
	{		
		_noPaneltySwitcher->SetVisibility(ESlateVisibility::HitTestInvisible);
		_noPaneltySwitcher->SetActiveWidgetIndex(1);
	}
}

void UGsUIPopupRecoveryPenalty::InvalidFreeRestoreCount()
{
	bool canFreeCount = 0;

	FText freeRestoreCountText;
	if (_penaltyType == EGsPenaltyType::Exp)
	{
		canFreeCount = GGameData()->GetUserData()->freeExpRestoreCount > 0;
		freeRestoreCountText = FText::Format(_countFormat, GGameData()->GetUserData()->freeExpRestoreCount);
		_freeReviveCountText->SetText(freeRestoreCountText);
	}
	else
	{
		canFreeCount = GGameData()->GetUserData()->freeItemRestoreCount > 0;		
		freeRestoreCountText = FText::Format(_countFormat, GGameData()->GetUserData()->freeItemRestoreCount);
		_freeReviveCountText->SetText(freeRestoreCountText);
	}	
	
	SetHighlight(_freeReviveCountText, canFreeCount);
}

void UGsUIPopupRecoveryPenalty::InvalidLeftTime()
{
	for (UUserWidget* widget : _penaltyListView->GetDisplayedEntryWidgets())
	{
		UGsRecoveryPenaltyListEntry* entry = Cast<UGsRecoveryPenaltyListEntry>(widget);
		if (entry)
		{
			entry->InvalidTime();
		}
	}
}

void UGsUIPopupRecoveryPenalty::InvalidHolyWater()
{
	if (_penaltyType != EGsPenaltyType::Exp)
	{
		return;
	}

	bool isEmpty = (GGameData()->GetCurrency(CurrencyType::DEATH_EXP_RECOVERY) > 0);
	FText countText = FText::Format(_countFormat, GGameData()->GetCurrency(CurrencyType::DEATH_EXP_RECOVERY));
	_holyWaterChargeCountTextBlock->SetText(countText);
	SetHighlight(_holyWaterChargeCountTextBlock, isEmpty);
}

void UGsUIPopupRecoveryPenalty::InvalidCurrency()
{
	_currencyTypeSwitcher->SetActiveWidgetIndex(_currencyIndex);

	switch (_currencyIndex)
	{
	case (int32)EGsPenaltyCurrencyType::Gold:
	{
		_paymentCurrentType = EGsPenaltyCurrencyType::Gold;
		_currencyType = CurrencyType::GOLD;
		break;
	}
	case (int32)EGsPenaltyCurrencyType::Dia:
	{
		_paymentCurrentType = EGsPenaltyCurrencyType::Dia;
		_currencyType = CurrencyType::DIA;
		break;
	}
	case (int32)EGsPenaltyCurrencyType::HolyWater:
	{
		_paymentCurrentType = EGsPenaltyCurrencyType::HolyWater;
		_currencyType = CurrencyType::DEATH_EXP_RECOVERY;
		break;
	}
	}
}

void UGsUIPopupRecoveryPenalty::OnClose()
{
	GUI()->CloseAllStack(this);	
}

void UGsUIPopupRecoveryPenalty::OnChangeCurrency()
{
	_currencyIndex++;
	if (_penaltyType == EGsPenaltyType::Exp)
	{
		if (_currencyIndex > 2)
		{
			_currencyIndex = 0;
		}
	}
	else
	{
		if (_currencyIndex > 1)
		{
			_currencyIndex = 0;
		}
	}

	InvalidCurrency();

	for (UUserWidget* widget : _penaltyListView->GetDisplayedEntryWidgets())
	{
		UGsRecoveryPenaltyListEntry* entry = Cast<UGsRecoveryPenaltyListEntry>(widget);
		if (entry)
		{
			entry->InvalidCurrency();
		}
	}
}

void UGsUIPopupRecoveryPenalty::InvalidExpPenaltyCount()
{
	int32 count = GGameData()->GetUserData()->expPenalty.Num();	
	FString countText = FString::Printf(TEXT("%d/%d"), count, _maxExpPenaltyCount);
	int32 index = _toggleGroup.GetSelectedIndex();

	_expCountOffText->SetText(FText::FromString(countText));
	_expCountOnText->SetText(FText::FromString(countText));
}

void UGsUIPopupRecoveryPenalty::InvalidItemCount()
{
	int32 count = GGameData()->GetUserData()->itemPenalty.Num();
	FString countText = FString::Printf(TEXT("%d/%d"), count, _maxItemPenaltyCount);
	int32 index = _toggleGroup.GetSelectedIndex();

	_itemCountOffText->SetText(FText::FromString(countText));
	_itemCountOnText->SetText(FText::FromString(countText));
}

void UGsUIPopupRecoveryPenalty::OnRecovery()
{
	if (EGsPenaltyType::Item == _penaltyType)
	{
		if (GItem()->IsInvenSlotFull(_selectedItemPenalty.Num()))
		{			
			FGsUIHelper::TraySectionMessageTicker(EGsNotiSection::EXTSYSTEM, _inventoryFullText);
			return;
		}
	}

	//send restore data
	if (GUI()->IsActiveWidget(POPUP_RECOVERY_PENALTY_YESNO))
	{
		return;
	}

	TWeakObjectPtr<UUserWidget> widget = GUI()->OpenAndGetWidget(POPUP_RECOVERY_PENALTY_YESNO);
	if (false == widget.IsValid())
	{
		return;
	}

	UGsUIPopupRecoveryPenaltyYesNo* castWidget = Cast<UGsUIPopupRecoveryPenaltyYesNo>(widget.Get());
	if (nullptr == castWidget)
	{
		return;
	}

	if (_penaltyType == EGsPenaltyType::Exp)
	{
		castWidget->SetExpPenalty(this, _selectedExpPenalty);
	}
	else
	{	
		castWidget->SetItemPenalty(this, _selectedItemPenalty);
	}
}

void UGsUIPopupRecoveryPenalty::OnClickTab(int32 InIndex)
{
	if (InIndex == _selectedTabIndex)
		return;

	_selectedTabIndex = InIndex;

	if (InIndex == 0)
	{
		_penaltyType = EGsPenaltyType::Exp;	
		_holyWaterPanel->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}
	else
	{
		_penaltyType = EGsPenaltyType::Item;
		_holyWaterPanel->SetVisibility(ESlateVisibility::Collapsed);
	}

	_paymentCurrentType = EGsPenaltyCurrencyType::Gold;
	_currencyType = CurrencyType::GOLD;
	_currencyIndex = (int32)EGsPenaltyCurrencyType::Gold;

	_selectedExpPenalty.Empty();
	_selectedItemPenalty.Empty();

	Invalid();
}

void UGsUIPopupRecoveryPenalty::OnInputCancel()
{
	Close();
}

void UGsUIPopupRecoveryPenalty::SetHighlight(class UTextBlock* inWidget, bool inHighLight)
{
	FLinearColor color;
	if (inHighLight)
	{
		color = FGsUIColorHelper::GetColor(EGsUIColorType::HIGHLIGHT_TEXT_COLOR);
		inWidget->SetColorAndOpacity(color);
	}
	else
	{
		color = FGsUIColorHelper::GetColor(EGsUIColorType::HIGHLIGHT_TEXT_COLOR);
		inWidget->SetColorAndOpacity(color);
	}
}

void UGsUIPopupRecoveryPenalty::ClearRestorePenaltyList()
{
	_selectedItemPenalty.Empty();
	_selectedExpPenalty.Empty();
}

CurrencyType UGsUIPopupRecoveryPenalty::GetCurrencyType(EGsPenaltyCurrencyType inCurrencyType)
{
	switch (inCurrencyType)
	{
	case EGsPenaltyCurrencyType::Dia:
	{
		return CurrencyType::DIA;
	}
	break;
	case EGsPenaltyCurrencyType::Gold:
	{
		return CurrencyType::GOLD;
	}
	break;
	case EGsPenaltyCurrencyType::HolyWater:
	{
		return CurrencyType::DEATH_EXP_RECOVERY;
	}
	break;
	}

	return CurrencyType::DIA;
}

EGsPenaltyCurrencyType UGsUIPopupRecoveryPenalty::GetCurrencyType(CurrencyType inCurrencyType)
{
	switch (inCurrencyType)
	{
	case CurrencyType::DIA:
	{
		return EGsPenaltyCurrencyType::Dia;
	}
	break;
	case CurrencyType::GOLD:
	{
		return EGsPenaltyCurrencyType::Gold;
	}
	break;
	case CurrencyType::DEATH_EXP_RECOVERY:
	{
		return EGsPenaltyCurrencyType::HolyWater;
	}
	break;
	}

	return EGsPenaltyCurrencyType::Gold;
}
