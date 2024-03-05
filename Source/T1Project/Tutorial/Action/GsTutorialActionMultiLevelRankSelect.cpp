// Fill out your copyright notice in the Description page of Project Settings.


#include "GsTutorialActionMultiLevelRankSelect.h"
#include "DataSchema/Tutorial/GsSchemaTutorialActionUIInteraction.h"
//#include "Skill/GsSkill.h"
//#include "Skill/GsSkillHelper.h"
//#include "Management/ScopeGlobal/GsGameDataManager.h"
#include "Management/ScopeGlobal/GsUIManager.h"
#include "UI/UIContent/Window/GsUIWindowMultiLevelRank.h"
#include "UI/UIContent/Tray/GsUITrayTutorial.h"


bool FGsTutorialActionMultiLevelRankSelect::OnStart()
{
	if (false == Super::OnStart())
	{
		return false;
	}

	_actionUITable = _table->actionUIInteractionRow.GetRow();
	if (nullptr == _actionUITable)
	{
		return false;
	}

	TWeakObjectPtr<UGsUIWidgetBase> widget = GUI()->GetWidgetByKey(_actionUITable->wbpKey);
	if (false == widget.IsValid())
	{
		return false;
	}

	_window = Cast<UGsUIWindowMultiLevelRank>(widget.Get());
	if (false == _window.IsValid())
	{
		return false;
	}

	_window->OnTutorialListEvent.BindRaw(this, &FGsTutorialActionMultiLevelRankSelect::OnListEvent);
	_window->SetTutorial(true);

	return true;
}

void FGsTutorialActionMultiLevelRankSelect::OnEnd(bool bInSendMessage)
{
	if (_window.IsValid())
	{
		_window->SetTutorial(false);
	}

	_actionUITable = nullptr;
	_window = nullptr;
	
	Super::OnEnd(bInSendMessage);
}

void FGsTutorialActionMultiLevelRankSelect::OnListEvent(UWidget* InTargetWidget)
{
	if (false == _window.IsValid())
	{
		OnEnd();
		return;
	}

	_window->OnTutorialListEvent.Unbind();

	if (InTargetWidget->IsA<UUserWidget>())
	{
		if (false == _tutorialUI->SetTargetData(_table->id, _actionUITable, InTargetWidget))
		{
			OnEnd();
		}
	}
}
