// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUIGuildAuctionResultEntry.h"

#include "Components/TextBlock.h"


void UGsUIGuildAuctionResultEntry::SetData(const FText& InGradeName, const FText& InUserName)
{
	_textBlockGrade->SetText(InGradeName);
	_textBlockUserName->SetText(InUserName);
}
