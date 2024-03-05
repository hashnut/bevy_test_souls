#include "GsUIPopupChat.h"
#include "Chat/GsUIListEntryChat.h"
#include "Chat/GsUIListDataChat.h"
#include "Chat/GsUIChatTabUseSubTab.h"
#include "Management/ScopeGlobal/GsUIManager.h"
#include "Management/ScopeGlobal/GsGuildManager.h"
#include "Management/ScopeGame/GsChatManager.h"
#include "Management/ScopeGame/GsPartyManager.h"
#include "Management/ScopeGame/GsGameObjectManager.h"
#include "Message/MessageParam/GsUIMessageParam.h"
#include "UI/UIContent/Helper/GsUIHelper.h"

#include "UI/UIControlLib/ContentWidget/GsSwitcherButton.h"
#include "UI/UIControlLib/ContentWidget/GsButton.h"
#include "UI/UIControlLib/ContentWidget/GsEditableTextBox.h"
#include "UI/UIContent/Popup/GsUIPopupChatUserInfo.h"
#include "UI/UIContent/Popup/GsUIPopupMacro.h"
#include "UI/UIContent/Popup/GsUIPopupChatOptionNBlock.h"
#include "UI/UIContent/Popup/GsUIPopupCommonPay.h"

#include "Components/TextBlock.h"
#include "Components/ListView.h"
#include "Components/ScrollBox.h"
#include "Components/PanelWidget.h"
#include "Components/WidgetSwitcher.h"

#include "GameObject/ObjectClass/GsGameObjectPlayer.h"
#include "Management/GsMessageHolder.h"

#include "Net/GsNetSendService.h"
#include "StringConv.h"
#include "UTIL/GsText.h"
#include "Async.h"
#include "ContentsServer/GsContentsServerHelper.h"
#include "Net/GsNetSendServiceWorld.h"

// 구분하여 패킷을 보내는 챗 타입 - Whisper는 다른 챗 
ENUM_RANGE_BY_VALUES(ChatType, ChatType::WORLD, ChatType::NORMAL, ChatType::PARTY, ChatType::GUILD, ChatType::INVADE, ChatType::INTER, ChatType::ALLY)

// 오픈 가능 tabType
ENUM_RANGE_BY_FIRST_AND_LAST(EGsChatViewType, EGsChatViewType::ALL, EGsChatViewType::GROUP)
ENABLE_ENUM_OPERATORS(EGsChatViewType)

// ChatPopup에서 입력가능한 챗 타입
ENUM_RANGE_BY_VALUES(EGsExChatType, EGsExChatType::WORLD, EGsExChatType::INVADE, EGsExChatType::NORMAL, EGsExChatType::PARTY, 
									EGsExChatType::GUILD, EGsExChatType::WHISPER, EGsExChatType::INTERSERVER, EGsExChatType::GROUP)
ENABLE_ENUM_OPERATORS(EGsExChatType)

void UGsUIPopupChat::BeginDestroy()
{
	if (_editEnterText)
	{
		_editEnterText->OnTextCommitted.RemoveDynamic(this, &UGsUIPopupChat::OnCommitedText);
		_editEnterText->OnTextChanged.RemoveDynamic(this, &UGsUIPopupChat::OnChangedText);
	}

	Super::BeginDestroy();
}

void UGsUIPopupChat::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	// Tab Toggle Group
	TMap<EGsChatViewType, UGsUIChatTabUseSubTab*> btnTabs =
	{ 
		{EGsChatViewType::ALL, _btnTabAll}, {EGsChatViewType::SYSTEM,_btnTabSystem}, {EGsChatViewType::NORMAL, _btnTabNormal},
		{EGsChatViewType::INVASION, _btnTabInvasion}, {EGsChatViewType::SERVER, _btnTabWorld}, {EGsChatViewType::PARTY, _btnTabParty}, 
		{EGsChatViewType::GROUP, _btnTabAlliance}, {EGsChatViewType::GUILD, _btnTabGuild},{EGsChatViewType::WHISPER, _btnTabWhisper}
	};

	for (auto iter : btnTabs)
	{
		iter.Value->SetUseSubTab(false);
		_tabIndexPair.Add(_tabButtonPanel->GetChildIndex(iter.Value), iter.Key);
	}

	_btnTabAlliance->SetUseSubTab(true);
	_tabIndexPair.KeyStableSort([](auto lKey, auto rKey) { return lKey < rKey; });

	for (auto& pair : _tabIndexPair)
	{
		_toggleGroup.AddToggleWidget(Cast<UGsUIChatTabUseSubTab>(_tabButtonPanel->GetChildAt(pair.Key)));
	}

	_curTab = INVALID_CHAT_TAB;

	_toggleGroup.OnSelectChanged.BindUObject(this, &UGsUIPopupChat::OnTabChanged);
	_toggleGroup.SetSelectedIndex(0);

	// ScrollBox Scroll 설정 
	_scroll_Chat->InitializeData();
	_scroll_Chat->OnUserScrolled.AddDynamic(this, &UGsUIPopupChat::OnScrollOffset);

	TWeakObjectPtr<UGsUIPopupChat> thiz = this;

	_scroll_Chat->ClearListItems();
	_scroll_Chat->OnEntryWidgetGenerated().AddLambda([thiz](UUserObjectScrollEntry& entry) {
		if (UGsUIListEntryChat* chatEntry = Cast<UGsUIListEntryChat>(&entry))
		{
			if (!chatEntry->IsValideCallback())
			{
				chatEntry->SetSelectionCallback([thiz](UGsUIListDataChat* data) {
					if(thiz.IsValid()) thiz->OnPopupUserInfo(data);
				});
			}
		}
	});

	_scroll_Chat->OnEntryWidgetReleased().AddLambda([](UUserWidget& entry) {
		if (UGsUIListEntryChat* chatEntry = Cast<UGsUIListEntryChat>(&entry))
		{
			if (chatEntry->IsValideCallback())
			{
				chatEntry->SetSelectionCallback(nullptr);
			}
		}
	});

	// 입력 선택 메뉴
	_inputSelector->OnCreateEntry.BindUObject(this, &UGsUIPopupChat::OnCreateMenuEntry); 
	_inputSelector->OnSelectedEntry.BindUObject(this, &UGsUIPopupChat::OnInputSelected);

	_btnTabAlliance->OnSelectedEntry.BindUObject(this, &UGsUIPopupChat::OnSelectSubTab);

	// 버튼 설정
	_btnMacro->OnClicked.AddDynamic(this, &UGsUIPopupChat::OnPopupMacro);
	_btnMngBlock->OnClicked.AddDynamic(this, &UGsUIPopupChat::OnPopupOptionNUserBlock);
	_btnResetWhispererName->OnClicked.AddDynamic(this, &UGsUIPopupChat::OnClickResetToWhisperName);
	_btnGroupChat->OnClicked.AddDynamic(this, &UGsUIPopupChat::OnPopupGroupChat);

	// 에디트 박스 설정
	FText::FindText(TEXT("ChatText"), TEXT("ChatBlock"), _editHintProhibition);
	FText::FindText(TEXT("ChatText"), TEXT("ChatAble"), _editHint);
	FText::FindText(TEXT("ChatText"), TEXT("Whisper_Idle"), _editHintWhisperTarget);
	FText::FindText(TEXT("ChatText"), TEXT("Whisper_Ready"), _editHintWhisperFormat);

	_editEnterText->SetIsEnabled(true);
	_editEnterText->SetHintText(_editHint);

	_editEnterText->OnTextCommitted.AddDynamic(this, &UGsUIPopupChat::OnCommitedText);
	_editEnterText->OnTextChanged.AddDynamic(this, &UGsUIPopupChat::OnChangedText);

	_backupTextColor = _editEnterText->GetColorAndOpacity();

	_curMaxInput = MAX_CHAR_CHAT_LEN;
}

void UGsUIPopupChat::NativeConstruct()
{
	Super::NativeConstruct();

	FGsMessageHolder* msgHolder = GMessage();

	_chatMessageHandlers.Emplace(msgHolder->GetChat().AddUObject(
		MessageContentChat::ADD, this, &UGsUIPopupChat::OnChatListAdded));

	_chatMessageHandlers.Emplace(msgHolder->GetChat().AddUObject(
		MessageContentChat::ADD_GUILD, this, &UGsUIPopupChat::OnChatListInserted));

	_chatMessageHandlers.Emplace(msgHolder->GetChat().AddUObject(
		MessageContentChat::CHANGE_STATE, this, &UGsUIPopupChat::OnChatStateChanged));

	_chatMessageHandlers.Emplace(msgHolder->GetChat().AddUObject(
		MessageContentChat::INVALIDATE, this, &UGsUIPopupChat::OnChatListRefresh));

	_chatMessageHandlers.Emplace(msgHolder->GetChat().AddUObject(
		MessageContentChat::CHECK_EXIST_BY_NAME, this, &UGsUIPopupChat::OnResultCheckUserExistByName));

	_chatMessageHandlers.Emplace(msgHolder->GetChat().AddUObject(
		MessageContentChat::OPERATE_MESSAGE_UPDATE, this, &UGsUIPopupChat::OnOperateMessageAdded));

	_tradeMessageHandlers.Emplace(msgHolder->GetPersonalTrading().AddUObject(
		MessageContentPersoncalTrading::CLOSE, this, &UGsUIPopupChat::OnCloseTrade));

	_chatMessageHandlers.Emplace(msgHolder->GetChat().AddUObject(
		MessageContentChat::CHATGROUP_LIST, this, &UGsUIPopupChat::OnUpdateChatGroupInfo));

	_chatMessageHandlers.Emplace(msgHolder->GetChat().AddUObject(
		MessageContentChat::CHATGROUP_ADD, this, &UGsUIPopupChat::OnUpdateChatGroupInfo));

	_chatMessageHandlers.Emplace(msgHolder->GetChat().AddUObject(
		MessageContentChat::CHATGROUP_INVALIDATE, this, &UGsUIPopupChat::OnUpdateChatGroupInfo));

	_chatMessageHandlers.Emplace(msgHolder->GetChat().AddUObject(
		MessageContentChat::CHATGROUP_REMOVE, this, &UGsUIPopupChat::OnUpdateChatGroupInfo));

	_chatMessageHandlers.Emplace(msgHolder->GetChat().AddUObject(
		MessageContentChat::CHATGROUP_ROOMNAME_CHANGED, this, &UGsUIPopupChat::OnUpdateChatGroupNameChanged));

	_chatMessageHandlers.Emplace(msgHolder->GetChat().AddUObject(
		MessageContentChat::CHATGROUP_ROOMAUTHGRADE_CHANGED, this, &UGsUIPopupChat::OnUpdateChatGroupAuthGrade));

	_sizeBoxNormal->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	_sizeBoxTrade->SetVisibility(ESlateVisibility::Collapsed);
	_isTradeMode = false;

	InitializeData(nullptr);
}
	
void UGsUIPopupChat::NativeDestruct()
{
	for(auto& e : _chatMessageHandlers)
		GMessage()->GetChat().Remove(e);

	_chatMessageHandlers.Empty();

	_editEnterText->SetColorAndOpacity(_backupTextColor);

	_scroll_Chat->Destruct();

	_sizeBoxNormal->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	_sizeBoxTrade->SetVisibility(ESlateVisibility::Collapsed);
	_isTradeMode = false;

	Super::NativeDestruct();
}

void UGsUIPopupChat::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if(_curTab == INVALID_CHAT_TAB)
		_toggleGroup.SetSelectedIndex(0, true);//OnTabChanged 불리기 전은, 강제로 toggleGroup 처리를 지시해야 한다

	if (_isUpdate && _editEnterText->bIsEnabled)
	{	
		_editEnterText->SetFocus();
		_isUpdate = _editEnterText->HasKeyboardFocus() == false;
	}

#if PLATFORM_WINDOWS || WITH_EDITOR
	bool curFocus = IsInputFocused();
	if (_InputEnableFocus != curFocus)
	{
		OnInputFocusReceived(curFocus);
	}
#endif
}

void UGsUIPopupChat::CloseInternal()
{
	OnExit();

	Super::CloseInternal();
}

void UGsUIPopupChat::InitializeData(UGsUIPopupChat::PopupInitData* initParam)
{
	EGsChatViewType useTab = initParam ? initParam->selectTab : INVALID_CHAT_TAB;
	EGsExChatType useInput = initParam ? initParam->inputChatType : INVALID_EXCHAT_TYPE;

	uint64 useSubTabGroup = initParam && useTab == EGsChatViewType::GROUP ? initParam->selectSubTabRoomId : 0;
	uint64 useGroupInput = initParam && useInput == EGsExChatType::GROUP ? initParam->inputGroupRoomId : 0;

	if (GSChat()->GetCurrentHudChatViewType() == EGsChatViewType::ALL && _curTab != EGsChatViewType::ALL)
	{
		_curTab = EGsChatViewType::ALL;
		_curSubTabRoomId = 0;
		_inputChatType = EGsExChatType::NORMAL;
		_inputGroupRoomId = 0;

		_whisperTarget = FText::GetEmpty();
		_targetWorldId = 0;
	}

	useTab = useTab == INVALID_CHAT_TAB ? _curTab : useTab;
	useInput = useInput == INVALID_EXCHAT_TYPE ? _inputChatType : useInput;
	useSubTabGroup = useSubTabGroup == 0 ? _curSubTabRoomId : useSubTabGroup;
	useGroupInput = useGroupInput == 0 ? _inputGroupRoomId : useGroupInput;

	// history Tab을 먼저 정하고 입력 선택을 한다
	InitTabAndInput(useTab, useSubTabGroup, useInput, useGroupInput, initParam ? initParam->initWhisper : TEXT(""));

	_curTab = INVALID_CHAT_TAB;
	_curSubTabRoomId = 0;

	_inputChatType = INVALID_EXCHAT_TYPE;
	_inputGroupRoomId = useGroupInput;

	_whisperTarget = FText::GetEmpty();
	_targetWorldId = 0;

	GSChat()->SetToWhisperWorldId(0);
	if (useSubTabGroup != 0)
	{
		auto index = _groupRooms.IndexOfByPredicate([&useSubTabGroup](auto e) { return e.Pin()->GetRoomId() == useSubTabGroup; });
		if (index != INDEX_NONE)
		{
			_btnTabAlliance->SetSelectMenuEntry(index);
		}
	}

	if (auto selIndexIter = _tabIndexPair.FindKey(useTab))
	{
		_toggleGroup.SetSelectedIndex(*selIndexIter, true);
		_tabCategory->ScrollWidgetIntoView(_tabButtonPanel->GetChildAt(*selIndexIter), false); 
	}
	
	if (useInput != INVALID_EXCHAT_TYPE)
	{
		auto menuIndex = _dropDownMenu.IndexOfByPredicate([useInput](auto& pair) { return pair.Key == useInput; });
		_inputSelector->SetSelectMenuEntry(menuIndex);
	}

	if (_inputChatType == EGsExChatType::WHISPER && initParam)
	{
		InitReservedWhisperInfo(initParam);
	}

	OnInputFocusReceived(IsInputFocused());
}

void UGsUIPopupChat::OnExit()
{
	_inputSelector->CloseMenuPopup();

	if (UGsUIManager* uiManager = GUI())
	{
		auto weakBlockUI = uiManager->GetWidgetByKey("PopupChatBlockList");
		auto weakEmojiUI = uiManager->GetWidgetByKey("PopupChatEmoji");

		if (weakBlockUI.IsValid())
		{
			weakBlockUI->Close();
		}

		if (weakEmojiUI.IsValid())
		{
			weakEmojiUI->Close();
		}
	}

	_isOpenMacro = false;
	_IsOpenOption = false;
}

void UGsUIPopupChat::OnCloseTrade()
{
	Close();
}

void UGsUIPopupChat::OnClickClose()
{
	if (_isTradeMode)
	{
		GMessage()->GetPersonalTrading().SendMessage(MessageContentPersoncalTrading::CLOSE_POPUP_CHAT);
	}

	Close();
}

void UGsUIPopupChat::OnTabChanged(int32 index)
{
	if (_curTab == _tabIndexPair[index]) return;
	//auto SetMenuMethod = [this](int tabIndex) {
	//
	//	const auto menuData = _inputSelector->GetMenuEntryDataList();
	//	if (const auto data = StaticCastSharedPtr<FGsChatOption>(_inputSelector->GetMenuEntryOptionData(tabIndex).Pin()))
	//	{
	//		if (data->_method == FGsChatOption::Method::MenuHidden)
	//			_inputSelector->SetMenuPanelActive(false);
	//		else if (data->_method == FGsChatOption::Method::PartMenu)
	//		{
	//			_inputSelector->SetMenuPanelActive(true);
	//
	//			TArray<EGsChatViewType> views;
	//			Algo::TransformIf(menuData, views, [](auto& e) {
	//				const auto option = StaticCastSharedPtr<FGsChatOption>(e._data);
	//				return option && option->_method == FGsChatOption::Method::PartMenu;
	//				}, [](auto& e) { return StaticCastSharedPtr<FGsChatOption>(e._data)->_chatTab; });
	//
	//			int menuIndex = 0;
	//			for (auto& e : menuData)
	//			{
	//				if (const auto option = StaticCastSharedPtr<FGsChatOption>(e._data))
	//				{
	//					_inputSelector->SetEnableMenuEntry(menuIndex++, views.Find(option->_chatTab) != INDEX_NONE, true);
	//				}
	//			}
	//		}
	//	}
	//};

	_guildChatPos = 0;
	_preChatTop = 0;
	_scrollProc = 0;

	_isUserScrolled = false;
	_toBottomScrolled = true;

	_btnMacro->SetIsEnabled(true);
	_editEnterText->SetColorAndOpacity(_backupTextColor);

	auto tab = _curTab = _tabIndexPair[index];
	auto subTabRoomId = tab == EGsChatViewType::GROUP ? _curSubTabRoomId : 0;

	// 현재 탭의 챗 리스트
	SetChatListItem(tab, subTabRoomId);

	// 현재 탭에 맞는 입력 메뉴 선택 및, editbox 설정
	SelectInputWithTab(tab, subTabRoomId);

	_editEnterText->SetText(FText::GetEmpty());
	if (_editEnterText->bIsEnabled)
		_editEnterText->SetFocus();

	GSChat()->SetSelectedHudChat(tab);
}

bool UGsUIPopupChat::OnSelectSubTab(int32 index, bool forced)
{
	auto newRoomId = _groupRooms.IsValidIndex(index) ? _groupRooms[index].Pin()->GetRoomId() : 0;
	if (_curSubTabRoomId == newRoomId) // || _curTab != EGsChatViewType::GROUP) 
		return true;

	auto subTabRoomId = _curSubTabRoomId = index < 0 ? 0 : newRoomId;

	SetChatListItem(EGsChatViewType::GROUP, _curSubTabRoomId);
	SelectInputWithTab(_curTab, subTabRoomId);

	_editEnterText->SetText(FText::GetEmpty());
	if (_editEnterText->bIsEnabled)
		_editEnterText->SetFocus();

	GSChat()->SetSelectedHudChat(EGsChatViewType::GROUP, subTabRoomId);
	return true;
}

bool UGsUIPopupChat::OnInputSelected(int32 index, bool forced)
{
	auto groupId = _inputGroupRoomId;
	auto changeInput = _dropDownMenu.IsValidIndex(index)?  _dropDownMenu[index].Key : EGsExChatType::NONE;
	if (const auto data = StaticCastSharedPtr<FGsChatOption>(_inputSelector->GetMenuEntryOptionData(index).Pin()))
	{
		groupId = data->_chatGroupId;
	}

	if (!forced && (_inputChatType == changeInput && _inputGroupRoomId == groupId)) return false;

	_btnResetWhispererName->SetVisibility(ESlateVisibility::Collapsed);
	_editEnterText->SetColorAndOpacity(_backupTextColor);

	GSChat()->SetToWhisperWorldId(0);

	_whisperTarget = FText::GetEmpty();
	_targetWorldId = 0;
	_inputGroupRoomId = 0;

	auto preType = _inputChatType;

	_inputChatType = changeInput;

	if (preType == EGsExChatType::WHISPER)
	{
		CommitWhisperInput(FText::GetEmpty(), FText::GetEmpty());
	}

	_editEnterText->SetText(FText::GetEmpty());
	_editEnterText->SetHintText(GetCurrentHintMessage(_inputChatType == EGsExChatType::NONE ? EGsChatViewType::SYSTEM : _curTab, index));

	if (_editEnterText->bIsEnabled)
	{
		_editEnterText->SetFocus(); //widget focused 되어 있어야 키보드 포커스 작동
	}

	if (_inputChatType == EGsExChatType::WHISPER)
	{
		_btnResetWhispererName->SetVisibility(ESlateVisibility::Visible);
		CommitWhisperInput(FText::GetEmpty(), FText::GetEmpty());
	}
	else if (_inputChatType == EGsExChatType::INVADE && GSChat()->IsEnableFreeChat(EGsExChatType::INVADE))
	{
		_editEnterText->SetHintText(GetCurrentHintMessage(EGsChatViewType::INVASION));
		_editEnterText->SetColorAndOpacity(_inputSelector->GetMenuEntryColor(index));
	}
	else if (_inputChatType == EGsExChatType::GROUP)
	{
		if (const auto data = StaticCastSharedPtr<FGsChatOption>(_inputSelector->GetMenuEntryOptionData(index).Pin()))
		{
			_inputGroupRoomId = data->_chatGroupId;
		}
	}
	
	return _inputChatType != EGsExChatType::NONE;
}

void UGsUIPopupChat::OnChangedText(const FText& OriginalText)
{
	auto str = OriginalText.ToString();
	TArray<TCHAR> arrText = str.GetCharArray();

	uint32 code = 0;
	int32 byteLen = 0;
	int32 txtLen = 0;

	TArray<const TCHAR*> delimArray = GSChat()->worldStartDelim;
	const auto worldStartDelim = delimArray.GetData();

	if (_curMaxInput == MAX_CHAR_NAME_LEN)
	{
		TArray<FString> strArray;
		int32 checkLength = str.ParseIntoArray(strArray, worldStartDelim, delimArray.Num(), true) > 0 ? MAX_CHAR_SERVER_NAME_LEN : MAX_CHAR_NAME_LEN;

		FString outString;
		auto res = FGsTextUtil::CheckedUserNameLengthLimit(str, outString, checkLength);
		if (res == FGsTextUtil::EUserNameValid::InvalidLength)
		{
			_editEnterText->SetText(FText::FromString(outString));
		}
	}
	else
	{
		for (auto srcIter = arrText.CreateConstIterator(); srcIter && *srcIter != TEXT('\0'); srcIter++, txtLen++)
		{
			code = (uint32)(*srcIter);
			if (code >= 0x80) byteLen++;
			byteLen++;

			if (byteLen > _curMaxInput)
				break;
		}

		if (byteLen > _curMaxInput)
		{
			_editEnterText->SetText(FText::FromString(str.Left(txtLen)));
		}
	}
}

void UGsUIPopupChat::OnCommitedText(const FText& inputText, ETextCommit::Type InCommitType)
{
	if (InCommitType == ETextCommit::Type::OnEnter && !inputText.IsEmpty())
	{
		if (!InputDelegate.ExecuteIfBound(inputText))
		{
			if (_curMaxInput != MAX_CHAR_NAME_LEN)
			{
				CommitChatInput(inputText);
			}
		}
	}
	else if (InCommitType == ETextCommit::OnCleared)
	{
		_editEnterText->SetText(FText::GetEmpty());
		_editEnterText->SetFocus();
	}
}

void UGsUIPopupChat::OnScrollOffset(float offset) 
{
	if (_scroll_Chat->GetNumItems() <= 0) return;

	auto top = _scroll_Chat->GetItemIndexByOffset(offset);
	_toBottomScrolled = _scroll_Chat->IsScrollEndPosition();

	if (_scrollProc != 0) return; //lock
	if (_preChatTop > top)
	{
		_isUserScrolled = true;

		if (top <= REQ_GUILDCHAT_OFFSET)
		{
			if (_curTab == EGsChatViewType::ALL || _curTab == EGsChatViewType::GUILD)
			{
				if (CheckEnableInput(EGsExChatType::GUILD))
				{
					if (top > 0 && _guildChatPos == 0)
					{
						_guildChatPos = top;
						_scrollProc |= 0x01;
						GSChat()->SendReqGuildChat();
					}
				}
			}

			top = 0;
		}
	}
	else if (_isUserScrolled)
	{
		_isUserScrolled = false;
	}
	_preChatTop = top;
}

void UGsUIPopupChat::OnCreateMenuEntry(int32 index, struct FMenuData& menuData)
{
	TArray<EGsChatViewType> menuTab = { EGsChatViewType::INVASION, EGsChatViewType::SERVER,  EGsChatViewType::NORMAL,
					EGsChatViewType::GROUP, EGsChatViewType::GUILD, EGsChatViewType::PARTY, EGsChatViewType::WHISPER };

	if (_groupRooms.Num() > 1)
	{
		auto count = _groupRooms.Num() - 1;
		for (int i = 0; i < count; i++)
		{
			menuTab.Insert(EGsChatViewType::GROUP, 3);
		}
	}

	if (UGsUIChatTabUseSubTab* tabButton = _dropDownMenu.IsValidIndex(index) ? _dropDownMenu[index].Value : nullptr)
	{
		// tab과 동일한 색과 이름으로 dropdown menu 를 구성한다
		menuData._menuColor	= tabButton->GetTabBaseColor();
		menuData._menuDisableColor = tabButton->GetTabDisabledColor();
		menuData._menuSelectColor = tabButton->GetTabSelectColor();
		menuData._menuName = tabButton->GetTabTitle();
		menuData._data = MakeShared<FGsChatOption>(menuTab[index]);

		if (_dropDownMenu[index].Key == EGsExChatType::GROUP && _groupRooms.Num() > 0) //서브 탭과 동일한 이름으로 dropdown menu 를 구성한다
		{
			int startKey = _dropDownMenu.IndexOfByPredicate([](auto& e) { return e.Key == EGsExChatType::GROUP; });
			auto groupIndex = index - startKey;

			if (_groupRooms.IsValidIndex(groupIndex))
			{
				StaticCastSharedPtr<FGsChatOption>(menuData._data)->_chatGroupId = _groupRooms[groupIndex].Pin()->GetRoomId();
				menuData._menuName = FText::FromString(_groupRooms[groupIndex].Pin()->GetGroupRoomName());
			}
		}
	}
}

void UGsUIPopupChat::OnClickResetToWhisperName()
{
	if (_inputChatType == EGsExChatType::WHISPER)
	{	
		GSChat()->SetToWhisperWorldId(0);

		_whisperTarget = FText::GetEmpty();
		_targetWorldId = 0;
		_editEnterText->SetIsEnabled(true);
		CommitWhisperInput(_whisperTarget, FText::GetEmpty());
	}
}

void UGsUIPopupChat::OnPopupUserInfo(UGsUIListDataChat* data)
{
	FGsGameDataManager* gameDataMgr = GGameData();
	if (nullptr == gameDataMgr)
		return;

	const FGsNetUserData* myUserData = gameDataMgr->GetUserData();
	if (nullptr == myUserData)
		return;

	if (data)
	{
		//나는 띄우지 않도록
		if (myUserData->mGameId == data->GetChatterUID())
			return;

		if (myUserData->mUserName.Compare(data->GetNickName()) == 0 && 
			myUserData->mHomeWorldId == data->GetHomeWorldId())
			return;

		if (UGsUIManager* uiManager = GUI())
		{
			TWeakObjectPtr<UGsUIPopupChatUserInfo> weakUserUI = Cast<UGsUIPopupChatUserInfo>(uiManager->OpenAndGetWidget("PopupChatUserInfo"));
			if (weakUserUI.IsValid())
			{
				UGsUIPopupChatUserInfo::PopupInitData initData{ 0, data->GetNickName(), data->GetText(), true, EGsUserContentsOwner::USER_CHEAT, data->GetHomeWorldId() };
				weakUserUI->SetData(&initData, false);
				weakUserUI->SetPosition(_panelPopupChat);
			}
		}
	}
}

void UGsUIPopupChat::OnPopupMacro()
{
	if (UGsUIManager* uiManager = GUI())
	{
		auto widget = uiManager->GetWidgetByKey("PopupChatMacro");
		if (widget.IsValid() && _isOpenMacro)
		{
			_isOpenMacro = false;
			widget->Close();
			return;
		}

		if (_inputChatType == EGsExChatType::WHISPER)
		{
			if (!_whisperTarget.IsEmpty())
			{
				auto macro = Cast<UGsUIPopupMacro>(uiManager->OpenAndGetWidget("PopupChatMacro"));
				UGsUIPopupMacro::PopupInitData param{ 0, _curTab, [this](const FText& text) {
					CommitWhisperInput(_whisperTarget, text);
				} };
				macro->InitializeData(&param);
				_isOpenMacro = true;
			}
		}
		else
		{
			auto macro = Cast<UGsUIPopupMacro>(uiManager->OpenAndGetWidget("PopupChatMacro"));
			UGsUIPopupMacro::PopupInitData param{ 0, _curTab, [this](const FText& text) {
				CommitChatInput(text);
			} };
			macro->InitializeData(&param);
			_isOpenMacro = true;
		}
	}
}

void UGsUIPopupChat::OnPopupOptionNUserBlock()
{
	if (UGsUIManager* uiManager = GUI())
	{
		auto widget = uiManager->GetWidgetByKey("PopupChatOption");
		if (widget.IsValid() && _IsOpenOption)
		{
			_IsOpenOption = false;
			widget->Close();
			return;
		}
		uiManager->OpenAndGetWidget("PopupChatOption");
		_IsOpenOption = true;
	}
}

void UGsUIPopupChat::OnPopupGroupChat()
{
	if (UGsUIManager* uiManager = GUI())
	{
		auto widget = uiManager->GetWidgetByKey("PopupGroupChat");
		if (widget.IsValid() && _isOpenGroupChat)
		{
			_isOpenGroupChat = false;
			widget->Close();
			return;
		}
		uiManager->OpenAndGetWidget("PopupGroupChat");
		_isOpenGroupChat = true;
	}
}

void UGsUIPopupChat::CommitChatInput(const FText& msg)
{
	static auto CheckAllSpaceText = [](const FString str)
	{
		TArray<TCHAR> arrText = str.GetCharArray();
		return Algo::AllOf(arrText, [](auto& e) { return e == 0x3000 || e == TEXT(' ') || e == TEXT('\t') || e == TEXT('\0'); });
	};

	static auto IsChatTypeValidRange = [](ChatType val)
	{
		static TArray<ChatType> _range;
		if (_range.Num() <= 0)
		{
			for (auto e : TEnumRange<ChatType>()) _range.Add(e);
		}

		return _range.Find(val) != INDEX_NONE;
	};

	if (!msg.IsEmpty() && !CheckAllSpaceText(msg.ToString()))
	{	
		if (GSChat()->IsNoSpammingChat(_inputChatType))//server chat & interserver chat 동일하게 취급
		{
			ChatType chatType = GSChat()->ExChatTypeToChatType(_inputChatType);
			if (IsChatTypeValidRange(chatType))
			{
				if (_inputChatType == EGsExChatType::INVADE && CheckEnableInput(EGsExChatType::INVADE))
				{
					if (GSChat()->IsEnableFreeChat())
					{
						FGsNetSendService::SendReqChat(ChatType::INVADE, msg.ToString(), false);
					}
					else if (auto popup = Cast<UGsUIPopupCommonPay>(GUI()->OpenAndGetWidget(TEXT("PopupCommonPay")).Get()))
					{
						//침공채팅 이용시 다이아가 소모됩니다
						FText InvadeDiaChatConfirm;
						FText::FindText(TEXT("ChatText"), TEXT("InvadeDiaChatConfirm"), InvadeDiaChatConfirm);

						auto pay = GSChat()->GetPayinvasionChat();
						popup->SetData(InvadeDiaChatConfirm, FGsCostPackage::CreateSimple(pay.type, pay.amount), [msg](bool isOK) {
							if (isOK)
							{
								FGsNetSendService::SendReqChat(ChatType::INVADE, msg.ToString(), true);
							}
							});
					}
				}
				else if (_inputChatType == EGsExChatType::GROUP)
				{
					if (CheckEnableInput(_inputChatType, _inputGroupRoomId))
					{
						FGsNetSendService::SendReqChat(chatType, msg.ToString(), false, _inputGroupRoomId);
					}
				}
				else FGsNetSendService::SendReqChat(chatType, msg.ToString());
			}
		}
	}

	_editEnterText->SetText(FText::GetEmpty());

	if (_inputChatType == EGsExChatType::INVADE && GSChat()->IsEnableFreeChat(EGsExChatType::INVADE))
	{
		FText ChatInvadeText;
		FText::FindText(TEXT("ChatText"), TEXT("ChatInvadeText"), ChatInvadeText);

		FText invadeHintText = FText::Format(ChatInvadeText, GSChat()->GetRemainFreeInvasionChat(), GSChat()->GetInvasionFreeChatCount());
		_editEnterText->SetHintText(invadeHintText);
	}
	else
	{
		_editEnterText->SetHintText(_editHint);
	}
}

void UGsUIPopupChat::CommitWhisperInput(const FText& usernick, const FText& msg)
{
	InputDelegate.Unbind();
	_curMaxInput = MAX_CHAR_CHAT_LEN;

	if (_inputChatType == EGsExChatType::WHISPER)
	{
		auto fnInputTargetName = [this](const FText& inputMsg) {

			FText userNickText = inputMsg;
			FText worldName = FText::GetEmpty();

			auto strTmp = inputMsg.ToString();
			int32 findIndex = 0;

			TArray<const TCHAR*> delimArray = GSChat()->worldStartDelim;
			const auto worldStartDelim = delimArray.GetData();

			TArray<FString> strCheck;
			strTmp.ParseIntoArray(strCheck, worldStartDelim, delimArray.Num(), true);
			if (strCheck.IsValidIndex(0))
			{
				userNickText = FText::FromString(strCheck[0]);
				if (strCheck.IsValidIndex(1))
				{
					worldName = FText::FromString(strCheck[1]);
				}
			}
			_targetWorldId = worldName.IsEmpty() ? 0 : FGsContentsServerHelper::GetWorldIdWithWorldPrefix(worldName);

			if ((_targetWorldId == GGameData()->GetUserData()->mHomeWorldId  || _targetWorldId == 0) &&
				GGameData()->GetUserData()->mUserName.Compare(userNickText.ToString()) == 0)
			{
				//자기 자신에게는 귓속말을 보낼 수 없습니다.
				FText findText;
				FText::FindText(TEXT("ChatText"), TEXT("WhisperTicker1"), findText);
				FGsUIHelper::TraySectionMessageTicker(EGsNotiSection::EXTSYSTEM, findText);

				_targetWorldId = 0;
				_whisperTarget = FText::GetEmpty();
				CommitWhisperInput(FText::GetEmpty(), FText::GetEmpty());

				GSChat()->SetToWhisperWorldId(0);
			}
			else
			{
				_whisperTarget = userNickText;
				_editEnterText->SetIsEnabled(false);

				WorldId homeWorld = GGameData()->GetUserData()->mHomeWorldId;
				WorldId checkWorldId = _targetWorldId == 0 ? homeWorld : _targetWorldId;

				if(checkWorldId != homeWorld)
					 FGsNetSendServiceWorld::SendReqCheckInterUserExistByName(userNickText.ToString(), checkWorldId);
				else FGsNetSendService::SendReqCheckHomeUserExistByName(userNickText.ToString()); //이름으로 유저 존재유무 확인
			}
		};

		auto fnInputWhisperMsg = [this](const FText& inputMsg) {
			CommitWhisperInput(_whisperTarget, inputMsg);
		};

		if (!msg.IsEmpty() && !usernick.IsEmpty())
		{	
			if (GSChat()->IsNoSpammingChat(EGsExChatType::WHISPER))
			{
				WorldId checkWorldId = _targetWorldId == 0 ? GGameData()->GetUserData()->mHomeWorldId : _targetWorldId;
				GSChat()->SetToWhisperWorldId(checkWorldId);

				FGsNetSendService::SendReqWhisper(usernick.ToString(), msg.ToString(), checkWorldId); 
			}

			CommitWhisperInput(_whisperTarget, FText::GetEmpty());
		}
		else if (!usernick.IsEmpty())
		{
			FString hintText = FString::Format(*_editHintWhisperFormat.ToString(), { usernick.ToString() });

			if (_targetWorldId > 0)
			{
				hintText = FString::Format(*_editHintWhisperFormat.ToString(), { FGsTextUtil::CombineUserAndPrefix(usernick, _targetWorldId, true) });
			}

			InputDelegate.BindLambda(fnInputWhisperMsg);

			_editEnterText->SetHintText(FText::FromString(hintText));
			_editEnterText->SetText(FText::GetEmpty());

			_curMaxInput = MAX_CHAR_CHAT_LEN;
			_isUpdate = true;
		}
		else
		{
			GSChat()->SetToWhisperWorldId(0);
			InputDelegate.BindLambda(fnInputTargetName);

			_editEnterText->SetHintText(_editHintWhisperTarget);
			_editEnterText->SetText(FText::GetEmpty());
			_curMaxInput = MAX_CHAR_NAME_LEN;
			_targetWorldId = 0;
			_isUpdate = true;
		}
	}
}

void UGsUIPopupChat::OnInputFocusReceived(bool focus)
{	
#if PLATFORM_WINDOWS || WITH_EDITOR
	_InputEnableFocus = focus;

	if (focus) 
		 EnableEditableInput();
	else DisableEditableInput();
#endif
}

void UGsUIPopupChat::OnChatListAdded(const IGsMessageParam* param)
{
	if (!param) return;
	if (auto chatItemParam = param->Cast<const FGsUIMsgParamChatItem>())
	{
		if (auto chatItem = chatItemParam->_item.Pin())
		{
			if ((_curTab == EGsChatViewType::ALL && chatItemParam->_isVisibleFullChat) || 
				(_curTab == chatItem->_chatViewType && (chatItemParam->_roomNumber == 0 || _curSubTabRoomId == chatItemParam->_roomNumber)))
			{
				if (UGsUIListDataChat* listItem = NewObject<UGsUIListDataChat>())
				{
					_noHistroySelection->SetActiveWidgetIndex(0);
					
					listItem->SetItemData(chatItem);
					_scroll_Chat->AddItem(listItem);

					int32 maxChat = GSChat()->chatTabMax;
					while (_scroll_Chat->GetListItems().Num() > maxChat)
					{
						_scroll_Chat->RemoveItem(_scroll_Chat->GetItemAt(0));
					}
					
					if (_toBottomScrolled)
					{
						_scroll_Chat->SetScrollEnd();
					}

					if (listItem->GetChatType() == EGsExChatType::INVADE && GSChat()->IsEnableFreeChat(EGsExChatType::INVADE))
					{
						FText ChatInvadeText;
						FText::FindText(TEXT("ChatText"), TEXT("ChatInvadeText"), ChatInvadeText);

						FText invadeHintText = FText::Format(ChatInvadeText, GSChat()->GetRemainFreeInvasionChat(), GSChat()->GetInvasionFreeChatCount());
						_editEnterText->SetHintText(invadeHintText);
					}
					else
					{
						_editEnterText->SetColorAndOpacity(_backupTextColor);
						_editEnterText->SetHintText(_editHint);
					}
				}

			}
		}
	}
}

void UGsUIPopupChat::OnChatListInserted(const IGsMessageParam* param) 
{
	const FGsUIMsgParamChat* chatItemParam = param == nullptr ? nullptr : param->Cast<const FGsUIMsgParamChat>();

	if (!chatItemParam || _scrollProc == 0 || chatItemParam->_chatType != intType(EGsExChatType::GUILD)) 
		return;

	EGsExChatType chatType = static_cast<EGsExChatType>(chatItemParam->_chatType);

	if ( _curTab == EGsChatViewType::ALL ||
		(_curTab == EGsChatViewType::GUILD && chatType == EGsExChatType::GUILD))
	{
		auto preItemsCount = _scroll_Chat->GetListItems().Num();

		TArray<TWeakPtr<FGsChatItem>> chatList;
		GSChat()->GetChatDataList(_curTab, chatList);

		auto diff = (chatList.Num() - preItemsCount);

		if (chatList.Num() <= 0 || chatItemParam->_addCount == 0)
		{
			if (chatType == EGsExChatType::GUILD)
			{
				_scrollProc -= 1;
				_guildChatPos = 0;
			}

			if (_scrollProc < 0) _scrollProc = 0;
			return;
		}

		TArray<UGsUIListDataChat*> listItems;
		for (auto weakItem : chatList)
		{
			if (UGsUIListDataChat* listItem = NewObject<UGsUIListDataChat>())
			{
				listItem->SetItemData(weakItem);
				listItems.Add(listItem);
			}
		}

		_noHistroySelection->SetActiveWidgetIndex(listItems.Num() > 0 ? 0 : 1);

		_scroll_Chat->ClearListItems();
		_scroll_Chat->SetListItems(listItems);

		if (chatType == EGsExChatType::GUILD && (_scrollProc & 0x01))
		{
			_scroll_Chat->SetScrollIndex(_guildChatPos + diff + 1);
			_guildChatPos = 0;
			_scrollProc -= 1;
		}

		if (_scrollProc < 0) _scrollProc = 0;
	}
}

void UGsUIPopupChat::OnChatStateChanged(const IGsMessageParam* param)
{
	auto chatItemParam = param == nullptr ? nullptr : param->Cast<const FGsUIMsgParamChat>();
	if (!chatItemParam) return;

	auto useTab = _curTab;
	auto useSubTabGroup = _curSubTabRoomId;
	auto useInput = _inputChatType;
	auto useGroupInput = _inputGroupRoomId;
	InitTabAndInput(useTab, useSubTabGroup, useInput, useGroupInput);

	//_curTab = INVALID_CHAT_TAB;
	//_curSubTabRoomId = 0;
	//_inputChatType = INVALID_EXCHAT_TYPE;
	//_inputGroupRoomId = 0;

	if (auto selIndexIter = _tabIndexPair.FindKey(useTab))
	{
		_toggleGroup.SetSelectedIndex(*selIndexIter, true);
	}

	if (useSubTabGroup != 0)
	{
		auto index = _groupRooms.IndexOfByPredicate([&useSubTabGroup](auto e) { return e.Pin()->GetRoomId() == useSubTabGroup; });
		if (index != INDEX_NONE)
		{
			_btnTabAlliance->SetSelectMenuEntry(index);
		}
	}

	if (useInput != INVALID_EXCHAT_TYPE)
	{
		auto menuIndex = _dropDownMenu.IndexOfByPredicate([useInput](auto& pair) { return pair.Key == useInput; });
		_inputSelector->SetSelectMenuEntry(menuIndex);
	}
}

//차단으로 인한 전체 갱신
void UGsUIPopupChat::OnChatListRefresh(const IGsMessageParam* param)
{
	if (auto chatItemParam = (param == nullptr ? nullptr : param->Cast<const FGsUIMsgParamChat>()))
	{
		if (chatItemParam->_chatType == intType(EGsExChatType::Max) && _curTab != EGsChatViewType::ALL)
			return;
	}

	SetChatListItem(_curTab);
}

void UGsUIPopupChat::OnResultCheckUserExistByName(const IGsMessageParam* param)
{
	if (auto chatParam = (param == nullptr ? nullptr : param->Cast<const FGsUIMsgParamChatUser>()))
	{
		//_targetWorldId = chatParam->_worldId;
		if (!chatParam->_enableChat)
		{
			_whisperTarget = FText::GetEmpty();
			_targetWorldId = 0;
		}

		_editEnterText->SetIsEnabled(true);
		CommitWhisperInput(_whisperTarget, FText::GetEmpty());
	}
}

void UGsUIPopupChat::OnOperateMessageAdded(const IGsMessageParam* param)
{
	if (!param) return;
	if (auto chatItemParam = param->Cast<const FGsUIMsgParamChatItem>())
	{
		if (auto chatItem = chatItemParam->_item.Pin())
		{
			if (UGsUIListDataChat* listItem = NewObject<UGsUIListDataChat>())
			{
				listItem->SetItemData(chatItem);
				_scroll_Chat->AddItem(listItem);

				int32 maxChat = GSChat()->chatTabMax;
				while (_scroll_Chat->GetListItems().Num() > maxChat)
				{
					_scroll_Chat->RemoveItem(_scroll_Chat->GetItemAt(0));
				}

				if (_toBottomScrolled)
				{
					_scroll_Chat->SetScrollEnd();
				}
			}

			_noHistroySelection->SetActiveWidgetIndex(0);
		}
	}
}

void UGsUIPopupChat::OnUpdateChatGroupInfo(const IGsMessageParam* param)
{
	auto chatItemParam = param == nullptr ? nullptr : param->Cast<const FGsUIMsgParamChatGroup>();
	if (!chatItemParam) return;

	auto useTab = _curTab;
	auto useSubTabGroup = _curSubTabRoomId;
	auto useInput = _inputChatType;
	auto useGroupInput = _inputGroupRoomId;
	InitTabAndInput(useTab, useSubTabGroup, useInput, useGroupInput);

	if (auto selIndexIter = _tabIndexPair.FindKey(useTab))
	{
		_toggleGroup.SetSelectedIndex(*selIndexIter, true);
	}

	if (useSubTabGroup != 0)
	{
		auto index = _groupRooms.IndexOfByPredicate([&useSubTabGroup](auto e) { return e.Pin()->GetRoomId() == useSubTabGroup; });
		if (index != INDEX_NONE)
		{
			_btnTabAlliance->SetSelectMenuEntry(index);
		}
	}

	if (useInput != INVALID_EXCHAT_TYPE)
	{
		auto menuIndex = _dropDownMenu.IndexOfByPredicate([useInput](auto& pair) { return pair.Key == useInput; });
		_inputSelector->SetSelectMenuEntry(menuIndex);
	}
}

void UGsUIPopupChat::OnUpdateChatGroupNameChanged(const IGsMessageParam* param)
{
	auto chatItemParam = param == nullptr ? nullptr : param->Cast<const FGsUIMsgParamChatGroup>();
	if (!chatItemParam) return;

	if (auto room = GGuild()->GetGroupChatRoom(chatItemParam->_roomId).Pin())
	{
		_groupRooms.Reset();
		GGuild()->GetGroupChatRooms(_groupRooms);

		if (_btnTabAlliance->GetIsEnabled())
		{
			auto roomId = chatItemParam->_roomId;
			const auto subTabData = _btnTabAlliance->GetMenuEntryDataList();

			auto subTabIndex = subTabData.IndexOfByPredicate([&roomId](const auto& e) {
					if (auto data = StaticCastSharedPtr<FGsGroupChat>(e._data)) return data->_chatGroupId == roomId;
					return false;
				});
			_btnTabAlliance->SetMenuEntryName(subTabIndex, FText::FromString(room->GetGroupRoomName()));

			const auto menuData = _inputSelector->GetMenuEntryDataList();
			auto inputIndex = menuData.IndexOfByPredicate([&roomId](const auto& e) {
					if (auto data = StaticCastSharedPtr<FGsChatOption>(e._data)) return data->_chatTab == EGsChatViewType::GROUP && data->_chatGroupId == roomId;
					return false;
				});

			_inputSelector->SetMenuEntryName(inputIndex, FText::FromString(room->GetGroupRoomName()));
		}
	}
}

void UGsUIPopupChat::OnUpdateChatGroupAuthGrade(const IGsMessageParam* param)
{
	auto useTab = _curTab;
	auto useSubTabGroup = _curSubTabRoomId;
	auto useInput = _inputChatType;
	auto useGroupInput = _inputGroupRoomId;
	InitTabAndInput(useTab, useSubTabGroup, useInput, useGroupInput);

	if (auto selIndexIter = _tabIndexPair.FindKey(useTab))
	{
		_toggleGroup.SetSelectedIndex(*selIndexIter, true);
	}

	if (useSubTabGroup != 0)
	{
		auto index = _groupRooms.IndexOfByPredicate([&useSubTabGroup](auto e) { return e.Pin()->GetRoomId() == useSubTabGroup; });
		if (index != INDEX_NONE)
		{
			_btnTabAlliance->SetSelectMenuEntry(index);
		}
	}

	if (useInput != INVALID_EXCHAT_TYPE)
	{
		auto menuIndex = _dropDownMenu.IndexOfByPredicate([useInput](auto& pair) { return pair.Key == useInput; });
		_inputSelector->SetSelectMenuEntry(menuIndex);
	}
}

void UGsUIPopupChat::SetChatListItem(EGsChatViewType listType, uint64 groupId /*= 0*/)
{
	_toBottomScrolled = true;
	TArray<TWeakPtr<FGsChatItem>> chatList;
	GSChat()->GetChatDataList(listType, chatList, groupId);

	_btnResetWhispererName->SetVisibility(listType == EGsChatViewType::WHISPER ?
		ESlateVisibility::Visible : ESlateVisibility::Collapsed);

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

	_noHistroySelection->SetActiveWidgetIndex(chatList.Num() > 0 ? 0 : 1);
}

void UGsUIPopupChat::SelectInputWithTab(EGsChatViewType tab, uint64 subTabRoomId)
{
	// 현재 탭에서 선택되어야 하는 입력 챗 타입 메뉴 항목 찾기
	const auto menuData = _inputSelector->GetMenuEntryDataList();
	auto inputIndex = menuData.IndexOfByPredicate([&tab, &subTabRoomId](const auto& e) {
		if (auto data = StaticCastSharedPtr<FGsChatOption>(e._data)) return data->_chatTab == tab && data->_chatGroupId == subTabRoomId;
		return false;
		});

	// 선택 되어야 하는 입력 타입이 있을 때 강제 set
	if (inputIndex != INDEX_NONE)
	{
		_inputSelector->SetMenuPanelActive(false); // 메뉴 선택 불가
		_inputSelector->SetVisibility(ESlateVisibility::Visible); // 입력 선택 enable
		_inputSelector->SetSelectMenuEntry(inputIndex);

		_editEnterText->SetIsEnabled(true);
		_editEnterText->SetHintText(GetCurrentHintMessage(tab, inputIndex));

		if (tab == EGsChatViewType::INVASION && GSChat()->IsEnableFreeChat()) // 침공 채팅시 는 입력 컬러 바꿈
		{
			_editEnterText->SetColorAndOpacity(_inputSelector->GetMenuEntryColor(inputIndex));
		}
		else if (tab == EGsChatViewType::GROUP && CheckEnableInput(EGsExChatType::GROUP, subTabRoomId) == false)
		{
			_editEnterText->SetIsEnabled(false);

			OnInputSelected(INDEX_NONE, true);
		}
	}
	else
	{
		if (tab == EGsChatViewType::ALL) // 선택 되어야 하는 입력 타입이 없을 때, 반드시 전체 탭이어야 한다
		{
			// 선택된 메뉴는 없지만, 전체 탭이어서 모든 탭이 선택 가능할 때, Normal Chatting 을 기본 선택
			auto menuIndex = [this](EGsExChatType type)->int32 {
				return _dropDownMenu.IndexOfByPredicate([type](auto& pair) { return pair.Key == type; });
				};

			_inputSelector->SetVisibility(ESlateVisibility::Visible);
			_inputSelector->SetSelectMenuEntry(menuIndex(EGsExChatType::NORMAL));
			_inputSelector->SetMenuPanelActive(true);

			_editEnterText->SetIsEnabled(true);
			_editEnterText->SetHintText(GetCurrentHintMessage(tab, inputIndex));
		}
		else // 현재 탭이 메뉴 선택 불가능 한 탭일 때 (System)
		{
			_inputSelector->SetVisibility(ESlateVisibility::Collapsed);
			_inputSelector->SetSelectMenuEntry(INDEX_NONE);
			_btnMacro->SetIsEnabled(false);
			_editEnterText->SetIsEnabled(false);

			OnInputSelected(INDEX_NONE, true);
			_editEnterText->SetHintText(GetCurrentHintMessage(tab, inputIndex));
		}
	}
}

void UGsUIPopupChat::InitInputDropDownMenu()
{
	using DropDownPair = TPair<EGsExChatType, UGsUIChatTabUseSubTab*>;
	_dropDownMenu = { DropDownPair{EGsExChatType::INVADE, _btnTabInvasion}, DropDownPair{EGsExChatType::WORLD, _btnTabWorld},
					  DropDownPair{EGsExChatType::NORMAL, _btnTabNormal }, DropDownPair{EGsExChatType::GROUP, _btnTabAlliance},
					  DropDownPair{EGsExChatType::GUILD, _btnTabGuild},
					  DropDownPair{EGsExChatType::PARTY, _btnTabParty}, DropDownPair{EGsExChatType::WHISPER, _btnTabWhisper} };

	_groupRooms.Reset();
	int groupChatRoom = GGuild()->GetGroupChatRooms(_groupRooms);

	if (groupChatRoom > 0)
	{
		int insertKey = _dropDownMenu.IndexOfByPredicate([](auto& e) { return e.Key == EGsExChatType::GROUP; });
		for (int index = 0; index < groupChatRoom - 1; index++)
		{
			_dropDownMenu.Insert(DropDownPair{ EGsExChatType::GROUP, _btnTabAlliance }, insertKey);
		}
	}

	_inputSelector->InitializeMenu(_dropDownMenu.Num());

	if (_btnTabAlliance->GetIsEnabled())
	{
		_btnTabAlliance->SetMenuPanelActive(true);
		_btnTabAlliance->InitializeMenu(_groupRooms.Num(), -1);

		for (int index = 0; index < groupChatRoom; index++)
		{
			auto data = _groupRooms[index].Pin();
			_btnTabAlliance->SetMenuEntryName(index, FText::FromString(data->GetGroupRoomName()));
			_btnTabAlliance->SetMenuEntryOptionData(index, MakeShared<FGsGroupChat>(data->GetRoomId()));
		}
	}
}

void UGsUIPopupChat::InitTabAndInput(IN OUT EGsChatViewType& tab, IN OUT uint64& tabGroupId, IN OUT EGsExChatType& input, IN OUT uint64& inputGroupId, IN FString whisperer /*= TEXT("")*/)
{
	InitInputDropDownMenu();

	auto menuIndex = [this](EGsExChatType type)->int32 {
		return _dropDownMenu.IndexOfByPredicate([type](auto& pair) { return pair.Key == type; });
		};

	if (tabGroupId != 0 && GGuild()->GetGroupChatRoom(tabGroupId).IsValid() == false)
	{
		tabGroupId = 0;
	}

	if (inputGroupId != 0 && GGuild()->GetGroupChatRoom(inputGroupId).IsValid() == false)
	{
		inputGroupId = 0;
	}

	_btnGroupChat->SetIsEnabled(GGuild()->IsGuild());

	// chatting Tab  guild, party, invade set
	_btnTabParty->SetIsEnabled(CheckEnableTab(EGsChatViewType::PARTY));
	_btnTabGuild->SetIsEnabled(CheckEnableTab(EGsChatViewType::GUILD));
	_btnTabAlliance->SetIsEnabled(CheckEnableTab(EGsChatViewType::GROUP));
	_btnTabInvasion->SetIsEnabled(CheckEnableTab(EGsChatViewType::INVASION));
	// 2023/12/20 PKT - Battle Arena에서는 '서버'ab 비활성화
	_btnTabWorld->SetIsEnabled(CheckEnableTab(EGsChatViewType::SERVER));

	// DropDown Menu guild, allience, party, invade set
	_inputSelector->SetEnableMenuEntry(menuIndex(EGsExChatType::PARTY), CheckEnableInput(EGsExChatType::PARTY));
	_inputSelector->SetEnableMenuEntry(menuIndex(EGsExChatType::GUILD), CheckEnableInput(EGsExChatType::GUILD));
	_inputSelector->SetEnableMenuEntry(menuIndex(EGsExChatType::INVADE), CheckEnableInput(EGsExChatType::INVADE));
	// 2023/12/20 PKT - Battle Arena에서는 '서버'ab 비활성화
	_inputSelector->SetEnableMenuEntry(menuIndex(EGsExChatType::WORLD), CheckEnableInput(EGsExChatType::WORLD));

	bool enableGroupTab = CheckEnableTab(EGsChatViewType::GROUP);
	auto index = menuIndex(EGsExChatType::GROUP);
	if (enableGroupTab)
	{
		auto count = _groupRooms.Num() + index;
		for (; index < count; index++)
		{
			enableGroupTab = false;
			if (const auto data = StaticCastSharedPtr<FGsChatOption>(_inputSelector->GetMenuEntryOptionData(index).Pin()))
			{
				enableGroupTab = CheckEnableInput(EGsExChatType::GROUP, data->_chatGroupId);
			}
			_inputSelector->SetEnableMenuEntry(index, enableGroupTab);
		}
	}
	else
	{
		_inputSelector->SetEnableMenuEntry(index, false);
	}

	// history Tab을 먼저 정하고 입력 선택을 한다
	if (IsValidRange(tab) && whisperer.IsEmpty() == false)
	{
		tab = tab == EGsChatViewType::WHISPER ? EGsChatViewType::WHISPER : tab;
	}

	tab = IsValidRange(tab) ? tab : INVALID_CHAT_TAB;
	if (!CheckEnableTab(tab)) tab = EGsChatViewType::ALL;

	input = IsValidRange(input) ? input : INVALID_EXCHAT_TYPE;

	if ((input == EGsExChatType::GROUP && inputGroupId == 0) || //없어진 채팅방
		(input != EGsExChatType::GROUP && !CheckEnableInput(input))) //입력할 수 없는 채팅방
	{
		input = EGsExChatType::NORMAL;
	}
}

void UGsUIPopupChat::InitReservedWhisperInfo(PopupInitData* initParam)
{
	_targetWorldId = initParam ? initParam->worldId : 0;
	_whisperTarget = initParam ? FText::FromString(initParam->initWhisper) : FText::GetEmpty();
	_editEnterText->SetIsEnabled(true);

	int32 findIndex = 0;
	auto strTmp = _whisperTarget.ToString();

	TArray<const TCHAR*> delimArray = GSChat()->worldStartDelim;
	const auto worldStartDelim = delimArray.GetData();

	TArray<FString> strArray;
	int32 checkLength = strTmp.ParseIntoArray(strArray, worldStartDelim, delimArray.Num(), true);

	if (checkLength > 0)
	{
		_whisperTarget = FText::FromString(strArray[0]);
		if (_targetWorldId == 0 && strArray.IsValidIndex(1))
		{
				FText worldPrefix = FText::FromString(strArray[1]);
				_targetWorldId = FGsContentsServerHelper::GetWorldIdWithWorldPrefix(worldPrefix);
		}
	}

	GSChat()->SetToWhisperWorldId(_targetWorldId);
	CommitWhisperInput(_whisperTarget, FText::GetEmpty());
}

FText UGsUIPopupChat::GetCurrentHintMessage(EGsChatViewType tab, int tabIndex /*= INDEX_NONE*/)
{	
	if (tab == EGsChatViewType::INVASION && GSChat()->IsEnableFreeChat())
	{
		FText ChatInvadeText;
		FText::FindText(TEXT("ChatText"), TEXT("ChatInvadeText"), ChatInvadeText);

		return FText::Format(ChatInvadeText, GSChat()->GetRemainFreeInvasionChat(), GSChat()->GetInvasionFreeChatCount());
	}
	else if (tab == EGsChatViewType::GROUP && _curSubTabRoomId <= 0)
	{
		//“그룹 채팅방 선택 후 대화를 입력해 주세요.” 
		FText ChatGroupText;
		FText::FindText(TEXT("ChatText"), TEXT("ChatGroupText"), ChatGroupText);
		return ChatGroupText;
	}
	else if (tab == EGsChatViewType::GROUP && _curSubTabRoomId > 0)
	{
		if (CheckEnableInput(EGsExChatType::GROUP, _curSubTabRoomId) == false)
		{
			FText ChatGroupText;
			FText::FindText(TEXT("ChatText"), TEXT("UI_ChatGroup_BanChat"), ChatGroupText);
			return ChatGroupText;
		}
	}
	
	if (tabIndex == INDEX_NONE)
	{
		if (tab != EGsChatViewType::ALL) return _editHintProhibition;
	}

	return _editHint;
}

bool UGsUIPopupChat::CheckEnableInput(EGsExChatType inputType, uint64 groupId /*= 0*/)
{
	if (inputType == EGsExChatType::GROUP)
	{
		return GSChat()->IsEnableGuildGroupChatInRoom(groupId);
	}

	return GSChat()->IsEnableChatForChatType(GSChat()->ExChatTypeToChatType(inputType));
}

bool UGsUIPopupChat::CheckEnableTab(EGsChatViewType tab)
{
	TArray<TWeakPtr<FGsGuildGroupChatRoom>> rooms;
	switch (tab)
	{
	case EGsChatViewType::PARTY:	return GSChat()->IsEnableChatForChatType(ChatType::PARTY);
	case EGsChatViewType::SERVER:   return GSChat()->IsEnableChatForChatType(ChatType::WORLD);
	case EGsChatViewType::INVASION: return GSChat()->IsEnableChatForChatType(ChatType::INVADE);
	case EGsChatViewType::GUILD:	return GSChat()->IsEnableChatForChatType(ChatType::GUILD);
	case EGsChatViewType::GROUP:	return GGuild()->IsGuild() && GGuild()->GetGroupChatRooms(rooms) > 0;
	default: break;
	}
	return true;
}

void UGsUIPopupChat::OnInputCancel()
{
	OnClickClose();
}

void UGsUIPopupChat::SetTradeMode()
{
	_sizeBoxNormal->SetVisibility(ESlateVisibility::Collapsed);
	_sizeBoxTrade->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	_isTradeMode = true;
}

bool UGsUIPopupChat::IsInputFocused()
{
	return _editEnterText && _editEnterText->HasKeyboardFocus();
}
