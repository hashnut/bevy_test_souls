// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUIGuildContentMember.h"

#include "Components/ScrollBox.h"

#include "UI/UIControlLib/ContentWidget/GsSwitcherButton.h"
#include "UI/UIControlLib/Control/GsDynamicPanelSlotHelper.h"
#include "UI/UIContent/Window/Guild/GsUIGuildMemberListEntry.h"
#include "UI/UIContent/Popup/GsUIPopupChatUserInfo.h"

#include "Message/MessageParam/GsGuildMessageParam.h"

#include "Guild/GsGuildMemberData.h"

#include "Management/ScopeGlobal/GsGuildManager.h"
#include "Management/ScopeGlobal/GsUIManager.h"
#include "Management/GsMessageHolder.h"


void UGsUIGuildContentMember::BeginDestroy()
{
	if (_slotHelper)
	{
		_slotHelper->OnRefreshEntry.RemoveDynamic(this, &UGsUIGuildContentMember::OnRefreshEntry);
	}
	_slotHelper = nullptr;

	_dataList.Empty();	
	_toggleGroup.Clear();
	_filterList.Empty();

	Super::BeginDestroy();
}

void UGsUIGuildContentMember::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	_btnFilterLevel->OnClicked.AddDynamic(this, &UGsUIGuildContentMember::OnClickFilterLevel);
	_btnFilterName->OnClicked.AddDynamic(this, &UGsUIGuildContentMember::OnClickFilterName);
	_btnFilterMemberGrade->OnClicked.AddDynamic(this, &UGsUIGuildContentMember::OnClickFilterMemberGrade);
	_btnFilterTime->OnClicked.AddDynamic(this, &UGsUIGuildContentMember::OnClickFilterTime);
	_btnFilterContribution->OnClicked.AddDynamic(this, &UGsUIGuildContentMember::OnClickFilterContribution);
	_btnFilterAccumContribution->OnClicked.AddDynamic(this, &UGsUIGuildContentMember::OnClickFilterAccumContribution);
	_btnFilterAttendance->OnClicked.AddDynamic(this, &UGsUIGuildContentMember::OnClickAttendance);

	_slotHelper = NewObject<UGsDynamicPanelSlotHelper>(this);
	_slotHelper->Initialize(_entryWidgetClass, _panelScrollRoot);
	_slotHelper->OnRefreshEntry.AddDynamic(this, &UGsUIGuildContentMember::OnRefreshEntry);

	_toggleGroup.OnSelectChanged.BindUObject(this, &UGsUIGuildContentMember::OnSelectChangeMemberEntry);

	_filterList.Emplace(_btnFilterLevel);
	_filterList.Emplace(_btnFilterName);
	_filterList.Emplace(_btnFilterMemberGrade);
	_filterList.Emplace(_btnFilterTime);
	_filterList.Emplace(_btnFilterContribution);
	_filterList.Emplace(_btnFilterAccumContribution);
	_filterList.Emplace(_btnFilterAttendance);
}

void UGsUIGuildContentMember::NativeConstruct()
{
	Super::NativeConstruct();

	// 주의: 순서주의. 소팅함수 연결
	_sortFuncList.Empty();
	_sortFuncList.Emplace([this](int32 InState)
		{
			SortByLevel(InState);
		});
	_sortFuncList.Emplace([this](int32 InState)
		{
			SortByName(InState);
		});	
	_sortFuncList.Emplace([this](int32 InState)
		{
			SortByMemberGrade(InState);
		});
	_sortFuncList.Emplace([this](int32 InState)
		{
			SortByTime(InState);
		});
	_sortFuncList.Emplace([this](int32 InState)
		{
			SortByContribution(InState);
		});
	_sortFuncList.Emplace([this](int32 InState)
		{
			SortByAccumContribution(InState);
		});
	_sortFuncList.Emplace([this](int32 InState)
		{
			SortByAttendance(InState);
		});

	_userContextMsgHandlerList.Emplace(GMessage()->GetUserContext().AddUObject(MessageContentUserContext::OPEN,
		this, &UGsUIGuildContentMember::OnOpenUserContext));
	_userContextMsgHandlerList.Emplace(GMessage()->GetUserContext().AddUObject(MessageContentUserContext::CLOSE,
		this, &UGsUIGuildContentMember::OnCloseUserContext));
}

void UGsUIGuildContentMember::NativeDestruct()
{
	_dataList.Empty();
	_toggleGroup.Clear();
	_sortFuncList.Empty();

	FGsMessageHolder* msgHolder = GMessage();
	for (TPair<MessageContentUserContext, FDelegateHandle>& iter : _userContextMsgHandlerList)
	{
		msgHolder->GetUserContext().Remove(iter);
	}
	_userContextMsgHandlerList.Empty();

	Super::NativeDestruct();
}

void UGsUIGuildContentMember::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	
	const float MOVE_TIME = 0.1f;
	const float MOVE_TIME_INV = 10.0f;
	if (_time < MOVE_TIME)
	{
		FWidgetTransform panelRenderTransform = _panelScrollRoot->RenderTransform;
		float MOVE_RATE = _time * MOVE_TIME_INV;

		if (_isReverse)
		{
			panelRenderTransform.Translation.Y = _panelMoveOffset * (1 - MOVE_RATE);
		}
		else
		{
			panelRenderTransform.Translation.Y = _panelMoveOffset * MOVE_RATE;
			if (_scrollMoveOffset > 0)
			{
				_scrollBox->SetScrollOffset(((1 - MOVE_RATE) * _scrollStartOffset + _scrollMoveOffset * MOVE_RATE));
			}
		}
		
		_time += InDeltaTime;

		if (_time >= MOVE_TIME)
		{
			if (_isReverse)
			{
				panelRenderTransform.Translation.Y = 0;
			}
			else
			{
				panelRenderTransform.Translation.Y = _panelMoveOffset;
				_panelMoveOffset = 0;
				if (_scrollMoveOffset > 0)
				{
					_scrollBox->SetScrollOffset(_scrollMoveOffset);
					_scrollMoveOffset = 0;
				}
			}
		}
		_panelScrollRoot->SetRenderTransform(panelRenderTransform);
	}
}

void UGsUIGuildContentMember::InvalidateAllInternal()
{
	InvalidateList();
}

void UGsUIGuildContentMember::SendRequestPacket()
{
	GGuild()->GetSendController().SendRequestMemberList();
}

void UGsUIGuildContentMember::Show(bool bIsMyGuild, bool IsStackRecoverFlag /*= false*/)
{
	_currFilter = EGsFilterType::MemberGrade;

	InvalidateAll();
}

void UGsUIGuildContentMember::OnResetTimeContents(ResetTimeTicket InType)
{
	FGsGuildManager* guildMgr = GGuild();

	// 리스트를 무조건 다시 요청한 후 받아서 출석, 공헌도 등을 갱신
	guildMgr->GetSendController().ResetCoolTime(FGsGuildSendCoolTimeController::EGsCoolTimeType::MemberList);
	guildMgr->GetSendController().SendRequestMemberList();
}

void UGsUIGuildContentMember::OnNotifyChangeMemberGrade(const FGsGuildMsgParamMemberGrade* InParam)
{
	// 소팅 적용을 위해 전체 갱신
	InvalidateAll();
}

void UGsUIGuildContentMember::OnChangeMyAuthority()
{
	// 선택중인 데이터를 갱신시킴
	int32 selectedIndex = _toggleGroup.GetSelectedIndex();
	if (_dataList.IsValidIndex(selectedIndex))
	{
		_slotHelper->RefreshByIndex(selectedIndex);
	}
}

void UGsUIGuildContentMember::OnRecieveMemberList()
{
	InvalidateAll();
}

void UGsUIGuildContentMember::OnReceiveChangeMemberGrade(UserDBId InUserDBId)
{
	// 소팅 적용을 위해 전체 갱신
	InvalidateAll();
}

void UGsUIGuildContentMember::OnNotifyJoinMember()
{
	// 해당 멤버의 정보를 얻기 위해 리스트 요청
	FGsGuildManager* guildMgr = GGuild();
	guildMgr->GetSendController().ResetCoolTime(FGsGuildSendCoolTimeController::EGsCoolTimeType::MemberList);
	guildMgr->GetSendController().SendRequestMemberList();
}

void UGsUIGuildContentMember::OnReceiveRemoveMember()
{
	InvalidateAll();
}

void UGsUIGuildContentMember::InvalidateList()
{
	_dataList.Empty();
	_dataList.Append(GGuild()->GetMemberList());

	// 0 은 선택되지 않은 상태
	int32 index = 0;
	if (_filterList.IsValidIndex(_currFilter))
	{
		index = _filterList[_currFilter]->GetSwitcherIndex();
	}

	// 0 상태는 없도록 한다
	index = (0 == index) ? 1 : index;

	SetFilter(_currFilter, index);

	_toggleGroup.Clear();
	_toggleGroup.AddToggleWidgetByParentPanel(_panelScrollRoot);
	// 선택을 해제 시켜놓기 위함
	_toggleGroup.SetSelectedIndex(-1);
}

void UGsUIGuildContentMember::OnSelectChangeMemberEntry(int32 InIndex)
{
	// 따로할게 없을듯?
}

void UGsUIGuildContentMember::OnRefreshEntry(int32 InIndex, UWidget* InEntry)
{
	if (false == _dataList.IsValidIndex(InIndex))
	{
		return;
	}

	if (UGsUIGuildMemberListEntry* entry = Cast<UGsUIGuildMemberListEntry>(InEntry))
	{
		entry->SetData(_dataList[InIndex]);
		entry->SetClickUserNameCallback([this](UserDBId InUserDBId, UWidget* InWidget)
			{
				_lastClickedUserDBId = InUserDBId;
				_lastClickedButton = InWidget;
			});
	}
}

void UGsUIGuildContentMember::OnClickFilterLevel()
{
	// 0->1, 1->2, 2->1 로 만든다. (눌렀을 때는 0으로 만들지 않음)
	int32 index = (1 == _btnFilterLevel->GetSwitcherIndex()) ? 2 : 1;
	SetFilter(EGsFilterType::Level, index);
}

void UGsUIGuildContentMember::OnClickFilterName()
{
	// 0->1, 1->2, 2->1 로 만든다. (눌렀을 때는 0으로 만들지 않음)
	int32 index = (1 == _btnFilterName->GetSwitcherIndex()) ? 2 : 1;
	SetFilter(EGsFilterType::Name, index);
}

void UGsUIGuildContentMember::OnClickFilterMemberGrade()
{
	// 0->1, 1->2, 2->1 로 만든다. (눌렀을 때는 0으로 만들지 않음)
	int32 index = (1 == _btnFilterMemberGrade->GetSwitcherIndex()) ? 2 : 1;	
	SetFilter(EGsFilterType::MemberGrade, index);
}

void UGsUIGuildContentMember::OnClickFilterTime()
{
	// 0->1, 1->2, 2->1 로 만든다. (눌렀을 때는 0으로 만들지 않음)
	int32 index = (1 == _btnFilterTime->GetSwitcherIndex()) ? 2 : 1;
	SetFilter(EGsFilterType::Time, index);
}

void UGsUIGuildContentMember::OnClickFilterContribution()
{
	// 0->1, 1->2, 2->1 로 만든다. (눌렀을 때는 0으로 만들지 않음)
	int32 index = (1 == _btnFilterContribution->GetSwitcherIndex()) ? 2 : 1;
	SetFilter(EGsFilterType::Contribution, index);
}

void UGsUIGuildContentMember::OnClickFilterAccumContribution()
{
	// 0->1, 1->2, 2->1 로 만든다. (눌렀을 때는 0으로 만들지 않음)
	int32 index = (1 == _btnFilterAccumContribution->GetSwitcherIndex()) ? 2 : 1;
	SetFilter(EGsFilterType::AccumContribution, index);
}

void UGsUIGuildContentMember::OnClickAttendance()
{
	// 0->1, 1->2, 2->1 로 만든다. (눌렀을 때는 0으로 만들지 않음)
	int32 index = (1 == _btnFilterAttendance->GetSwitcherIndex()) ? 2 : 1;
	SetFilter(EGsFilterType::Attendance, index);
}

void UGsUIGuildContentMember::OnOpenUserContext(const IGsMessageParam* InParm)
{
	int32 index = 0;
	for (index = 0;index < _dataList.Num();++index)
	{
		if (_lastClickedUserDBId == _dataList[index]->_userDBId)
		{
			break;
		}
	}

	UWidget* memberEntry = _panelScrollRoot->GetChildAt(index);
	if (nullptr == memberEntry)
		return;

	UWidget* lastMemberEntry = _panelScrollRoot->GetChildAt(_dataList.Num() - 1);
	if (nullptr == lastMemberEntry)
		return;

	TWeakObjectPtr<UGsUIPopupChatUserInfo> contextPopup = Cast<UGsUIPopupChatUserInfo>(GUI()->GetWidgetByKey(TEXT("PopupChatUserInfo")));
	if (false == contextPopup.IsValid())
		return;

	TWeakObjectPtr<UGsUIUserInfo> userInfoUI = Cast<UGsUIUserInfo>(contextPopup->GetUserInfoUI());
	if (false == userInfoUI.IsValid())
		return;

	float scrollBoxEndOffset = _scrollBox->GetScrollOffsetOfEnd(); // 마지막 위젯이 화면에 보일때의 스크롤 위치
	float scrollBoxCurrentOffset = _scrollBox->GetScrollOffset();
	float scrollBoxBottomOffset = scrollBoxCurrentOffset + _scrollBox->GetCachedGeometry().GetLocalSize().Y;
	float perOffset = memberEntry->GetCachedGeometry().GetLocalSize().Y;
	float targetTopOffset = perOffset * index;
	float targetBottomOffset = perOffset * (index + 1);
	float contextHeight = userInfoUI->GetCachedGeometry().GetLocalSize().Y;
	if (targetTopOffset < scrollBoxCurrentOffset)
	{
		// 가려졌거나 살짝 가려진 상태 : 유저 정보가 다 보이는 위치로
		_scrollBox->ScrollWidgetIntoView(memberEntry, true, EDescendantScrollDestination::TopOrLeft);
	}
	else
	{
		if (targetBottomOffset > scrollBoxBottomOffset)
		{
			// 가려졌거나 살짝 가려진 상태 : 컨텍스트 ui가 다 보이는 위치로 
			float desiredOffset = scrollBoxBottomOffset - contextHeight - targetTopOffset;

			_time = 0;
			_isReverse = false;
			if (FMath::Abs(desiredOffset) <= FMath::Abs(scrollBoxEndOffset - scrollBoxCurrentOffset))
			{
				// 스크롤링
				_scrollStartOffset = _scrollBox->GetScrollOffset();
				_scrollMoveOffset = scrollBoxCurrentOffset - desiredOffset;
				_panelMoveOffset = 0;
			}
			else
			{
				// 스크롤링 + 패널이동
				_scrollBox->ScrollWidgetIntoView(lastMemberEntry, true, EDescendantScrollDestination::BottomOrRight);
				_scrollMoveOffset = 0;
				_panelMoveOffset = desiredOffset + (scrollBoxEndOffset - scrollBoxCurrentOffset);
			}
		}
		else if (targetTopOffset > scrollBoxBottomOffset - contextHeight)
		{
			// 컨텍스트가 출력될 공간보다 낮게 위치했을 경우
			float desiredOffset = scrollBoxBottomOffset - contextHeight - targetTopOffset;
			
			_time = 0;
			_isReverse = false;
			if (FMath::Abs(desiredOffset) < FMath::Abs(scrollBoxEndOffset - scrollBoxCurrentOffset))
			{
				_scrollStartOffset = _scrollBox->GetScrollOffset();
				_scrollMoveOffset = scrollBoxCurrentOffset - desiredOffset;
				_panelMoveOffset = 0;
			}
			else
			{
				_scrollBox->ScrollWidgetIntoView(lastMemberEntry, true, EDescendantScrollDestination::BottomOrRight);
				_scrollMoveOffset = 0;
				_panelMoveOffset = desiredOffset + scrollBoxEndOffset - scrollBoxCurrentOffset;
			}
		}
		else
		{
			// 완전 보이는 상태
		}
	}
	userInfoUI->SetTarget(_lastClickedButton.Get());
	userInfoUI->SetIsDelayedOpen(false);
	_scrollBox->EndInertialScrolling();
}

void UGsUIGuildContentMember::OnCloseUserContext(const struct IGsMessageParam* InParm)
{
	FWidgetTransform panelRenderTransform = _panelScrollRoot->RenderTransform;
	if (panelRenderTransform.Translation.Y == 0)
	{
		_time = 1.0f;
	}
	else
	{
		_isReverse = true;
		_time = 0.f;
	}
	_scrollMoveOffset = 0.f;
}

void UGsUIGuildContentMember::SetFilter(EGsFilterType InType, int32 InState)
{
	_currFilter = InType;

	SetUISelectFilter(InType, InState);

	if (_sortFuncList.IsValidIndex(InType))
	{
		_sortFuncList[InType](InState);
	}

	_slotHelper->RefreshAll(_dataList.Num());
}

void UGsUIGuildContentMember::SortByLevel(int32 InState)
{
	// 1: 내림차순, 2: 오름차순
	if (1 == InState)
	{
		_dataList.Sort([](const FGsGuildMemberData& A, const FGsGuildMemberData& B)
			{
				return (A._level >= B._level) ? true : false;
			});
	}
	else
	{
		_dataList.Sort([](const FGsGuildMemberData& A, const FGsGuildMemberData& B)
			{
				return (A._level < B._level) ? true : false;
			});
	}
}

void UGsUIGuildContentMember::SortByName(int32 InState)
{
	// 1: 내림차순, 2: 오름차순
	if (1 == InState)
	{
		_dataList.Sort([](const FGsGuildMemberData& A, const FGsGuildMemberData& B)
			{
				return (A._userName >= B._userName) ? true : false;
			});
	}
	else
	{
		_dataList.Sort([](const FGsGuildMemberData& A, const FGsGuildMemberData& B)
			{
				return (A._userName < B._userName) ? true : false;
			});
	}
}

void UGsUIGuildContentMember::SortByMemberGrade(int32 InState)
{
	// 이건 기본 정렬값으로 쓰며, 2순위 누적 기여도, 3순위 레벨

	// 1: 내림차순, 2: 오름차순
	if (1 == InState)
	{
		_dataList.Sort([](const FGsGuildMemberData& A, const FGsGuildMemberData& B)
			{
				// 1순위 등급
				if (A._memberGrade > B._memberGrade)
				{
					return true;
				}
				else if (A._memberGrade < B._memberGrade)
				{
					return false;
				}

				// 2순위 누적 기여도
				if (A._accumContribution > B._accumContribution)
				{
					return true;
				}
				else if (A._accumContribution < B._accumContribution)
				{
					return false;
				}

				// 3순위 레벨
				if (A._level > B._level)
				{
					return true;
				}
				else if(A._level < B._level)
				{
					return false;
				}

				return false;
			});
	}
	else
	{
		_dataList.Sort([](const FGsGuildMemberData& A, const FGsGuildMemberData& B)
			{
				// 1순위 등급
				if (A._memberGrade < B._memberGrade)
				{
					return true;
				}
				else if (A._memberGrade > B._memberGrade)
				{
					return false;
				}

				// 2순위 누적 기여도
				if (A._accumContribution < B._accumContribution)
				{
					return true;
				}
				else if (A._accumContribution > B._accumContribution)
				{
					return false;
				}

				// 3순위 레벨
				if (A._level < B._level)
				{
					return true;
				}
				else if (A._level > B._level)
				{
					return false;
				}

				return false;
			});
	}
}

void UGsUIGuildContentMember::SortByTime(int32 InState)
{
	// 현재 접속중일 경우 값이 0이다.
	// 최근에 나간 유저일 수록 _lastLoginTimeOriginal가 큰 값이다.

	// 1: 내림차순, 2: 오름차순
	if (1 == InState)
	{
		_dataList.Sort([](const FGsGuildMemberData& A, const FGsGuildMemberData& B)
			{
				// 1순위: 접속 중인 유저 구분
				if (0 == A._lastLoginTimeOriginal && 0 != B._lastLoginTimeOriginal)
				{
					return true;
				}
				else if (0 != A._lastLoginTimeOriginal && 0 == B._lastLoginTimeOriginal)
				{
					return false;
				}

				// 2순위: 접속시간
				if (A._lastLoginTimeOriginal >= B._lastLoginTimeOriginal)
				{
					return true;
				}

				return false;
			});
	}
	else
	{
		_dataList.Sort([](const FGsGuildMemberData& A, const FGsGuildMemberData& B)
			{
				// 1순위: 접속 중인 유저 구분
				if (0 != A._lastLoginTimeOriginal && 0 == B._lastLoginTimeOriginal)
				{
					return true;
				}
				else if (0 == A._lastLoginTimeOriginal && 0 != B._lastLoginTimeOriginal)
				{
					return false;
				}

				// 2순위: 접속시간
				if (A._lastLoginTimeOriginal < B._lastLoginTimeOriginal)
				{
					return true;
				}

				return false;
			});
	}
}

void UGsUIGuildContentMember::SortByContribution(int32 InState)
{
	// 1: 내림차순, 2: 오름차순
	if (1 == InState)
	{
		_dataList.Sort([](const FGsGuildMemberData& A, const FGsGuildMemberData& B)
			{
				return (A._weeklyContribution >= B._weeklyContribution) ? true : false;
			});
	}
	else
	{
		_dataList.Sort([](const FGsGuildMemberData& A, const FGsGuildMemberData& B)
			{
				return (A._weeklyContribution < B._weeklyContribution) ? true : false;
			});
	}
}


void UGsUIGuildContentMember::SortByAccumContribution(int32 InState)
{
	// 1: 내림차순, 2: 오름차순
	if (1 == InState)
	{
		_dataList.Sort([](const FGsGuildMemberData& A, const FGsGuildMemberData& B)
			{
				return (A._accumContribution >= B._accumContribution) ? true : false;
			});
	}
	else
	{
		_dataList.Sort([](const FGsGuildMemberData& A, const FGsGuildMemberData& B)
			{
				return (A._accumContribution < B._accumContribution) ? true : false;
			});
	}
}

void UGsUIGuildContentMember::SortByAttendance(int32 InState)
{
	// 1: 내림차순, 2: 오름차순
	if (1 == InState)
	{
		_dataList.Sort([](const FGsGuildMemberData& A, const FGsGuildMemberData& B)
			{
				return (true == A._attendance) ? true : false;
			});
	}
	else
	{
		_dataList.Sort([](const FGsGuildMemberData& A, const FGsGuildMemberData& B)
			{
				return (false == A._attendance) ? true : false;
			});
	}
}

void UGsUIGuildContentMember::SetUISelectFilter(EGsFilterType InType, int32 InState)
{
	int32 index = static_cast<int32>(InType);

	for (int32 i = 0; i < _filterList.Num(); ++i)
	{
		UGsSwitcherButton* btn = _filterList[i];

		if (i == index)
		{
			btn->SetSwitcherIndex(InState);
		}
		else
		{
			btn->SetSwitcherIndex(0);
		}
	}
}
