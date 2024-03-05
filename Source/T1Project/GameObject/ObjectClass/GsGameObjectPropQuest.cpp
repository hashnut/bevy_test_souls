
#include "GsGameObjectPropQuest.h"
#include "GameObject/Quest/GsQuestHandler.h"
#include "Management/ScopeGlobal/GsQuestManager.h"
#include "Quest/GsQuestInstance.h"
#include "Net/GsNetSendServiceWorld.h"

void UGsGameObjectPropQuest::SetQuestTargetActive(bool inIsActive, int In_Index /*= 0*/)
{
	//로컬 플레이어가 인터렉션 중이라면 로컬에서만 퀘스트 마크 숨김 처리(두번 인터렉션 하는 것을 막기 위해)
	bool isActive = (inIsActive
		&& (false == _isInteracting));

	SetQuestMarkActive(isActive);
	SetInteractionActive(isActive, In_Index);
}

void UGsGameObjectPropQuest::InvalidWidget()
{
	//콜리전 킬 수 있는 여부를 먼저 업데이트 하고 퀘스트 마크를 킬지 말지 결정한다. 
	bool isFinish = IsFinish(_interactionEndUserCount);
	bool canInteraction = false;
	if (false == isFinish)
	{
		canInteraction = CanInteraction(_interactionStartUserCount, _interactionEndUserCount);

#if PROP_INTERACTION_DEBUG && false
		if (false == canInteraction)
		{
			GSLOG(Log, TEXT("prop debug - InvalidWidget - canInteraction is false - id : %lld\tinteraction start count : %d\tinteraction end count : %d"), GetGameId(), _interactionStartUserCount, _interactionEndUserCount);
		}
		else
		{
			GSLOG(Log, TEXT("prop debug - InvalidWidget - canInteraction is true - id : %lld\tinteraction start count : %d\tinteraction end count : %d"), GetGameId(), _interactionStartUserCount, _interactionEndUserCount);
		}
#endif
	}
	else
	{
#if PROP_INTERACTION_DEBUG && false
		GSLOG(Log, TEXT("prop debug - InvalidWidget - prop interaction is finish - id : %lld\tmulti interaction count : %d\tcurrent interaction count : %d"), GetGameId(), _propData->multipleInteractionCountMax, _interactionEndUserCount);
#endif
	}

	SetCanInteraction(canInteraction);

	bool isActive = (false == _isInteracting) && IsQuestTarget();
	SetQuestTargetActive(isActive);
}

void UGsGameObjectPropQuest::SendInteractionStart(const struct FGsPropInteractionMessageParamContents* inParam)
{
	FGsQuestHandler* handler = GSQuest()->GetQuestHandler();
	if (nullptr == handler)
	{
		return;
	}

	const TArray<FGsQuestInstance*>&  questInstanceArray = handler->GetQuesInstancetList();
	TArray<QuestPreocessData> preocessDataList;
	QuestObjectiveId outObjectiveId;
	for (const FGsQuestInstance* questInstance : questInstanceArray)
	{
		if (nullptr == questInstance
			|| false == questInstance->IsInteractionPropContents(inParam->_tableId, outObjectiveId))
		{
			continue;
		}

		preocessDataList.Emplace(questInstance->GetQuestId(), outObjectiveId);
	}

	if (0 == preocessDataList.Num())
	{
		return;
	}

	FGsNetSendServiceWorld::SendInteractQuestPropStart(GetGameId(), preocessDataList);
}

void UGsGameObjectPropQuest::SendInteractionEnd(const struct FGsPropInteractionMessageParamContents* inParam)
{
	FGsQuestHandler* handler = GSQuest()->GetQuestHandler();
	if (nullptr == handler)
	{
		return;
	}

	const TArray<FGsQuestInstance*>& questInstanceArray = handler->GetQuesInstancetList();
	TArray<QuestPreocessData> preocessDataList;
	QuestObjectiveId outObjectiveId;
	for (const FGsQuestInstance* questInstance : questInstanceArray)
	{
		if (nullptr == questInstance
			|| false == questInstance->IsInteractionPropContents(inParam->_tableId, outObjectiveId))
		{
			continue;
		}			

		preocessDataList.Emplace(questInstance->GetQuestId(), outObjectiveId);
	}

	if (0 == preocessDataList.Num())
	{
		return;
	}

	FGsNetSendServiceWorld::SendInteractQuestPropEnd(GetGameId(), preocessDataList);
}

bool UGsGameObjectPropQuest::CheckNeedToDeactiveInteractionWidget()
{
	return false;
}
