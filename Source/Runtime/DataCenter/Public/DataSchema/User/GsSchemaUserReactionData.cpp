/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#include "Runtime/DataCenter/Public/DataSchema/User/GsSchemaUserReactionData.h"
#include "../Plugins/RSQL/Source/Runtime/Public/DataTable/GsDataTable.h"
FGsSchemaUserReactionData::FGsSchemaUserReactionData()
{
	SET_ALL_PROPERTIES_FROM_ASSETSCHEMA_DEFAULT(FGsSchemaUserReactionData)
	
	genderType = CreatureGenderType::ALL;
	reactionInfo = TArray<FGsSchemaUserReactionInfo> ();
	reactionInfo.AddUninitialized(1);
	FGsSchemaUserReactionInfo::StaticStruct()->InitializeStruct(reactionInfo.GetData(), 1);
	auto& __Local__0 = reactionInfo[0];
}
