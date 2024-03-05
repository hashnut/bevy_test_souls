// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Blueprint/UserWidget.h"
#include "Blueprint/IUserObjectListEntry.h"

#include "Shared/Shared/SharedInclude/SharedTypes.h"

#include "GsUIGuildAuctionListEntry.generated.h"


class UGsUIRedDotBase;
class UImage;
class UTextBlock;
class UGsHorizontalBoxIconSelector;
class UGsUIIconItemInventory;
class UWidgetSwitcher;
class UGsButton;
class UGsUIIconItem;
struct FGsGuildAuctionItem;

/**
 * 기사단 경매 목록 Entry
 */
UCLASS()
class T1PROJECT_API UGsUIGuildAuctionListEntry : public UUserWidget, public IUserObjectListEntry
{
	GENERATED_BODY()

protected:
	/* 레드닷 */
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUIRedDotBase* _uiRedDot;

	/* 선택 여부 */
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UImage* _imgSelected;

	/* 획득 정보 */
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockDate;

	/* 아이템 정보 */
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsHorizontalBoxIconSelector* _iconSelector;

	UPROPERTY()
	UGsUIIconItemInventory* _uiIcon;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockItemName;

	/* 입찰 */
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidgetSwitcher* _switcherBidder;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockBidderName;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockPrice;

	/* 정산 */
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnResult;

	/* 남은 시간 */
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockRemainTime;

	/* 입찰 */
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidgetSwitcher* _switcherBiddingType;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidgetSwitcher* _switcherBiddingNormal;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnBiddingNormalOn;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnBiddingNormalOff;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidgetSwitcher* _switcherBiddingAppointment;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnBiddingAppointmentOn;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnBiddingAppointmentOff;

protected:
	GuildAuctionId _auctionId = INVALID_GUILD_AUCTION_ID;

	FText _remainTimeText;

	FLinearColor _colorNormal;
	FLinearColor _colorAppointment;

protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	virtual void NativeOnListItemObjectSet(UObject* ListItemObject) override;

public:
	void SetIsSelected(bool bInIsSelected);

protected:
	UFUNCTION()
	void OnClickResult();
	UFUNCTION()
	void OnClickBidding();
	UFUNCTION()
	void OnClickBiddingOff();

	void OnLongPressIcon(UGsUIIconItem& InIcon);

public:
	void InvalidateAll();

	void SetUIItem(const FGsGuildAuctionItem* InAuctionItem);
	void SetUIBid(const FGsGuildAuctionItem* InAuctionItem);
	void SetUIState(const FGsGuildAuctionItem* InAuctionItem);
	bool CheckAndInvalidateRemainTime();

	GuildAuctionId GetAuctionId() { return _auctionId; }
};