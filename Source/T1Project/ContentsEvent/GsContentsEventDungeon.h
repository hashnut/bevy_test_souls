#pragma once

#include "DataSchema/ContentsEvent/GsSchemaContentsEventDungeon.h"
#include "GsContentsEventBase.h"
#include "Shared/Client/SharedEnums/SharedEventEnum.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"

class FGsContentsEventDungeon final
	: public TGsContentsEventBase<ContentsEventType::DUNGEON, FGsSchemaContentsEventDungeon, UGsTableContentsEventDungeon>
{

private:
	using Super = TGsContentsEventBase<ContentsEventType::DUNGEON, FGsSchemaContentsEventDungeon, UGsTableContentsEventDungeon>;

public:
	FGsContentsEventDungeon(const ContentsEventGroupId InContentsEventGroupId, const ContentsEventId InContentsEventId)
		: Super(InContentsEventGroupId, InContentsEventId)
	{
	}

};
