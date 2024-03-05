// Fill out your copyright notice in the Description page of Project Settings.


#include "GsScopeBase.h"
#include "GSGameInstance.h"

void UGsScopeBase::Init()
{
	for (IGsManager* mgr : _managerList)
	{
		if (nullptr != mgr)
		{
			mgr->Initialize();
		}
	}
}

void UGsScopeBase::InitAfterResourceDownload()
{
	for (IGsManager* mgr : _managerList)
	{
		if (nullptr != mgr)
		{
			mgr->InitializeAfterResourceDownload();
		}
	}
}

void UGsScopeBase::Update(float elapsed)
{
	for (IGsManager* mgr : _managerList)
	{
		if (nullptr != mgr)
		{
			mgr->Update(elapsed);
		}
	}
}

void UGsScopeBase::Finalize()
{
	for (IGsManager* mgr : _managerList)
	{
		if (mgr)
		{
			mgr->Finalize();
		}
	}

	for (IGsManager* notObjectMgr : _fObjectManagerList)
	{
		if (notObjectMgr)
		{
			delete notObjectMgr;
			notObjectMgr = nullptr;
		}
	}

	_managerList.Empty();	
	_fObjectManagerList.Empty();
	_uObjectManagerList.Empty();
}

void UGsScopeBase::Shutdown()
{
	for (IGsManager* mgr : _managerList)
	{
		if (mgr)
		{
			mgr->Shutdown();
		}
	}

	for (IGsManager* notObjectMgr : _fObjectManagerList)
	{
		if (notObjectMgr)
		{
			delete  notObjectMgr;
			notObjectMgr = nullptr;
		}
	}

	_fObjectManagerList.Empty();
	_uObjectManagerList.Empty();
	_managerList.Empty();
}


void UGsScopeBase::QuitGame()
{
	UGsGameInstance* instance = Cast<UGsGameInstance>(GetWorld()->GetGameInstance());
	if (nullptr != instance)
	{
		instance->QuitGame();
	}
}
