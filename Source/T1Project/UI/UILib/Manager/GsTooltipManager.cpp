// Fill out your copyright notice in the Description page of Project Settings.


#include "GsTooltipManager.h"

#include "BPFunction/UI/GsBlueprintFunctionLibraryUI.h"

#include "UI/UIContent/Tray/GsUITrayItemNameToolTip.h"

#include "UI/UIControlLib/Interface/GsNameTooltipInterface.h"

#include "UI/UILib/Define/GsUIDefine.h"


void UGsTooltipManager::Initialize()
{

}

void UGsTooltipManager::Finalize()
{

}

UGsUITrayItemNameToolTip* UGsTooltipManager::OpenNameTooltip(UUserWidget* InTarget, const FText& InName, ItemGrade InGrade /*= ItemGrade::NORMAL*/)
{
	if (InName.IsEmpty())
		return nullptr;

	if (UGsUITrayItemNameToolTip* nameTooltip = UGsBlueprintFunctionLibraryUI::AddNameTooltip(InTarget, UI_DEPTH_SYSTEM_TRAY))
	{
		nameTooltip->SetNameTooltipData(InName, InGrade);
		nameTooltip->SetTarget(InTarget);
		SetNameTooltipTarget(InTarget);
		return nameTooltip;
	}

	return nullptr;
}

void UGsTooltipManager::CloseNameTooltip()
{
	if (IGsNameTooltipInterface* targetWidget = Cast<IGsNameTooltipInterface>(GetNameTooltipTarget()))
	{
		targetWidget->CloseNameTooltip();
	}

	SetNameTooltipTarget(nullptr);
}
