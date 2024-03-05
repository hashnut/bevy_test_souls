#include "GsQuestGuideManagement.h"

#include "Management/GsMessageHolder.h"
#include "Message/GsMessageUI.h"
#include "Message/GsMessageContents.h"
#include "Message/GsMessageGameObject.h"
#include "Message/MessageParam/GsUIMessageParam.h"
#include "Message/MessageParam/GsQuestMessageParam.h"

#include "Quest/Table/GsQuestGuideTableCacheSet.h"

#include "DataSchema/Quest/Guide/GsSchemaQuestGuideStory.h"

#include "Management/ScopeGlobal/GsGameDataManager.h"
#include "Management/ScopeGlobal/GsQuestManager.h"
#include "Option/GsServerOption.h"

#include "Shared/Client/SharedEnums/SharedQuestEnum.h"


// 초기화
void FGsQuestGuideManagement::Initialize(FGsQuestGuideTableCacheSet* inGuideQuestTableCacheSet)
{
	BindMessages();

	_guideQuestTableCacheSet = inGuideQuestTableCacheSet;
}

// 클리어
void FGsQuestGuideManagement::Finalize()
{
	UnbindMessages();
}

void FGsQuestGuideManagement::Clear()
{

}

void FGsQuestGuideManagement::BindMessages()
{
	// 이벤트 등록
	FGsMessageHolder* message = GMessage();

	MQuest& msgQuest = message->GetQuest();
	// 메인 퀘스트가 완료 후 보상받았을 때
	_listQuestDelegate.Emplace(
		msgQuest.AddRaw(MessageContentQuest::QUEST_MAIN_REWARDED, this, &FGsQuestGuideManagement::RecvQuestMainRewardedMessge));
	// 가이드 퀘스트가 완료 후 보상받았을 때
	_listQuestDelegate.Emplace(
		msgQuest.AddRaw(MessageContentQuest::QUEST_GUIDE_REWARDED, this, &FGsQuestGuideManagement::RecvQuestGuideRewardedMessge));
	_listQuestDelegate.Emplace(
		msgQuest.AddRaw(MessageContentQuest::QUEST_GUIDE_ACCEPTED, this, &FGsQuestGuideManagement::RecvQuestGuideAcceptedMessge));
	_listQuestDelegate.Emplace(
		msgQuest.AddRaw(MessageContentQuest::QUEST_GUIDE_COMPLETED, this, &FGsQuestGuideManagement::RecvQuestGuideCompletedMessge));
	_listQuestDelegate.Emplace(
		msgQuest.AddRaw(MessageContentQuest::QUEST_GUIDE_HUD_ONOFF, this, &FGsQuestGuideManagement::RecvQuestGuideHudOnOff));

	// 레벨이 변경됐을때
	MUI& msgUI = message->GetUI();
	_listUIDelegate.Emplace(msgUI.AddRaw(MessageUI::LEVEL_UP, this, &FGsQuestGuideManagement::OnChangeLocalPlayerLevel));

	MGameObject& msgGameObject = message->GetGameObject();
	// 로컬pc가 스폰 완료 됐을 때
	_listGameObjectDelegate.Emplace(
		msgGameObject.AddRaw(MessageGameObject::LOCAL_SPAWN_ME_COMPLETE, this, &FGsQuestGuideManagement::OnLocalPlayerSpawnComplete));
}

void FGsQuestGuideManagement::UnbindMessages()
{
	FGsMessageHolder* msg = GMessage();
	if (nullptr != msg)
	{
		if (_listQuestDelegate.Num() != 0)
		{
			for (auto iter : _listQuestDelegate)
			{
				msg->GetQuest().Remove(iter);
			}
			_listQuestDelegate.Empty();
		}

		if (_listUIDelegate.Num() != 0)
		{
			for (auto iter : _listUIDelegate)
			{
				msg->GetUI().Remove(iter);
			}
			_listUIDelegate.Empty();
		}

		if (_listGameObjectDelegate.Num() != 0)
		{
			for (auto iter : _listGameObjectDelegate)
			{
				msg->GetGameObject().Remove(iter);
			}
			_listGameObjectDelegate.Empty();
		}
	}
}

void FGsQuestGuideManagement::RecvQuestMainRewardedMessge(const IGsMessageParam* In_data)
{
	// 메인퀘스트가 완료되면 제한걸려있는 가이드 퀘스트가 해금됐을수도 있으니 확인
	Update();
}

void FGsQuestGuideManagement::RecvQuestGuideAcceptedMessge(const IGsMessageParam* In_data)
{
	Update(GuideListUpdateType::Preserve);
}

void FGsQuestGuideManagement::RecvQuestGuideCompletedMessge(const IGsMessageParam* In_data)
{
	Update(GuideListUpdateType::Preserve);
}

void FGsQuestGuideManagement::RecvQuestGuideRewardedMessge(const IGsMessageParam* In_data)
{
	FGsServerOption* serverOption = GServerOption();
	if (nullptr == serverOption)
		return;

	// 가이드 퀘스트가 완료 되면 
	// on 목록도, off 목록도 삭제
	// 빈자리 다시 채우기

	const FGsStoryParam* msg = In_data->Cast<const FGsStoryParam>();
	
	serverOption->SetQuestGuideOffList(false, msg->_storyId);
	serverOption->SetQuestGuideOnList(false, msg->_storyId);

	Update();
}

void FGsQuestGuideManagement::OnChangeLocalPlayerLevel(const IGsMessageParam* In_data)
{
	const FGsUIMsgParamLevelUp* msg = In_data->Cast<const FGsUIMsgParamLevelUp>();

	uint16 preLevel = msg->_preLevel;
	uint16 currentLevel = msg->_currentLevel;

	// 레벨업을 하거나
	// 렙따를 했을 수 있다.
	// 등록 가능한거 뺄꺼 계산을 다시 해준다.
	Update();
}

void FGsQuestGuideManagement::RecvQuestGuideHudOnOff(const IGsMessageParam* In_data)
{
	FGsServerOption* serverOption = GServerOption();
	if (nullptr == serverOption)
		return;

	const FGsQuestSlotTouchParam* msg = In_data->Cast<const FGsQuestSlotTouchParam>();

	if (msg)
	{
		bool isOn = msg->_isSelected;
		StoryId storyId = msg->_storyId;
		if (isOn)
		{
			serverOption->SetQuestGuideOnList(isOn, storyId);	
			serverOption->SetQuestGuideOffList(!isOn, storyId);
		}
		else
		{			
			serverOption->SetQuestGuideOnList(isOn, storyId);
			serverOption->SetQuestGuideOffList(!isOn, storyId);			
		}

		// 유저 선택으로 키고 껐을 경우에는 자동으로 추가해주지 않는다
		Update(isOn ? GuideListUpdateType::Add : GuideListUpdateType::Subtract);
	}
}

void FGsQuestGuideManagement::OnLocalPlayerSpawnComplete(const IGsMessageParam* In_data)
{
	// 유저가 스폰하면 on 리스트 산출 (HUD 노출되는 갯수 유지)
	Update(GuideListUpdateType::Preserve);
}

void FGsQuestGuideManagement::Update(GuideListUpdateType InType)
{
	FGsServerOption* serverOption = GServerOption();
	if (nullptr == serverOption)
		return;

	// 가이드 퀘스트 수락/완료, 유저 조작, 플레이어 스폰 시, HUD 에 자동추가하지 않음 (C2URWQ-3935)
	int activeGuideQuestCount = MAX_QUEST_GUIDE_HUD_VIEW_COUNT;

	switch (InType)
	{
	case FGsQuestGuideManagement::Add:
		activeGuideQuestCount = FMath::Clamp(_totalOnList.Num() + 1, 0, MAX_QUEST_GUIDE_HUD_VIEW_COUNT);
		break;
	case FGsQuestGuideManagement::Subtract:
		activeGuideQuestCount = FMath::Clamp(_totalOnList.Num() - 1, 0, MAX_QUEST_GUIDE_HUD_VIEW_COUNT);
		break;
	case FGsQuestGuideManagement::Preserve:
		activeGuideQuestCount = serverOption->GetQuestGuideOnList().Num();
		break;
	case FGsQuestGuideManagement::None:
		break;
	default:
		break;
	}

	_totalOnList.Empty();

	_userOffList = serverOption->GetQuestGuideOffList();
	_userOnList = serverOption->GetQuestGuideOnList();		

	// 데이터 체크 및 삭제
	InvalidateCheck();
	// 유저가 등록 한 on 갯수
	int onCount = _totalOnList.Num();

	// 원하는 갯수만큼 이미 등록되어 있을 경우
	if (activeGuideQuestCount <= onCount)
	{
		
	}
	else
	{
		// 유저가 보임 상태로 등록한게 지정된 갯수보다 작다.
		// 시스템 체크로 지정 갯수만큼 채우자

		// 1순위 : 완료 상태로 보상을 받을 수 있는 상태
		// 2순위 : 진행중인 상태
		// 3순위 : 습득 가능 상태

		// 아래 코드는 자동으로 퀘스트를 HUD 리스트에 추가해 주는 코드이다
		// 자동으로 추가해 줄때, serverOption 에도 해당 퀘스트들을 추가해 주어야 한다
		// 유저가 직접 끈 스토리는 제외시킨다.
		do
		{
			StoryId outStoryId = 0;
			if (Claim(outStoryId))
			{
				_totalOnList.Emplace(outStoryId);

				serverOption->SetQuestGuideOnList(true, outStoryId);
				serverOption->SetQuestGuideOffList(false, outStoryId);
			}
			else
			{
				break;
			}

			if (activeGuideQuestCount <= _totalOnList.Num())
			{
				break;
			}

		} while (true);
	}

	GMessage()->GetQuest().SendMessage(MessageContentQuest::QUEST_GUIDE_INVALID_INSTANCE, nullptr);
}

bool FGsQuestGuideManagement::Claim(OUT StoryId& outStoryId)
{
	if (false == GetStoryCompleted(outStoryId))
	{
		if (false == GetStoryProgress(outStoryId))
		{
			if (false == GetStoryWaiting(outStoryId))
			{
				return false;
			}
		}
	}

	return true;
}

void FGsQuestGuideManagement::InvalidateCheck()
{
	FGsServerOption* serverOption = GServerOption();
	if (nullptr == serverOption)
		return;

	// _userOnList 빼고
	// 서버옵션쪽에도 빼고

	TArray<StoryId> deleteList;
	for (auto iter = _userOnList.CreateConstIterator(); iter; ++iter)
	{
		StoryId storyId = static_cast<StoryId>(*iter);

		QuestState outQuestState = QuestState::NONE;
		QuestWindowUISlotType outSlotType = QuestWindowUISlotType::STORY_PROGRESS;
		GSQuest()->GetGuideQuestSlotStateInfo(storyId, outQuestState, outSlotType);
		if (QuestState::REWARDED == outQuestState)
		{
			deleteList.Add(storyId);

			//_userOnList.Remove(storyId);
			serverOption->SetQuestGuideOnList(false, storyId);

			// 다음으로 전진
			//++iter;
		}
	}

	for (StoryId deleteId : deleteList)
	{
		_userOnList.Remove(deleteId);
	}

	_totalOnList.Append(_userOnList);
}

bool FGsQuestGuideManagement::GetStoryCompleted(OUT StoryId& outStoryId)
{
	// 1순위 : 완료 상태로 보상을 받을 수 있는 상태
	outStoryId = GetStoryId(GuideStateType::Completed);
	return (0 < outStoryId)? true : false;
}

bool FGsQuestGuideManagement::GetStoryProgress(OUT StoryId& outStoryId)
{
	// 2순위 : 진행중인 상태
	outStoryId = GetStoryId(GuideStateType::Progress);
	return (0 < outStoryId) ? true : false;
}

bool FGsQuestGuideManagement::GetStoryWaiting(OUT StoryId& outStoryId)
{
	// 3순위 : 습득 가능 상태
	outStoryId = GetStoryId(GuideStateType::Waiting);
	return (0 < outStoryId) ? true : false;
}

StoryId FGsQuestGuideManagement::GetStoryId(GuideStateType inType)
{
	FGsServerOption* serverOption = GServerOption();
	if (nullptr == serverOption)
		return 0;

	const FGsNetUserData* userData = GGameData()->GetUserData();
	if (nullptr == userData)
		return 0;

	uint16 userLevel = userData->mLevel;

	TArray<TSharedPtr<FGsQuestGuideStoryTableCache>>& guideStoryTableCacheList = _guideQuestTableCacheSet->GetGuideStoryTableCache();
	for (TSharedPtr<FGsQuestGuideStoryTableCache> guidePtr : guideStoryTableCacheList)
	{
		QuestId questId = guidePtr->GetQuestId();
		TSharedPtr<FGsQuestData> questData = GSQuest()->GetQuestData(questId);
		if (!questData.IsValid())
			continue;

		// 유저가 off 한건 제외
		StoryId storyId = questData->GetStoryId();
		if (_userOffList.ContainsByPredicate([storyId](StoryId el){
				return el == storyId;
			}))
		{
			continue;
		}
		// 이미 on 해둔것도 제외
		if (_userOnList.ContainsByPredicate([storyId](StoryId el){
				return el == storyId;
			}))
		{
			continue;
		}

		// 이미 on 해둔것도 제외
		if (_totalOnList.ContainsByPredicate([storyId](StoryId el) {
			return el == storyId;
			}))
		{
			continue;
		}

		FGsQuestDynamicData* dynamicData = questData->GetQuestDynamicData();
		if (nullptr == dynamicData)
		{
			dynamicData = questData->ClaimQuestDynamicData();
		}

		if (nullptr == dynamicData)
			continue;
				
		QuestState state = dynamicData->GetQuestState();
		if (state == QuestState::REWARDED)	// 완료한 퀘스트		
			continue;
			
		if (state == QuestState::COMPLETED)
		{
			// 진행중인 퀘스트
			if (GuideStateType::Completed == inType)
			{
				return questData->GetStoryId();
			}
		}
		else if (state == QuestState::ACCEPTED)
		{
			// 진행중인 퀘스트
			if (GuideStateType::Progress == inType)
			{
				return questData->GetStoryId();
			}
		}
		else
		{
			const FGsSchemaQuestGuideStory* schemaQuestGuideStory = guidePtr->GetSchemaQuestStory();
			if (nullptr == schemaQuestGuideStory)
				continue;

			int32 requireLevel = schemaQuestGuideStory->requireLevel;

			if (userLevel < requireLevel)
				continue;	// 요구 레벨 제한에 걸린경우
			
			if (const FGsSchemaQuestMainStory* aSchemaQuestMainStory = schemaQuestGuideStory->requireMainStoryId.GetRow())
			{
				if (GSQuest()->GetCurrentStoryId() <= static_cast<StoryId>(aSchemaQuestMainStory->id))
					continue;	// 요구 메인스토리 제한에 걸린경우
			}

			// 대기 중인 퀘스트
			if (GuideStateType::Waiting == inType)
			{
				return questData->GetStoryId();
			}

			//if (aSchemaQuestMainStory != nullptr)
			//{
			//	// 대기 중인 퀘스트
			//	if (GuideStateType::Waiting == inType)
			//	{
			//		return questData->GetStoryId();
			//	}
			//}
			//else
			//{
			//	if (GSQuest()->GetCurrentStoryId() < static_cast<StoryId>(aSchemaQuestMainStory->id))
			//		continue;	// 요구 메인스토리 제한에 걸린경우
			//	
			//	// 대기 중인 퀘스트
			//	if (GuideStateType::Waiting == inType)
			//	{
			//		return questData->GetStoryId();
			//	}
			//}			
		}		
	}

	return 0;
}