// Fill out your copyright notice in the Description page of Project Settings.


#include "GsStatDistributionListEntry.h"
#include "Blueprint/UserWidget.h"
#include "Components/SizeBox.h"
#include "Components/ListView.h"
#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"
#include "UTIL/GsText.h"
#include "UTIL/GsTableUtil.h"
#include "GameObject/Stat/GsStatDefine.h"
#include "GameFlow/GsGameFlowGame.h"
#include "GameFlow/GameContents/ContentsGame/Hud/GsGameStateHud.h"
#include "GameFlow/GameContents/ContentsGame/Hud/Stat/GsPlayerStatDistributionController.h"
#include "GameFlow/GameContents/ContentsGame/Hud/Stat/GsPlayerStatHandler.h"
#include "GameFlow/GameContents/ContentsGame/GsContentsManagerGame.h"
#include "Stat/GsSchemaStatConvertData.h"
#include "Management/ScopeGlobal/GsGameFlowManager.h"
#include "Management/ScopeGame/GsGameObjectManager.h"
#include "Management/ScopeGlobal/GsUIManager.h"
#include "Management/GsMessageHolder.h"
#include "PanelWidget.h"
#include "UI/UIControlLib/ContentWidget/GsAccelationButton.h"
#include "UI/UIContent/Helper/GsUIColorHelper.h"
#include "UI/UIContent/Define/GsUIContentDefine.h"
#include "UI/UIContent/Define/EGsUIColorType.h"
#include "UI/UIContent/Popup/Stat/GsStatListViewEntry.h"
#include "UI/UIContent/Helper/GsUIHelper.h"
#include "Item/Data/GsItemStatData.h"

void UGsStatDistributionListEntry::NativeOnInitialized()
{
	Super::NativeOnInitialized();	
}

void UGsStatDistributionListEntry::NativeConstruct()
{
	Super::NativeConstruct();

	_decreaseButton->_shortTouchEvent.AddUObject(this, &UGsStatDistributionListEntry::OnDecreaseStat);
	_decreaseButton->_longTouchEvent.AddUObject(this, &UGsStatDistributionListEntry::OnLongTouchDecreaseStat);

	_increaseButton->_shortTouchEvent.AddUObject(this, &UGsStatDistributionListEntry::OnIncreaseStat);
	_increaseButton->_longTouchEvent.AddUObject(this, &UGsStatDistributionListEntry::OnLongTouchIncreaseStat);

	BindMessage();

	_statInfoSlotList.Empty();
	_statDistributionHandler = FGsPlayerStatHandler::GetInstance()->GetStatDistributionController();
}

void UGsStatDistributionListEntry::NativeDestruct()
{
	_statInfoSlotList.Empty();

	_decreaseButton->_shortTouchEvent.Clear();
	_decreaseButton->_longTouchEvent.Clear();

	_increaseButton->_shortTouchEvent.Clear();
	_increaseButton->_longTouchEvent.Clear();

	UnbindMessage();

	Super::NativeDestruct();
}

void UGsStatDistributionListEntry::NativeOnListItemObjectSet(UObject* inListItemObject)
{
	if (nullptr == inListItemObject)
	{
		return;
	}

	UGsStatDistributionListEntryData* data = Cast<UGsStatDistributionListEntryData>(inListItemObject);
	if (nullptr == data)
	{
		return;
	}

	_statType = data->_statType;	
	_invalidEvent = data->_invalidEvent;

	//init stat name
	InitStatName();
	InitSecondaryStatNames();

	InvalidWidget();
}

void UGsStatDistributionListEntry::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	IGsInvalidateUIInterface::CheckInvalidate();

	Super::NativeTick(MyGeometry, InDeltaTime);
}

void UGsStatDistributionListEntry::OnIncreaseStat()
{
	//increase stat distribution point
	IncreaseStat();
}

void UGsStatDistributionListEntry::OnLongTouchIncreaseStat(float inDelta)
{
	//increase stat distribution point
	IncreaseStat((int32)inDelta);
}

void UGsStatDistributionListEntry::OnDecreaseStat()
{	
	//Decrease stat distribution point
	DecreaseStat();
}

void UGsStatDistributionListEntry::OnLongTouchDecreaseStat(float inDelta)
{
	//decrease stat distribution point
	DecreaseStat((int32)inDelta);
}

void UGsStatDistributionListEntry::IncreaseStat(int32 inStatPoint)
{
	//increase stat distribution point
	if (nullptr == _statDistributionHandler)
	{
		return;
	}
	
	if (_statDistributionHandler->CanIncrease(_statType))
	{
		if (_statDistributionHandler->IncreaseStat(_statType, inStatPoint))
		{
			_statDistributionHandler->BackUpDistributedPoint();

			InvalidateAll();
		}

		GMessage()->GetUI().SendMessage(MessageUI::INVALID_PLAYER_STAT_DISTRIBUTION, nullptr);
	}
	else
	{
		if (_statDistributionHandler->IsMaxStat(_statType))
		{
			FText maxStatText;
			FText::FindText(TEXT("StatText"), TEXT("UI_StatMax"), maxStatText);
			FGsUIHelper::TraySectionMessageTicker(EGsNotiSection::EXTSYSTEM, maxStatText);
		}
	}
}

void UGsStatDistributionListEntry::DecreaseStat(int32 inStatPoint)
{
	//increase stat distribution point
	if (nullptr == _statDistributionHandler)
	{
		return;
	}

	if (_statDistributionHandler->CanDecrease(_statType))
	{
		_statDistributionHandler->DecreaseStat(_statType, inStatPoint);
		_statDistributionHandler->BackUpDistributedPoint();

		InvalidateAll();
	}

	GMessage()->GetUI().SendMessage(MessageUI::INVALID_PLAYER_STAT_DISTRIBUTION, nullptr);
}

void UGsStatDistributionListEntry::InvalidWidget()
{
	//invalidate stat value
	InvalidateAll();
	InvalidateButtons();
}

void UGsStatDistributionListEntry::InitStatName()
{
	if (_bonusStatNameText)
	{
		FString statName = FGsTextUtil::GetStringFromEnum(TEXT("StatType"), _statType);
		_bonusStatNameText->SetText(FText::FromString(statName));
	}
}

void UGsStatDistributionListEntry::InvalidateParents()
{
	if (_invalidEvent)
	{
		_invalidEvent();
	}
}

void UGsStatDistributionListEntry::InvalidateStatValueColor()
{
	if (nullptr == _bonusStatPoint)
	{
		return;
	}

	if (nullptr == _statDistributionHandler)
	{
		return;
	}

	if (_statDistributionHandler->GetCurrentDistributeStatPoint(_statType) > 0)
	{
		FLinearColor highlight = FGsUIColorHelper::GetColor(EGsUIColorType::HIGHLIGHT_TEXT_COLOR);
		_bonusStatPoint->SetColorAndOpacity(highlight);
	}
	else
	{
		FLinearColor normal = FGsUIColorHelper::GetColor(EGsUIColorType::NORMAL_TEXT_COLOR);
		_bonusStatPoint->SetColorAndOpacity(normal);
	}
}

void UGsStatDistributionListEntry::InvalidateSecondaryStatValueColor()
{
	if (nullptr == _statDistributionHandler)
	{
		return;
	}

	TArray<bool> changeArray;
	if (false == _statDistributionHandler->TryCheckChangeSecondaryStatValue(changeArray, _statType))
	{
		return;
	}
	
	//get detail stat value
	int32 statValueNum = changeArray.Num();
	int32 statSlotNum = _statInfoSlotList.Num();

	if (statValueNum != statSlotNum)
	{
		FString statTypeName = FGsTextUtil::GetStringFromEnum(TEXT("StatType"), _statType);
		GSLOG(Error, TEXT("Stat num and stat value num is different : %s"), *statTypeName);
		return;
	}

	for (int32 i = 0; i < statSlotNum; ++i)
	{
		if (UGsStatListViewEntry* listViewEntry = _statInfoSlotList[i])
		{
			listViewEntry->SetHighlight(changeArray[i]);
		}
	}
}

void UGsStatDistributionListEntry::InvalidateStatValue()
{
	if (nullptr == _statDistributionHandler)
	{
		return;
	}

	FText statValue;
	if (false == _statDistributionHandler->TryGetStatValue(statValue, _statType))
	{		
		return;
	}

	//get stat point
	_bonusStatPoint->SetText(statValue);	
}

void UGsStatDistributionListEntry::InitSecondaryStatNames()
{
	const FGsSchemaStatConvertData* statConvert = UGsTableUtil::FindRowById<UGsTableStatConvertData, FGsSchemaStatConvertData>(_statType);
	if (nullptr == statConvert)
	{
		return;
	}

	if (false == statConvert->convertList.IsValidIndex(0))
	{
		return;
	}

	FGsSchemaStatConvertTable convertData = statConvert->convertList[0];
	int32 statNum = convertData.statList.Num();

	_statVerticalBox->ClearChildren();
	_statInfoSlotList.Empty();

	UUserWidget* widget = nullptr;
	UGsStatListViewEntry* slot = nullptr;
	for (const FGsItemStatData& data : convertData.statList)
	{
		widget = CreateWidget(this, _statSlotTemplate);
		if (nullptr == widget)
		{
			continue;
		}
		
		slot = Cast<UGsStatListViewEntry>(widget);
		if (nullptr == slot)
		{
			continue;
		}

		slot->SetStatName(data.type);		
		_statVerticalBox->AddChild(widget);
		_statInfoSlotList.Add(slot);
	}
}

void UGsStatDistributionListEntry::InvalidateSecondaryStatValue()
{
	if (nullptr == _statDistributionHandler)
	{
		return;
	}


	TArray<FGsBonusStat> statInfoList;
	TArray<FText> statValueList;
	if (false == _statDistributionHandler->TryGetSecondaryStatValueAndText(statInfoList, statValueList, _statType))
	{
		return;
	}

	//get detail stat value
	int32 statInfoNum = statInfoList.Num();
	int32 statValueNum = statValueList.Num();
	int32 statSlotNum = _statInfoSlotList.Num();

	if (statValueNum != statSlotNum ||
		statValueNum != statInfoNum)
	{
		FString statTypeName = FGsTextUtil::GetStringFromEnum(TEXT("StatType"), _statType);
		GSLOG(Error, TEXT("Stat num and stat value num is different : %s"), *statTypeName);
		return;
	}

	for (int32 i = 0; i < statSlotNum; ++i)
	{
		if (UGsStatListViewEntry* listViewEntry = _statInfoSlotList[i])
		{
			int32 prevStatValue = listViewEntry->GetStatValue();
			if (prevStatValue != statInfoList[i].statValue)
			{
				if (prevStatValue != 0)
				{
					listViewEntry->ShowEffect();
				}
				listViewEntry->SetStatValue(statValueList[i], statInfoList[i].statValue);
			}
		}
	}
}

void UGsStatDistributionListEntry::InvalidateButtons()
{
	if (_statDistributionHandler)
	{
		bool enableIncrease = _statDistributionHandler->CanIncrease(_statType);
		_increaseButton->SetIsEnabled(enableIncrease);
		
		bool enableDecrase = _statDistributionHandler->CanDecrease(_statType);
		_decreaseButton->SetIsEnabled(enableDecrase);		
	}
}

void UGsStatDistributionListEntry::BindMessage()
{
	if (FGsMessageHolder* holder = GMessage())
	{
		_uiMessageHandlerArray.Empty();
		_uiMessageHandlerArray.Emplace(holder->GetUI().AddUObject(MessageUI::INVALID_PLAYER_STAT_DISTRIBUTION
			, this
			, &UGsStatDistributionListEntry::OnInvalidWidget));
	}
}

void UGsStatDistributionListEntry::UnbindMessage()
{
	if (FGsMessageHolder* holder = GMessage())
	{		
		for (const MsgUIHandle& pair : _uiMessageHandlerArray)
		{
			holder->GetUI().Remove(pair);
		}
		_uiMessageHandlerArray.Empty();
	}
}

void UGsStatDistributionListEntry::OnInvalidWidget(const struct IGsMessageParam* inParam)
{
	InvalidateButtons();
}

void UGsStatDistributionListEntry::InvalidateAllInternal()
{
	InvalidateStatValue();
	InvalidateStatValueColor();

	InvalidateSecondaryStatValue();
	InvalidateSecondaryStatValueColor();
}