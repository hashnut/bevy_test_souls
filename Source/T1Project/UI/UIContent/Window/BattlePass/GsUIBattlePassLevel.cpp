// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUIBattlePassLevel.h"
#include "UI/UIContent/Window/BattlePass/GsUIBattlePassLevelRewardItem.h"
#include "UI/UIContent/Window/BattlePass/GsUIBattlePassLevelBonusItem.h"
#include "UI/UIContent/Window/BattlePass/GsUIBattlePassLevelHelper.h"
#include "UI/UIContent/Common/GsUICurrencyButton.h"
#include "UI/UIContent/Popup/GsUIPopupCommonPay.h"
#include "UI/UIContent/Popup/GsUIPopupCommonItemConsume.h"
#include "UI/UIControlLib/ContentWidget/GsButton.h"
#include "Components/PanelWidget.h"
#include "Components/ScrollBox.h"

#include "BattlePass/GsBattlePassData.h"
#include "BattlePass/GsBattlePassLevelData.h"

#include "Management/ScopeGlobal/GsGameDataManager.h"
#include "Management/ScopeGlobal/GsUIManager.h"
#include "Management/ScopeGame/GsBattlePassManager.h"
#include "Item/GsItemManager.h"
#include "Item/GsItemIngredient.h"
#include "Currency/GsCostPackage.h"

#include "Net/GsNetSendServiceWorld.h"
#include "TimerManager.h"

bool UGsUIBattlePassLevel::isBlockClick = false;
FTimerHandle UGsUIBattlePassLevel::clickTimer;

void UGsUIBattlePassLevel::BeginDestroy()
{
	if (nullptr != _levelScrollBoxHelper)
	{
		_levelScrollBoxHelper->ClearScrollPanel();
		_levelScrollBoxHelper = nullptr;
	}

	Super::BeginDestroy();
}

void UGsUIBattlePassLevel::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	_levelScrollView->OnUserScrolled.AddDynamic(this, &UGsUIBattlePassLevel::OnScrollEvent);

	_levelScrollBoxHelper = NewObject<UGsUIBattlePassLevelHelper>(this);
	_levelScrollBoxHelper->InitScrollPanel(_levelScrollView);

	_btnRewardEnable->OnClickCurrencyButton.BindUObject(this, &UGsUIBattlePassLevel::OnClickOpenAdditionalReward);
	_btnRecvRewardAll->OnClicked.AddDynamic(this, &UGsUIBattlePassLevel::OnReceiveRewardAll);
}

void UGsUIBattlePassLevel::NativeConstruct()
{
	Super::NativeConstruct();

	if (auto battlePassData = _battlePassData.Pin())
	{
		_btnRecvRewardAll->SetIsEnabled(battlePassData->IsSetRedDot());
	}

	_btnRewardEnable->SetOnClickLockTime(1.f);
}

void UGsUIBattlePassLevel::NativeDestruct()
{
	if (_levelScrollBoxHelper)
	{
		_levelScrollBoxHelper->ClearAllScrollWidgets();
	}

	Super::NativeDestruct();
}

void UGsUIBattlePassLevel::SetData(TWeakPtr<FGsBattlePassData> passData)
{
	_battlePassData = passData;
	_passBonusData = passData.IsValid()? passData.Pin()->GetPassLevelBonusData() : nullptr;

	_passLevelData.Reset();
	if (passData.IsValid())
	{	
		passData.Pin()->GetPassLevelList(_passLevelData);
	}

	for (FString widgetKey : _inWidgets)
	{
		auto wedget = GUI()->GetWidgetByKey(*widgetKey);
		if (wedget.IsValid()) 
			wedget->Close();
	}

	_inWidgets.Reset();
}

void UGsUIBattlePassLevel::RefreshUI(bool init)
{
	isBlockClick = false; 
	GetWorld()->GetTimerManager().ClearTimer(clickTimer);

	if (init)
	{
		CreateLevelWidgets();
	}

	RefreshLevelWidgets();

	FText UI_PassReward_ActivatePass = FText::GetEmpty();
	FText::FindText(TEXT("BattlePassText"), TEXT("UI_PassReward_ActivatePass"), UI_PassReward_ActivatePass);
	_btnRewardEnable->SetButtonName(UI_PassReward_ActivatePass);

	_addRewardDesc = FText::GetEmpty();
		
	if (auto passData = _battlePassData.Pin())
	{
		SetScrollToWidget(passData->GetProgressLevelData());

		if (IsScrollEndPosition() || _isScrollEndPosition)
		{
			_isScrollEndPosition = true;
			SetDisplayItem(nullptr);
		}
		else
		{
			if (auto curLevel = passData->GetProgressLevelData().Pin())
			{
				SetDisplayItem(curLevel->GetDPLevelData());
			}
			else SetDisplayItem(nullptr);
		}

		_btnRecvRewardAll->SetIsEnabled(passData->IsSetRedDot());

		auto openPay = passData->GetAdditionalRewardOpenPay();
		_btnRewardEnable->SetIsEnabled(openPay._payType == openPay.ITEM || openPay._payType == openPay.CURRENCY);
		_btnRewardEnable->SetVisibility((openPay._payType == openPay.ITEM || openPay._payType == openPay.CURRENCY) ? 
			ESlateVisibility::Visible : ESlateVisibility::Collapsed);

		//패스를 구매하여 추가보상을 얻으실 수 있습니다
		FText  UI_PassReward_Notice = FText::GetEmpty();
		FText::FindText(TEXT("BattlePassText"), TEXT("UI_PassReward_Notice"), UI_PassReward_Notice);

		//추가 보상 활성화 완료
		FText  UI_PassReward_Activated = FText::GetEmpty();
		FText::FindText(TEXT("BattlePassText"), TEXT("UI_PassReward_Activated"), UI_PassReward_Activated);

		_addRewardDesc = _btnRewardEnable->GetIsEnabled() ?  UI_PassReward_Notice : UI_PassReward_Activated;

		if (openPay._payType == openPay.CURRENCY)
		{
			Currency currGold = GGameData()->GetCurrency(openPay._currency.type);

			_btnRewardEnable->SetData(openPay._currency.type, openPay._currency.amount);
			_btnRewardEnable->SetAmountTextColor((int64(currGold) >= openPay._currency.amount) ? EGsUIColorType::DEFAULT_ENOUGH : EGsUIColorType::DEFAULT_LACK);
		}
		else if (openPay._payType == openPay.ITEM)
		{
			int32 ownItemCount = 0;
			auto itemList = GItem()->FindByTID(openPay._item.itemId, ItemType::MAX);
			for (auto item : itemList)
			{
				if (false == item.IsValid())
					continue;

				if (auto ingredientItem = item.Pin()->GetCastItem<FGsItemIngredient>())
					ownItemCount += ingredientItem->GetAmount();
			}

			_btnRewardEnable->SetData(openPay._item.itemId, openPay._item.amount);
			_btnRewardEnable->SetAmountTextColor((int64(ownItemCount) >= openPay._item.amount) ? EGsUIColorType::DEFAULT_ENOUGH : EGsUIColorType::DEFAULT_LACK);
		}
	}
}

void UGsUIBattlePassLevel::CreateLevelWidgets()
{
	_levelScrollBoxHelper->RemoveScrollWidget(_bonusItem);
	_levelScrollBoxHelper->CashedAddScrollWidget(_entryReward, _passLevelData.Num());

	if (_passBonusData.IsValid())
	{
		_levelScrollBoxHelper->AddScrollWidget(_bonusItem);
	}
}

void UGsUIBattlePassLevel::RefreshLevelWidgets()
{
	TArray<UUserWidget*> widgets;
	_levelScrollBoxHelper->GetAllScrollWidgets(widgets);

	_endTopOffset = 0.f;
	int levelIndex = 0;
	for (auto widget : widgets)
	{
		if (auto reward = Cast<UGsUIBattlePassLevelRewardItem>(widget))
		{
			if (_passLevelData.IsValidIndex(levelIndex))
			{
				reward->SetData(_passLevelData[levelIndex++]);

				if (!reward->OnReceiveReward.IsBound())
					reward->OnReceiveReward.BindUObject(this, &UGsUIBattlePassLevel::OnReceiveReward);
				reward->RefreshUI();
			}
		}
		else if (auto bonus = Cast<UGsUIBattlePassLevelBonusItem>(widget))
		{
			bonus->SetData(_passBonusData);
			if (!bonus->OnReceiveReward.IsBound())
				bonus->OnReceiveReward.BindUObject(this, &UGsUIBattlePassLevel::OnReceiveRewardBonus);
			bonus->RefreshUI();
		}
	}
}

void UGsUIBattlePassLevel::SetActiveAddReward()
{
	if (_btnRewardEnable->GetIsEnabled())
	{
		RefreshUI(false);
	}
}

void UGsUIBattlePassLevel::SetRecvReward(Level level, bool isAddReward /*= false*/)
{
	auto index = _passLevelData.IndexOfByPredicate([level](auto e) { 
		if (e.IsValid()) return e.Pin()->GetLevel() == level; return false; });

	if (index != INDEX_NONE)
	{
		if (auto entry = Cast<UGsUIBattlePassLevelRewardItem>(_levelScrollBoxHelper->GetScrollWidget(index) ))
		{
			entry->RefreshUI();
		}
	}
}


void UGsUIBattlePassLevel::OnScrollEvent(float Offset)
{
	if (GetCountLevelItems() <= 0) return;
	if (IsScrollEndPosition())
	{
		if (_isScrollEndPosition == false)
		{
			_isScrollEndPosition = true;
			SetDisplayItem(nullptr);
			_levelScrollView->ScrollToEnd();
		}
		return;
	}

	_isScrollEndPosition = false;

	//같은 사이즈라는 가정임 
	auto diff = _levelScrollView->GetDesiredSize().X / float(GetCountLevelItems());
	if (diff <= 0) return;
	
	auto endTopOffset = _endTopOffset = _endTopOffset == 0.f ? _levelScrollView->GetScrollOffsetOfEnd() : _endTopOffset;
	auto endTopItem = FMath::CeilToInt(endTopOffset / diff);
	auto viewCount = GetCountLevelItems() - endTopItem;

	auto topItem = FMath::TruncToInt(Offset / diff);
	auto dpItem = topItem + viewCount;

	if (_passLevelData.IsValidIndex(dpItem))
	{
		SetDisplayItem(_passLevelData[dpItem].Pin()->GetDPLevelData());
	}
	else
	{
		SetDisplayItem(nullptr);
	}
}

void UGsUIBattlePassLevel::SetDisplayItem(TWeakPtr<FGsBattlePassLevelData> dpItem)
{
	//GSLOG(Log, TEXT("********OnScrollEvent dpItem.IsValid() ? [%d]"), dpItem.IsValid());

	_dpReward->SetData(dpItem);
	_dpReward->RefreshUI();
	_dpLevelPanel->SetVisibility(dpItem.IsValid() ? 
		ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed);
}

void UGsUIBattlePassLevel::OnClickOpenAdditionalReward()
{
	if (auto battlePassData = _battlePassData.Pin())
	{
		auto passId = battlePassData->GetBattlePassId();
		auto pay = battlePassData->GetAdditionalRewardOpenPay();
		FString popupKey = TEXT("PopupCommonPay");
		if (auto popup = Cast<UGsUIPopupCommonPay>(GUI()->OpenAndGetWidget(*popupKey).Get()))
		{
			FText UI_ActivatePass_MainText;
			FText::FindText(TEXT("BattlePassText"), TEXT("UI_ActivatePass_MainText"), UI_ActivatePass_MainText);

			if (pay._payType == pay.CURRENCY)
			{
				popup->SetData(UI_ActivatePass_MainText,
					FGsCostPackage::CreateSimple(pay._currency.type, pay._currency.amount),
					[passId](bool isOK) {
						if (isOK)
						{
							FGsNetSendServiceWorld::SendReqActiveBattlePassAddReward(passId);
						}
					});
			}
			else if (pay._payType == pay.ITEM)
			{
				auto itemId = pay._item.itemId;
				popup->SetData(UI_ActivatePass_MainText,
					FGsCostPackage::CreateSimple(pay._item.itemId, pay._item.amount),
					[passId, itemId](bool isOK) {
						if (isOK)
						{
							FGsNetSendServiceWorld::SendReqActiveBattlePassAddReward(passId, itemId);
						}
					});
			}
			_inWidgets.Add(popupKey);
			_inWidgets.Add(TEXT("PopupCommonPayLack"));
		}
	}
}

void UGsUIBattlePassLevel::OnReceiveRewardAll()
{
	if (auto battlePassData = _battlePassData.Pin())
	{
		auto passId = battlePassData->GetBattlePassId();
		FGsNetSendServiceWorld::SendReqBattlePassLevelRewardList(passId);
	}
}

void UGsUIBattlePassLevel::OnReceiveReward(bool isAdditionalReward, TWeakPtr<FGsBattlePassLevelData> weaklevelData)
{
	if (isBlockClick) return;

	if (auto battlePassData = _battlePassData.Pin())
	{
		auto passId = battlePassData->GetBattlePassId();

		if (auto levelData = weaklevelData.Pin())
		{
			isBlockClick = true;

			auto level = levelData->GetLevel();
			FGsNetSendServiceWorld::SendReqGetBattlePassLevelReward(passId, level, isAdditionalReward);

			GetWorld()->GetTimerManager().ClearTimer(clickTimer);
			GetWorld()->GetTimerManager().SetTimer(clickTimer, [] {
					isBlockClick = false;
				}, 0.5f, false);
		}
	}
}

void UGsUIBattlePassLevel::OnReceiveRewardBonus(TWeakPtr<FGsBattlePassLevelBonusData> bonusData)
{
	if (isBlockClick) return;

	if (auto battlePassData = _battlePassData.Pin())
	{
		auto passId = battlePassData->GetBattlePassId();

		if (auto bonusLevel = bonusData.Pin())
		{
			isBlockClick = true;
			FGsNetSendServiceWorld::SendReqGetBattlePassLevelReward(passId, 0, false);

			GetWorld()->GetTimerManager().ClearTimer(clickTimer);
			GetWorld()->GetTimerManager().SetTimer(clickTimer, [] {
				isBlockClick = false;
				}, 0.5f, false);
		}
	}
}

void UGsUIBattlePassLevel::SetScrollToWidget(TWeakPtr<FGsBattlePassLevelData> level)
{
	if (level.IsValid())
	{
		auto procIndex = _passLevelData.IndexOfByPredicate([level](auto e) { return e == level; });
		if (procIndex != INDEX_NONE)
		{
			if (auto widget = _levelScrollBoxHelper->GetScrollWidget(procIndex))
			{
				_levelScrollView->ScrollWidgetIntoView(widget, false, EDescendantScrollDestination::Center);
			}
		}
	}
	else
	{
		_levelScrollView->ScrollToEnd();
	}
}

bool UGsUIBattlePassLevel::IsScrollEndPosition()
{
	return FMath::IsNearlyZero(_levelScrollView->GetScrollOffsetOfEnd() - _levelScrollView->GetScrollOffset());
}


void UGsUIBattlePassLevel::OnAckReceiveBattlePassLevelRewardAll(const TArray<Level>& receivedLevel)
{
	TArray<UUserWidget*> widgets;
	_levelScrollBoxHelper->GetAllScrollWidgets(widgets);

	_endTopOffset = 0.f;
	int levelIndex = 0;
	for (auto widget : widgets)
	{
		if (auto reward = Cast<UGsUIBattlePassLevelRewardItem>(widget))
		{
			if (_passLevelData.IsValidIndex(levelIndex))
			{
				if (receivedLevel.Find(_passLevelData[levelIndex].Pin()->GetLevel())!=INDEX_NONE)
				{
					reward->RefreshUI();
				}
			}
			levelIndex++;
		}
		else if (auto bonus = Cast<UGsUIBattlePassLevelBonusItem>(widget))
		{
			if (receivedLevel.Find(0))
			{
				bonus->RefreshUI();
			}
		}
	}
}

void UGsUIBattlePassLevel::OnUpdateRedDot()
{
	if (auto battlePassData = _battlePassData.Pin())
	{
		_btnRecvRewardAll->SetIsEnabled(battlePassData->IsSetRedDot());
	}
}
