#include "GsContentsEventNpcShop.h"

#include "Management/GsMessageHolder.h"

#include "Message/MessageParam/GsContentsEventMessageParam.h"

#include "Message/GsMessageContentHud.h"

#include "GameFlow/GameContents/ContentsGame/Hud/Skill/GsSkillUIHandler.h"

#include "Shared/Client/SharedEnums/SharedEventEnum.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"

TPair<MessageContentContentsEvent, FDelegateHandle> FGsContentsEventNpcShop::_contentsEventStateChangedCompleted;

FGsContentsEventNpcShop::FGsContentsEventNpcShop(const ContentsEventGroupId InContentsEventGroupId, const ContentsEventId InContentsEventId)
	: Super(InContentsEventGroupId, InContentsEventId)
{
	MContentsEvent* ContentsEvent = FGsContentsEventMessageParamContentsEventStateChangedCompleted::GetMessageHandler();
	if (nullptr == ContentsEvent)
	{
		GSLOG(Error, TEXT("nullptr == ContentsEvent"));
	}
	else
	{
		if (FGsContentsEventNpcShop::_contentsEventStateChangedCompleted.Value.IsValid())
		{
			ContentsEvent->RemoveIF(FGsContentsEventNpcShop::_contentsEventStateChangedCompleted);
		}

		FGsContentsEventNpcShop::_contentsEventStateChangedCompleted = ContentsEvent->AddStatic(MessageContentContentsEvent::CONTENTS_EVENT_STATE_CHANGED_COMPLETED, &FGsContentsEventNpcShop::OnContentsEventStateChangedCompleted);
	}
}

void FGsContentsEventNpcShop::OnContentsEventStateChangedCompleted(const IGsMessageParam& InMessageParam)
{
	const FGsContentsEventMessageParamContentsEventStateChangedCompleted* MessageParam
		= StaticCast<const FGsContentsEventMessageParamContentsEventStateChangedCompleted*>(&InMessageParam);

	if (nullptr == MessageParam)
	{
		GSLOG(Error, TEXT("nullptr == MessageParam"));
		return;
	}

	if (!MessageParam->IsAffected(ContentsEventType::NPC_SHOP))
	{
		return;
	}

	// notify to hud slot(if in town)
	if (FGsSkillUIHandler::IsSaftyZone() == true)
	{
		GMessage()->GetContentsHud().SendMessage(MessageContentHud::INVALIDATE_NPCFUNCTION);
	}
}
