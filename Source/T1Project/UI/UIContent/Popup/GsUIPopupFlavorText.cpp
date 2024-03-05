// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/UIContent/Popup/GsUIPopupFlavorText.h"

void UGsUIPopupFlavorText::OnClickClose()
{
	Close();
}

void UGsUIPopupFlavorText::InitializeData(PopupInitData* initParam /*= nullptr*/)
{
	if (initParam)
	{
		_name = initParam->name;
		_flavor = initParam->flavorText;
	}
}

void UGsUIPopupFlavorText::OnInputCancel()
{
	OnClickClose();
}
