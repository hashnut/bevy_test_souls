// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUIGuildAuctionChat.h"

#include "Components/WidgetSwitcher.h"
#include "Components/ListView.h"
#include "Components/ScrollBox.h"

#include "Chat/GsChatItem.h"

#include "UI/UIControlLib/ContentWidget/GsButton.h"
#include "UI/UIControlLib/ContentWidget/GsScrollBox.h"
#include "UI/UIControlLib/ContentWidget/GsEditableTextBox.h"

#include "UI/UIContent/Popup/Chat/GsUIListDataChat.h"
#include "UI/UIContent/Popup/Chat/GsUIListEntryChat.h"

#include "UTIL/GsEnum.h"

#include "Management/GsMessageHolder.h"

#include "Management/ScopeGame/GsChatManager.h"
#include "Management/ScopeGlobal/GsGuildManager.h"

#include "Message/MessageParam/GsMessageParam.h"
#include "Message/MessageParam/GsUIMessageParam.h"

#include "Net/GsNetSendService.h"

#include "Shared/Client/SharedEnums/SharedChatEnum.h"


void UGsUIGuildAuctionChat::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	// 귓속말을 사용하지 않는다.
	_btnResetWhispererName->SetVisibility(ESlateVisibility::Collapsed);

	if (_scroll_Chat)
	{
		_scroll_Chat->OnUserScrolled.AddDynamic(this, &UGsUIGuildAuctionChat::OnUserScrolledChat);
	}

	if (_btnMacro)
	{
		_btnMacro->OnClicked.AddDynamic(this, &UGsUIGuildAuctionChat::OnClickMacro);
	}

	if (_editEnterText)
	{
		_editEnterText->SetIsEnabled(true);

		FText hintText;
		FText::FindText(TEXT("ChatText"), TEXT("ChatAble"), hintText);
		_editEnterText->SetText(FText::GetEmpty());
		_editEnterText->SetHintText(hintText);

		_editEnterText->OnTextCommitted.AddDynamic(this, &UGsUIGuildAuctionChat::OnCommitedText);
		_editEnterText->OnTextChanged.AddDynamic(this, &UGsUIGuildAuctionChat::OnChangedText);
	}
}

void UGsUIGuildAuctionChat::NativeConstruct()
{
	Super::NativeConstruct();

	FGsMessageHolder* msgHolder = GMessage();
	if (nullptr == msgHolder)
		return;

	MChat& msgChat = msgHolder->GetChat();
	_msgChatHandleList.Emplace(msgChat.AddUObject(MessageContentChat::ADD,
		this, &UGsUIGuildAuctionChat::OnChatListAdded));
	_msgChatHandleList.Emplace(msgChat.AddUObject(MessageContentChat::ADD_GUILD,
		this, &UGsUIGuildAuctionChat::OnChatListInserted));
	_msgChatHandleList.Emplace(msgChat.AddUObject(MessageContentChat::INVALIDATE,
		this, &UGsUIGuildAuctionChat::OnChatListRefresh));
	_msgChatHandleList.Emplace(msgChat.AddUObject(MessageContentChat::OPERATE_MESSAGE_UPDATE,
		this, &UGsUIGuildAuctionChat::OnOperateMessageAdded));

	//OnInputFocusReceived(IsInputFocused());

	if (_scroll_Chat)
	{
		_scroll_Chat->InitializeData();
		_scroll_Chat->ClearListItems();
		InvalidateChatListItem();
	}

	_guildChatPos = 0;
}

void UGsUIGuildAuctionChat::NativeDestruct()
{
	if (FGsMessageHolder* msgHolder = GMessage())
	{
		MChat& msgChat = msgHolder->GetChat();
		for (MsgChatHandle& handle : _msgChatHandleList)
		{
			msgChat.Remove(handle);
		}
		_msgChatHandleList.Empty();
	}

	Super::NativeDestruct();
}

void UGsUIGuildAuctionChat::Show()
{
	SetVisibility(ESlateVisibility::SelfHitTestInvisible);

	

	_editEnterText->SetText(FText::GetEmpty());
}

void UGsUIGuildAuctionChat::OnInputChat(const FText& InChatText)
{
	_editEnterText->SetText(FText::GetEmpty());

	UGsChatManager* chatManager = GSChat();
	if (nullptr == chatManager)
		return;

	if (InChatText.IsEmpty())
		return;

	TArray<TCHAR> arrText = InChatText.ToString().GetCharArray();
	bool isEnableText = Algo::AllOf(arrText, [](auto& e) { return e == 0x3000 || e == TEXT(' ') || e == TEXT('\t') || e == TEXT('\0'); });
	if (isEnableText)
		return;

	if (false == chatManager->IsNoSpammingChat(EGsExChatType::GUILD))
		return;

	else FGsNetSendService::SendReqChat(ChatType::GUILD, InChatText.ToString());
}

void UGsUIGuildAuctionChat::InvalidateChatListItem()
{
	UGsChatManager* chatManager = GSChat();
	if (nullptr == chatManager)
		return;

	_toBottomScrolled = true;
	TArray<TWeakPtr<FGsChatItem>> chatList;
	chatManager->GetChatDataList(EGsChatViewType::GUILD, chatList);

	TArray<UGsUIListDataChat*> listItems;
	for (auto weakItem : chatList)
	{
		if (UGsUIListDataChat* listItem = NewObject<UGsUIListDataChat>())
		{
			listItem->SetItemData(weakItem);
			listItems.Add(listItem);
		}
	}
	_scroll_Chat->SetListItems(listItems);
	_scroll_Chat->SetScrollEnd();

	_noHistorySelection->SetActiveWidgetIndex(chatList.Num() > 0 ? 0 : 1);
}

void UGsUIGuildAuctionChat::OnChatListAdded(const IGsMessageParam* InParam)
{
	if (nullptr == InParam)
		return;

	UGsChatManager* chatManager = GSChat();
	if (nullptr == chatManager)
		return;

	const FGsUIMsgParamChatItem* chatItemParam = InParam->Cast<const FGsUIMsgParamChatItem>();
	if (nullptr == chatItemParam)
		return;

	auto chatItem = chatItemParam->_item.Pin();
	if (nullptr == chatItem)
		return;

	if (EGsChatViewType::GUILD != chatItem->_chatViewType)
		return;

	UGsUIListDataChat* listItem = NewObject<UGsUIListDataChat>();
	if (nullptr == listItem)
		return;

	_noHistorySelection->SetActiveWidgetIndex(0);
	listItem->SetItemData(chatItem);
	_scroll_Chat->AddItem(listItem);

	int32 maxChat = chatManager->chatTabMax;
	while (_scroll_Chat->GetListItems().Num() > maxChat)
	{
		_scroll_Chat->RemoveItem(_scroll_Chat->GetItemAt(0));
	}

	if (_toBottomScrolled)
	{
		_scroll_Chat->SetScrollEnd();
	}
}

void UGsUIGuildAuctionChat::OnChatListInserted(const IGsMessageParam* InParam)
{
	if (nullptr == InParam)
		return;

	UGsChatManager* chatManager = GSChat();
	if (nullptr == chatManager)
		return;

	const FGsUIMsgParamChat* chatItemParam = InParam->Cast<const FGsUIMsgParamChat>();
	if (nullptr == chatItemParam)
		return;

	if (chatItemParam->_chatType != (uint16)(EGsExChatType::GUILD))
		return;

	auto preItemsCount = _scroll_Chat->GetListItems().Num();
	TArray<TWeakPtr<FGsChatItem>> chatList;
	chatManager->GetChatDataList(EGsChatViewType::GUILD, chatList);
	if (chatList.Num() <= 0 || preItemsCount == chatList.Num())
		return;

	int32 maxChat = chatManager->chatTabMax;
	TArray<UGsUIListDataChat*> listItems;
	auto riter = chatList.CreateIterator();
	riter.SetToEnd();
	riter--;
	for (int32 index = 0; riter && index < maxChat; riter--, index++)
	{
		if (UGsUIListDataChat* listItem = NewObject<UGsUIListDataChat>())
		{
			listItem->SetItemData(*riter);
			listItems.Insert(listItem, 0);
		}
	}

	_noHistorySelection->SetActiveWidgetIndex(listItems.Num() > 0 ? 0 : 1);
	_scroll_Chat->ClearListItems();
	_scroll_Chat->SetListItems(listItems);

	_scroll_Chat->SetScrollIndex(_guildChatPos + (listItems.Num() - preItemsCount) + 1);
	_guildChatPos = 0;
}

void UGsUIGuildAuctionChat::OnChatListRefresh(const IGsMessageParam* InParam)
{
	InvalidateChatListItem();
}

void UGsUIGuildAuctionChat::OnOperateMessageAdded(const IGsMessageParam* InParam)
{
	if (nullptr == InParam)
		return;

	UGsChatManager* chatManager = GSChat();
	if (nullptr == chatManager)
		return;

	const FGsUIMsgParamChatItem* chatItemParam = InParam->Cast<const FGsUIMsgParamChatItem>();
	if (nullptr == chatItemParam)
		return;
	
	auto chatItem = chatItemParam->_item.Pin();
	if (nullptr == chatItem)
		return;

	if (UGsUIListDataChat* listItem = NewObject<UGsUIListDataChat>())
	{
		listItem->SetItemData(chatItem);
		_scroll_Chat->AddItem(listItem);

		int32 maxChat = chatManager->chatTabMax;
		while (_scroll_Chat->GetListItems().Num() > maxChat)
		{
			_scroll_Chat->RemoveItem(_scroll_Chat->GetItemAt(0));
		}

		if (_toBottomScrolled)
		{
			_scroll_Chat->SetScrollEnd();
		}
	}

	_noHistorySelection->SetActiveWidgetIndex(0);
}

void UGsUIGuildAuctionChat::OnUserScrolledChat(float InOffset)
{
	UGsChatManager* chatManager = GSChat();
	if (nullptr == chatManager)
		return;

	if (_scroll_Chat->GetNumItems() <= 0)
		return;

	int32 top = _scroll_Chat->GetItemIndexByOffset(InOffset);
	_toBottomScrolled = _scroll_Chat->IsScrollEndPosition();

	auto usePos = top;
	if (_preChatTop > top)
	{
		const int REQ_KNIGHTS_COUNT{ 2 };

		if (_isUserScrolled == false && top <= REQ_KNIGHTS_COUNT)
		{
			_isUserScrolled = true; // 끌어내려 가장 위로
			top = 0;
		}
	}
	else if (_guildChatPos == 0)
	{
		_isUserScrolled = false; // 끌어올림
	}

	_preChatTop = top;

	if (GGuild()->IsGuild() && chatManager->IsValidPreGuildChatList())
	{
		if (_isUserScrolled && _guildChatPos == 0)
		{
			chatManager->SendReqGuildChat();
			_guildChatPos = usePos;
		}
	}
}

void UGsUIGuildAuctionChat::OnClickMacro()
{
	OnClickedMacro.ExecuteIfBound();
}

void UGsUIGuildAuctionChat::OnChangedText(const FText& InChangedText)
{
	FString strChangedText = InChangedText.ToString();
	TArray<TCHAR> arrText = strChangedText.GetCharArray();

	uint32 code = 0;
	int32 byteLen = 0;
	int32 txtLen = 0;

	for (auto srcIter = arrText.CreateConstIterator(); srcIter && *srcIter != TEXT('\0'); srcIter++, txtLen++)
	{
		code = (uint32)(*srcIter);
		if (code >= 0x80) byteLen++;
		byteLen++;

		//_curMaxInput = MAX_CHAR_NAME_LEN;
		if (byteLen > MAX_CHAR_CHAT_LEN)
			break;
	}

	if (byteLen > MAX_CHAR_CHAT_LEN)
	{
		_editEnterText->SetText(FText::FromString(strChangedText.Left(txtLen)));
	}
}

void UGsUIGuildAuctionChat::OnCommitedText(const FText& InInputText, ETextCommit::Type InCommitType)
{
	if (InCommitType == ETextCommit::Type::OnEnter && !InInputText.IsEmpty())
	{
		OnInputChat(InInputText);
	}
	else if (InCommitType == ETextCommit::OnCleared)
	{
		_editEnterText->SetText(FText::GetEmpty());
		_editEnterText->SetFocus();
	}
}
