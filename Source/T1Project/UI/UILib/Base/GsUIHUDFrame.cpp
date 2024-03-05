// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUIHUDFrame.h"
#include "UI/UILib/Base/GsUIHUD.h"


void UGsUIHUDFrame::SetHUDMode(EGsUIHUDMode InMode)
{
	for (UGsUIHUD* hud : _hudList)
	{
		if (hud)
		{
			hud->SetHUDMode(InMode);
		}				
	}
}