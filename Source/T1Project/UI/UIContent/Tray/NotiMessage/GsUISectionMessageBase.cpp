// Fill out your copyright notice in the Description page of Project Settings.
#include "UI/UIContent/Tray/NotiMessage/GsUISectionMessageBase.h"
#include "Management/ScopeGlobal/GsUIManager.h"
#include "CanvasPanel.h"
#include "CanvasPanelSlot.h"
#include "RichTextBlock.h"
#include "Border.h"

void UGsUISectionMessageBase::NativeConstruct()
{
	Super::NativeConstruct();

	if (_border)
	{
		_border->SetVisibility(ESlateVisibility::Hidden);
	}

	_richTextBlock->SetText(FText::GetEmpty());

	_taskManager._taskFinished = [this]() {

		_richTextBlock->SetText(FText::GetEmpty());
		_border->SetVisibility(ESlateVisibility::Collapsed);
	};
}

void UGsUISectionMessageBase::NativeDestruct()
{
	_taskManager._taskFinished = nullptr;
	Super::NativeDestruct();
}

void UGsUISectionMessageBase::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	_richTextBlock->SetTextStyleSet(TextStyleSet);
	if (bOverrideDefaultStyle)
		_richTextBlock->SetDefaultTextStyle(DefaultTextStyleOverride);
}

void UGsUISectionMessageBase::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (_initSection)
	{
		_taskManager.doTasks(InDeltaTime);
	}
}

void UGsUISectionMessageBase::OnClearWork()
{
	_taskManager.clearTask(true);
}

void UGsUISectionMessageBase::InitSection(float maxRange, UCanvasPanelSlot* panelSlot)
{
	InitSection();
}

void UGsUISectionMessageBase::InitSection()
{
	_initSection = true;
}

void UGsUISectionMessageBase::EndSection()
{
	_initSection = false;
	OnClearWork();
	_workQueue.Reset();
}

bool UGsUISectionMessageBase::IsRunningSection()
{
	if (_workQueue.IsEmpty() && _taskManager._taskQueue.IsEmpty() && !_taskManager.runTask().IsValid())
		return true;
	return false;
}

void UGsUISectionMessageBase::SetData(TWeakPtr<FGsTickerMessageDataEx> data, SectionPriority priority /*= SectionPriority::None*/)
{
	if (!data.IsValid()) return; //|| !_initSection

	if (priority == SectionPriority::None)
	{
		_workQueue.PushBack(data);
		OnInitWork(data);
	}
	else if (priority == SectionPriority::Instant)
	{
		for (auto e : _workQueue)
		{
			if (_onCompleteProgress)
			{
				_onCompleteProgress(_section, e);
			}
		}
		_workQueue.Empty();
		_workQueue.PushBack(data);

		OnClearWork();
		OnInitWork(data);
	}
	else if (priority == SectionPriority::Insert)
	{
		_workQueue.PushFront(data);

		OnClearWork();
		for (auto e : _workQueue)
		{
			OnInitWork(e);
		}
	}
}

void UGsUISectionMessageBase::ForcedConditionHide(FGsUITickerHandler::TrayWidgetState curvState)
{
	if (curvState == FGsUITickerHandler::TrayWidgetState::AllHide ||
		curvState == FGsUITickerHandler::TrayWidgetState::Private)
	{
		OnClearWork();
	}
}

void UGsUISectionMessageBase::ForcedConditionShow(FGsUITickerHandler::TrayWidgetState prevState) //강제 hide상태일때 추가되어 있던 CloseMessage들을 삭제하고 정상화
{
	if (prevState == FGsUITickerHandler::TrayWidgetState::AllHide ||
		prevState == FGsUITickerHandler::TrayWidgetState::Private)
	{
		if (_workQueue.IsEmpty()) return;
		_workQueue.RemoveAll([](auto e) { return !e.IsValid() || e.Pin()->_condition == TrayCondition::TC_Close;});

		OnClearWork();
		for (auto e : _workQueue)
		{
			this->OnInitWork(e);
		}
	}
}
