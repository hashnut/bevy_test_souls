#include "GsUIHUDChat.h"
#include "UI/UIControlLib/ContentWidget/GsButton.h"

#include "ScrollBox.h"

#include "Chat/GsUIChatItem.h"
#include "Management/GsMessageHolder.h"
#include "Management/ScopeGame/GsChatManager.h"
#include "Management/ScopeGlobal/GsUIManager.h"
#include "UI/UIContent/Popup/GsUIPopupChat.h"

void UGsUIHUDChat::NativeOnInitialized()
{
	Super::NativeOnInitialized();
}

void UGsUIHUDChat::NativeConstruct()
{
	Super::NativeConstruct();
	_secWndClose = GSChat()->hudEraseSec;

	OnChatAdded();
}

bool UGsUIHUDChat::UpdateManagerTick(float InDeltaTime)
{
	if (false == Super::UpdateManagerTick(InDeltaTime))
	{
		return false;
	}

	if (_lastInputTime > 0 && isWindowCloseTime())
	{
		OnChatAdded();
		_lastInputTime = 0;
		_chatLog->SetVisibility(ESlateVisibility::Collapsed);
	}

	return true;
}

void UGsUIHUDChat::OnChatAdded()
{
	TArray<TWeakPtr<FGsChatItem>> chatList;
	auto count = GSChat()->GetHudChatList(chatList);

	int32 index = 0;
	UGsUIChatItem* chatItems[4] = { _chatItem4, _chatItem3, _chatItem2, _chatItem1 };

	if (count > 0)
	{
		_lastInputTime = FGsTimeSyncUtil::GetServerNowSecond();

		auto iter = chatList.CreateConstIterator();
		iter.SetToEnd(); iter--;

		for (; iter; iter--, index++)
		{
			//auto data = iter->Pin();
			//auto markupPrefix = data->_namePrefix.IsEmpty() ? TEXT("") : data->_markupPrefix;

			chatItems[index]->SetChatData(*iter);
			//chatItems[index]->AddChatText(markupPrefix + data->_markupNick + data->_markupText);
			chatItems[index]->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		}
	}

	for (; index < 4; index++)
	{
		if (chatItems[index])
		{
			chatItems[index]->AddChatText("");
		}		
	}

	_chatLog->ScrollToEnd();
	_chatLog->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
}
