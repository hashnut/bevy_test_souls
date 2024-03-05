#include "GsBattleArenaDungeonState.h"
#include "../Management/GsMessageHolder.h"
#include "../Message/GsMessageBattleArena.h"
#include "../Management/ScopeGame/GsDungeonManager.h"
#include "../Dungeon/GsDungeonGroupBase.h"
#include "../Dungeon/GsDungeonHelper.h"
#include "../Hud/BattleArena/GsBattleArenaHelper.h"

bool FGsBattleArenaDungeonState::Enter(MapId InMapId)
{
	if (false == Super::Enter(InMapId))
	{
		return false;
	}

	GMessage()->GetBattleArena().SendMessage(MessageContentBattleArena::ENTER_ARENA, nullptr);

	return true;
}

void FGsBattleArenaDungeonState::Exit()
{
	Super::Exit();

	GMessage()->GetBattleArena().SendMessage(MessageContentBattleArena::EXIT_ARENA, nullptr);
}

bool FGsBattleArenaDungeonState::IsExitNotifyMessage(FText& InMessage1, FText& InMessage2)
{
	InMessage1 = InMessage2 = FText::GetEmpty();

	const MapId currentMapId = GetMapId();

	const FGsDungeonData* dungeonData = GSDungeon()->FindDungeonDataByMapId(currentMapId);
	const FGsSchemaMapData* schemaMapData = dungeonData ? dungeonData->GetSchema()->mapId.GetRow() : nullptr;
	if (nullptr == schemaMapData)
	{
		return false;
	}

	FText format;
	FGsDungeonHelper::FindText(TEXT("Popup_Common_Leave"), format);
	InMessage1 = FText::Format(format, schemaMapData->MapStringId);

	FGsBattleArenaHelper::FindText(TEXT("Popup_GuildArena_Entrance_Notice"), InMessage2);

	return true;
}