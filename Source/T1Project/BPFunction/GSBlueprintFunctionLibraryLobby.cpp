// Fill out your copyright notice in the Description page of Project Settings.


#include "GsBlueprintFunctionLibraryLobby.h"
#include "T1Project.h"
#include "Util/GsText.h"
#include "Management/GsMessageHolder.h"
#include "Management/ScopeGlobal/GsNetManager.h"
#include "UI/UIContent/Helper/GsUIHelper.h"


void UGsBlueprintFunctionLibraryLobby::SendEventGSLobby(EGS_LOBBY_Enum inMessage)
{
	//GSLOG(Warning, *EnumToString(EGS_LOBBY_Enum, inMessage));

	auto& stageMsgHandler = GMessage()->GetStage();

	// 개선: 구조화
	switch (inMessage)
	{
	case EGS_LOBBY_Enum::GS_LOBBY_END_INTRO:
	{		
	}
	break;	
	case EGS_LOBBY_Enum::GS_LOBBY_BACKTO_SERVER_SELECT:
	{		
	}
	break;
	case EGS_LOBBY_Enum::GS_LOBBY_BACKTO_CHARACTER_CREATE:
	{
		stageMsgHandler.SendMessage(MessageStage::ENTER_CHARACTERCREATE_STAGE);
	}
	break;
	}
}