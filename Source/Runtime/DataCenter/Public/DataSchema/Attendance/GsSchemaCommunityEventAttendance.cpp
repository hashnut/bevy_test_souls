/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#include "Runtime/DataCenter/Public/DataSchema/Attendance/GsSchemaCommunityEventAttendance.h"
#include "Runtime/CoreUObject/Public/UObject/NoExportTypes.h"
#include "Runtime/Engine/Classes/EditorFramework/AssetImportData.h"
#include "Runtime/Engine/Classes/Engine/DataTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/DataTable/GsDataTable.h"
FGsSchemaCommunityEventAttendance::FGsSchemaCommunityEventAttendance()
{
	SET_ALL_PROPERTIES_FROM_ASSETSCHEMA_DEFAULT(FGsSchemaCommunityEventAttendance)
	
	id = 0;
	attendanceType = AttendanceType::DEFAULT;
	reuse = false;
	rewardList = TArray<FGsSchemaAttendanceReward> ();
	rewardList.AddUninitialized(1);
	FGsSchemaAttendanceReward::StaticStruct()->InitializeStruct(rewardList.GetData(), 1);
	auto& __Local__0 = rewardList[0];
}
