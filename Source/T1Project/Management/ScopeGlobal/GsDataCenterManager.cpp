#include "GsDataCenterManager.h"

#ifdef GS_PATCH_COMPLEATED_EDITOR_SIMULATION
#include "Classes/GsSingleton.h"
#include "Message/GsMessageStage.h"
#endif

#include "Runtime/CoreUObject/Public/UObject/UObjectGlobals.h"
#include "Runtime/CoreUObject/Public/UObject/ObjectMacros.h"

#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "Management/GsMessageHolder.h"
#include "Management/GsScopeHolder.h"
#include "Message/GsMessageSystem.h"
#include "T1Project.h"
#include "Message/MessageParam/GsMessageParam.h"
#include "Message/MessageParam/GsStageMessageParam.h"

void FGsDataCenterManager::Initialize()
{
#if WITH_EDITOR
	if (nullptr != _tableManager)
	{
		GSLOG(Warning, TEXT("_tableManager was already instanced."));
	}
#endif

	_tableManager = NewObject<UGsTableManager>(GetTransientPackage(), UGsTableManager::StaticClass(), NAME_None, RF_MarkAsRootSet);

	UGsTableManager::SetInstance(_tableManager);

	{
		FGsMessageHolder* MessageHolder = GMessage();
		if (nullptr == MessageHolder)
		{
			GSLOG(Error, TEXT("nullptr == MessageHolder"));
		}
		else
		{
			MStageParam& StageParam = MessageHolder->GetStageParam();
			_listStageHandleDelegate.Emplace(StageParam.AddRaw(MessageStage::LOBBY_STATE_UPDATE_NOTIFY, this, &FGsDataCenterManager::OnLobbyStateUpdateNotify));
		}
	}

#ifdef GS_PATCH_COMPLEATED_EDITOR_SIMULATION
	{
		_listStageDelegate.Empty();
		FGsMessageHolder* MessageHolder = GMessage();
		check(!!MessageHolder);
		MStage& MessageHandler = MessageHolder->GetStage();
		_listStageDelegate.Emplace(MessageHandler.AddRaw(MessageStage::ENTER_NETADDRESSSELECT_STAGE, this, &FGsDataCenterManager::PatchCompleteEditorSimulation));
	}
#endif
}

void FGsDataCenterManager::Finalize()
{
#ifdef GS_PATCH_COMPLEATED_EDITOR_SIMULATION
	{
		FGsMessageHolder* MessageHolder = GMessage();
		if (nullptr != MessageHolder)
		{
			MStage& MessageHandler = MessageHolder->GetStage();
			for (MsgStageHandle& DelegateHandlePair : _listStageDelegate)
			{
				MessageHandler.Remove(DelegateHandlePair);
			}
		}
		_listStageDelegate.Empty();
	}
#endif

	FGsMessageHolder* MessageHolder = GMessage();
	if (nullptr == MessageHolder)
	{
		GSLOG(Error, TEXT("nullptr == MessageHolder"));
	}
	else
	{
		MStageParam& StageParam = MessageHolder->GetStageParam();
		for (MsgStageHandle& StageHandle : _listStageHandleDelegate)
		{
			StageParam.Remove(StageHandle);
		}
		_listStageHandleDelegate.Empty();
	}

	UGsTableManager::SetInstance(nullptr);

	if (nullptr != _tableManager)
	{
		if (_tableManager->IsValidLowLevel())
		{
			_tableManager->RemoveFromRoot();

			if (!_tableManager->IsPendingKill())
			{
				_tableManager->MarkPendingKill();
			}
		}

		_tableManager = nullptr;
	}
}

const UGsTableManager* FGsDataCenterManager::Get() const
{
#if WITH_EDITOR
	if (nullptr == _tableManager || !_tableManager->IsValidLowLevel())
	{
		GSLOG(Warning, TEXT("nullptr == _tableManager || !_tableManager->IsValidLowLevel()"));
	}
	else if (_tableManager->IsPendingKill())
	{
		GSLOG(Warning, TEXT("_tableManager->IsPendingKill()"));
	}
#endif

	return _tableManager;
}

void FGsDataCenterManager::OnLobbyStateUpdateNotify(const IGsMessageParam* InMessageParam)
{
	const FGsLobbyFlowMsgParam* LobbyFlowMsgParam = InMessageParam->Cast<const FGsLobbyFlowMsgParam>();
	if (nullptr == LobbyFlowMsgParam || nullptr == _tableManager)
	{
		return;
	}

	switch (LobbyFlowMsgParam->_messageType)
	{
	case MessageStage::REQUEST_PATCH_START:
	{
		GSLOG(Log, TEXT("_tableManager->SetTableFileAccessable(false)"));
		_tableManager->SetTableFileAccessable(false);
		break;
	}

	case MessageStage::CHECK_PATHC_COMPLETE:
	case MessageStage::COMPLETED_PATCH:
	{
		GSLOG(Log, TEXT("_tableManager->SetTableFileAccessable(true)"));
		_tableManager->SetTableFileAccessable(true);
		break;
	}
	}
}

#ifdef GS_PATCH_COMPLEATED_EDITOR_SIMULATION
void FGsDataCenterManager::PatchCompleteEditorSimulation()
{
	if (nullptr == _tableManager)
	{
		GSLOG(Warning, TEXT("nullptr == _tableManager"));
		return;
	}
	_tableManager->PatchCompleteEditorSimulation();
}
#endif
