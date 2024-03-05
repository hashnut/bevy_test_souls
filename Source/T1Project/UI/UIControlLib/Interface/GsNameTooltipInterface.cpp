// Fill out your copyright notice in the Description page of Project Settings.


#include "GsNameTooltipInterface.h"

#include "BPFunction/UI/GsBlueprintFunctionLibraryUI.h"


UGsNameTooltipInterface::UGsNameTooltipInterface(const class FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{

}

void IGsNameTooltipInterface::CloseNameTooltip()
{
	if (_nameToolTipObjPtr.IsValid())
	{
		UGsBlueprintFunctionLibraryUI::RemoveWidget(_nameToolTipObjPtr.Get());
		_nameToolTipObjPtr.Reset();
	}
}
