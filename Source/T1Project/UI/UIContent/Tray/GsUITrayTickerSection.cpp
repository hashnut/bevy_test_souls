#include "GsUITrayTickerSection.h"
#include "NotiMessage/GsUISectionMessageBase.h"
#include "NotiMessage/GsUISectionMessageSlide.h"
#include "CanvasPanel.h"
#include "CanvasPanelSlot.h"
#include "Management/ScopeGlobal/GsUIManager.h"

TMap<EGsNotiSection, FString> UGsUITrayTickerSection::_sectionTag;

void UGsUITrayTickerSection::SetData(EGsNotiSection sectionType, const FText& InText, 
	SectionPriority priority, float InPlayTime, TrayCondition condition)
{
	if (auto iter = _sections.Find(sectionType))
	{
		UGsUISectionMessageBase* section = *iter;
		FText outText = ParseTrayText(InText, sectionType);

		auto data = MakeShared<FGsTickerMessageDataEx>(outText, InPlayTime, 0, condition);
		_messageData.Add(data);
		section->SetData(data, priority);
	}
}

void UGsUITrayTickerSection::SetDelayData(EGsNotiSection sectionType, const FText& InText,
	SectionPriority priority, float InPlayTime, float DelayTime, TrayCondition condition) //, EGsTickerAnimationType InAnimType
{
	if (auto iter = _sections.Find(sectionType))
	{
		UGsUISectionMessageBase* section = *iter;
		FText outText = ParseTrayText(InText, sectionType);

		auto data = MakeShared<FGsTickerMessageDataEx>(outText, InPlayTime, DelayTime, condition);
		_messageData.Add(data);
		section->SetData(data, priority);
	}
}

void UGsUITrayTickerSection::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	_maxWidth = 0.f;
	//_alert->SetVisibility(ESlateVisibility::Collapsed);

	for (auto section : { _section1, _section2, _section3 }) //, _section4
	{
		_sections.Add(section->GetSectionType(), section);
	}

	auto slots = _panel->GetSlots();

	TArray<UWidget*> childList = _panel->GetAllChildren();
	for (auto child : childList)
	{
		if (child->IsA<UGsUISectionMessageBase>())
		{
			auto index = _panel->GetChildIndex(child);

			if (slots.IsValidIndex(index))
			{
				auto section = Cast<UGsUISectionMessageBase>(child);
				_unitSlots.Add(section->GetSectionType(), Cast<UCanvasPanelSlot>(slots[index]));
			}
		}
	}
}

void UGsUITrayTickerSection::BeginDestroy()
{
	_sections.Empty();
	_unitSlots.Empty();

	Super::BeginDestroy();
}

void UGsUITrayTickerSection::NativeConstruct()
{
	Super::NativeConstruct();

	_closed = false;
	for (auto message : _sections)
	{
		if (auto slotIter = _unitSlots.Find(message.Key))
		{
			message.Value->_onStartProgress = [this](EGsNotiSection sec, TWeakPtr<FGsTickerMessageDataEx> start) {
				_runMessges.Emplace(sec, start);
			};

			message.Value->_onCompleteProgress = [this](EGsNotiSection sec, TWeakPtr<FGsTickerMessageDataEx> end) {

				if (auto iter = _runMessges.Find(sec))
				{
					if (*iter == end) _runMessges.Remove(sec);
				}
				_messageData.Remove(end.Pin());
			};

			message.Value->InitSection();
		}
	}
}

void UGsUITrayTickerSection::NativeDestruct()
{
	_closed = true;
	_runMessges.Reset();
	_messageData.Reset();
	for (auto message : _sections)
	{
		if (auto slotIter = _unitSlots.Find(message.Key))
		{
			message.Value->_onStartProgress = nullptr;
			message.Value->_onCompleteProgress = nullptr;
			message.Value->EndSection();
		}
	}

	Super::NativeDestruct();
}


void UGsUITrayTickerSection::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (_messageData.Num() <= 0)
	{
		Close(true);
	}

	if (_maxWidth <= 0.f)
	{
		_maxWidth = _maxRange->GetDesiredSize().X - 100.f;

		if (_maxWidth > 0)
		{
			for (auto message : _sections)
			{
				if (auto slotIter = _unitSlots.Find(message.Key))
				{
					message.Value->InitSection(_maxWidth, *slotIter);
				}
			}
		}
	}
}

void UGsUITrayTickerSection::Close(bool bImmediate, bool bRevcoverPrevStack)
{
	if (_closed) return;
	
	Super::Close(bImmediate, bRevcoverPrevStack);
}

FText UGsUITrayTickerSection::ParseTrayText(const FText& inText, EGsNotiSection sectionType)
{
	FString sectionHeader;
	if (auto headIter = _sectionTag.Find(sectionType)) sectionHeader = *headIter;

	auto resultText = inText.ToString();

	if (sectionType != EGsNotiSection::ALERT)
	{
		resultText = resultText.Replace(TEXT("\n"), TEXT(" "));
	}

	if (sectionHeader.IsEmpty()) return FText::FromString(resultText);

	TArray<FString> procStr;
	resultText.ParseIntoArray(procStr, TEXT("</>"), false); //위치정보

	if (procStr.Num() < 1)
	{
		return FText::FromString(sectionHeader + resultText + TEXT("</>"));
	}

	resultText.Empty();
	for (auto str : procStr)
	{
		if (str.IsEmpty()) continue;

		FString procText = str;
		FString secDefText = str;
		int pos = 0;

		while(!procText.IsEmpty() && !procText.StartsWith(TEXT("<")))
		{
			procText = procText.Right(str.Len() - pos);
			pos++;
		}

		if (pos >= 0)
		{
			resultText += (sectionHeader + str.Left(pos - 1) + TEXT("</>"));
			if(!procText.IsEmpty()) resultText += (procText + TEXT("</>"));
		}
		else
		{
			resultText += (sectionHeader + str + TEXT("</>"));
		}
	}

	return FText::FromString(resultText);
}

void UGsUITrayTickerSection::SetHideVisibility()
{
	_prevState = GUI()->GetTickerState();
	for (auto e : _sections)
	{
		e.Value->ForcedConditionHide(_prevState);
	}

	Super::SetHideVisibility();
}

void UGsUITrayTickerSection::RestoreShowVisibility()
{
	if (_prevState == FGsUITickerHandler::TrayWidgetState::AllHide ||
		_prevState == FGsUITickerHandler::TrayWidgetState::Private)
	{
		_messageData.RemoveAll([](auto e) { return e->_condition == TrayCondition::TC_Close; });
		_runMessges.Empty();
	}

	for (auto e : _sections)
	{
		e.Value->ForcedConditionShow(_prevState);
	}

	_prevState = FGsUITickerHandler::TrayWidgetState::None;

	Super::RestoreShowVisibility();
}
