// Fill out your copyright notice in the Description page of Project Settings.


#include "GsInvalidateUIInterface.h"


UGsInvalidateUIInterface::UGsInvalidateUIInterface(const class FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{

}
void IGsInvalidateUIInterface::InvalidateAll()
{ 
	SetDirtyInvalidate(true); 
}
void IGsInvalidateUIInterface::SetDirtyInvalidate(bool InDirty)
{ 
	_bDirty = InDirty; 
}
void IGsInvalidateUIInterface::CheckInvalidate()
{
	if (GetDirtyInvalidate())
	{
		SetDirtyInvalidate(false);
		InvalidateAllInternal();
	}
}
