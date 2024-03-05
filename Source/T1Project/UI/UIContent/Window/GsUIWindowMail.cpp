// Fill out your copyright notice in the Description page of Project Settings.
#include "GsUIWindowMail.h"

#include "Shared/Client/SharedEnums/SharedMailEnum.h"
#include "Management/ScopeGame/GsMailManager.h"
#include "Management/GsMessageHolder.h"
#include "Message/GsMessageContents.h"

#include "Mail/GsMailDefine.h"
#include "Mail/GsUIMailSlot.h"

#include "ScrollBox.h"
#include "TextBlock.h"

#include "Image.h"
#include "CanvasPanel.h"

#include "UI/UIControlLib/Control/GsDynamicPanelSlotHelper.h"
#include "UI/UIControlLib/Control/GsToggleGroup.h"
#include "UI/UIControlLib/ContentWidget/GsButton.h"
#include "UI/UIControlLib/ContentWidget/GsSwitcherButton.h"


void UGsUIWindowMail::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	_helpContentsKey = EGsHelpContents::Mail;

	// 스크롤 세팅
	_scrollBoxHelper = NewObject<UGsDynamicPanelSlotHelper>(this);
	_scrollBoxHelper->Initialize(_entryWidgetClass, _scrollBox);
	_scrollBoxHelper->OnRefreshEntry.AddDynamic(this, &UGsUIWindowMail::OnRefreshEntry);

	// 탭 메뉴 생성 및 이벤트 세팅
	_toggleGroupTab.AddToggleWidget(_switchBtnUser);
	_toggleGroupTab.AddToggleWidget(_switchBtnGuild);
	_toggleGroupTab.AddToggleWidget(_switchBtnAccount);
	_toggleGroupTab.OnSelectChanged.BindUObject(this, &UGsUIWindowMail::OnClickTab);

	// 버튼 세팅
	_btnClose->OnClicked.AddDynamic(this, &UGsUIWindowMail::OnClickCloseAllStack);
	_btnReceiveAll->OnClicked.AddDynamic(this, &UGsUIWindowMail::OnClickReceiveAll);
	_btnDeleteAll->OnClicked.AddDynamic(this, &UGsUIWindowMail::OnClickDeleteAll);
	_btnPagePrev->OnClicked.AddDynamic(this, &UGsUIWindowMail::OnClickPagePrev);
	_btnPageNext->OnClicked.AddDynamic(this, &UGsUIWindowMail::OnClicPageNext);
}

void UGsUIWindowMail::NativeConstruct()
{
	Super::NativeConstruct();
}

void UGsUIWindowMail::NativeDestruct()
{
	// 스크롤 옵셋 저장
	_scrollOffset = _scrollBox->GetScrollOffset();

	//갱신
	InvalidateAll();
	
	Super::NativeDestruct();
}

// 닫기
void UGsUIWindowMail::CloseInternal()
{
	Super::CloseInternal();
}

bool UGsUIWindowMail::SyncContentState()
{
	if (false == IsSameContentState(FGsContentsMode::InGame::ContentsMail))
	{
		GMessage()->GetContents().SendMessage(MessageContents::ACTIVE_WINDOW_MAIL);
		return true;
	}

	return false;
}

void UGsUIWindowMail::BeginDestroy()
{
	if (_scrollBoxHelper)
	{
		_scrollBoxHelper->OnRefreshEntry.RemoveDynamic(this, &UGsUIWindowMail::OnRefreshEntry);
	}
	_scrollBoxHelper = nullptr;

	_toggleGroupTab.Clear();

	Super::BeginDestroy();
}

void UGsUIWindowMail::InvalidateAllInternal()
{
	Super::InvalidateAllInternal();

	InvalidateList(_selectedTabIndex);
}

void UGsUIWindowMail::InvalidateList(int inIndex)
{
	_mailList.Empty();
	
	GSMail()->GetMailList((MailBoxType)inIndex, _mailList);

	int mailCount = _mailList.Num();

	// 메일이 없을 때 우편함이 비어있습니다.
	_canvasPanelNoList->SetVisibility((0 >= mailCount) ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);

	// [B1] | ejrrb10 | 우편 취합 이슈(C2URWQ-5054) 2번 수정 (while 문 + FMath::Clamp 추가) 
	while (0 < _pageIndex)
	{
		if (mailCount <= _pageIndex * MAX_PAGE_SLOT_COUNT)
		{
			// 해당 페이지 보다 메일이 적게 있다
			// 메일갯수에 맞춰 페이지를 줄여준다.
			--_pageIndex;
		}
		else
		{
			break;
		}
	}

	_pageIndex = FMath::Clamp(_pageIndex, 0, mailCount / MAX_PAGE_SLOT_COUNT);

	if (mailCount > ((_pageIndex + 1) * MAX_PAGE_SLOT_COUNT))
		mailCount = MAX_PAGE_SLOT_COUNT;
	else
		mailCount = mailCount - ((_pageIndex)* MAX_PAGE_SLOT_COUNT);

	_scrollBoxHelper->RefreshAll(mailCount);
	_scrollBox->SetScrollOffset(_scrollOffset);

	// 메일 갯수
	UpdateTextCount();

	// 페이지 숫자
	UpdatePageNum();

	// 레드닷 갱신
	UpdateRedDot();
}

void UGsUIWindowMail::InIt()
{
	// 시작은 개인부터
	_toggleGroupTab.SetSelectedIndex(0, true);
	_selectedTabIndex = 0;
	_pageIndex = 0;
}

// 탭 클릭했을때
void UGsUIWindowMail::OnClickTab(int32 inIndex)
{
	if (inIndex == _selectedTabIndex)
		return;

	_selectedTabIndex = inIndex;
	_toggleGroupTab.SetSelectedIndex(inIndex, true);
	_pageIndex = 0;
	
	InvalidateList(_selectedTabIndex);
}

// 모두 받기 눌렀을 때
void UGsUIWindowMail::OnClickReceiveAll()
{
	GSMail()->SendMailReceiveAll((MailBoxType)_selectedTabIndex);
}

// 모두 삭제 눌렀을 때
void UGsUIWindowMail::OnClickDeleteAll()
{
	GSMail()->SendMailDeleteAll((MailBoxType)_selectedTabIndex);
}

// 페이지 앞
void UGsUIWindowMail::OnClickPagePrev()
{
	if (_pageIndex <= 0)
		return;

	--_pageIndex;
	InvalidateList(_selectedTabIndex);
}

// 페이지 뒤
void UGsUIWindowMail::OnClicPageNext()
{
	int mailCount = _mailList.Num();

	if ((_pageIndex + 1) * MAX_PAGE_SLOT_COUNT >= mailCount)
		return;
		
	++_pageIndex;
	InvalidateList(_selectedTabIndex);
}

// 슬롯 갱신
void UGsUIWindowMail::OnRefreshEntry(int32 inIndex, UWidget* InEntry)
{
	int mailNum = _mailList.Num();
	if (inIndex >= mailNum)
		return;

	int curruntIndex = inIndex + ((_pageIndex)* MAX_PAGE_SLOT_COUNT);
	if (curruntIndex >= mailNum)
		return;

	if (_mailList.IsValidIndex(inIndex))
	{
		if (UGsUIMailSlot* slot = Cast<UGsUIMailSlot>(InEntry))
		{
			slot->SetData(_mailList[curruntIndex]);
		}
	}
}

// 탭별 우편 갯수 ? / ?
void UGsUIWindowMail::UpdateTextCount()
{
	// (5/10)
	FText findText;
	FText::FindText(TEXT("UICommonText"), TEXT("HUDHPValue"), findText);

	int pageMailCount = 0;
	int mailListCount = _mailList.Num();

	int pageCount = mailListCount / MAX_PAGE_SLOT_COUNT;

	if (pageCount > _pageIndex)
		pageMailCount = MAX_PAGE_SLOT_COUNT;
	else
		pageMailCount = mailListCount % MAX_PAGE_SLOT_COUNT;
	
	_textMailCount->SetText(FText::Format(findText, pageMailCount, MAX_PAGE_SLOT_COUNT));
}

// 페이지 숫자
void UGsUIWindowMail::UpdatePageNum()
{
	// (5/10)
	FText findText;
	FText::FindText(TEXT("UICommonText"), TEXT("HUDHPValue"), findText);

	int mailListCount = _mailList.Num();

	int pageMax = mailListCount / MAX_PAGE_SLOT_COUNT;
	if (pageMax > 0 && mailListCount % MAX_PAGE_SLOT_COUNT == 0)
		--pageMax;

	_textPageNum->SetText(FText::Format(findText, _pageIndex + 1, pageMax + 1));
}

// 레드닷 갱신
void UGsUIWindowMail::UpdateRedDot()
{
	TArray<class UImage*> imgRedDotList;
	imgRedDotList.Add(_userTabRedDot);
	imgRedDotList.Add(_guildTabRedDot);
	imgRedDotList.Add(_accountTabRedDot);

	for (int i = 0; i < (int)MailBoxType::MAX; ++i)
	{
		bool outIsRedDot = false;
		GSMail()->IsRedDot((MailBoxType)i, outIsRedDot);

		if (i < imgRedDotList.Num())
		{
			imgRedDotList[i]->SetVisibility((outIsRedDot) ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed);
		}
	}

	imgRedDotList.Empty();
}