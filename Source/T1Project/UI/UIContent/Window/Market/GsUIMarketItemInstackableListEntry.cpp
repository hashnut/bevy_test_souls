// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUIMarketItemInstackableListEntry.h"
#include "Components/TextBlock.h"
#include "UI/UIControlLib/ContentWidget/GsBorder.h"
#include "UI/UIControlLib/ContentWidget/GsCheckBox.h"
#include "UI/UIContent/Helper/GsUIColorHelper.h"
#include "Market/GsMarketItemData.h"
#include "Market/GsMarketHelper.h"
#include "Item/GsItem.h"
#include "Item/GsItemEquipment.h"
#include "Item/GsItemStat.h"
#include "GameObject/Stat/GsStatHelper.h"
#include "Shared/Client/SharedEnums/SharedStatEnum.h"
#include "Management/GsMessageHolder.h"
#include "Management/ScopeGame/GsMarketManager.h"
#include "Message/MessageParam/GsMarketMessageParam.h"


void UGsUIMarketItemInstackableListEntry::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	_borderLongPress->OnFinishedLongPress.BindUObject(this, &UGsUIMarketItemInstackableListEntry::OnFinishedLongPress);
	_checkBoxSelect->OnCheckStateChangedEx.AddDynamic(this, &UGsUIMarketItemInstackableListEntry::OnClickCheckBox);
}

void UGsUIMarketItemInstackableListEntry::NativeDestruct()
{
	_listItemPtr = nullptr;

	Super::NativeDestruct();
}

void UGsUIMarketItemInstackableListEntry::NativeOnListItemObjectSet(UObject* ListItemObject)
{
	_listItemPtr = Cast<UGsMarketItemListItem>(ListItemObject);
	if (false == _listItemPtr.IsValid())
	{
		return;
	}

	InvalidateAll(_listItemPtr.Get());
}

FGsItem* UGsUIMarketItemInstackableListEntry::GetItem()
{
	if (_listItemPtr.IsValid())
	{
		return (_listItemPtr->_data) ? _listItemPtr->_data->_item : nullptr;
	}

	return nullptr;
}

void UGsUIMarketItemInstackableListEntry::RefreshAll()
{
	InvalidateAll(GetListItem<UGsMarketItemListItem>());
}

bool UGsUIMarketItemInstackableListEntry::IsSlotLongPressed() const
{
	return _borderLongPress->IsLongPressed();
}

void UGsUIMarketItemInstackableListEntry::InvalidateAll(UGsMarketItemListItem* InListItem)
{
	_transactionId = INVALID_MARKET_TRANSACTION_ID;
	_bIsWorldMarket = InListItem->_bIsWorldMarket;

	const FGsMarketItemData* data = InListItem->_data;
	if (nullptr == data)
	{
		return;
	}

	const FGsSchemaItemCommon* table = data->GetTable();
	if (nullptr == table)
	{
		return;
	}

	_transactionId = data->_transactionId;

	// 이름
	SetUIItemName(table->name, data->GetEnchantLevel(), table->grade);

	// 아이콘
	if (_iconPtr.IsValid())
	{
		_iconPtr->SetNotOwnItem(data->GetItemId(), data->GetAmount(), data->GetEnchantLevel());
		_iconPtr->SetSlotEffectMagicalForge(data->_item);
		// 롱프레스 타겟으로 설정
		_borderLongPress->SetLongpressBarTarget(_iconPtr.Get());
	}

	// 스탯 표시
	SetUIItemStat(_textBlockStat);

	// 가격
	_textBlockPrice->SetText(FText::AsNumber(data->_price));

	if (nullptr == data->_item)
	{
		return;
	}

	FGsItemEquipment* equipItem = data->_item->GetCastItem<FGsItemEquipment>();
	if (nullptr == equipItem)
	{
		return;
	}

	// 제련옵션
	SetUIRefineOption(equipItem);

	// 실드 횟수
	_textBlockShieldCount->SetText(FText::AsNumber(equipItem->GetEnchantShieldCount()));

	// 특성
	SetUIEnchantProperty(equipItem);

	// 체크표시
	bool bIsChecked = false;
	if (FGsMarketManager* marketMgr = GSMarket())
	{
		if (0 < _transactionId)
		{
			bIsChecked = marketMgr->IsMultiplePurchaseTarget(_bIsWorldMarket, _transactionId);
		}	
	}

	_checkBoxSelect->SetIsSelected(bIsChecked);
}

void UGsUIMarketItemInstackableListEntry::SetUIEnchantProperty(FGsItemEquipment* InEquipItem)
{
	const FString strPropName = InEquipItem->GetBonusStatTitleName();
	if (strPropName.IsEmpty())
	{
		_textBlockProp->SetVisibility(ESlateVisibility::Hidden);
	}
	else
	{
		_textBlockProp->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		_textBlockProp->SetText(FText::FromString(InEquipItem->GetBonusStatTitleName()));
		_textBlockProp->SetColorAndOpacity(FGsUIColorHelper::GetColorItemGrade(InEquipItem->GetGradeType()));
	}
}

void UGsUIMarketItemInstackableListEntry::SetUIRefineOption(FGsItemEquipment* InEquipItem)
{
	TArray<FGsItemStat*> statDataList;
	if (false == InEquipItem->GetAllRefineOptionStatList(statDataList))
	{
		_textBlockRefineOption->SetVisibility(ESlateVisibility::Collapsed);
		return;
	}

	_textBlockRefineOption->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

	// 4줄만 표현한다
	int32 lineCount = 4;
	FString strRefine;

	for (int32 i = 0; i < statDataList.Num(); ++i)
	{
		if (0 >= lineCount)
		{
			break;
		}

		if (false == strRefine.IsEmpty())
		{
			strRefine.Append(TEXT("\n"));
		}

		FGsItemStat* statData = statDataList[i];
		FGsStatHelper::GetDesplayStatNameAndValue(statData->GetStatType(), statData->GetStatOriValue(), strRefine);

		--lineCount;
	}

	// 4줄 안찼으면 제련 보너스도 찍음
	if (0 < lineCount)
	{
		TPair<StatType, int32> bonusStatData; // statType , value(만분율)
		InEquipItem->GetEquipItemRefineBonusStatInfo(bonusStatData);
		if (false == strRefine.IsEmpty())
		{
			strRefine.Append(TEXT("\n"));
		}

		StatType statType = bonusStatData.Key;
		int32 statValue = bonusStatData.Value;

		FText statName;
		FText valueText;
		FGsStatHelper::GetDisPlayStatName(statType, statName);
		FGsStatHelper::GetDisPlayStatValueWithMarkFormat(statType, statValue, valueText);

		strRefine.Append(FString::Format(TEXT("{0} {1}"), {
			*statName.ToString(), valueText.ToString() }));

		/*
		// 만분율 값을 퍼센트로 변환하므로 value / 10000 * 100 = value * 0.01
		strRefine.Append(FString::Format(TEXT("{0} {1}%"), {
			*statName.ToString(), FMath::FloorToInt(statValue * 0.01f) }));
			* */
	}

	_textBlockRefineOption->SetText(FText::FromString(strRefine));
}

void UGsUIMarketItemInstackableListEntry::OnFinishedLongPress()
{
	OpenItemDetailInfo(GetItem());
}

void UGsUIMarketItemInstackableListEntry::OnClickCheckBox(int32 IndexInGroup, bool bIsChecked)
{
	if (0 == _transactionId)
	{
		return;
	}
	
	// 최대 갯수 체크
	if (bIsChecked)
	{
		int32 count = GSMarket()->GetMultiplePurchaseCount(_bIsWorldMarket);
		int32 maxCount = FGsMarketHelper::GetMaxMultiplePurchaseCount();
		if (maxCount <= count)
		{
			// 선택 취소
			_checkBoxSelect->SetIsSelected(false);

			return;
		}
	}

	FGsMarketMsgParamId param(_transactionId);

	if (bIsChecked)
	{
		GMessage()->GetMarket().SendMessage(MessageContentMarket::ADD_TO_PURCHASE_LIST, &param);
	}
	else
	{
		GMessage()->GetMarket().SendMessage(MessageContentMarket::REMOVE_FROM_PURCHASE_LIST, &param);
	}
}
