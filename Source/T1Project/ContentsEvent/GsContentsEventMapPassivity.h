#pragma once

#include "DataSchema/ContentsEvent/GsSchemaContentsEventMapPassivity.h"
#include "GsContentsEventBase.h"
#include "Shared/Client/SharedEnums/SharedEventEnum.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"

class FGsContentsEventMapPassivity final
	: public TGsContentsEventBase<ContentsEventType::MAP_PASSIVITY, FGsSchemaContentsEventMapPassivity, UGsTableContentsEventMapPassivity>
{

private:
	using Super = TGsContentsEventBase<ContentsEventType::MAP_PASSIVITY, FGsSchemaContentsEventMapPassivity, UGsTableContentsEventMapPassivity>;

public:
	FGsContentsEventMapPassivity(const ContentsEventGroupId InContentsEventGroupId, const ContentsEventId InContentsEventId)
		: Super(InContentsEventGroupId, InContentsEventId)
	{
	}

};
