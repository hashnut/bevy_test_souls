// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUIReuseScroll.h"
#include "UTIL/GsTimeUtil.h"
#include "SBoxPanel.h"
#include "Management/GsMessageHolder.h"
#include "Components/HorizontalBox.h"
#include "Components/CanvasPanel.h"
#include "Components/ScrollBox.h"
#include "Components/VerticalBox.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/WidgetSwitcher.h"
#include "UI/UIControlLib/Control/GsDynamicIconSlotHelper.h"
#include "UI/UIControlLib/Control/GsDynamicPanelSlotHelper.h"
#include "UI/UIControlLib/ContentWidget/GsWrapBoxIconSelector.h"
#include "UI/UIContent/Common/ReuseIconSlot/GsUIReuseItemIconSlotBase.h"
#include "UI/UIContent/Common/ReuseIconSlot/GsUIReuseItemIconSlotHud.h"
#include "UI/UIContent/Common/ReuseIconSlot/GsUIReuseItemIconSlotSimple.h"
#include "Management/ScopeGame/GsQuickSlotManager.h"


void UGsUIReuseScroll::BeginDestroy()
{
	_switcherPanelList.Empty();
	_pageIndexPosMap.Empty();

	_endScrollOffset = 0.f;

	Super::BeginDestroy();
}

void UGsUIReuseScroll::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	_switcherPanelList.Emplace(_firstPageSwitcher);
	_switcherPanelList.Emplace(_secondPageSwitcher);
	_switcherPanelList.Emplace(_thirdPageSwitcher);
	_switcherPanelList.Emplace(_fourthPageSwitcher);
	_switcherPanelList.Emplace(_fifthPageSwitcher);
	_switcherPanelList.Emplace(_sixthPageSwitcher);

	_timer = NewObject<UGsSimpleTimer>();
	float min;
	float max;
	_curve->GetTimeRange(min, max);
	_timer->SetMaxTime(max);

	_timer->Stop();
	_timer->Reset();

	_scrollBox->OnUserScrolled.AddDynamic(this, &UGsUIReuseScroll::OnChangeMainScrollEvent);
}

void UGsUIReuseScroll::NativeConstruct()
{
	Super::NativeConstruct();

	if (0.f >= _endScrollOffset)
	{
		_reserveTargetPageType = EGsIconSlotCountType::One;
		_curPageSwitcherType = EGsIconSlotCountType::One;
		_eSlotPageIndexType = EGsIconSlotCountType::One;

		_scrollBox->ScrollToEnd();
	}
	else
	{
		_eSlotPageIndexType = _curPageSwitcherType;
		_scrollBox->SetScrollOffset(GetSlotPos(_curPageSwitcherType));
		CheckPageCondition();
		InvalidateSwitcher();
	}
}

void UGsUIReuseScroll::NativeDestruct()
{
	_isScrolling = false;
	_isAutoScrolling = false;
	_isWindosInputAutoScroll = false;

	Super::NativeDestruct();
}

void UGsUIReuseScroll::UpdateManagerTick(float InDeltaTime)
{
	if (false == IsVisible())
	{
		return;
	}

	// 스크롤 end 되는 시점을 알 수 없기에 tick 처리에서 값을 가져와서 초기 세팅함
	if (0.f >= _endScrollOffset && 0.f < _scrollBox->GetScrollOffset())
	{
		_endScrollOffset = _scrollBox->GetScrollOffset();
		CreateEachPagePosition();
		_eSlotPageIndexType = _reserveTargetPageType;
		_curPageSwitcherType = _reserveTargetPageType;
		_scrollBox->SetScrollOffset(GetSlotPos(_eSlotPageIndexType));
		CheckPageCondition();
		InvalidateSwitcher();
	}

	// 스크롤 하다가 릴리즈 한 상태
	if (false == _scrollBox->HasMouseCapture() && true == _isScrolling)
	{
		// 다음틱에서..
		if (false == _timer->CanTick())
		{
			_timer->Reset();
			_timer->Start();
			return;
		}

		if (_timer->IsFinish())
		{
			_timer->Stop();
		}

		_timer->Tick(InDeltaTime);
		if (_curve)
		{
			float curveValue = _curve->GetFloatValue(_timer->GetCurrentTime());
			AutoScroll(curveValue);
		}
	}

	if (_isWindosInputAutoScroll)
	{
		// 다음틱에서..
		if (false == _timer->CanTick())
		{
			_timer->Reset();
			_timer->Start();
			return;
		}

		if (_timer->IsFinish())
		{
			_timer->Stop();
		}

		_timer->Tick(InDeltaTime);
		if (_curve)
		{
			float curveValue = _curve->GetFloatValue(_timer->GetCurrentTime());
			AutoScroll(curveValue);
		}
	}
}

void UGsUIReuseScroll::WindowskeyInputLeft()
{
	if (false == _isAutoScrolling)
	{
		_eScrollDirectionType = ScrollDircetionType::Scroll_Left;
		_isWindosInputAutoScroll = true;
	}
}

void UGsUIReuseScroll::WindowskeyInputRight()
{
	if (false == _isAutoScrolling)
	{
		_eScrollDirectionType = ScrollDircetionType::Scroll_Right;
		_isWindosInputAutoScroll = true;
	}
}

void UGsUIReuseScroll::CreateEachPagePosition()
{
	_pageIndexPosMap.Empty();

	float add = EGsIconSlotCountType::One == _maxSlotPageType? 1.f : (float)_maxSlotPageType;
	float calcSize = _endScrollOffset / add;
	for (uint8 i = 0; i < (uint8)_maxSlotPageType + 1; ++i)
	{
		float value = i * calcSize;
		_pageIndexPosMap.Emplace((EGsIconSlotCountType)i,value);
	}
}

void UGsUIReuseScroll::InvalidateSwitcher()
{
	if (ScrollDircetionType::Scroll_Left == _eScrollDirectionType)
	{
		_curPageSwitcherType = GetSwitcherPageType(false);
	}
	else if (ScrollDircetionType::Scroll_Right == _eScrollDirectionType)
	{
		_curPageSwitcherType = GetSwitcherPageType(true);
	}

	for (uint8 i = 0; i < _switcherPanelList.Num(); ++i)
	{
		if (i > (uint8)_maxSlotPageType)
		{
			break;
		}

		// 0 : on 1: off
		bool state = i == (uint8)_curPageSwitcherType;
		_switcherPanelList[i]->SetActiveWidgetIndex(!state);
	}

	if (EGsIconSlotType::HudType == _iconSlotType)
	{
		GSQuickSlot()->SetLastSlotPage((uint8)_curPageSwitcherType);
	}
}

EGsIconSlotCountType UGsUIReuseScroll::GetSwitcherPageType(bool InNextType)
{
	if ((true == InNextType) && (_maxSlotPageType == _curPageSwitcherType))
	{
		return  EGsIconSlotCountType::One;
	}

	switch (_curPageSwitcherType)
	{
	case EGsIconSlotCountType::One:
		return true == InNextType ? EGsIconSlotCountType::Two : _maxSlotPageType;
	case EGsIconSlotCountType::Two:
		return true == InNextType ? EGsIconSlotCountType::Three : EGsIconSlotCountType::One;
	case EGsIconSlotCountType::Three:
		return true == InNextType ? EGsIconSlotCountType::Four : EGsIconSlotCountType::Two;
	case EGsIconSlotCountType::Four:
		return true == InNextType ? EGsIconSlotCountType::Five : EGsIconSlotCountType::Three;
	case EGsIconSlotCountType::Five:
		return true == InNextType ? EGsIconSlotCountType::Six : EGsIconSlotCountType::Four;
	case EGsIconSlotCountType::Six:
		return true == InNextType ? EGsIconSlotCountType::One : EGsIconSlotCountType::Five;
	}

	return EGsIconSlotCountType::Two;
}

float UGsUIReuseScroll::GetSlotPos(EGsIconSlotCountType InCurSlotPageType)
{
	if (false == _pageIndexPosMap.Contains(InCurSlotPageType))
		return 0.f;

	return _pageIndexPosMap[InCurSlotPageType];
}

// InNextPage - true : nextPage false : beforePage
float UGsUIReuseScroll::MoveTargetPagePos(bool InNextPage)
{
	if (true == InNextPage)
	{
		uint8 nextIndex = (uint8)_eSlotPageIndexType + 1;
		return GetSlotPos((EGsIconSlotCountType)nextIndex);	
	}
	else
	{
		uint8 beforeIndex = (uint8)_eSlotPageIndexType - 1;
		return GetSlotPos((EGsIconSlotCountType)beforeIndex);	
	}

	return 0.f;
}

float UGsUIReuseScroll::GetSlotCenterPos(EGsIconSlotCountType InCurSlotPageType)
{
	if (false == _pageIndexPosMap.Contains(InCurSlotPageType))
		return 0.f;

	float value = EGsIconSlotCountType::One == _maxSlotPageType ? 1.f : (float)_maxSlotPageType;
	float calcSize = _endScrollOffset / value;
	float pos = _pageIndexPosMap[InCurSlotPageType];

	return pos + (calcSize / 2.f);
}

void UGsUIReuseScroll::CheckTargetSlot()
{
	float pagePos = GetSlotPos(_eSlotPageIndexType);
	float curPos = _scrollBox->GetScrollOffset();

	if (curPos >= pagePos) // 다음 페이지로 이동 가능한지 term 체크
	{
		float centerPos = GetSlotCenterPos(_eSlotPageIndexType);
		float check = 0.f >= _autoScrollValue ? 0.f : _autoScrollValue;
		float calc = centerPos - check;
		if (curPos >= calc) // 다음 페이지로 자동 이동 처리
		{
			_eScrollDirectionType = ScrollDircetionType::Scroll_Right;
		}
		else // 현재 페이지로 원복 처리
		{
			_eScrollDirectionType = ScrollDircetionType::Scroll_Return_Left;
		}
	}
	else // 이전 페이지로 이동 가능한지 term 체크
	{
		float beforeCenterPos = GetSlotCenterPos(GetPageType(false));
		float check = 0.f >= _autoScrollValue ? 0.f : _autoScrollValue;
		float calc = beforeCenterPos + check;
		if (curPos <= calc) // 이전 페이지로 자동 이동 처리
		{
			_eScrollDirectionType = ScrollDircetionType::Scroll_Left;
		}
		else // 현재 페이지로 원복 처리
		{
			_eScrollDirectionType = ScrollDircetionType::Scroll_Return_Right;
		}
	}
}

// 현재 포커싱된 페이지가 첫번째 혹은 마지막 페이지 일때
// 페이지를 옮기는 작업
void UGsUIReuseScroll::CheckPageCondition()
{
	if (EGsIconSlotCountType::One == _eSlotPageIndexType)
	{
		SetSlotMove((uint8)_maxSlotPageType, (uint8)EGsIconSlotCountType::One);
		_scrollBox->SetScrollOffset(GetSlotPos(EGsIconSlotCountType::Two));
		_eSlotPageIndexType = EGsIconSlotCountType::Two;
	}
	else if (_maxSlotPageType == _eSlotPageIndexType)
	{
		SetSlotMove((uint8)EGsIconSlotCountType::One, (uint8)_maxSlotPageType);
		_scrollBox->SetScrollOffset(GetSlotPos(GetPageType(_maxSlotPageType, false)));
		_eSlotPageIndexType = GetPageType(_maxSlotPageType, false);
	}
	else
	{	
		if (ScrollDircetionType::Scroll_Left == _eScrollDirectionType)
		{
			if (EGsIconSlotCountType::Two == _eSlotPageIndexType)
			{
				SetSlotMove((uint8)_maxSlotPageType, (uint8)EGsIconSlotCountType::One);
				_scrollBox->SetScrollOffset(GetSlotPos(EGsIconSlotCountType::Two));
				return;
			}
			else
			{
				_eSlotPageIndexType = GetPageType(false);
			}
		}
		if (ScrollDircetionType::Scroll_Right == _eScrollDirectionType)
		{
			if (GetPageType(_maxSlotPageType, false) == _eSlotPageIndexType)
			{
				SetSlotMove((uint8)EGsIconSlotCountType::One, (uint8)_maxSlotPageType);
				_scrollBox->SetScrollOffset(GetSlotPos(GetPageType(_maxSlotPageType, false)));
				return;
			}
			else
			{
				_eSlotPageIndexType = GetPageType(true);
			}
		}		
	}
}

void UGsUIReuseScroll::OnFinishedTargetPos()
{
	switch (_eScrollDirectionType)
	{
	case UGsUIReuseScroll::Scroll_Return_Left:
		break;
	case UGsUIReuseScroll::Scroll_Return_Right:
		break;
	case UGsUIReuseScroll::Scroll_Left:
	{
		CheckPageCondition();
		break;
	}
	case UGsUIReuseScroll::Scroll_Right:
	{
		CheckPageCondition();
		break;
	}
	}

	_isScrolling = false;
	_isAutoScrolling = false;
	_isWindosInputAutoScroll = false;
	_timer->Stop();

	// 순서 변경 ㄴㄴ
	InvalidateSwitcher();
	_eScrollDirectionType = ScrollDircetionType::Scroll_None;

#if WITH_EDITOR
	GSLOG(Warning, TEXT("UGsUIReuseScroll::OnFinishedTargetPos() - curPos : %f"), _scrollBox->GetScrollOffset());
#endif
}

void UGsUIReuseScroll::MoveNextScroll(float InCurveData)
{
	float curPos = _scrollBox->GetScrollOffset();
	float targetPos = MoveTargetPagePos(true);
	if (targetPos > curPos)
	{
		_scrollBox->SetScrollOffset(curPos + InCurveData);
	}
	else
	{
		_scrollBox->SetScrollOffset(targetPos);
		OnFinishedTargetPos();	
	}
}

void UGsUIReuseScroll::MoveBeforeScroll(float InCurveData)
{
	float curPos = _scrollBox->GetScrollOffset();
	float targetPos = MoveTargetPagePos(false);
	if (targetPos < curPos)
	{
		_scrollBox->SetScrollOffset(curPos - InCurveData);
	}
	else
	{
		_scrollBox->SetScrollOffset(targetPos);
		OnFinishedTargetPos();		
	}
}

void UGsUIReuseScroll::MoveRetrunLeftScroll(float InCurveData)
{
	float targetPos = GetSlotPos(_eSlotPageIndexType);
	float curPos = _scrollBox->GetScrollOffset();
	if (curPos > targetPos)
	{
		_scrollBox->SetScrollOffset(curPos - InCurveData);
	}
	else // 도착했으면 강제 setOffset
	{
		_isScrolling = false;
		_isAutoScrolling = false;
		_timer->Stop();
		_scrollBox->SetScrollOffset(targetPos);
		_eScrollDirectionType = ScrollDircetionType::Scroll_None;
	}
}

void UGsUIReuseScroll::MoveRetrunRightScroll(float InCurveData)
{
	float targetPos = GetSlotPos(_eSlotPageIndexType);
	float curPos = _scrollBox->GetScrollOffset();
	if (curPos < targetPos)
	{
		_scrollBox->SetScrollOffset(curPos + InCurveData);
	}
	else // 도착했으면 강제 setOffset
	{
		_isScrolling = false;
		_isAutoScrolling = false;
		_timer->Stop();
		_scrollBox->SetScrollOffset(targetPos);
		_eScrollDirectionType = ScrollDircetionType::Scroll_None;
	}
}

void UGsUIReuseScroll::AutoScroll(float InCurveData)
{
	_isAutoScrolling = true;
	switch (_eScrollDirectionType)
	{
	case UGsUIReuseScroll::Scroll_None:
		_isAutoScrolling = false;
		break;
	case UGsUIReuseScroll::Scroll_Return_Left:
		MoveRetrunLeftScroll(InCurveData);
		break;
	case UGsUIReuseScroll::Scroll_Return_Right:
		MoveRetrunRightScroll(InCurveData);
		break;
	case UGsUIReuseScroll::Scroll_Left:
		MoveBeforeScroll(InCurveData);
		break;
	case UGsUIReuseScroll::Scroll_Right:
		MoveNextScroll(InCurveData);
		break;

	}
}

void UGsUIReuseScroll::OnChangeMainScrollEvent(float InCurOffset)
{
	if (true == _scrollBox->HasMouseCapture())
	{
		if (true == _isAutoScrolling)
			return;

		_isScrolling = true;
		_timer->Stop();

		CheckTargetSlot();
	}
	else
	{
		float curveValue = _curve->GetFloatValue(_timer->GetCurrentTime());
		AutoScroll(curveValue);
		if (false == _isAutoScrolling && false == _isScrolling)
		{
			// targetPos 로 강제로 set 처리한다
			float targetPos = GetSlotPos(_eSlotPageIndexType);
			_scrollBox->SetScrollOffset(targetPos);
		}
	}
}

EGsIconSlotCountType UGsUIReuseScroll::GetPageType(bool InNextType)
{
	switch (_eSlotPageIndexType)
	{
	case EGsIconSlotCountType::One:
		return true == InNextType ? EGsIconSlotCountType::Two : _maxSlotPageType;
	case EGsIconSlotCountType::Two:
		return true == InNextType ? EGsIconSlotCountType::Three : EGsIconSlotCountType::One;
	case EGsIconSlotCountType::Three:
		return true == InNextType ? EGsIconSlotCountType::Four : EGsIconSlotCountType::Two;
	case EGsIconSlotCountType::Four:
		return true == InNextType ? EGsIconSlotCountType::Five : EGsIconSlotCountType::Three;
	case EGsIconSlotCountType::Five:
		return true == InNextType ? EGsIconSlotCountType::Six : EGsIconSlotCountType::Four;
	case EGsIconSlotCountType::Six:
		return true == InNextType ? EGsIconSlotCountType::One : EGsIconSlotCountType::Five;
	}

	return EGsIconSlotCountType::Two;
}

EGsIconSlotCountType UGsUIReuseScroll::GetPageType(EGsIconSlotCountType InType, bool InNextType)
{
	switch (InType)
	{
	case EGsIconSlotCountType::One:
		return true == InNextType ? EGsIconSlotCountType::Two : _maxSlotPageType;
	case EGsIconSlotCountType::Two:
		return true == InNextType ? EGsIconSlotCountType::Three : EGsIconSlotCountType::One;
	case EGsIconSlotCountType::Three:
		return true == InNextType ? EGsIconSlotCountType::Four : EGsIconSlotCountType::Two;
	case EGsIconSlotCountType::Four:
		return true == InNextType ? EGsIconSlotCountType::Five : EGsIconSlotCountType::Three;
	case EGsIconSlotCountType::Five:
		return true == InNextType ? EGsIconSlotCountType::Six : EGsIconSlotCountType::Four;
	case EGsIconSlotCountType::Six:
		return true == InNextType ? EGsIconSlotCountType::One : EGsIconSlotCountType::Five;
	}

	return EGsIconSlotCountType::Two;
}