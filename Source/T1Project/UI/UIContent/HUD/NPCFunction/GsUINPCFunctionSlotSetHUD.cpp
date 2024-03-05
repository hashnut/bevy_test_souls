#include "GsUINPCFunctionSlotSetHUD.h"

#include "Components/ScrollBox.h"
#include "Components/WidgetSwitcher.h"
#include "UMG/Public/Components/PanelWidget.h"

#include "SlateCore/Public/Layout/Geometry.h"

#include "Data/GsDataContainManager.h"
#include "Data/GsGlobalConstant.h"

#include "DataCenter/Public/Shared/Client/SharedEnums/SharedCreatureEnum.h"

#include "Runtime/DataCenter/Public/DataSchema/GameObject/Npc/GsSchemaNpcData.h"
#include "Runtime/DataCenter/Public/DataSchema/GameObject/Npc/GsSchemaNpcFunctionData.h"

#include "Management/ScopeGame/GsInteractionManager.h"
#include "Management/ScopeGame/GsGameObjectManager.h"

#include "GameObject/Interaction/GsInteractionNpcInfo.h"
#include "GameObject/ObjectClass/GsGameObjectBase.h"

#include "AI/Function/GsAIFunc.h"

#include "UI/UIContent/HUD/NPCFunction/GsUINPCFunctionSlotPageHUD.h"
#include "UI/UIContent/HUD/NPCFunction/GsUINPCFunctionButton.h"
#include "UI/UIContent/HUD/NPCFunction/GsUINPCFunctionSlotMenuList.h"



void UGsUINPCFunctionSlotSetHUD::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	_scroll->OnUserScrolled.AddDynamic(this, &UGsUINPCFunctionSlotSetHUD::OnScrolledEvent);
	
	_pageList.Empty();
	

	for (int32 i = 0, maxCount = _panelRootPage->GetChildrenCount(); i < maxCount; ++i)
	{
		if (UGsUINPCFunctionSlotPageHUD* page = Cast<UGsUINPCFunctionSlotPageHUD>(_panelRootPage->GetChildAt(i)))
		{
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

void UGsUINPCFunctionSlotSetHUD::NativeConstruct()
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
	_maxPage = _pageList.Num() - 2; // 앞, 뒤의 두 페이지를 뺀 값	
	_backOffset = (_unitOffset * (_maxPage + 1)) - _pageChangeOffset;
	_minOffsetLock = 0.f;
	_maxOffsetLock = 0.f;

	_scroll->SetScrollOffset(_unitOffset);
	_destOffset = _unitOffset;

	SetPageIcon(_currPage);

	ResetMenuCheckValue();
}

void UGsUINPCFunctionSlotSetHUD::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (_bIsScrolling)
	{
		if (false == _scroll->HasMouseCapture())
		{
			OnScrollFinished();
		}
	}

	if (_menuTarget.IsValid())
	{
		CalcMenuPos();
	}
}

void UGsUINPCFunctionSlotSetHUD::OnKeyboardPress(int32 InSlotId)
{
	DefNPCFunctionSlotList slotList;
	FindSlots(InSlotId, slotList);

	for (UGsUINPCFunctionButton* slot : slotList)
	{
		if (slot)
		{
			slot->OnKeyboardPress();
		}
	}
}

void UGsUINPCFunctionSlotSetHUD::OnKeyboardRelease(int32 InSlotId)
{
	DefNPCFunctionSlotList slotList;
	FindSlots(InSlotId, slotList);

	for (UGsUINPCFunctionButton* slot : slotList)
	{
		if (slot)
		{
			slot->OnKeyboardRelease();
		}
	}
}

void UGsUINPCFunctionSlotSetHUD::OnScrolledEvent(float CurrentOffset)
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
void UGsUINPCFunctionSlotSetHUD::OnScrollStarted()
{
	_bIsScrolling = true;
	_startOffset = _destOffset; // 이전 마지막 위치
	int32 index = GetIndexByOffset(_startOffset);
	_minOffsetLock = (index - 1) * _unitOffset;
	_maxOffsetLock = (index + 1) * _unitOffset;

	ClearAutoScrollTimer();

	// close menu list
	if (_menuList->IsOpen() == true)
	{
		ResetMenuCheckValue();
		_menuList->Close();
	}
}

void UGsUINPCFunctionSlotSetHUD::OnScrollFinished()
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
	GetWorld()->GetTimerManager().SetTimer(_autoScrollTimer, this, &UGsUINPCFunctionSlotSetHUD::OnAutoScrollTimer, 0.016f, true);
}
void UGsUINPCFunctionSlotSetHUD::ClearAutoScrollTimer()
{
	if (_autoScrollTimer.IsValid())
	{
		GetWorld()->GetTimerManager().ClearTimer(_autoScrollTimer);
	}
}

void UGsUINPCFunctionSlotSetHUD::OnAutoScrollTimer()
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

void UGsUINPCFunctionSlotSetHUD::OnEndAutoScroll()
{
	ClearAutoScrollTimer();

	_scroll->SetScrollOffset(_destOffset);
}

void UGsUINPCFunctionSlotSetHUD::SetPageIcon(int32 InPage)
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

void UGsUINPCFunctionSlotSetHUD::UpdateAllSlot()
{
	int npcFunctionSlotMaxCount = GData()->GetGlobalData()->_npcFunctionSlotMaxCount;

	UGsGameObjectBase* localBase = GSGameObject()->FindObject(EGsGameObjectType::LocalPlayer);

	DefNPCFunctionSlotList slotList;
	for (int32 i = 1; i <= npcFunctionSlotMaxCount; ++i)
	{
		slotList.Empty();
		FindSlots(i, slotList);

		const FGsSchemaNpcFunctionData* findFunctionData = GSInteraction()->FindNpcFunctionDataBySlotId(i);		
		if (findFunctionData != nullptr)
		{
			TArray<FGsTownInteractionNpcInfo*> arrayNpcInfo;
			if (findFunctionData->_npcFunctionType == NpcFunctionType::EVENT_SHOP)
			{
				arrayNpcInfo = GSInteraction()->GetServerTownNpcDataBySlotId(i);
			}
			else
			{
				arrayNpcInfo = GSInteraction()->GetTownNpcDataBySlotId(i);
			}
			bool isUseable = false;
			TArray<int> arrayNpcTblId;
			if (arrayNpcInfo.Num() != 0)
			{
				for(FGsTownInteractionNpcInfo* info: arrayNpcInfo)
				{
					if (nullptr == info)
					{
						continue;
					}
					arrayNpcTblId.Add(info->_npcId);
				}

				isUseable = true;
			}

			bool isMoveButton = FGsAIFunc::IsNPCMoveTarget(localBase, arrayNpcTblId, false);

			for (UGsUINPCFunctionButton* slot : slotList)
			{
				slot->SetData(arrayNpcTblId, findFunctionData, isUseable, isMoveButton,
					[this](UUserWidget* In_target, TArray<int> In_arrayNpcTblId)
					{
						OnShowMenuList(In_target, In_arrayNpcTblId);
					});
			}
		}
		else
		{
			for (UGsUINPCFunctionButton* slot : slotList)
			{
				slot->SetEmptySlot();
			}
		}
	}
}

void UGsUINPCFunctionSlotSetHUD::FindSlots(int32 InSlotId, OUT DefNPCFunctionSlotList& OutList)
{

	for (TWeakObjectPtr<UGsUINPCFunctionSlotPageHUD> page : _pageList)
	{
		if (page.IsValid())
		{
			if (UGsUINPCFunctionButton* slot = page->GetSlot(InSlotId))
			{
				OutList.Emplace(slot);
			}
		}
	}
}

int32 UGsUINPCFunctionSlotSetHUD::GetIndexByOffset(float InOffset)
{
	if (_unitOffset > InOffset)
	{
		return 0;
	}

	return FMath::FloorToInt(InOffset / _unitOffset);
}

int32 UGsUINPCFunctionSlotSetHUD::GetPageByIndex(int32 InIndex)
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

void UGsUINPCFunctionSlotSetHUD::OnShowMenuList(UUserWidget* In_target, TArray<int> In_arrayNpcTblId)
{
	if (In_target == nullptr)
	{
		return;
	}

	if (_menuList->IsOpen() == true)
	{
		ResetMenuCheckValue();
		_menuList->Close();
		return;
	}

	_menuTarget = In_target;

	_menuList->SetMenuListData(In_arrayNpcTblId);
	_menuList->Open();
	_menuList->SetRenderOpacity(0.f); // 좌표계산위해

	CalcMenuPos();
}

UGsUINPCFunctionButton* UGsUINPCFunctionSlotSetHUD::SetTutorialTarget(int32 InSlotId)
{
	_tutorialSlotId = InSlotId;
	if (0 > _tutorialSlotId)
	{
		_tutorialOffset = 0.f;

		return nullptr;
	}

	_currPage = (7 < InSlotId) ? 2 : 1;
	_tutorialOffset = _unitOffset * _currPage;

	_bIsScrolling = false;	
	_startOffset = 0.f;

	_scroll->SetScrollOffset(_tutorialOffset);
	_destOffset = _tutorialOffset;

	SetPageIcon(_currPage);

	if (_pageList.IsValidIndex(_currPage))
	{
		if (_pageList[_currPage].IsValid())
		{
			return _pageList[_currPage]->GetSlot(InSlotId);
		}
	}

	return nullptr;
}

void UGsUINPCFunctionSlotSetHUD::CalcMenuPos()
{
	// 닫히는 것 체크
	if (false == _menuList->IsOpen())
	{
		ResetMenuCheckValue();
		return;
	}

	FGeometry menuGeometry = _menuList->GetCachedGeometry();
	FVector2D menuLocalSize = menuGeometry.GetLocalSize();
	FVector2D menuAbsPos = menuGeometry.AbsolutePosition;

	if (FVector2D::ZeroVector == menuLocalSize)
	{
		return;
	}

	// CachedGeometry 는 화면에 한번 그려지고 난 다음에야 제대로 계산됨. 값이 일정해질 때까지 대기
	if (_prevMenuSize != menuLocalSize ||
		_prevMenuPos != menuAbsPos)
	{
		_prevMenuSize = menuLocalSize;
		_prevMenuPos = menuAbsPos;
		return;
	}

	++_checkCount;

	// UIScale 변경 시 적용에 시간이 걸려 2틱정도 대기함
	if (3 > _checkCount)
	{
		return;
	}

	_checkCount = 0;

	FGeometry targetGeometry = _menuTarget->GetCachedGeometry();
	FVector2D targetPosition = targetGeometry.GetAbsolutePosition();
	FGeometry myGeometry = GetCachedGeometry();

	FVector2D menuAbsSize = menuGeometry.GetAbsoluteSize();
	menuAbsSize.X = menuAbsSize.X * 0.5f;
	FVector2D newPosition = myGeometry.AbsoluteToLocal(targetPosition - menuAbsSize);

	_menuList->SetRenderTranslation(newPosition);
	_menuList->SetRenderOpacity(1.f);

	// 열려있는 동안에 해상도 변경등 변화에 대응하기 위해 계속 체크하도록 _menuTarget을 비우지 않음
	//_menuTarget = nullptr;
}

void UGsUINPCFunctionSlotSetHUD::ResetMenuCheckValue()
{
	_prevMenuSize = FVector2D::ZeroVector;
	_prevMenuPos = FVector2D::ZeroVector;
	_menuTarget = nullptr;
	_checkCount = 0;
}
