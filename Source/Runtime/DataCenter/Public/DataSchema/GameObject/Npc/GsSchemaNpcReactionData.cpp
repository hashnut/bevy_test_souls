/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#include "DataSchema/GameObject/Npc/GsSchemaNpcReactionData.h"
#include "../Plugins/RSQL/Source/Runtime/Public/DataTable/GsDataTable.h"
FGsSchemaNpcReactionData::FGsSchemaNpcReactionData()
{
	SET_ALL_PROPERTIES_FROM_ASSETSCHEMA_DEFAULT(FGsSchemaNpcReactionData)
	
	id = 0;
	reactionInfo = TArray<FGsSchemaNpcReactionInfo> ();
	reactionInfo.AddUninitialized(1);
	FGsSchemaNpcReactionInfo::StaticStruct()->InitializeStruct(reactionInfo.GetData(), 1);
	auto& __Local__0 = reactionInfo[0];
}
