/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#include "Runtime/DataCenter/Public/DataSchema/Cinematic/GsSchemaSequenceResData.h"
#include "Runtime/Engine/Classes/EditorFramework/AssetImportData.h"
#include "Runtime/Engine/Classes/Engine/DataTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/DataTable/GsDataTable.h"
FGsSchemaSequenceResData::FGsSchemaSequenceResData()
{
	SET_ALL_PROPERTIES_FROM_ASSETSCHEMA_DEFAULT(FGsSchemaSequenceResData)
	
	sequenceResId = 0;
	isSkip = false;
	sequenceTextType = EGsSequenceTextType::SEQUENCE_TEXT_TYPE_NONE;
	durationSec = 0;
	viewRewardId.DataTable = nullptr;
	dimmedWaitTime = 0.000000f;
	isCallDimmedOff = false;
	soundMix = EGsSoundMixType::Cinemas;
}
