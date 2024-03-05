//// Fill out your copyright notice in the Description page of Project Settings.
//
//
//#include "GsUIGuildContentAuction.h"
//
//#include "Components/WidgetSwitcher.h"
//#include "Components/ScrollBox.h"
//#include "Components/TextBlock.h"
//#include "Components/EditableText.h"
//
//#include "Currency/GsCurrencyHelper.h"
//
//#include "Management/GsMessageHolder.h"
//
//#include "Management/ScopeGlobal/GsUIManager.h"
//#include "Management/ScopeGlobal/GsGuildManager.h"
//
//#include "UI/UIControlLib/ContentWidget/GsButton.h"
//#include "UI/UIControlLib/Control/GsDynamicPanelSlotHelper.h"
//#include "UI/UIControlLib/ContentWidget/GsCheckBox.h"
//
//#include "UI/UIContent/Helper/GsUIHelper.h"
//#include "UI/UIContent/Common/Numpad/GsUINumpadDefault.h"
//#include "UI/UIContent/Popup/GsUIPopupGuildAuctionBidding.h"
//#include "UI/UIContent/Popup/GsUIPopupGuildAuctionResult.h"
//
//#include "UI/UIContent/Window/GsUIWindowGuildMain.h"
//#include "UI/UIContent/Window/Guild/Auction/GsUIGuildAuctionChat.h"
//#include "UI/UIContent/Window/Guild/Auction/GsUIGuildAuctionChatMacro.h"
//#include "UI/UIContent/Window/Guild/Auction/GsUIGuildAuctionListEntry.h"
//
//#include "UTIL/GsTimeSyncUtil.h"
//
//#include "Net/GsNetSendServiceGuild.h"
//
//
//void UGsUIGuildContentAuction::NativeOnInitialized()
//{
//	Super::NativeOnInitialized();
//
//	if (nullptr == _scrollBoxHelper)
//	{
//		_scrollBoxHelper = NewObject<UGsDynamicPanelSlotHelper>(this);
//		_scrollBoxHelper->Initialize(_entryWidgetClass, _panelAuctionList);
//		_scrollBoxHelper->OnRefreshEntry.AddDynamic(this, &UGsUIGuildContentAuction::OnRefreshEntryAuction);
//	}
//	
//	if (_checkBoxNumpad)
//	{
//		_checkBoxNumpad->OnCheckStateChangedEx.AddDynamic(this, &UGsUIGuildContentAuction::OnChangeCheckNumpad);
//	}
//
//	if (_inputBidPrice)
//	{
//		_inputBidPrice->OnTextChanged.AddDynamic(this, &UGsUIGuildContentAuction::OnTextChangedBidPrice);
//		_inputBidPrice->OnTextCommitted.AddDynamic(this, &UGsUIGuildContentAuction::OnTextCommittedBidPrice);
//	}
//
//	if (_btnPayback)
//	{
//		_btnPayback->OnClicked.AddDynamic(this, &UGsUIGuildContentAuction::OnClickPayback);
//	}
//
//	if (_uiNumpad)
//	{
//		_uiNumpad->OnChangeNumber().BindUObject(this, &UGsUIGuildContentAuction::OnChangeNumpadNumber);
//	}
//
//	if (_uiChat)
//	{
//		_uiChat->OnClickedMacro.BindUObject(this, &UGsUIGuildContentAuction::OnClickedMacro);
//	}
//	if (_uiChatMacro)
//	{
//		_uiChatMacro->OnClickedMacroEntry.BindUObject(this, &UGsUIGuildContentAuction::OnClickedMacroEntry);
//	}
//
//	_toggleGroupAuction.OnSlotClicked.BindUObject(this, &UGsUIGuildContentAuction::OnClickAuctionEntry);
//	//_toggleGroupAuction.OnSelectChanged.BindUObject(this, &UGsUIGuildContentAuction::OnClickAuctionEntry);
//}
//
//void UGsUIGuildContentAuction::NativeDestruct()
//{
//	_auctionItemList.Empty();
//
//	Super::NativeDestruct();
//}
//
//void UGsUIGuildContentAuction::SendRequestPacket()
//{
//	FGsGuildManager* guildManager = GGuild();
//	if (nullptr == guildManager)
//		return;
//
//	FGsGuildAuctionData* auctionData = guildManager->GetMyGuildAuctionData();
//	if (nullptr == auctionData)
//		return;
//
//	auctionData->SendReqAuctionList();
//}
//
//void UGsUIGuildContentAuction::Show(bool bIsMyGuild, bool IsStackRecoverFlag /*= false*/)
//{
//	FGsGuildManager* guildManager = GGuild();
//	if (nullptr == guildManager)
//		return;
//
//	FGsGuildAuctionData* auctionData = guildManager->GetMyGuildAuctionData();
//	if (nullptr == auctionData)
//		return;
//
//	/*기사단 채팅*/
//	const int32 ENABLE_CHAT = 0;
//	_switcherChat->SetActiveWidgetIndex(ENABLE_CHAT);
//	_uiChat->Show();
//	_uiChatMacro->Hide();
//
//	/*입찰금 & 숫자패드*/
//	_checkBoxNumpad->SetIsSelected(false);
//	_maxAuctionPrice = auctionData->GetMaxAuctionPrice();
//	_uiNumpad->SetMaxNumber(_maxAuctionPrice);
//	_uiNumpad->SetNumber(0, true);
//	SetBidPrice(0);
//
//	/*경매 목록*/
//	UpdateList();
//	InvalidateList();
//	_toggleGroupAuction.SetSelectedIndex(-1);
//
//	InvalidateCommission();
//	InvalidatePayback();
//}
//
//void UGsUIGuildContentAuction::Hide()
//{
//	//OnReqUpdateMainTabRedDot.ExecuteIfBound(static_cast<int32>(UGsUIWindowGuildMain::EGsGuildMainTab::Auction));
//}
//
//void UGsUIGuildContentAuction::UpdateList()
//{
//	FGsGuildManager* guildManager = GGuild();
//	if (nullptr == guildManager)
//		return;
//
//	FGsGuildAuctionData* auctionData = guildManager->GetMyGuildAuctionData();
//	if (nullptr == auctionData)
//		return;
//
//	time_t currentTime = FGsTimeSyncUtil::GetServerNowSecond();
//	_auctionItemList.Empty();
//	const TArray<FGsGuildAuctionItem>& auctionList = auctionData->GetAuctionList();
//	for (int32 i = 0; i < auctionList.Num(); ++i)
//	{
//		// 경매진행중이 아닌 항목은 노출시키지 않는다.
//		if (auctionList[i].GetAuctionState() != GuildDivisionState::AUCTION)
//			continue;
//
//		// 만료 예정 시각을 지났다면 노출시키지 않는다.
//		if (auctionList[i].GetExpiredDateTime() < currentTime)
//			continue;
//
//		_auctionItemList.Emplace(&auctionList[i]);
//	}
//	_auctionItemList.Sort([](const FGsGuildAuctionItem& lhs, const FGsGuildAuctionItem& rhs)
//		{
//			return lhs.GetExpiredDateTime() > rhs.GetExpiredDateTime();
//		});
//}
//
//void UGsUIGuildContentAuction::InvalidateList()
//{
//	FGsGuildManager* guildManager = GGuild();
//	if (nullptr == guildManager)
//		return;
//
//	FGsGuildAuctionData* auctionData = guildManager->GetMyGuildAuctionData();
//	if (nullptr == auctionData)
//		return;
//
//	const int32 AUCTION_EXIST = 0;
//	const int32 AUCTION_NONE = 1;
//
//	/* 빈 패널 or 경매 패널 결정 / 경매 슬롯 목록 갱신 */
//	_switcherList->SetActiveWidgetIndex(_auctionItemList.Num() > 0 ? AUCTION_EXIST : AUCTION_NONE);
//	_scrollBoxHelper->RefreshAll(_auctionItemList.Num());
//	_toggleGroupAuction.Clear();
//	_toggleGroupAuction.AddToggleWidgetByParentPanel(_panelAuctionList);
//
//	/* 경매 등록 수 설정 */
//	FString strRegister = FString::Printf(TEXT("%d/%d"), _auctionItemList.Num(), auctionData->GetMaxAuctionCount());
//	_textBlockRegister->SetText(FText::FromString(strRegister));
//}
//
//void UGsUIGuildContentAuction::InvalidateCommission()
//{
//	FGsGuildManager* guildManager = GGuild();
//	if (nullptr == guildManager)
//		return;
//
//	FGsGuildAuctionData* auctionData = guildManager->GetMyGuildAuctionData();
//	if (nullptr == auctionData)
//		return;
//
//	/* 경매 수수료 설정 */
//	FText textCommission;
//	auctionData->GetCommissionRateText(textCommission);
//	_textBlockCommission->SetText(textCommission);
//}
//
//void UGsUIGuildContentAuction::InvalidatePayback()
//{
//	FGsGuildManager* guildManager = GGuild();
//	if (nullptr == guildManager)
//		return;
//
//	FGsGuildAuctionData* auctionData = guildManager->GetMyGuildAuctionData();
//	if (nullptr == auctionData)
//		return;
//
//	/* 회수금 영역 초기화 */
//	_btnPayback->SetIsEnabled(auctionData->GetRemainPayback() > 0);
//	_textBlockPayback->SetText(FText::AsNumber(auctionData->GetRemainPayback()));
//}
//
//void UGsUIGuildContentAuction::OnReceiveAuctionList()
//{
//	UpdateList();
//	InvalidateList();
//	_toggleGroupAuction.SetSelectedIndex(-1);
//
//	FGsGuildManager* guildManager = GGuild();
//	if (nullptr == guildManager)
//		return;
//
//	/*레드닷*/
//	guildManager->GetRedDotCheckData().UpdateAuctionCheckTime();
//	guildManager->CheckRedDotByType(FGsGuildManager::EGsGuildRedDotType::Auction);
//}
//
//void UGsUIGuildContentAuction::OnReceiveAuctionDetail()
//{
//	UGsUIManager* uiManager = GUI();
//	if (nullptr == uiManager)
//		return;
//
//	TWeakObjectPtr<UGsUIWidgetBase> widget = uiManager->GetWidgetByKey(TEXT("PopupGuildAuctionCalculate"));
//	if (widget.IsValid())
//	{
//		if (UGsUIPopupGuildAuctionResult* popup = Cast<UGsUIPopupGuildAuctionResult>(widget.Get()))
//		{
//			popup->InvalidateList();
//		}
//	}
//}
//
//void UGsUIGuildContentAuction::InvalidateItem(uint64 InAuctionId)
//{
//	for (int32 i = 0; i < _auctionItemList.Num(); ++i)
//	{
//		if (_auctionItemList[i]->GetAuctionId() == InAuctionId)
//		{
//			_scrollBoxHelper->RefreshByIndex(i);
//			return;
//		}
//	}
//}
//
//void UGsUIGuildContentAuction::OnResetAuctionList()
//{
//	FGsGuildManager* guildManager = GGuild();
//	if (nullptr == guildManager)
//		return;
//
//	FGsGuildAuctionData* auctionData = guildManager->GetMyGuildAuctionData();
//	if (nullptr == auctionData)
//		return;
//
//	auctionData->SendReqAuctionList();
//}
//
//void UGsUIGuildContentAuction::OnRefreshEntryAuction(int32 InIndex, class UWidget* InEntry)
//{
//	if (false == _auctionItemList.IsValidIndex(InIndex))
//		return;
//
//	UGsUIGuildAuctionListEntry* entry = Cast<UGsUIGuildAuctionListEntry>(InEntry);
//	if (nullptr == entry)
//		return;
//
//	// 경매중이 아니면 노출시키지 않는다.
//	if (_auctionItemList[InIndex]->GetAuctionState() != GuildDivisionState::AUCTION)
//	{
//		entry->SetVisibility(ESlateVisibility::Collapsed);
//		return;
//	}
//
//	//entry->SetData(_auctionItemList[InIndex]);
//
//	//if (false == entry->OnClickedBidding.IsBound())
//	//{
//	//	entry->OnClickedBidding.BindUObject(this, &UGsUIGuildContentAuction::OnClickBidding);
//	//}
//}
//
//void UGsUIGuildContentAuction::OnChangeCheckNumpad(int32 InIndexGroup, bool InIsChecked)
//{
//	const int32 ENABLE_CHAT = 0;
//	const int32 ENABLE_NUMPAD = 1;
//
//	_switcherChat->SetActiveWidgetIndex(InIsChecked ? ENABLE_NUMPAD : ENABLE_CHAT);
//	_uiChatMacro->Hide();
//}
//
//void UGsUIGuildContentAuction::OnTextChangedBidPrice(const FText& InText)
//{
//	Currency price = GetPriceFromText(InText);
//	SetBidPriceWithNumpad(price);
//}
//
//void UGsUIGuildContentAuction::OnTextCommittedBidPrice(const FText& InText, ETextCommit::Type InType)
//{
//	if (ETextCommit::OnEnter == InType)
//	{
//		Currency price = GetPriceFromText(InText);
//		SetBidPriceWithNumpad(price);
//	}
//	else if (ETextCommit::OnEnter == InType)
//	{
//		SetBidPriceWithNumpad(0);
//	}
//}
//
//void UGsUIGuildContentAuction::OnClickPayback()
//{
//	FGsGuildManager* guildManager = GGuild();
//	if (nullptr == guildManager)
//		return;
//
//	FGsGuildAuctionData* auctionData = guildManager->GetMyGuildAuctionData();
//	if (nullptr == auctionData)
//		return;
//
//	if (auctionData->GetRemainPayback() <= 0)
//		return;
//
//	FGsNetSendServiceGuild::SendReqGuildAuctionReturnCurrency();
//}
//
//void UGsUIGuildContentAuction::OnClickAuctionEntry(int32 InIndex, bool bInIsSelected)
//{
//	if (false == _auctionItemList.IsValidIndex(InIndex))
//		return;
//
//	FGsGuildManager* guildManager = GGuild();
//	if (nullptr == guildManager)
//		return;
//
//	FGsGuildAuctionData* auctionData = guildManager->GetMyGuildAuctionData();
//	if (nullptr == auctionData)
//		return;
//
//	if (_auctionItemList[InIndex]->GetBidderName().IsEmpty())
//	{
//		SetBidPriceWithNumpad(_auctionItemList[InIndex]->GetBidPrice());
//	}
//	else
//	{
//		Currency autoAddedBidPrice = _auctionItemList[InIndex]->GetBidPrice() + auctionData->GetAutoAddedBid();
//		SetBidPriceWithNumpad(autoAddedBidPrice);
//	}
//}
//
//void UGsUIGuildContentAuction::OnClickBidding(const FGsGuildAuctionItem* InAuctionItem)
//{
//	if (nullptr == InAuctionItem)
//		return;
//
//	// 입찰 가능 여부 1차 판단 후 팝업 노출
//	UGsUIManager* uiManager = GUI();
//	if (nullptr == uiManager)
//		return;
//
//	// 입찰 가능한 아이템인지 판단 (내가 입찰 가능한 대상에 포함되었는가?)
//	if (false == InAuctionItem->CanBid())
//	{
//		FText errorText;
//		FText::FindText(TEXT("GuildAuctionUIText"), TEXT(""), errorText);
//		FGsUIHelper::TrayMessageTicker(errorText);
//		return;
//	}
//
//	if (InAuctionItem->GetBidderName().IsEmpty())
//	{
//		if (_bidPrice < InAuctionItem->GetBidPrice())
//		{
//			// TEXT: 경매 시작 금액은 최소 {0} 이상이어야 합니다.
//			FText errorText;
//			FText::FindText(TEXT("GuildAuctionUIText"), TEXT("Appointment_PopUp_BeginPriceLack"), errorText);
//			errorText = FText::Format(errorText, InAuctionItem->GetBidPrice());
//			FGsUIHelper::TrayMessageTicker(errorText);
//			return;
//		}
//	}
//	else
//	{
//		// 현재 입찰금보다 커야 입찰이 가능하다.
//		if (_bidPrice <= InAuctionItem->GetBidPrice())
//		{
//			// TEXT: 입찰금이 부족합니다.
//			FText errorText;
//			FText::FindText(TEXT("GuildAuctionUIText"), TEXT("Auction_Bid_Shortage_Notify"), errorText);
//			FGsUIHelper::TrayMessageTicker(errorText);
//			return;
//		}
//	}
//
//	time_t serverNowSec = FGsTimeSyncUtil::GetServerNowSecond();
//	if (InAuctionItem->GetExpiredDateTime() < serverNowSec)
//	{
//		// 입찰 가능 시간이 지났다.
//		return;
//	}
//	
//	// 다이아 재화 확인
//	if (false == FGsCurrencyHelper::CheckCurrency(CurrencyType::DIA, _bidPrice, true))
//		return;
//
//	TWeakObjectPtr<UGsUIWidgetBase> widget = uiManager->OpenAndGetWidget(TEXT("PopupGuildBidding"));
//	if (widget.IsValid())
//	{
//		if (UGsUIPopupGuildAuctionBidding* popup = Cast<UGsUIPopupGuildAuctionBidding>(widget.Get()))
//		{
//			popup->SetData(InAuctionItem, _bidPrice);
//		}
//	}
//}
//
//void UGsUIGuildContentAuction::OnClickedMacro()
//{
//	if (_uiChatMacro->IsVisible())
//	{
//		_uiChatMacro->Hide();
//	}
//	else
//	{
//		_uiChatMacro->Show();
//	}
//}
//
//void UGsUIGuildContentAuction::OnClickedMacroEntry(const FText& InMacroText)
//{
//	_uiChatMacro->Hide();
//	_uiChat->OnInputChat(InMacroText);
//}
//
//void UGsUIGuildContentAuction::OnChangeNumpadNumber(int32 InNumber)
//{
//	SetBidPrice(InNumber);
//}
//
//void UGsUIGuildContentAuction::SetBidPrice(Currency InPrice)
//{
//	FString strNumber = FString::Printf(TEXT("%llu"), InPrice);
//	_inputBidPrice->SetText(FText::FromString(strNumber));
//	_bidPrice = InPrice;
//}
//
//void UGsUIGuildContentAuction::SetBidPriceWithNumpad(Currency InPrice)
//{
//	SetBidPrice(InPrice);
//	_uiNumpad->SetNumber(InPrice, false);
//}
//
//Currency UGsUIGuildContentAuction::GetPriceFromText(const FText& InText)
//{
//	const int32 DEFAULT_PRICE = 0;
//
//	FString strTemp = InText.ToString();
//	strTemp = strTemp.Replace(TEXT(","), TEXT(""));
//	strTemp = strTemp.Replace(TEXT("+"), TEXT(""));
//	FString strBidPrice = strTemp.Replace(TEXT("-"), TEXT(""));
//	
//	if (strBidPrice.IsEmpty())
//	{
//		return 0;
//	}
//
//	if (false == strBidPrice.IsNumeric())
//	{
//		return _bidPrice;
//	}
//
//	// 최대 _maxAuctionPrice 까지만 입력 가능하다.
//	int32 length = strBidPrice.Len();
//	FString maxPriceLength = FString::FromInt(_maxAuctionPrice);
//	if (length > maxPriceLength.Len())
//	{
//		strBidPrice = maxPriceLength;
//	}
//	
//	// 보유한 다이아 금액까지만 입력 가능하다.
//	Currency changedBidPrice = FCString::Atoi(*strBidPrice);
//	Currency myDiaAmount = FGsCurrencyHelper::GetCurrencyAmount(CurrencyType::DIA);
//	if (changedBidPrice > myDiaAmount)
//	{
//		changedBidPrice = myDiaAmount;
//	}
//
//	return (changedBidPrice < 0) ? 0 : (changedBidPrice < _maxAuctionPrice) ? changedBidPrice : _maxAuctionPrice;
//}
