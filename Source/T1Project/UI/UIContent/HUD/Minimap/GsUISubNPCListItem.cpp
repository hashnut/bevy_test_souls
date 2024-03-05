#include "GsUISubNPCListItem.h"

#include "GameObject/Quest/GsQuestHandler.h"
#include "UI/UIControlLib/ContentWidget/GsButton.h"
#include "Management/GsMessageHolder.h"
#include "Management/ScopeGame/GsGameObjectManager.h"
#include "Message/MessageParam/GsMessageParam.h"
#include "DataSchema/GameObject/Npc/GsSchemaNpcFunctionData.h"
#include "T1Project.h"
#include "UMG/Public/Components/WidgetSwitcher.h"
#include "UMG/Public/Components/TextBlock.h"
#include "../Message/GsMessageGameObject.h"
#include "../Message/MessageParam/GsQuestMessageParam.h"
#include "../Message/MessageParam/GsAIMessageParam.h"
#include "../Management/ScopeGame/GsClientSpawnManager.h"
#include "DataSchema/Map/Spot/GsSchemaSpotInfo.h"
#include "Shared/Client/SharedEnums/SharedQuestEnum.h"
#include "Runtime/DataCenter/Public/Quest/GsQuestEnum.h"
#include "DataSchema/GameObject/Npc/GsSchemaNpcData.h"
#include "DataSchema/Quest/GsSchemaQuestObjectiveAreaPos.h"
#include "../../Helper/GsUIHelper.h"
#include "UMG/Public/Blueprint/UserWidget.h"

void UGsUISubNPCListItem::NativeOnInitialized()
{
	_btnItem->OnClicked.AddDynamic(this, &UGsUISubNPCListItem::OnClickItem);

	Super::NativeOnInitialized();
}

void UGsUISubNPCListItem::OnClickItem()
{
	if (nullptr == _moveData)
		return;

	if (_moveData->GetQuestNpcState() == EQuestNpcState::STATE_SUB_WATING)
	{
		FGsAIQuestAutoMoveMessageParam param;
		param._questId = _moveData->GetQuestId();
		param._moveType = QuestObjectiveType::INTERACT_NPC;
		param._mapId = _moveData->GetLevel();
		param._range = _moveData->GetInNpcInteractionRange();
		param._npcId = _moveData->GetNpcId();

		if (FGsClientSpawnManager* spawnManager = GSClientSpawn())
		{
			FGsSchemaSpotInfo frontSpotData;
			if (true == spawnManager->TryGetSpot(_moveData->GetLevel(), _moveData->GetNpcPosInfo().spotId, frontSpotData))
			{
				param._arrPos.Emplace(frontSpotData.pos);
			}
		}

		GMessage()->GetGameObject().SendMessage(MessageGameObject::QUEST_UIACTION_NPC_TO_AUTOPROGRESS, &param);

		FText findText;
		FText::FindText(TEXT("QuestUIText"), TEXT("SubAutomoveNpc"), findText);

		FGsUIHelper::TrayMessageTicker(findText);
	}
	else
	{
		FGsQuestSlotTouchParam param(QuestKey(0), true, _moveData->GetStoryId());
		GMessage()->GetGameObject().SendMessage(MessageGameObject::QUEST_UIACTION_TOUCH, &param);

		FText findText;
		FText::FindText(TEXT("QuestUIText"), TEXT("SubAutomoveGoal"), findText);

		FGsUIHelper::TrayMessageTicker(findText);
	}
}

// 데이터 세팅
void UGsUISubNPCListItem::SetData(TSharedPtr<FGsSubQuestNpcMoveData> inMoveData)
{
	if (!inMoveData.IsValid())
		return;

	_moveData = inMoveData;

	int32 npcId = inMoveData->GetNpcId();
	const FGsSchemaNpcData* npcData = UGsTableUtil::FindRowById<UGsTableNpcData, FGsSchemaNpcData>(npcId);
	if (nullptr == npcData)
		return;

	_textNPCName->SetText(npcData->nameText);
	
	// 진행중 1 , 퀘스트 대기 0
	int index = (inMoveData->GetQuestNpcState() == EQuestNpcState::STATE_SUB_ACCEPT) ? 1 : 0;
	_switcherNpcStateIcon->SetActiveWidgetIndex(index);
}
