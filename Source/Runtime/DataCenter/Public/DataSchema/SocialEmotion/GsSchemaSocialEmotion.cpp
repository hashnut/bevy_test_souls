/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#include "Runtime/DataCenter/Public/DataSchema/SocialEmotion/GsSchemaSocialEmotion.h"
#include "Runtime/Engine/Classes/EditorFramework/AssetImportData.h"
#include "Runtime/Engine/Classes/Engine/DataTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/DataTable/GsDataTable.h"
FGsSchemaSocialEmotion::FGsSchemaSocialEmotion()
{
	SET_ALL_PROPERTIES_FROM_ASSETSCHEMA_DEFAULT(FGsSchemaSocialEmotion)
	
	socialId = 0;
	socialCategory = UserSocialEmotion::NONE;
	isLoopMotion = false;
	order = 0;
	isLock = false;
	secCoolTime = 0;
	itemId.DataTable = nullptr;
}
