// Fill out your copyright notice in the Description page of Project Settings.


#include "GsPlayerDetailInfoMsgReceiver.h"

#include "GameFlow/GameContents/ContentsGame/Hud/PlayerDetailInfo/Handler/GsPlayerDetailInfoHandler.h"

#include "Message/MessageParam/GsPlayerDetailInfoMessageParam.h"
#include "Message/GsMessageGameObject.h"

#include "Management/GsMessageHolder.h"
#include "Management/ScopeGlobal/GsUIManager.h"
#include "Management/ScopeGlobal/GsGameDataManager.h"

#include "UI/UIContent/Popup/Stat/GsUIPopupPlayerStatDetail.h"


FGsPlayerDetailInfoMsgReceiver::~FGsPlayerDetailInfoMsgReceiver()
{
	_playerDetailInfoHandler = nullptr;
	RemoveMessage();
}

void FGsPlayerDetailInfoMsgReceiver::RemoveMessage()
{
	FGsMessageHolder* msg = GMessage();
	check(msg);

	for (TPair<MessageContentPlayerInfo, FDelegateHandle>& elemItem : _listPlayerDetailInfoDelegate)
	{
		msg->GetPlayerDetailInfo().Remove(elemItem);
	}
	_listPlayerDetailInfoDelegate.Empty();

}

void FGsPlayerDetailInfoMsgReceiver::RegisterMessage()
{
	FGsMessageHolder* msg = GMessage();
	check(msg);

	_listPlayerDetailInfoDelegate.Emplace(msg->GetPlayerDetailInfo().AddRaw(MessageContentPlayerInfo::OPEN,
		this, &FGsPlayerDetailInfoMsgReceiver::Open));
	_listPlayerDetailInfoDelegate.Emplace(msg->GetPlayerDetailInfo().AddRaw(MessageContentPlayerInfo::CLOSE,
		this, &FGsPlayerDetailInfoMsgReceiver::Close));
		
}

void FGsPlayerDetailInfoMsgReceiver::Open(FGsPlayerDetailInfoMessageParamBase* )
{
	if (UGsUIManager* uiManager = GUI())
	{
		if (true == uiManager->IsActiveWidget(TEXT("PopupPlayerDetailInfo")))
		{
			Close(nullptr);
			return;
		}

		uiManager->Open(TEXT("PopupPlayerDetailInfo"));

		// ÀÜ¿© ½ºÅÈ Á¸Àç ½Ã ½ºÅÈ ¼³Á¤ ÆË¾÷ Open
		int32 bonusStatPoint = GGameData()->GetUserData()->GetStatPoint();
		if (bonusStatPoint > 0)
		{
			UGsUIPopupPlayerStatDetail* popupStatDetail = Cast<UGsUIPopupPlayerStatDetail>(uiManager->OpenAndGetWidget(TEXT("PopupPlayerStatDetail")));
			if (nullptr == popupStatDetail)
				return;

			popupStatDetail->SetTab(EGsPlayerStatDetailType::Distribution);
		}
	}

	GMessage()->GetGameObject().SendMessage(MessageGameObject::CAMERA_EQUIP_VIEW_START, nullptr);
}

void FGsPlayerDetailInfoMsgReceiver::Close(FGsPlayerDetailInfoMessageParamBase* )
{
	if (UGsUIManager* uiManager = GUI())
	{
		uiManager->CloseByKeyName(TEXT("PopupPlayerDetailInfo"));
	}	
}