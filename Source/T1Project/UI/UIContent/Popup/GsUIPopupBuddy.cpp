#include "GsUIPopupBuddy.h"

#include "T1Project.h"
#include "Shared/Shared/SharedInclude/SharedDefine.h"

#include "UI/UIControlLib/Control/GsDynamicPanelSlotHelper.h"
#include "UI/UIControlLib/ContentWidget/GsButton.h"
#include "UI/UIContent/Helper/GsUIHelper.h"
#include "UI/UIControlLib/Control/GsToggleGroup.h"

#include "Blueprint/UserWidget.h"
#include "ScrollBox.h"
#include "VerticalBox.h"
#include "Widget.h"
#include "EditableTextBox.h"
#include "WidgetSwitcher.h"
#include "CanvasPanel.h"
#include "TextBlock.h"
#include "UTIL/GsText.h"
#include "Image.h"
#include "HorizontalBox.h"


#include "Management/ScopeGame/GsBuddyManager.h"
#include "Management/GsMessageHolder.h"
#include "Management/ScopeGlobal/GsUIManager.h"

#include "Message/MessageParam/GsUIMessageParam.h"
#include "Message/GsMessageContents.h"

#include "Buddy/GsBuddyDefine.h"
#include "Buddy/GsBuddyInfo.h"
#include "Buddy/GsUIBuddySlot.h"
#include "GsUIPopupChatUserInfo.h"


void UGsUIPopupBuddy::BeginDestroy()
{
	if (_scrollBoxHelper)
	{
		_scrollBoxHelper->OnRefreshEntry.RemoveDynamic(this, &UGsUIPopupBuddy::OnRefreshEntry);
	}
	_scrollBoxHelper = nullptr;
	_toggleGroupTab.Clear();

	Super::BeginDestroy();
}

void UGsUIPopupBuddy::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	_scrollBoxHelper = NewObject<UGsDynamicPanelSlotHelper>(this);
	_scrollBoxHelper->Initialize(_entryWidgetClass, _verticalBox);
	_scrollBoxHelper->OnRefreshEntry.AddDynamic(this, &UGsUIPopupBuddy::OnRefreshEntry);

	// 탭 메뉴 생성 및 이벤트 세팅
	_toggleGroupTab.AddToggleWidgetByParentPanel(_panelTabRoot);
	_toggleGroupTab.OnSelectChanged.BindUObject(this, &UGsUIPopupBuddy::OnClickTab);

	_toggleGroupTabSub.AddToggleWidgetByParentPanel(_subButtonBox);
	_toggleGroupTabSub.OnSelectChanged.BindUObject(this, &UGsUIPopupBuddy::OnClickTabSub);

	_btnClose->OnClicked.AddDynamic(this, &UGsUIPopupBuddy::OnClickClose);
	_btnAdd->OnClicked.AddDynamic(this, &UGsUIPopupBuddy::OnClickAdd);

	// 친구이름 체크
	_editUserName->OnTextChanged.AddDynamic(this, &UGsUIPopupBuddy::OnTextChangedBuddyName);
	_editUserName->OnTextCommitted.AddDynamic(this, &UGsUIPopupBuddy::OnTextCommittedBuddyName);

	GSLOG(Error, TEXT("----NativeOnInitialized"));
	GSLOG_S(Error);
}

void UGsUIPopupBuddy::NativeConstruct()
{
	Super::NativeConstruct();

	// 첫 탭을 기본으로 지정
	_tabIndex = (int32)EGsBuddyTabType::Buddy;
	_tabSubIndex = (int32)EGsBuddySubTabType::All;
	_toggleGroupTab.SetSelectedIndex(_tabIndex, true);
	_toggleGroupTabSub.SetSelectedIndex(_tabSubIndex, false);

	FGsMessageHolder* msg = GMessage();
	auto& uiHandler = msg->GetUI();
	_updateListHandler = uiHandler.AddUObject(MessageUI::BUDDY_UPDATE_LIST,
		this, &UGsUIPopupBuddy::UpdateList);

	//갱신
	InvalidateAll();
	GSLOG(Error, TEXT("----NativeConstruct"));
	GSLOG_S(Error);
}

void UGsUIPopupBuddy::SetRedDot()
{
	FGsBuddyManager* buddyManager = GSBuddy();
	if (nullptr == buddyManager)
		return;

	bool isVisible = buddyManager->GetBuddyTabRedDot();
	_buddyRedDot->SetVisibility(isVisible ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed);

	isVisible = buddyManager->GetBuddyAcceptTabRedDot();
	_buddyAcceptRedDot->SetVisibility(isVisible ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed);
}

void UGsUIPopupBuddy::NativeDestruct()
{
	FGsMessageHolder* msg = GMessage();
	auto& uiHandler = msg->GetUI();
	uiHandler.Remove(_updateListHandler);

	SaveRedDot();
	
	Super::NativeDestruct();
}

void UGsUIPopupBuddy::InvalidateAllInternal()
{
	Super::InvalidateAllInternal();

	//레드닷
	SetRedDot();

	InvalidateList((EGsBuddyTabType)_tabIndex, (EGsBuddySubTabType)_tabSubIndex);
}

void UGsUIPopupBuddy::InvalidateList(EGsBuddyTabType InIndex, EGsBuddySubTabType InSubIndex)
{
	_verticalBoxApply->SetVisibility((EGsBuddyTabType::Apply == (EGsBuddyTabType)InIndex) ?
		ESlateVisibility::Visible : ESlateVisibility::Collapsed);

	_panelSubTabRoot->SetVisibility((EGsBuddyTabType::Buddy == (EGsBuddyTabType)InIndex) ?
		ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed);

	_buddyList.Empty();	

	GSBuddy()->GetBuddyList(InIndex, InSubIndex, _buddyList);
		
	int32 dataCount = _buddyList.Num();	
	if (0 >= dataCount)
	{
		// 빈페이지 일때 연출
		_switcherNoList->SetVisibility(ESlateVisibility::Visible);
		_switcherNoList->SetActiveWidgetIndex((int)InIndex);
	}
	else
	{
		_switcherNoList->SetVisibility(ESlateVisibility::Collapsed);
	}

	// 스크롤 셋팅
	_scrollBoxHelper->RefreshAll(dataCount);
	
	UpdateTextCount();
}

void UGsUIPopupBuddy::UpdateTextCount()
{
	// (5/10)
	FText findText;
	FText::FindText(TEXT("QuestUIText"), TEXT("countQuest"), findText);

	_textBuddyCount->SetText(FText::Format(findText, _buddyList.Num(), MAX_BUDDY_COUNT));
}

void UGsUIPopupBuddy::OnRefreshEntry(int32 InIndex, UWidget* InEntry)
{
	if (InIndex >= _buddyList.Num())
		return;
	
	if (_buddyList.IsValidIndex(InIndex))
	{
		if (UGsUIBuddySlot* slot = Cast<UGsUIBuddySlot>(InEntry))
		{
			slot->SetData(InIndex, _toggleGroupTab.GetSelectedIndex(), _buddyList[InIndex]);
			slot->OnClick.BindUObject(this, &UGsUIPopupBuddy::OnClickSlot);
		}
	}
}

void UGsUIPopupBuddy::OnClickSlot(int inSlotIndex)
{	
	SetSlotSelect(_slotIndex, false);
	SetSlotSelect(inSlotIndex, true);

	_slotIndex = inSlotIndex;
}

void UGsUIPopupBuddy::SetSlotSelect(int inSlotIndex, bool isSelected)
{
	int listNum = _buddyList.Num();
	if (inSlotIndex < listNum)
	{
		UWidget* entry = _scrollBoxHelper->GetEntry(inSlotIndex);
		if (UGsUIBuddySlot* slot = Cast<UGsUIBuddySlot>(entry))
		{
			slot->SetIsSelected(isSelected);
		}
	}
}

// 닫기 클릭
void UGsUIPopupBuddy::OnClickClose()
{
	Close();
}

// 친구추가 버튼 on
void UGsUIPopupBuddy::OnClickAdd()
{
	if (nullptr != _editUserName)
	{
		FString userName = _editUserName->GetText().ToString();
		if (!userName.IsEmpty())
		{
			// 버튼 클릭 후 이름 삭제
			_editUserName->SetText(FText::FromString(""));

			FString outUserName;
			FGsTextUtil::EUserNameValid eValid = FGsTextUtil::CheckedUserNameLengthLimit(userName, outUserName);
			if (FGsTextUtil::EUserNameValid::InvalidLength == eValid)
			{
				// TEXT: 글자 수 조건이 맞지 않습니다.
				FText textMsg;
				FText::FindText(TEXT("GuildText"), TEXT("Alert_WrongTextLength"), textMsg);
				FGsUIHelper::PopupOK(textMsg);
				return;
			}

			if (FGsTextUtil::EUserNameValid::InvalidFormat == eValid)
			{
				// TEXT: 지원하지 않는 문자가 포함 되어있습니다.
				FText textMsg;
				FText::FindText(TEXT("GuildText"), TEXT("Alert_WrongLanguage"), textMsg);
				FGsUIHelper::PopupOK(textMsg);
				return;
			}

			FGsUIMsgParamBuddy param = FGsUIMsgParamBuddy(0, userName);
			GMessage()->GetUI().SendMessage(MessageUI::BUDDY_ADD, &param);			
		}
	}
}

// 친구, 신청, 수락 tab
void UGsUIPopupBuddy::OnClickTab(int32 InIndex)
{	
	if (_tabIndex != InIndex)
	{
		if (EGsBuddyTabType(_tabIndex) == EGsBuddyTabType::Buddy)
		{
			_buddyRedDot->SetVisibility(ESlateVisibility::Collapsed);
			GSBuddy()->SetBuddyTabRedDot(false);
		}

		if (EGsBuddyTabType(InIndex) == EGsBuddyTabType::Buddy)
		{
			_tabSubIndex = (int32)EGsBuddySubTabType::All;
			_toggleGroupTabSub.SetSelectedIndex(_tabSubIndex, false);
		}
	}

	_tabIndex = InIndex;	
	_selectUserDBId = 0;	// 탭이동 시 선택된 슬롯 없음

	InvalidateList((EGsBuddyTabType)_tabIndex, (EGsBuddySubTabType)_tabSubIndex);	
}

// 전체, 내서버, 다른서버
void UGsUIPopupBuddy::OnClickTabSub(int32 InIndex)
{
	if (_tabSubIndex == InIndex)
	{
		return;
	}

	_tabSubIndex = InIndex;
	_selectUserDBId = 0;	// 탭이동 시 선택된 슬롯 없음
	InvalidateList((EGsBuddyTabType)_tabIndex, (EGsBuddySubTabType)_tabSubIndex);
}

void UGsUIPopupBuddy::SaveRedDot()
{
	// 창 닫힐때 친구탭은 무조건 끄기
	_buddyRedDot->SetVisibility(ESlateVisibility::Collapsed);

	if (FGsBuddyManager* buddyManager = GSBuddy())
	{
		buddyManager->SetBuddyTabRedDot(false);
	}	
}

// 친구이름 체크
void UGsUIPopupBuddy::OnTextChangedBuddyName(const FText & InText)
{
	FString text = InText.ToString();
	FString resultText = text.Replace(TEXT(" "), TEXT(""));
	bool isTextChanged = (false == text.Equals(resultText));

	int32 length = FGsTextUtil::GetStringLengthNameType(resultText);

	// 지정 글자수 이상 입력하지 못하게 막음
	if (MAX_CHAR_SERVER_NAME_LEN < length)
	{
		resultText.RemoveAt(resultText.Len() - 1);
		isTextChanged = true;
	}

	if (isTextChanged)
	{
		_editUserName->SetText(FText::FromString(resultText));
	}
}

void UGsUIPopupBuddy::OnTextCommittedBuddyName(const FText & InText, ETextCommit::Type InCommitType)
{	
}

void UGsUIPopupBuddy::UpdateList(const IGsMessageParam* InParam)
{
	InvalidateAll();
}

void UGsUIPopupBuddy::OnInputCancel()
{
	if (UGsUIManager* uiManager = GUI())
	{
		TWeakObjectPtr<UGsUIWidgetBase> widget = uiManager->GetWidgetByKey(TEXT("PopupChatUserInfo"));
		if (widget.IsValid())
		{
			uiManager->CloseByKeyName(TEXT("PopupChatUserInfo"));
			return;
		}
	}

	OnClickClose();
}