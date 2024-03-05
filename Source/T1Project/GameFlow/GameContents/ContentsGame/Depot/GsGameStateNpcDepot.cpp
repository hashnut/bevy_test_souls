// Fill out your copyright notice in the Description page of Project Settings.


#include "GsGameStateNpcDepot.h"

#include "EventProgress/GsEventProgressDefine.h"

#include "KeyMapping/GsKeyMappingEnum.h"

#include "Management/GsMessageHolder.h"
#include "Management/ScopeGame/GsDepotManager.h"
#include "Management/ScopeGame/GsEventProgressManager.h"

#include "Message/MessageParam/GsInteractionMessageParam.h"
#include "Message/MessageParam/GsItemContentsMessageParam.h"
#include "Message/MessageParam/GsUIMessageParam.h"
#include "Message/MessageParam/GsMessageParam.h"

#include "Input/GsInputEventMsgBase.h"

#include "Item/GsItemManager.h"


FGsGameStateNpcDepot::FGsGameStateNpcDepot() : FGsContentsGameBase(FGsContentsMode::InGame::ContentsNpcDepot)
{
	if (FGsMessageHolder* message = GMessage())
	{
		_interactionMsgHandler = message->GetGameObject().AddRaw(MessageGameObject::INTERACTION_CONTENTS_BY_NPC_START,
			this, &FGsGameStateNpcDepot::OnInteractionContents);
	}
}

FGsGameStateNpcDepot::~FGsGameStateNpcDepot()
{
	if (FGsMessageHolder* message = GMessage())
	{
		message->GetGameObject().Remove(_interactionMsgHandler);
	}
}

void FGsGameStateNpcDepot::Enter()
{
	Super::Enter();

	UGsUIManager* uiMgr = GUI();
	if (nullptr == uiMgr)
		return;

	// 창고 UI 열기
	TWeakObjectPtr<UGsUIWidgetBase> windowWidget = uiMgr->OpenAndGetWidget(TEXT("WindowDepot"));
	if (false == windowWidget.IsValid())
		return;

	// UI HideFlag 설정
	uiMgr->SetHideFlags(EGsUIHideFlags::STATE_NPC_SHOP);

	// 상단 재화 UI 표시 (기본)
	SetNavigationBar(CurrencyType::NONE);

	// 이동 막기
	if (FGsMessageHolder* message = GMessage())
	{
		FGsInputEventMsgFlag msg(EGsKeyMappingInputFlags::SETTING_NPC, true);
		message->GetInput().SendMessage(MessageInput::BlockInput, msg);
	}
}

void FGsGameStateNpcDepot::Exit()
{
	UGsUIManager* uiMgr = GUI();
	if (nullptr == uiMgr)
		return;

	uiMgr->CloseByKeyName(TEXT("WindowDepot"));
	uiMgr->ClearHideFlags(EGsUIHideFlags::STATE_NPC_SHOP);

	if (FGsEventProgressManager* eventProgressManager = GSEventProgress())
	{
		eventProgressManager->OnReqFinishEvent(EGsEventProgressType::NPC_SHOP);
	}

	// 이동 풀기
	if (FGsMessageHolder* message = GMessage())
	{
		FGsInputEventMsgFlag msg(EGsKeyMappingInputFlags::SETTING_NPC, false);
		message->GetInput().SendMessage(MessageInput::BlockInput, msg);
	}

	Super::Exit();
}

void FGsGameStateNpcDepot::OnInteractionContents(const IGsMessageParam* In_data)
{
	const FGsInteractionMessageParamContents* interactionParam = In_data->Cast<const FGsInteractionMessageParamContents>();
	if (nullptr == interactionParam)
		return;

	const FGsSchemaNpcData* npcData = UGsTableUtil::FindRowById<UGsTableNpcData, FGsSchemaNpcData>(interactionParam->_npcTblId);
	if (nullptr == npcData)
	{
#if WITH_EDITOR
		GSLOG(Warning, TEXT("FGsSchemaNpcData::FindRowById is EmptyData - _npcTblId : %d"), interactionParam->_npcTblId);
#endif
		return;
	}

	if (NpcFunctionType::DEPOT != npcData->npcFunctionType)
		return;

	FGsDepotManager* depotMgr = GSDepot();
	if (nullptr == depotMgr)
		return;

	depotMgr->SetNpcCreatureID(npcData->id);

	if (FGsMessageHolder* message = GMessage())
	{
		message->GetContents().SendMessage(MessageContents::ACTIVE_WINDOW_NPC_DEPOT);
	}
	
}