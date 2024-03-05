// Fill out your copyright notice in the Description page of Project Settings.

#include "GsUIPlayerStatDistributionPanel.h"
#include "Components/SizeBox.h"
#include "Components/ListView.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Blueprint/UserWidget.h"
#include "Paper2D/Classes/PaperSprite.h"

#include "DataSchema/Stat/GsSchemaBonusStatConfig.h"

#include "Shared/Client/SharedEnums/SharedCommonEnum.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"

#include "UI/UIControlLib/ContentWidget/GsButton.h"
#include "UI/UIContent/Popup/Stat/GsStatDistributionListEntry.h"
#include "UI/UIContent/Popup/GsUIPopupCommonPay.h"
#include "UI/UIContent/Popup/GsUIPopupItemDetailMain.h"
#include "UI/UIContent/Popup/GsUIPopupCommonItemConsume.h"
#include "UI/UIContent/Helper/GsUIHelper.h"
#include "UI/UIContent/Helper/GsUIColorHelper.h"
#include "UI/UIContent/Helper/GsUIStringHelper.h"

#include "Net/GsNetSendServiceWorld.h"

#include "Management/ScopeGame/GsUnlockManager.h"
#include "Management/ScopeGlobal/GsUIManager.h"
#include "Management/ScopeGlobal/GsGameFlowManager.h"
#include "Management/ScopeGlobal/GsGameDataManager.h"
#include "Management/GsScopeGlobal.h"
#include "Management/GsMessageHolder.h"

#include "Message/GsMessageContents.h"
#include "Message/MessageParam/GsItemContentsMessageParam.h"

#include "GameFlow/GameContents/ContentsGame/Hud/GsGameStateHud.h"
#include "GameFlow/GameContents/GsContentsMode.h"
#include "GameFlow/GameContents/ContentsGame/GsContentsManagerGame.h"
#include "GameFlow/GsGameFlowGame.h"
#include "GameFlow/GameContents/ContentsGame/Hud/Stat/GsPlayerStatHandler.h"
#include "GameFlow/GameContents/ContentsGame/Hud/Stat/GsPlayerStatDistributionController.h"

#include "Item/GsItem.h"
#include "Item/GsItemManager.h"

#include "T1Project.h"

void UGsUIPlayerStatDistributionPanel::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	_statDecisionButton->OnClicked.AddDynamic(this, &UGsUIPlayerStatDistributionPanel::OnStatDecision);
	_btnCountExtend->OnClicked.AddDynamic(this, &UGsUIPlayerStatDistributionPanel::OnClickBonusStatItemCountExtend);
	_btnStatReset->OnClicked.AddDynamic(this, &UGsUIPlayerStatDistributionPanel::OnClickStatReset);
}

void UGsUIPlayerStatDistributionPanel::NativeConstruct()
{
	Super::NativeConstruct();

	FText::FindText(TEXT("ItemUIText"), TEXT("ItemUi_Consume_Notice_Scroll_Reset"), _topText);
	FText::FindText(TEXT("StatText"), TEXT("Popup_BonusStat_ExtendMax"), _bonusStatExtendMaxText);

	_statDecisionButtonDisable->SetIsEnabled(false);
	_playerStatHandler = FGsPlayerStatHandler::GetInstance()->GetStatDistributionController();
	if (_playerStatHandler)
	{
		_playerStatHandler->ResetBackupStatDistribution();
		_playerStatHandler->InitStat();
		_playerStatHandler->ResetStatDistribution();
	}

	if (const UGsTableBonusStatConfig* statConfigTable = Cast<UGsTableBonusStatConfig>(FGsSchemaBonusStatConfig::GetStaticTable()))
	{
		TArray<const FGsSchemaBonusStatConfig*> rows;
		if (statConfigTable->GetAllRows(rows) && 0 < rows.Num())
		{
			_bonusStatConfig = rows[0];
		}
	}

	InvalidateRemainBonusStatPoint();
	InitEntryList();
	InvalidateStatDistributionButton();
	InitializeBonusStatItemInfo();
	BindMessage();
}

void UGsUIPlayerStatDistributionPanel::NativeDestruct()
{
	_statDistributionListView->ClearListItems();

	if (true == _btnBonusStatItem->_onFinishedLongPressGauge.IsBoundToObject(this) 
		&& true == _longPressDelegate.IsValid())
	{
		_btnBonusStatItem->_onFinishedLongPressGauge.Remove(_longPressDelegate);
		_longPressDelegate.Reset();
	}

	UnbindMessage();

	Super::NativeDestruct();
}

void UGsUIPlayerStatDistributionPanel::BindMessage()
{	
	FGsMessageHolder* holder = GMessage();
	if (nullptr == holder)
	{
		return;
	}	

	//hud message
	_hudMessageHandlerArray.Empty();
	_hudMessageHandlerArray.Emplace(holder->GetContentsHud().AddUObject(MessageContentHud::INVALIDATE_STAT
		, this
		, &UGsUIPlayerStatDistributionPanel::InvalidateWidget));

	//ui meesage
	_uiMessageHandlerArray.Empty();
	_uiMessageHandlerArray.Emplace(holder->GetUI().AddUObject(MessageUI::INVALID_PLAYER_STAT_DISTRIBUTION
		, this
		, &UGsUIPlayerStatDistributionPanel::OnInvalidWidget));
	_uiMessageHandlerArray.Emplace(holder->GetUI().AddUObject(MessageUI::INVALID_BONUS_STAT_ITEM_COUNT
		, this
		, &UGsUIPlayerStatDistributionPanel::OnInvalidBonusStatItemCount));
}

void UGsUIPlayerStatDistributionPanel::UnbindMessage()
{
	FGsMessageHolder* holder = GMessage();
	if (nullptr == holder)
	{
		return;
	}

	for (const MsgHudHandle& pair : _hudMessageHandlerArray)
	{
		holder->GetContentsHud().Remove(pair);
	}
	_hudMessageHandlerArray.Empty();

	for (const MsgUIHandle& pair : _uiMessageHandlerArray)
	{
		holder->GetUI().Remove(pair);
	}
	_uiMessageHandlerArray.Empty();
}

void UGsUIPlayerStatDistributionPanel::InitEntryList()
{
	//get number of stat type
	int32 num = MAIN_STAT_ARRAY.Num();
	TArray<UGsStatDistributionListEntryData*> dataList;
	UGsStatDistributionListEntryData* data = nullptr;
	for (int i = 0; i < num; ++i)
	{
		data = NewObject<UGsStatDistributionListEntryData>();
		data->_statType = MAIN_STAT_ARRAY[i];
		data->_invalidEvent = [this]() {
			InvalidateRemainBonusStatPoint();
			InvalidateStatDistributionButton();
		};

		dataList.Add(data);
	}

	_statDistributionListView->SetListItems(dataList);		
	_statDistributionListView->SetScrollbarVisibility(ESlateVisibility::Hidden);
}

void UGsUIPlayerStatDistributionPanel::InvalidateEntryList()
{
	TArray<UObject*> itemList = _statDistributionListView->GetListItems();
	UUserWidget* widget = nullptr;
	UGsStatDistributionListEntry* entry = nullptr;
	for (UObject* item : itemList)
	{
		widget = _statDistributionListView->GetEntryWidgetFromItem(item);
		if (nullptr == widget)
		{
			continue;
		}

		entry = Cast<UGsStatDistributionListEntry>(widget);
		if (nullptr == entry)
		{
			continue;
		}

		entry->InvalidWidget();
	}
}

void UGsUIPlayerStatDistributionPanel::InvalidateRemainBonusStatPoint()
{	
	if (nullptr == _playerStatHandler)
	{
		return;
	}

	_bonusStatPoint = _playerStatHandler->GetCurrentRemainBonusStatPoint();
	_statRemainPointText->SetText(FText::FromString(FString::FromInt(_bonusStatPoint)));
}

void UGsUIPlayerStatDistributionPanel::OnStatDecision()
{
	if (GSUnlock()->IsContentsUnlock(EGsUnlockContentsType::STAT_DISTRIBUTION, true))
	{
		//send stat decision packet to server
		FGsNetSendServiceWorld::SendRequestBonusStatIncrease();
	}
}

void UGsUIPlayerStatDistributionPanel::OnClickStatReset()
{
	if (nullptr == _bonusStatConfig)
	{
		return;
	}	

	UGsUIPopupCommonItemConsume* popupCommonItemConsume = Cast<UGsUIPopupCommonItemConsume>(GUI()->OpenAndGetWidget(TEXT("PopupCommonItemConsume")));
	if (nullptr == popupCommonItemConsume)
	{
		return;
	}
		
	int32 resetStatItemId = _bonusStatConfig->resetStatItem;
	popupCommonItemConsume->SetDataNotCheckItem(_topText, FText::GetEmpty(), resetStatItemId, 1,
		[resetStatItemId](bool InIsOk)
		{
			if (false == InIsOk)
			{
				return;
			}

			UGsItemManager* itemManager = GItem();
			if (nullptr == itemManager)
			{
				return;
			}			

			int32 resetStatItemCount = itemManager->GetAllItemCountByTID(resetStatItemId);
			if (0 >= resetStatItemCount)
			{
				return;
			}
			
			TArray<TWeakPtr<FGsItem>> Items = itemManager->FindByTID(resetStatItemId, ItemType::CONSUME);
			for (TWeakPtr<FGsItem>& Item : Items)
			{
				if (false == Item.IsValid())
				{
					continue;
				}

				itemManager->TryUseItem(Item.Pin()->GetDBID(), 1);
				break;
			}
		});
}

void UGsUIPlayerStatDistributionPanel::OnClickBonusStatItemCountExtend()
{
	if (nullptr == _bonusStatConfig)
	{
		return;
	}		

	int32 currentExtendedNumber = GGameData()->GetUserData()->GetBonusStatItemExtendCount();
	int32 maxExtendNumber = _bonusStatConfig->bonusStatItemConsumeMaxExtendList.Num();
	if (currentExtendedNumber < 0)
	{
		return;
	}		

	if (currentExtendedNumber == maxExtendNumber)
	{
		return;
	}		

	UGsUIPopupCommonPay* popupCommonPay = Cast<UGsUIPopupCommonPay>(GUI()->OpenAndGetWidget(TEXT("PopupCommonPay")));
	if (nullptr == popupCommonPay)
	{
		return;
	}		

	if (false == _bonusStatConfig->bonusStatItemConsumeMaxExtendList.IsValidIndex(currentExtendedNumber))
	{
		return;
	}		

	const FGsSchemaItemCommon* bonusStatItemData = FGsItem::GetTableDataByID(_bonusStatConfig->bonusStatItem);
	if (nullptr == bonusStatItemData)
	{
		return;
	}

	CurrencyType currencyType = _bonusStatConfig->bonusStatItemConsumeMaxExtendList[currentExtendedNumber].currencyType;
	int32 currencyCost = _bonusStatConfig->bonusStatItemConsumeMaxExtendList[currentExtendedNumber].currencyCost;	
	FText formatText = FText::Format(_bonusStatExtendMaxText, bonusStatItemData->name);
	popupCommonPay->SetData(formatText,
		FGsCostPackage::CreateSimple(currencyType, currencyCost),
		[&](bool InIsOk)
		{
			if (InIsOk)
			{
				FGsNetSendServiceWorld::SendReqBonusStatExtendCount();
			}
		});
}

void UGsUIPlayerStatDistributionPanel::InvalidateWidget()
{	
	//분배 중에 음식이나 버프 효과로 인해 스탯이 변경될 수 있으므로 스탯과 보너스 포인트 재계산
	if (_playerStatHandler)
	{
		_playerStatHandler->InitStat();
		_playerStatHandler->Recalculate();
	}

	InvalidateRemainBonusStatPoint();
	InvalidateStatDistributionButton();
	InvalidateEntryList();
}

void UGsUIPlayerStatDistributionPanel::InvalidateStatDistributionButton()
{
	if (nullptr == _playerStatHandler)
	{
		return;
	}
	
	if (0 == _playerStatHandler->GetRemainBonusStatPoint())
	{		
		_statDecisionButton->SetIsEnabled(false);
		_statDecisionButton->SetVisibility(ESlateVisibility::Hidden);
		_statDecisionButtonDisable->SetVisibility(ESlateVisibility::Visible);
		return;
	}

	if (0 == _playerStatHandler->GetDistributeStatPoint())
	{
		_statDecisionButton->SetIsEnabled(false);
		_statDecisionButton->SetVisibility(ESlateVisibility::Hidden);
		_statDecisionButtonDisable->SetVisibility(ESlateVisibility::Visible);
		return;
	}

	_statDecisionButton->SetIsEnabled(true);
	_statDecisionButton->SetVisibility(ESlateVisibility::Visible);
	_statDecisionButtonDisable->SetVisibility(ESlateVisibility::Hidden);
}

void UGsUIPlayerStatDistributionPanel::InitializeBonusStatItemInfo()
{
	if (nullptr == _bonusStatConfig)
	{
		return;
	}		

	const FGsSchemaItemCommon* bonusStatItemData = FGsItem::GetTableDataByID(_bonusStatConfig->bonusStatItem);
	if (nullptr == bonusStatItemData)
	{
		return;
	}		

	// 이미지 설정
	TWeakObjectPtr<UImage> weakThis(_imageBonusStatItem);
	FGsUIHelper::AsyncLoadSpriteFromPath(bonusStatItemData->icon, FStreamableDelegateParam::CreateWeakLambda(this, [weakThis](UObject* inRes)
		{
			if (UImage* strongThis = weakThis.Get())
			{
				strongThis->SetBrushFromAtlasInterface(Cast<UPaperSprite>(inRes));
			}
		}));

	// 롱프레스 설정
	if (false == _btnBonusStatItem->_onFinishedLongPressGauge.IsBoundToObject(this))
	{
		_longPressDelegate = _btnBonusStatItem->_onFinishedLongPressGauge.AddUObject(this, &UGsUIPlayerStatDistributionPanel::OnFinishedLongPress);
	}
	_btnBonusStatItem->OnChangeLongPress(true);

	InvalidateBonusStatItemInfo();
}

void UGsUIPlayerStatDistributionPanel::InvalidateBonusStatItemInfo()
{
	if (nullptr == _bonusStatConfig)
	{
		return;
	}		

	const FGsSchemaItemCommon* bonusStatItemData = FGsItem::GetTableDataByID(_bonusStatConfig->bonusStatItem);
	if (nullptr == bonusStatItemData)
	{
		return;
	}		

	int32 usedCount = GGameData()->GetUserData()->GetUsedBonusStatItemCount(); // 사용한 수량
	int32 maxCount = GGameData()->GetUserData()->GetBonusStatItemExtendCount() + _bonusStatConfig->bonusStatItemConsumeMax; // 최대 사용가능 수량
	FText countText;
	FGsUIStringHelper::GetTwoValueSlashedText(usedCount, maxCount, countText);
	EGsUIColorType countColor = (usedCount == maxCount) ? EGsUIColorType::HIGHLIGHT_TEXT_COLOR : EGsUIColorType::NORMAL_TEXT_COLOR;

	_textBlockBonusStatItem->SetText(bonusStatItemData->name);
	_textBlockItemCount->SetText(countText);
	_textBlockItemCount->SetColorAndOpacity(FGsUIColorHelper::GetColor(countColor));

	if (usedCount < maxCount)
	{
		_imageExtend->SetIsEnabled(false);
		_btnCountExtend->SetIsEnabled(false);
	}
	else if (usedCount == maxCount)
	{
		int32 currentExtendedNumber = GGameData()->GetUserData()->GetBonusStatItemExtendCount();
		int32 maxExtendNumber = _bonusStatConfig->bonusStatItemConsumeMaxExtendList.Num();
		if (currentExtendedNumber < maxExtendNumber)
		{
			_imageExtend->SetIsEnabled(true);
			_btnCountExtend->SetIsEnabled(true);
		}
		else
		{
			_imageExtend->SetIsEnabled(false);
			_btnCountExtend->SetIsEnabled(true);
		}
	}
	else
	{
#if WITH_EDITOR
		GSLOG(Warning, TEXT("[BonusStatItem] Used Count(%d) > MaxCount(%d)"), usedCount, maxCount);
#endif
	}
}

void UGsUIPlayerStatDistributionPanel::OnInvalidWidget(const struct IGsMessageParam* inParam)
{
	InvalidateRemainBonusStatPoint();
	InvalidateStatDistributionButton();
}

void UGsUIPlayerStatDistributionPanel::OnInvalidBonusStatItemCount(const struct IGsMessageParam* inParam)
{
	InvalidateBonusStatItemInfo();
}

void UGsUIPlayerStatDistributionPanel::OnFinishedLongPress()
{
	FGsItemDetailOpenMessageParamTID param(_bonusStatConfig->bonusStatItem, true, false, UGsUIPopupItemDetailMain::CenterPosType::CenterPos);
	GMessage()->GetItemContents().SendMessage(MessageContentItem::DETAIL_DISPLAY_ITEM_TID, &param);
}