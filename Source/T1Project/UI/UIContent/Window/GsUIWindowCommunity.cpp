// Fill out your copyright notice in the Description page of Project Settings.
#include "GsUIWindowCommunity.h"
#include "UI/UIControlLib/ContentWidget/GsButton.h"
#include "Message/GsMessageContents.h"

#include "Management/GsMessageHolder.h"
#include "Management/ScopeGame/GsCommunityEventManager.h"

#include "UI/UIContent/Window/Community/GsUIAttendancePage.h"
#include "UI/UIContent/Window/Community/GsUICommunityEventMissionPage.h"
#include "UI/UIControlLib/ContentWidget/GsButton.h"
#include "UI/UIControlLib/Control/GsDynamicPanelSlotHelper.h"
#include "UI/UIControlLib/Control/GsToggleGroup.h"
#include "UI/UIContent/Window/Community/GsUIAttendanceTab.h"

#include "Attendance/GsAttendanceInfo.h"

#include "CanvasPanel.h"
#include "TextBlock.h"
#include "UserWidget.h"
#include "WidgetSwitcher.h"
#include "RichTextBlock.h"

#include "DataSchema/CommunityGirl/GsSchemaCommunityEvent.h"
#include "UTIL/GsTimeSyncUtil.h"
#include "Management/ScopeGlobal/GsHiveManager.h"
#include "SlateBrush.h"
#include "Engine/AssetManager.h"
#include "Paper2D/Classes/PaperSprite.h"
#include "SlateTextureAtlasInterface.h"
#include "UMG/Public/Components/Image.h"
#include "UI/UIContent/Helper/GsTimeStringHelper.h"
#include "Async.h"
#include "HIVESDK.h"


void UGsUIWindowCommunity::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	_helpContentsKey = EGsHelpContents::Event;

	_btnClose->OnClicked.AddDynamic(this, &UGsUIWindowCommunity::OnClickCloseAllStack);
	_btnBack->OnClicked.AddDynamic(this, &UGsUIWindowCommunity::OnClickBack);
	_btnMainNotice->OnClicked.AddDynamic(this, &UGsUIWindowCommunity::OnClickPromotionNotice);
	_btnCommunity->OnClicked.AddDynamic(this, &UGsUIWindowCommunity::OnClickCommunity);

	TArray<UWidget*> childList = _tabScrollPanel->GetAllChildren();
	for (auto child : childList)
	{
		if (child->IsA<UGsUIAttendanceTab>())
		{
			child->RemoveFromParent();
		}
	}
	childList.Empty();

	_tabScrollBoxHelper = NewObject<UGsDynamicPanelSlotHelper>(this);
	_tabScrollBoxHelper->Initialize(_tabScrollBoxEntryClass, _tabScrollPanel);
	_tabScrollBoxHelper->OnCreateEntry.AddDynamic(this, &UGsUIWindowCommunity::OnCreateTabButtonEntry);
	_tabScrollBoxHelper->OnRefreshEntry.AddDynamic(this, &UGsUIWindowCommunity::OnRefreshTabButtonEntry);

	_tabToggleGroup.OnSelectChanged.BindUObject(this, &UGsUIWindowCommunity::OnClickTab);	
		
	FText::FindText(TEXT("SystemText"), TEXT("NDays"), _textDay);		
	FText::FindText(TEXT("SystemText"), TEXT("NHours"), _textHour);
	FText::FindText(TEXT("SystemText"), TEXT("NMinutes"), _textMin);	
	FText::FindText(TEXT("SystemText"), TEXT("NSeconds"), _textSec);
	FText::FindText(TEXT("CommunityGirlText"), TEXT("UI_Screen_OverTime"), _textOverTime);
	FText::FindText(TEXT("CommunityGirlText"), TEXT("UI_Screen_Condition_EventComplete"), _textConditionComplated);
}

void UGsUIWindowCommunity::BeginDestroy()
{
	if (nullptr != _tabScrollBoxHelper)
	{
		_tabScrollBoxHelper->OnCreateEntry.RemoveDynamic(this, &UGsUIWindowCommunity::OnCreateTabButtonEntry);
		_tabScrollBoxHelper->OnRefreshEntry.RemoveDynamic(this, &UGsUIWindowCommunity::OnRefreshTabButtonEntry);
	}
	_tabToggleGroup.Clear();
	
	Super::BeginDestroy();
}


void UGsUIWindowCommunity::NativeConstruct()
{
	Super::NativeConstruct();
	
	_tapIndex = -1;
	_eventId = INVALID_COMMUNITY_EVENT_ID;
	SetDirtyInvalidate(true);
}

void UGsUIWindowCommunity::NativeDestruct()
{
	if (FGsCommunityEventManager* commnunityMgr = GSCommunityEvent())
	{
		commnunityMgr->SaveRedDot();
	}
	Super::NativeDestruct();
}

FReply UGsUIWindowCommunity::NativeOnTouchStarted(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent)
{
	return FReply::Unhandled();
}

void UGsUIWindowCommunity::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	if (_dlgEndTime > 0.f)
	{
		_dlgEndTime -= InDeltaTime;
	}

	if (_dlgEndTime <= 0)
	{
		_dlgEndTime = 0.f;
		_talkBox->SetVisibility(ESlateVisibility::Collapsed);
	}

	if (0 < _updateRemainSec)
	{
		_updateRemainSec -= InDeltaTime;

		if (0 > _updateRemainSec)
		{
			_updateRemainSec = _updateSec;
			UpdateConditionTime();
		}
	}
}

// 닫기
void UGsUIWindowCommunity::CloseInternal()
{
	GMessage()->GetCommunity().SendMessage(MessageContentCommunity::REDDOT_UPDATE, nullptr);

	Super::CloseInternal();
}

bool UGsUIWindowCommunity::SyncContentState()
{
	if (false == IsSameContentState(FGsContentsMode::InGame::ContentsCommunity))
	{
		GMessage()->GetContents().SendMessage(MessageContents::ACTIVE_WINDOW_COMMUNITY);
		return true;
	}

	return false;
}

void UGsUIWindowCommunity::InvalidateAllInternal()
{
	Super::InvalidateAllInternal();	
}

void UGsUIWindowCommunity::OnClickBack()
{
	Super::OnClickBack();
}

void UGsUIWindowCommunity::OnClickCloseAllStack()
{
	Super::OnClickCloseAllStack();
}

void UGsUIWindowCommunity::ChangeDialog(const FGsSchemaCommunityDialog* dlg)
{
	_talkText->SetText(dlg->Dialog);
	_talkBox->SetVisibility(ESlateVisibility::Visible);

	_dlgEndTime = dlg->timeLimit;
}

void UGsUIWindowCommunity::UpdateCommunityEvent()
{
	_communityEventIdList.Empty();
	GSCommunityEvent()->GetCommunityEventDataList(_communityEventIdList);
	int pageNum = _communityEventIdList.Num();

	if (pageNum <= 0)
	{
		return;
	}
	_tabScrollBoxHelper->RefreshAll(pageNum);

	if (0 > _tapIndex)
	{
		// 처음 열리는 거면
		_eventId = _communityEventIdList[0];
	}
	else
	{
		CommunityEventId eventId = _eventId;
		_tapIndex = _communityEventIdList.IndexOfByPredicate([eventId](CommunityEventId el)
			{
				return el == eventId;
			});

		if (0 > _tapIndex)
		{
			_eventId = _communityEventIdList[0];
		}
	}

	if (_eventId == INVALID_COMMUNITY_EVENT_ID)
	{
		return;
	}

	int tapIndex = _tapIndex < 0 ? 0 : _tapIndex;
	UpdatePage(_eventId, tapIndex);
	_tapIndex = tapIndex;
	_tabToggleGroup.SetSelectedIndex(_tapIndex, true);	
}

void UGsUIWindowCommunity::OnClickPromotionNotice()
{
	if (!GHive()->IsSignIn()) return;

	// 윈도우에서는 HIVESDKCEFImeEnable와 HIVESDKCEFImeDisable을 호출해줘야 한글이 입력된다.
	// 후에 hive 쪽에 한글입력이 필요한 부분이 있을 시 
	// 꼭 같이 넣어주자.
#if PLATFORM_WINDOWS
	HIVESDKCEFImeEnable();
#endif
	Promotion::showPromotion(PromotionType::NOTICE, true, [](ResultAPI const& result, PromotionEventType promotionEventType) 
		{
#if PLATFORM_WINDOWS
		HIVESDKCEFImeDisable();
#endif
			if (result.isSuccess())
			{
				//열때/닫을때 둘다 일단은 tab 변동
				if (promotionEventType == PromotionEventType::OPEN)
				{
					AsyncTask(ENamedThreads::GameThread, []() {
						GMessage()->GetCommunity().SendMessage(MessageContentCommunity::SELECT_TAB, nullptr);
					});

					Promotion::updatePromotionData();
				}
				else if (promotionEventType == PromotionEventType::CLOSE)
				{
					AsyncTask(ENamedThreads::GameThread, []() {
						GMessage()->GetCommunity().SendMessage(MessageContentCommunity::SELECT_TAB, nullptr);
					});
				}
			}
			else
			{
				SCREEN_MESSAGE(1004, 200.0f, FColor::Green, "OnClickPromotionNotice:  code[%d], message[%s], error[%d]",
					result.code, *FString(result.toString().c_str()), result.errorCode);
			}
		});
}

void UGsUIWindowCommunity::OnClickCommunity()
{
	GHive()->ShowCommunity();
}

void UGsUIWindowCommunity::OnCreateTabButtonEntry(UWidget* InEntry)
{
	if (UGsUIAttendanceTab* tab = Cast<UGsUIAttendanceTab>(InEntry))
	{
		_tabToggleGroup.AddToggleWidget(tab);
		tab->_delegateClickTab.BindUObject(this, &UGsUIWindowCommunity::OnClickTab);
	}
}

// tab 스크롤 업데이트
void UGsUIWindowCommunity::OnRefreshTabButtonEntry(int32 inIndex, class UWidget* InEntry)
{
	if(auto tab = Cast<UGsUIAttendanceTab>(InEntry))
	{
		int tabNum = _communityEventIdList.Num();
		if (0 < tabNum)
		{
			CommunityEventId id = _communityEventIdList[inIndex];
			
			TSharedPtr<FGsCommunityData> outCommunityData = GSCommunityEvent()->GetCommunityEventData(id);
			if (outCommunityData.IsValid())
			{
				tab->SetData(id, outCommunityData->GetTitleName(), GSCommunityEvent()->IsRedDot(id));
			}
		}
	}
}

void UGsUIWindowCommunity::OnClickTab(CommunityEventId inEventId, bool inIsOn)
{
	CommunityEventId eventId = inEventId;
	if (_communityEventIdList.ContainsByPredicate([eventId](CommunityEventId el) {
		return el == eventId;
		}))
	{
		int index = _communityEventIdList.IndexOfByPredicate([eventId](CommunityEventId el)
			{
				return el == eventId;
			});

		UpdatePage(eventId, index);
		_tapIndex = index;
		_eventId = eventId;
	}
}

void UGsUIWindowCommunity::UpdatePage(CommunityEventId inEventId, int inIndex)
{
	TSharedPtr<FGsCommunityData> outCommunityData = GSCommunityEvent()->GetCommunityEventData(inEventId);
	if (!outCommunityData.IsValid())
		return;

	const FGsSchemaCommunityEvent* schemaCommunityEvent = outCommunityData->GetSchemaCommunityEvent();
	if (nullptr == schemaCommunityEvent)
		return;

	// 배너셋팅
	SetBanner(schemaCommunityEvent);

	_imgBanner->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

	if (schemaCommunityEvent->eventType == CommunityEventType::BANNER)
	{
		_communityEventType->SetVisibility(ESlateVisibility::Collapsed);
	}
	else
	{
		_communityEventType->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}

	if (_communityEventType)
	{
		_communityEventType->SetActiveWidgetIndex((int)schemaCommunityEvent->eventType);
	}

	if (schemaCommunityEvent->eventType == CommunityEventType::ATTENDANCE)
	{
		if (_tapIndex != inIndex)
			_attendancePage->OnSelectTab(inIndex, schemaCommunityEvent);
		else
			_attendancePage->InvalidateAll();
	}
	else if (schemaCommunityEvent->eventType == CommunityEventType::MISSION)
	{
		if (_tapIndex != inIndex)
			_communityEventMissionPage->OnSelectMission(inIndex, schemaCommunityEvent);
		else
			_communityEventMissionPage->InvalidateAll();

		//if (_tapIndex >= 0) GSCommunityEvent()->SetRedDot(inEventId); //미리 꺼두고 : 두번 호출되어 페이지 확인 레드닷을 켤 수가 없어, tabIndex init값을 -1로 고침
		GSCommunityEvent()->SetRedDot(inEventId);
	}
	else
	{
		//if (_tapIndex >= 0) GSCommunityEvent()->SetRedDot(inEventId); //미리 꺼두고 : 두번 호출되어 페이지 확인 레드닷을 켤 수가 없어, tabIndex init값을 -1로 고침
		GSCommunityEvent()->SetRedDot(inEventId);
	}

	if (_tapIndex != inIndex && _communityEventIdList.IsValidIndex(_tapIndex))
	{
		TSharedPtr<FGsCommunityData> preCommunityData = GSCommunityEvent()->GetCommunityEventData(_communityEventIdList[_tapIndex]);
		if (preCommunityData.IsValid())
		{
			if (const auto data = preCommunityData->GetSchemaCommunityEvent())
			{
				if (data->eventType == CommunityEventType::BANNER ||
					data->eventType == CommunityEventType::MISSION)
				{
					_tabScrollBoxHelper->RefreshByIndex(_tapIndex); //다른 탭으로 넘어갈때 갱신
				}
			}
		}
	}

	if (_richTextEndTime)
	{	
		_updateRemainSec = 0.0f;
		FText text = schemaCommunityEvent->eventTimeText;	

		_overTimeCanvasPanel->SetVisibility(ESlateVisibility::Collapsed);
		
		if (ECommunityEventTimeType::CONDITION == schemaCommunityEvent->eventTimeTextType)
		{
			if (true == outCommunityData->GetIsConditionComplated())
			{
				// 보상을 다 받아서 몇일 후 이벤트가 종료된다.
				_textCondition = _textConditionComplated;
			}
			else
			{
				_textCondition = text;				
			}

			_tmCondition = outCommunityData->GetEndTime();
			_updateRemainSec = _updateSec;
			UpdateConditionTime();
		}
		else
		{
			if (ECommunityEventTimeType::ALWAYS == schemaCommunityEvent->eventTimeTextType)
			{
				_richTextEndTime->SetText(text);
			}
			else if (ECommunityEventTimeType::PERIOD == schemaCommunityEvent->eventTimeTextType)
			{
				FDateTime tmEnd;
				FDateTime::Parse(schemaCommunityEvent->eventTimeEnd, tmEnd);

				FDateTime clientDateTime = FGsTimeSyncUtil::ConvertSeverDateTimeToClientDateTime(tmEnd);

				FString endDateText;
				FGsTimeStringHelper::GetTimeStringNYMD(clientDateTime, endDateText);

				FText outputText = FText::Format(text, FText::FromString(endDateText));
				_richTextEndTime->SetText(outputText);
			}
		}
	}

	if (_richTextEndTime->GetText().IsEmptyOrWhitespace())
	{
		_richTextEndTime->SetVisibility(ESlateVisibility::Collapsed);
	}
	else
	{
		_richTextEndTime->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}

#if UE_EDITOR
	_eventIdCanvasPanel->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

	FText textEventId = FText::FromString(FString::Printf(TEXT("EventId : %lld"), inEventId));
	_textEventId->SetText(textEventId);
#else
	_eventIdCanvasPanel->SetVisibility(ESlateVisibility::Collapsed);
#endif
}

void UGsUIWindowCommunity::OnClickTab(int32 inIndex)
{
	if (_communityEventIdList.IsValidIndex(inIndex) == false)
		return;
	
	CommunityEventId eventId = _communityEventIdList[inIndex];
	if (0 >= eventId)
		return;
}

void UGsUIWindowCommunity::SetBanner(const FGsSchemaCommunityEvent* inCommunityEvent)
{
	if (nullptr == inCommunityEvent)
		return;

	UTexture2D* image = Cast<UTexture2D>(UAssetManager::GetStreamableManager().LoadSynchronous(inCommunityEvent->bannerImgPath));
	if (image)
	{
		_imgBanner->SetBrushFromTexture(image);
	}
}

// tab 업데이트
void UGsUIWindowCommunity::UpdateTab()
{
	int tabCount = _tabScrollBoxHelper->GetItemCount();

	for (int i = 0; i < tabCount; ++i)
	{
		UWidget* widget = _tabScrollBoxHelper->GetEntry(i);
		if (nullptr == widget)
		{
			continue;
		}

		if (UGsUIAttendanceTab* tab = Cast<UGsUIAttendanceTab>(widget))
		{
			int tabNum = _communityEventIdList.Num();
			if (i < tabNum)
			{
				bool isFind = false;

				CommunityEventId eventId = tab->GetEventId();
				if (_communityEventIdList.ContainsByPredicate([eventId](CommunityEventId el) {
					return el == eventId;
					}))
				{
					TSharedPtr<FGsCommunityData> outCommunityData = GSCommunityEvent()->GetCommunityEventData(eventId);
					if (outCommunityData.IsValid())
					{
						if (outCommunityData.IsValid())
						{
							tab->SetRedDot(GSCommunityEvent()->IsRedDot(eventId));
						}
					}
				}				
			}
		}
	}
}

void UGsUIWindowCommunity::UpdateConditionTime()
{
	// “이벤트 기간 : 이벤트 종료까지 {0} 남음” 
	// {0} = durationHoursReceive → 일 또는 시간으로 1개 단위까지 표현 (ex. 720 = 30일, 24 = 1일, 12 = 12시간)
	// {0}은 1시간 기준으로 잔여 시간 갱신 
	// 일 → 시간으로 변경 (잔여 시간 24 = 1일 ▶ 잔여시간 23 = 23시간)
		
	FDateTime time = FGsTimeSyncUtil::ConvertSeverDateTimeToClientDateTime(_tmCondition);
	FDateTime nowTime = FGsTimeSyncUtil::GetClientNowDateTime();

	FTimespan span = time - nowTime;
	FString strTime;

	// n일 이상: n일 표기
	if (0 < span.GetDays())
	{
		if (!_textDay.IsEmpty())
		{
			strTime = FText::Format(_textDay, span.GetDays()).ToString();
		}

		FText outputText = FText::Format(_textCondition, FText::FromString(strTime));
		_richTextEndTime->SetText(outputText);

		return;
	}

	// n시간 이상: n시간 표기
	if (0 < span.GetHours())
	{
		if (!_textHour.IsEmpty())
		{
			strTime = FText::Format(_textHour, span.GetHours()).ToString();
		}

		FText outputText = FText::Format(_textCondition, FText::FromString(strTime));
		_richTextEndTime->SetText(outputText);

		return;
	}

	// n분 이상: n분 전으로 표기.
	if (0 < span.GetMinutes())
	{
		if (!_textMin.IsEmpty())
		{
			strTime = FText::Format(_textMin, span.GetMinutes()).ToString();
		}

		FText outputText = FText::Format(_textCondition, FText::FromString(strTime));
		_richTextEndTime->SetText(outputText);

		return;
	}

	if (0 < span.GetSeconds())
	{
		if (!_textSec.IsEmpty())
		{
			strTime = FText::Format(_textSec, span.GetSeconds()).ToString();
		}

		FText outputText = FText::Format(_textCondition, FText::FromString(strTime));
		_richTextEndTime->SetText(outputText);

		return;
	}

	if (0 >= span.GetSeconds())
	{
		_overTimeCanvasPanel->SetVisibility(ESlateVisibility::Visible);
		_updateRemainSec = 0.0f;
		_richTextEndTime->SetText(_textOverTime);
	}
}

void UGsUIWindowCommunity::UpdateExpire()
{
	int tabCount = _tabScrollBoxHelper->GetItemCount();

	for (int i = 0; i < tabCount; ++i)
	{
		UWidget* widget = _tabScrollBoxHelper->GetEntry(i);
		if (nullptr == widget)
		{
			continue;
		}

		if (UGsUIAttendanceTab* tab = Cast<UGsUIAttendanceTab>(widget))
		{
			int tabNum = _communityEventIdList.Num();
			if (i < tabNum)
			{
				bool isFind = false;

				CommunityEventId eventId = tab->GetEventId();
				if (_communityEventIdList.ContainsByPredicate([eventId](CommunityEventId el) {
					return el == eventId;
					}))
				{
					TSharedPtr<FGsCommunityData> outCommunityData = GSCommunityEvent()->GetCommunityEventData(eventId);
					if (outCommunityData.IsValid())
					{
						if (false == outCommunityData->IsValid())
						{
							if (outCommunityData->GetType() == CommunityEventType::ATTENDANCE)
							{
								_attendancePage->SetExpire();
							}
							else if (outCommunityData->GetType() == CommunityEventType::MISSION)
							{
								_communityEventMissionPage->SetExpire();
							}

							tab->SetRedDot(false);
						}
					}
				}
			}
		}
	}
}

void UGsUIWindowCommunity::SetEventOverTime(bool inIsShow)
{
	if (_overTimeCanvasPanel)
	{
		_overTimeCanvasPanel->SetVisibility((true == inIsShow) ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Collapsed);
	}
}