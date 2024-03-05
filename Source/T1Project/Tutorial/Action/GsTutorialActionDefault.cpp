// Fill out your copyright notice in the Description page of Project Settings.


#include "GsTutorialActionDefault.h"
#include "UI/UIContent/Tray/GsUITrayTutorial.h"


bool FGsTutorialActionDefault::OnStart()
{
	if (false == Super::OnStart())
	{
		return false;
	}

	if (false == _tutorialUI->SetNoTargetData(_table->id))
	{
		return false;
	}

	return true;
}
