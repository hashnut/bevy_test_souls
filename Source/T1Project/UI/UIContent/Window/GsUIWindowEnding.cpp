// Fill out your copyright notice in the Description page of Project Settings.

#include "GsUIWindowEnding.h"
#include "T1Project.h"
#include "T1Project/GSGameInstance.h"
#include "Management/ScopeGlobal/GsSoundManager.h"
#include "Management/ScopeGlobal/GsLevelManager.h"
#include "Button.h"

void UGsUIWindowEnding::NativeConstruct()
{
	Super::NativeConstruct();

	BtnOk = Cast<UButton>(GetWidgetFromName(TEXT("GsButton_Exit")));
	if (nullptr != BtnOk)
	{
		BtnOk->OnReleased.AddDynamic(this , &UGsUIWindowEnding::OnReleased);
	}
}

void UGsUIWindowEnding::NativeDestruct()
{
	BtnOk->OnReleased.Clear();

	Super::NativeDestruct();
}

void UGsUIWindowEnding::OnReleased()
{
	// Lobby Scene 으로 이동 ( Data All Reset )
	if (GetWorld())
	{
		
		if (UGsGameInstance* inst = Cast<UGsGameInstance>(GetWorld()->GetGameInstance()))
		{
			if (UGsLevelManager* level = GLevel())
			{
				level->BackToLobby();
			}
		}
	
	}

	Close();
}
