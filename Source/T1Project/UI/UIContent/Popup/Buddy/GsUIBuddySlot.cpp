#include "GsUIBuddySlot.h"

#include "T1Project.h"

#include "UserWidget.h"
#include "Image.h"
#include "TextBlock.h"
#include "SizeBox.h"

#include "UI/UIControlLib/ContentWidget/GsButton.h"
#include "UI/UIControlLib/Control/GsToggleGroupEntry.h"
#include "UI/UIContent/Helper/GsTimeStringHelper.h"
#include "UI/UIContent/Helper/GsUIColorHelper.h"
#include "UI/UIContent/Define/EGsUIColorType.h"
#include "UI/UIContent/Popup/GsUIPopupChatUserInfo.h"

#include "Buddy/GsBuddyInfo.h"
#include "Buddy/GsBuddyDefine.h"

#include "Management/GsMessageHolder.h"
#include "Message/MessageParam/GsUIMessageParam.h"
#include "Message/GsMessageUI.h"

#include "Management/ScopeGlobal/GsUIManager.h"
#include "UTIL/GsText.h"


void UGsUIBuddySlot::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	_btnSlot->OnClicked.AddDynamic(this, &UGsUIBuddySlot::OnClickSlot);
	_btnUserInfo->OnClicked.AddDynamic(this, &UGsUIBuddySlot::OnClickUserInfo);
	_btnCancel->OnClicked.AddDynamic(this, &UGsUIBuddySlot::OnClickCancel);
	_btnAccept->OnClicked.AddDynamic(this, &UGsUIBuddySlot::OnClickAccept);	
}

// 데이터 세팅
void UGsUIBuddySlot::SetData(int inSlotIndex, int inTabIndex, TSharedPtr<FGsBuddyInfo>& inBuddyInfo)
{
	if (!inBuddyInfo.IsValid())
		return;
	
	_slotIndex = inSlotIndex;
	_tabIndex = inTabIndex;
	_buddyInfo = inBuddyInfo;

	_textUserName->SetText(FText::FromString(inBuddyInfo->GetBuddyUserNamePrefix()));
	
	SetOnlineStatus();
	SetIsSelected(false);	
}

void UGsUIBuddySlot::SetOnlineStatus()
{
	if (0 != _tabIndex)
		return;

	switch (_buddyInfo->GetOnLineStateType())
	{
	case EBuddyOnLineStateType::ONLINE:
		{
			// 접속중
			FText findText;
			FText::FindText(TEXT("FriendText"), TEXT("FriendOnline"), findText);
			_textLoginStatus->SetText(findText);
			_textLoginStatus->SetColorAndOpacity(FGsUIColorHelper::GetColor(EGsUIColorType::BUDDY_ONLINE_COLOR));
		}
		break;
	case EBuddyOnLineStateType::OFFLINE:
		{
			// 오프라인 시간
			FString outString;
			FGsTimeStringHelper::GetElapsedTime(_buddyInfo->GetElapsedTime(), outString);
			_textLoginStatus->SetText(FText::FromString(outString));
			_textLoginStatus->SetColorAndOpacity(FGsUIColorHelper::GetColor(EGsUIColorType::BUDDY_OFFLINE_COLOR));
		}
		break;
	case EBuddyOnLineStateType::UNKNOWN:
		{
			// 알수없음
			FText findText;
			FText::FindText(TEXT("FriendText"), TEXT("FriendInterServer"), findText);
			_textLoginStatus->SetText(findText);
			_textLoginStatus->SetColorAndOpacity(FGsUIColorHelper::GetColor(EGsUIColorType::BUDDY_OFFLINE_COLOR));
		}
		break;
	case EBuddyOnLineStateType::MAX:
		break;
	default:
		break;
	}
}

void UGsUIBuddySlot::SetIsSelected(bool bInIsSelected)
{
	_bIsSelected = bInIsSelected;
	if (_imgSelected == nullptr)
	{
		return;
	}

	_imgSelected->SetVisibility((bInIsSelected)? ESlateVisibility::SelfHitTestInvisible: ESlateVisibility::Hidden);

	// 슬롯 아이템 정렬
	SetSlotItem();
}

bool UGsUIBuddySlot::GetIsSelected() const
{
	return _bIsSelected;
}

// 유저 컨텍스트 메뉴
void UGsUIBuddySlot::OnClickUserInfo()
{
	if (_buddyInfo.IsValid())
	{
		TWeakObjectPtr<UGsUIPopupChatUserInfo> contextPopup = Cast<UGsUIPopupChatUserInfo>(GUI()->OpenAndGetWidget(TEXT("PopupChatUserInfo")));
		if (contextPopup.IsValid())
		{
			UGsUIPopupChatUserInfo::PopupInitData initData{ _buddyInfo->GetUserDBId(), _buddyInfo->GetBuddyUserName(), TEXT(""), true, EGsUserContentsOwner::BUDDY_MEMBER, _buddyInfo->GetWorldId() };
			contextPopup->SetData(&initData, false);
		}
	}
}

// X 버튼 클릭
void UGsUIBuddySlot::OnClickCancel()
{
	FGsUIMsgParamBuddy param = FGsUIMsgParamBuddy(_buddyInfo->GetUserDBId(), _buddyInfo->GetBuddyUserNamePrefix());
	
	switch ((EGsBuddyTabType)_tabIndex)
	{
	case EGsBuddyTabType::Buddy:	
		GMessage()->GetUI().SendMessage(MessageUI::BUDDY_DELETE, &param);
		break;
	case EGsBuddyTabType::Apply:
		GMessage()->GetUI().SendMessage(MessageUI::BUDDY_ADD_CANCEL, &param);
		break;
	case EGsBuddyTabType::Accpept:
		GMessage()->GetUI().SendMessage(MessageUI::BUDDY_INVITE_REFUSE, &param);
		break;
	default:
		break;
	}
}

// 수락
void UGsUIBuddySlot::OnClickAccept()
{
	FGsUIMsgParamBuddy param = FGsUIMsgParamBuddy(_buddyInfo->GetUserDBId(), _buddyInfo->GetBuddyUserNamePrefix());
	GMessage()->GetUI().SendMessage(MessageUI::BUDDY_INVITE_ACCEPT, &param);

#if WITH_EDITOR
	GSLOG(Log, TEXT("[Buddy] UGsUIBuddySlot ClickAccept"));
#endif
}

// 아이템 클릭
void UGsUIBuddySlot::OnClickSlot()
{
	if (0 != _tabIndex)
		return;

	//SetIsSelected(!_bIsSelected);
	OnClick.ExecuteIfBound(_slotIndex);
}

void UGsUIBuddySlot::SetSlotItem()
{
	AllItemHide();

	if (_bIsSelected)
	{
		switch ((EGsBuddyTabType)_tabIndex)
		{
		case EGsBuddyTabType::Buddy:
			_btnUserInfo->SetVisibility(ESlateVisibility::Visible);
			_btnCancel->SetVisibility(ESlateVisibility::Visible);
			break;
		case EGsBuddyTabType::Apply:
			break;
		case EGsBuddyTabType::Accpept:
			break;
		default:
			break;
		}
	}
	else
	{
		switch ((EGsBuddyTabType)_tabIndex)
		{
		case EGsBuddyTabType::Buddy:
			_sizeBoxTime->SetVisibility(ESlateVisibility::Visible);
			break;
		case EGsBuddyTabType::Apply:
			_btnCancel->SetVisibility(ESlateVisibility::Visible);
			break;
		case EGsBuddyTabType::Accpept:
			_btnAccept->SetVisibility(ESlateVisibility::Visible);
			_btnCancel->SetVisibility(ESlateVisibility::Visible);
			break;
		default:
			break;
		}
	}
}

void UGsUIBuddySlot::AllItemHide()
{
	_btnUserInfo->SetVisibility(ESlateVisibility::Collapsed);
	_btnCancel->SetVisibility(ESlateVisibility::Collapsed);
	_btnAccept->SetVisibility(ESlateVisibility::Collapsed);
	_sizeBoxTime->SetVisibility(ESlateVisibility::Collapsed);
}