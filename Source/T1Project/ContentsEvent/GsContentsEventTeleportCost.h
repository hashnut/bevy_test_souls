#pragma once

#include "DataSchema/ContentsEvent/GsSchemaContentsEventTeleportCost.h"
#include "GsContentsEventBase.h"
#include "Shared/Client/SharedEnums/SharedEventEnum.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"

struct FGsCostOption;

class FGsContentsEventTeleportCost final
	: public TGsContentsEventBase<ContentsEventType::TELEPORT_COST, FGsSchemaContentsEventTeleportCost, UGsTableContentsEventTeleportCost>
{

private:
	using Super = TGsContentsEventBase<ContentsEventType::TELEPORT_COST, FGsSchemaContentsEventTeleportCost, UGsTableContentsEventTeleportCost>;

public:
	FGsContentsEventTeleportCost(const ContentsEventGroupId InContentsEventGroupId, const ContentsEventId InContentsEventId)
		: Super(InContentsEventGroupId, InContentsEventId)
	{
	}

public:
	/**
	 * 할인 계산에 사용할 코스트옵션
	 */
	TSharedPtr<FGsCostOption> MakeCostOption() const;

public:
	/**
	 * 현재 활성화된 텔레포트 비용 옵션이 있다면, 그 비용 옵션을 반환함.
	 * 활성화된 텔레포트 비용이 없다면, nullptr 반환함.
	 */
	static TSharedPtr<FGsCostOption> CreateActivatedTeleportCostOption();

};
