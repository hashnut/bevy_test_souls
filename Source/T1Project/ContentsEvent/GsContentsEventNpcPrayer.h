#pragma once

#include "DataSchema/ContentsEvent/GsSchemaContentsEventNpcPrayer.h"
#include "GsContentsEventBase.h"
#include "Message/GsMessageContents.h"
#include "Shared/Client/SharedEnums/SharedEventEnum.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"

struct IGsMessageParam;

class FGsContentsEventNpcPrayer final
	: public TGsContentsEventBase<ContentsEventType::NPC_PRAYER, FGsSchemaContentsEventNpcPrayer, UGsTableContentsEventNpcPrayer>
{

private:
	using Super = TGsContentsEventBase<ContentsEventType::NPC_PRAYER, FGsSchemaContentsEventNpcPrayer, UGsTableContentsEventNpcPrayer>;

private:
	static TPair<MessageContentContentsEvent, FDelegateHandle> _contentsEventStateChangedCompleted;

public:
	FGsContentsEventNpcPrayer(const ContentsEventGroupId InContentsEventGroupId, const ContentsEventId InContentsEventId);

protected:
	bool IsActivatedSelf() const final;

	void OnChangeActivate(const bool InIsActivated) final;

public:
	static void OnContentsEventStateChangedCompleted(const IGsMessageParam& InMessageParam);

};
