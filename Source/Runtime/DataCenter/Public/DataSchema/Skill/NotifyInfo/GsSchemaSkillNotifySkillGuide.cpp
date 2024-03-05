/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#include "Runtime/DataCenter/Public/DataSchema/Skill/NotifyInfo/GsSchemaSkillNotifySkillGuide.h"
#include "Runtime/Engine/Classes/EditorFramework/AssetImportData.h"
#include "Runtime/Engine/Classes/Engine/DataTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/DataTable/GsDataTable.h"
FGsSchemaSkillNotifySkillGuide::FGsSchemaSkillNotifySkillGuide()
{
	SET_ALL_PROPERTIES_FROM_ASSETSCHEMA_DEFAULT(FGsSchemaSkillNotifySkillGuide)
	
	duration = 0.100000f;
	effectPath = FSoftObjectPath(TEXT("/Game/Blueprint/SkillGuardLine/BP_GuidePcAimCircle.BP_GuidePcAimCircle"));
	isAttach = false;
	isOverride = false;
	overrideCollisionInfo.DataTable = nullptr;
}
