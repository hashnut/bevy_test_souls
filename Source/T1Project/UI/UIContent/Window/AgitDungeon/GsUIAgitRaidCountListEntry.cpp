// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUIAgitRaidCountListEntry.h"
#include "Components/WidgetSwitcher.h"


void UGsUIAgitRaidCountListEntry::SetData(bool bIsActive)
{
	_switcherIcon->SetActiveWidgetIndex(bIsActive ? 1 : 0);
}
