// Fill out your copyright notice in the Description page of Project Settings.

#include "GsUIPopupChatOptionNBlock.h"
#include "Management/ScopeGame/GsChatManager.h"
#include "Management/GsMessageHolder.h"
#include "Message/MessageParam/GsUIMessageParam.h"
#include "Chat/GsUIChatBlockEntry.h"
#include "Chat/GsUIChatOptionButton.h"

#include "UI/UIContent/Helper/GsUIHelper.h"
#include "UI/UIControlLib/Control/GsDynamicPanelSlotHelper.h"
#include "UI/UIControlLib/ContentWidget/GsSwitcherButton.h"
#include "UI/UIControlLib/ContentWidget/GsButton.h"
#include "ScrollBox.h"
#include "EditableTextBox.h"
#include "TextBlock.h"
#include "VerticalBox.h"
#include "CanvasPanel.h"
#include "WidgetSwitcher.h"

#include "Guild/GsGuildGroupChatData.h"
#include "Net/GsNetSendService.h"

void UGsUIPopupChatOptionNBlock::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	_checkViewChat =
	{
		/*full*/ { EGsOptionChat::SYSTEM ,_checkSystem }, 
		{ EGsOptionChat::NORMAL_CHAT, _checkNormal }, { EGsOptionChat::SERVER_CHAT, _checkServer }, 
		{ EGsOptionChat::INVADE_CHAT, _checkInvade }, { EGsOptionChat::PARTY_CHAT, _checkParty }, 
		{ EGsOptionChat::ALLYGUILD_CHAT, _checkAllience }, { EGsOptionChat::GUILD_CHAT, _checkKnights }, 
		{ EGsOptionChat::WHISPER, _checkWhisper }
	};

	TArray<UWidget*> childList = _listPanel->GetAllChildren();
	for (auto child : childList)
	{
		if (child->IsA<UGsUIChatBlockEntry>())
		{
			child->RemoveFromParent();
		}
	}

	_scrollBoxHelper = NewObject<UGsDynamicPanelSlotHelper>(this);
	_scrollBoxHelper->Initialize(_entryWidgetClass, _listPanel);
	_scrollBoxHelper->SetEmptyListPanel(_emptyPanel);
	_scrollBoxHelper->OnCreateEntry.AddDynamic(this, &UGsUIPopupChatOptionNBlock::OnCreateEntry);
	_scrollBoxHelper->OnRefreshEntry.AddDynamic(this, &UGsUIPopupChatOptionNBlock::OnRefreshEntry);

	_toggleGroup.AddToggleWidget(_tabBtnChatSetting);
	_toggleGroup.AddToggleWidget(_tabBtnBlockList);
	_toggleGroup.OnSelectChanged.BindUObject(this, &UGsUIPopupChatOptionNBlock::OnSelectedTab);

	_btnBlock->OnClicked.AddDynamic(this, &UGsUIPopupChatOptionNBlock::OnClickBlock);

	_touchClose->OnPressed.AddDynamic(this, &UGsUIPopupChatOptionNBlock::OnTouchedExtraRange);
}

void UGsUIPopupChatOptionNBlock::BeginDestroy()
{
	if (nullptr != _scrollBoxHelper)
	{
		_scrollBoxHelper->OnCreateEntry.RemoveDynamic(this, &UGsUIPopupChatOptionNBlock::OnCreateEntry);
		_scrollBoxHelper->OnRefreshEntry.RemoveDynamic(this, &UGsUIPopupChatOptionNBlock::OnRefreshEntry);
		_scrollBoxHelper = nullptr;
	}

	Super::BeginDestroy();
}

void UGsUIPopupChatOptionNBlock::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (_selectedIndex == -1)
		_toggleGroup.SetSelectedIndex(_toggleGroup.GetSelectedIndex(), true);
}

void UGsUIPopupChatOptionNBlock::NativeConstruct()
{
	_blockList.Empty();
	_blockList.Append(GGameData()->GetChatBlockList());

	_scrollBoxHelper->RefreshAll(_blockList.Num());
	_textCount->SetText(FText::FromString(FString::Format(TEXT("{0}/{1}"), { _blockList.Num(), 100 })));

	FGsMessageHolder* msgHolder = GMessage();
	_msgHandler = msgHolder->GetChat().AddUObject(MessageContentChat::INVALIDATE_BLOCK_LIST,
		this, &UGsUIPopupChatOptionNBlock::OnChatBlockListUpdate);

	Super::NativeConstruct();

	InitializeData(nullptr);
}

void UGsUIPopupChatOptionNBlock::NativeDestruct()
{
	FGsMessageHolder* msgHolder = GMessage();
	msgHolder->GetChat().Remove(_msgHandler);

	Super::NativeDestruct();
}

void UGsUIPopupChatOptionNBlock::CloseInternal()
{
	if (_checkAll->GetIsSelected())
	{
		for (auto& iter : _checkViewChat)
		{
			GSChat()->SetChatViewOption(iter.Key, true);
		}

		for (auto e : _checkViewGroupChat)
		{
			GSChat()->SetGroupChatViewOption(e->GetIndexInGroup(), true);
		}
	}
	else
	{
		for (auto iter : _checkViewChat)
		{
			GSChat()->SetChatViewOption(iter.Key, iter.Value->GetIsSelected());
		}

		for (auto e : _checkViewGroupChat)
		{
			GSChat()->SetGroupChatViewOption(e->GetIndexInGroup(), e->GetIsSelected());
		}
	}

	GSChat()->SetChatViewOption(EGsOptionChat::TALK_POPUP, _checkTalkBubble->GetIsSelected());
	GSChat()->SetChatViewOption(EGsOptionChat::EMOJI_POPUP, _checkEmoticon->GetIsSelected());

	GSChat()->SetChatViewOption(EGsOptionChat::MAX, false, true);

	OnExit();

	Super::CloseInternal();
}

void UGsUIPopupChatOptionNBlock::InitializeData(PopupInitData* initParam)
{
	if (isInitialzed) return;

	TArray<TWeakPtr<FGsGuildGroupChatRoom>> chatGroupRooms;
	int groupRoomCount = GGuild()->GetGroupChatRooms(chatGroupRooms);

	int32 fullchatCount = 0;
	for (auto chatopt : _checkViewChat)
	{
		fullchatCount += GSChat()->GetChatViewOption(chatopt.Key) ? 1 : 0;
	}

	for (auto room : chatGroupRooms)
	{
		fullchatCount += GSChat()->GetChatGroupViewOption(room.Pin()->GetRoomId()) ? 1 : 0;
	}

	TWeakObjectPtr<UGsUIPopupChatOptionNBlock> thiz = this;

	_checkAll->SetIndexInGroup(99);
	_checkAll->SetIsSelected(fullchatCount == _checkViewChat.Num() + groupRoomCount);
	_checkAll->SetToggleCallback([thiz](int32 index, bool isSelected) {
			if(thiz.IsValid()) thiz->OnSelectedAllCheck(isSelected);
		});

	for (auto iter : _checkViewChat)
	{
		iter.Value->SetIndexInGroup(static_cast<int32>(iter.Key));
		iter.Value->SetIsSelected(GSChat()->GetChatViewOption(iter.Key));
		iter.Value->SetToggleCallback([thiz](int32 index, bool isSelected) {
				if (thiz.IsValid()) thiz->OnSelectedTabCheck(static_cast<EGsOptionChat>(index), isSelected);
			});
	}

	_groupChatOptions->SetVisibility(groupRoomCount <= 0 ? ESlateVisibility::Collapsed : ESlateVisibility::SelfHitTestInvisible);
	_optionPanel->ClearChildren();

	_checkViewGroupChat.Reset();
	while (groupRoomCount-- > 0)
	{
		auto index = _checkViewGroupChat.Add(Cast<UGsUIChatOptionButton>(CreateWidget(GetWorld(), _entryGroupButton)));
		_optionPanel->AddChild(_checkViewGroupChat[index]);

		if (auto room = chatGroupRooms[index].Pin())
		{
			_checkViewGroupChat[index]->SetButtonName(FText::FromString(room->GetGroupRoomName()));
			_checkViewGroupChat[index]->SetIndexInGroup(room->GetRoomId());
			_checkViewGroupChat[index]->SetIsSelected(GSChat()->GetChatGroupViewOption(room->GetRoomId()));
			_checkViewGroupChat[index]->SetToggleCallback([thiz](uint64 roomId, bool isSelected) {
				if (thiz.IsValid()) thiz->OnSelectedGroupCheck(roomId, isSelected);
			});
		}
	}

	_checkTalkBubble->SetIndexInGroup(static_cast<int32>(EGsOptionChat::TALK_POPUP));
	_checkTalkBubble->SetIsSelected(GSChat()->GetChatViewOption(EGsOptionChat::TALK_POPUP));
	_checkTalkBubble->SetToggleCallback([thiz](int32 index, bool isSelected) {
			if (thiz.IsValid()) thiz->OnTalkBubblePopCheck(isSelected);
		});

	_checkEmoticon->SetIndexInGroup(static_cast<int32>(EGsOptionChat::EMOJI_POPUP));
	_checkEmoticon->SetIsSelected(GSChat()->GetChatViewOption(EGsOptionChat::EMOJI_POPUP));
	_checkEmoticon->SetToggleCallback([thiz](int32 index, bool isSelected) {
			if (thiz.IsValid()) thiz->OnEmojiPopCheck(isSelected);
		});

	isInitialzed = true;
}

void UGsUIPopupChatOptionNBlock::OnExit()
{
	if (!isInitialzed) return;

	_checkTalkBubble->SetToggleCallback(nullptr);
	_checkEmoticon->SetToggleCallback(nullptr);
	_checkAll->SetToggleCallback(nullptr);

	for (auto iter : _checkViewChat)
	{
		iter.Value->SetToggleCallback(nullptr);
	}

	for (auto e : _checkViewGroupChat)
	{
		e->SetToggleCallback(nullptr);
	}

	isInitialzed = false;
}

void UGsUIPopupChatOptionNBlock::OnTouchedExtraRange()
{
	Close();
}

void UGsUIPopupChatOptionNBlock::OnClickClose()
{
	Close();
}

void UGsUIPopupChatOptionNBlock::OnSelectedTab(int32 index)
{
	_selectedIndex = index;
	_tabSwitcher->SetActiveWidgetIndex(index);
}

void UGsUIPopupChatOptionNBlock::OnClickBlock()
{
	auto blockName = _editEnterText->GetText();
	if (!blockName.IsEmpty())
	{
		if (GGameData()->GetUserData()->mUserName.Compare(blockName.ToString()) == 0)
		{
			//자기 자신을 차단할 수 없습니다.
			FText findText;
			FText::FindText(TEXT("ChatText"), TEXT("ChatTicker10"), findText);
			FGsUIHelper::TraySectionMessageTicker(EGsNotiSection::EXTSYSTEM, findText);
			return;
		}

		auto cmpName = blockName.ToString();
		if (_blockList.FindByPredicate([cmpName](FGsChatBlockUser& e) {
			return 	e._name.Compare(cmpName) == 0;
			}))
		{
			//이미 차단한 유저입니다
			FText findText;
			FText::FindText(TEXT("ChatText"), TEXT("ChatTicker7"), findText);
			FGsUIHelper::TraySectionMessageTicker(EGsNotiSection::EXTSYSTEM, findText);
			return;
		}

		FGsNetSendService::SendReqBlockChatUser(blockName.ToString());
	}

	_editEnterText->SetText(FText::GetEmpty());
}

void UGsUIPopupChatOptionNBlock::OnCreateEntry(UWidget* InEntry)
{
	if (auto entry = Cast<UGsUIChatBlockEntry>(InEntry))
	{
		entry->OnReqUnblock.BindUObject(this, &UGsUIPopupChatOptionNBlock::OnReqChatUserUnblock);
	}
}

void UGsUIPopupChatOptionNBlock::OnRefreshEntry(int32 index, UWidget* InEntry)
{
	if (_blockList.IsValidIndex(index))
	{
		if (auto entry = Cast<UGsUIChatBlockEntry>(InEntry))
		{
			entry->SetData(&_blockList[index]);
			entry->RegreshUI();
		}
	}
}

void UGsUIPopupChatOptionNBlock::OnChatBlockListUpdate(const IGsMessageParam* param)
{
	auto chatParam = param->Cast<const FGsUIMsgParamChatBlock>();

	_blockList.Empty();
	_blockList.Append(GGameData()->GetChatBlockList());
	_scrollBoxHelper->RefreshAll(_blockList.Num());

	_textCount->SetText(FText::FromString(FString::Format(TEXT("{0}/{1}"), { _blockList.Num(), 100 })));
}

void UGsUIPopupChatOptionNBlock::OnReqChatUserUnblock(UserDBId uid)
{
	FGsNetSendService::SendReqUnblockChatUser(uid);
}

void UGsUIPopupChatOptionNBlock::OnSelectedAllCheck(bool selection)
{
	_checkAll->SetIsSelected(selection);

	for (auto iter : _checkViewChat)
	{
		iter.Value->SetIsSelected(selection);
	}

	for (auto e : _checkViewGroupChat)
	{
		e->SetIsSelected(selection);
	}
}

void UGsUIPopupChatOptionNBlock::OnSelectedTabCheck(EGsOptionChat type, bool selection)
{
	_checkViewChat[type]->SetIsSelected(selection);

	if (selection == false && _checkAll->GetIsSelected())
	{
		_checkAll->SetIsSelected(false);
	}
	else
	{
		int32 fullchatCount = 0;
		for (auto iter : _checkViewChat)
		{
			fullchatCount += iter.Value->GetIsSelected() ? 1 : 0;
		}

		for (auto e : _checkViewGroupChat)
		{
			fullchatCount += e->GetIsSelected() ? 1 : 0;
		}

		_checkAll->SetIsSelected(fullchatCount == _checkViewChat.Num() + _checkViewGroupChat.Num());
	}
}

void UGsUIPopupChatOptionNBlock::OnSelectedGroupCheck(uint64 roomId, bool selection)
{
	if (auto btnIter = _checkViewGroupChat.FindByPredicate([roomId](auto e) { return e->GetIndexInGroup() == roomId; }))
	{
		(*btnIter)->SetIsSelected(selection);

		if (selection == false && _checkAll->GetIsSelected())
		{
			_checkAll->SetIsSelected(false);
		}
		else
		{
			int32 fullchatCount = 0;
			for (auto iter : _checkViewChat)
			{
				fullchatCount += iter.Value->GetIsSelected() ? 1 : 0;
			}

			for (auto e : _checkViewGroupChat)
			{
				fullchatCount += e->GetIsSelected() ? 1 : 0;
			}

			_checkAll->SetIsSelected(fullchatCount == _checkViewChat.Num() + _checkViewGroupChat.Num());
		}
	}
}

void UGsUIPopupChatOptionNBlock::OnTalkBubblePopCheck(bool selection)
{
	_checkTalkBubble->SetIsSelected(selection);
}

void UGsUIPopupChatOptionNBlock::OnEmojiPopCheck(bool selection)
{
	_checkEmoticon->SetIsSelected(selection);
}

void UGsUIPopupChatOptionNBlock::OnInputCancel()
{
	OnTouchedExtraRange();
}