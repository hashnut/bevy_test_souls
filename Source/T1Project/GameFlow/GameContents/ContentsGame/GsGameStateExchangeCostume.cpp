#include "GsGameStateExchangeCostume.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"

#include "GameFlow/GameContents/GsContentsMode.h"
#include "Management/ScopeGlobal/GsUIManager.h"
#include "Management/ScopeGame/GsEventProgressManager.h"
#include "Management/ScopeGame/GsSummonManager.h"

#include "UI/UIContent/Window/GsUIWindowExchangeCostume.h"
#include "Hud/Summon/GsSummonHandler.h"
#include "EventProgress/GsEventProgressDefine.h"
#include "Message/MessageParam/GsMessageParam.h"
#include "Message/MessageParam/GsCostumeMessageParam.h"

FGsGameStateExchangeCostume::FGsGameStateExchangeCostume() 
	: FGsContentsGameBase(FGsContentsMode::InGame::ContentsExchangeCostume)
{

}

FGsGameStateExchangeCostume::~FGsGameStateExchangeCostume()
{
	// 2023/6/13 PKT - 이곳에서 Exit함수 호출 하면 Update Reddot을 호출하여 크레시 발생 이슈 있음.
	//Exit();
}

void FGsGameStateExchangeCostume::Enter()
{
	FGsContentsGameBase::Enter();

	auto& costumeMsgManager = GMessage()->GetCostume();

	_msgCostumeHandleList.Emplace(costumeMsgManager.AddRaw(MessageCostume::EXCHANGE_COSTUME,
		this, &FGsGameStateExchangeCostume::OnExchangeCostumeByItem));

	_msgCostumeHandleList.Emplace(costumeMsgManager.AddRaw(MessageCostume::EXCHANGE_EVENT_COMPLETE,
		this, &FGsGameStateExchangeCostume::OnExchangeEventComplete));

	if (UGsUIManager* uiManager = GUI())
	{
		auto widget = uiManager->OpenAndGetWidget(TEXT("WindowExchangeCostume"));
		if (widget.IsValid())
		{
			_windowUI = Cast<UGsUIWindowExchangeCostume>(widget);
		}
	}

	_summonSceneHandler = UGsSummonManager::GetSummonHandler();

	CloseNavigationBar();
}

void FGsGameStateExchangeCostume::Exit()
{
	_windowUI = nullptr;

	if (_summonSceneHandler)
	{
		// 현재 play 중일때만 endsummon
		if (_summonSceneHandler->GetIsSummonPlay() == true)
		{
			_summonSceneHandler->EndSummon(false);

		}
		_summonSceneHandler = nullptr;
	}

	if (UGsUIManager* uiMgr = GUI())
	{
		uiMgr->ClearHideFlags(uiMgr->GetHideFlagsCurrent());
	}

	if (FGsMessageHolder* messageMgr = GMessage())
	{
		messageMgr->GetHudRedDot().SendMessage(MessageContentHUDRedDot::COSTUME, false);
	}

	if (FGsEventProgressManager* eventProgressManager = GSEventProgress())
	{
		eventProgressManager->OnReqFinishEvent(EGsEventProgressType::EXCHANGE_COSTUME_ROOM);
	}

	auto& costumeMsgManager = GMessage()->GetCostume();
	for (auto handler : _msgCostumeHandleList)
	{
		costumeMsgManager.Remove(handler);
	}

	FGsContentsGameBase::Exit();
}

void FGsGameStateExchangeCostume::OnExchangeCostumeByItem(const IGsMessageParam* InParam)
{
	if (const FGsCostumeMsgParamExchangeCostume* param = InParam->Cast<const FGsCostumeMsgParamExchangeCostume>())
	{
		_windowUI.Get()->SetCostumeExchangeTicketInfo(param->_selectedItem);
	}
}

void FGsGameStateExchangeCostume::OnExchangeEventComplete(const IGsMessageParam* InParam)
{
	if (_windowUI.IsValid())
	{
		_windowUI.Get()->Close();
	}
}