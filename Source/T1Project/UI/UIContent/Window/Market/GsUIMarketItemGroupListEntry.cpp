// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUIMarketItemGroupListEntry.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "UI/UIControlLib/ContentWidget/GsButton.h"
#include "UI/UIControlLib/ContentWidget/GsToggleButton.h"
#include "UI/UIControlLib/ContentWidget/GsBorder.h"
#include "UI/UIContent/Helper/GsUIHelper.h"
#include "UI/UIContent/Helper/GsUIColorHelper.h"
#include "UI/UIContent/Window/Market/List/GsMarketItemGroupListItem.h"
#include "Market/GsMarketItemGroupData.h"
#include "Market/GsMarketHelper.h"
#include "DataSchema/Market/GsSchemaMarketConfig.h"
#include "Net/GsNetSendServiceMarket.h"
#include "Management/ScopeGame/GsMarketManager.h"
#include "Item/GsItem.h"
#include "Item/GsItemEquipment.h"
#include "Item/GsItemStat.h"


void UGsUIMarketItemGroupListEntry::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	_tBtnBookmark->OnClicked.AddDynamic(this, &UGsUIMarketItemGroupListEntry::OnClickBookmark);
	_borderLongPress->OnFinishedLongPress.BindUObject(this, &UGsUIMarketItemGroupListEntry::OnFinishedLongPress);
}

void UGsUIMarketItemGroupListEntry::NativeDestruct()
{
	_itemId = 0;
	_bIsExist = false;

	Super::NativeDestruct();
}

void UGsUIMarketItemGroupListEntry::NativeOnListItemObjectSet(UObject* ListItemObject)
{
	UGsMarketItemGroupListItem* listItem = Cast<UGsMarketItemGroupListItem>(ListItemObject);
	if (nullptr == listItem)
	{
		return;
	}

	InvalidateAll(listItem);
}

void UGsUIMarketItemGroupListEntry::RefreshAll()
{
	InvalidateAll(GetListItem<UGsMarketItemGroupListItem>());
}

bool UGsUIMarketItemGroupListEntry::IsSlotLongPressed() const
{
	return _borderLongPress->IsLongPressed();
}

void UGsUIMarketItemGroupListEntry::InvalidateAll(UGsMarketItemGroupListItem* InListItem)
{
	_bIsWorldMarket = InListItem->_bIsWorldMarket;

	const FGsMarketItemGroupData* data = InListItem->_data;
	if (nullptr == data)
	{
		return;
	}

	const FGsSchemaItemCommon* table = data->GetTable();
	if (nullptr == table)
	{
		return;
	}

	_itemId = table->id;

	// 이름
	SetUIItemName(table->name, 0, table->grade);

	// 아이콘
	if (_iconPtr.IsValid())
	{
		_iconPtr->SetNotOwnItem(data->GetItemId(), 1, InListItem->_enchantLevel);
		// 롱프레스 타겟으로 설정
		_borderLongPress->SetLongpressBarTarget(_iconPtr.Get());
	}

	// 스탯 표시
	SetUIItemStat(_textBlockStat);

	FGsMarketControllerBase* controller = GSMarket()->GetController(_bIsWorldMarket);
	if (nullptr == controller)
	{
		return;
	}

	// 즐겨찾기 표시
	_tBtnBookmark->SetIsSelected(controller->IsBookmarkedItem(data->GetItemId()));

	// 최저판매단가, 판매목록 표시. 존재하지 않으면 0으로 세팅
	_bIsExist = InListItem->IsSalesItemExist();
	if (_bIsExist)
	{
		FText textLowestPrice;
		FGsMarketHelper::GetMarketPriceText(InListItem->GetLowestPrice(), true, textLowestPrice);

		_textBlockLowestPrice->SetText(textLowestPrice);
		_textBlockSalesCount->SetText(FText::AsNumber(InListItem->GetSalesCount()));
		_imgDimmed->SetVisibility(ESlateVisibility::Collapsed);
	}
	else
	{
		_textBlockLowestPrice->SetText(FText::AsNumber(0));
		_textBlockSalesCount->SetText(FText::AsNumber(0));

		// 주의: 아이콘 롱프레스 때문에 Visible로 설정하지 않는다
		_imgDimmed->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}
}


void UGsUIMarketItemGroupListEntry::OnClickBookmark()
{
	// 주의: 초기화 시 패킷 보내지 않도록 주의할 것. 테스트 필요.
	if (0 == _itemId)
	{
		return;
	}

	FGsMarketControllerBase* controller = GSMarket()->GetController(_bIsWorldMarket);
	if (nullptr == controller)
	{
		return;
	}


	// 토글을 위해 반대부호
	bool bIsChecked = !_tBtnBookmark->GetIsSelected();
	if (bIsChecked)
	{
		// 최대개수 체크
		if (const FGsSchemaMarketConfig* config = FGsMarketHelper::GetMarketConfig())
		{
			int32 maxBookmarkCount = _bIsWorldMarket ? config->wdMaxBookmarkCount : config->maxBookmarkCount;

			if (maxBookmarkCount <= controller->GetBookmarkCount())
			{
				// TEXT: 즐겨찾기 최대 등록개수를 초과합니다.
				FText textMsg;
				if (FText::FindText(TEXT("MarketText"), TEXT("Ticker_Bookmark_InvalidMaxCount"), textMsg))
				{
					FGsUIHelper::TraySectionMessageTicker(EGsNotiSection::EXTSYSTEM, textMsg);
				}

				return;
			}

			FGsNetSendServiceMarket::SendReqBookmarkRegister(_bIsWorldMarket, _itemId);
		}
	}
	else
	{
		FGsNetSendServiceMarket::SendReqBookmarkDeregistered(_bIsWorldMarket, _itemId);
	}
}

void UGsUIMarketItemGroupListEntry::OnFinishedLongPress()
{
	OpenItemDetailInfo(GetItem());
}
