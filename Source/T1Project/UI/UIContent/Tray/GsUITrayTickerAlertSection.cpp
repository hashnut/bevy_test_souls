#include "GsUITrayTickerAlertSection.h"
#include "NotiMessage/GsUISectionMessageBase.h"
#include "CanvasPanel.h"
#include "CanvasPanelSlot.h"

void UGsUITrayTickerAlertSection::NativeOnInitialized()
{
	UGsUITray::NativeOnInitialized();
}

void UGsUITrayTickerAlertSection::SetData(const FText& InText, NoticeType noticeType, SectionPriority priority, float InPlayTime, TrayCondition condition)
{
	auto data = MakeShared<FGsTickerMessageDataEx>(InText, InPlayTime, 0, condition);
	data->_type = noticeType;

	_messageData.Add(data);
	_alert->SetData(data, priority);
}

void UGsUITrayTickerAlertSection::NativeConstruct()
{
	Super::NativeConstruct();

	_closed = false;
	_alert->_onStartProgress = [this](EGsNotiSection sec, TWeakPtr<FGsTickerMessageDataEx> start) {
				_runMessges.Emplace(sec, start);
			};

	_alert->_onCompleteProgress = [this](EGsNotiSection sec, TWeakPtr<FGsTickerMessageDataEx> end) {

				if (auto iter = _runMessges.Find(sec))
				{
					if (*iter == end) _runMessges.Remove(sec);
				}
				_messageData.Remove(end.Pin());
			};

	_alert->InitSection();
}

void UGsUITrayTickerAlertSection::NativeDestruct()
{
	_closed = true;
	_runMessges.Reset();
	_messageData.Reset();

	_alert->_onStartProgress = nullptr;
	_alert->_onCompleteProgress = nullptr;
	_alert->EndSection();

	Super::NativeDestruct();
}


void UGsUITrayTickerAlertSection::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (_messageData.Num() <= 0)
	{
		Close(true);
	}
}

void UGsUITrayTickerAlertSection::Close(bool bImmediate, bool bRevcoverPrevStack)
{
	if (_closed) return;

	Super::Close(bImmediate, bRevcoverPrevStack);
}


void UGsUITrayTickerAlertSection::SetHideVisibility()
{
	_alert->ForcedConditionHide();

	Super::SetHideVisibility();
}

void UGsUITrayTickerAlertSection::RestoreShowVisibility()
{
	_messageData.RemoveAll([](auto e) { return e->_condition == TrayCondition::TC_Close;});
	_runMessges.Empty();
	
	_alert->ForcedConditionShow();
	Super::RestoreShowVisibility();
}
