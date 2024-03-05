// Fill out your copyright notice in the Description page of Project Settings.


#include "GsPlayerDetailInfoHandler.h"

#include "GameFlow/GameContents/ContentsGame/Hud/PlayerDetailInfo/MsgReceiver/GsPlayerDetailInfoMsgReceiver.h"


FGsPlayerDetailInfoHandler::~FGsPlayerDetailInfoHandler()
{
}

void FGsPlayerDetailInfoHandler::Init()
{
	_msgReciver = new FGsPlayerDetailInfoMsgReceiver();
	_msgReciver->SetHandler(this);
	_msgReciver->RegisterMessage();
}

void FGsPlayerDetailInfoHandler::Close()
{
	if (nullptr != _msgReciver)
	{
		delete _msgReciver;
		_msgReciver = nullptr;
	}
}

void FGsPlayerDetailInfoHandler::OnReconection()
{
	
}