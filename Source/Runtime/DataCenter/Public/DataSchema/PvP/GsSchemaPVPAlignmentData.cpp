/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#include "DataSchema/PVP/GsSchemaPVPAlignmentData.h"
#include "Runtime/CoreUObject/Public/UObject/NoExportTypes.h"
#include "Runtime/Engine/Classes/Engine/DataTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/DataTable/GsDataTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
FGsSchemaPVPAlignmentData::FGsSchemaPVPAlignmentData()
{
	SET_ALL_PROPERTIES_FROM_ASSETSCHEMA_DEFAULT(FGsSchemaPVPAlignmentData)
	
	Id = 0;
	pvpAlignmentPointMax = 0;
	threatDuration = 0;
	userKillPoint = 0;
	grade = PVPAlignmentGrade::NETURAL;
}
