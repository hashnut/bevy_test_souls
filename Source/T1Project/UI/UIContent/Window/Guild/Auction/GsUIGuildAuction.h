// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "UI/UIContent/Window/Guild/Auction/GsUIGuildStorageBase.h"

#include "Shared/Shared/SharedInclude/SharedTypes.h"

#include "GsUIGuildAuction.generated.h"


class UWidgetSwitcher;
class UGsUIGuildStorageComboBox;
struct FGsGuildAuctionItem;
/**
 * 기사단 정산 - 경매 페이지
 */
UCLASS()
class T1PROJECT_API UGsUIGuildAuction : public UGsUIGuildStorageBase
{
	GENERATED_BODY()

protected:
	// 경매 목록 <-> 빈 목록
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidgetSwitcher* _switcherList;

	// 경매 목록
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UListView* _listView;

	// 기사단 채팅 <-> 숫자 패드
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidgetSwitcher* _switcherChat;

	// 기사단 채팅 & 매크로
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UGsUIGuildAuctionChat* _uiChat;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UGsUIGuildAuctionChatMacro* _uiChatMacro;

	// 숫자 패드
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UGsUINumpadDefault* _uiNumpad;

	// 입찰금 입력 영역
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UEditableText* _inputBidPrice;

	// 입찰금 입력 숫자패드 On/Off 버튼
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UGsCheckBox* _checkBoxNumpad;

	// 목록 필터
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUIGuildStorageComboBox* _comboItemCategory;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUIGuildStorageComboBox* _comboBid;

protected:
	class FGsGuildAuctionData* _cachedAuctionData = nullptr;

	TArray<GuildAuctionId> _auctionIdList;

	Currency _bidPrice = 0;
	Currency _maxAuctionPrice = 0;

	const int32 AUCTION_EXIST = 0;
	const int32 AUCTION_NONE = 1;

	bool _requestList = false;

protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

public:
	virtual void Show(bool InIsStackRecoverFlag) override;
	virtual void Hide() override;

	virtual void InvalidateTick() override;

	void InvalidateList();
	void OnReceiveAuctionList();
	void OnUpdatedAuctionItem(uint64 InAuctionId);
	void OnRemovedAuctionItem(uint64 InAuctionId);

	void SendRequestPacket();

protected:
	UFUNCTION()
	void OnChangeCheckNumpad(int32 InIndexGroup, bool InIsChecked);
	UFUNCTION()
	void OnTextChangedBidPrice(const FText& InText);
	UFUNCTION()
	void OnTextCommittedBidPrice(const FText& InText, ETextCommit::Type InType);
	
	void OnClickAuctionEntry(UObject* InListItemData);
	void OnClickedMacro();
	void OnClickedMacroEntry(const FText& InMacroText);
	void OnChangeNumpadNumber(int32 InNumber);
	void OnListViewScrolled(float InOffsetInItems, float InDistanceRemaining);

	void OnClickComboMenuCategory();
	void OnClickComboMenuBid();
	void OnSelectComboBoxCategory(int32 InMainIndex, int32 InSubIndex);
	void OnSelectComboBoxBid(int32 InIndex);

	void SetBidPrice(Currency InPrice);
	void SetBidPriceWithNumpad(Currency InPrice);
	Currency GetPriceFromText(const FText& InText);
};