// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUIDropDownMenuChat.h"
#include "UI/UIControlLib/ContentWidget/GsSwitcherButton.h"
#include "Components/WidgetSwitcher.h"

void UGsUIDropDownMenuChat::SetMenuPanelActive(bool set)
{
	 _useMenuPanel = set; 
	 _mainBtnSwitcher->SetVisibility(set ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed);
	 //_btnMenuOn->SetIsEnabled(set); // Color ¶§¹®¿¡

	 if (!_useMenuPanel)
	 {
		 CloseMenuPopup();
	 }
}
