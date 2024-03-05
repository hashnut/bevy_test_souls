#pragma once

#include "GsNetMessageHandleInterface.h"
#include "Message/GsMessageNet.h"
#include "Net/GSNet.h"
#include "Shared/Client/SharedEnums/SharedEventEnum.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "Shared/Shared/SharedPackets/PD_Gateway_Client.h"
#include "UTIL/GsTimeSyncUtil.h"

class FGsNetManager;

class FGsNetMessageHandlerContentsEvent final : public IGsNetMessageHandler, FGsNetMessageGateway
{

	friend class FGsNetSendServiceContentsEvent;

public:
	/**
	 * 컨텐츠이벤트그룹 덮어쓰기 데이터
	 */
	struct FGsContentsEventGroupData final
	{
		const ContentsEventGroupId _contentsEventGroupId;
		const FDateTime _startUtc;
		const FDateTime _endUtc;

		FGsContentsEventGroupData(const ContentsEventGroupId InContentsEventGroupId, const FDateTime InStartUtc, const FDateTime InEndUtc)
			: _contentsEventGroupId(InContentsEventGroupId)
			, _startUtc(InStartUtc)
			, _endUtc(InEndUtc)
		{
		}

		FGsContentsEventGroupData(PD::GC::PKT_GC_CONTENTS_EVENT_GROUP_ACTIVATED_LIST_READ::ContentsEventList& InContentsEventList)
			: FGsContentsEventGroupData(InContentsEventList.GroupId(), FGsTimeSyncUtil::ConvertServerTimeToUTCDateTime(InContentsEventList.StartTime()), FGsTimeSyncUtil::ConvertServerTimeToUTCDateTime(InContentsEventList.EndTime()))
		{
		}

		FGsContentsEventGroupData(PD::GC::PKT_GC_CONTENTS_EVENT_GROUP_ACTIVATE_READ& InContentsEventGroupData)
			: FGsContentsEventGroupData(InContentsEventGroupData.GroupId(), FGsTimeSyncUtil::ConvertServerTimeToUTCDateTime(InContentsEventGroupData.StartTime()), FGsTimeSyncUtil::ConvertServerTimeToUTCDateTime(InContentsEventGroupData.EndTime()))
		{
		}
	};

	/**
	 * 컨텐츠이벤트 덮어쓰기 데이터
	 */
	struct FGsContentsEventData final
	{
		const ContentsEventType _contentsEventType;
		const ContentsEventId _contentsEventId;

		FGsContentsEventData(const ContentsEventType InContentsEventType, const ContentsEventId InContentsEventId)
			: _contentsEventType(InContentsEventType)
			, _contentsEventId(InContentsEventId)
		{
		}

		FGsContentsEventData(PD::GC::PKT_GC_CONTENTS_EVENT_ACTIVATED_LIST_READ::ActivatedList& InActivatedList)
			: FGsContentsEventData(InActivatedList.ContentsEventType(), InActivatedList.ContentsEventId())
		{
		}
	};

	/************************************************************************/
	/* instancing															*/
	/************************************************************************/

public:
	void InitializeMessage(FGsNetManager* InManager) final;

	void FinalizeMessage() final;

	/**
	 */
	void CONTENTS_EVENT_GROUP_ACTIVATED_LIST(const FGsNet::Packet& InPacket);

	/**
	 */
	void CONTENTS_EVENT_GROUP_ACTIVATE(const FGsNet::Packet& InPacket);

	/**
	 */
	void CONTENTS_EVENT_GROUP_DEACTIVATE(const FGsNet::Packet& InPacket);

	/**
	 */
	void CONTENTS_EVENT_ACTIVATED_LIST(const FGsNet::Packet& InPacket);

	/**
	 */
	void CONTENTS_EVENT_ACTIVATE(const FGsNet::Packet& InPacket);

	/**
	 */
	void CONTENTS_EVENT_DEACTIVATE(const FGsNet::Packet& InPacket);
	
};
