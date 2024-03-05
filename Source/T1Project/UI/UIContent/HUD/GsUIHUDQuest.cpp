// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUIHUDQuest.h"

#include "GameObject/Define/GsGameObjectDefine.h"
#include "GameObject/ObjectClass/GsGameObjectLocalPlayer.h"
#include "GameObject/Quest/GsQuestHandler.h"

#include "Quest/GsUIHUDQuestSlot.h"
#include "Quest/GsQuestInstance.h"

#include "Management/ScopeGame/GsGameObjectManager.h"
#include "Management/GsMessageHolder.h"
#include "Management/ScopeGlobal/GsQuestManager.h"
#include "Management/ScopeGame/GsUnlockManager.h"

#include "Message/GsMessageContents.h"
#include "Message/MessageParam/GsQuestMessageParam.h"

#include "UI/UIControlLib/Control/GsDynamicPanelSlotHelper.h"

#include "UTIL/GsGameObjectUtil.h"

#include "DataCenter/Public/KeyMapping/GsKeyMappingEnum.h"

#include "Components/PanelWidget.h"
#include "Components/ScrollBox.h"
#include "Quest/GsQuestData.h"
#include "../Option/GsServerOption.h"


void UGsUIHUDQuest::NativeOnInitialized()
{
	// 스크롤 셋팅
	if (nullptr == _scrollBoxHelper)
	{
		_scrollBoxHelper = NewObject<UGsDynamicPanelSlotHelper>(this);
		_scrollBoxHelper->Initialize(_scrollBoxEntryClass, _panelScrollRoot);
		_scrollBoxHelper->OnRefreshEntry.AddDynamic(this, &UGsUIHUDQuest::OnRefreshEntryScrollBox);
	}

	_scrollBox->OnUserScrolled.AddDynamic(this, &UGsUIHUDQuest::OnScrollQuestList);

	Super::NativeOnInitialized();
}

void UGsUIHUDQuest::NativeDestruct()
{
	SetTutorialTarget(0, false);

	Super::NativeDestruct();
}

void UGsUIHUDQuest::InvalidateAllInternal()
{
	UpdateList();
}

void UGsUIHUDQuest::BeginDestroy()
{
	if (nullptr != _scrollBoxHelper)
	{
		_scrollBoxHelper->OnRefreshEntry.RemoveDynamic(this, &UGsUIHUDQuest::OnRefreshEntryScrollBox);
		_scrollBoxHelper = nullptr;
	}

	_questList.Empty();
	_questSlotList.Empty();
	_activeQuestKey = QuestKey::KeyNone();

	Super::BeginDestroy();
}

void UGsUIHUDQuest::OnRefreshEntryScrollBox(int32 InIndex, UWidget* InEntry)
{
	int8 listNum = _questList.Num();
	if (listNum > InIndex)
	{
		FGsQuestInstance* obj = _questList[InIndex];
		if (nullptr != obj)
		{
			UGsUIHUDQuestSlot* slot = Cast<UGsUIHUDQuestSlot>(InEntry);
			if (nullptr != slot)
			{
				slot->SetIsSelected(false);
				slot->Update(obj, InIndex);
				slot->SetTutorialHideTeleport(_bTutorialHideTeleport);
				slot->_delegateClickSlot.BindUObject(this, &UGsUIHUDQuest::OnClickSlot);
				slot->_delegateClickTeleport.BindUObject(this, &UGsUIHUDQuest::OnClickTeleport);

				if (obj->GetQuestKey() == _activeQuestKey)
				{
					_activeSlotIndex = InIndex;
					slot->SetIsSelected(true);
				}

				_questSlotList.Add(slot);
			}
		}
	}
}

void UGsUIHUDQuest::OnClickSlot(int32 InIndex, int32 InType)
{
	if (false == GSUnlock()->IsContentsUnlock(EGsUnlockContentsType::QUEST, true))
		return;

	// https://jira.com2us.com/jira/browse/C2URWQ-5394
	// a 퀘스트 대화연출 서버 send 후 recv 받기전
	// b 퀘스트 액티브 시도시 
	// a 퀘스트 recv 후 대화연출 시작되고 
	// b 퀘스트 이동이되는 현상 막기
	if (true == UGsGameObjectUtil::IsInputFlagBlock(EGsKeyMappingInputFlags::INPUT_CONTENTS_WAIT_SERVER_RESPONSE))
	{
		return;
	}

	int num = _questSlotList.Num();
	if (InIndex >= num)
		return;

	if (_activeSlotIndex != InIndex)
	{
		if (InType == (int32)EGsQuestUIState::QUEST_STATE_PROGRESS)
		{
			// 다른 슬롯을 눌렀을 경우
			if (-1 != _activeSlotIndex && num > _activeSlotIndex)
			{
				if (_questSlotList[_activeSlotIndex].IsValid())
				{
					_questSlotList[_activeSlotIndex]->SetIsSelected(false);
				}
			}

			if (_questSlotList[InIndex].IsValid())
			{
				_questSlotList[InIndex]->SetIsSelected(true);
			}
			
			_activeSlotIndex = InIndex;
		}

		int8 listNum = _questList.Num();
		FGsMessageHolder* msg = GMessage();

		if (0 < listNum && listNum >= InIndex)
		{
			if (FGsQuestInstance* obj = _questList[InIndex])
			{
				_activeQuestKey = obj->GetQuestKey();

				FGsQuestSlotTouchParam param(_activeQuestKey, true, 0);
				msg->GetGameObject().SendMessage(MessageGameObject::QUEST_UIACTION_TOUCH, &param);
			}
		}
	}
	else
	{
		FGsQuestSlotTouchParam param(_activeQuestKey, false, 0);
		GMessage()->GetGameObject().SendMessage(MessageGameObject::QUEST_UIACTION_TOUCH, &param);

		// 같은 슬롯을 눌렀을 경우 (해지)
		if (_questSlotList[InIndex].IsValid())
		{
			_questSlotList[InIndex]->SetIsSelected(false);
		}

		_activeSlotIndex = -1;
		_activeQuestKey = QuestKey::KeyNone();
	}
}

void UGsUIHUDQuest::OnClickTeleport(int32 InIndex)
{
	int8 listNum = _questList.Num();
	FGsMessageHolder* msg = GMessage();
	if (0 < listNum && listNum >= InIndex)
	{
		if (FGsQuestInstance* obj = _questList[InIndex])
		{
			FGsQuestTeleportParam param(obj->GetQuestId(), obj->GetStoryId(), obj->GetQuestIndex());
			msg->GetGameObject().SendMessage(MessageGameObject::QUEST_UIACTION_TELEPORT, &param);
		}
	}
}

void UGsUIHUDQuest::UpdateList()
{
	_activeSlotIndex = -1;
	_activeQuestKey = GSQuest()->GetAutoProgressQuestKey();
	_questList.Empty();

	if (FGsQuestHandler* questHandler = GetQuestHandler())
	{
		questHandler->QuestInstancetListSort();
		const TArray<FGsQuestInstance*>& questList = questHandler->GetQuesInstancetList();

		if (IsTutorial())
		{
			// 튜토리얼일 경우, 튜토리얼 대상 퀘스트 아이템만 보여준다
			for (auto& iter : questList)
			{
				if (_tutorialQuestId == iter->GetQuestId() &&
					iter->IsShowHudUI())
				{
					_questList.Add(iter);
					break;
				}
			}
		}
		else
		{
			for (auto& iter : questList)
			{
				if (iter->IsShowHudUI())
				{
					_questList.Add(iter);
				}
			}			
		}

		int scrollCount = _questSlotList.Num();
		int questCount = _questList.Num();

		if (scrollCount != questCount)
		{
			_questSlotList.Empty();
			_scrollBoxHelper->RefreshAll(questCount);
		}
		else
		{
			for (int i = 0; i < questCount; ++i)
			{
				FGsQuestInstance* obj = _questList[i];
				if (nullptr != obj)
				{
					UGsUIHUDQuestSlot* slot = Cast<UGsUIHUDQuestSlot>(_scrollBoxHelper->GetEntry(i));
					if (nullptr != slot)
					{
						slot->SetIsSelected(false);
						slot->Update(obj, i);
						slot->SetTutorialHideTeleport(_bTutorialHideTeleport);
						slot->_delegateClickSlot.BindUObject(this, &UGsUIHUDQuest::OnClickSlot);
						slot->_delegateClickTeleport.BindUObject(this, &UGsUIHUDQuest::OnClickTeleport);

						if (obj->GetQuestKey() == _activeQuestKey)
						{
							_activeSlotIndex = i;
							slot->SetIsSelected(true);
						}
					}
				}
			}
		}

		if (IsTutorial())
		{
			if (0 < _questList.Num() && 0 < _scrollBoxHelper->GetItemCount())
			{
				if (UWidget* firstWidget = _scrollBoxHelper->GetEntry(0))
				{
					OnTutorialQuestListEvent.ExecuteIfBound(firstWidget);
				}
			}
		}
	}
}

FGsQuestHandler* UGsUIHUDQuest::GetQuestHandler()
{
	if (UGsGameObjectManager* objManager = GSGameObject())
	{
		if (UGsGameObjectBase* local = objManager->FindObject(EGsGameObjectType::LocalPlayer))
		{
			UGsGameObjectLocalPlayer* castLocal = local->CastGameObject<UGsGameObjectLocalPlayer>();
			if (FGsQuestHandler* questHandler = castLocal->GetQuestHandler())
			{
				return questHandler;
			}
		}
	}

	return nullptr;
}

void UGsUIHUDQuest::OnScrollQuestList(float InCurrentOffset)
{
	if (IsTutorial())
	{
		_scrollBox->SetScrollOffset(0.f);
	}
}

void UGsUIHUDQuest::SetTutorialTarget(QuestId InQuestId, bool bHideTeleport)
{
	_tutorialQuestId = InQuestId;
	_bTutorialHideTeleport = bHideTeleport;

	if (0 == _tutorialQuestId)
	{
		OnTutorialQuestListEvent.Unbind();
		_scrollBox->SetAllowOverscroll(true);

		// 리스트 원상복구
		UpdateList();

		return;
	}

	_scrollBox->SetAllowOverscroll(false);

	UpdateList();
}
