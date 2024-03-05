// Fill out your copyright notice in the Description page of Project Settings.


#include "GsQuestHandler.h"

#include "Shared/Shared/SharedPackets/PD_World_Client_Bypass.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "Shared/Client/SharedEnums/SharedQuestEnum.h"
#include "SharedPointer.h"

#include "Net/GsNetSendServiceWorld.h"
#include "Net/GsNetSendServiceQuest.h"

#include "GameFlow/GameContents/ContentsGame/GsGameStateBoss.h"

#include "Management/GsMessageHolder.h"
#include "Management/ScopeGlobal/GsUIManager.h"
#include "Management/ScopeGlobal/GsQuestManager.h"
#include "Management/ScopeGlobal/GsGameDataManager.h"
#include "Management/ScopeGame/GsEventProgressManager.h"
#include "Management/ScopeGlobal/GsOptionManager.h"
#include "Management/ScopeGame/GsAIManager.h"
#include "Management/ScopeGame/GsGameObjectManager.h"
#include "Management/ScopeGame/GsTutorialManager.h"
#include "Management/ScopeGame/GsObserverManager.h"

#include "Quest/GsQuestPoolManager.h"
#include "Quest/GsQuestMainInstance.h"
#include "Quest/Management/GsQuestTargetManagement.h"
#include "Quest/Management/GsQuestActionManagement.h"
#include "Quest/GsQuestSubInstance.h"
#include "Quest/GsQuestGuideInstance.h"
#include "Quest/Repeat/GsSchemaQuestRepeatStory.h"
#include "Quest/Repeat/GsSchemaQuestRepeatScroll.h"
#include "Quest/Repeat/GsSchemaQuestRepeatScrollRefreshInfoSet.h"
#include "Quest/Icon/GsSchemaQuestImg.h"
#include "Quest/GsQuestData.h"
#include "Runtime/DataCenter/Public/Quest/GsQuestEnum.h"
#include "Quest/GsQuestInstance.h"
#include "Quest/Table/GsQuestGuideTableCacheSet.h"
#include "Quest/Table/GsQuestSubTableCacheSet.h"
#include "Quest/Table/GsQuestGuildTableCacheSet.h"
#include "Quest/Table/GsQuestRepeatTableCacheSet.h"
#include "Quest/Guild/GsSchemaQuestGuildStory.h"

#include "UI/UILib/Base/GsUIWidgetBase.h"

#include "UI/UIContent/Popup/GsUIPopupTeleport.h"
#include "UI/UIContent/Popup/GsUIPopupYesNo.h"
#include "UI/UIContent/Popup/GsUIPopupYesNoTwoMessage.h"
#include "UI/UIContent/Tray/GsUITrayMovie.h"
#include "UI/UIContent/Helper/GsUIHelper.h"
#include "UI/UIContent/Popup/GsUIPopupYesNoCurrencyRefreshCount.h"
#include "UI/UIContent/Popup/GsUIPopupQuestAccept.h"
#include "UI/UIContent/Popup/GsUIPopupQuestAcceptRepeat.h"
#include "UI/UIContent/Popup/GsUIPopupQuestReward.h"
#include "UI/UIContent/Popup/GsUIPopupQuestRewardRandom.h"

#include "Message/MessageParam/GsMessageParam.h"
#include "Message/MessageParam/GsQuestMessageParam.h"
#include "Message/MessageParam/GsAIMessageParam.h"
#include "Message/MessageParam/GsInteractionMessageParam.h"
#include "Message/MessageParam/GsGameObjectMessageParam.h"

#include "Input/GsInputEventMsgBase.h"
#include "Input/TouchMove/FGsAutoMoveBlockList.h"
#include "Option/GsServerOption.h"

#include "GameObject/Define/GsGameObjectDefine.h"
#include "GameObject/ObjectClass/GsGameObjectBase.h"
#include "Gameobject/Objectclass/GsGameObjectLocalPlayer.h"
#include "GameObject/Interaction/GsInteractionHandlerLocalPlayer.h"

#include "Reward/GsRewardHelper.h"

#include "Currency/GsCostOption.h"
#include "Currency/GsCostPackage.h"
#include "Currency/GsCurrencyHelper.h"

#include "GameObject/Target/GsTargetHandlerBase.h"

#include "ContentsEvent/GsContentsEventTeleportCost.h"

#include "T1Project.h"
#include "EventProgress/GsEventProgressDefine.h"
#include "UTIL/GsText.h"
#include "UTIL/GsTableUtil.h"
#include "Quest/Management/GsQuestGuideManagement.h"
#include "DataCenter/Public/Quest/GsGuideQuestWindowEnum.h"
#include "Quest/GsQuestRepeatInstance.h"



void FGsQuestHandler::Initialize(UGsGameObjectBase* Local)
{
	_owner = Local->CastGameObject<UGsGameObjectLocalPlayer>();
	_questPoolManager = GSQuest()->GetQuestPoolManager();

	// 이벤트 등록
	FGsMessageHolder* message = GMessage();
	// 퀘스트UI에서 텔레포트버튼이 눌러 졌을 때
	_listGameObjectDelegate.Emplace(
		message->GetGameObject().AddRaw(MessageGameObject::QUEST_UIACTION_TELEPORT, this, &FGsQuestHandler::OnTeleport));

	_listGameObjectDelegate.Emplace(
		message->GetGameObject().AddRaw(MessageGameObject::QUEST_UIACTION_NPC_TO_TELEPORT, this, &FGsQuestHandler::OnNpcToTeleport));

	_listGameObjectDelegate.Emplace(
		message->GetGameObject().AddRaw(MessageGameObject::QUEST_UIACTION_NPC_TO_AUTOPROGRESS, this, &FGsQuestHandler::OnNpcToAutoProgress));

	_listQuestDelegate.Emplace(
		message->GetQuest().AddRaw(MessageContentQuest::QUEST_UIACTION_GIVEUP, this, &FGsQuestHandler::OnQuestGiveUp));

	// 퀘스트UI에서 터치가 발생 했을 때
	_listGameObjectDelegate.Emplace(
		message->GetGameObject().AddRaw(MessageGameObject::QUEST_UIACTION_TOUCH, this, &FGsQuestHandler::OnTouchQuest));

	_listGameObjectDelegate.Emplace(
		message->GetGameObject().AddRaw(MessageGameObject::QUEST_GUIDE_POPUP_ACCEPT, this, &FGsQuestHandler::OpenPopupQuestGuideAccept));

	_listGameObjectDelegate.Emplace(
		message->GetGameObject().AddRaw(MessageGameObject::QUEST_POPUP_ACCEPT, this, &FGsQuestHandler::OpenPopupQuestAccept));

	_listGameObjectDelegate.Emplace(
		message->GetGameObject().AddRaw(MessageGameObject::QUEST_GUIDE_POPUP_REWARD, this, &FGsQuestHandler::OpenPopupQuestGuideReward));

	_listGameObjectDelegate.Emplace(
		message->GetGameObject().AddRaw(MessageGameObject::QUEST_GUIDE_AUTO_PROGRESS, this, &FGsQuestHandler::OnQuestGuideAutoProgress));

	_listGameObjectDelegate.Emplace(
		message->GetGameObject().AddRaw(MessageGameObject::QUEST_COMPLETE_REWARD, this, &FGsQuestHandler::OpenPopupQuestReward));

	// 퀘스트 자동 진행이 끝났을 때
	_listGameObjectDelegate.Emplace(
		message->GetGameObject().AddRaw(MessageGameObject::AI_QUEST_AUTO_MOVE_END, this, &FGsQuestHandler::OnQuestAutoMoveEnd));

	// 퀘스트 자동 진행 시 맵간 이동이 일어날 때 워프 위치 알림
	_listGameObjectDelegate.Emplace(
		message->GetGameObject().AddRaw(MessageGameObject::AI_MOVE_NEXT_MAP_WARP, this, &FGsQuestHandler::OnQuestAutoMoveNextMap));

	_listGameObjectDelegate.Emplace(
		message->GetGameObject().AddRaw(MessageGameObject::OBJ_WARP_START, this, &FGsQuestHandler::OnQuestAutoMoveWarpStart));

	//// npc 인터랙션 버튼을 수동으로 클릭 했을 때
	_listGameObjectDelegate.Emplace(
		message->GetGameObject().AddRaw(MessageGameObject::INTERACTION_CONTENTS_BY_NPC_START, this, &FGsQuestHandler::OnInteractionNpcContents));

	// prop start 인터랙션이 발생 했을 때
	_listGameObjectDelegate.Emplace(
		message->GetGameObject().AddRaw(MessageGameObject::INTERACTION_CONTENTS_BY_PROP_START, this, &FGsQuestHandler::OnInteractionPropStart));

	// prop end 인터랙션이 발생 했을 때
	_listGameObjectDelegate.Emplace(
		message->GetGameObject().AddRaw(MessageGameObject::INTERACTION_CONTENTS_BY_PROP_END, this, &FGsQuestHandler::OnInteractionPropEnd));

	// 로컬pc가 스폰 완료 됐을 때
	_listGameObjectDelegate.Emplace(
		message->GetGameObject().AddRaw(MessageGameObject::LOCAL_SPAWN_ME_COMPLETE, this, &FGsQuestHandler::OnLocalPlayerSpawnComplete));

	_listGameObjectDelegate.Emplace(
		message->GetGameObject().AddRaw(MessageGameObject::LOCAL_SPAWN_ME_ACTION, this, &FGsQuestHandler::OnLocalPlayerSpawnAction));

	_listGameObjectDelegate.Emplace(
		message->GetGameObject().AddRaw(MessageGameObject::LOCAL_DIE, this, &FGsQuestHandler::OnLocalPlayerDie));

	// 유저가 허도 퀘스트오토 버튼 눌렀을때
	_listGameObjectDelegate.Emplace(
		message->GetGameObject().AddRaw(MessageGameObject::AI_QUEST_AUTO_BUTTON_CLICK, this, &FGsQuestHandler::OnUIActionHudAuto));

	// 퀘스트 액티브 끄기(다른 컨텐츠에서)
	_listGameObjectDelegate.Emplace(
		message->GetGameObject().AddRaw(MessageGameObject::QUEST_ACTIVE_OFF, this, &FGsQuestHandler::OnQuestActiveOff));

	_listGameObjectDelegate.Emplace(
		message->GetGameObject().AddRaw(MessageGameObject::QUEST_AUTOPROGRESS_REQUEST, this, &FGsQuestHandler::OnAutoProgressRequest));

	_listGameObjectDelegate.Emplace(
		message->GetGameObject().AddRaw(MessageGameObject::QUEST_WINDOW_OPEN, this, &FGsQuestHandler::OnQuestWindowOpen));

	_listGameObjectDelegate.Emplace(
		message->GetGameObject().AddRaw(MessageGameObject::QUEST_REPEAT_REWARD_REFRESH, this, &FGsQuestHandler::OnQuestRepeatRewardRefresh));

	// 유저가 조이스틱을 눌렀을 때		
	_actionDelegates.Emplace(
		message->GetInput().AddRaw(MessageInput::PressJoystick, this, &FGsQuestHandler::OnActiveAIOffQuestAuto));

	// 유저가 터치 이동 시도할때
	_actionDelegates.Emplace(
		message->GetInput().AddRaw(MessageInput::TerrainTouchMove, this, &FGsQuestHandler::OnActiveAIOffQuestAuto));

	_listGameObjectDelegate.Emplace(
		message->GetGameObject().AddRaw(MessageGameObject::LOCAL_TARGET_CHANGED, this, &FGsQuestHandler::OnLocalTargetChanged));

	// 시네마틱이 끝났을 때
	_listUiDelegate.Emplace(
		message->GetUI().AddRaw(MessageUI::STOP_MOVIE, this, &FGsQuestHandler::OnSequencePlayerStop));

	_listUiDelegate.Emplace(
		message->GetUI().AddRaw(MessageUI::SCAN_CLICK, this, &FGsQuestHandler::OnScanClick));

	_listQuestDelegate.Emplace(
		message->GetQuest().AddRaw(MessageContentQuest::QUEST_GUIDE_INVALID_INSTANCE, this, &FGsQuestHandler::InvalidGuideInstance));	

	// user가 레벨업 했을 때
	_levelHandler = message->GetUserBasicInfo().AddRaw(MessageUserInfo::LEVEL, this, &FGsQuestHandler::OnChangeLocalPlayerLevel);

	_listGameObjectDelegate.Emplace(
		message->GetGameObject().AddRaw(MessageGameObject::LOCAL_CAMP_HIT, this, &FGsQuestHandler::OnLocalCampHit));

	_itemContentsDelegateList.Emplace(
		message->GetItemContents().AddRaw(MessageContentItem::USE_WARP_ITEM, this, &FGsQuestHandler::OnUseWarpItem));

	_listGameObjectDelegate.Emplace(
		message->GetGameObject().AddRaw(MessageGameObject::LOCAL_CONTENTS_WARP_REQUEST, this, &FGsQuestHandler::OnRequestContentsWarp));

	_listGameObjectDelegate.Emplace(
		message->GetGameObject().AddRaw(MessageGameObject::LOCAL_REACTION, this, &FGsQuestHandler::OnLocalReaction));

	_listSystemDelegate.Emplace(
		message->GetSystem().AddRaw(MessageSystem::WARP_WITH_DESPAWN_READY, this, &FGsQuestHandler::OnWarpReady));

	_listSystemDelegate.Emplace(
		message->GetSystem().AddRaw(MessageSystem::WARP_FAILUER, this, &FGsQuestHandler::OnWarpFailure));
}

void FGsQuestHandler::Finalize()
{
	FlushAllInstance();

	FGsMessageHolder* msg = GMessage();

	if (_listQuestDelegate.Num() != 0)
	{
		for (auto iter : _listQuestDelegate)
		{
			msg->GetQuest().Remove(iter);
		}
		_listQuestDelegate.Empty();
	}

	if (_listGameObjectDelegate.Num() != 0)
	{
		for (auto& iter : _listGameObjectDelegate)
		{
			msg->GetGameObject().Remove(iter);
		}
		_listGameObjectDelegate.Empty();
	}

	if (_actionDelegates.Num() != 0)
	{
		for (auto iter : _actionDelegates)
		{
			msg->GetInput().Remove(iter);
		}
		_actionDelegates.Empty();
	}

	if (_listSystemDelegate.Num() != 0)
	{
		for (auto iter : _listSystemDelegate)
		{
			msg->GetSystem().Remove(iter);
		}
		_listSystemDelegate.Empty();
	}

	if (_listUiDelegate.Num() != 0)
	{
		for (auto iter : _listUiDelegate)
		{
			msg->GetUI().Remove(iter);
		}
		_listUiDelegate.Empty();
	}

	if (_itemContentsDelegateList.Num() != 0)
	{
		for (auto iter : _itemContentsDelegateList)
		{
			msg->GetItemContents().Remove(iter);
		}
		_itemContentsDelegateList.Empty();
	}

	msg->GetUserBasicInfo().Remove(_levelHandler);	
}

void FGsQuestHandler::OnTouchQuest(const IGsMessageParam* In_data)
{
	const FGsQuestSlotTouchParam* touchParam =
		In_data->Cast<const FGsQuestSlotTouchParam>();

	const QuestKey& questKey = touchParam->_questKey;
	StoryId storyId = touchParam->_storyId;

	FGsQuestInstance* questInstance = FindQuestInstance(questKey, touchParam->_storyId);
	if (nullptr == questInstance)
	{
		// 로그 삽입
		// 터치가 발생했는데 존재하지않는 퀘스트이다.
		return;
	}

	EGsQuestUIState outState;
	questInstance->GetQuestUIState(outState);

	//if (GSQuest()->GetDebugTest())
	//{
#if WITH_EDITOR
		GSLOG(Warning, TEXT("[Quest] OnTouchQuest QuestId : [%llu], QuestState : [%s], QuestUIState : [%s]"), questInstance->GetQuestId(),
			*GetEnumToString(QuestState, questInstance->GetQuestState()), *GetEnumToString(EGsQuestUIState, outState));
#endif
	//}

	switch (outState)
	{
	case EGsQuestUIState::QUEST_STATE_REWARD:
	{
		if (false == IsInRequestList(questInstance->GetQuestKey()))
		{
			if (questInstance->GetQuestType() == QuestType::REPEAT)
				OpenPopupQuestRepeatReward(questInstance);
			else
				OpenPopupQuestReward(questInstance);
		}
	}
	break;
	case EGsQuestUIState::QUEST_STATE_LEVEL_LOCKED:
	{
		// 레벨 제한이라고 티커 연출 추가
	}
	break;
	case EGsQuestUIState::QUEST_STATE_PROGRESS:
	{
		// 자동진행 연결		
		if (true == touchParam->_isSelected)
		{
			if (questInstance->GetQuestState() == QuestState::COMPLETED)
			{
				if (false == IsInRequestList(questInstance->GetQuestNextKey()))
				{
					// 이 상태는 자동으로 다음 퀘스트를 습득하거나
					// 이곳을 눌러 퀘스트를 완료해주세요. 상태로 넘어가기 때문에 유저가 클릭할수있는 상태가 아니다.
					// 클릭이 됐다는건 어떤 이유에서 다음 상태로 넘어가지 못하는 이슈가 발생했다는 뜻이다.
					// 다시 다음 퀘스트 습득을 요청하도록 하자.
					questInstance->AddNextQuest();
				}
			}
			else
			{
				if (false == IsInRequestList(questInstance->GetQuestKey()))
				{
					// 자동이동 start
					if (!questInstance->StartAutoProgress(touchParam->_isAuto))
					{
						// ui 갱신
						GMessage()->GetContentsHud().SendMessage(MessageContentHud::INVALIDATE_QUEST);
					}
				}
			}
		}
		else
		{
			OnUserAutoMoveStop();
		}
		
	}
	break;
	case EGsQuestUIState::QUEST_STATE_READY:
	{
		if (false == IsInRequestList(questInstance->GetQuestKey()))
			OpenPopupQuestAccept(questInstance);
	}
	break;
	default:
		break;
	}
}

void FGsQuestHandler::OpenPopupQuestAccept(TSharedPtr<FGsQuestData> inQuestData, SpawnId inSpwanId)
{
	FGsQuestSubData* subData = static_cast<FGsQuestSubData*>(inQuestData.Get());
	if (nullptr == subData)
		return;

	TSharedPtr<FGsQuestSubStoryTableCache> storyTableCache = subData->GetQuestSubStoryTableCache();
	if (!storyTableCache.IsValid())
		return;

	TSharedPtr<FGsQuestTableCache> questTableCache = subData->GetQuestTableCache();
	if (!questTableCache.IsValid())
		return;

	const FGsSchemaQuestSubStory* story = storyTableCache->GetSchemaQuestStory();
	if (nullptr == story)
	{
		return;
	}

	RewardId mainRewardId = INVALID_REWARD_ID;
	RewardType mainRewardType = RewardType::MAX;
	if (const FGsSchemaRewardData* mainReward = story->mainRewardId.GetRow())
	{
		mainRewardId = mainReward->id;
		mainRewardType = mainReward->rewardType;
	}

	RewardId subRewardId = INVALID_REWARD_ID;
	RewardType subRewardType = RewardType::MAX;	
	if (const FGsSchemaRewardData* subReward = story->subRewardId.GetRow())
	{
		subRewardId = subReward->id;
		subRewardType = subReward->rewardType;
	}

	QuestContentsType contentsType = QuestContentsType::NORMAL;
	if (const FGsSchemaQuest* schemaQuest = questTableCache->GetSchemaQuest())
	{
		contentsType = schemaQuest->questContentsType;
	}

	FGsQuestPopUpParam param(story->id, questTableCache->GetQuestId(), INVALID_QUEST_INDEX,
		story->storyTitleText, story->storyDescriptionText, 
		mainRewardId, subRewardId,
		inSpwanId, QuestType::SUB, contentsType);

	OpenPopupAccept(param._questType, &param, mainRewardType, subRewardType);
}

void FGsQuestHandler::OpenPopupQuestRepeatAccept(const QuestIndex inQuestIndex)
{	
	FGsQuestRepeatPopUpParam param;
	RewardType mainRewardType = RewardType::MAX;
	RewardType subRewardType = RewardType::MAX;

	if (GetQuestRepeatPopupParam(inQuestIndex, param, mainRewardType, subRewardType))
	{
		OpenPopupAccept(QuestType::REPEAT, &param, mainRewardType, subRewardType);
	}
}

void FGsQuestHandler::RefrehPopupQuestRepeat(const QuestIndex inQuestIndex)
{
	OpenPopupQuestRepeatAccept(inQuestIndex);
}

void FGsQuestHandler::OpenCorrespondingWindow(const FGsSchemaQuestGuideStory* InSchemaQuestStory)
{
	FGsMessageHolder* messageHolder = GMessage();
	if (nullptr == messageHolder)
	{
		GSLOG(Error, TEXT("nullptr == messageHolder"));
		return;
	}

	if (nullptr == InSchemaQuestStory)
	{
		GSLOG(Error, TEXT("nullptr == InSchemaQuestStory"));
		return;
	}

	EGsGuideQuestWindowType windowType = InSchemaQuestStory->guideQuestWindow;

	// 특정 윈도우를 열 필요가 없을 경우 스트링만 출력한다
	if (EGsGuideQuestWindowType::ACTIVE_HUD == windowType)
	{
		ShowDetailDescText(InSchemaQuestStory);
		return;
	}

	auto messageContentsToWindow = [](EGsGuideQuestWindowType InWindowType) -> MessageContents
		{
			return static_cast<MessageContents>(static_cast<uint16>(InWindowType));
		};

	messageHolder->GetContents().SendMessage(messageContentsToWindow(windowType));

	// 예외로 보스일 경우, 첫 보스를 선택한다 -> 이건 기본적으로 보스 컨텐츠에서 알아서 처리했어야 하는 부분이다.
	if (messageContentsToWindow(windowType) == MessageContents::ACTIVE_BOSS)
	{
		FGsGameStateBoss::GetInstance()->SelectFirstFieldBoss();
	}
}

void FGsQuestHandler::ShowDetailDescText(const FGsSchemaQuestGuideStory* InSchemaQuestStory)
{
	if (nullptr == InSchemaQuestStory)
	{
		GSLOG(Error, TEXT("nullptr == InSchemaQuestStory"));
		return;
	}

	FText detailText = InSchemaQuestStory->storyDetailDescText;
	FGsUIHelper::TrayMessageTicker(detailText);
}

bool FGsQuestHandler::GetQuestRepeatPopupParam(IN const QuestIndex inQuestIndex, 
	OUT FGsQuestRepeatPopUpParam& outPopUpParam,
	OUT RewardType& outMainRewardType,
	OUT RewardType& outSubRewardType)
{
	UGsQuestManager* questManager = GSQuest();
	if (nullptr == questManager)
		return false;

	FGsQuestInstance* questRepeatInstance = nullptr;
	for (FGsQuestInstance* questInstance : _questInstanceList)
	{
		if (nullptr == questInstance)
			continue;

		if (questInstance->GetQuestType() != QuestType::REPEAT)
			continue;

		if (inQuestIndex == questInstance->GetQuestIndex())
		{
			questRepeatInstance = questInstance;
			break;
		}
	}

	if (nullptr == questRepeatInstance)
		return false;

	StoryId storyId = questRepeatInstance->GetStoryId();
	QuestId questId = questRepeatInstance->GetQuestId();
	QuestIndex questIndex = questRepeatInstance->GetQuestIndex();

	TSharedPtr<FGsQuestRepeatStoryTableCache> storyTableCache = GSQuest()->GetQuestRepeatStoryTableCache(storyId);
	if (!storyTableCache.IsValid())
		return false;

	const FGsSchemaQuestRepeatStory* schemaQuestRepeatStory = storyTableCache->GetSchemaQuestStory();
	if (nullptr == schemaQuestRepeatStory)
		return false;


	FSoftObjectPath imgPath;
	const FGsSchemaQuestImg* questImg = schemaQuestRepeatStory->iconRes.GetRow();
	if (nullptr != questImg)
	{
		imgPath = questImg->resPath;
	}

	RewardId mainRewardId = 0;
	if (const FGsSchemaRewardData* mainReward = schemaQuestRepeatStory->mainRewardId.GetRow())
	{
		mainRewardId = mainReward->id;
		outMainRewardType = mainReward->rewardType;
	}

	RewardId subRewardId = 0;	
	if (const FGsSchemaRewardData* subReward = schemaQuestRepeatStory->subRewardId.GetRow())
	{
		subRewardId = subReward->id;
		outSubRewardType = subReward->rewardType;
	}

	bool isRefreshReward = (schemaQuestRepeatStory->repeatStoryType == RepeatStoryType::SCROLL) ? true : false;

	FGsQuestRepeatPopUpParam parm(storyId, questId, questIndex,
		schemaQuestRepeatStory->storyTitleText, schemaQuestRepeatStory->storyDescriptionText, 
		mainRewardId, subRewardId,
		imgPath, isRefreshReward);

	outPopUpParam = parm;

	return true;
}

bool FGsQuestHandler::OpenPopupQuestAccept(FGsQuestInstance* inQuest)
{
	FText titleText;
	inQuest->GetTitleText(titleText);
	FText descText;
	inQuest->GetDescText(descText);

	FGsQuestPopUpParam param(inQuest->GetStoryId(), inQuest->GetQuestId(), inQuest->GetQuestIndex(), titleText, descText,
		inQuest->GetMainRewardId(), inQuest->GetSubRewardId(), 0, inQuest->GetQuestType());

	RewardType mainRewardType = RewardType::MAX;
	if (const FGsSchemaRewardData* mainReward = UGsTableUtil::FindRowById<UGsTableRewardData, FGsSchemaRewardData>(inQuest->GetMainRewardId()))
	{
		mainRewardType = mainReward->rewardType;
	}

	RewardType subRewardType = RewardType::MAX;
	if (const FGsSchemaRewardData* subReward = UGsTableUtil::FindRowById<UGsTableRewardData, FGsSchemaRewardData>(inQuest->GetSubRewardId()))
	{
		subRewardType = subReward->rewardType;
	}
		
	return OpenPopupAccept(param._questType, &param, mainRewardType, subRewardType);
}

void FGsQuestHandler::OpenPopupQuestAccept(const IGsMessageParam* In_data)
{
	const FGsStoryParam* storyParam =
		In_data->Cast<const FGsStoryParam>();

	_reserveStoryIdAccpet = storyParam->_storyId;
	GSEventProgress()->OnReqStartEvent(EGsEventProgressType::UI_POPUP_QUESTACCEPT, this);	
}

void FGsQuestHandler::OpenPopupQuestGuideAccept(const IGsMessageParam* In_data)
{
	const FGsStoryParam* storyParam =
		In_data->Cast<const FGsStoryParam>();

	StoryId storyId = storyParam->_storyId;

	TSharedPtr<FGsQuestGuideStoryTableCache> storyTableCache = GSQuest()->GetGuideStoryTableCache(storyId);
	if (!storyTableCache.IsValid())
		return;

	if (true == IsInRequestList(QuestKey(storyTableCache->GetQuestId())))
		return;

	const FGsSchemaQuestGuideStory* schemaQuestGuideStory = storyTableCache->GetSchemaQuestStory();
	if (nullptr == schemaQuestGuideStory)
		return;

	RewardId mainRewardId = 0;
	RewardType mainRewardType = RewardType::MAX;
	if (const FGsSchemaRewardData* mainReward = schemaQuestGuideStory->mainRewardId.GetRow())
	{
		mainRewardId = mainReward->id;
		mainRewardType = mainReward->rewardType;
	}

	RewardId subRewardId = 0;
	RewardType subRewardType = RewardType::MAX;
	if (const FGsSchemaRewardData* subReward = schemaQuestGuideStory->subRewardId.GetRow())
	{
		subRewardId = subReward->id;
		subRewardType = subReward->rewardType;
	}

	FGsQuestPopUpParam param(storyId, storyTableCache->GetQuestId(), INVALID_QUEST_INDEX, 
		schemaQuestGuideStory->storyTitleText, schemaQuestGuideStory->storyDescriptionText,
		mainRewardId, subRewardId, 0, QuestType::GUIDE);

	OpenPopupAccept(param._questType, &param, mainRewardType, subRewardType);
}

void FGsQuestHandler::OnQuestGuideAutoProgress(const IGsMessageParam* In_data)
{
	const FGsStoryParam* storyParam =
		In_data->Cast<const FGsStoryParam>();

	StoryId storyId = storyParam->_storyId;

	TSharedPtr<FGsQuestGuideStoryTableCache> storyTableCache = GSQuest()->GetGuideStoryTableCache(storyId);
	if (!storyTableCache.IsValid())
		return;

	if (true == IsInRequestList(QuestKey(storyTableCache->GetQuestId())))
		return;

	const FGsSchemaQuestGuideStory* schemaQuestGuideStory = storyTableCache->GetSchemaQuestStory();
	if (nullptr == schemaQuestGuideStory)
		return;

	FGsObserverManager* observerMgr = GSObserver();
	if (nullptr == observerMgr)
	{
		GSLOG(Error, TEXT("nullptr == observerMgr"));
		return;
	}

	FGsGameDataManager* gameDataMgr = GGameData();
	if (nullptr == gameDataMgr)
	{
		GSLOG(Error, TEXT("nullptr == gameDataMgr"));
		return;
	}

	UGsLevelManager* levelMgr = GLevel();
	if (nullptr == levelMgr)
	{
		GSLOG(Error, TEXT("nullptr == levelMgr"));
		return;
	}

	// [관전 / 침공 / 인터 필드 / 파티 던전 / 기사단 던전 / 엘리트 던전 / 싱글 필드] 에서는 스트링만 출력
	if (observerMgr->GetIsObserveStart() ||
		gameDataMgr->IsInvadeWorld() ||
		gameDataMgr->IsInInterWorld() ||
		levelMgr->IsPartyDungeonMap() ||
		levelMgr->IsGuildDungeonMap() ||
		levelMgr->IsEliteDungeonMap() ||
		levelMgr->IsInstanceSingleMap())
	{
		ShowDetailDescText(schemaQuestGuideStory);
		return;
	}

	// 언락을 따로 체크하지 않는다. 특정 가이드 퀘스트가 열렸다는 건, 해당 컨텐츠가 언락되었다는 것을 의미한다. (데이터로 조정)
	OpenCorrespondingWindow(schemaQuestGuideStory);
}

void FGsQuestHandler::OpenPopupQuestReward(const IGsMessageParam* In_data)
{
	FGsUIHelper::HideBlockUI();

	const FGsStoryParam* storyParam =
		In_data->Cast<const FGsStoryParam>();

	StoryId storyId = storyParam->_storyId;
	const QuestKey& questKey = storyParam->_questKey;

	FGsQuestInstance* questInstance = FindQuestInstance(questKey, storyId);
	if (nullptr == questInstance)
	{
		// 존재하지않는 퀘스트이다.
		return;
	}
	else
	{
		if (questInstance->GetQuestState() != QuestState::COMPLETED)
			return;		
	}

	if (false == IsInRequestList(questInstance->GetQuestKey()))
	{
		if (questInstance->GetQuestType() == QuestType::REPEAT)
			OpenPopupQuestRepeatReward(questInstance);
		else if (questInstance->GetQuestType() == QuestType::MAIN)
		{
			_reserveStoryIdReward = storyParam->_storyId;
			GSEventProgress()->OnReqStartEvent(EGsEventProgressType::UI_POPUP_QUESTCOMPLETE, this);
		}
		else
			OpenPopupQuestReward(questInstance);
	}
}

bool FGsQuestHandler::OpenPopupQuestReward(FGsQuestInstance* inQuest)
{
	FText titleText;
	inQuest->GetTitleText(titleText);
	FText descText;
	inQuest->GetDescText(descText);

	FGsQuestPopUpParam param(inQuest->GetStoryId(), inQuest->GetQuestId(), inQuest->GetQuestIndex(), titleText, descText,
		inQuest->GetMainRewardId(), inQuest->GetSubRewardId(), 0, inQuest->GetQuestType(), inQuest->GetQuestContentsType());
	
	RewardType mainRewardType = RewardType::MAX;
	if (const FGsSchemaRewardData* mainReward = UGsTableUtil::FindRowById<UGsTableRewardData, FGsSchemaRewardData>(inQuest->GetMainRewardId()))
	{
		mainRewardType = mainReward->rewardType;
	}

	RewardType subRewardType = RewardType::MAX;
	if (const FGsSchemaRewardData* subReward = UGsTableUtil::FindRowById<UGsTableRewardData, FGsSchemaRewardData>(inQuest->GetSubRewardId()))
	{
		subRewardType = subReward->rewardType;
	}

	return OpenPopupReward(&param, mainRewardType, subRewardType);
}

void FGsQuestHandler::OpenPopupQuestRepeatReward(FGsQuestInstance* inQuest)
{
	OpenPopupQuestReward(inQuest);
}

void FGsQuestHandler::OpenPopupQuestGuideReward(const IGsMessageParam* In_data)
{
	const FGsStoryParam* storyParam =
		In_data->Cast<const FGsStoryParam>();

	StoryId storyId = storyParam->_storyId;

	FGsQuestInstance* questInstance = FindQuestInstance(QuestKey::KeyNone(), storyId);
	if (nullptr == questInstance)
	{
#if WITH_EDITOR
		GSLOG(Log, TEXT("[Quest]Popup Reward StoryId : %d, QuestInstance == nullptr"), storyId);
#endif
		// 만약 보상을 수령할 가이드 퀘스트의 인스턴스가 nullptr 이면 인스턴스를 만들어야 한다
		// QuestKey 의 _questIndex 를 0 으로 주어도 되는지 확인이 필요하다;
		TSharedPtr<FGsQuestGuideStoryTableCache> storyTableCache = GSQuest()->GetGuideStoryTableCache(storyId);
		if (!storyTableCache.IsValid())
			return;

		QuestId questId = storyTableCache->GetQuestId();

		UGsQuestManager* questManager = GSQuest();
		if (nullptr == questManager)
			return;

		questManager->SetQuestData(questId, QuestState::COMPLETED, 0);
		questInstance = ClaimQuestInstance(storyId, QuestKey(questId));

		if (nullptr == questInstance)
		{
#if WITH_EDITOR
			GSLOG(Log, TEXT("[Quest]Popup Reward StoryId : %d, QuestInstance == nullptr"), storyId);
#endif
			return;
		}
	}

	OpenPopupQuestReward(questInstance);
}

// 퀘스트 반복 보상 새로고침
void FGsQuestHandler::OnQuestRepeatRewardRefresh(const IGsMessageParam* In_data)
{
	const FGsQuestParam* questParam = In_data->Cast<const FGsQuestParam>();

	QuestId questId = questParam->_questId;
	FGsQuestRepeatData* repeatData = nullptr;
	if (questParam)
	{		
		TSharedPtr<FGsQuestData> questData = GSQuest()->GetQuestData(questId);
		if (false == questData.IsValid())
		{
			return;
		}

		repeatData = static_cast<FGsQuestRepeatData*>(questData.Get());		
	}

	if (nullptr == repeatData)
	{
		return;
	}

	const FGsSchemaQuestRepeatScrollRefreshInfoSet* refreshInfoSet = GetRepeatRefrehTable(
		repeatData->GetRepeatScrollId(questParam->_questIndex));
	if (nullptr == refreshInfoSet)
	{
		return;
	}

	// 재화가 부족할 경우 바로 구매 안내 팝업으로 유도
	QuestIndex questIndex = questParam->_questIndex;
	uint8 refreshCount = repeatData->GetRefreshCount(questIndex);
	int32 remainCount = refreshInfoSet->currencyInfoList.Num() - refreshCount;

	if (false == refreshInfoSet->currencyInfoList.IsValidIndex(refreshCount))
	{
		return;
	}

	const FGsSchemaQuestRepeatScrollRefreshInfo& refreshInfo = refreshInfoSet->currencyInfoList[refreshCount];
	
	if (false == FGsCurrencyHelper::CheckCurrency(refreshInfo.costCurrencyType, refreshInfo.costCurrencyAmount, false))
	{
		// 재화 부족 구매유도 팝업 열기
		Currency lackAmount = refreshInfo.costCurrencyAmount - FGsCurrencyHelper::GetCurrencyAmount(refreshInfo.costCurrencyType);
		FGsCurrencyHelper::OpenLackCurrencyPopup(refreshInfo.costCurrencyType, lackAmount);

		return;
	}

	TWeakObjectPtr<UGsUIWidgetBase> widget = GUI()->OpenAndGetWidget(TEXT("PopupYesNoCurrencyRefreshCount"));
	if (widget.IsValid())
	{
		if (UGsUIPopupYesNoCurrencyRefreshCount* popup = Cast<UGsUIPopupYesNoCurrencyRefreshCount>(widget.Get()))
		{
			// TEXT: 현재 표시된 퀘스트와 보상이 변경될 수 있습니다. 계속 진행하시겠습니까?
			FText textMsg;
			FText::FindText(TEXT("QuestUIText"), TEXT("UI_Popup_Text_RepeatRefreshAlert"), textMsg);
			popup->SetData(textMsg, remainCount, refreshInfo.costCurrencyType, refreshInfo.costCurrencyAmount,
				[this, questId, questIndex](bool bIsOk)
				{
					if (bIsOk)
					{
						bool isRefreshPossible = GSQuest()->IsQuestRefreshPossible(questId, questIndex);
						if (isRefreshPossible)
						{
							_refreshQuestKey = QuestKey(questId, questIndex);
							FGsNetSendServiceQuest::SendReqQuestRepeatRefresh(questIndex);
						}
						else
						{
							FText textMsg;
							FText::FindText(TEXT("QuestUIText"), TEXT("UI_Ticker_CannotRefresh"), textMsg);
							FGsUIHelper::PopupOK(textMsg);
						}
					}
				});
		}
	}
}

void FGsQuestHandler::OnTeleport(const IGsMessageParam* In_data)
{
	const FGsQuestTeleportParam* teleportParam =
		In_data->Cast<const FGsQuestTeleportParam>();

	OnTeleport(FindQuestInstance(QuestKey(teleportParam->_questId, teleportParam->_questIndex), teleportParam->_storyId));
}

void FGsQuestHandler::OnTeleport(FGsQuestInstance* inQuest)
{
	if (nullptr == inQuest)
	{
		GSLOG(Error, TEXT("Quest instance is missing"));
		return;
	}

	if (UGsGameObjectBase* local = GSGameObject()->FindObject(EGsGameObjectType::LocalPlayer))
	{
		if (local->IsCrowdControlLock(CreatureActionType::CREATURE_ACTION_TYPE_WARP))
		{
			FText result;
			FText::FindText(TEXT("UserTeleportText"), TEXT("ErrorTeleportInvalidState"), result);
			FGsUIHelper::TrayMessageTicker(result);

			return;
		}
	}

	UGsQuestManager* questManager = GSQuest();
	if (nullptr != questManager)
	{
		if (false == questManager->IsTeleportPossible(inQuest->GetQuestType(), inQuest->GetStoryId()))
		{
			FText result;
			FText::FindText(TEXT("UserTeleportText"), TEXT("ErrorTeleportInvalidState"), result);
			FGsUIHelper::TrayMessageTicker(result);

			return;
		}
	}

	if (false == IsPossibleQuestWarp(inQuest->GetQuestKey()))
	{
		FText result;
		FText::FindText(TEXT("UserTeleportText"), TEXT("ErrorTeleportInvalidState"), result);
		FGsUIHelper::TrayMessageTicker(result);
		return;
	}

	int32 outTeleportGold;
	QuestObjectiveId outObjectiveId;
	if (false == inQuest->GetTeleportData(outTeleportGold, outObjectiveId))
	{
		GSLOG(Error, TEXT("Get Teleport data fail - quest id : %lld"), inQuest->GetQuestId());
		return;
	}

	_reservedCostPackage = FGsCostPackage::CreateSimple(CurrencyType::GOLD, outTeleportGold);

	// 텔레포트 비용 할인 객체가 있을 경우, 추가
	TSharedPtr<FGsCostOption> CostOption = FGsContentsEventTeleportCost::CreateActivatedTeleportCostOption();
	if (CostOption.IsValid())
	{
		_reservedCostPackage->AddCostOption(CostOption.ToSharedRef());
	}

	_reserveObjectiveId = outObjectiveId;
	_reserveQuestId = inQuest->GetQuestId();
	_reserveQuestKey = inQuest->GetQuestKey();
	_reserveIsAccpeted = true;

	GSLOG(Log, TEXT("FGsQuestHandler::OnTeleport S EventProgress Type : UI_POPUP_TELEPORT - quest id : %lld"), _reserveQuestId);

	GSEventProgress()->OnReqStartEvent(EGsEventProgressType::UI_POPUP_TELEPORT, this);
}

void FGsQuestHandler::OnNpcToTeleport(const IGsMessageParam* In_data)
{
	if (UGsGameObjectBase* local = GSGameObject()->FindObject(EGsGameObjectType::LocalPlayer))
	{
		if (local->IsCrowdControlLock(CreatureActionType::CREATURE_ACTION_TYPE_WARP))
		{
			FText result;
			FText::FindText(TEXT("UserTeleportText"), TEXT("ErrorTeleportInvalidState"), result);
			FGsUIHelper::TrayMessageTicker(result);

			return;
		}
	}

	UGsQuestManager* questManager = GSQuest();
	if (nullptr == questManager)
		return;

	const FGsQuestTeleportParam* teleportParam =
		In_data->Cast<const FGsQuestTeleportParam>();

	QuestId questId = teleportParam->_questId;
	StoryId storyId = teleportParam->_storyId;

	int32 outTeleportGold;
	QuestObjectiveId outObjectiveId;
	if (true == questManager->GetQuestTeleportData(questId, outTeleportGold, outObjectiveId))
	{
		_reservedCostPackage = FGsCostPackage::CreateSimple(CurrencyType::GOLD, outTeleportGold);

		// 텔레포트 비용 할인 객체가 있을 경우, 추가
		TSharedPtr<FGsCostOption> CostOption = FGsContentsEventTeleportCost::CreateActivatedTeleportCostOption();
		if (CostOption.IsValid())
		{
			_reservedCostPackage->AddCostOption(CostOption.ToSharedRef());
		}

		_reserveObjectiveId = outObjectiveId;
		_reserveQuestId = questId;
		_reserveQuestKey = QuestKey(questId, 0);
		_reserveIsAccpeted = false;

		GSEventProgress()->OnReqStartEvent(EGsEventProgressType::UI_POPUP_TELEPORT, this);
	}

	questManager->SetIsWarpToNpc(true);
	questManager->SetWarpToNpcId(teleportParam->_npcId);
}

void FGsQuestHandler::OnNpcToAutoProgress(const IGsMessageParam* In_data)
{
	const FGsAIQuestAutoMoveMessageParam* autoMoveParam =
		In_data->Cast<const FGsAIQuestAutoMoveMessageParam>();	

	UGsQuestManager* questManager = GSQuest();
	if (nullptr == questManager)
		return;

	if (false == FGsAutoMoveBlockList::IsAutoMovePossibility())
	{
		FText text;
		FText::FindText(TEXT("QuestUIText"), TEXT("Ticker_unableQuest"), text);
		FGsUIHelper::TrayMessageTicker(text);

		return;
	}

	OnUserAutoMoveStop();

	questManager->SetIsGotoMoveNpc(true);
	questManager->SetGogotoMoveNpcId(autoMoveParam->_npcId);
	questManager->SetGogotoMoveNpcMapId(autoMoveParam->_mapId);
	questManager->SetGogotoMoveNpcPos(autoMoveParam->_arrPos);

	ReQuestNpcToAutoProgress();

	/*GSQuest()->SetIsMovingToNpc(true);
	GSQuest()->SetAutoProgressQuest(QuestKey(autoMoveParam->_questId));
	GMessage()->GetGameObject().SendMessage(MessageGameObject::AI_QUEST_AUTO_MOVE_START, autoMoveParam);*/
}

void FGsQuestHandler::ReQuestNpcToAutoProgress()
{
	UGsQuestManager* questManager = GSQuest();
	if (nullptr == questManager)
		return;

	UGsGameObjectBase* localBase = GSGameObject()->FindObject(EGsGameObjectType::LocalPlayer);
	if (nullptr == localBase)
	{
		return;
	}

	UGsGameObjectLocalPlayer* local = localBase->CastGameObject<UGsGameObjectLocalPlayer>();
	if (nullptr == local)
	{
		return;
	}

	FGsInteractionHandlerLocalPlayer* localInteraction = local->GetCastInteraction<FGsInteractionHandlerLocalPlayer>();
	if (localInteraction == nullptr)
	{
		return;
	}
	// 1. auto toggle 시 기존 이동 데이터 삭제하므로 
	// toggle 후(이동 데이터 삭제 후) 이동 요청 해야함
	// https://jira.com2us.com/jira/browse/C2URWQ-6895
	//GMessage()->GetGameObject().SendMessage(MessageGameObject::AI_MODE_ACTIVE_TOGGLE, nullptr);
	// 2. https://jira.com2us.com/jira/browse/C2URWQ-3294 지라 요청
	// off를 호출해야되는데 toggle을 요청해서 재요청 온 이슈 수정

	FVector vecFrontPos = FVector::ZeroVector;
	if (0 < questManager->GetGogotoMoveNpcPos().Num())
	{
		vecFrontPos = questManager->GetGogotoMoveNpcPos()[0];
	}
	GSAI()->SetActiveAI(false);
	localInteraction->TryInteractionNpcTblIdWithMapId(questManager->GetGogotoMoveNpcId(), questManager->GetGogotoMoveNpcMapId()
	, EGsInteractionMoveType::None, vecFrontPos);
}

// 스토리 포기
void FGsQuestHandler::OnQuestGiveUp(const IGsMessageParam* In_data)
{
	const FGsStoryParam* storyParam = In_data->Cast<const FGsStoryParam>();
	StoryId storyId = storyParam->_storyId;
	QuestKey questKey = storyParam->_questKey;
	QuestId questId = 0;
	QuestIndex questIndex = INVALID_QUEST_INDEX;
	FText titleText;
	QuestType questType;

	for (FGsQuestInstance* questInstance : _questInstanceList)
	{
		if (nullptr == questInstance)
			continue;

		if (questInstance->GetStoryId() != storyId)
			continue;

		if (!questKey.IsKeyNone())
		{
			if (questInstance->GetQuestKey() != questKey)
				continue;
		}

		questId = questInstance->GetQuestId();
		questIndex = questInstance->GetQuestIndex();
		questType = questInstance->GetQuestType();
		questInstance->GetTitleText(titleText);
		break;
	}

	TWeakObjectPtr<UGsUIWidgetBase> widget = GUI()->OpenAndGetWidget(TEXT("PopupYesNoTwoMessage"));
	if (!widget.IsValid())
		return;

	if (UGsUIPopupYesNoTwoMessage* popup = Cast<UGsUIPopupYesNoTwoMessage>(widget.Get()))
	{
		FText textMsg1;
		FText::FindText(TEXT("UICommonText"), TEXT("Number_PointLarge1"), textMsg1);
		textMsg1 = FText::Format(textMsg1, titleText);
		// TEXT: 퀘스트를 포기하시겠습니까?
		FText textMsg2;
		FText::FindText(TEXT("QuestUIText"), TEXT("ConfirmGiveUp"), textMsg2);

		// 스토리를 포기하시겠습니까?
		popup->SetData(textMsg1, textMsg2, [storyId, questId, questType, questIndex](bool bYes)
			{
				if (bYes)
				{
					bool isGiveUpPossible = GSQuest()->IsQuestGiveUpPossible(questType, storyId, questId, questIndex);
					if (isGiveUpPossible)
					{
						// 퀘스트 포기 패킷 전송
						if (questType == QuestType::SUB)
							FGsNetSendServiceWorld::SendQuestSubGiveUp(storyId, questId);
						else if (questType == QuestType::REPEAT)
							FGsNetSendServiceWorld::SendQuestRepeatGiveUp(storyId, questId, questIndex);
					}
					else
					{
						FText textMsg3;
						FText::FindText(TEXT("QuestUIText"), TEXT("CompletedQuest"), textMsg3);
						FGsUIHelper::PopupOK(textMsg3);
					}
				}
			});
	}
}

// 습득 메세지 (클라 -> 클라)
void FGsQuestHandler::AddQuest(QuestId inQuestId)
{
	bool isPlayerDead = GSGameObject()->IsLocalPlayerDead();
	if (!isPlayerDead)
	{
		FGsNetSendServiceWorld::SendQuestMainAccept(inQuestId);
	}
}

// 습득 메세지 (클라 -> 클라)
void FGsQuestHandler::AddQuestSub(StoryId inStoryId, QuestId inQuestId)
{
	bool isPlayerDead = GSGameObject()->IsLocalPlayerDead();
	if (!isPlayerDead)
	{
		FGsNetSendServiceWorld::SendQuestSubAccept(inStoryId, inQuestId, 0);
	}
}

// 습득 메세지 (클라 -> 클라)
void FGsQuestHandler::AddQuestRepeat(StoryId inStoryId, QuestId inQuestId)
{
	bool isPlayerDead = GSGameObject()->IsLocalPlayerDead();
	if (!isPlayerDead)
	{
		FGsNetSendServiceWorld::SendQuestRepeatAccept(inStoryId, inQuestId);
	}
}

// 스토리의 첫번째 퀘스트 습득 메세지(클라 -> 클라)
void FGsQuestHandler::AddStoryQuest(QuestId inQuestId)
{
	UpdateQuestInstance(0, QuestKey(inQuestId), QuestState::NONE);
}

// 수락 메세지 (서버 -> 클라)
void FGsQuestHandler::AcceptQuest(const StoryId inStoryId, const QuestKey& inQuestKey)
{
	AcceptQuestInstance(inStoryId, inQuestKey, QuestState::ACCEPTED);
}

// 수락 메세지 (서버 -> 클라)
void FGsQuestHandler::AcceptQuestSub(const StoryId inStoryId, const QuestKey& inQuestKey)
{
	GSQuest()->AcceptSubQuestUpdata(inStoryId, inQuestKey._questId);
	AcceptQuest(inStoryId, inQuestKey);
}

// 수락 메세지 (서버 -> 클라)
void FGsQuestHandler::AcceptQuestRepeat(const StoryId inStoryId, const QuestKey& inQuestKey)
{
	UGsQuestManager* questManager = GSQuest();
	if (nullptr == questManager)
		return;

	bool isRefresh = !_refreshQuestKey.IsKeyNone();
	// 새로고침 했을 경우 기존 꺼 삭제
	if (isRefresh)
	{
		RemoveQuestInstance(INVALID_STORY_ID, _refreshQuestKey);
		questManager->ResetQuestRepeat(_refreshQuestKey);

		// 삭제 후 키값도 정리
		_refreshQuestKey = QuestKey::KeyNone();
	}

	AcceptQuest(inStoryId, inQuestKey);	

	if (!isRefresh)
	{
		// 메세지 박스 오픈
		OpenPopupQuestRepeatAccept(inQuestKey._questIndex);
	}
	else
	{
		FGsStoryParam param(inStoryId, inQuestKey);
		GMessage()->GetGameObject().SendMessage(MessageGameObject::QUEST_REPEAT_REACCEPT, &param);
	}
}

void FGsQuestHandler::AcceptQuestGuide(const QuestKey& inQuestKey)
{
	UGsQuestManager* questManager = GSQuest();
	if (nullptr == questManager)
		return;

	questManager->AcceptGuideQuestUpdata(inQuestKey._questId);
	AcceptQuest(questManager->GetGuideStoryIdByQuestId(inQuestKey._questId), inQuestKey);
}

void FGsQuestHandler::AcceptQuestBoard(const StoryId inStoryId, const QuestKey& inQuestKey)
{
	AcceptQuest(inStoryId, inQuestKey);
}

// 업데이트 메세지 (서버 -> 클라)
void FGsQuestHandler::UpdateQuest(const StoryId inStoryId, const QuestKey& inQuestKey, const QuestState inState, 
										const TArray<int32>& inObjectiveValueList, bool inIsUpdate, 
											EventActionIndex inPreEventActionIndex, EventActionIndex inPostEventActionIndex, EventActionIndex inPostStoryEventActionIndex)
{
	if (QuestState::DELETED == inState)
	{
		RemoveQuestInstance(inStoryId, inQuestKey);
		return;
	}

	UpdateQuestInstance(inStoryId, inQuestKey, inState, inObjectiveValueList, inIsUpdate, inPreEventActionIndex, inPostEventActionIndex, inPostStoryEventActionIndex);
}

// 메인 보상 메세지 (서버 -> 클라)
void FGsQuestHandler::RewardQuest(const StoryId inStoryId, const QuestId inQuestId)
{
	UpdateQuestInstance(inStoryId, QuestKey(inQuestId), QuestState::REWARDED);
}

// 서브 보상 메세지 (서버 -> 클라)
void FGsQuestHandler::RewardQuestSub(const StoryId inStoryId, const QuestId inQuestId)
{
	GSQuest()->RewardSubQuestUpdata(inStoryId, inQuestId);

	UGsQuestManager* questManager = GSQuest();
	if (nullptr == questManager)
		return;

	if (FGsServerOption* serverOption = GServerOption())
	{
		serverOption->SetQuestSubUIVisibility(true, inStoryId);
	}

	questManager->SetQuestData(inQuestId, QuestState::REWARDED);
	RemoveQuestInstance(inStoryId, QuestKey(inQuestId));
}

// 서브 보상 메세지 (반복 -> 클라)
void FGsQuestHandler::RewardQuestRepeat(const StoryId inStoryId, const QuestKey& inQuestKey)
{
	UGsQuestManager* questManager = GSQuest();
	if (nullptr == questManager)
		return;

	if (FGsServerOption* serverOption = GServerOption())
	{
		serverOption->SetQuestRepeatUIVisibility(true, inQuestKey._questIndex);
	}

	RemoveQuestInstance(inStoryId, inQuestKey);
	questManager->ResetQuestRepeat(inQuestKey);
}

void FGsQuestHandler::RewardQuestGuild(const StoryId inStoryId, const QuestId inQuestId)
{
	UGsQuestManager* questManager = GSQuest();
	if (nullptr == questManager)
		return;

	questManager->SetQuestData(inQuestId, QuestState::REWARDED);
	RemoveQuestInstance(inStoryId, QuestKey(inQuestId));
}

void FGsQuestHandler::RewardQuestGuide(const StoryId inStoryId, const QuestId inQuestId)
{
	UGsQuestManager* questManager = GSQuest();
	if (nullptr == questManager)
		return;

	StoryId storyId = questManager->GetGuideStoryIdByQuestId(inQuestId);

	questManager->SetQuestData(inQuestId, QuestState::REWARDED);
	RemoveQuestInstance(storyId, QuestKey(inQuestId));
	questManager->ClearQuestGuide(inQuestId);

	FGsStoryParam param(storyId);
	GMessage()->GetQuest().SendMessage(MessageContentQuest::QUEST_GUIDE_REWARDED, &param);
}

void FGsQuestHandler::RewardBoxListQuestRepeat(const QuestIndex inQuestIndex, uint8 inRefreshCount, 
	uint32 inRepeatScrollId, const TArray<RewardBoxItemIdPair>& inRewardBoxItemIdList)
{
	UGsQuestManager* questManager = GSQuest();
	if (nullptr == questManager)
		return;

	for (FGsQuestInstance* questInstance : _questInstanceList)
	{
		if (nullptr == questInstance)
			continue;

		if (questInstance->GetQuestType() != QuestType::REPEAT)
			continue;
		
		if (inQuestIndex == questInstance->GetQuestIndex())
		{
			questManager->SetQuestRewardBoxItemIdList(questInstance->GetQuestId(), inQuestIndex, inRefreshCount, 
				inRepeatScrollId, inRewardBoxItemIdList);
			break;
		}
	}

	FGsQuestRepeatPopUpParam param;
	RewardType mainRewardType = RewardType::MAX;
	RewardType subRewardType = RewardType::MAX;
	if (GetQuestRepeatPopupParam(inQuestIndex, param, mainRewardType, subRewardType))
	{
		GMessage()->GetGameObject().SendMessage(MessageGameObject::QUEST_REPEAT_REWARD_BOX_RECEIVE, &param);
	}	
}

void FGsQuestHandler::RemoveRankQuest(const StoryId inStoryId)
{
	UGsQuestManager* questManager = GSQuest();
	if (nullptr == questManager)
		return;

	TArray<TSharedPtr<FGsQuestData>> outQuestList;
	if (questManager->GetRankQuestDataList(inStoryId, outQuestList))
	{
		for (TSharedPtr<FGsQuestData> quest : outQuestList)
		{
			RemoveQuestInstance(inStoryId, QuestKey(quest->GetQuestId()));
			questManager->ClearQuestRank(quest->GetQuestId());
		}
	}
}

void FGsQuestHandler::GiveUpQuest(IN const StoryId inStoryId, IN const QuestKey& inQuestKey)
{
	if (inQuestKey == GSQuest()->GetAutoProgressQuestKey())
	{
		OnUserAutoMoveStop();
	}

	RemoveQuestInstance(inStoryId, inQuestKey);
}

void FGsQuestHandler::GiveUpQuest(IN const StoryId inStoryId, OUT QuestId& outQuestId)
{
	QuestKey questKey = QuestKey::KeyNone();
	for (FGsQuestInstance* questInstance : _questInstanceList)
	{
		if (nullptr == questInstance)
			continue;

		if (questInstance->GetStoryId() != inStoryId)
			continue;

		outQuestId = questInstance->GetQuestId();
		questKey = questInstance->GetQuestKey();
		break;
	}

	if (questKey == GSQuest()->GetAutoProgressQuestKey())
	{
		OnUserAutoMoveStop();
	}

	RemoveQuestInstance(inStoryId, questKey);
}

void FGsQuestHandler::GiveUpQuestSub(const StoryId inStoryId)
{
	QuestId outQuestId = 0;
	GiveUpQuest(inStoryId, outQuestId);
	GSQuest()->GiveUpSubQuestUpdata(inStoryId, outQuestId);
}

void FGsQuestHandler::GiveUpQuestRepeat(const StoryId inStoryId, const QuestKey& inQuestKey)
{
	GiveUpQuest(inStoryId, inQuestKey);
	GSQuest()->ResetQuestRepeat(inQuestKey);
}

void FGsQuestHandler::GiveUpQuestBoard(IN const StoryId inStoryId)
{
	QuestId outQuestId = 0;
	GiveUpQuest(inStoryId, outQuestId);
}

void FGsQuestHandler::GiveUpQuestGuild(IN const StoryId inStoryId)
{
	QuestId outQuestId = 0;
	GiveUpQuest(inStoryId, outQuestId);
	GSQuest()->ClearQuestGuild(outQuestId);
}

void FGsQuestHandler::FlushAllInstance()
{
	if (nullptr != _questPoolManager)
	{
		for (FGsQuestInstance* questInstance : _questInstanceList)
		{
			if (nullptr == questInstance)
				continue;

			questInstance->Clear();
			_questPoolManager->ReleaseInstance(questInstance->GetQuestType(), questInstance);
		}
	}

	if (nullptr != _questPoolManager)
	{
		_questPoolManager->Finalize();
	}

	_questInstanceList.Empty();
	_mainQuestInstance = nullptr;
	_requestList.Empty();
	_reserveStoryIdAccpet = 0;
	_reserveStoryIdReward = 0;
	_refreshQuestKey = QuestKey::KeyNone();
	_repeatWarpWaiting = false;
	_isWarpReady = false;
	_isUseWarpRandomScroll = false;
}

void FGsQuestHandler::AcceptQuestInstance(StoryId inStoryId, const QuestKey& inQuestKey, QuestState inState)
{
	UGsQuestManager* questManager = GSQuest();
	if (nullptr == questManager)
		return;

	questManager->SetQuestData(inQuestKey._questId, inState, inQuestKey._questIndex);

	FGsQuestInstance* questInstance = FindQuestInstance(inQuestKey);
	if (nullptr == questInstance)
	{
		questInstance = ClaimQuestInstance(inStoryId, inQuestKey);
	}

	if (nullptr == questInstance)
		return;

	questInstance->Accept();
	questInstance->SetQuestUIEffectType(EGsQuestUIEffectType::QUEST_UI_EFFECT_NEW);

	// ui 갱신
	GMessage()->GetContentsHud().SendMessage(MessageContentHud::INVALIDATE_QUEST);
}

void FGsQuestHandler::UpdateQuestInstance(StoryId inStoryId, const QuestKey& inQuestKey, 
			QuestState inState, const TArray<int32>& inObjectiveValueList, bool inIsUpdate, 
				EventActionIndex inPreEventActionIndex, EventActionIndex inPostEventActionIndex, EventActionIndex inPostStoryEventActionIndex)
{
	UGsQuestManager* questManager = GSQuest();
	if (nullptr == questManager)
		return;

	questManager->SetQuestData(inQuestKey._questId, inState, inObjectiveValueList, 
				inPreEventActionIndex, inPostEventActionIndex, inPostStoryEventActionIndex, inQuestKey._questIndex);

	FGsQuestInstance* questInstance = FindQuestInstance(inQuestKey);
	if (nullptr == questInstance)
	{
		questInstance = ClaimQuestInstance(inStoryId, inQuestKey);
	}

	if (nullptr == questInstance)
		return;

	questInstance->Update(inState);
	if(inIsUpdate)
		questInstance->SetQuestUIEffectType(EGsQuestUIEffectType::QUEST_UI_EFFECT_UPDATE);

	// ui 갱신
	GMessage()->GetContentsHud().SendMessage(MessageContentHud::INVALIDATE_QUEST);
}

void FGsQuestHandler::UpdateQuestInstance(StoryId inStoryId, const QuestKey& inQuestKey, QuestState inState)
{
	UGsQuestManager* questManager = GSQuest();
	if (nullptr == questManager)
		return;

	questManager->SetQuestData(inQuestKey._questId, inState, inQuestKey._questIndex);

	FGsQuestInstance* questInstance = FindQuestInstance(inQuestKey);
	if (nullptr == questInstance)
	{
		questInstance = ClaimQuestInstance(inStoryId, inQuestKey);
	}

	questInstance->Update(inState);
	questInstance->SetQuestUIEffectType(EGsQuestUIEffectType::QUEST_UI_EFFECT_UPDATE);

	// ui 갱신
	GMessage()->GetContentsHud().SendMessage(MessageContentHud::INVALIDATE_QUEST);
}


void FGsQuestHandler::RemoveQuestInstance(StoryId inStoryId, const QuestKey& inQuestKey)
{
	// 퀘스트 Data 삭제
	GSQuest()->RemoveActiveQuestKey(inQuestKey._questId, inQuestKey._questIndex);

	// 퀘스트 Instance 삭제
	FGsQuestInstance* quest = FindQuestInstance(inQuestKey);
	if (nullptr == quest)
		return;

	RemoveQuestInstance(quest);
}

void FGsQuestHandler::RemoveQuestInstance(FGsQuestInstance* inQuest)
{
	if (nullptr != inQuest)
		inQuest->Clear();

	_questInstanceList.Remove(inQuest);

	if (nullptr == _questPoolManager)
		return;

	_questPoolManager->ReleaseInstance(inQuest->GetQuestType(), inQuest);

	// ui 갱신
	GMessage()->GetContentsHud().SendMessage(MessageContentHud::INVALIDATE_QUEST);
}

FGsQuestInstance* FGsQuestHandler::FindQuestInstance(const QuestKey& inQuestKey, StoryId inStoryId)
{
	if (inQuestKey.IsKeyNone() && 0 < inStoryId)
	{		
		for (FGsQuestInstance* questInstance : _questInstanceList)
		{
			if (nullptr == questInstance)
				continue;

			if (questInstance->GetStoryId() == inStoryId)
			{
				return questInstance;
			}
		}		
	}	

	for (FGsQuestInstance* questInstance : _questInstanceList)
	{
		if (nullptr == questInstance)
			continue;

		if (questInstance->GetQuestKey() == inQuestKey)
		{
			return questInstance;
		}
	}

	return nullptr;
}

FGsQuestInstance* FGsQuestHandler::ClaimQuestInstance(StoryId inStoryId, const QuestKey& inQuestKey)
{
	UGsQuestManager* questManger = GSQuest();
	if (nullptr == questManger)
		return nullptr;

	// 퀘스트 타입 (메인, 일일퀘,,,)
	const QuestType type = questManger->GetQuestType(inQuestKey._questId);
	FGsQuestInstance* questInstance = nullptr;

	switch (type)
	{
	case QuestType::MAIN:
	{
		if (nullptr != _mainQuestInstance)
		{
			QuestKey mainQuestKey = _mainQuestInstance->GetQuestKey();
			QuestId nextQuestId = 0;
			QuestId nextStoryQuestId = 0;
			if (mainQuestKey == inQuestKey)
			{
				return _mainQuestInstance;
			}
			else
			{
				if (!questManger->IsNextQuest(mainQuestKey._questId, nextQuestId))
				{
					nextStoryQuestId = questManger->GetFirstQuestId();
					if (mainQuestKey._questId != nextStoryQuestId)
					{
						GSLOG(Log, TEXT("[Quest]ClaimQuestInstance mainQuestId : %llu !=  nextStoryQuestId :  %llu"), mainQuestKey._questId, nextStoryQuestId);
					}

					//check(inQuestId == nextStoryQuestId);
				}
				else
				{
					if (mainQuestKey._questId != nextQuestId)
					{
						GSLOG(Log, TEXT("[Quest]ClaimQuestInstance mainQuestId : %llu !=  nextQuestId :  %llu"), mainQuestKey._questId, nextQuestId);
					}

					//check(nextQuestId == inQuestId);					
				}

				// 퀘스트 Data 삭제
				questManger->RemoveActiveQuestKey(mainQuestKey._questId);
				questInstance = _mainQuestInstance;				
			}
		}
		else
		{
			_mainQuestInstance = questInstance = _questPoolManager->ClaimInstance(type);
		}
	}
	break;
	case QuestType::SUB:
	{
		for (FGsQuestInstance* instance : _questInstanceList)
		{
			if (nullptr == instance)
				continue;

			if (instance->GetQuestType() == type)
			{
				if (inStoryId == instance->GetStoryId())
				{
					GSQuest()->RemoveActiveQuestKey(instance->GetQuestId());
					questInstance = instance;
					break;
				}
			}
		}

		if (nullptr == questInstance)
			questInstance = _questPoolManager->ClaimInstance(type);
	}
	break;
	case QuestType::REPEAT:
	{
		for (FGsQuestInstance* instance : _questInstanceList)
		{
			if (nullptr == instance)
				continue;

			if (instance->GetQuestType() == type)
			{
				if (inQuestKey == instance->GetQuestKey())
				{
					GSQuest()->RemoveActiveQuestKey(instance->GetQuestId(), instance->GetQuestIndex());
					questInstance = instance;
					break;
				}
			}
		}

		if (nullptr == questInstance)
			questInstance = _questPoolManager->ClaimInstance(type);
	}
	break;
	case QuestType::MULTI_LEVEL_RANK:
	case QuestType::GUILD:
	{
		questInstance = _questPoolManager->ClaimInstance(type);
	}
	break;
	case QuestType::GUIDE:
	{
		questInstance = _questPoolManager->ClaimInstance(type);
	}
	break;
	default:
		break;
	}

	if (nullptr != questInstance)
	{
		questInstance->Initialize(_owner, inStoryId, inQuestKey);

		GSLOG(Log, TEXT("[Quest]ClaimQuestInstance inQuestId : %llu"), inQuestKey._questId);
		if (!_questInstanceList.Contains(questInstance))
		{
			_questInstanceList.Add(questInstance);			
		}
	}

	return questInstance;
}

void FGsQuestHandler::OnQuestAutoMoveEnd(const IGsMessageParam* In_data)
{
	const FGsAIQuestMessageParam* autoMoveParam =
		In_data->Cast<const FGsAIQuestMessageParam>();

	QuestKey activeQuestKey = GSQuest()->GetAutoProgressQuestKey();
	QuestId questId = autoMoveParam->_questId;
	if (activeQuestKey._questId == questId)
	{
		if (FGsQuestInstance* questInstance = FindQuestInstance(activeQuestKey))
		{
			questInstance->ArrivedAutoProgress(In_data);
		}

		// ui 갱신
		GMessage()->GetContentsHud().SendMessage(MessageContentHud::INVALIDATE_QUEST);
	}
}

void FGsQuestHandler::OnQuestAutoMoveNextMap(const IGsMessageParam* In_data)
{
	const FGsAIMoveNextMapWarpInfoParam* warpInfoParam =
		In_data->Cast<const FGsAIMoveNextMapWarpInfoParam>();

	QuestKey activeQuestKey = GSQuest()->GetAutoProgressQuestKey();
	if (activeQuestKey.IsKeyNone())
		return;

	if (FGsQuestInstance* questInstance = FindQuestInstance(activeQuestKey))
	{
		questInstance->NextMapAutoProgress(In_data);
	}
}

void FGsQuestHandler::OnQuestAutoMoveWarpStart(const IGsMessageParam* In_data)
{
	GSQuest()->SetIsSpawnMeCompleteStartProgress(true);
}

void FGsQuestHandler::OnInteractionNpcContents(const IGsMessageParam* In_data)
{
	const FGsInteractionMessageParamContents* interactionParam =
		In_data->Cast<const FGsInteractionMessageParamContents>();

	int npcTbID = interactionParam->_npcTblId;

	UGsQuestManager* questManager = GSQuest();
	if (nullptr == questManager)
		return;

	if (questManager->GetIsGotoMoveNpc())
	{
		if (questManager->GetGogotoMoveNpcId() == npcTbID)
		{
			questManager->SetIsGotoMoveNpc(false);
		}
	}

	const FGsSchemaNpcData* npcData = UGsTableUtil::FindRowById<UGsTableNpcData, FGsSchemaNpcData>(npcTbID);
	if (nullptr == npcData)
	{
#if WITH_EDITOR
		GSLOG(Warning, TEXT("[Quest]FGsSchemaNpcData::FindRowById is EmptyData - npcTbID : %d"), npcTbID);
#endif
		return;
	}

	if (NpcFunctionType::QUEST_DIALOG != npcData->npcFunctionType)
		return;

	for (FGsQuestInstance* questInstance : _questInstanceList)
	{
		if (nullptr == questInstance)
			continue;

		bool isReturnValue = questInstance->OnInteractionContents(In_data);
		if (true == isReturnValue)
		{
			return;
		}
	}

	// 서브 퀘스트 존재 여부를 체크하여 퀘스트 처리
	EQuestNpcState outNpcState;
	TSharedPtr<FGsQuestData> questData = questManager->GetSubQuestData(interactionParam->_npcTblId, outNpcState);

	if (outNpcState == EQuestNpcState::STATE_NONE ||
		outNpcState == EQuestNpcState::STATE_SUB_WATING)
	{
		if (!questData.IsValid())
			return;

		if (UGsGameObjectBase* localPlayerObjBase = GSGameObject()->FindObject(EGsGameObjectType::LocalPlayer))
		{
			if (FGsTargetHandlerBase* targetHandler = localPlayerObjBase->GetTargetHandler())
			{
				// 타겟 클리어후 
				// 팝업출력
				targetHandler->ClearTarget(true);
			}
		}

		OpenPopupQuestAccept(questData, interactionParam->_spawnId);
		questManager->SetIsMovingToNpc(false);

		// 2023/5/5 PKT - [대화연출] 서브퀘스트 자동진행중 AUTO가 켜지는 현상(재업로드) - 관련 추가
		// https://jira.com2us.com/jira/browse/C2URWQ-4469
		GSAI()->SetActiveAI(false);
	}
	else
	{
		FText outText;

		switch (outNpcState)
		{
		case EQuestNpcState::STATE_SUB_INVALID_ACCEPT:
		case EQuestNpcState::STATE_SUB_INVALID_MAIN_STORY:
		case EQuestNpcState::STATE_SUB_INVALID_SUB_STORY:
		case EQuestNpcState::STATE_SUB_INVALID_REQUIRE_LEVEL:
		{
			FText::FindText(TEXT("QuestUIText"), TEXT("HoldInfo"), outText);
			break;
		}
		case EQuestNpcState::STATE_SUB_ACCEPT:
		{
			FText::FindText(TEXT("QuestUIText"), TEXT("Quest_Already_Accept"), outText);
			break;
		}
		case EQuestNpcState::STATE_PROGRESS:
		case EQuestNpcState::STATE_REWARD_WATING:
		{
			// CSH - 2023/05/07
			// 보상 팝업이 나타나야하는 경우.
			// 이쪽에서 처리되어야 할 지, OnInteractionContents 쪽에서 처리되어야 할 지
			// 모호해서 반응하지 않도록 처리해두었습니다.
			break;
		}
		default:
			break;
		}

		if (FGsEventProgressManager* eventProgressManager = GSEventProgress())
		{
			eventProgressManager->OnReqFinishEvent(EGsEventProgressType::NPC_DIALOG);
		}

		FGsUIHelper::TrayMessageTicker(outText);

		questManager->SetIsMovingToNpc(false);
		GSAI()->SetActiveAI(false);
		// Quest 인터랙션이 조건에 부합하지 않아 캔들됨
		GMessage()->GetGameObject().SendMessage(MessageGameObject::INTERACTION_CONTENTS_BY_QUEST_CANCEL, interactionParam);
	}
}

bool FGsQuestHandler::IsHasInteractionQuest(int32 inNpcTblId, int64 inGameId)
{
	int npcTbID = inNpcTblId;

	const FGsSchemaNpcData* npcData = UGsTableUtil::FindRowById<UGsTableNpcData, FGsSchemaNpcData>(npcTbID);
	if (nullptr == npcData)
	{
#if WITH_EDITOR
		GSLOG(Warning, TEXT("[Quest]FGsSchemaNpcData::FindRowById is EmptyData - npcTbID : %d"), npcTbID);
#endif
		return false;
	}

	if (NpcFunctionType::QUEST_DIALOG != npcData->npcFunctionType)
		return false;

	for (FGsQuestInstance* questInstance : _questInstanceList)
	{
		if (nullptr == questInstance)
			continue;

		FGsInteractionMessageParamContents param(inNpcTblId, 0, inGameId);
		bool isReturnValue = questInstance->OnInteractionContents(&param);
		if (true == isReturnValue)
		{
			return true;
		}
	}

	return false;
}

void FGsQuestHandler::OnInteractionPropStart(const IGsMessageParam* In_data)
{
	const FGsPropInteractionMessageParamContents* interactionParam = In_data->Cast<const FGsPropInteractionMessageParamContents>();

	int32 tableId = interactionParam->_tableId;
	const FGsSchemaPropData* propData = UGsTableUtil::FindRowById<UGsTablePropData, FGsSchemaPropData>(tableId);
	if (nullptr == propData)
	{
		return;
	}

	UGsGameObjectBase* find = GSGameObject()->FindObject(EGsGameObjectType::PropInteraction, interactionParam->_gameId);
	if (nullptr == find)
	{
		return;
	}

	UGsGameObjectPropTouch* propInteraction = Cast<UGsGameObjectPropTouch>(find);
	if (nullptr == propInteraction)
	{
		return;
	}

	propInteraction->SendInteractionStart(interactionParam);
}

void FGsQuestHandler::OnInteractionPropEnd(const IGsMessageParam* In_data)
{
	const FGsPropInteractionMessageParamContents* interactionParam = In_data->Cast<const FGsPropInteractionMessageParamContents>();

	int32 tableId = interactionParam->_tableId;
	const FGsSchemaPropData* propData = UGsTableUtil::FindRowById<UGsTablePropData, FGsSchemaPropData>(tableId);
	if (nullptr == propData)
	{
		return;
	}

	UGsGameObjectBase* find = GSGameObject()->FindObject(EGsGameObjectType::PropInteraction, interactionParam->_gameId);
	if (nullptr == find)
	{
		return;
	}

	UGsGameObjectPropTouch* propInteraction = Cast<UGsGameObjectPropTouch>(find);
	if (nullptr == propInteraction)
	{
		return;
	}

	propInteraction->SendInteractionEnd(interactionParam);
}

void FGsQuestHandler::OnLocalPlayerSpawnComplete(const IGsMessageParam* In_data)
{	
	SpawnInit();
	// 페이드 연출후 호출해서 ui 갱신안되어 추가 갱신 호출
	GMessage()->GetContentsHud().SendMessage(MessageContentHud::INVALIDATE_QUEST);
}

void FGsQuestHandler::OnLocalPlayerSpawnAction(const IGsMessageParam* In_data)
{
	UGsQuestManager* questManager = GSQuest();
	if (nullptr == questManager)
		return;

	// 서브퀘스트 엔피씨 셋팅
	questManager->SetSubNpcTartget();
}

void FGsQuestHandler::Init()
{
	UGsQuestManager* questManager = GSQuest();
	if (nullptr == questManager)
		return;

	// 실행 중인 QuestAction를 중지한다.
	if (FGsQuestActionManagement* actionManagement = questManager->GetQuestActionManagement())
		actionManagement->StopQuestAction();

	// 모든 인스턴스 삭제
	FlushAllInstance();

	const TArray<QuestKey>& activeQuestDataList = questManager->GetActiveQuestKeyList();
	int num = activeQuestDataList.Num();
	for (int i = 0; i < num; ++i)
	{
		const QuestKey& questKey = activeQuestDataList[i];
		QuestId questId = questKey._questId;

		TSharedPtr<FGsQuestData> questData = questManager->GetQuestData(questId);
		if (!questData.IsValid())
		{
#ifdef WITH_EDITOR
			const FString failTickerString =
				FString::Printf(TEXT("find fail QuestData, QuestId: %d"), questId);

			FText failTickerText = FText::FromString(failTickerString);
			FGsUIHelper::PopupOK(failTickerText);
#endif //WITH_EDITOR
			continue;
		}

		if (questId == questManager->GetFirstQuestId())
		{
			FGsQuestDynamicData* dynamicData = questData->GetQuestDynamicData(questKey._questIndex);
			if (nullptr != dynamicData)
			{
				if (QuestState::NONE == dynamicData->GetQuestState())
				{
					ClaimQuestInstance(questData->GetStoryId(), questKey);
				}
			}			
			//questInstance->Update(questState);
		}
	}
}

// 스폰시 하는 초기화
void FGsQuestHandler::SpawnInit()
{
	//#if WITH_EDITOR
	GSLOG(Log, TEXT("[Quest]FGsQuestHandler::SpawnInit =========================="));
	//#endif
	
	// warp 또는 spawn이 끝났다.
	SetWarpReady(false);	

	UGsQuestManager* questManager = GSQuest();
	if (nullptr == questManager)
		return;

	questManager->SetIsUseWarpScroll(false);

	int questCount = _questInstanceList.Num();

	GSLOG(Log, TEXT("[Quest]FGsQuestHandler::SpawnInit questCount : %d"), questCount);

	for (int i = 0; i < questCount; ++i)
	{
		GSLOG(Log, TEXT("[Quest]FGsQuestHandler::SpawnInit QuestCount Index : %d"), i);

		if(false == _questInstanceList.IsValidIndex(i))
			continue;

		FGsQuestInstance* questInstance = _questInstanceList[i];
		if (nullptr == questInstance)
			continue;
	
		const QuestKey& questKey = questInstance->GetQuestKey();
		QuestId questId = questKey._questId;
		GSLOG(Log, TEXT("[Quest]FGsQuestHandler::SpawnInit questId : %llu"), questId);

		TSharedPtr<FGsQuestData> questData = questManager->GetQuestData(questId);
		if (questData == nullptr && !questData.IsValid())
		{
			const FString failTickerString =
				FString::Printf(TEXT("FGsQuestHandler::SpawnInit() find fail QuestData, QuestId: %d"), questInstance->GetQuestId());

			FText failTickerText = FText::FromString(failTickerString);
			FGsUIHelper::PopupOK(failTickerText);

			continue;
		}

		FGsQuestDynamicData* dynamicData = questData->GetQuestDynamicData(questKey._questIndex);
		if (nullptr == dynamicData)
			continue;		

		QuestState questState = dynamicData->GetQuestState();
		FString strState = GetEnumToString(QuestState, QuestState(questState));

		GSLOG(Log, TEXT("[Quest]FGsQuestHandler::SpawnInit QuestState : %s"), *strState);

		if (questId == questManager->GetFirstQuestId())
		{
			questInstance->Update(questState);
		}
	}

	questManager->UpdateGuideQuestActiveList();

	if (questManager->GetIsGotoMoveNpc())
	{
		if (questManager->IsSpawnMeCompleteStartProgress())
		{
			questManager->SetIsSpawnMeCompleteStartProgress(false);
			ReQuestNpcToAutoProgress();
		}
		else
		{
			questManager->SetIsGotoMoveNpc(false);
		}
		return;
	}

	if (questManager->IsWarpToNpc())
	{
		// 해당 npc로 인터렉션 발생
		UGsGameObjectBase* npc =
			GSGameObject()->FindObjectLocalNearestNonPlayerByNPCId(questManager->GetWarpToNpcId());

		if (nullptr != npc)
		{
			FGsInteractionMessageParam param;
			param._objectType = npc->GetObjectType();
			param._targetGameID = npc->GetGameId();
			GMessage()->GetGameObject().SendMessage(MessageGameObject::INTERACTION_BUTTON_CLICK, &param);
		}
		questManager->SetIsWarpToNpc(false);
	}
}

// 자동이동이 취소 될 만한 이벤트가 발생했을때
void FGsQuestHandler::OnUIAction(const FGsInputEventMsgBase& In_msg)
{
	OnUserAutoMoveStop();
}

void FGsQuestHandler::OnUIActionHudAuto(const IGsMessageParam* In_data)
{
	GSAI()->SetActiveAI(false);
	OnUserAutoMoveStop();
}

void FGsQuestHandler::OnUserAutoMoveStop(const IGsMessageParam* In_data)
{
	SetAutoMoveStop();
}

// 퀘스트 자동이동 off
// ai 자동전투 on
void FGsQuestHandler::OnActiveAIOffQuestAuto(const FGsInputEventMsgBase& In_msg)
{
	UGsQuestManager* questManager = GSQuest();
	if (nullptr == questManager)
		return;

	if (questManager->GetIsGotoMoveNpc())
		questManager->SetIsGotoMoveNpc(false);

	if (false == questManager->IsAutoProgressing())
		return;

	QuestKey activeQuestkey = GSQuest()->GetAutoProgressQuestKey();
	if (activeQuestkey.IsKeyNone())
		return;

	// 퀘스트 자동이동 off 
	SetAutoMoveStop();

	FGsPrimitiveInt32 param(static_cast<int32>(false));
	GMessage()->GetGameObject().SendMessage(MessageGameObject::AI_QUEST_AUTO_ACTIVE, &param);

	FGsQuestInstance* questInstance = FindQuestInstance(activeQuestkey);
	if (nullptr != questInstance)
	{
		// ai 자동전투 on
		GSAI()->SetActiveAI(true);		
	}
}

// 타겟지정 수동조작
void FGsQuestHandler::OnLocalTargetChanged(const IGsMessageParam* In_data)
{
	if (nullptr == In_data)
	{
		return;
	}

	const FGsGameObjectMessageParamTarget* castParam = In_data->Cast<const FGsGameObjectMessageParamTarget>();
	if (nullptr == castParam)
	{
		return;
	}

	FGsTargetHandlerBase::SelectTargetFilter selectTargetFilte = FGsTargetHandlerBase::SelectTargetFilter::None;
	if (!(FGsTargetHandlerBase::SelectTargetFilter::LockOn == castParam->_targetFilterType ||
		FGsTargetHandlerBase::SelectTargetFilter::ScanList == castParam->_targetFilterType))
	{
		return;
	}

	UGsQuestManager* questManager = GSQuest();
	if (nullptr == questManager)
		return;

	if (false == questManager->IsAutoProgressing())
		return;

	// 퀘스트 자동이동 off 
	SetAutoMoveStop();

	// ai 자동전투 on
	GSAI()->SetActiveAI(true);

	FGsPrimitiveInt32 param(static_cast<int32>(false));
	GMessage()->GetGameObject().SendMessage(MessageGameObject::AI_QUEST_AUTO_ACTIVE, &param);
}

void FGsQuestHandler::SetAutoMoveStop(const IGsMessageParam* In_data)
{
	UGsQuestManager* questManager = GSQuest();
	if (nullptr == questManager)
		return;

	QuestKey activeQuestkey = questManager->GetAutoProgressQuestKey();
	if (true == activeQuestkey.IsKeyNone())
		return;

	questManager->ClearAutoProgressQuestKey();	

	FGsQuestInstance* questInstance = FindQuestInstance(activeQuestkey);
	if (nullptr != questInstance)
	{
		questInstance->StopAutoProgress();
	}

	// AI 멈춤 전달
	// 요거 빼면 생기는 문제
	// 1. 퀘스트 이동(자동진행)
	// 2. 지형 pick 이동
	// 3. 지형 이동이 시작되고 다음 tick에서 aiquest에서 퀘스트 진행 상태 보고 멈춰버리면 퀘스트 이동이 아니라 
	// 다음에 들어온 지형 pic 이동을 멈추게함
	GMessage()->GetGameObject().SendMessage(MessageGameObject::AI_QUEST_AUTO_MOVE_STOP, nullptr);

	// ui 갱신
	GMessage()->GetContentsHud().SendMessage(MessageContentHud::INVALIDATE_QUEST);
}

void FGsQuestHandler::SetAutoMoveStart(const IGsMessageParam* In_data)
{
	QuestKey activeQuestkey = GSQuest()->GetAutoProgressQuestKey();
	if (false == activeQuestkey.IsKeyNone())
		return;

	FGsQuestInstance* questInstance = FindQuestInstance(activeQuestkey);
	if (nullptr == questInstance)
		return;

	questInstance->StartAutoProgress();
}

void FGsQuestHandler::OnSequencePlayerStop(const IGsMessageParam*)
{
	SetAutoMoveStart();
}

bool FGsQuestHandler::GetAutoMoveRemainDistance(OUT float& outRemainDistance)
{
	QuestKey activeQuestkey = GSQuest()->GetAutoProgressQuestKey();
	if (true == activeQuestkey.IsKeyNone())
		return false;

	FGsQuestInstance* questInstance = FindQuestInstance(activeQuestkey);

	if (nullptr == questInstance)
		return false;

	outRemainDistance = questInstance->GetRemainDistance();

	return true;
}

void FGsQuestHandler::ClearQuest()
{
	FlushAllInstance();
	// ui 갱신
	GMessage()->GetContentsHud().SendMessage(MessageContentHud::INVALIDATE_QUEST);
}

void FGsQuestHandler::ClearQuest(QuestType inQuestType)
{
	int num = _questInstanceList.Num();
	for (int i = num - 1; i >= 0; --i)
	{
		FGsQuestInstance* instance = _questInstanceList[i];
		if (nullptr == instance)
			continue;

		if (inQuestType == instance->GetQuestType())
		{
			instance->Clear();
			_questInstanceList.Remove(instance);

			if (nullptr == _questPoolManager)
				return;

			_questPoolManager->ReleaseInstance(inQuestType, instance);
		}
	}

	if (QuestType::MAIN == inQuestType)
		_mainQuestInstance = nullptr;

	// ui 갱신
	GMessage()->GetContentsHud().SendMessage(MessageContentHud::INVALIDATE_QUEST);
}

// 서버에 요청(수락, 완료)할 퀘스트아이디 추가
void FGsQuestHandler::AddRequestList(const QuestKey inQuestKey)
{
	if (false == _requestList.Contains(inQuestKey))
		_requestList.Add(inQuestKey);
}

// 서버에 응답(수락, 완료)받고 퀘스트아이디 삭제
void FGsQuestHandler::RemoveRequestList(const QuestKey inQuestKey)
{
	if (true == _requestList.Contains(inQuestKey))
		_requestList.Remove(inQuestKey);
}

// 서버에 요청중인가?
bool FGsQuestHandler::IsInRequestList(const QuestKey inQuestKey)
{
	return _requestList.Contains(inQuestKey);
}

bool FGsQuestHandler::IsInProgressList(const QuestKey inQuestKey)
{
	return (nullptr == FindQuestInstance(inQuestKey)) ? false : true;
}

bool FGsQuestHandler::IsPossibleQuestWarp(const QuestKey inQuestKey)
{
	if (_reserveQuestKey == inQuestKey)
	{
		if (true == _reserveIsAccpeted)
		{
			if (false == IsInProgressList(inQuestKey))
			{
				return false;
			}
		}
	}

	if (true == IsInRequestList(inQuestKey))
	{
		return false;
	}

	if (true == IsWarpReady())
	{
		return false;
	}

	if (true == IsUsingWarpRandomScroll())
	{
		return false;
	}

	return true;
}

int32 FGsQuestHandler::GetQuestCount(QuestType inQuestType)
{
	int32 count = 0;
	for (FGsQuestInstance* questInstance : _questInstanceList)
	{
		if (nullptr == questInstance)
			continue;

		if (questInstance->GetQuestType() == inQuestType)
		{
			++count;
		}

	}

	return count;
}

int32 FGsQuestHandler::GetQuestGuideCount()
{
	int32 count = 0;
	for (FGsQuestInstance* questInstance : _questInstanceList)
	{
		if (nullptr == questInstance)
			continue;

		if (QuestType::GUIDE == questInstance->GetQuestType())
		{
			if (true == questInstance->IsShowHudUI())
			{
				++count;
			}
		}
	}

	return count;
}

const QuestState FGsQuestHandler::GetQuestState(QuestKey inQuestKey)
{
	if (const FGsQuestInstance* questInstance = FindQuestInstance(inQuestKey))
	{
		return questInstance->GetQuestState();
	}

	return QuestState::NONE;
}

bool FGsQuestHandler::GetQuestObjectiveType(QuestKey inQuestKey, OUT QuestObjectiveType& outObjectiveType)
{
	if (FGsQuestInstance* questInstance = FindQuestInstance(inQuestKey))
	{
		return questInstance->GetActiveObjectiveType(outObjectiveType);
	}

	return false;
}

FGsQuestInstance* FGsQuestHandler::GetQuestInstance(StoryId inStoryId, QuestKey inQuestKey)
{
	return FindQuestInstance(inQuestKey, inStoryId);
}

// 퀘스트 액션 매니지먼트 접근
FGsQuestActionManagement* FGsQuestHandler::GetQuestActionManagement() const
{
	return GSQuest()->GetQuestActionManagement();
}

// 퀘스트 타겟 매니지먼트 접근
FGsQuestTargetManagement* FGsQuestHandler::GetQuestTargetManagement() const
{
	return GSQuest()->GetQuestTargetManagement();
}

void FGsQuestHandler::OnLocalPlayerDie(const IGsMessageParam* In_data)
{
	OnUserAutoMoveStop();
	OffUIActionHudAuto();
}

void FGsQuestHandler::OnActiveAI(const IGsMessageParam* In_data)
{
	const FGsPrimitiveInt32* param = In_data->Cast<const FGsPrimitiveInt32>();
	if (bool isActive = static_cast<bool>(param->_data))
	{
		OnUserAutoMoveStop();
	}
}

void FGsQuestHandler::OnChangeLocalPlayerLevel(uint64 InLevel)
{
	UGsQuestManager* questManager = GSQuest();
	if (nullptr == questManager)
		return;

	questManager->SetSubNpcTartget();
	//questManager->AddGuideQuestToHudAutomatically();
	questManager->UpdateGuideQuestActiveList();

	for (FGsQuestInstance* questInstance : _questInstanceList)
	{
		if (nullptr == questInstance)
			continue;

		questInstance->UpdateUIState();
	}

	// ui 갱신
	FGsMessageHolder* msg = GMessage();
	if (nullptr == msg)
		return;

	msg->GetGameObject().SendMessage(MessageGameObject::QUEST_WINDOW_UPDATE, nullptr);
	msg->GetContentsHud().SendMessage(MessageContentHud::INVALIDATE_QUEST);
	msg->GetHudRedDot().SendMessage(MessageContentHUDRedDot::QUEST, false);
}

// 퀘스트 액티브 끄기(다른 컨텐츠에서)
void FGsQuestHandler::OnQuestActiveOff(const IGsMessageParam*)
{
	OnUserAutoMoveStop();
}

void FGsQuestHandler::OnAutoMoveFailed(const IGsMessageParam* In_data)
{
}

// 퀘스트 자동진행 요청	
void FGsQuestHandler::OnAutoProgressRequest(const IGsMessageParam*)
{
	// 유저의 요청에 퀘스트가 자동진행 중이였고
	// 루팅의 이유로 멈춰 있다가
	// 다시 자동진행하라고 요청받았을때
	UGsQuestManager* questManager = GSQuest();
	if (nullptr == questManager)
		return;

	QuestKey activeQuestkey = GSQuest()->GetAutoProgressQuestKey();
	if (false == activeQuestkey.IsKeyNone())
		return;

	FGsQuestInstance* questInstance = FindQuestInstance(activeQuestkey);
	if (nullptr == questInstance)
		return;

	questInstance->UpdateAutoProgress();
}

// 이벤트 시작
void FGsQuestHandler::StartEvent(EGsEventProgressType In_startType, EGsEventProgressType In_preType)
{	
	GSLOG(Log, TEXT("FGsQuestHandler::StartEvent E In_startType:[%s], In_preType:[%s]"), *GetEnumToString(EGsEventProgressType, In_startType), *GetEnumToString(EGsEventProgressType, In_preType));

	if (EGsEventProgressType::UI_POPUP_TELEPORT == In_startType)
	{
		TWeakObjectPtr<UGsUIWidgetBase> widget = GUI()->OpenAndGetWidget(TEXT("PopupTeleport"));
		if (widget.IsValid())
		{
			UGsUIPopupTeleport* popup = Cast<UGsUIPopupTeleport>(widget.Get());
			if (popup)
			{
				popup->SetData(PopValidReservedCostPackage(), _reserveQuestKey, _reserveObjectiveId, _reserveIsAccpeted);
			}
		}
	}
	else if (EGsEventProgressType::UI_POPUP_QUESTACCEPT == In_startType)
	{
		FGsQuestInstance* questInstance = FindQuestInstance(_reserveQuestKey, _reserveStoryIdAccpet);
		if (nullptr == questInstance)
		{
			// 존재하지않는 퀘스트이다.
			return;
		}

		if (false == IsInRequestList(questInstance->GetQuestKey()))
		{
			// 열기를 실패할 경우, 바로 종료
			if (false == OpenPopupQuestAccept(questInstance))
			{
				GSEventProgress()->OnReqFinishEvent(EGsEventProgressType::UI_POPUP_QUESTACCEPT);
			}
		}
	}
	else if (EGsEventProgressType::UI_POPUP_QUESTCOMPLETE == In_startType)
	{
		FGsQuestInstance* questInstance = FindQuestInstance(QuestKey::KeyNone(), _reserveStoryIdReward);
		if (nullptr == questInstance)
		{
			// 존재하지않는 퀘스트이다.
			return;
		}

		if (false == IsInRequestList(questInstance->GetQuestKey()))
		{
			// 열기를 실패할 경우, 바로 종료
			if (false == OpenPopupQuestReward(questInstance))
			{
				GSEventProgress()->OnReqFinishEvent(EGsEventProgressType::UI_POPUP_QUESTCOMPLETE);
			}
		}
	}
	else if (EGsEventProgressType::QUEST_AUTO_TELEPORT == In_startType)
	{
		// 워프요청
		FGsNetSendServiceWorld::SendQuestWarp(_reserveQuestKey._questId, _reserveQuestKey._questIndex, _reserveObjectiveId, _reserveIsAccpeted);

		if (UGsQuestManager* questManager = GSQuest())
		{
			questManager->SetWarpQuestId(_reserveQuestKey);
			questManager->SetIsSpawnMeCompleteStartProgress(true);
		}

		FGsEventProgressManager* eventProgressManager = GSEventProgress();

		if (eventProgressManager != nullptr)
		{
			eventProgressManager->ClearAllData();
		}
	}
}

// 이벤트 종료
void FGsQuestHandler::FinishEvent(EGsEventProgressType In_finishType, EGsEventProgressType In_nextType)
{
	GSLOG(Log, TEXT("FGsQuestHandler::FinishEvent F In_finishType:[%s], In_nextType:[%s]"), *GetEnumToString(EGsEventProgressType, In_finishType), *GetEnumToString(EGsEventProgressType, In_nextType));

	if (EGsEventProgressType::UI_POPUP_TELEPORT == In_finishType)
	{
		GUI()->CloseByKeyName(TEXT("PopupTeleport"), true);
	}
	else if (EGsEventProgressType::UI_POPUP_QUESTACCEPT == In_finishType)
	{
		
	}
	else if (EGsEventProgressType::UI_POPUP_QUESTCOMPLETE == In_finishType)
	{
		
	}
	else if (EGsEventProgressType::QUEST_AUTO_TELEPORT == In_finishType)
	{		
	}
}

void FGsQuestHandler::OnScanClick(const IGsMessageParam*)
{
	for (FGsQuestInstance* questInstance : _questInstanceList)
	{
		if (nullptr == questInstance)
			continue;

		if (questInstance->GetQuestType() == QuestType::GUIDE)
		{
			FGsQuestGuideInstance* guideInstance = static_cast<FGsQuestGuideInstance*>(questInstance);
			guideInstance->UpdateObjectiveScanType();
		}
	}
}

void FGsQuestHandler::UpdateQuestGuide()
{
	UGsQuestManager* questManager = GSQuest();
	if (nullptr == questManager)
		return;

	const TArray<QuestKey>& activeQuestDataList = questManager->GetActiveQuestKeyList();
	int num = activeQuestDataList.Num();
	for (int i = 0; i < num; ++i)
	{
		if (false == activeQuestDataList.IsValidIndex(i))
			continue;

		const QuestKey& questKey = activeQuestDataList[i];
		QuestId questId = questKey._questId;

		TSharedPtr<FGsQuestData> questData = questManager->GetQuestData(questId);
		if (!questData.IsValid())
		{
#ifdef WITH_EDITOR
			const FString failTickerString =
				FString::Printf(TEXT("find fail QuestData, QuestId: %d"), questId);

			FText failTickerText = FText::FromString(failTickerString);
			FGsUIHelper::PopupOK(failTickerText);
#endif //WITH_EDITOR
			continue;
		}

		if (QuestType::GUIDE != questData->GetQuestType())
			continue;

		if (nullptr == FindQuestInstance(questKey))
		{
			if (FGsQuestInstance* questInstance = ClaimQuestInstance(questData->GetStoryId(), questKey))
			{
				FGsQuestDynamicData* dynamicData = questData->GetQuestDynamicData(questKey._questIndex);
				if (nullptr == dynamicData)
					continue;

				questInstance->Update(dynamicData->GetQuestState());
			}
		}
	}

	GMessage()->GetGameObject().SendMessage(MessageGameObject::QUEST_WINDOW_UPDATE, nullptr);
	GMessage()->GetContentsHud().SendMessage(MessageContentHud::INVALIDATE_QUEST);
}

bool FGsQuestHandler::IsQuestGuideShowHud(StoryId inStoryId)
{
	int32 count = 0;
	for (FGsQuestInstance* questInstance : _questInstanceList)
	{
		if (nullptr == questInstance)
			continue;

		if (QuestType::GUIDE != questInstance->GetQuestType())
			continue;

		if (inStoryId != questInstance->GetStoryId())
			continue;

		if (true == questInstance->IsShowHudUI())
		{
			return true;
		}
	}

	return false;
}

void FGsQuestHandler::RemoveQuestGuide(const StoryId inStoryId)
{
	for (FGsQuestInstance* questInstance : _questInstanceList)
	{
		if (nullptr == questInstance)
			continue;

		if (QuestType::GUIDE != questInstance->GetQuestType())
			continue;

		if (inStoryId != questInstance->GetStoryId())
			continue;

		RemoveQuestInstance(questInstance);
		break;
	}
}


void FGsQuestHandler::InvalidGuideInstance(const IGsMessageParam* In_data)
{
	FGsQuestGuideManagement* guideManagement = GSQuest()->GetQuestGuideManagement();
	if (nullptr == guideManagement)
		return;
	
	TArray<StoryId> guideList = guideManagement->GetGuideVaildList();
	TArray<StoryId> deleteStoryIdList;
	for (auto It = _questInstanceList.CreateConstIterator(); It; ++It)
	{
		FGsQuestInstance* questInstance = static_cast<FGsQuestInstance*>(*It);
		if (nullptr == questInstance)
			continue;

		if (QuestType::GUIDE != questInstance->GetQuestType())
			continue;

		StoryId storyId = questInstance->GetStoryId();
		if (!guideList.Contains(storyId))
		{
			deleteStoryIdList.Add(storyId);
		}
	}

	for (StoryId deleteStoryid : deleteStoryIdList)
	{
		FGsQuestInstance* deleteQuestInstance = FindQuestInstance(QuestKey::KeyNone(), deleteStoryid);
		if (nullptr != deleteQuestInstance)
		{
			RemoveQuestInstance(deleteQuestInstance);
		}
	}

	if (UGsQuestManager* questMgr = GSQuest())
	{
		for (StoryId vaildStoryId : guideList)
		{
			QuestId questId = questMgr->GetGuideQuestIdByStoryId(vaildStoryId);
			QuestKey questKey = QuestKey(questId);

			TSharedPtr<FGsQuestData> questData = questMgr->GetQuestData(questKey);
			if (false == questData.IsValid())
				continue;

			if (nullptr == FindQuestInstance(questKey))
			{
				if (FGsQuestInstance* questInstance = ClaimQuestInstance(vaildStoryId, questKey))
				{
					FGsQuestDynamicData* dynamicData = questData->GetQuestDynamicData(questKey._questIndex);
					if (nullptr == dynamicData)
					{
						dynamicData = questData->ClaimQuestDynamicData();
					}

					questInstance->Update(dynamicData->GetQuestState());
				}
			}
		}
	}

	// Window 가 열려 있는 상태라면 신규 가이드 퀘스트 해금 시 해금 상태를 바로 갱신 (정렬 X)
	GMessage()->GetQuest().SendMessage(MessageContentQuest::QUEST_GUIDE_AREA_REFRESH, nullptr);
}

const FGsSchemaQuestRepeatScrollRefreshInfoSet* FGsQuestHandler::GetRepeatRefrehTable(int32 inRepeatScrollId)
{
	if (const FGsSchemaQuestRepeatScroll* row =
		UGsTableUtil::FindRowById<UGsTableQuestRepeatScroll, FGsSchemaQuestRepeatScroll>(inRepeatScrollId))
	{
		return row->refreshInfoSet.GetRow();
	}

	return nullptr;
}

TSharedRef<FGsCostPackage> FGsQuestHandler::PopValidReservedCostPackage()
{
	if (0 == _reservedCostPackage->GetCostElementsCount())
	{
		// 현재 지정된 비용이 하나도 없다면 골드:0원으로 임의 처리함.
		GSLOG(Error, TEXT("0 == _reservedCostPackage->GetCostElementsCount(). Need to add any cost element."));
		_reservedCostPackage->AddCost(CurrencyType::GOLD, 0);
	}

	return _reservedCostPackage;
}

void FGsQuestHandler::OnQuestWindowOpen(const IGsMessageParam*)
{
	FGsNetSendServiceQuest::SendReqQuestRepeatRewardBoxList();
}

bool FGsQuestHandler::OpenPopupAccept(QuestType InType, const IGsMessageParam* InParam, RewardType InMainType, RewardType InSubType, bool bIsTest)
{
	// 메인 퀘스트에서 자동으로 팝업을 띄워주는데 EventProgress를 사용해서 실제 팝업 오픈은 다음 틱에 불림 
	// 튜토리얼 발생시 팝업을 모두 닫는데, 다음 틱에 수락 팝업이 오픈이 되어 튜토리얼 진행이 막히는 문제 수정
	if (QuestType::MAIN == InType)
	{		
		if (FGsTutorialManager* tutorialMgr = GSTutorial())
		{
			// 튜토리얼 중엔 수락창 출력을 막되, 첫 튜토리얼은 퀘스트 수락 팝업 안내를 하므로 예외처리
			if (tutorialMgr->IsActiveTutorial())
			{
				if (false == tutorialMgr->IsFirstTutorial())
				{
					return false;
				}
			}
		}
	}	

	// 반복 퀘스트 타입인가
	bool bIsRepeat = (QuestType::REPEAT == InType) ? true : false;
	if (false == bIsRepeat)
	{
		// 메인이나 서브보상에 퀘스트 다시 받기가 있는가
		bIsRepeat = (RewardType::PICK_REWARD_BOX_LIST == InMainType) ? true : false;
		if (RewardType::PICK_REWARD_BOX_LIST == InSubType)
		{
			bIsRepeat = true;
		}
	}
	
	if (bIsRepeat)
	{
		TWeakObjectPtr<UGsUIWidgetBase> widget = GUI()->OpenAndGetWidget(TEXT("PopupQuestAcceptRepeat"));
		if (widget.IsValid())
		{
			if (UGsUIPopupQuestAcceptRepeat* popup = Cast<UGsUIPopupQuestAcceptRepeat>(widget.Get()))
			{
				popup->SetIsTest(bIsTest);
				popup->SetData(InParam);
			}
		}
	}
	else
	{
		TWeakObjectPtr<UGsUIWidgetBase> widget = GUI()->OpenAndGetWidget(TEXT("PopupQuestAccept"));
		if (widget.IsValid())
		{
			if (UGsUIPopupQuestAccept* popup = Cast<UGsUIPopupQuestAccept>(widget.Get()))
			{
				popup->SetIsTest(bIsTest);
				popup->SetData(InParam);
			}
		}
	}

	return true;
}	

bool FGsQuestHandler::OpenPopupReward(const FGsQuestPopUpParamBase* InParam, RewardType InMainType, RewardType InSubType, bool bIsTest)
{
	// 튜토리얼 중엔 보상창 출력을 막는다
	if (GSTutorial()->IsActiveTutorial())
	{
		return false;
	}

	// 인벤토리 상태 체크
	if (false == FGsRewardHelper::CheckInventoryState(InParam->_mainRewardId, InParam->_subRewardId))
	{
		return false;
	}

	// 메인이 랜덤 연출
	if (RewardType::RANDOM_ONE == InMainType ||
		RewardType::PICK_REWARD_BOX_LIST == InMainType)
	{
		TWeakObjectPtr<UGsUIWidgetBase> widget = GUI()->OpenAndGetWidget(TEXT("PopupQuestRewardMainRandom"));
		if (widget.IsValid())
		{
			if (UGsUIPopupQuestRewardRandom* popup = Cast<UGsUIPopupQuestRewardRandom>(widget.Get()))
			{
				popup->SetIsTest(bIsTest);
				popup->SetData(InParam);
			}
		}

		return true;
	}
	
	// 서브보상은 없을 수 있음
	if (RewardType::MAX != InSubType)
	{
		// 서브가 랜덤 연출
		if (RewardType::RANDOM_ONE == InSubType ||
			RewardType::SUB_RANDOM_ONE == InSubType ||
			RewardType::PICK_REWARD_BOX_LIST == InSubType)
		{
			TWeakObjectPtr<UGsUIWidgetBase> widget = GUI()->OpenAndGetWidget(TEXT("PopupQuestRewardSubRandom"));
			if (widget.IsValid())
			{
				if (UGsUIPopupQuestRewardRandom* popup = Cast<UGsUIPopupQuestRewardRandom>(widget.Get()))
				{
					popup->SetIsTest(bIsTest);
					popup->SetData(InParam);
				}
			}

			return true;
		}
	}

	// 랜덤 연출 없음
	TWeakObjectPtr<UGsUIWidgetBase> widget = GUI()->OpenAndGetWidget(TEXT("PopupQuestReward"));
	if (widget.IsValid())
	{
		if (UGsUIPopupQuestReward* popup = Cast<UGsUIPopupQuestReward>(widget.Get()))
		{
			popup->SetIsTest(bIsTest);
			popup->SetData(InParam);
		}
	}

	return true;
}

void FGsQuestHandler::OnLocalCampHit(const IGsMessageParam* In_data)
{
	const FGsCampActorMessageParam* param = In_data->Cast<const FGsCampActorMessageParam>();
	if (param == nullptr)
	{
		return;
	}
	
	for (FGsQuestInstance* questInstance : _questInstanceList)
	{
		if (nullptr == questInstance)
			continue;

		questInstance->OnCampHit(param->_campActor);
	}
}

void FGsQuestHandler::OnQuestRepeatComplete()
{
	GSLOG(Error, TEXT("OnQuestRepeatComplete 0"));

	UGsLevelManager* levelMgr = GLevel();
	if (nullptr == levelMgr)
		return;

	UGsQuestManager* questManager = GSQuest();
	if (nullptr == questManager)
		return;

	UWorld* world = questManager->GetWorld();
	if (nullptr == world)
		return;

	if (true == IsUsingWarpRandomScroll())
	{
#if WITH_EDITOR
		GSLOG(Log, TEXT("OnQuestRepeatComplete _isUseWarpRandomScroll == true"));
#endif
		return;
	}

	if (true == IsWarpReady())
	{
#if WITH_EDITOR
		GSLOG(Log, TEXT("OnQuestRepeatComplete _isWarpReady == true"));
#endif
		return;
	}

	bool isUseRandomScroll = questManager->IsUseRandomScroll();
	if (isUseRandomScroll)
	{
		questManager->SetIsUseRandomScroll(false);
	}

	// 플레이어가 옵션에서 자동진행을 워프로 선택하였는가? 
	bool isQuestAutoTeleport = true;
	if (UGsGameUserSettings* gameUserSettings = GGameUserSettings())
	{
		isQuestAutoTeleport =
			static_cast<bool>(gameUserSettings->GetOutputSetting(EGsOptionOutput::QUEST_AUTO_TELEPORT));
	}

	// 이미 수행중인 반복퀘스트가 있다면
	if (true == questManager->IsAutoProgressing())
	{
		QuestKey questKey = questManager->GetAutoProgressQuestKey();
		if (questKey.IsKeyNone())
			return;

		if (QuestType::REPEAT != questManager->GetQuestType(questKey._questId))
			return;

		FGsQuestInstance* currQuestInstance = FindQuestInstance(questKey);
		if (nullptr == currQuestInstance)
			return;

		FGsQuestRepeatInstance* repeatInstance = static_cast<FGsQuestRepeatInstance*>(currQuestInstance);
		if (nullptr == repeatInstance)
			return;

		if (RepeatStoryType::BOARD != repeatInstance->GetRepeatStoryType())
			return;

		EGsQuestUIState outState;
		currQuestInstance->GetQuestUIState(outState);
		if (EGsQuestUIState::QUEST_STATE_PROGRESS == outState &&	// 현재 수행가능한 상태인가?
			currQuestInstance->IsShowHudUI())						// 허드에서 등록된 퀘스트만 자동진행 또는 자동워프 가능							
		{
			if (isQuestAutoTeleport && isUseRandomScroll && currQuestInstance->GetIsTeleportAble())
			{
				OnAutoTeleport(currQuestInstance);
			}
		}
	}
	else
	{
		if (false == questManager->IsRepeatQuestAutoPressing())
			return;

		// 다음 퀘스트 체크 시 퀘스트 규칙 순 정렬
		QuestInstancetListSort();
		// 다음 반복퀘가 있을 시 자동진행한다.
		for (FGsQuestInstance* questInstance : _questInstanceList)
		{
			if (nullptr == questInstance)
				continue;

			if (QuestType::REPEAT != questInstance->GetQuestType())
				continue;

			FGsQuestRepeatInstance* repeatInstance = static_cast<FGsQuestRepeatInstance*>(questInstance);
			if (nullptr == repeatInstance)
				continue;

			if (RepeatStoryType::BOARD != repeatInstance->GetRepeatStoryType())
				continue;

			EGsQuestUIState outState;
			questInstance->GetQuestUIState(outState);
			if (EGsQuestUIState::QUEST_STATE_PROGRESS == outState &&	// 현재 수행가능한 상태인가?
				questInstance->IsShowHudUI())							// 허드에서 등록된 퀘스트만 자동진행 또는 자동워프 가능										
			{
				if (isQuestAutoTeleport && isUseRandomScroll && questInstance->GetIsTeleportAble())
				{
					OnAutoTeleport(questInstance);
					return;
				}

				// 워프선택되어 있으면 워프
				if (isQuestAutoTeleport)
				{
					if (questInstance->GetIsTeleportAble())	// 워프가 가능한 퀘스트인가?
					{
						// 행동제약 상태면
						if (UGsGameObjectBase* local = GSGameObject()->FindObject(EGsGameObjectType::LocalPlayer))
						{
							if (local->IsCrowdControlLock(CreatureActionType::CREATURE_ACTION_TYPE_WARP))
							{
								_repeatWarpWaiting = true;
								return;
							}
						}

						if (false == OnAutoTeleport(questInstance))
						{
							FGsQuestSlotTouchParam param(questInstance->GetQuestKey(), true, 0, true);
							OnTouchQuest(&param);
						}
					}
				}
				else
				{
					FGsQuestSlotTouchParam param(questInstance->GetQuestKey(), true, 0, true);
					OnTouchQuest(&param);
				}

				// 다시 상태값을 false 셋팅
				questManager->SetIsRepeatQuestAutoPressing(false);

				// ui 갱신
				GMessage()->GetContentsHud().SendMessage(MessageContentHud::INVALIDATE_QUEST);
				return;
			}
		}
	}
}

bool FGsQuestHandler::OnAutoTeleport(FGsQuestInstance* inQuest)
{
	if (nullptr == inQuest)
	{
		GSLOG(Error, TEXT("Quest instance is missing"));
		return false;
	}

	// 자동이동이 불가능
	UGsQuestManager* questManager = GSQuest();
	if (nullptr != questManager)
	{
		if (false == questManager->IsTeleportPossible(inQuest->GetQuestType(), inQuest->GetStoryId()))
		{
			FText result;
			FText::FindText(TEXT("UserTeleportText"), TEXT("ErrorTeleportInvalidState"), result);
			FGsUIHelper::TrayMessageTicker(result);

			return false;
		}
	}

	int32 outTeleportGold;
	QuestObjectiveId outObjectiveId;
	if (false == inQuest->GetTeleportData(outTeleportGold, outObjectiveId))
	{
		GSLOG(Error, TEXT("Get Teleport data fail - quest id : %lld"), inQuest->GetQuestId());
		return false;
	}

	_reservedCostPackage = FGsCostPackage::CreateSimple(CurrencyType::GOLD, outTeleportGold);

	// 텔레포트 비용 할인 객체가 있을 경우, 추가
	TSharedPtr<FGsCostOption> CostOption = FGsContentsEventTeleportCost::CreateActivatedTeleportCostOption();
	if (CostOption.IsValid())
	{
		_reservedCostPackage->AddCostOption(CostOption.ToSharedRef());
	}

	// 비용 부족
	if (!_reservedCostPackage->PlayerHasEnough())
	{
		// 이동에 필요한 재화가 부족합니다
		FText findText;
		FText::FindText(TEXT("QuestUIText"), TEXT("AutoQuestTeleport_NoGold"), findText);
		FGsUIHelper::TrayMessageTicker(findText);

		return false;
	}

	_reserveObjectiveId = outObjectiveId;
	_reserveQuestId = inQuest->GetQuestId();
	_reserveQuestKey = inQuest->GetQuestKey();
	_reserveIsAccpeted = true;

	GSLOG(Log, TEXT("FGsQuestHandler::OnTeleport S EventProgress Type : QUEST_AUTO_TELEPORT - quest id : %lld"), _reserveQuestId);

	GSEventProgress()->OnReqStartEvent(EGsEventProgressType::QUEST_AUTO_TELEPORT, this);

	return true;
}

void FGsQuestHandler::QuestInstancetListSort()
{
	TArray<FGsQuestInstance*> questMainList;
	TArray<FGsQuestInstance*> questSubList;
	TArray<FGsQuestInstance*> questRepeatList;
	TArray<FGsQuestInstance*> questRepeatCompletedList;
	TArray<FGsQuestInstance*> questRepeatScrollList;
	TArray<FGsQuestInstance*> questRepeatSortList;
	TArray<FGsQuestInstance*> questOtherList;

	for (auto& iter : _questInstanceList)
	{
		if (iter->GetQuestType() == QuestType::MAIN)
		{
			questMainList.Add(iter);
		}
		else if (iter->GetQuestType() == QuestType::SUB)
		{
			questSubList.Add(iter);
		}
		else if (iter->GetQuestType() == QuestType::REPEAT)
		{
			RepeatStoryType repeatStoryType = GSQuest()->GetRepeatStoryType(iter->GetStoryId());
			if (RepeatStoryType::BOARD == repeatStoryType)
			{
				if (iter->GetQuestState() == QuestState::COMPLETED)
				{
					questRepeatCompletedList.Add(iter);
				}
				else
				{
					questRepeatList.Add(iter);
				}
			}
			else
			{
				questRepeatScrollList.Add(iter);
			}
		}
		else
		{
			questOtherList.Add(iter);
		}
	}

	questRepeatList.Sort([](const FGsQuestInstance& A, const FGsQuestInstance& B)
		{
			if (A.GetQuestId() < B.GetQuestId())
				return true;
			else if (A.GetQuestId() == B.GetQuestId())
			{
				if (A.GetQuestIndex() < B.GetQuestIndex())
					return true;
			}
			return false;
		});

	if (FGsServerOption* serverOption = GServerOption())
	{
		const TArray<QuestStoryIdPair>& questIndexList = serverOption->GetRepeatQuestviewIndexList();
		for (QuestStoryIdPair idPair : questIndexList)
		{
			QuestId questId = idPair.mQuestId;
			QuestIndex questIndex = idPair.mQuestIndex;

			FGsQuestInstance** instance = questRepeatList.FindByPredicate(
				[questId, questIndex](const FGsQuestInstance* key) -> bool
				{
					return key->GetQuestIndex() == questIndex && key->GetQuestId() == questId;
				});

			if (nullptr != instance)
			{
				questRepeatSortList.Add(*instance);
			}
		}

		int sortNum = questRepeatSortList.Num();
		int num = questRepeatList.Num();
		if (sortNum != num)
		{
			for (FGsQuestInstance* questInstance : questRepeatList)
			{
				QuestIndex index = questInstance->GetQuestIndex();
				bool find = questRepeatSortList.ContainsByPredicate(
					[index](const FGsQuestInstance* key) -> bool
					{
						return key->GetQuestIndex() == index;
					});

				if (false == find)
				{
					questRepeatSortList.Add(questInstance);
				}
			}
		}
	}

	_questInstanceList.Empty();
	_questInstanceList += questMainList
		+= questSubList
		+= questRepeatCompletedList
		+= questRepeatSortList
		+= questRepeatScrollList
		+= questOtherList;
}

void FGsQuestHandler::OnUseWarpItem(const IGsMessageParam* InParam)
{
	const FGsPrimitiveInt32* param = InParam->Cast<const FGsPrimitiveInt32>();
	if (nullptr == param)
		return;

	UGsQuestManager* questManager = GSQuest();
	if (nullptr == questManager)
		return;
	
	ItemCategorySub itemCategorySub = static_cast<ItemCategorySub>(param->_data);
	if (itemCategorySub == ItemCategorySub::WARP_TO_RANDOM_SCROLL ||
		itemCategorySub == ItemCategorySub::WARP_TO_BOOKMARK_SCROLL)
	{	
		// 자동텔레포트직전, 랜덤순간이동주문서사용한상태인경우
		// 퀘스트진행유지→ 랜덤순간이동이후, 퀘스트텔레포트(O)		
#if WITH_EDITOR
		GSLOG(Log, TEXT("OnUseWarpItem WARP_TO_RANDOM_SCROLL"));
#endif
		SetUsingWarpRandomScroll(true);
		questManager->SetIsUseRandomScroll(true);
			
		return;
	}
	else if (itemCategorySub == ItemCategorySub::WARP_TO_TOWN_SCROLL || 
		itemCategorySub == ItemCategorySub::WARP_TO_AGIT)
	{
		questManager->SetIsUseWarpScroll(true);		
	}

	OffUIActionHudAuto();
}

void FGsQuestHandler::OnRequestContentsWarp(const IGsMessageParam* InParam)
{
	OffUIActionHudAuto();
}

void FGsQuestHandler::OffUIActionHudAuto()
{
	UGsQuestManager* questManager = GSQuest();
	if (nullptr == questManager)
		return;

	QuestKey activeQuestkey = questManager->GetAutoProgressQuestKey();
	if (true == activeQuestkey.IsKeyNone())
		return;

	questManager->ClearAutoProgressQuestKey();
	questManager->SetIsRepeatQuestAutoPressing(false);

	FGsQuestInstance* questInstance = FindQuestInstance(activeQuestkey);
	if (nullptr != questInstance)
	{
		questInstance->StopAutoProgress();
		// ai 자동전투 on
		GSAI()->SetActiveAI(false);
	}

	GMessage()->GetGameObject().SendMessage(MessageGameObject::AI_QUEST_AUTO_MOVE_STOP, nullptr);
	GMessage()->GetContentsHud().SendMessage(MessageContentHud::INVALIDATE_QUEST);

	FGsPrimitiveInt32 param(static_cast<int32>(false));
	GMessage()->GetGameObject().SendMessage(MessageGameObject::AI_QUEST_AUTO_ACTIVE, &param);
}

void FGsQuestHandler::OnLocalReaction(const IGsMessageParam* In_data)
{
	UGsQuestManager* questManager = GSQuest();
	if (nullptr == questManager)
		return;

	if (false == questManager->IsRepeatQuestAutoPressing())
		return;	

	if (_repeatWarpWaiting)
	{
		if (UGsGameObjectBase* local = GSGameObject()->FindObject(EGsGameObjectType::LocalPlayer))
		{
			if (false == local->IsCrowdControlLock(CreatureActionType::CREATURE_ACTION_TYPE_WARP))
			{
				OnQuestRepeatComplete();
				_repeatWarpWaiting = !_repeatWarpWaiting;
			}
		}
	}
}

void FGsQuestHandler::OnWarpReady()
{
#if WITH_EDITOR
	GSLOG(Log, TEXT("GsQuestHandler::OnWarpReady()"));
#endif

	// 워프대기가 방생했다.
	SetWarpReady(true);

	// 랜덤 이동 주문서를 사용해서 워프 대기가 발생했다.
	if (true == IsUsingWarpRandomScroll())
	{
		// 랜덤 이동 주문서 사용과
		// 워프대기 상태의 공간을 체크하기 위해 
		// 세팅했던 값 초기화
		SetUsingWarpRandomScroll(false);
	}
}

void FGsQuestHandler::OnWarpFailure()
{
	// 랜덤 이동 주문서를 사용해서 워프 대기 중 실패가 왔기때문에
	// 랜덤 이동 주문서 사용 값 초기화
	if (true == IsUsingWarpRandomScroll())
	{
		SetUsingWarpRandomScroll(false);
	}

	UGsQuestManager* questManager = GSQuest();
	if (nullptr == questManager)
		return;

	questManager->SetIsUseRandomScroll(false);

	// 워프 사용 전에 완료 된 반복 퀘스트가 있었으므로
	// 다음 퀘스트 진행을 불러준다.
	if (questManager->IsRepeatQuestAutoPressing())
	{
		OnQuestRepeatComplete();
	}
}

void FGsQuestHandler::ClearUsingWarpScroll()
{
	SetUsingWarpRandomScroll(false);

	UGsQuestManager* questManager = GSQuest();
	if (nullptr == questManager)
		return;

	questManager->SetIsUseRandomScroll(false);
}