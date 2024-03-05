#include "GsContentsEventNpcPrayer.h"

#include "DataSchema/GameObject/Npc/GsSchemaNpcData.h"

#include "Management/GsMessageHolder.h"

#include "Management/ScopeGame/GsGameObjectManager.h"

#include "Message/GsMessageContentHud.h"

#include "GameFlow/GameContents/ContentsGame/Hud/Skill/GsSkillUIHandler.h"

#include "GameObject/ObjectClass/GsGameObjectNonPlayer.h"

#include "Shared/Shared/SharedInclude/SharedTypes.h"

#include "Message/MessageParam/GsContentsEventMessageParam.h"

#include "T1Project.h"

#include "Shared/Client/SharedEnums/SharedEventEnum.h"

#include "Classes/GsMessageHandlerOneParam.h"

#include "Message/MessageParam/GsMessageParam.h"

#include "Message/GsMessageContents.h"

TPair<MessageContentContentsEvent, FDelegateHandle> FGsContentsEventNpcPrayer::_contentsEventStateChangedCompleted;

FGsContentsEventNpcPrayer::FGsContentsEventNpcPrayer(const ContentsEventGroupId InContentsEventGroupId, const ContentsEventId InContentsEventId)
	: Super(InContentsEventGroupId, InContentsEventId)
{
	MContentsEvent* ContentsEvent = FGsContentsEventMessageParamContentsEventStateChangedCompleted::GetMessageHandler();
	if (nullptr == ContentsEvent)
	{
		GSLOG(Error, TEXT("nullptr == ContentsEvent"));
	}
	else
	{
		if (FGsContentsEventNpcPrayer::_contentsEventStateChangedCompleted.Value.IsValid())
		{
			ContentsEvent->RemoveIF(FGsContentsEventNpcPrayer::_contentsEventStateChangedCompleted);
		}

		FGsContentsEventNpcPrayer::_contentsEventStateChangedCompleted = ContentsEvent->AddStatic(MessageContentContentsEvent::CONTENTS_EVENT_STATE_CHANGED_COMPLETED, &FGsContentsEventNpcPrayer::OnContentsEventStateChangedCompleted);
	}
}

bool FGsContentsEventNpcPrayer::IsActivatedSelf() const
{
	return Super::IsActivatedSelf();
}

void FGsContentsEventNpcPrayer::OnChangeActivate(const bool InIsActivated)
{
	const FGsSchemaContentsEventNpcPrayer* TableData = _tableData;


	// notify to gameobject nonplayer(event prayer)

	if (TableData == nullptr)
	{
		return;
	}
	const FGsSchemaNpcData* npcData = TableData->npcData.GetRow();
	if (npcData == nullptr)
	{
		return;
	}

	UGsGameObjectBase* findObj =
		GSGameObject()->FindObjectLocalNearestNonPlayerByNPCId(npcData->id);
	if (findObj == nullptr)
	{
		return;
	}

	UGsGameObjectNonPlayer* nonPlayerObj = Cast<UGsGameObjectNonPlayer>(findObj);
	if (nonPlayerObj == nullptr)
	{
		return;
	}
	nonPlayerObj->SetInteractionActive(InIsActivated);
}

void FGsContentsEventNpcPrayer::OnContentsEventStateChangedCompleted(const IGsMessageParam& InMessageParam)
{
	const FGsContentsEventMessageParamContentsEventStateChangedCompleted* MessageParam
		= StaticCast<const FGsContentsEventMessageParamContentsEventStateChangedCompleted*>(&InMessageParam);

	if (nullptr == MessageParam)
	{
		GSLOG(Error, TEXT("nullptr == MessageParam"));
		return;
	}

	if (!MessageParam->IsAffected(ContentsEventType::NPC_PRAYER))
	{
		return;
	}

	GSLOG(Log, TEXT("FGsContentsEventNpcPrayer::OnContentsEventStateChangedCompleted()"));

	// notify to hud slot(if in town)
	if (FGsSkillUIHandler::IsSaftyZone() == true)
	{
		GMessage()->GetContentsHud().SendMessage(MessageContentHud::INVALIDATE_NPCFUNCTION);
	}
}
