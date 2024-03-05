// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUITrayEnchantFairyBase.h"
#include "Management/ScopeGlobal/GsUIManager.h"

// Add default functionality here for any IIGsFairyEnchantEffectTray functions that are not pure virtual.

void FGsTaskTrayUI::onStart()
{
	if (_onFnxStarted) _onFnxStarted();
	_widget = Cast<UGsUITrayEnchantFairyBase>(GUI()->GetWidgetByKey(*_trayUIName));
}

void FGsTaskTrayUI::onFinish()
{
	if (_onFnxFinished) _onFnxFinished();
}

bool FGsTaskTrayUI::cond()
{
	if (auto tray = _widget.Get())
	{
		return (tray->IsPlaying() && tray->IsFinished());
	}
	return _finished;
}