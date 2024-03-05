#pragma once

#include "DataSchema/ContentsEvent/GsSchemaContentsEventReward.h"
#include "GsContentsEventBase.h"
#include "Shared/Client/SharedEnums/SharedEventEnum.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"

class FGsContentsEventReward final : public TGsContentsEventBase<ContentsEventType::REWARD, FGsSchemaContentsEventReward, UGsTableContentsEventReward>
{

	using Super = TGsContentsEventBase<ContentsEventType::REWARD, FGsSchemaContentsEventReward, UGsTableContentsEventReward>;

public:
	FGsContentsEventReward(const ContentsEventGroupId InContentsEventGroupId, const ContentsEventId InContentsEventId)
		: Super(InContentsEventGroupId, InContentsEventId)
	{
	}

};
