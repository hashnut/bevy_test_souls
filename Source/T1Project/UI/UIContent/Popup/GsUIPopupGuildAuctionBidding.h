// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "UI/UILib/Base/GsUIPopup.h"

#include "Shared/Shared/SharedInclude/SharedTypes.h"

#include "GsUIPopupGuildAuctionBidding.generated.h"


class UGsButton;
class UTextBlock;
class UGsWrapBoxIconSelector;
class UGsUIIconItemInventory;
class FGsItem;
class UGsUIIconItem;
struct FGsGuildAuctionItem;
/**
 * 기사단 경매 입찰 팝업
 */
UCLASS()
class T1PROJECT_API UGsUIPopupGuildAuctionBidding : public UGsUIPopup
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnOk;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnCancel;

	// 아이템 정보
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockItemName;
	
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsWrapBoxIconSelector* _iconSelector;

	UPROPERTY()
	UGsUIIconItemInventory* _uiIcon;

	// 입찰금
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockPrice;

	/**
	 * 안내 문구 : GuildAuctionUIText - Auction_Bid_Popup_Description
	 * 입찰금 : GuildAuctionUIText - Auction_Bidding
	 */

protected:
	GuildAuctionId _auctionId = INVALID_GUILD_AUCTION_ID;
	Currency _bidPrice = 0;

protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

	virtual void OnInputCancel() override;

protected:
	UFUNCTION()
	void OnClickOk();

	UFUNCTION()
	void OnClickCancel();

	void OnLongPressIcon(UGsUIIconItem& InIcon);

public:
	void SetData(const FGsGuildAuctionItem* InAuctionItem, Currency InPrice);
	GuildAuctionId GetAuctionId() { return _auctionId; }
	Currency GetBidPrice() { return _bidPrice; }

};
