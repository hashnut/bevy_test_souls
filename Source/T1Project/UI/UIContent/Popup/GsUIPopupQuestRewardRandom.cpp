// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUIPopupQuestRewardRandom.h"

#include "Net/GsNetSendServiceWorld.h"
#include "Net/GsNetSendServiceQuest.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"

#include "UI/UIControlLib/ContentWidget/GsButton.h"
#include "UI/UIContent/Common/Reward/GsUIRewardInfoBase.h"

#include "Management/ScopeGlobal/GsQuestManager.h"
#include "Management/GsMessageHolder.h"
#include "Message/MessageParam/GsUIMessageParam.h"
#include "Message/MessageParam/GsQuestMessageParam.h"
#include "Management/ScopeGame/GsEventProgressManager.h"

#include "TextBlock.h"
#include "Message/GsMessageUI.h"
#include "Reward/GsRewardHelper.h"



void UGsUIPopupQuestRewardRandom::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	_btnClose->OnClicked.AddDynamic(this, &UGsUIPopupQuestRewardRandom::OnClickOk);

	// 퀘스트 완료창에서는 슬롯 선택기능을 사용 가능하게 켜준다.
	// ALL 타입 등 선택이 필요 없을 경우 UGsUIRewardInfoBase 내부 로직에서 클릭을 막는다.
	_rewardInfo->SetEnableToggleGroup(true);
}

void UGsUIPopupQuestRewardRandom::NativeConstruct()
{
	Super::NativeConstruct();

	BindMessages();
	
	_btnClose->SetIsEnabled(false);
}

void UGsUIPopupQuestRewardRandom::NativeDestruct()
{
	UnbindMessages();	

	_remainTime = 0.f;

	Super::NativeDestruct();
}

void UGsUIPopupQuestRewardRandom::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (0 < _remainTime)
	{
		_remainTime -= InDeltaTime;
		if (0 >= _remainTime)
		{
			_btnClose->SetIsEnabled(true);
			_remainTime = 0.f;
		}
	}
}

void UGsUIPopupQuestRewardRandom::CloseInternal()
{
	if (FGsEventProgressManager* eventProgressMgr = GSEventProgress())
	{
		eventProgressMgr->OnReqFinishEvent(EGsEventProgressType::UI_POPUP_QUESTCOMPLETE);
	}

	Super::CloseInternal();
}

void UGsUIPopupQuestRewardRandom::OnInputOk()
{
	Close();
}

void UGsUIPopupQuestRewardRandom::OnInputCancel()
{
	Close();
}

void UGsUIPopupQuestRewardRandom::BindMessages()
{
	FGsMessageHolder* msgMgr = GMessage();
	if (nullptr == msgMgr)
	{
		return;
	}

	_msgUIHandleList.Emplace(msgMgr->GetUI().AddUObject(MessageUI::REWARD_RANDOM_ITEMBAG_INDEX,
		this, &UGsUIPopupQuestRewardRandom::OnReceiveRandomItemBagIndex));
	_msgUIHandleList.Emplace(msgMgr->GetUI().AddUObject(MessageUI::QUEST_REWARD_FAIL,
		this, &UGsUIPopupQuestRewardRandom::OnRecvRewardResultFailed));

	_msgGameObjectHandleList.Emplace(msgMgr->GetGameObject().AddUObject(MessageGameObject::QUEST_REPEAT_REWARD_BOX_RECEIVE,
		this, &UGsUIPopupQuestRewardRandom::OnReceiveRepeatRewardBox));
}

void UGsUIPopupQuestRewardRandom::UnbindMessages()
{
	FGsMessageHolder* msgMgr = GMessage();
	if (nullptr == msgMgr)
	{
		return;
	}

	MUI& msgUI = msgMgr->GetUI();
	for (MsgUIHandle& handle : _msgUIHandleList)
	{
		msgUI.Remove(handle);
	}
	_msgUIHandleList.Empty();

	MGameObject& msgGameObject = msgMgr->GetGameObject();
	for (MsgGameObjHandle& handle : _msgGameObjectHandleList)
	{
		msgGameObject.Remove(handle);
	}
	_msgGameObjectHandleList.Empty();
}

void UGsUIPopupQuestRewardRandom::SetData(const IGsMessageParam* InParam)
{
	Super::SetData(InParam);

	bool bIsRequestRepeatReward = false;

	// 해당 타입일 경우, 아이콘 정보가 없으면 서버에 요청
	if (RewardType::PICK_REWARD_BOX_LIST == FGsRewardHelper::GetRewardType(_rewardMainId) ||
		RewardType::PICK_REWARD_BOX_LIST == FGsRewardHelper::GetRewardType(_rewardSubId))
	{
		bIsRequestRepeatReward = SendRequestRepeatRewardBox();
	}

	// 아이콘 정보 세팅
	if (false == bIsRequestRepeatReward)
	{
		SetRewardData(_rewardMainId, _rewardSubId);
	}

	_textTitle->SetColorAndOpacity(GSQuest()->GetTextColor(_questType, _storyId));
	
	// 랜덤은 연출을 위해 먼저 결과를 받는다
	SendRequestReward();

	// n초간 닫기 버튼 막기
	_remainTime = _randomWaitTime;
}

bool UGsUIPopupQuestRewardRandom::SendRequestRepeatRewardBox()
{
	if (false == IsTest())
	{
		TArray<RewardBoxItemIdPair> boxIdPairList;
		if (false == FGsRewardHelper::GetRewardBoxItemIdPairList(_questId, _questIndex, boxIdPairList))
		{
			FGsNetSendServiceQuest::SendReqQuestRepeatRewardBox(_questIndex);
			return true;
		}
	}

	return false;
}

void UGsUIPopupQuestRewardRandom::SendRequestReward()
{
	if (IsTest())
	{
		return;
	}

	switch (_questType)
	{
	case QuestType::MAIN:
	{
		FGsNetSendServiceWorld::SendQuestMainReward(_questId, 
			INVALID_ITEM_BAG_INDEX, INVALID_ITEM_BAG_INDEX);
	}
	break;
	case QuestType::SUB:
	{
		FGsNetSendServiceWorld::SendQuestSubReward(_storyId, _questId,
			INVALID_ITEM_BAG_INDEX, INVALID_ITEM_BAG_INDEX);
	}
	break;
	case QuestType::GUIDE:
	{
		FGsNetSendServiceWorld::SendQuestGuideReward(_questId,
			INVALID_ITEM_BAG_INDEX, INVALID_ITEM_BAG_INDEX);
	}
	break;
	case QuestType::REPEAT:
	{
		FGsNetSendServiceWorld::SendQuestRepeatReward(_storyId, _questId, _questIndex,
			INVALID_ITEM_BAG_INDEX, INVALID_ITEM_BAG_INDEX);
	}
	break;
	case QuestType::GUILD:
	{
		FGsNetSendServiceQuest::SendReqQuestGuildReward(_questId);
	}
	break;
	}
}

void UGsUIPopupQuestRewardRandom::OnReceiveRepeatRewardBox(const IGsMessageParam* InParam)
{
	// 랜덤 보상 목록 수령 후 보상 목록 업데이트
	if (const FGsQuestRepeatPopUpParam* param = InParam->Cast<const FGsQuestRepeatPopUpParam>())
	{
		if (param->_questIndex == _questIndex)
		{
			SetRewardData(_rewardMainId, _rewardSubId);
		}
	}
}

void UGsUIPopupQuestRewardRandom::OnReceiveRandomItemBagIndex(const IGsMessageParam* InParam)
{
	if (false == _rewardInfo->IsRandomEffect())
	{
		return;
	}

	const FGsUIMsgParamRewardRandomItemBag* param = InParam->Cast<const FGsUIMsgParamRewardRandomItemBag>();
	if (param)
	{
		if (param->_questId == _questId)
		{
			if (INVALID_ITEM_BAG_INDEX != param->_mainBagIndex)
			{
				_rewardInfo->SetReceivedIndex(param->_mainBagIndex);
			}
			else if (INVALID_ITEM_BAG_INDEX != param->_subBagIndex)
			{
				_rewardInfo->SetReceivedIndex(param->_subBagIndex);
			}
		}
	}
}

void UGsUIPopupQuestRewardRandom::OnRecvRewardResultFailed(const IGsMessageParam* InParam)
{
	_remainTime = 0;
	_btnClose->SetIsEnabled(true);
}
