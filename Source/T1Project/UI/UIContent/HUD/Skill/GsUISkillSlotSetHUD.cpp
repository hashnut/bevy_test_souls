// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUISkillSlotSetHUD.h"
#include "Components/ScrollBox.h"
#include "Components/PanelWidget.h"
#include "Components/WidgetSwitcher.h"
#include "UI/UIContent/HUD/Skill/GsUISkillSlotPageHUD.h"
#include "UI/UIContent/HUD/Skill/GsUISkillButtonBase.h"
#include "Management/GsMessageHolder.h"
#include "Management/ScopeGlobal/GsSkillManager.h"
#include "Skill/GsSkill.h"


void UGsUISkillSlotSetHUD::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	_scroll->OnUserScrolled.AddDynamic(this, &UGsUISkillSlotSetHUD::OnScrolledEvent);

	_pageList.Empty();
	for (int32 i = 0, maxCount = _panelRootPage->GetChildrenCount(); i < maxCount; ++i)
	{
		if (UGsUISkillSlotPageHUD* page = Cast<UGsUISkillSlotPageHUD>(_panelRootPage->GetChildAt(i)))
		{
			page->InitializePage(i);
			_pageList.Emplace(page);
		}
	}

	_pageIconList.Empty();
	for (int32 i = 0, maxCount = _panelRootPageIcon->GetChildrenCount(); i < maxCount; ++i)
	{
		if (UWidgetSwitcher* pageIcon = Cast<UWidgetSwitcher>(_panelRootPageIcon->GetChildAt(i)))
		{
			_pageIconList.Emplace(pageIcon);
		}
	}
}

void UGsUISkillSlotSetHUD::NativeConstruct()
{
	Super::NativeConstruct();

	for (int i = 0; i < _pageList.Num(); ++i)
	{
		if (_pageList[i].IsValid())
		{
			// 처음과 끝 페이지는 무한 스크롤 연출을 위한 가짜 페이지
			if (0 == i ||
				(_pageList.Num() - 1) == i)
			{
				_pageList[i]->SetIsFakePage(true);
			}
			else
			{
				_pageList[i]->SetIsFakePage(false);
			}
		}
	}

	// 이게 켜져있으면 좌표 꼬인다!
	_scroll->SetAllowOverscroll(false);

	_currPage = 1;
	_bIsScrolling = false;
	_startOffset = 0.f;
	_maxPage = _pageList.Num() - 2; // 앞, 뒤의 두 페이지를 뺀 값(실제 페이지 수)
	_backOffset = (_unitOffset * (_maxPage + 1)) - _pageChangeOffset;
	_minOffsetLock = 0.f;
	_maxOffsetLock = 0.f;

	_scroll->SetScrollOffset(_unitOffset);
	_destOffset = _unitOffset;

	SetPageIcon(_currPage);
}

void UGsUISkillSlotSetHUD::NativeDestruct()
{
	ClearAutoScrollTimer();

	_tutorialSlotId = -1;
	_tutorialOffset = 0.f;

	Super::NativeDestruct();
}

void UGsUISkillSlotSetHUD::UpdateManagerTick(float InDeltaTime)
{
	if (_bIsScrolling)
	{
		if (false == _scroll->HasMouseCapture())
		{
			OnScrollFinished();
		}
	}

	for (TWeakObjectPtr<UGsUISkillSlotPageHUD> page : _pageList)
	{
		if (page.IsValid())
		{
			page->UpdateManagerTick(InDeltaTime);
		}
	}
}

void UGsUISkillSlotSetHUD::OnKeyboardPress(int32 InSlotId)
{
	DefSkillSlotList slotList;
	FindSlots(InSlotId, slotList);

	for (UGsUISkillButtonBase* slot : slotList)
	{
		slot->OnKeyboardPress();
	}
}

void UGsUISkillSlotSetHUD::OnKeyboardRelease(int32 InSlotId)
{
	DefSkillSlotList slotList;
	FindSlots(InSlotId, slotList);

	for (UGsUISkillButtonBase* slot : slotList)
	{
		slot->OnKeyboardRelease();
	}
}

void UGsUISkillSlotSetHUD::OnScrolledEvent(float CurrentOffset)
{
	if (IsTutorial())
	{
		_scroll->SetScrollOffset(_tutorialOffset);
		return;
	}

	if (false == _scroll->HasMouseCapture())
	{
		OnScrollFinished();
		return;
	}

	if (false == _bIsScrolling)
	{
		OnScrollStarted();
	}

	// 스크롤 락
	if (CurrentOffset < _minOffsetLock)
	{
		_scroll->SetScrollOffset(_minOffsetLock);
	}
	else if (CurrentOffset > _maxOffsetLock)
	{
		_scroll->SetScrollOffset(_maxOffsetLock);
	}
}

void UGsUISkillSlotSetHUD::OnScrollStarted()
{
	_bIsScrolling = true;
	_startOffset = _destOffset; // 이전 마지막 위치

	int32 index = GetIndexByOffset(_startOffset);
	_minOffsetLock = (index - 1) * _unitOffset;
	_maxOffsetLock = (index + 1) * _unitOffset;

	ClearAutoScrollTimer();

	// close skill target select
	GSkill()->SetIsShowSkillTargetSelect(false);
}

void UGsUISkillSlotSetHUD::OnScrollFinished()
{
	// 스크롤 힘이 남아 느리게 이동하다 자동 스크롤로 빨라지는 현상 방지
	_scroll->EndInertialScrolling();

	_bIsScrolling = false;

	float currOffset = _scroll->GetScrollOffset();
	int32 index = GetIndexByOffset(currOffset);	
	float dragDist = currOffset - _startOffset;

	if (_pageSnapOffset < FMath::Abs(dragDist))
	{
		// 페이지가 변하지 않았을 때만
		int32 startIndex = GetIndexByOffset(_startOffset);
		if (startIndex == index)
		{
			// 드래그 방향(->)
			if (0 < dragDist)
			{
				index += 1;
			}
		}
	}
	else
	{
		// 드래그 거리를 안볼 경우, 페이지의 반을 넘어갔는지 체크
		float pageOffset = index * _unitOffset;		
		float pageOffsetCenter = pageOffset + _unitOffset * 0.5f;

		if (pageOffsetCenter < currOffset)
		{
			index += 1;
		}
	}

	_currPage = GetPageByIndex(index);
	SetPageIcon(_currPage);

	// 스냅 목표 위치 설정
	_destOffset = _currPage * _unitOffset;

	// 무한스크롤로 페이지가 넘어갈 경우, 해당 위치로 옮겨두고 자동 스크롤 시작
	if (_unitOffset < FMath::Abs(_destOffset - currOffset))
	{
		if (0 == index)
		{
			_scroll->SetScrollOffset((_maxPage * _unitOffset) + currOffset);
		}
		else if (_maxPage < index)
		{
			float diff = currOffset - _backOffset;
			_scroll->SetScrollOffset(_unitOffset + diff - _pageChangeOffset);
		}
	}

	// 자동 스크롤 타이머 설정. 60프레임 가정으로 0.016초 세팅
	ClearAutoScrollTimer();
	GetWorld()->GetTimerManager().SetTimer(_autoScrollTimer, this, &UGsUISkillSlotSetHUD::OnAutoScrollTimer, 0.016f, true);
}

void UGsUISkillSlotSetHUD::OnAutoScrollTimer()
{
	float currOffset = _scroll->GetScrollOffset();
	bool bMovePositive = (currOffset < _destOffset) ? true : false;

	// 스크롤 방향에 따른 처리
	if (bMovePositive)
	{
		currOffset += (_autoScrollSpeed * 0.016f);
		if (currOffset >= _destOffset)
		{
			OnEndAutoScroll();
			return;
		}
	}
	else
	{
		currOffset -= (_autoScrollSpeed * 0.016f);
		if (currOffset <= _destOffset)
		{
			OnEndAutoScroll();
			return;
		}
	}

	_scroll->SetScrollOffset(currOffset);
}

void UGsUISkillSlotSetHUD::OnEndAutoScroll()
{
	ClearAutoScrollTimer();

	_scroll->SetScrollOffset(_destOffset);
}

void UGsUISkillSlotSetHUD::ClearAutoScrollTimer()
{
	if (_autoScrollTimer.IsValid())
	{
		GetWorld()->GetTimerManager().ClearTimer(_autoScrollTimer);
	}
}

int32 UGsUISkillSlotSetHUD::GetPageByIndex(int32 InIndex)
{
	if (0 == InIndex)
	{
		return _maxPage;
	}
	else if (_maxPage < InIndex)
	{
		return 1;
	}

	return InIndex;
}

int32 UGsUISkillSlotSetHUD::GetIndexByOffset(float InOffset)
{
	if (_unitOffset > InOffset)
	{
		return 0;
	}

	return FMath::FloorToInt(InOffset / _unitOffset);
}

void UGsUISkillSlotSetHUD::SetPageIcon(int32 InPage)
{
	int32 index = InPage - 1;

	for (int32 i = 0; i < _pageIconList.Num(); ++i)
	{
		if (_pageIconList[i].IsValid())
		{
			_pageIconList[i]->SetActiveWidgetIndex((i == index) ? 1 : 0);
		}
	}
}

void UGsUISkillSlotSetHUD::FindSlots(int32 InSlotId, OUT DefSkillSlotList& OutList)
{
	for (TWeakObjectPtr<UGsUISkillSlotPageHUD> page : _pageList)
	{
		if (page.IsValid())
		{
			if (UGsUISkillButtonBase* slot = page->GetSlot(InSlotId))
			{
				OutList.Emplace(slot);
			}
		}
	}
}

void UGsUISkillSlotSetHUD::FindSlotsBySkillId(SkillId InSkillId, OUT DefSkillSlotList& OutList)
{
	for (TWeakObjectPtr<UGsUISkillSlotPageHUD> page : _pageList)
	{
		if (page.IsValid())
		{
			if (UGsUISkillButtonBase* slot = page->GetSlotBySkillId(InSkillId))
			{
				OutList.Emplace(slot);
			}
		}
	}
}

void UGsUISkillSlotSetHUD::UpdateAllSlots()
{
	UGsSkillManager* skillMgr = GSkill();
	if (nullptr == skillMgr)
	{
		return;
	}	

	DefSkillSlotList slotList;
	for (int32 i = 1; i <= MAX_COSTUME_SKILL_SLOT_ID; ++i)
	{
		slotList.Empty();
		FindSlots(i, slotList);

		if (const FGsSkill* skill = skillMgr->FindSkillSlot(i))
		{
			for (UGsUISkillButtonBase* slot : slotList)
			{
				slot->SetData(skill);
			}
		}
		else
		{
			for (UGsUISkillButtonBase* slot : slotList)
			{
				slot->SetEmptySlot();
			}
		}
	}
}

UGsUISkillButtonBase* UGsUISkillSlotSetHUD::SetTutorialTarget(int32 InSlotId)
{
	_tutorialSlotId = InSlotId;
	if (0 <= _tutorialSlotId)
	{
		// 스크롤 막기
		// 강제 페이지 세팅
		int32 tempCurrPage = 1;
		if (MAX_ACTIVE_SKILL_SLOT_ID < InSlotId && MAX_COSTUME_SKILL_SLOT_ID > InSlotId)
		{
			tempCurrPage = 2;
		}

		if (_pageList.IsValidIndex(tempCurrPage))
		{
			TWeakObjectPtr< UGsUISkillSlotPageHUD> page = _pageList[tempCurrPage];

			if (page.IsValid())
			{
				if (UGsUISkillButtonBase* button = page->GetSlot(InSlotId))
				{
					// 슬롯이 비어있으면 false
					if (false == button->IsEmpty())
					{
						_currPage = tempCurrPage;

						_tutorialOffset = _unitOffset * _currPage;

						_bIsScrolling = false;
						_startOffset = 0.f;

						_scroll->SetScrollOffset(_tutorialOffset);
						_destOffset = _tutorialOffset;

						SetPageIcon(_currPage);

						return button;
					}
				}
			}
		}
	}

	_tutorialOffset = 0.f;

	return nullptr;
}
