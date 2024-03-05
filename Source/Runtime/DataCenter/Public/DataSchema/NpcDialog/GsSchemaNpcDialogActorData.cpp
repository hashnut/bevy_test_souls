/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#include "DataSchema/NpcDialog/GsSchemaNpcDialogActorData.h"
#include "Runtime/CoreUObject/Public/UObject/NoExportTypes.h"
#include "Runtime/Engine/Classes/EditorFramework/AssetImportData.h"
#include "Runtime/Engine/Classes/Engine/DataTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/DataTable/GsDataTable.h"
FGsSchemaNpcDialogActorData::FGsSchemaNpcDialogActorData()
{
	SET_ALL_PROPERTIES_FROM_ASSETSCHEMA_DEFAULT(FGsSchemaNpcDialogActorData)
	
	actorType = EGsNpcDialogActorType::ACTOR_MY;
	isMotionPlay = true;
	expressionType = EGsEmoticonType::EMOTICON_EMPTY;
	preActorState = EGsNpcDialogActorStateType::NONE;
	PostActorState = EGsNpcDialogActorStateType::NONE;
}
