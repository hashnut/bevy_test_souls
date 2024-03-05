//// Fill out your copyright notice in the Description page of Project Settings.
//
//#pragma once
//
//#include "CoreMinimal.h"
//
//#include "UI/UIContent/Window/Guild/GsUIGuildContentBase.h"
//
//#include "UI/UIControlLib/Control/GsToggleGroup.h"
//
//#include "GsUIGuildContentAuction.generated.h"
//
//
//class UWidgetSwitcher;
//class UScrollBox;
//class UGsDynamicPanelSlotHelper;
//class UGsUIGuildAuctionChat;
//class UGsUIGuildAuctionChatMacro;
//class UGsUINumpadDefault;
//class UEditableText;
//class UGsCheckBox;
//class UTextBlock;
//class UGsButton;
//class UPanelWidget;
//struct FGsGuildAuctionItem;
///**
// * 기사단 경매 페이지
// */
//
//UCLASS()
//class T1PROJECT_API UGsUIGuildContentAuction : public UGsUIGuildContentBase
//{
//	GENERATED_BODY()
//
//protected:
//	// 경매 목록 <-> 빈 목록
//	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
//	UWidgetSwitcher* _switcherList;
//
//	/* 경매 목록 */
//	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GsUI")
//	TSubclassOf<UUserWidget> _entryWidgetClass;
//
//	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
//	UScrollBox* _scrollBox;
//
//	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
//	UPanelWidget* _panelAuctionList;
//
//	UPROPERTY()
//	UGsDynamicPanelSlotHelper* _scrollBoxHelper;
//
//	// 기사단 채팅 <-> 숫자 패드
//	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
//	UWidgetSwitcher* _switcherChat;
//
//	// 기사단 채팅 & 매크로
//	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
//	UGsUIGuildAuctionChat* _uiChat;
//
//	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
//	UGsUIGuildAuctionChatMacro* _uiChatMacro;
//
//	// 숫자 패드
//	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
//	UGsUINumpadDefault* _uiNumpad;
//
//	// 입찰금 입력 영역
//	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
//	UEditableText* _inputBidPrice;
//
//	// 입찰금 입력 숫자패드 On/Off 버튼
//	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
//	UGsCheckBox* _checkBoxNumpad;
//
//	// 경매 등록 수
//	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
//	UTextBlock* _textBlockRegister;
//
//	// 경매 수수료
//	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
//	UTextBlock* _textBlockCommission;
//
//	// 회수 가능한 금액
//	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
//	UTextBlock* _textBlockPayback;
//
//	// 회수 버튼
//	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
//	UGsButton* _btnPayback;
//
//protected:
//	FGsToggleGroup _toggleGroupAuction;
//
//	TArray<const FGsGuildAuctionItem*> _auctionItemList;
//
//	Currency _bidPrice = 0;
//	Currency _maxAuctionPrice = 0;
//
//public:
//	virtual FString GetTabName() const override { return TEXT("Auction"); }
//
//protected:
//	virtual void NativeOnInitialized() override;
//	virtual void NativeDestruct() override;
//
//public:
//	// 메인 탭 변경 후 필요한 패킷을 요청
//	virtual void SendRequestPacket() override;
//	virtual void Show(bool bIsMyGuild, bool IsStackRecoverFlag = false) override;
//	virtual void Hide() override;
//	
//	void UpdateList();
//	void InvalidateList();
//	void InvalidateCommission();
//	void InvalidatePayback();
//	void OnReceiveAuctionList();
//	void OnReceiveAuctionDetail();
//	void InvalidateItem(uint64 InAuctionId);
//	void OnResetAuctionList();
//
//protected:
//	UFUNCTION()
//	void OnRefreshEntryAuction(int32 InIndex, class UWidget* InEntry);
//	UFUNCTION()
//	void OnChangeCheckNumpad(int32 InIndexGroup, bool InIsChecked);
//	UFUNCTION()
//	void OnTextChangedBidPrice(const FText& InText);
//	UFUNCTION()
//	void OnTextCommittedBidPrice(const FText& InText, ETextCommit::Type InType);
//	UFUNCTION()
//	void OnClickPayback();
//
//	void OnClickAuctionEntry(int32 InIndex, bool bInIsSelected);
//	void OnClickBidding(const FGsGuildAuctionItem* InAuctionItem);
//	void OnClickedMacro();
//	void OnClickedMacroEntry(const FText& InMacroText);
//	void OnChangeNumpadNumber(int32 InNumber);
//
//protected:
//	void SetBidPrice(Currency InPrice);
//	void SetBidPriceWithNumpad(Currency InPrice);
//	Currency GetPriceFromText(const FText& InText);
//
//};