/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#include "Runtime/DataCenter/Public/DataSchema/Chat/GsSchemaChatTypeViewInfo.h"
#include "Runtime/CoreUObject/Public/UObject/NoExportTypes.h"
#include "Runtime/Engine/Classes/EditorFramework/AssetImportData.h"
#include "Runtime/Engine/Classes/Engine/DataTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/DataTable/GsDataTable.h"
FGsSchemaChatTypeViewInfo::FGsSchemaChatTypeViewInfo()
{
	SET_ALL_PROPERTIES_FROM_ASSETSCHEMA_DEFAULT(FGsSchemaChatTypeViewInfo)
	
	chaType = EGsExChatType::NORMAL;
	useChatType = EGsExChatType::NORMAL;
	viewType = EGsChatViewType::NORMAL;
	accountGradeTag = {};
	accountGradeTag.Reserve(2);
	accountGradeTag.Add(0, FString(TEXT("Normal")));
	accountGradeTag.Add(4, FString(TEXT("GM")));
	useOtherWorldPrefix = false;
	prefixFmt = FString(TEXT("<TAG>[{0}]</>"));
	nameFmt = FString(TEXT("<TAG>[{0}] </>"));
	textFmt = FString(TEXT("<TAGl>{0}</>"));
}
