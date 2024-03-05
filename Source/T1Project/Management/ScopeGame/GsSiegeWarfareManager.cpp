// Fill out your copyright notice in the Description page of Project Settings.


#include "GsSiegeWarfareManager.h"
#include "DataSchema/Tax/GsSchemaTaxConfig.h"


FGsSiegeWarfareManager::~FGsSiegeWarfareManager()
{
	Finalize();
}

void FGsSiegeWarfareManager::Initialize()
{
	_taxHandler = new FGsTaxDataHandler();
}

void FGsSiegeWarfareManager::Finalize()
{
	if (_taxHandler)
	{
		_taxHandler->Finalize();
		delete _taxHandler;
	}

	_taxHandler = nullptr;
}

void FGsSiegeWarfareManager::InitializeAfterResourceDownload()
{
	if (_taxHandler)
	{
		_taxHandler->Initialize();
	}
}

void FGsSiegeWarfareManager::ResetTimeContents(ResetTimeTicket InType)
{
	// TODO: 시작/종료시간 리셋이 되어야 할 듯한데, 서버 논의 필요함
}
