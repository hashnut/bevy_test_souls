// Fill out your copyright notice in the Description page of Project Settings.


#include "GsGameStateMarket.h"

#include "Management/GsMessageHolder.h"

#include "Management/ScopeGlobal/GsUIManager.h"
#include "Management/ScopeGame/GsMarketManager.h"
#include "UI/UIContent/Window/GsUIWindowMarket.h"
#include "UI/UIContent/Tray/GsUITrayNavigationBar.h"
#include "Market/GsMarketHelper.h"


FGsGameStateMarket::FGsGameStateMarket() : FGsContentsGameBase(FGsContentsMode::InGame::ContentsMarket)
{
}

FGsGameStateMarket::~FGsGameStateMarket()
{
}

void FGsGameStateMarket::Enter()
{
	FGsContentsGameBase::Enter();	

	UGsUIManager* uiMgr = GUI();
	if (false == uiMgr->IsActiveWidget(TEXT("WindowMarket")))
	{
		uiMgr->Open(TEXT("WindowMarket"));
	}

	// 네비게이션 바 재화 세팅 (드롭다운 메뉴 감춤)
	SetNavigationBar(CurrencyType::NONE);

	// 젠 표시
	ItemId itemId = FGsMarketHelper::GetWorldMarketRegistrationFeeItemId();
	if (INVALID_ITEM_ID != itemId)
	{
		SetNavigationBarItemId(itemId);
	}

	// 우측 정렬 거리 벌리기
	TWeakObjectPtr<UGsUIWidgetBase> widget = uiMgr->GetWidgetByKey(TEXT("TrayNavigationBar"));
	if (widget.IsValid())
	{
		if (UGsUITrayNavigationBar* naviBar = Cast<UGsUITrayNavigationBar>(widget.Get()))
		{
			naviBar->SetNaviBarAlign(UGsUITrayNavigationBar::EGsNaviBarAlign::MARKET);
		}
	}
}

void FGsGameStateMarket::Exit()
{
	// 혹시 열려있으면 같이 닫기
	if (UGsUIManager* uiMgr = GUI())
	{
		uiMgr->CloseByKeyName(TEXT("TrayMarketFilter"));
		uiMgr->CloseByKeyName(TEXT("TrayMarketSearch"));
	}

	// 검색정보 등을 제거
	if (FGsMarketManager* marketMgr = GSMarket())
	{
		marketMgr->ClearFilterData();
		marketMgr->ClearSearchData();
	}

	// HUD 관련 레드닷 갱신 요청. 파라미터 false값 상관 없음
	if (FGsMessageHolder* messageMgr = GMessage())
	{
		messageMgr->GetHudRedDot().SendMessage(MessageContentHUDRedDot::MARKET, false);
	}

	FGsContentsGameBase::Exit();
}
