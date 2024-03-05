// Fill out your copyright notice in the Description page of Project Settings.


#include "GsTutorialActionHUDQuestSelect.h"
#include "DataSchema/Tutorial/GsSchemaTutorialActionUIInteraction.h"
#include "DataSchema/Tutorial/GsSchemaTutorialTargetId.h"
#include "Management/GsMessageHolder.h"
#include "Management/ScopeGlobal/GsUIManager.h"
#include "UI/UIContent/Tray/GsUITrayTutorial.h"
#include "UI/UIContent/HUD/GsUIHUDFrameMain.h"
#include "UI/UIContent/HUD/GsUIHUDQuest.h"
#include "UI/UIContent/HUD/GsUIHUDSideTab.h"
#include "UI/UIContent/HUD/Quest/GsUIHUDQuestSlot.h"


bool FGsTutorialActionHUDQuestSelect::OnStart()
{
	if (false == Super::OnStart())
	{
		return false;
	}
	
	const FGsSchemaTutorialTargetId* idTable = _table->actionRow.GetRowByType<FGsSchemaTutorialTargetId>();
	if (nullptr == idTable)
	{
		return false;
	}

	_actionUITable = _table->actionUIInteractionRow.GetRow();
	if (nullptr == _actionUITable)
	{
		return false;
	}

	_questUI = nullptr;

	TWeakObjectPtr<UGsUIWidgetBase> widget = GUI()->GetWidgetByKey(_actionUITable->wbpKey);
	if (widget.IsValid())
	{
		if (UGsUIHUDFrameMain* hudMain = Cast<UGsUIHUDFrameMain>(widget.Get()))
		{
			_questUI = hudMain->GetQuest();

			// 튜토리얼 직전 스캔버튼 누르는 등 여러 요인이 있어 강제로 퀘스트 탭으로 전환
			if (UGsUIHUDSideTab* sideTab = hudMain->GetSideTab())
			{
				sideTab->SetSelectedIndex(UGsUIHUDSideTab::EGsHUDSideTabType::QUEST);
			}
		}
	}

	if (false == _questUI.IsValid())
	{
		return false;
	}

	_questUI->OnTutorialQuestListEvent.BindRaw(this, &FGsTutorialActionHUDQuestSelect::OnTutorialQuestListEvent);
	_questUI->SetTutorialTarget(idTable->targetId, true);	

	return true;
}

void FGsTutorialActionHUDQuestSelect::OnEnd(bool bInSendMessage)
{
	if (_questUI.IsValid())
	{
		_questUI->SetTutorialTarget(0, false);
	}

	_questUI = nullptr;
	_actionUITable = nullptr;

	Super::OnEnd(bInSendMessage);
}

void FGsTutorialActionHUDQuestSelect::OnTutorialQuestListEvent(UWidget* InTargetWidget)
{
	if (false == _questUI.IsValid())
	{
		OnEnd();
		return;
	}

	_questUI->OnTutorialQuestListEvent.Unbind();

	// 이전 스탭에서 광클 시 퀵메뉴가 열려 퀘스트 슬롯을 클릭할 수 없는 이슈 수정 위함
	GMessage()->GetContents().SendMessage(MessageContents::CLOSE_QUICK_MENU);

	UGsUIHUDQuestSlot* questSlot = nullptr;
	if (InTargetWidget->IsA<UGsUIHUDQuestSlot>())
	{
		questSlot = Cast<UGsUIHUDQuestSlot>(InTargetWidget);
	}

	if (nullptr == questSlot)
	{
		OnEnd();
		return;
	}

	if (EGsTutorialUIInteractionType::INTERFACE_EVENT == _actionUITable->targetInteractionType)
	{
		UWidget* visualWidget = UGsUITrayTutorial::RecursiveFindWidget(questSlot,
			FName(_actionUITable->visualRootWidget), FName(_actionUITable->visualTargetWidget));

		if (false == _tutorialUI->SetTargetData(_table->id, _actionUITable, questSlot, visualWidget))
		{
			OnEnd();
			return;
		}
	}
	else
	{
		UWidget* targetWidget = UGsUITrayTutorial::RecursiveFindWidget(questSlot,
			FName(_actionUITable->targetRootWidget), FName(_actionUITable->targetWidget));

		if (false == _tutorialUI->SetTargetData(_table->id, _actionUITable, targetWidget))
		{
			OnEnd();
			return;
		}
	}
}
