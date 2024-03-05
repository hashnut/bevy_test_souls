// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUIIconBase.h"
#include "UI/UILib/Manager/GsWidgetPoolManager.h"
#include "BPFunction/UI/GsBlueprintFunctionLibraryUI.h"
#include "T1Project.h"


void UGsUIIconBase::ReleaseToPool()
{
	if (false == bIsManagedByPool)
	{
		return;
	}

	UGsWidgetPoolManager* poolManager = UGsBlueprintFunctionLibraryUI::GetWidgetPoolManager();
	if (poolManager)
	{
		poolManager->ReleaseWidget(GetClass(), this);
	}
}
