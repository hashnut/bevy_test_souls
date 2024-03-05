// Fill out your copyright notice in the Description page of Project Settings.


#include "GsTutorialActionUIInteraction.h"
#include "DataSchema/Tutorial/GsSchemaTutorialActionUIInteraction.h"
#include "UI/UIContent/Tray/GsUITrayTutorial.h"


bool FGsTutorialActionUIInteraction::OnStart()
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

	if (false == _tutorialUI->SetTargetData(_table->id, actionUITable))
	{
		return false;
	}

	return true;
}
