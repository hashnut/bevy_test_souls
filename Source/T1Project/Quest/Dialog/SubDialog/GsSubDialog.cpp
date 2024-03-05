// Fill out your copyright notice in the Description page of Project Settings.

#include "GsSubDialog.h"
#include "Quest/Dialog/GsDialogBase.h"
#include "Management/GsMessageHolder.h"
#include "Management/ScopeGlobal/GsUIManager.h"
#include "Management/ScopeGlobal/GsSoundManager.h"
#include "Message/GsMessageUI.h"
#include "SubDialog/GsSchemaSubDialogData.h"
#include "UI/UIContent/Tray/GsUITraySubDialog.h"
#include "Sound/GsSoundMixerController.h"
#include "T1Project.h"

void FGsSubDialog::OpenDialog(const FName& InKey, const FGsSchemaSubDialogData* inSubDialogData, bool inIsVoicePlayable)
{
	Super::OpenDialog(InKey);

	// 무조건 플레이 중인 보이스 중지
	OnStopVoice();

	_subDialogData = inSubDialogData;
	if (_subDialogData != nullptr) 
	{
		_remainCut = _subDialogData->cutIdList.Num();
	}
	
	// 새로 들어온 보이스 셋팅
	_isVoicePlayable = inIsVoicePlayable;	

	// 연출시작
	StartCutAction();
}

void FGsSubDialog::OpenDialog(const FName& InKey, int32 inDialogId)
{
	Super::OpenDialog(InKey, inDialogId);

	_subDialogData = GetSubDialogData(inDialogId);
	if (_subDialogData != nullptr)
	{
		_remainCut = _subDialogData->cutIdList.Num();
	}

	// 연출시작
	StartCutAction();
}

// 서브 대화창 오픈
void FGsSubDialog::StartCutAction()
{
	// DialogData가 없다.
	if (nullptr == _subDialogData)
		return;

	_cutCount = _subDialogData->cutIdList.Num();

	// 컷수가 남은 컷보다 작을수 없다.
	if (_remainCut > _cutCount)
		return;

	int index = _cutCount - _remainCut;
	if (false == _subDialogData->cutIdList.IsValidIndex(index))
		return;

	FGsSchemaSubDialogCutDataRow cutDataRow = _subDialogData->cutIdList[index];
	const FGsSchemaSubDialogCutData* cutData = cutDataRow.GetRow();

	// 컷 데이타가 없다.
	if (nullptr == cutData)
		return;
	
	if (_uiDialog.IsValid())
	{
		if (class UGsUITraySubDialog* uiSubDialog = Cast<UGsUITraySubDialog>(_uiDialog))
		{
			uiSubDialog->_delegateRemainTimeOver.BindRaw(this, &FGsSubDialog::NextCutAction);
			uiSubDialog->StartAction(cutData, _isVoicePlayable);
		}

		--_remainCut;
	}	
}

// 다음 대화 시작
void FGsSubDialog::NextCutAction()
{
	if (0 < _remainCut)
	{
		StartCutAction();
	}
	else
	{
		CloseDialog();
	}
}

void FGsSubDialog::CloseDialog()
{
	OnStopVoice();
	Super::CloseDialog();

	GMessage()->GetUI().SendMessage(MessageUI::FINISH_SUB_DIALOG, nullptr);
}

const FGsSchemaSubDialogData* FGsSubDialog::GetSubDialogData(int32 InRowKey)
{
	const UGsTableSubDialogData* table = Cast<UGsTableSubDialogData>(FGsSchemaSubDialogData::GetStaticTable());
	if (nullptr == table)
	{
		return nullptr;
	}

	const FGsSchemaSubDialogData* schemaSubDialogData = nullptr;
	if (false == table->FindRowById(InRowKey, schemaSubDialogData))
	{
		return nullptr;
	}

	return schemaSubDialogData;
}

void FGsSubDialog::OnStopVoice()
{
	if (_uiDialog.IsValid())
	{
		if (class UGsUITraySubDialog* uiSubDialog = Cast<UGsUITraySubDialog>(_uiDialog))
		{
			uiSubDialog->OnStopVoice();
			_isSoundMixerChanged = false;
		}
	}
}

void FGsSubDialog::ForceStopDialog()
{
#if WITH_EDITOR
	if (_subDialogData)
	{
		GSLOG(Warning, TEXT("[FGsSubDialog::ForceStopDialog] SubDialog (id : %d)"), _subDialogData->Id);
	}	
#endif

	CloseDialog();
}