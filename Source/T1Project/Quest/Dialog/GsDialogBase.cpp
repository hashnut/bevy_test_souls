// Fill out your copyright notice in the Description page of Project Settings.


#include "GsDialogBase.h"
#include "Management/ScopeGlobal/GsUIManager.h"
#include "GameObject/ObjectClass/GsGameObjectBase.h"

void FGsDialogBase::OpenDialog(const FName& InKey, int32 inDialogId)
{
	_uiManager = GetUIManager();
	if (_uiManager.IsValid())
	{
		_uiDialog = _uiManager->GetWidgetByKey(InKey);
		if (false == _uiDialog.IsValid())
		{
			_uiDialog = _uiManager->OpenAndGetWidget(InKey);
		}
	}
}

void FGsDialogBase::CloseDialog()
{
	if (nullptr != _uiDialog)
	{
		_uiDialog->Close();
	}
}

// UIManager ¼± ¼ÂÆÃ
TWeakObjectPtr<class UGsUIManager> FGsDialogBase::GetUIManager()
{
	if (nullptr == _uiManager)
	{
		_uiManager = GUI();
	}

	return _uiManager;
}
