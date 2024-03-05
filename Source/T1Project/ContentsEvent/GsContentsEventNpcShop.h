#pragma once

#include "DataSchema/ContentsEvent/GsSchemaContentsEventNpcShop.h"
#include "GsContentsEventBase.h"
#include "Message/GsMessageContents.h"
#include "Shared/Client/SharedEnums/SharedEventEnum.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"

struct IGsMessageParam;

class FGsContentsEventNpcShop final
	: public TGsContentsEventBase<ContentsEventType::NPC_SHOP, FGsSchemaContentsEventNpcShop, UGsTableContentsEventNpcShop>
{

private:
	using Super = TGsContentsEventBase<ContentsEventType::NPC_SHOP, FGsSchemaContentsEventNpcShop, UGsTableContentsEventNpcShop>;

private:
	static TPair<MessageContentContentsEvent, FDelegateHandle> _contentsEventStateChangedCompleted;

public:
	FGsContentsEventNpcShop(const ContentsEventGroupId InContentsEventGroupId, const ContentsEventId InContentsEventId);


public:
	static void OnContentsEventStateChangedCompleted(const IGsMessageParam& InMessageParam);
};
