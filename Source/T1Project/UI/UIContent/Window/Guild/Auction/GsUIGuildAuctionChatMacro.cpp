// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUIGuildAuctionChatMacro.h"

#include "Components/PanelWidget.h"

#include "UI/UIContent/Popup/Chat/GsUIChatMacroEntry.h"

#include "Management/ScopeGame/GsChatManager.h"


void UGsUIGuildAuctionChatMacro::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	TArray<UWidget*> childList = _panelMacro->GetAllChildren();
	for (int32 i = 0; i < childList.Num(); ++i)
	{
		if (UGsUIChatMacroEntry* macroEntry = Cast<UGsUIChatMacroEntry>(childList[i]))
		{
			macroEntry->SetData(i, GSChat()->GetChatMacro(i));
			macroEntry->OInputText.BindLambda([i](const FText& InText)
				{
					GSChat()->SetChatMacro(i, InText, false);
				});
			macroEntry->OnSendText.BindLambda([i, this]()
				{
					const FText& chatText = GSChat()->GetChatMacro(i);
					OnClickedMacroEntry.ExecuteIfBound(chatText);
				});
			_macroEntryList.Emplace(macroEntry);
		}
	}
}

void UGsUIGuildAuctionChatMacro::NativeConstruct()
{
	Super::NativeConstruct();

	UGsChatManager* chatManager = GSChat();
	if (nullptr == chatManager)
		return;

	for (int32 i = 0; i < _macroEntryList.Num(); ++i)
	{
		_macroEntryList[i]->SetData(i, chatManager->GetChatMacro(i));
	}
}

void UGsUIGuildAuctionChatMacro::Show()
{
	SetVisibility(ESlateVisibility::SelfHitTestInvisible);
}

void UGsUIGuildAuctionChatMacro::Hide()
{
	SetVisibility(ESlateVisibility::Collapsed);
}