// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUIWindowMarket.h"
#include "Components/PanelWidget.h"
#include "Components/WidgetSwitcher.h"
#include "UI/UIControlLib/ContentWidget/GsButton.h"
#include "UI/UIControlLib/ContentWidget/GsSwitcherButton.h"
#include "UI/UIContent/Common/RedDot/GsUIRedDotBase.h"
#include "UI/UIContent/Window/Market/GsUIMarketPageBase.h"
#include "UI/UIContent/Window/Market/GsUIMarketPageSearch.h"
#include "UI/UIContent/Window/Market/GsUIMarketPageSell.h"
#include "UI/UIContent/Window/Market/GsUIMarketPageSettlement.h"
#include "UI/UIContent/Window/Market/GsUIMarketPageLog.h"
#include "UI/UIContent/Window/Market/GsUITooltipWorldMarketServerList.h"
#include "UI/UIContent/Helper/GsUIHelper.h"
#include "Management/GsMessageHolder.h"
#include "Management/ScopeGlobal/GsUIManager.h"
#include "Management/ScopeGame/GsMarketManager.h"
#include "Management/ScopeGame/GsUnlockManager.h"
#include "Message/MessageParam/GsMessageParam.h"
#include "Message/MessageParam/GsMarketMessageParam.h"
#include "DataSchema/GsSchemaEnums.h"
#include "Market/GsMarketHelper.h"
#include "Shared/Client/SharedEnums/PD_ResultEnum.h"


void UGsUIWindowMarket::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	_helpContentsKey = EGsHelpContents::Market;

	// 버튼 설정
	_btnBack->OnClicked.AddDynamic(this, &UGsUIWindowMarket::OnClickBack);
	_btnClose->OnClicked.AddDynamic(this, &UGsUIWindowMarket::OnClickCloseAllStack);

	// 페이지 설정
	// 주의: 순서가 EGsMarketMainTab 와 일치해야 함
	_pageList.Emplace(_pageSearch);
	_pageList.Emplace(_pageSell);
	_pageList.Emplace(_pageSettlement);
	_pageList.Emplace(_pageLog);

	// 서버 탭 설정
	_toggleGroupServer.AddToggleWidget(_sBtnNormal);
	_toggleGroupServer.AddToggleWidget(_sBtnWorld);
	_toggleGroupServer.OnSlotClicked.BindUObject(this, &UGsUIWindowMarket::OnClickServer);
	_toggleGroupServer.OnChangeBlocked.BindUObject(this, &UGsUIWindowMarket::OnClickBlockedWorldTab);

	// 서버 리스트 툴팁
	_sBtnTooltip->OnPressed.AddDynamic(this, &UGsUIWindowMarket::OnPressTooltipWorldServerList);

	// 메인 탭 설정
	_toggleGroupMainTab.AddToggleWidgetByParentPanel(_panelMainTab);
	_toggleGroupMainTab.OnSelectChanged.BindUObject(this, &UGsUIWindowMarket::OnSelectMainTab);
	_toggleGroupMainTab.OnChangeBlocked.BindUObject(this, &UGsUIWindowMarket::OnClickBlockedTab);

	// 광클 방지
	_sBtnWorld->SetOnClickLockSec(1.f);
	_sBtnNormal->SetOnClickLockSec(1.f);
	_coolTimeServerTab.SetIntervalSecond(0.5f);
}

void UGsUIWindowMarket::NativeConstruct()
{
	Super::NativeConstruct();

	BindMessages();

	FGsMarketManager* marketMgr = GSMarket();
	if (nullptr == marketMgr)
	{
		return;
	}

	// 레드닷 체크
	marketMgr->CheckRedDot(true);

	// 월드탭 언락체크
	CheckUnlockWorld(true);

	// 검색 바로가기로 들어온 경우 스택 복구를 무시함(C2URWQ-7555)
	const TPair<ItemId, bool>& searchInfo = marketMgr->GetSearchItemInfo();
	if (INVALID_ITEM_ID != searchInfo.Key)
	{	
		if (IsStackRecoverFlag())
		{
			SetStackRecoverFlag(false);
		}
	}

	// 스택 복구
	if (IsStackRecoverFlag())
	{
		_toggleGroupServer.SetSelectedIndex(static_cast<int32>(_currServerTab));
		SetUIServer(_currServerTab);

		SetUIPage(_currMainTab);
	}
	else
	{
		if (INVALID_ITEM_ID == searchInfo.Key)
		{
			// 일반 진입
			_toggleGroupServer.SetSelectedIndex(static_cast<int32>(EGsMarketServerTab::Normal));
			SetUIServer(EGsMarketServerTab::Normal);
		}
		else
		{
			// 바로가기로 진입
			EGsMarketServerTab targetTab = searchInfo.Value ? EGsMarketServerTab::World : EGsMarketServerTab::Normal;

			_toggleGroupServer.SetSelectedIndex(static_cast<int32>(targetTab));
			SetUIServer(targetTab);
		}

		// 반드시 검색탭으로 갱신이 일어날 수 있게 true 넣어야 함
		_toggleGroupMainTab.SetSelectedIndex(EGsMarketMainTab::Search, true);
	}
}

void UGsUIWindowMarket::NativeDestruct()
{
	UnbindMessages();

	if (FGsMarketManager* marketMgr = GSMarket())
	{
		marketMgr->ClearFilterData();
		marketMgr->ClearSearchData();
	}

	if (false == IsStackRecoverFlag())
	{
		_currServerTab = EGsMarketServerTab::MaxType;
		_currMainTab = EGsMarketMainTab::MaxType;
	}

	SetRedDotDirty(false);

	Super::NativeDestruct();
}

void UGsUIWindowMarket::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (_bRedDotDirty)
	{
		if (FGsMarketManager* marketMgr = GSMarket())
		{
			_uiRedDot->SetRedDot(marketMgr->CheckRedDoyByServer(IsWorldMarket()), EGsIconRedDotType::NORMAL);

			_uiRedDotWorld->SetRedDot(marketMgr->CheckRedDoyByServer(true), EGsIconRedDotType::NORMAL);
			_uiRedDotNormal->SetRedDot(marketMgr->CheckRedDoyByServer(false), EGsIconRedDotType::NORMAL);
		}

		_bRedDotDirty = false;
	}
}

bool UGsUIWindowMarket::OnBack()
{
	// 검색, 필터 Tray가 열려 있으면 먼저 닫는다(Tray지만 스택처럼 닫도록 처리함)
	UGsUIManager* uiMgr = GUI();
	
	if (uiMgr->IsActiveWidget(TEXT("TrayMarketFilter")))
	{
		// 두번째 인자 true이면 열려있는 창이다
		TWeakObjectPtr<UGsUIWidgetBase> tray = uiMgr->GetWidgetByKey(TEXT("TrayMarketFilter"), true);
		if (tray.IsValid())
		{
			tray->Close();
			return true;
		}
	}

	if (uiMgr->IsActiveWidget(TEXT("TrayMarketSearch")))
	{
		// 두번째 인자 true이면 열려있는 창이다
		TWeakObjectPtr<UGsUIWidgetBase> tray = uiMgr->GetWidgetByKey(TEXT("TrayMarketSearch"), true);
		if (tray.IsValid())
		{
			tray->Close();
			return true;
		}
	}

	return Super::OnBack();
}

bool UGsUIWindowMarket::SyncContentState()
{
	if (false == IsSameContentState(FGsContentsMode::InGame::ContentsMarket))
	{
		GMessage()->GetContents().SendMessage(MessageContents::ACTIVE_WINDOW_MARKET);
		return true;
	}

	return false;
}

void UGsUIWindowMarket::BindMessages()
{
	FGsMessageHolder* messageMgr = GMessage();

	MMarket& msgMarket = messageMgr->GetMarket();
	_msgHandleMarketList.Emplace(msgMarket.AddUObject(MessageContentMarket::UPDATE_ITEM_LIST,
		this, &UGsUIWindowMarket::OnUpdateItemList));
	_msgHandleMarketList.Emplace(msgMarket.AddUObject(MessageContentMarket::UPDATE_BOOKMARK,
		this, &UGsUIWindowMarket::OnUpdateBookmark));
	_msgHandleMarketList.Emplace(msgMarket.AddUObject(MessageContentMarket::RCV_SELL_REGISTERED_ITEM,
		this, &UGsUIWindowMarket::OnRcvSellRegisterdItem));
	_msgHandleMarketList.Emplace(msgMarket.AddUObject(MessageContentMarket::RCV_SELL_CANCEL_ITEM,
		this, &UGsUIWindowMarket::OnRcvSellCancelItem));
	_msgHandleMarketList.Emplace(msgMarket.AddUObject(MessageContentMarket::UPDATE_SEARCH,
		this, &UGsUIWindowMarket::OnUpdateSearch));
	_msgHandleMarketList.Emplace(msgMarket.AddUObject(MessageContentMarket::UPDATE_FILTER,
		this, &UGsUIWindowMarket::OnUpdateFilter));
	_msgHandleMarketList.Emplace(msgMarket.AddUObject(MessageContentMarket::UPDATE_RED_DOT,
		this, &UGsUIWindowMarket::OnUpdateRedDot));
	_msgHandleMarketList.Emplace(msgMarket.AddUObject(MessageContentMarket::RCV_ITEM_DETAIL_INFO,
		this, &UGsUIWindowMarket::OnRcvItemDetailInfo));
	_msgHandleMarketList.Emplace(msgMarket.AddUObject(MessageContentMarket::RCV_SEARCH_RESULT,
		this, &UGsUIWindowMarket::OnRcvSearchResult));
	_msgHandleMarketList.Emplace(msgMarket.AddUObject(MessageContentMarket::REQ_REFRESH,
		this, &UGsUIWindowMarket::OnReqRefresh));
	_msgHandleMarketList.Emplace(msgMarket.AddUObject(MessageContentMarket::RCV_BOOKMARK_LIST,
		this, &UGsUIWindowMarket::OnRcvBookmarkmList));
	_msgHandleMarketList.Emplace(msgMarket.AddUObject(MessageContentMarket::RCV_PURCHASE_RESULT,
		this, &UGsUIWindowMarket::OnRcvBuyResult));
	_msgHandleMarketList.Emplace(msgMarket.AddUObject(MessageContentMarket::ADD_TO_PURCHASE_LIST,
		this, &UGsUIWindowMarket::OnAddToPurchaseList));
	_msgHandleMarketList.Emplace(msgMarket.AddUObject(MessageContentMarket::REMOVE_FROM_PURCHASE_LIST,
		this, &UGsUIWindowMarket::OnRemoveFromPurchaseList));

	MUnlock& msgUnlock = messageMgr->GetUnlock();
	_msgHandleUnlockList.Emplace(msgUnlock.AddUObject(MessageContentUnlock::NOTIFY_CONTENTS_UNLOCK,
		this, &UGsUIWindowMarket::OnContentsUnlock));
	_msgHandleUnlockList.Emplace(msgUnlock.AddUObject(MessageContentUnlock::INVALIDATE_ALL_UNLOCK,
		this, &UGsUIWindowMarket::OnContentsUnlock));
}

void UGsUIWindowMarket::UnbindMessages()
{
	FGsMessageHolder* messageMgr = GMessage();

	MMarket& msgMarket = messageMgr->GetMarket();
	for (MsgHandleMarket& handle : _msgHandleMarketList)
	{
		msgMarket.Remove(handle);
	}
	_msgHandleMarketList.Empty();

	MUnlock& msgUnlock = messageMgr->GetUnlock();
	for (MsgHandleUnlock& handle : _msgHandleUnlockList)
	{
		msgUnlock.Remove(handle);
	}
	_msgHandleUnlockList.Empty();
}

void UGsUIWindowMarket::OnClickServer(int32 InIndex, bool bIsSelected)
{
	if (bIsSelected)
	{
		EGsMarketServerTab nextTab = static_cast<EGsMarketServerTab>(InIndex);

		// 주의: ToggleGroup의 Select가 아니라 SlotClick이벤트를 사용해서 Block에 막히지 않으므로 별도로 체크하여 막아준다
		if (EGsMarketServerTab::World == nextTab)
		{
			if (false == GSUnlock()->IsContentsUnlock(EGsUnlockContentsType::MARKET_WORLD, false))
			{
				return;
			}
		}

		if (false == _coolTimeServerTab.IsTimeOver())
		{
			if (EGsMarketServerTab::MaxType != _currServerTab)
			{
				// 원상복구시킴
				_toggleGroupServer.SetSelectedIndex(static_cast<int32>(_currServerTab));
				return;
			}
		}
		_coolTimeServerTab.UpdateTime();

		
		if (nextTab == _currServerTab)
		{
			return;
		}

		// 필터, 탭, 검색 정보 등 유지할 것이 있나 확인... 인데 탭을 어떻게 유지하지 ㄱ-;;;; 값을 미리 넣어줘야하나
		if (false == CheckChangeServerMaintainStatus(nextTab))
		{
			if (FGsMarketManager* marketMgr = GSMarket())
			{
				marketMgr->ClearFilterData();
				marketMgr->ClearSearchData();
			}
		}

		SetUIServer(nextTab);

		// 반드시 검색탭으로 갱신이 일어날 수 있게 true 넣어야 함
		_toggleGroupMainTab.SetSelectedIndex(EGsMarketMainTab::Search, true);
	}
}

void UGsUIWindowMarket::OnClickBlockedWorldTab(int32 InIndex)
{
	if (static_cast<int32>(EGsMarketServerTab::World) == InIndex)
	{
		GSUnlock()->IsContentsUnlock(EGsUnlockContentsType::MARKET_WORLD, true);
	}
}

void UGsUIWindowMarket::OnSelectMainTab(int32 InIndex)
{
	SetUIPage(static_cast<EGsMarketMainTab>(InIndex));

	// 이전에 열려있던 팝업 닫기
	GUI()->CloseByKeyName(TEXT("PopupMarketPurchase"));
}

void UGsUIWindowMarket::OnClickBlockedTab(int32 InIndex)
{
	if (static_cast<int32>(EGsMarketMainTab::Sell) == InIndex)
	{
		if (IsWorldMarket())
		{
			GSUnlock()->IsContentsUnlock(EGsUnlockContentsType::MARKET_WORLD_SELL, true);
		}
		else
		{
			GSUnlock()->IsContentsUnlock(EGsUnlockContentsType::MARKET_SELL, true);
		}
	}
}

void UGsUIWindowMarket::OnPressTooltipWorldServerList()
{
	if (_tooltipWorldMarketServerList->IsOpen())
	{
		_tooltipWorldMarketServerList->Close();
	}
	else
	{
		_tooltipWorldMarketServerList->Open();
	}
}

void UGsUIWindowMarket::SetUIServer(EGsMarketServerTab InType)
{
	_currServerTab = InType;

	bool bIsWorldMarket = IsWorldMarket();

	_panelWorldBackground->SetVisibility(bIsWorldMarket ?
		ESlateVisibility::HitTestInvisible : ESlateVisibility::Collapsed);

	_sBtnTooltip->SetSwitcherIndex(bIsWorldMarket ? 1 : 0);

	CheckUnlock(true);
}

void UGsUIWindowMarket::SetUIPage(EGsMarketMainTab InTabType)
{
	// 이전 페이지 닫기
	if (_pageList.IsValidIndex(_currMainTab))
	{
		if (UGsUIMarketPageBase* prevPage = _pageList[_currMainTab])
		{
			prevPage->Hide();
		}
	}

	_currMainTab = InTabType;
	_switcherPage->SetActiveWidgetIndex(InTabType);

	if (_pageList.IsValidIndex(InTabType))
	{
		if (UGsUIMarketPageBase* currPage = _pageList[InTabType])
		{
			currPage->Show(IsWorldMarket());
		}
	}

	SetRedDotDirty(true);
}

void UGsUIWindowMarket::CheckUnlockWorld(bool InIsFirstCheck)
{
	bool bIsWorldUnlock = GSUnlock()->IsContentsUnlock(EGsUnlockContentsType::MARKET_WORLD);
	int32 worldIndex = static_cast<int32>(EGsMarketServerTab::World);

	if (bIsWorldUnlock)
	{
		_toggleGroupServer.DeleteBlockedIndex(worldIndex);
	}
	else
	{
		if (false == _toggleGroupServer.IsBlockedIndex(worldIndex))
		{
			_toggleGroupServer.AddBlockedIndex(worldIndex);

			// 블락이 없다가 걸릴때만 리셋함
			// 첫 진입 이후 로직을 타는 경우는, 서버에서 강제로 다시 잠궜을 경우라 강제로 첫 탭으로 되돌린다
			if (false == InIsFirstCheck)
			{
				if (worldIndex == _toggleGroupServer.GetSelectedIndex())
				{
					_toggleGroupServer.SetSelectedIndex(0);
					SetUIServer(EGsMarketServerTab::Normal);

					// 반드시 검색탭으로 갱신이 일어날 수 있게 true 넣어야 함
					_toggleGroupMainTab.SetSelectedIndex(EGsMarketMainTab::Search, true);
				}
			}
		}
	}
}

void UGsUIWindowMarket::CheckUnlock(bool InIsFirstCheck)
{
	bool bIsSellUnlock = false;
	if (IsWorldMarket())
	{
		bIsSellUnlock = GSUnlock()->IsContentsUnlock(EGsUnlockContentsType::MARKET_WORLD_SELL);
	}
	else
	{
		bIsSellUnlock = GSUnlock()->IsContentsUnlock(EGsUnlockContentsType::MARKET_SELL);
	}

	int32 sellIndex = static_cast<int32>(EGsMarketMainTab::Sell);

	if (bIsSellUnlock)
	{
		_toggleGroupMainTab.DeleteBlockedIndex(sellIndex);
	}
	else
	{
		if (false == _toggleGroupMainTab.IsBlockedIndex(sellIndex))
		{
			_toggleGroupMainTab.AddBlockedIndex(sellIndex);

			// 블락이 없다가 걸릴때만 리셋함
			// 첫 진입 이후 로직을 타는 경우는, 서버에서 강제로 다시 잠궜을 경우라 강제로 첫 탭으로 되돌린다
			if (false == InIsFirstCheck)
			{
				_toggleGroupMainTab.SetSelectedIndex(0);
			}
		}
	}
}

bool UGsUIWindowMarket::CheckChangeServerMaintainStatus(EGsMarketServerTab InNextTab)
{
	FGsMarketManager* marketMgr = GSMarket();
	if (nullptr == marketMgr)
	{
		return false;
	}
	
	ItemId shortcutItemId = marketMgr->GetShortcutItemId();
	if (INVALID_ITEM_ID == shortcutItemId)
	{
		// 바로가기로 들어온 아이템이 아니먄,
		// 검색어가 있는 경우엔 무조건 검색
		bool bIsWorldMarket = IsWorldMarket();
		const FString strSearch = marketMgr->GetSearchString(bIsWorldMarket);
		if (false == strSearch.IsEmpty())
		{
			// 주의: 탭이 바뀌므로 현재 마켓상태의 반대로 넣어준다
			marketMgr->SendSearchedItemList(!bIsWorldMarket, strSearch,
				_pageSearch->GetCurrCategory(), _pageSearch->GetCurrSubCategoryIndex());

			return true;
		}
	}
	else
	{
		// 바로가기로 들어온 아이템이면, 
		// 월드마켓일 경우 등록 가능한 항목만 검색 수행, 아니면 클리어 유지
		if (EGsMarketServerTab::World == InNextTab)
		{
			if (const FGsSchemaItemCommon* itemData =
				UGsTableUtil::FindRowById<UGsTableItemCommon, FGsSchemaItemCommon>(shortcutItemId))
			{
				ItemGrade grade = FGsMarketHelper::GetWorldMarketMinimumItemGrade();
				if (grade <= itemData->grade)
				{
					marketMgr->SendSearchItem(true, shortcutItemId);
					return true;
				}
			}
		}
		else
		{
			marketMgr->SendSearchItem(false, shortcutItemId);
			return true;
		}
	}	

	// 필터 정보가 있으면 그대로 유지
	FGsMarketFilterData& filter = marketMgr->GetFilterData();
	if (filter.IsFilter())
	{
		return true;
	}

	return false;
}

void UGsUIWindowMarket::OnUpdateItemList(const IGsMessageParam*)
{
	if (UGsUIMarketPageBase* page = GetCurrentPage())
	{
		page->OnUpdateItemList();
	}
}

void UGsUIWindowMarket::OnUpdateBookmark(const IGsMessageParam* InParam)
{
	if (const FGsMarketMsgParamId* param = InParam->Cast<const FGsMarketMsgParamId>())
	{
		if (UGsUIMarketPageBase* page = GetCurrentPage())
		{
			page->OnUpdateBookmark(param->_id);
		}
	}
}

void UGsUIWindowMarket::OnRcvSellRegisterdItem(const IGsMessageParam* InParam)
{
	if (const FGsMarketMsgParamId* param = InParam->Cast<const FGsMarketMsgParamId>())
	{
		if (UGsUIMarketPageBase* page = GetCurrentPage())
		{
			page->OnRcvSellRegisterdItem(param->_id);
		}
	}
}

void UGsUIWindowMarket::OnRcvSellCancelItem(const IGsMessageParam* InParam)
{
	if (const FGsMarketMsgParamId* param = InParam->Cast<const FGsMarketMsgParamId>())
	{
		if (UGsUIMarketPageBase* page = GetCurrentPage())
		{
			page->OnRcvSellCancelItem(param->_id);
		}
	}
}

void UGsUIWindowMarket::OnUpdateSearch(const IGsMessageParam* InParam)
{
	if (const FGsMarketMsgParamString* param = InParam->Cast<const FGsMarketMsgParamString>())
	{
		if (UGsUIMarketPageBase* page = GetCurrentPage())
		{
			page->OnUpdateSearch(param->_str);
		}
	}
}

void UGsUIWindowMarket::OnUpdateFilter(const IGsMessageParam* InParam)
{
	if (const FGsMarketMsgParamId* param = InParam->Cast<const FGsMarketMsgParamId>())
	{
		if (UGsUIMarketPageBase* page = GetCurrentPage())
		{
			page->OnUpdateFilter((0 < param->_id) ? true : false);
		}
	}
}

void UGsUIWindowMarket::OnUpdateRedDot(const IGsMessageParam* InParam)
{
	SetRedDotDirty(true);
}

void UGsUIWindowMarket::OnRcvItemDetailInfo(const IGsMessageParam* InParam)
{
	if (const FGsMarketMsgParamId* param = InParam->Cast<const FGsMarketMsgParamId>())
	{
		if (UGsUIMarketPageBase* page = GetCurrentPage())
		{
			page->OnRcvItemDetailInfo(param->_id);
		}
	}
}

void UGsUIWindowMarket::OnRcvSearchResult(const IGsMessageParam* InParam)
{
	const TPair<ItemId, bool>& searchInfo = GSMarket()->GetSearchItemInfo();

	// 외부에서 특정 검색으로 들어올 경우 강제 갱신 수행
	// 거래소 내에서 검색할 경우 탭을 유지하기 위해 강제갱신 수행하지 않음
	if (INVALID_ITEM_ID != searchInfo.Key)
	{
		// 탭이 다를 경우에 강제 변경
		EGsMarketServerTab targetTab = searchInfo.Value ? EGsMarketServerTab::World : EGsMarketServerTab::Normal;
		if (targetTab != _currServerTab)
		{
			_toggleGroupServer.SetSelectedIndex(static_cast<int32>(targetTab));
			SetUIServer(targetTab);
		}

		_toggleGroupMainTab.SetSelectedIndex(EGsMarketMainTab::Search, true);
	}

	if (UGsUIMarketPageBase* page = GetCurrentPage())
	{
		page->OnRcvSearchResult();
	}
}

void UGsUIWindowMarket::OnReqRefresh(const IGsMessageParam*)
{
	if (UGsUIMarketPageBase* page = GetCurrentPage())
	{
		page->OnReqRefresh();
	}
}

void UGsUIWindowMarket::OnRcvBookmarkmList(const IGsMessageParam*)
{
	if (UGsUIMarketPageBase* page = GetCurrentPage())
	{
		page->OnRcvBookmarkList();
	}
}

void UGsUIWindowMarket::OnRcvBuyResult(const IGsMessageParam* InParam)
{
	// 거래소가 아닌 곳에서 다중구매 시의 처리 분기를 위해 여기서 메시지 출력
	if (const FGsMarketMsgParamPurchaseResult* param = InParam->Cast<const FGsMarketMsgParamPurchaseResult>())
	{
		if (param->IsSuccess())
		{
			if (UGsUIMarketPageBase* page = GetCurrentPage())
			{
				page->OnRcvBuySuccess(param->_id);
			}

			// TEXT: 구매에 성공하였습니다
			FText textMsg;
			if (FText::FindText(TEXT("MarketText"), TEXT("Ticker_Purchase_Success"), textMsg))
			{
				FGsUIHelper::TraySectionMessageTicker(EGsNotiSection::EXTSYSTEM, textMsg);
			}
		}
		else
		{
			if (UGsUIMarketPageBase* page = GetCurrentPage())
			{
				page->OnRcvBuyFail();
			}

			PD::Result result = static_cast<PD::Result>(param->_result);
			switch (result)
			{
			case PD::Result::MARKET_ERROR_CANT_PURCHASE_MY_TRANSACTION:
			{
				// TEXT: 자신이 판매 등록한 아이템입니다
				FText textMsg;
				if (FText::FindText(TEXT("MarketText"), TEXT("Ticker_Purchase_Fail_CrossTrading"), textMsg))
				{
					FGsUIHelper::TraySectionMessageTicker(EGsNotiSection::EXTSYSTEM, textMsg);
				}
			}
			break;
			case PD::Result::MARKET_ERROR_NOT_FOUND_TRANSACTION:
			case PD::Result::MARKET_ERROR_DB_PURCHASE_NOT_FOUND_TRANSACTION:
			{
				// TEXT: 이미 팔리거나 판매 취소된 아이템입니다.
				FText textMsg;
				if (FText::FindText(TEXT("MarketText"), TEXT("Ticker_Purchase_Fail_Soldout"), textMsg))
				{
					FGsUIHelper::TraySectionMessageTicker(EGsNotiSection::EXTSYSTEM, textMsg);
				}
			}
			break;
			default:
			{
				// TEXT: 구매에 실패하였습니다.
				FText textMsg;
				if (FText::FindText(TEXT("MarketText"), TEXT("Ticker_Purchase_Fail"), textMsg))
				{
					FGsUIHelper::TraySectionMessageTicker(EGsNotiSection::EXTSYSTEM, textMsg);
				}

				FGsUIHelper::PopupNetError(result);
			}
			break;
			}			
		}
	}
}

void UGsUIWindowMarket::OnAddToPurchaseList(const IGsMessageParam* InParam)
{
	if (EGsMarketMainTab::Search == _currMainTab)
	{
		if (const FGsMarketMsgParamId* param = InParam->Cast<const FGsMarketMsgParamId>())
		{
			_pageSearch->OnAddToPurchaseList(static_cast<MarketTransactionId>(param->_id));
		}
	}
}

void UGsUIWindowMarket::OnRemoveFromPurchaseList(const IGsMessageParam* InParam)
{
	if (EGsMarketMainTab::Search == _currMainTab)
	{
		if (const FGsMarketMsgParamId* param = InParam->Cast<const FGsMarketMsgParamId>())
		{
			_pageSearch->OnRemoveFromPurchaseList(static_cast<MarketTransactionId>(param->_id));
		}
	}
}

void UGsUIWindowMarket::OnContentsUnlock(const IGsMessageParam* InParam)
{
	if (nullptr == InParam)
	{
		CheckUnlockWorld(false);
		CheckUnlock(false);
	}
	else
	{
		if (const FGsPrimitivePairInt32* param = InParam->Cast<const FGsPrimitivePairInt32>())
		{
			if (static_cast<int32>(EGsUnlockContentsType::MARKET_WORLD) == param->_first)
			{
				CheckUnlockWorld(false);
			}
			else if (static_cast<int32>(EGsUnlockContentsType::MARKET_WORLD_SELL) == param->_first)
			{
				CheckUnlock(false);
			}
			else if (static_cast<int32>(EGsUnlockContentsType::MARKET_SELL) == param->_first)
			{
				CheckUnlock(false);
			}
		}
	}
}

UGsUIMarketPageBase* UGsUIWindowMarket::GetCurrentPage() const
{
	if (_pageList.IsValidIndex(_currMainTab))
	{
		return _pageList[_currMainTab];
	}

	return nullptr;
}

bool UGsUIWindowMarket::IsWorldMarket() const
{
	return (EGsMarketServerTab::World == _currServerTab) ? true : false;
}
