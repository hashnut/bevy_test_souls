// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/UIContent/Window/Community/GsUIAttendancePage.h"

#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "Shared/Client/SharedEnums/SharedCommunityEventEnum.h"

#include "T1Project.h"
#include "Item/GsItemManager.h"

#include "UI/UIControlLib/ContentWidget/GsButton.h"
#include "UI/UIControlLib/Control/GsDynamicIconSlotHelper.h"
#include "UI/UIControlLib/Control/GsDynamicPanelSlotHelper.h"
#include "UI/UIControlLib/ContentWidget/GsWrapBoxIconSelector.h"
#include "UI/UIControlLib/Control/GsToggleGroup.h"
#include "UI/UIContent/Common/GsUICurrencyButton.h"
#include "UI/UIContent/Window/Community/GsUIAttendanceTab.h"
#include "UI/UIContent/Window/Community/GsUIAttendanceItem.h"
#include "UI/UIContent/Helper/GsUIColorHelper.h"
#include "UI/UIContent/Define/EGsUIColorType.h"

#include "Management/ScopeGame/GsCommunityEventManager.h"

#include "Attendance/GsAttendanceDefine.h"
#include "Attendance/GsAttendanceInfo.h"

#include "Attendance/Reward/GsSchemaAttendanceReward.h"
#include "Attendance/GsSchemaAttendanceEvent.h"
#include "Attendance/Reward/GsSchemaAttendanceCurrency.h"
#include "Attendance/Reward/GsSchemaAttendanceItem.h"

#include "DataSchema/CommunityGirl/GsSchemaCommunityEvent.h"
#include "DataSchema/Attendance/GsSchemaCommunityEventAttendance.h"

#include "UserWidget.h"
#include "Text.h"
#include "Components/CanvasPanel.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Components/RichTextBlock.h"
#include "Components/WidgetSwitcher.h"
#include "DataSchema/GsSchemaResetTimeTicketConfig.h"
#include "UTIL/GsCronUtil.h"
#include "UI/UIContent/Helper/GsTimeStringHelper.h"
#include "UMG/Public/Components/ScrollBox.h"



void UGsUIAttendancePage::BeginDestroy()
{
	if (nullptr != _listScrollBoxHelper)
	{
		_listScrollBoxHelper->OnRefreshEntry.RemoveDynamic(this, &UGsUIAttendancePage::OnRefreshEntryListScrollBox);
		_listScrollBoxHelper = nullptr;
	}

	Super::BeginDestroy();
}

void UGsUIAttendancePage::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	// tab 스크롤 셋팅
	if (nullptr == _listScrollBoxHelper)
	{
		_listScrollBoxHelper = NewObject<UGsDynamicPanelSlotHelper>(this);
		_listScrollBoxHelper->Initialize(_listWrapEntryClass, (UPanelWidget*)_listWrapBox);
		_listScrollBoxHelper->OnRefreshEntry.AddDynamic(this, &UGsUIAttendancePage::OnRefreshEntryListScrollBox);
	}

	_btnRewardRev->OnClicked.AddDynamic(this, &UGsUIAttendancePage::OnClickReward);
	_btnPurchase->OnClickCurrencyButton.BindUObject(this, &UGsUIAttendancePage::OnClickPurchase);

	// 버튼 정보 표시
	FText textBtnName;
	// TEXT: 구매하기
	FText::FindText(TEXT("AttendanceRewardText"), TEXT("UI_Button_Buy"), textBtnName);

	_btnPurchase->SetButtonName(textBtnName);
	_btnPurchase->SetIsEnabled(true);
}

void UGsUIAttendancePage::NativeDestruct()
{
	_itemIndex = 0;
	_tapIndex = 0;
	_expire = false;
	_isInvalidateAll = false;

	Super::NativeDestruct();
}

void UGsUIAttendancePage::NativeConstruct()
{
	Super::NativeDestruct();
}

// list 스크롤 업데이트
void UGsUIAttendancePage::OnRefreshEntryListScrollBox(int32 inIndex, class UWidget* InEntry)
{
	TSharedPtr<FGsAttendanceData> attendanceData = GSCommunityEvent()->GetAttendanceData(_attendanceId);
	if (!attendanceData.IsValid())
		return;	

	if (_schemaAttendance)
	{
		if (inIndex < _schemaAttendance->rewardList.Num())
		{
			if (UGsUIAttendanceItem* item = Cast<UGsUIAttendanceItem>(InEntry))
			{
				bool isTodayIndex = false; // 오늘 출석 인덱스야?
				bool isAcceptable = false; // 오늘 출석했어?
				if (false == attendanceData->IsCheckInToday())
				{
					// 출석 안했어.					
					// 오늘 출석 인덱스야?
					if (inIndex == attendanceData->GetCount())
					{
						// 오늘 받을 게 있어
						isAcceptable = true;
						isTodayIndex = true;
					}
				}
				else
				{
					// 오늘 출석했어
					// 오늘 받을 게 없어					
					// 오늘 출석 인덱스야?
					if (inIndex + 1 == attendanceData->GetCount())
					{
						isTodayIndex = true;
					}
				}

				if (false == _isInvalidateAll && isTodayIndex)
				{
					if (!attendanceData->IsAllCompleted())
					{
						UpdateRewardScrollOffset(inIndex, item);
					}
					else
					{
						SetRewardScrollOffSet(0.0f);
					}
				}

				bool isRewarded = false;
				if (inIndex < attendanceData->GetCount())
					isRewarded = true;
				
				item->SetData(inIndex, isRewarded, isAcceptable, &_schemaAttendance->rewardList[inIndex]);
				item->SetIsSelected(false);				

				if (false == item->_delegateClickSlot.IsBoundToObject(this))
				{
					item->_delegateClickSlot.BindUObject(this, &UGsUIAttendancePage::OnClickIconSlot);
				}
			}
		}
	}
}

void UGsUIAttendancePage::UpdateRewardScrollOffset(int32 inIndex, UGsUIAttendanceItem* inItem)
{
	if (nullptr == inItem)
		return;
	
	float heightSize = inItem->GetHeightSize();
	float scrollOff = (inIndex / 7) * heightSize;
	scrollOff = scrollOff + ((inIndex / 7) * _listWrapBox->InnerSlotPadding.Y);

	SetRewardScrollOffSet(scrollOff);
}

// 구입
void UGsUIAttendancePage::OnClickPurchase()
{
	if (true == _expire)
		return;

	TSharedPtr<FGsAttendanceData> attendanceData = GSCommunityEvent()->GetAttendanceData(_attendanceId);
	if (!attendanceData.IsValid())
		return;

	GSCommunityEvent()->SendReqPurchaseAttendance(attendanceData->GetId());
}

// 보상
void UGsUIAttendancePage::OnClickReward()
{
	if (true == _expire)
		return;

	TSharedPtr<FGsAttendanceData> attendanceData = GSCommunityEvent()->GetAttendanceData(_attendanceId);
	if (!attendanceData.IsValid())
		return;
	
	if (true == attendanceData->IsCheckInToday())
		return;

	if (true == attendanceData->IsCompleted())
		return;

	GSCommunityEvent()->SendReqCheckAttendance(attendanceData->GetId(), (AttendanceCheckIdx)attendanceData->GetCount() + 1);
}

void UGsUIAttendancePage::UpdatePage(int32 InIndex)
{
	TSharedPtr<FGsAttendanceData> attendanceData = GSCommunityEvent()->GetAttendanceData(_attendanceId);
	if (!attendanceData.IsValid())
		return;

	if (nullptr == _schemaAttendance)
		return;

	if (nullptr == _schemaCommunityEvent)
		return;

	const FGsSchemaCommunityEventDetail* eventDetail = _schemaCommunityEvent->detail.GetRow();
	if (nullptr != eventDetail)
	{
		_textTitle->SetText(eventDetail->name);
		_textDesc->SetText(eventDetail->desc);
		
		FDateTime tmEnd;
		FDateTime::Parse(_schemaCommunityEvent->eventTimeEnd, tmEnd);

		FDateTime clientDateTime = FGsTimeSyncUtil::ConvertSeverDateTimeToClientDateTime(tmEnd);

		FString endDateText;
		FGsTimeStringHelper::GetTimeStringNYMD(clientDateTime, endDateText);
		
		_textEndTime->SetText(FText::FromString(endDateText));
	}

	// 기간이 정해져 있을 경우, 이벤트 타입과 관계 없이 유효기간을 출력
	if (_schemaCommunityEvent->eventTimeTextType == ECommunityEventTimeType::PERIOD)
	{
		FDateTime startClientDateTime = FGsTimeSyncUtil::ConvertSeverDateTimeToClientDateTime(attendanceData->GetStartTime());

		// 언제 부터 ~ 언제까지
		FString startDateText;
		FGsTimeStringHelper::GetTimeStringNYMD(startClientDateTime, startDateText);

		FDateTime endClientDateTime = FGsTimeSyncUtil::ConvertSeverDateTimeToClientDateTime(attendanceData->GetEndTime());

		FString endDateText;
		FGsTimeStringHelper::GetTimeStringNYMD(endClientDateTime, endDateText);

		FText textExpr;
		FText::FindText(TEXT("AttendanceRewardText"), TEXT("UI_Main_ExpirationDate"), textExpr);

		textExpr = FText::Format(textExpr, FText::FromString(startDateText), FText::FromString(endDateText));

		_textExpirationDate->SetText(textExpr);
		_textExpirationDate->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}
	else
	{
		_textExpirationDate->SetVisibility(ESlateVisibility::Collapsed);
	}
	

	// 획득 시점 텍스트는 기본적으로 흰색으로 세팅
	//_textResetData->SetColorAndOpacity(FGsUIColorHelper::GetColor(EGsUIColorType::NORMAL_TEXT_COLOR));
	_textResetData->SetDefaultColorAndOpacity(FGsUIColorHelper::GetColor(EGsUIColorType::NORMAL_TEXT_COLOR));

	FText textReset;
	switch (_schemaAttendance->attendanceType)
	{
	case AttendanceType::DEFAULT:
	{
		// 구매할 필요가 없는 기본 출석부의 경우, 획득 보상 설명을 그냥 넣어 준다
		FText::FindText(TEXT("AttendanceRewardText"), TEXT("UI_Main_Reward_Desc"), textReset);

		// 보상 획득 리셋 타임은 일일 보상이 열리는 시점(0시)임
		GetResetTimeText(ResetTimeTicket::RTT_ATTENDANCE_DAILY, textReset);
	}
	break;
	case AttendanceType::NEW:
	case AttendanceType::RETURNING:
	case AttendanceType::EVENT:
	{
		// 매일 보상 언제 받을 수 있는지 텍스트 세팅
		if (false == attendanceData->IsUsable())
		{
			// TEXT : 구매 후 보상을 획득할 수 있습니다
			FText::FindText(TEXT("AttendanceRewardText"), TEXT("UI_Main_BeforeBuy_Desc"), textReset);

			// 구매 전 회색
			//_textResetData->SetColorAndOpacity(FGsUIColorHelper::GetColor(EGsUIColorType::DEFAULT_DISABLE));
			_textResetData->SetDefaultColorAndOpacity(FGsUIColorHelper::GetColor(EGsUIColorType::DEFAULT_DISABLE));
		}
		else
		{
			// TEXT: 매일 오전 {0}시부터 보상을 획득할 수 있습니다. 획득한 보상은 <P>인벤토리</>로 지급됩니다.
			FText::FindText(TEXT("AttendanceRewardText"), TEXT("UI_Main_Reward_Desc"), textReset);

			// 보상 획득 리셋 타임은 일일 보상이 열리는 시점(0시)임
			GetResetTimeText(ResetTimeTicket::RTT_ATTENDANCE_DAILY, textReset);
		}
	}
	break;
	default:
		break;
	}

	// 기존 설명 텍스트의 폰트 사이즈를 유효기간 텍스트 사이즈와 맞추기
	FTextBlockStyle textStyle = _textResetData->GetDefaultTextStyle();
	textStyle.Font.Size = _textExpirationDate->Font.Size;
	_textResetData->SetDefaultTextStyle(textStyle);

	_textResetData->SetText(textReset);

	int rewardCount = _schemaAttendance->rewardList.Num();
	if (0 < rewardCount)
	{
		_listScrollBoxHelper->RefreshAll(rewardCount);
	}

	_dailyBonusItem->SetVisibility(ESlateVisibility::Collapsed);
	
	bool isUsable = attendanceData->IsUsable();
	_btnSwitcher->SetActiveWidgetIndex(static_cast<int32>(!isUsable));
	_deemedCanvasPanel->SetVisibility(isUsable ? ESlateVisibility::Collapsed : ESlateVisibility::HitTestInvisible);

	if (false == isUsable)
	{
		const FGsSchemaAttendanceEvent* schemaAttendanceEvent =
			_schemaAttendance->typeInfo.GetRowByType<FGsSchemaAttendanceEvent>();
		if (nullptr == schemaAttendanceEvent)
			return;

		CostType costType = schemaAttendanceEvent->costType;
		switch (costType)
		{
		case CostType::CURRENCY:
		{
			if (const FGsSchemaAttendanceCurrency* schemaAttendanceCurrency =
				schemaAttendanceEvent->typeInfo.GetRowByType<FGsSchemaAttendanceCurrency>())
			{
				_btnPurchase->SetData(schemaAttendanceCurrency->currencyType, schemaAttendanceCurrency->currencyAmount);
				//_btnPurchase->UpdateAmountTextColorLocalPlayer(); // R2: 구매시 확인 팝업 띄우는 항목은 흰글씨 표기
			}
		}
		break;
		case CostType::ITEM:
		{
			if (const FGsSchemaAttendanceItem* schemaAttendanceItem =
				schemaAttendanceEvent->typeInfo.GetRowByType<FGsSchemaAttendanceItem>())
			{
				if (const FGsSchemaItemCommon* itemCommon = schemaAttendanceItem->itemId.GetRow())
				{
					_btnPurchase->SetData(itemCommon->id, schemaAttendanceItem->itemCount);
				}
			}
		}
		break;
		default:
			break;
		}

		_redDot->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		FText text;

		// 오늘 출석을 받았거나, 이미 모두 완료했으면 '보상 수령' 버튼 및 레드닷 비활성화
		if (attendanceData->IsCheckInToday() || attendanceData->IsCompleted())
		{
			FText::FindText(TEXT("AttendanceRewardText"), TEXT("UI_Button_AlreadyGot"), text);
			_textBtnRewardRev->SetText(text);

			_btnRewardRev->SetIsEnabled(false);
			_redDot->SetVisibility(ESlateVisibility::Collapsed);
		}
		else
		{		
			// 유효 날짜가 아닌 경우
			if (attendanceData->IsValidDay() == false)
			{
				_btnRewardRev->SetIsEnabled(false);
				_redDot->SetVisibility(ESlateVisibility::Collapsed);
			}
			else
			{
				FText::FindText(TEXT("AttendanceRewardText"), TEXT("UI_Button_GetReward"), text);
				_textBtnRewardRev->SetText(text);

				_btnRewardRev->SetIsEnabled(true);
				_redDot->SetVisibility(ESlateVisibility::Visible);
			}
		}
	}

#if UE_EDITOR
	_attendanceIdCanvasPanel->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

	FText textMissionId = FText::FromString(FString::Printf(TEXT("AttendanceId : %lld"), _attendanceId));
	_textAttendanceId->SetText(textMissionId);
#else
	_attendanceIdCanvasPanel->SetVisibility(ESlateVisibility::Collapsed);
#endif
}

bool UGsUIAttendancePage::GetResetTimeText(IN ResetTimeTicket inResetTimeTicket, OUT FText& outText)
{
	const UGsTableResetTimeTicketConfig* table =
		Cast<const UGsTableResetTimeTicketConfig>(FGsSchemaResetTimeTicketConfig::GetStaticTable());
	if (nullptr == table)
	{
		return false;
	}

	const FGsSchemaResetTimeTicketConfig* row = nullptr;
	if (false == table->FindRowById(inResetTimeTicket, row))
	{
		return false;
	}

	// UTC기준 리셋 시각
	FTimespan resetTimeUTC = FGsCronUtil::GetTimespan(row->makeCron);
	if (FTimespan::MinValue() == resetTimeUTC)
	{
		return false;
	}

	outText = FText::Format(outText, resetTimeUTC.GetHours());

	return true;
}

void UGsUIAttendancePage::OnClickIconSlot(int inIndex, class UWidget* inItem)
{
	if (nullptr == inItem)
		return;

	UGsUIAttendanceItem* itemIcon = Cast<UGsUIAttendanceItem>(inItem);
	if (nullptr == itemIcon)
		return;

	if (nullptr != _selectItem)
	{
		_selectItem->SetIsSelected(false);
	}

	_slotItem = inIndex;
	_selectItem = itemIcon;
	_selectItem->SetIsSelected(true);
}

void UGsUIAttendancePage::OnClickDailyBonusSlot(int inIndex, class UWidget* inItem)
{
	if (nullptr == inItem)
		return;	
}

void UGsUIAttendancePage::InvalidateAll()
{
	_isInvalidateAll = true;
	UpdatePage(_tapIndex);
	_isInvalidateAll = false;
}

void UGsUIAttendancePage::OnSelectTab(int index)
{
	_selectItem = nullptr;
	_tapIndex = index;
	UpdatePage(index);
}

void UGsUIAttendancePage::OnSelectTab(int index, const FGsSchemaCommunityEvent* inData)
{
	if (nullptr == inData)
		return;

	_selectItem = nullptr;
	_schemaCommunityEvent = inData;
	_tapIndex = index;

	const FGsSchemaCommunityEventAttendance* schemaAttendance
		= inData->typeInfoEventId.GetRowByType<FGsSchemaCommunityEventAttendance>();

	if (nullptr != schemaAttendance)
	{
		_schemaAttendance = schemaAttendance;
		_attendanceId = schemaAttendance->id;
	}

	UpdatePage(index);
}

void UGsUIAttendancePage::SetNoPage(bool inIsShow)
{
	if (_noPageCanvasPanel)
	{
		_noPageCanvasPanel->SetVisibility((true == inIsShow)? ESlateVisibility::HitTestInvisible : ESlateVisibility::Collapsed);
	}
}

void UGsUIAttendancePage::SetExpire()
{
	_expire = true;
}

void UGsUIAttendancePage::SetRewardScrollOffSet(float inValue)
{
	if (_scrollBox)
	{
		_scrollBox->SetScrollOffset(inValue);
	}
}