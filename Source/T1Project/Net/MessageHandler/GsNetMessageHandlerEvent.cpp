#include "GsNetMessageHandlerEvent.h"

#include "Runtime/CoreUObject/Public/UObject/NoExportTypes.h"
#include "Runtime/Engine/Classes/Engine/Engine.h"

#include "Management/ScopeGame/GsEventManager.h"
#include "Management/ScopeGlobal/GsNetManager.h"
#include "Management/ScopeGlobal/GsUIManager.h"
#include "Message/MessageParam/GsEventMessageParam.h"

#include "../Event/EventAction/GsEventActionCommonData.h"

#include "../UTIL/GsTableUtil.h"
#include "UI/UIContent/Tray/GsUITraySubDialog.h"
#include "UI/UIContent/Helper/GsUIHelper.h"

#include "Shared/Shared/SharedPackets/PD_World_Client_Bypass.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"

#include "../T1Project.h"



using namespace PD::SC;

void FGsNetMessageHandlerEvent::InitializeMessage(FGsNetManager* inManager)
{
	MProtocalWorld& ProtocalWorld = inManager->GetProtocalWorld();

	InitializeMessageDelegateList(&ProtocalWorld);

	ProtocalWorld.AddRaw(SCPacketId::SC_ACK_EVENT_ACTION, this, &FGsNetMessageHandlerEvent::AckEventAction);

	ProtocalWorld.AddRaw(SCPacketId::SC_EVENT_SCREEN_MESSAGE, this, &FGsNetMessageHandlerEvent::AckOnEventActionImmediate);
}

void FGsNetMessageHandlerEvent::FinalizeMessage()
{
	FinalizeMessageDelegateList();
}

/**
 * FGsNetSendServiceWorld::SendEventAction 의 응답
 */
void FGsNetMessageHandlerEvent::AckEventAction(const FGsNet::Packet& InPacket)
{
	PKT_SC_ACK_EVENT_ACTION_READ* Packet = reinterpret_cast<PKT_SC_ACK_EVENT_ACTION_READ*>(InPacket.Buffer);
	check(Packet);

	GSLOG(Log, TEXT("PKT_SC_ACK_EVENT_ACTION_READ, mResult:%d, mEventActionQuestId:%llu, mEventActionState:%d, mEventActionGroupId:%d, mEventActionIndex:%d"), Packet->Result(), Packet->EventActionQuestId(), Packet->EventActionState(), Packet->EventActionGroupId(), Packet->EventActionIndex());

	UGsEventManager* EventManager = GSEvent();
	if (nullptr == EventManager)
	{
		GSLOG(Warning, TEXT("nullptr == EventManager"));
		return;
	}

	const bool Succeeded = (Packet->Result() == PACKET_RESULT_SUCCESS);
	EventManager->EventActionPlayResponse(Succeeded, Packet->EventActionQuestId(), Packet->EventActionState(), Packet->EventActionGroupId(), Packet->EventActionIndex());
	if (!Succeeded)
	{
		GSLOG(Warning, TEXT("Packet->Result(): %d"), Packet->Result());

		PD::Result result = static_cast<PD::Result>(Packet->Result());
		// TODO: 하드코딩
		if (PD::Result::EVENT_ACTION_ERROR_ALL_COMPLETED == result || PD::Result::EVENT_ACTION_ERROR_INVALID_INDEX == result || PD::Result::EVENT_ACTION_ERROR_USER_WARP == result)
		{
			FGsUIHelper::TraySectionMessageTickerNetError(result);
			return;
		}

		FGsUIHelper::PopupNetError(result);
	}
}

void FGsNetMessageHandlerEvent::AckOnEventActionImmediate(const FGsNet::Packet& InPacket)
{
	PKT_SC_EVENT_SCREEN_MESSAGE_READ* Packet = reinterpret_cast<PKT_SC_EVENT_SCREEN_MESSAGE_READ*>(InPacket.Buffer);
	check(Packet);

	TArray<EvenParameterValue> paramDataSet;
	using ItParamType = PktIterator<PD::SC::PKT_SC_EVENT_SCREEN_MESSAGE_READ::ParameterList>;
	for (ItParamType iter = Packet->GetFirstParameterListIterator(); iter != Packet->GetLastParameterListIterator(); ++iter)
	{
		paramDataSet.Emplace(iter->ParameterValue());
	}

	EventSubDialogId eventId = Packet->EventId();
	bool isActive = Packet->Active();

	UGsEventManager* eventManager = GSEvent();
	if (nullptr == eventManager)
	{
		GSLOG(Warning, TEXT("nullptr == eventManager"));
		return;
	}

	eventManager->EventActionPlayImmediate(eventId, isActive, paramDataSet);
}