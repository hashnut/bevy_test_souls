// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUIPopupItemReceive.h"
#include "RichTextBlock.h"
#include "TextBlock.h"
#include "Management/ScopeGame/GsMailManager.h"
#include "UI/UIControlLib/Control/GsDynamicIconSlotHelper.h"
#include "UI/UIControlLib/ContentWidget/GsButton.h"
#include "UI/UIControlLib/ContentWidget/GsWrapBoxIconSelector.h"
#include "../Common/Icon/GsUIIconItemReward.h"
#include "../Item/GsItem.h"
#include "GsUIPopupItemDetailMain.h"
#include "../Message/GsMessageContents.h"
#include "../Management/GsMessageHolder.h"
#include "../Message/MessageParam/GsItemContentsMessageParam.h"
#include "../Common/Icon/GsUIIconItem.h"
#include "../Mail/GsMailInfo.h"
#include "DataSchema/Item/GsSchemaItemCommon.h"
#include "../UI/UIControlLib/Control/GsDynamicPanelSlotHelper.h"
#include "UI/UIContent/Popup/Mail/GsUIItemReceiveEntry.h"
#include "UMG/Public/Components/ScrollBox.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"

void UGsUIPopupItemReceive::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	// 버튼 세팅
	_btnOk->OnClicked.AddDynamic(this, &UGsUIPopupItemReceive::OnClickOk);

	// 첨부 아이템 슬롯 관리
	_slotHelper = NewObject<UGsDynamicPanelSlotHelper>(this);
	_slotHelper->Initialize(_itemSlotClass, (UPanelWidget*)_itemScrollBox);
	_slotHelper->OnRefreshEntry.AddDynamic(this, &UGsUIPopupItemReceive::OnRefreshEntry);
}

void UGsUIPopupItemReceive::NativeDestruct()
{
	Super::NativeDestruct();
}

void UGsUIPopupItemReceive::BeginDestroy()
{
	if (_slotHelper)
	{
		_slotHelper->OnRefreshEntry.RemoveDynamic(this, &UGsUIPopupItemReceive::OnRefreshEntry);
	}
	_slotHelper = nullptr;

	Super::BeginDestroy();
}


void UGsUIPopupItemReceive::OnClickOk()
{	
	Clear();
	Close();
}

void UGsUIPopupItemReceive::Clear()
{
	_currencyList.Empty();
	_itemList.Empty();
	_itemEquipList.Empty();
	_mailItemEquipList.Empty();
}

void UGsUIPopupItemReceive::SetData(IN TMap<uint32, Currency>& inCurrencyList, IN TMap<uint32, ItemAmount>& inItemList)
{
	// 기존 데이터 삭제
	Clear();

	// 값 셋팅
	_currencyList = inCurrencyList;
	_itemList = inItemList;

	// 보상 아이템 아이콘 리스트 갯수 셋팅
	_slotHelper->RefreshAll(_currencyList.Num() + _itemList.Num());
}

void UGsUIPopupItemReceive::SetData(IN TArray<TSharedPtr<FGsMailAttachment>>& inMailDataList)
{
	// 기존 데이터 삭제
	Clear();

	for (TSharedPtr<FGsMailAttachment> attachmentIter : inMailDataList)
	{
		const MailAttachmentId attachmentId = attachmentIter->GetId();
		const MailAttachmentAmount attachmentAmount = attachmentIter->GetAmount();
		uint64 amount = attachmentAmount;

		switch (attachmentIter->GetType())
		{
		case MailAttachmentType::CURRENCY:
		{
			if (true == _currencyList.Contains(attachmentId))
				amount += _currencyList.FindRef(attachmentId);

			_currencyList.Add(attachmentId, amount);
		}
		break;
		case MailAttachmentType::ITEM:
		{
			const FGsSchemaItemCommon* SchemaItemCommon =
				UGsTableUtil::FindRowById<UGsTableItemCommon, FGsSchemaItemCommon>(attachmentId);

			ItemType itemType = ItemType::NONE;
			if (nullptr != SchemaItemCommon)
			{
				itemType = SchemaItemCommon->type;
			}
			else
			{
				GSLOG(Error, TEXT("UGsUIPopupItemReceive : Can't Find Item From SchemaItemCommon Table [%d]"), attachmentId);
			}

			if (itemType != ItemType::EQUIP)
			{
				if (true == _itemList.Contains(attachmentId))
					amount += _itemList.FindRef(attachmentId);

				_itemList.Add(attachmentId, amount);
			}
			else
			{
				_mailItemEquipList.Add(attachmentIter);
			}
		}
		break;
		default:
			break;
		}
	}

	int totalNum = _currencyList.Num() + _itemList.Num() + _mailItemEquipList.Num();
	if (0 < totalNum)
	{
		// 보상 아이템 아이콘 리스트 갯수 셋팅
		_slotHelper->RefreshAll(_currencyList.Num() + _itemList.Num() + _mailItemEquipList.Num());
	}
	else
	{
		Close();
	}
}

void UGsUIPopupItemReceive::SetData(IN TArray<ItemDataCreated>& itemDBIdAmountPairList, IN TArray<CurrencyPair>& inCurrencyPairList)
{
	Clear();

	for (const ItemDataCreated& item : itemDBIdAmountPairList)
	{
		ItemId itemId = item.mTemplateId;
		ItemAmount itemAmount = item.mItemAmount;

		if (0 >= itemId)
			continue;

		const FGsSchemaItemCommon* SchemaItemCommon =
			UGsTableUtil::FindRowById<UGsTableItemCommon, FGsSchemaItemCommon>(itemId);

		if (nullptr == SchemaItemCommon)
		{
			GSLOG(Error, TEXT("UGsUIPopupItemReceive : Can't Find Item From ItemTable [%d]"), itemId);
			continue;
		}

		if (SchemaItemCommon->type != ItemType::EQUIP)
		{
			if (true == _itemList.Contains(itemId))
				itemAmount += _itemList.FindRef(itemId);

			_itemList.Add(itemId, itemAmount);
		}
		else
		{
			_itemEquipList.Add(&item);
		}
	}

	for (const CurrencyPair& currency : inCurrencyPairList)
	{
		uint32 currencyType = (uint32)currency.mCurrencyType;
		int amount = (int)currency.mCurrency;

		if (0 >= amount)
			continue;

		if (true == _currencyList.Contains(currencyType))
			amount += _currencyList.FindRef(currencyType);

		_currencyList.Add(currencyType, amount);
	}

	// 보상 아이템 아이콘 리스트 갯수 셋팅
	_slotHelper->RefreshAll(_currencyList.Num() + _itemList.Num() + _itemEquipList.Num());
}

void UGsUIPopupItemReceive::OnRefreshEntry(int32 InIndex, class UWidget* InEntry)
{
	UGsUIItemReceiveEntry* entry = Cast<UGsUIItemReceiveEntry>(InEntry);
	if (nullptr == entry)
		return;

	int itemNum = _itemList.Num();
	int currencyNum = _currencyList.Num();
	int itemEquipNum = _itemEquipList.Num();
	if (InIndex < currencyNum)
	{
		int index = 0;
		for (TMap<uint32, Currency>::TIterator it = _currencyList.CreateIterator(); it; ++it, ++index)
		{
			if (index == InIndex)
			{
				uint32 key = it.Key();
				entry->SetCurrencyData((CurrencyType)key, it.Value());
			}
		}
	}
	else if (InIndex < currencyNum + itemNum)
	{
		int index = currencyNum;
		for (TMap<uint32, ItemAmount>::TIterator it = _itemList.CreateIterator(); it; ++it, ++index)
		{
			if (index == InIndex)
			{
				uint32 key = it.Key();
				entry->SetItemData(key, it.Value());
			}
		}
	}
	else if (InIndex < currencyNum + itemNum + itemEquipNum)
	{
		int index = currencyNum + itemNum;
		for (TArray<const ItemDataCreated*>::TIterator it = _itemEquipList.CreateIterator(); it; ++it, ++index)
		{
			if (index == InIndex)
			{		
				const ItemDataCreated* itemEquip = *it;
				if (itemEquip)
				{
					entry->SetEquipItemData(itemEquip->mTemplateId, itemEquip->mItemAmount,
						itemEquip->mLevel, itemEquip->mEnchantShield, itemEquip->mEnchantBonusIndex);
				}
			}
		}
	}
	else
	{
		int index = currencyNum + itemNum + itemEquipNum;
		for (TArray<TSharedPtr<FGsMailAttachment>>::TIterator it = _mailItemEquipList.CreateIterator(); it; ++it, ++index)
		{
			if (index == InIndex)
			{
				TSharedPtr<FGsMailAttachment> mailAttachment = it->ToSharedRef();
				if (mailAttachment.IsValid())
				{
					entry->SetEquipItemData(mailAttachment->GetId(), mailAttachment->GetAmount(), 
						mailAttachment->GetLevel(), mailAttachment->GetEnchantShield(), mailAttachment->GetEnchantBonusIndex());
				}
			}
		}
	}
}

void UGsUIPopupItemReceive::OnInputCancel()
{
	Clear();
	Close();
}