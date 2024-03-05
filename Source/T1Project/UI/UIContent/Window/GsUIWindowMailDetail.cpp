// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUIWindowMailDetail.h"

#include "Shared/Client/SharedEnums/SharedMailEnum.h"
#include "Shared/Shared/SharedInclude/SharedDefine.h"

#include "Management/ScopeGame/GsMailManager.h"

#include "UI/UIControlLib/ContentWidget/GsButton.h"
#include "UI/UIControlLib/Control/GsDynamicIconSlotHelper.h"
#include "UI/UIControlLib/ContentWidget/GsWrapBoxIconSelector.h"
#include "UI/UIContent/Popup/GsUIPopupItemDetailMain.h"
#include "UI/UIContent/Common/Icon/GsUIIconItem.h"
#include "UI/UIContent/Common/Icon/GsUIIconItemReward.h"

#include "Mail/GsMailInfo.h"
#include "Item/GsItem.h"

#include "TextBlock.h"
#include "WidgetSwitcher.h"

#include "Message/MessageParam/GsItemContentsMessageParam.h"
#include "Message/GsMessageContents.h"
#include "Management/GsMessageHolder.h"
#include "GameFlow/GameContents/GsContentsMode.h"


void UGsUIWindowMailDetail::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	// 버튼 세팅
	_btnClose->OnClicked.AddDynamic(this, &UGsUIWindowMailDetail::OnClickCloseAllStack);

	_btnReceive->OnClicked.AddDynamic(this, &UGsUIWindowMailDetail::OnClickReceive);
	_btnDelete->OnClicked.AddDynamic(this, &UGsUIWindowMailDetail::OnClickDelete);

	// 첨부 아이템 슬롯 관리
	_listIconSlotHelper = NewObject<UGsDynamicIconSlotHelper>(this);
	_listIconSlotHelper->Initialize(_iconSelector);
	_listIconSlotHelper->OnCreateIcon.AddDynamic(this, &UGsUIWindowMailDetail::OnCreateIcon);
	_listIconSlotHelper->OnRefreshIcon.AddDynamic(this, &UGsUIWindowMailDetail::OnRefreshIcon);
}

void UGsUIWindowMailDetail::NativeConstruct()
{
	Super::NativeConstruct();

	if(0 < _mailDBId)
		SetData(_mailDBId);
}

void UGsUIWindowMailDetail::NativeDestruct()
{
	Super::NativeDestruct();
}

bool UGsUIWindowMailDetail::SyncContentState()
{
	if (false == IsSameContentState(FGsContentsMode::InGame::ContentsMail))
	{
		GMessage()->GetContents().SendMessage(MessageContents::ACTIVE_WINDOW_MAIL);
		return true;
	}

	return false;
}

void UGsUIWindowMailDetail::SetData(uint64 inMailDBId)
{
	FGsMailManager* mailManager = GSMail();
	if (nullptr == mailManager)
		return;

	TSharedPtr<FGsMailInfo> mailInfo;
	mailManager->GetMailData(inMailDBId, mailInfo);

	if (!mailInfo.IsValid())
		return;

	_mailDBId = inMailDBId;
	_listIconSlotHelper->Clear();
	_iconSelector->RemoveAllChildren();

	// 읽은 상태 저장
	mailManager->SendReqMailRead(mailInfo->GetMailBoxType(), inMailDBId);
	// 탭에 존재는 모두 우편갯수
	int count = mailManager->GetMailBoxTypeDataListCount(mailInfo->GetMailBoxType());

	// (5/10) 우편 갯수
	FText findText;
	FText::FindText(TEXT("UICommonText"), TEXT("HUDHPValue"), findText);
	_textMailCount->SetText(FText::Format(findText, count, MAX_LIMITED_QUANTITY_MAIL_LOAD_COUNT));
	
	_mailInfo = mailInfo;
	_btnSwicher->SetActiveWidgetIndex(_mailInfo->GetIsRewards() ? 0 : 1);
	_textBlockTitle->SetText(mailInfo->GetTextTitle());
	_textContents->SetText(_mailInfo->GetTextContent());

	InvalidateIconList();
}

// 받기 버튼 눌렀을 때
void UGsUIWindowMailDetail::OnClickReceive()
{
	GSMail()->SendMailReceive(_mailInfo->GetMailBoxType(), _mailInfo->GetMailDBId());	
	Close();
}

// 삭제 버튼 눌렀을 때
void UGsUIWindowMailDetail::OnClickDelete()
{
	GSMail()->SendMailDelete(_mailInfo->GetMailBoxType(), _mailInfo->GetMailDBId());
	Close();
}

void UGsUIWindowMailDetail::Close(bool bImmediate, bool bRecoverPrevStack)
{
	_mailDBId = 0;
	Super::Close();
}

void UGsUIWindowMailDetail::InvalidateIconList()
{
	// 보상 아이템 아이콘 리스트 표시
	_listIconSlotHelper->RefreshAll(_mailInfo->GetMailAttachmentList().Num());
}

void UGsUIWindowMailDetail::OnRefreshIcon(int32 InIndex, UGsUIIconBase * InIcon)
{
	UGsUIIconItemReward* itemIcon = Cast<UGsUIIconItemReward>(InIcon);
	if (itemIcon)
	{
		itemIcon->SetEnableDetailPopup(true);
		itemIcon->SetCenterPopup(true);
		itemIcon->SetItemNameToolTipFlag(true);

		const TArray<TSharedPtr<FGsMailAttachment>>& mailAttachmentList = _mailInfo->GetMailAttachmentList();
		if (InIndex < mailAttachmentList.Num())
		{
			TSharedPtr<FGsMailAttachment> mailAttachment = mailAttachmentList[InIndex];
			if (mailAttachment.IsValid())
			{
				switch (mailAttachment->GetType())
				{
				case MailAttachmentType::CURRENCY:
					itemIcon->SetItemNameToolTipFlag(true);
					itemIcon->SetCurrency((CurrencyType)mailAttachment->GetId(), mailAttachment->GetAmount());
					break;
				case MailAttachmentType::ITEM:
					itemIcon->SetItemNameToolTipFlag(true);
					itemIcon->SetNotOwnItem(mailAttachment->GetId(), mailAttachment->GetAmount(),
						mailAttachment->GetLevel(), mailAttachment->GetEnchantShield(), mailAttachment->GetEnchantBonusIndex());
					break;
				default:
					break;
				}
			}
		}
	}
}

void UGsUIWindowMailDetail::OnCreateIcon(UGsUIIconBase* InIcon)
{
	UGsUIIconItemReward* itemIcon = Cast<UGsUIIconItemReward>(InIcon);
	if (itemIcon)
	{
		itemIcon->OnFinishedLongPressEvent.AddUObject(this, &UGsUIWindowMailDetail::OnLongPressIcon);
	}
}

void UGsUIWindowMailDetail::OnLongPressIcon(UGsUIIconItem& InIcon)
{
	UGsUIIconItem::DefaultItemDetailOpener(InIcon, true);
}
