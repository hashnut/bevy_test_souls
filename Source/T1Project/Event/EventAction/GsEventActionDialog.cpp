#include "GsEventActionDialog.h"

#include "GsSchemaBase.h"
#include "Management/GsScopeGlobal.h"
#include "Management/GsScopeHolder.h"
#include "Management/GsMessageHolder.h"
#include "Management/ScopeGlobal/GsUIManager.h"

#include "Management/ScopeGlobal/GsQuestManager.h"

#include "Quest/Management/GsQuestActionManagement.h"

#include "Message/MessageParam/GsMessageParam.h"
#include "Message/MessageParam/GsCameraMessageParam.h"

#include "Camera/Define/GsCameraGameDefine.h"

#include "DataCenter/Public/DataSchema/Event/EventAction/GsSchemaEventActionNpcDialog.h"

#include "UTIL/GsTableUtil.h"

#include "T1Project/T1Project.h"
#include "Management/ScopeGame/GsGameObjectManager.h"

void UGsEventActionDialog::Initialize(const FGsSchemaBase* EventActionData)
{
	const FGsSchemaEventActionNpcDialog* eventActionNpcDialogData = StaticCast<const FGsSchemaEventActionNpcDialog*>(EventActionData);
	if (nullptr == eventActionNpcDialogData)
	{
		GSLOG(Error, TEXT("nullptr == eventActionNpcDialogData"));
		return;
	}

	_eventActionNpcDialogId = eventActionNpcDialogData->id;
}

bool UGsEventActionDialog::OnAfterPlay()
{
	// 시작 전 ui 다 닫기
	UGsUIManager* UiManager = GUI();
	if (nullptr == UiManager)
	{
		GSLOG(Error, TEXT("nullptr == UiManager"));
		return false;
	}

	UiManager->CloseAllStack();
	UiManager->HideTicker();

	AddObserver();
	
	const FGsSchemaEventActionNpcDialog* eventActionNpcDialogTbl = UGsTableUtil::FindRowById<UGsTableEventActionNpcDialog, FGsSchemaEventActionNpcDialog>(_eventActionNpcDialogId);
	if (eventActionNpcDialogTbl == nullptr)
	{
		return false;
	}

	FGsQuestActionManagement* actionManagement = GSQuest()->GetQuestActionManagement();
	if (nullptr == actionManagement)
	{
		return false;
	}

	const FGsSchemaNpcData* npcData = eventActionNpcDialogTbl->npcDataId.GetRow();
	const FGsSchemaNpcDialogData* dialogData = eventActionNpcDialogTbl->npcDialogData.GetRow();
	UGsGameObjectManager* GameObjectManager = GSGameObject();
	if (nullptr == npcData || nullptr == dialogData || nullptr == GameObjectManager)
	{
		return false;
	}

	UGsGameObjectBase* findObj = GameObjectManager->FindObjectLocalNearestNonPlayerByNPCId(npcData->id);
	if (nullptr == findObj)
	{
		return false;;
	}

	actionManagement->OpenEventDialog(npcData->id, dialogData->Id);
	return true;
}

void UGsEventActionDialog::OnFinish()
{
	RemoveObserver();

	UGsUIManager* UiManager = GUI();
	if (nullptr == UiManager)
	{
		GSLOG(Error, TEXT("nullptr == UiManager"));
	}
	else
	{
		UiManager->ShowTicker();
	}
}
void UGsEventActionDialog::AddObserver()
{
	if (_dialogEndHandle.Value.IsValid())
	{
		GSLOG(Error, TEXT("_dialogEndHandle.Value.IsValid(), _eventActionNpcDialogId:%d"), _eventActionNpcDialogId);
	}


	FGsMessageHolder* MessageHolder = GMessage();
	if (nullptr != MessageHolder)
	{
		_dialogEndHandle
			= MessageHolder->GetGameObject().AddUObject(MessageGameObject::CAMERA_DIALOG_END_LERP_END
				, this, &UGsEventActionDialog::CloseNpcDialog);
	}
}
void UGsEventActionDialog::RemoveObserver()
{
	if (!_dialogEndHandle.Value.IsValid())
	{
		return;
	}

	if (nullptr == GScope())
	{
		// 시네마틱 진행 중 게임 정지하면 이 부분에서 크래쉬를 발생시켜 예외 처리함.
		return;
	}

	FGsMessageHolder* MessageHolder = GMessage();
	if (nullptr != MessageHolder)
	{
		MessageHolder->GetGameObject().Remove(_dialogEndHandle);
		_dialogEndHandle.Value.Reset();
	}
}

void UGsEventActionDialog::CloseNpcDialog(const IGsMessageParam* In_data)
{
	const FGsCameraDialogEndMessageParam* paramTarget =
		In_data->Cast<const FGsCameraDialogEndMessageParam>();
	if (nullptr == paramTarget ||
		EGsCamearDialogEndCallBackType::EventAction != paramTarget->_cameraDialogCallbackType)
	{
		return;
	}

	Finish();
}
