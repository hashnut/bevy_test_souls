// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUIPopupChannelList.h"
#include "Components/ScrollBox.h"
#include "UI/UIControlLib/ContentWidget/GsButton.h"
#include "UI/UIControlLib/Control/GsDynamicPanelSlotHelper.h"
#include "UI/UIContent/Popup/ChannelList/GsUIChannelEntry.h"
#include "UI/UIContent/Helper/GsTimeStringHelper.h"
#include "Management/ScopeGlobal/GsGameDataManager.h"
#include "Management/GsMessageHolder.h"
#include "Message/MessageParam/GsMessageParam.h"
#include "Engine/World.h"
#include "Public/TimerManager.h"


void UGsUIPopupChannelList::BeginDestroy()
{
	if (_slotHelper)
	{
		_slotHelper->OnRefreshEntry.RemoveDynamic(this, &UGsUIPopupChannelList::OnRefreshEntry);
	}
	_slotHelper = nullptr;

	Super::BeginDestroy();
}

void UGsUIPopupChannelList::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	_btnClose->OnClicked.AddDynamic(this, &UGsUIPopupChannelList::OnClickClose);

	_slotHelper = NewObject<UGsDynamicPanelSlotHelper>(this);
	_slotHelper->Initialize(_entryWidgetClass, _panelChannel);
	_slotHelper->OnRefreshEntry.AddDynamic(this, &UGsUIPopupChannelList::OnRefreshEntry);
}

void UGsUIPopupChannelList::NativeConstruct()
{
	Super::NativeConstruct();

	
	_recieveChannelListHandler = GMessage()->GetUI().AddUObject(MessageUI::RECEIVE_CHANNEL_LIST,
		this, &UGsUIPopupChannelList::OnRecieveChannelList);
	

	// 텍스트 포맷을 미리 받아둔다
	// TEXT: {0} 후에 채널 이동 할 수 있습니다.
	FText::FindText(TEXT("UICommonText"), TEXT("ChannelDescRemainTime"), _textRemainTimeFormat);

	InvalidateAll();

	_scroll->ScrollToStart();
}

void UGsUIPopupChannelList::NativeDestruct()
{	
	GMessage()->GetUI().Remove(_recieveChannelListHandler);

	if (_timerHandle.IsValid())
	{
		if (UWorld* world = GetWorld())
		{
			world->GetTimerManager().ClearTimer(_timerHandle);
		}
	}

	Super::NativeDestruct();
}

void UGsUIPopupChannelList::InvalidateAllInternal()
{
	FGsGameDataManager* gameDataMgr = GGameData();
	float remainTime = gameDataMgr->GetChangeChannelRemainTime();
	if (0 < remainTime)
	{
		if (UWorld* world = GetWorld())
		{
			// 존재하고 있는 타이머가 이미 있다면 제거
			if (_timerHandle.IsValid())
			{
				world->GetTimerManager().ClearTimer(_timerHandle);
			}

			world->GetTimerManager().SetTimer(_timerHandle, this, &UGsUIPopupChannelList::OnTimer, 1.f, true);
		}
	}

	// 안내 문구 설정
	SetUIDescText(remainTime);

	_currChannel = gameDataMgr->GetCurrentChannel();	
	_dataList = &gameDataMgr->GetChannelList();	

	// 채널 리스트 갱신
	_slotHelper->RefreshAll(_dataList->Num());
}

void UGsUIPopupChannelList::OnInputCancel()
{
	Close();
}

void UGsUIPopupChannelList::OnRecieveChannelList(const IGsMessageParam*)
{
	InvalidateAll();
}

void UGsUIPopupChannelList::OnRefreshEntry(int32 InIndex, UWidget* InEntry)
{
	if (false == _dataList->IsValidIndex(InIndex))
	{
		return;
	}

	UGsUIChannelEntry* channelEntry = Cast<UGsUIChannelEntry>(InEntry);
	if (nullptr == channelEntry)
	{
		return;
	}

	const FGsMapChannelData& data = _dataList->GetData()[InIndex];
	channelEntry->SetData(data, (_currChannel == data._channel) ? true : false);
}

void UGsUIPopupChannelList::OnClickClose()
{
	OnInputCancel();
}

void UGsUIPopupChannelList::OnTimer()
{
	float remainTime = GGameData()->GetChangeChannelRemainTime();
	// 남은 시간이 모두 지났음
	if (0.f >= remainTime)
	{
		// 타이머 제거
		if (UWorld* world = GetWorld())
		{
			if (_timerHandle.IsValid())
			{
				world->GetTimerManager().ClearTimer(_timerHandle);
			}
		}

		remainTime = 0.f;
	}

	SetUIDescText(remainTime);
}

void UGsUIPopupChannelList::SetUIDescText(float InTime)
{
	if (0.f >= InTime)
	{
		// TEXT: 이동하려는 채널을 선택해 주세요
		FText::FindText(TEXT("UICommonText"), TEXT("ChannelDescSelect"), _textDesc);
		return;
	}

	if (false == _textRemainTimeFormat.IsEmpty())
	{
		FString strTime;
		FGsTimeStringHelper::GetTimeStringCeil(FTimespan::FromSeconds(InTime), strTime);

		// TEXT: n분 n초 후에 채널 이동 할 수 있습니다.
		_textDesc = FText::Format(_textRemainTimeFormat, FText::FromString(strTime));
	}
}