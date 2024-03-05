// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUIWindowQuest.h"
#include "VerticalBox.h"
#include "TreeView.h"
#include "WidgetSwitcher.h"

#include "Management/GsMessageHolder.h"
#include "Management/ScopeGlobal/GsQuestManager.h"
#include "Management/ScopeGame/GsUnlockManager.h"

#include "Runtime/DataCenter/Public/Quest/GsQuestEnum.h"
#include "Quest/Table/GsQuestMainTableCacheSet.h"
#include "Quest/Main/GsSchemaQuestMainChapter.h"
#include "Quest/Tab/GsUIWindowQuestTabBase.h"

#include "UI/UIContent/Window/Quest/GsUIWindowQuestTitleItem.h"
#include "UI/UIControlLib/Control/GsToggleGroup.h"
#include "UI/UIControlLib/ContentWidget/GsSwitcherButton.h"
#include "UI/UIControlLib/ContentWidget/GsButton.h"
#include "UI/UIControlLib/Define/GsIconDefine.h"
#include "UI/UIContent/Common/RedDot/GsUIRedDotBase.h"
#include "UI/UIControlLib/Control/GsDynamicPanelSlotHelper.h"

#include "Message/MessageParam/GsMessageParam.h"
#include "Message/GsMessageGameObject.h"
#include "Quest/Main/GsUIWindowQuestMain.h"
#include "Quest/Sub/GsUIWindowQuestSub.h"
#include "Quest/Repeat/GsUIWindowQuestRepeat.h"
#include "Quest/Guide/GsUIWindowQuestGuide.h"



void UGsUIWindowQuest::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	_helpContentsKey = EGsHelpContents::Quest;

	_toggleGroupTab.AddToggleWidget(_switchBtnMain);
	_toggleGroupTab.AddToggleWidget(_switchBtnSub);
	_toggleGroupTab.AddToggleWidget(_switchBtnRepeat);	
	_toggleGroupTab.AddToggleWidget(_switchBtnGuide);
	_toggleGroupTab.OnSelectChanged.BindUObject(this, &UGsUIWindowQuest::OnClickTab);
	_toggleGroupTab.OnChangeBlocked.BindUObject(this, &UGsUIWindowQuest::OnBlockedTab);

	_btnClose->OnClicked.AddDynamic(this, &UGsUIWindowQuest::OnClickCloseAllStack);

	_questMainArea->_onCloseQuestTab.BindUObject(this, &UGsUIWindowQuest::OnEventClose);
	_questSubArea->_onCloseQuestTab.BindUObject(this, &UGsUIWindowQuest::OnEventClose);
	_questRepeatArea->_onCloseQuestTab.BindUObject(this, &UGsUIWindowQuest::OnEventClose);
	_questGuideArea->_onCloseQuestTab.BindUObject(this, &UGsUIWindowQuest::OnEventClose);
}

void UGsUIWindowQuest::NativeConstruct()
{
	Super::NativeConstruct();
	
	RegisterMessages();

	// 시작은 메인부터
	_toggleGroupTab.SetSelectedIndex(0, true);
	CheckRedDot();
	CheckUnlock(true);

	GMessage()->GetGameObject().SendMessage(MessageGameObject::QUEST_WINDOW_OPEN, nullptr);
}

void UGsUIWindowQuest::NativeDestruct()
{
	DeregisterMessages();
	Super::NativeDestruct();
}

// 메세지 등록
void UGsUIWindowQuest::RegisterMessages()
{	
	MGameObject& msgGameObject = GMessage()->GetGameObject();

	_msgGameObjectHandleList.Emplace(msgGameObject.AddUObject(MessageGameObject::QUEST_WINDOW_UPDATE,
		this, &UGsUIWindowQuest::OnWindowQuestUpdate));

	_msgGameObjectHandleList.Emplace(msgGameObject.AddUObject(MessageGameObject::QUEST_WINDOW_UPDATE_OBJECTIVE,
		this, &UGsUIWindowQuest::OnWindowQuestObjectiveUpdate));

	MUnlock& msgUnlock = GMessage()->GetUnlock();
	_msgUnlockHandleList.Emplace(msgUnlock.AddUObject(MessageContentUnlock::NOTIFY_CONTENTS_UNLOCK,
		this, &UGsUIWindowQuest::OnContentsUnlock));
	_msgUnlockHandleList.Emplace(msgUnlock.AddUObject(MessageContentUnlock::INVALIDATE_ALL_UNLOCK,
		this, &UGsUIWindowQuest::OnContentsUnlock));
}

// 메세지 해지
void UGsUIWindowQuest::DeregisterMessages()
{	
	MGameObject& msgGameObject = GMessage()->GetGameObject();

	for (MsgGameObjHandle& handle : _msgGameObjectHandleList)
	{
		msgGameObject.Remove(handle);
	}
	_msgGameObjectHandleList.Empty();

	MUnlock& msgUnlock = GMessage()->GetUnlock();
	for (MsgHandleUnlock& elemUnlock : _msgUnlockHandleList)
	{
		msgUnlock.Remove(elemUnlock);
	}
	_msgUnlockHandleList.Empty();
}

void UGsUIWindowQuest::CloseInternal()
{
	GMessage()->GetContentsHud().SendMessage(MessageContentHud::INVALIDATE_QUEST);	
	//GMessage()->GetGameObject().SendMessage(MessageGameObject::QUEST_WINDOW_GUIDE_TAB_CLOSE, nullptr);

	Super::CloseInternal();
}

bool UGsUIWindowQuest::SyncContentState()
{
	if (false == IsSameContentState(FGsContentsMode::InGame::ContentsQuest))
	{
		GMessage()->GetContents().SendMessage(MessageContents::ACTIVE_WINDOW_QUEST);
		return true;
	}

	return false;
}

void UGsUIWindowQuest::BeginDestroy()
{
	_toggleGroupTab.Clear();

	Super::BeginDestroy();
}

// 메인/서브/반복 탭 클릭했을때
void UGsUIWindowQuest::OnClickTab(int32 InIndex)
{
	if (InIndex == _selectedTabIndex)
		return;

	_selectedTabIndex = InIndex;

	if (_tabSwicher)
		_tabSwicher->SetActiveWidgetIndex(InIndex);

	if (0 == InIndex)
	{
		_questMainArea->OnClickTab(true);
	}
	else
	{
		_questMainArea->OnClickTab(false);
	}
	 
	if (1 == InIndex)
	{
		_questSubArea->OnClickTab(true);
	}
	else
	{
		_questSubArea->OnClickTab(false);
	}
	
	if (2 == InIndex)
	{
		_questRepeatArea->OnClickTab();
	}
	
	if (3 == InIndex)
	{
		_questGuideArea->OnClickTab();
	}	
}

void UGsUIWindowQuest::OnBlockedTab(int32 InIndex)
{
	if (1 == InIndex)
	{
		GSUnlock()->IsContentsUnlock(EGsUnlockContentsType::QUEST_SUB, true);
	}
}

// window창이 열려 있을 때 퀘스트 변화가 생기면 받는 메세지 (완료,포기..등등의 )
void UGsUIWindowQuest::OnWindowQuestUpdate(const IGsMessageParam* In_data)
{
	int index = _tabSwicher->GetActiveWidgetIndex();

	if (0 == index)
	{
		_questMainArea->RefreshAll();
	}
	else if ( 1 == index)
	{
		_questSubArea->RefreshAll();
	}
	else if (2 == index)
	{
		//_questRepeatArea->RefreshAll();
	}
	else if (3 == index)
	{
		_questGuideArea->RefreshAll();
	}

	CheckRedDot();
}

void UGsUIWindowQuest::OnWindowQuestObjectiveUpdate(const IGsMessageParam* In_data)
{
	int index = _tabSwicher->GetActiveWidgetIndex();
	if (0 == index)
	{
		_questMainArea->RefreshQuestObjective();
	}
	else if (1 == index)
	{
		_questSubArea->RefreshQuestObjective();
	}
	else if (2 == index)
	{
		_questRepeatArea->RefreshQuestObjective();
	}
	else if (3 == index)
	{
		_questGuideArea->RefreshQuestObjective();
	}

	CheckRedDot();
}

void UGsUIWindowQuest::OnContentsUnlock(const struct IGsMessageParam* InParam)
{
	CheckUnlock(false);
}

void UGsUIWindowQuest::CheckRedDot()
{
	UGsQuestManager* questManager = GSQuest();
	if (nullptr == questManager)
		return;

	_tabSubRedDot->SetRedDot(questManager->IsSubQuestRedDot(), EGsIconRedDotType::CHECK);
	_tabRepeatRedDot->SetRedDot(questManager->IsRepeatQuestRedDot(), EGsIconRedDotType::CHECK);
	_tabGuideRedDot->SetRedDot(questManager->IsGuideQuestRedDot(), EGsIconRedDotType::CHECK);
}

void UGsUIWindowQuest::CheckUnlock(bool InIsFirstCheck)
{
	bool isSubQuestUnlock = GSUnlock()->IsContentsUnlock(EGsUnlockContentsType::QUEST_SUB);

	if (_toggleGroupTab.IsBlockedIndex(1))
	{
		if (true == isSubQuestUnlock)
		{
			// 서브 퀘스트 잠금 해제
			_toggleGroupTab.DeleteBlockedIndex(1);
			// 잠금 해제 UI 처리
		}
	}
	else
	{
		if (false == isSubQuestUnlock)
		{
			// 서브 퀘스트 잠금
			_toggleGroupTab.AddBlockedIndex(1);
			// 잠금 UI 처리

			// Window 첫 진입 외에 이쪽 로직을 타는 경우는 서버잠금 등에 의해서 다시 잠겼을 때.
			// 다시 잠겼을 경우 첫번째 탭으로 되돌린다.
			if (false == InIsFirstCheck)
			{
				_toggleGroupTab.SetSelectedIndex(0, true);
			}
		}
	}
}

void UGsUIWindowQuest::OnEventClose()
{
	Close(true);
}