// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUIGuildAuctionUserEntry.h"

#include "Components/TextBlock.h"


void UGsUIGuildAuctionUserEntry::SetData(const FString& InUserName)
{
	_textBlockUserName->SetText(FText::FromString(InUserName));
}