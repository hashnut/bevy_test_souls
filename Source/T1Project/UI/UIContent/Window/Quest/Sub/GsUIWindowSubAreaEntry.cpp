#include "GsUIWindowSubAreaEntry.h"

#include "GsSchemaEnums.h"
#include "Shared/Client/SharedEnums/SharedQuestEnum.h"
#include "Runtime/DataCenter/Public/Quest/GsQuestEnum.h"

#include "WidgetSwitcher.h"
#include "Runtime/DataCenter/Public/DataSchema/Map/GsSchemaMapData.h"
#include "UTIL/GsTableUtil.h"
#include "Runtime/DataCenter/Public/DataSchema/Dungeon/GsSchemaDungeonGroupData.h"
#include "Runtime/DataCenter/Public/DataSchema/Dungeon/GsSchemaDungeonData.h"
#include "Runtime/DataCenter/Public/DataSchema/GameObject/Npc/GsSchemaNpcData.h"
#include "Management/ScopeGlobal/GsLevelManager.h"
#include "Management/ScopeGlobal/GsQuestManager.h"


void UGsUIWindowSubAreaEntry::NativeOnInitialized()
{
	Super::NativeOnInitialized();
}

void UGsUIWindowSubAreaEntry::SetData(int inSlotIndex, int32 inMapId)
{
	const FGsSchemaMapData* mapData =
		UGsTableUtil::FindRowById<UGsTableMapData, FGsSchemaMapData>(inMapId);

	_nameText = FText::FromString(TEXT(""));
	if (nullptr == mapData)
	{
		GetDungeonGroupName(static_cast<DungeonGroupId>(inMapId), _nameText);
	}
	else
	{
		_nameText = mapData->MapStringId;
	}

	_numText = FText::AsNumber(inSlotIndex + 1);
	_mapId = inMapId;

	UGsQuestManager* questManager = GSQuest();
	if (nullptr == questManager)
		return;

	QuestWindowUISubStateType type = questManager->GetSubChapterStateType(_mapId);
	_switcherSelectBtn->SetActiveWidgetIndex(static_cast<int>(type));
	_switcherUnSelectBtn->SetActiveWidgetIndex(static_cast<int>(type));
}

void UGsUIWindowSubAreaEntry::Refresh()
{
	UGsQuestManager* questManager = GSQuest();
	if (nullptr == questManager)
		return;

	QuestWindowUISubStateType type = questManager->GetSubChapterStateType(_mapId);
	_switcherSelectBtn->SetActiveWidgetIndex(static_cast<int>(type));
	_switcherUnSelectBtn->SetActiveWidgetIndex(static_cast<int>(type));
}

bool UGsUIWindowSubAreaEntry::GetDungeonGroupName(DungeonGroupId inDungeonGroupId, OUT FText& outDungeonGroupName)
{
	TArray<const FGsSchemaDungeonGroupData*> dungeonGroupDataList;
	if (const UGsTable* table = FGsSchemaDungeonGroupData::GetStaticTable())
	{
		table->GetAllRows<FGsSchemaDungeonGroupData>(dungeonGroupDataList);
	}

	for (const FGsSchemaDungeonGroupData* dungeonGroupData : dungeonGroupDataList)
	{
		if (nullptr == dungeonGroupData)
			continue;

		if (dungeonGroupData->id != inDungeonGroupId)
			continue;

		outDungeonGroupName = dungeonGroupData->dungeonGroupName;
		return true;
	}

	return false;
}

bool UGsUIWindowSubAreaEntry::IsDungeonMap(IN int32 inDungeonGroupId)
{
	int32 key = GLevel()->GetCurrentLevelId();

	TArray<const FGsSchemaDungeonGroupData*> dungeonGroupDataList;
	if (const UGsTable* table = FGsSchemaDungeonGroupData::GetStaticTable())
	{
		table->GetAllRows<FGsSchemaDungeonGroupData>(dungeonGroupDataList);
	}

	for (const FGsSchemaDungeonGroupData* dungeonGroupData : dungeonGroupDataList)
	{
		if (nullptr == dungeonGroupData)
			continue;

		if (dungeonGroupData->id == inDungeonGroupId)
		{
			for (auto& iterDungeon : dungeonGroupData->dungeonList)
			{
				if (const FGsSchemaDungeonData* schemaDungeonData = iterDungeon.GetRow())
				{
					if (const FGsSchemaMapData* schemaMapData = schemaDungeonData->mapId.GetRow())
					{
						if (schemaMapData->id == key)
						{
							return true;
						}
					}
				}
			}
		}
	}

	return false;
}

void UGsUIWindowSubAreaEntry::SetSelect(bool inIsSelect)
{
	_switcherSelect->SetActiveWidgetIndex(static_cast<int>(inIsSelect));
	/*_bIsSelected = inIsSelect;

	_selectImg->SetVisibility((_bIsSelected) ?
		ESlateVisibility::Visible : ESlateVisibility::Collapsed);*/
}