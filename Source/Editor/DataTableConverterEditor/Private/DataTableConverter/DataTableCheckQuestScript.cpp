
#include "Runtime/CoreUObject/Public/UObject/UObjectGlobals.h"
#include "DataTableConverter/GsDataTableConvertBase.h"
#include "GsDataTable.h"

#include "DataSchema/Quest/GsSchemaQuest.h"
#include "DataSchema/Quest/Main/GsSchemaQuestMainStory.h"
#include "DataSchema/Quest/Main/GsSchemaQuestMainChapter.h"
#include "DataSchema/SubDialog/GsSchemaSubDialogData.h"

#include "LogMacros.h"
#include "NotificationManager.h"
#include "DataSchema/NpcDialog/GsSchemaNpcDialogCutData.h"


#define DEF_ERR_POPUP_MESSAGE(Message)\
FNotificationInfo Info(Message);\
Info.ExpireDuration = 10.0f;\
Info.bFireAndForget = true;\
Info.Image = FCoreStyle::Get().GetBrush(TEXT("MessageLog.Warning"));\
FSlateNotificationManager::Get().AddNotification(Info);\
UE_LOG(LogTemp, Error, TEXT("[Quest Script Error] %s"), *Message.ToString())

void PrintQuestKeyAndText(const FText& questScript)
{
	TOptional<FString> key = FTextInspector::GetKey(questScript);
	UE_LOG(LogTemp, Error, TEXT("[Quest Script] %s\t%s"), *key.GetValue(), *questScript.ToString());
}

bool CheckNpcDialogCutData(const FGsSchemaNpcDialogCutData* npcDialogCutData)
{
	if (nullptr == npcDialogCutData)
	{
		return false;
	}

	PrintQuestKeyAndText(npcDialogCutData->scriptText);

	return true;
}

bool CheckNpcDialogCutGroupData(const FGsSchemaNpcDialogCutGroupData* subDialogCutGroupData)
{
	if (nullptr == subDialogCutGroupData)
	{
		return false;
	}

	for (const FGsSchemaNpcDialogCutDataRow& el : subDialogCutGroupData->cutDataId)
	{
		CheckNpcDialogCutData(el.GetRow());
	}

	return true;
}

bool CheckSubDialogCutData(const FGsSchemaSubDialogCutData* subDialogCutData)
{
	if (nullptr == subDialogCutData)
		return false;

	PrintQuestKeyAndText(subDialogCutData->script);

	return true;
}

bool CheckNpcDialogData(const FGsSchemaNpcDialogData* npdDialogData)
{
	if (nullptr == npdDialogData)
		return false;

	for (const FGsSchemaNpcDialogCutGroupDataRow& el : npdDialogData->cutGroupId)
	{
		CheckNpcDialogCutGroupData(el.GetRow());
	}

	return true;
}

bool CheckSubDialogData(const FGsSchemaSubDialogData* subDialogData)
{
	if (nullptr == subDialogData)
		return false;

	for (const FGsSchemaSubDialogCutDataRow& el : subDialogData->cutIdList)
	{
		CheckSubDialogCutData(el.GetRow());
	}
	
	return true;
}

bool CheckQuest(const FGsSchemaQuest* questData)
{
	if (nullptr == questData)
		return false;

	CheckSubDialogData(questData->questBeginSubDialogId.GetRow());
	CheckSubDialogData(questData->questClearSubDialogId.GetRow());
	CheckNpcDialogData(questData->questClearNpcDialogId.GetRow());

	return true;
}


bool CheckSubDialogCutData(const FGsSchemaSubDialogData* subDialogData)
{
	if (nullptr == subDialogData)
		return false;

	for (const FGsSchemaSubDialogCutDataRow& el : subDialogData->cutIdList)
	{
		const FGsSchemaSubDialogCutData* data = el.GetRow();
		PrintQuestKeyAndText(data->script);
		
	}
	return true;
}

bool CheckQuestMainStory(const FGsSchemaQuestMainStory* questMainStory)
{
	CheckSubDialogCutData(questMainStory->storyBeginSubDialogId.GetRow());
	CheckSubDialogCutData(questMainStory->storyClearSubDialogId.GetRow());
	for (const FGsSchemaQuestRow& el : questMainStory->questIdList)
	{
		CheckQuest(el.GetRow());
	}
	return true;
}

IMPLEMENT_DATATABLE_CONVERTER(FDataTableCheckQuestScript)
bool FDataTableCheckQuestScript::Convert()
{
	UGsDataTable* DTGsQuestMainChapter = nullptr;

	// 테이블 로드
	if (!FGsDataTableConvertBase::LoadDataTable(
		TEXT("/Game/DataCenter/DataTable/Quest/Main/DTGsQuestMainChapter.DTGsQuestMainChapter"),
		DTGsQuestMainChapter))
	{
		FText message = FText::FromString(TEXT("not found TableAsset!!(DTGsQuestMainChapter)"));
		DEF_ERR_POPUP_MESSAGE(message);
		return false;
	}

	// QuestMainChapter 검색
	const TArray<FName> questMainChapterRowName(DTGsQuestMainChapter->GetRowNames());
	for (const FName& rowName : questMainChapterRowName)
	{
		if (const FGsSchemaQuestMainChapter* questMainChapter =
			DTGsQuestMainChapter->FindRow<FGsSchemaQuestMainChapter>(rowName, FString()))
		{
			for (const FGsSchemaQuestMainStoryRow& el : questMainChapter->questStoryList)
			{
				CheckQuestMainStory(el.GetRow());
			}
		}
	}
	return true;
}

