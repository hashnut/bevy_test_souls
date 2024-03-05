// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUISampleEntry.h"



void UGsUISampleEntry::SetListItem(UGsUISampleScroll::FGsSampleData* InItem)
{
	if (nullptr == InItem)
	{
		return;
	}
	
	_testText = FText::AsNumber(InItem->_id);
}
