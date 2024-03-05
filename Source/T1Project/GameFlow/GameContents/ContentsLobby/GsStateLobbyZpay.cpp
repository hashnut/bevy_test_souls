// Fill out your copyright notice in the Description page of Project Settings.


#include "GameFlow/GameContents/ContentsLobby/GsStateLobbyZpay.h"

#include "Data/GsLobbyConstant.h"

#include "Management/GsMessageHolder.h"
#include "Management/GsScopeHolder.h"
#include "Management/ScopeGlobal/GsBMShopManager.h"
#include "Management/ScopeGlobal/GsUIManager.h"
#include "Management/ScopeGlobal/GsNetManager.h"
#include "Management/ScopeLobby/GsLobbyDataManager.h"

#include "Net/GsNetSendServiceBMShop.h"

#include "GameFlow/GameContents/ContentsGame/Hud/BMShop/GsBMShopHandler.h"
#include "GameFlow/GameContents/ContentsGame/Hud/BMShop/GsIAPPurchaseBase.h"

#include "Shared/Shared/SharedInclude/SharedTypes.h"

#include "UI/UIContent/Helper/GsUIHelper.h"
#include "UI/UILib/Base/GsUIWidgetBase.h"
#include "UI/UIContent/Window/BMShop/GsUIWindowBMShop.h"

#include "T1Project.h"


FGsStateLobbyZpay::FGsStateLobbyZpay() : FGsContentsLobbyBase(FGsContentsMode::InLobby::Z_PAY)
{	
	_checkWaitProc = false;
}

FGsStateLobbyZpay::~FGsStateLobbyZpay()
{	
}

void FGsStateLobbyZpay::Enter()
{
	GSLOG(Warning, TEXT("FGsStateLobbyZpay::Enter()"));

	FGsContentsLobbyBase::Enter();

	_checkWaitProc = false;

	FGsBMShopManager* bmShopMgr = GBMShop();
	//if (false == bmShopMgr->IsValidHiveProduct() && false == bmShopMgr->IsUseZpayModeCheat())
	if (false == bmShopMgr->IsValidHiveProduct())
	{
		_checkWaitProc = true;
		return;
	}
	
	bmShopMgr->ResetShopData();

	RegisterMessages();

	if (UGsUIManager* uiManager = GUI())
	{	
		TWeakObjectPtr<UGsUIWidgetBase> widget = uiManager->OpenAndGetWidget(("WindowBMShop"));
	}
	FGsUIHelper::HideBlockUI();

	bmShopMgr->AddBMShopHandlerItemDetailMsg();
}

void FGsStateLobbyZpay::Update(float In_deltaTime)
{
	FGsContentsLobbyBase::Update(In_deltaTime);

	if (_checkWaitProc)
	{
		WaitProc();
	}
}

void FGsStateLobbyZpay::WaitProc()
{
	GSLOG(Warning, TEXT("FGsStateLobbyZpay::WaitProc"));

	FGsBMShopManager* shopMgr = GBMShop();
	if (shopMgr)
	{
		// 1. 하이브 콜백 호출한 뒤, 응답이 안온 콜백함수가 있는지 선 체크. 체크하여 있다면 올때까지 기다리기
		if (shopMgr->IsReceivedHiveCallback(FGsIAPPurchaseBase::CallbackResultType::Callback_MarketInit) ||
			shopMgr->IsReceivedHiveCallback(FGsIAPPurchaseBase::CallbackResultType::Callback_GetProductInfo))
		{
			GSLOG(Warning, TEXT("[FGsStateLobbyZpay::WaitProc] 1111"));
			return;
		}

		// 만약 하이브 통신 리트라이 할거라면 밑에 SendMessage 호출하면됨
		//FGsBMShopParambool param(false);
		//GMessage()->GetBMShop().SendMessage(MessageContentStoreBM::OPEN_BMSHOP_VALID_HIVE, &param);
		
		if (false == shopMgr->IsValidHiveProduct())
		{
			GSLOG(Warning, TEXT("[FGsStateLobbyZpay::WaitProc] 2222"));

			// 2. 모든 콜백함수가 온 경우라면, 하이브 통신이 제대로 이루어지지 않았기에 타이틀 화면으로 이동시킴
			// Error Popup
			FText findText;
			FText::FindText(TEXT("BMShopText"), TEXT("SyncPay_Error_Connect_Popup_Desc"), findText);
			FGsUIHelper::PopupOK(findText, [this]()
				{
					GMessage()->GetSystem().SendMessage(MessageSystem::RESTART_GAME);
				});

			return;
		}

		// 하이브 콜백 정상 확인으로 상점 세팅
		_checkWaitProc = false;

		shopMgr->ResetShopData();

		RegisterMessages();
		if (UGsUIManager* uiManager = GUI())
		{
			TWeakObjectPtr<UGsUIWidgetBase> widget = uiManager->OpenAndGetWidget(("WindowBMShop"));
		}
		FGsUIHelper::HideBlockUI();

		shopMgr->AddBMShopHandlerItemDetailMsg();
	}
}

void FGsStateLobbyZpay::Exit()
{
	UnregisterMessages();

	FGsContentsLobbyBase::Exit();
}

void FGsStateLobbyZpay::Close()
{
	UnregisterMessages();

	FGsContentsLobbyBase::Close();
}

void FGsStateLobbyZpay::RegisterMessages()
{
	FGsMessageHolder* msg = GMessage();
	if (nullptr == msg)
		return;

	_bmShopMsgHandlerList.Emplace(msg->GetBMShop().AddRaw(FGsBMShopKickZpayUser::MESSAGE_ID, this, &FGsStateLobbyZpay::OnKickUser));
}

void FGsStateLobbyZpay::UnregisterMessages()
{
	FGsMessageHolder* msg = GMessage();
	if (nullptr == msg)
		return;

	for (TPair<MessageContentStoreBM, FDelegateHandle>& elemBmShop : _bmShopMsgHandlerList)
	{
		msg->GetBMShop().Remove(elemBmShop);
	}
	_bmShopMsgHandlerList.Empty();

	GBMShop()->RemoveBMShopHandlerItemDetailMsg();
}

void FGsStateLobbyZpay::OnKickUser(FGsSharedParam& InMessageParam)
{
	GMessage()->GetSystem().SendMessage(MessageSystem::RESTART_GAME);

	FText findText;
	FText::FindText(TEXT("NetText"), TEXT("ZPAY_NOTICE_PURCHASE_SUCCESS_RECOVERY"), findText);
	FGsUIHelper::PopupSystemMsg(findText, []()
		{
		});
}

void FGsStateLobbyZpay::OnReconectionStart()
{
	FGsContentsLobbyBase::OnReconectionStart();
}

void FGsStateLobbyZpay::OnReconectionEnd()
{
	FGsContentsLobbyBase::OnReconectionEnd();
}

