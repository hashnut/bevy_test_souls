// Fill out your copyright notice in the Description page of Project Settings.


#include "GsTutorialActionSummonShowImmediately.h"
#include "DataSchema/Tutorial/GsSchemaTutorialActionUIInteraction.h"
#include "UI/UIContent/Tray/GsUITrayTutorial.h"
#include "UI/UIContent/Tray/GsUITraySummon.h"
#include "Management/ScopeGlobal/GsUIManager.h"
#include "Summon/GsSummonDefine.h"


bool FGsTutorialActionSummonShowImmediately::OnStart()
{
	if (false == Super::OnStart())
	{
		return false;
	}

	const FGsSchemaTutorialActionUIInteraction* actionUITable = 
		_table->actionUIInteractionRow.GetRow();
	if (nullptr == actionUITable)
	{
		return false;
	}
	
	TWeakObjectPtr<UGsUIWidgetBase> widget = GUI()->GetWidgetByKey(actionUITable->wbpKey);
	if (false == widget.IsValid())
	{
		return false;
	}

	UGsUITraySummon* traySummon = Cast<UGsUITraySummon>(widget.Get());
	if (nullptr == traySummon)
	{
		return false;
	}

	UWidget* targetWidget = nullptr;
	switch (traySummon->GetButtonType())
	{
	case EGsSummonBtnType::OPEN_ALL:
		targetWidget = UGsUITrayTutorial::RecursiveFindWidget(traySummon,
			FName(actionUITable->targetRootWidget), FName(TEXT("_btnOpenAll")));
		break;
	case EGsSummonBtnType::SHOW_RESULT:
		targetWidget = UGsUITrayTutorial::RecursiveFindWidget(traySummon,
			FName(actionUITable->targetRootWidget), FName(TEXT("_btnShowResult")));
		break;
	}

	if (nullptr == targetWidget)
	{
		return false;
	}

	if (false == _tutorialUI->SetTargetData(_table->id, actionUITable, targetWidget))
	{
		return false;
	}

	return true;
}
