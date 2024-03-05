// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUIRankingServerListEntry.h"

#include "Components/TextBlock.h"

#include "DataSchema/InterServer/World/GsSchemaWorldData.h"

#include "UI/UIControlLib/ContentWidget/GsButton.h"


void UGsUIRankingServerListEntry::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (_btnServer)
	{
		_btnServer->OnClicked.AddDynamic(this, &UGsUIRankingServerListEntry::OnClickServer);
	}
}

void UGsUIRankingServerListEntry::OnClickServer()
{
	OnClickedServer.ExecuteIfBound(_worldData);
}

void UGsUIRankingServerListEntry::SetData(const FGsSchemaWorldData* InWorldData)
{
	_worldData = InWorldData;

	_textBlockServer->SetText(_worldData->worldName);
}
