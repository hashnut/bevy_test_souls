#include "GsStageGameBase.h"

#include "Management/GsMessageHolder.h"
#include "Message/MessageParam/GsStageMessageParam.h"

void FGsStageGameBase::Enter()
{
	FGsGameStageEnterMsgParam param(GetType());
	GMessage()->GetStageParam().SendMessage(MessageStage::ENTER_STAGE_CALLBACK, &param);
}

void FGsStageGameBase::Exit()
{
	FGsStageFenceMsgParam param(false, false, nullptr);
	GMessage()->GetStageParam().SendMessage(MessageStage::LEAVE_STAGE_CALLBACK, &param);
}