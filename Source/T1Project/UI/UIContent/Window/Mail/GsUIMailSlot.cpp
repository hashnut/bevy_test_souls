#include "GsUIMailSlot.h"

#include "Shared/Client/SharedEnums/SharedMailEnum.h"
#include "Mail/GsMailInfo.h"

#include "Engine/World.h"
#include "Public/TimerManager.h"

#include "UserWidget.h"
#include "TextBlock.h"
#include "WidgetSwitcher.h"
#include "Image.h"

#include "UI/UIControlLib/ContentWidget/GsButton.h"
#include "UI/UIContent/Helper/GsTimeStringHelper.h"
#include "UI/UIControlLib/ContentWidget/GsWrapBoxIconSelector.h"
#include "UI/UIControlLib/Control/GsDynamicIconSlotHelper.h"
#include "UI/UIContent/Common/Icon/GsUIIconItemReward.h"
#include "UI/UIContent/Common/Icon/GsUIIconBase.h"
#include "UI/UIContent/Helper/GsUIStringHelper.h"
#include "UI/UIContent/Common/Icon/GsUIIconItem.h"

#include "Message/GsMessageContents.h"
#include "Message/MessageParam/GsUIMessageParam.h"
#include "Message/MessageParam/GsItemContentsMessageParam.h"
#include "Management/GsMessageHolder.h"
#include "Management/ScopeGame/GsMailManager.h"

#include "Data/GsDataContainManager.h"
#include "Item/GsItem.h"


void UGsUIMailSlot::BeginDestroy()
{
	_listIconSlotHelper = nullptr;	

	Super::BeginDestroy();
}

void UGsUIMailSlot::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	_btnSlot->OnClicked.AddDynamic(this, &UGsUIMailSlot::OnClickSlot);
	_btnRecvDel->OnClicked.AddDynamic(this, &UGsUIMailSlot::OnClickRecvDel);

	// 첨부 아이템 슬롯 관리
	if (nullptr == _listIconSlotHelper)
	{
		_listIconSlotHelper = NewObject<UGsDynamicIconSlotHelper>(this);
		_listIconSlotHelper->Initialize(_iconSelector);
		_listIconSlotHelper->OnCreateIcon.AddDynamic(this, &UGsUIMailSlot::OnCreateIcon);
		_listIconSlotHelper->OnRefreshIcon.AddDynamic(this, &UGsUIMailSlot::OnRefreshIconSlot);
	}

	_textTitle->SetText(FText::FromString("."));
}

void UGsUIMailSlot::NativeConstruct()
{
	Super::NativeConstruct();
}

void UGsUIMailSlot::NativeDestruct()
{
	_listIconSlotHelper->Clear();
	_iconSelector->RemoveAllChildren();

	OnHide();

	Super::NativeDestruct();
}

void UGsUIMailSlot::OnHide()
{
	if (_timerHandle.IsValid())
	{
		if (UWorld* world = GetWorld())
		{
			world->GetTimerManager().ClearTimer(_timerHandle);
		}
	}
}

// 데이터 세팅
void UGsUIMailSlot::SetData(TSharedPtr<FGsMailInfo>& inMailInfo)
{
	if (!inMailInfo.IsValid())
		return;

	_mailInfo = inMailInfo;	

	int titleEllipsisPos = 0;
	int contentsEllipsisPos = 0;
	const UGsGlobalConstant* globalConstant = GData()->GetGlobalData();
	if (nullptr != globalConstant)
	{
		titleEllipsisPos = globalConstant->_mailTitleEllipsisPos;
		contentsEllipsisPos = globalConstant->_mailContentsEllipsisPos;
	}

	FText outTextTitle;
	FGsUIStringHelper::GetEllipsisText(titleEllipsisPos, _mailInfo->GetTextTitle(), outTextTitle);
	_textTitle->SetText(outTextTitle);

	FText outTextContents;
	FGsUIStringHelper::GetEllipsisText(contentsEllipsisPos, _mailInfo->GetTextContent(), outTextContents);
	_textContents->SetText(outTextContents);	
	_btnSwicher->SetActiveWidgetIndex((_mailInfo->GetIsRewards())? 0 : 1);

	if (_mailInfo->GetIsRemainTime())
	{
		if (UWorld* world = GetWorld())
		{
			// 존재하고 있는 타이머가 이미 있다면 제거
			if (_timerHandle.IsValid())
			{
				world->GetTimerManager().ClearTimer(_timerHandle);
			}

			world->GetTimerManager().SetTimer(_timerHandle, this, &UGsUIMailSlot::OnTimer, 1.f, true);
		}
	}

	if (true == _mailInfo->GetIsRead())	// 읽었는데 보상이 있으면 그래도 보여줘 레드닷!!
		_imgRedDot->SetVisibility((_mailInfo->GetIsRewards()) ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed);
	else // 안읽었어 레드닷 보여줘!!
		_imgRedDot->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

	bool isRewards = _mailInfo->GetIsRewards();
	if (isRewards)
	{
		_iconSelector->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		_listIconSlotHelper->RefreshAll(_mailInfo->GetMailAttachmentList().Num());
	}
	else
	{
		_iconSelector->SetVisibility(ESlateVisibility::Collapsed);
	}

	// 최초 1회 한번 호출
	OnTimer();
}

// 받기/삭제 버튼 클릭
void UGsUIMailSlot::OnClickRecvDel()
{
	if (_mailInfo->GetIsRewards())
	{
		GSMail()->SendMailReceive(_mailInfo->GetMailBoxType(), _mailInfo->GetMailDBId());
	}
	else
	{
		GSMail()->SendMailDelete(_mailInfo->GetMailBoxType(), _mailInfo->GetMailDBId());
	}
}

// 아이템 클릭
void UGsUIMailSlot::OnClickSlot()
{
	// 상세보기 창 오픈
	FGsUIMsgParamMail param = FGsUIMsgParamMail(_mailInfo->GetMailDBId());
	GMessage()->GetMail().SendMessage(MessageContentMail::ACTIVE_WINDOW_MAILDETAIL, &param);
}

void UGsUIMailSlot::OnTimer()
{
	const FTimespan remainTime = _mailInfo->GetRemainTime();
	float remainSeconds = remainTime.GetTotalSeconds();

	if (0 < remainSeconds)
	{
		FString stringTime;
		int day = remainTime.GetDays();
		int hours = remainTime.GetHours();
		int minutes = remainTime.GetMinutes();

		if (0 < day)
		{
			stringTime.Append(FGsTimeStringHelper::GetDayString(day));
			stringTime.Append(TEXT(" "));

			if (0 < hours)
			{
				stringTime.Append(FGsTimeStringHelper::GetHourString(hours));
				stringTime.Append(TEXT(" "));
			}
		}
		
		if (stringTime.IsEmpty())
		{
			if (0 < hours)
			{
				stringTime.Append(FGsTimeStringHelper::GetHourString(hours));
				stringTime.Append(TEXT(" "));
				if (0 < minutes)
				{
					stringTime.Append(FGsTimeStringHelper::GetMinuteString(minutes));
					stringTime.Append(TEXT(" "));
				}
			}
		}
		
		if (stringTime.IsEmpty())
		{
			if (0 < minutes)
			{
				stringTime.Append(FGsTimeStringHelper::GetMinuteString(minutes));
				stringTime.Append(TEXT(" "));
			}
		}

		if (stringTime.IsEmpty())
		{
			FText textFind;
			if (FText::FindText(TEXT("FriendText"), TEXT("FriendOffline_Sec"), textFind))
			{
				stringTime = textFind.ToString();
			}
		}

		_textTime->SetText(FText::FromString(stringTime));
	}
	else
	{
		GSMail()->DeleteMail(_mailInfo->GetMailBoxType(), _mailInfo->GetMailDBId());
	}
}

void UGsUIMailSlot::OnRefreshIconSlot(int32 InIndex, UGsUIIconBase* InIcon)
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
					itemIcon->SetCurrency((CurrencyType)mailAttachment->GetId(), mailAttachment->GetAmount());
					break;
				case MailAttachmentType::ITEM:
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

void UGsUIMailSlot::OnCreateIcon(UGsUIIconBase* InIcon)
{
	UGsUIIconItemReward* itemIcon = Cast<UGsUIIconItemReward>(InIcon);
	if (itemIcon)
	{
		itemIcon->OnFinishedLongPressEvent.AddUObject(this, &UGsUIMailSlot::OnLongPressIcon);
	}
}

void UGsUIMailSlot::OnLongPressIcon(UGsUIIconItem& InIcon)
{
	UGsUIIconItem::DefaultItemDetailOpener(InIcon, true);
}
