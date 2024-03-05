/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#include "Runtime/DataCenter/Public/DataSchema/CommunityGirl/GsSchemaCommunityEvent.h"
#include "Runtime/Engine/Classes/EditorFramework/AssetImportData.h"
#include "Runtime/Engine/Classes/Engine/DataTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/DataTable/GsDataTable.h"
FGsSchemaCommunityEvent::FGsSchemaCommunityEvent()
{
	SET_ALL_PROPERTIES_FROM_ASSETSCHEMA_DEFAULT(FGsSchemaCommunityEvent)
	
	id = 0;
	isTest = false;
	tabIndex = 0;
	eventType = CommunityEventType::BANNER;
	durationHoursReceive = 0;
	durationHoursComplete = 0;
	communityEventConditionType = CommunityEventConditionType::DEFAULT_USER;
	eventTimeTextType = ECommunityEventTimeType::ALWAYS;
	detail.DataTable = nullptr;
}
