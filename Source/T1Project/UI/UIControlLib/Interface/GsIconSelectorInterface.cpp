// Fill out your copyright notice in the Description page of Project Settings.


#include "GsIconSelectorInterface.h"
#include "Components/PanelWidget.h"
#include "UI/UIContent/Common/Icon/GsUIIconBase.h"


UGsIconSelectorInterface::UGsIconSelectorInterface(const class FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{

}

void IGsIconSelectorInterface::RemoveAllChildren()
{
	UPanelWidget* panel = GetPanel();
	if (nullptr == panel)
	{
		return;
	}

	TArray<UWidget*> childList = panel->GetAllChildren();
	panel->ClearChildren();

	for (UWidget* child : childList)
	{
		if(UGsUIIconBase* icon = Cast<UGsUIIconBase>(child))
		{
			icon->ReleaseToPool();
		}
	}
}