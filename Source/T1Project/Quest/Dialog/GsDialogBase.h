// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/UILib/Base/GsUIWidgetBase.h"

class UGsUIManager;
class UGsGameObjectBase;
/**
 *
 */
class T1PROJECT_API FGsDialogBase
{
public:
	DECLARE_DELEGATE(FOnFinished);

protected:
	TWeakObjectPtr<class UGsUIManager>	_uiManager;
	TWeakObjectPtr<class UGsUIWidgetBase> _uiDialog;

public:
	FOnFinished _delegateOnFinished;


public:
	FGsDialogBase() = default;
	virtual ~FGsDialogBase() = default;

protected:
	TWeakObjectPtr<class UGsUIManager> GetUIManager();	// UIManager ¼± ¼ÂÆÃ

public:
	virtual void OpenDialog(const FName& InKey, int32 inDialogId = 0);
	virtual void CloseDialog();

public:
	bool IsActive() { return (nullptr == _uiDialog) ? false : _uiDialog->IsActive(); }
};
