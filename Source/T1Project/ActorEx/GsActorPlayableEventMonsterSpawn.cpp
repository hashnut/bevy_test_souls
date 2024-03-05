#include "GsActorPlayableEventMonsterSpawn.h"

#include "Management/ScopeLobby/GsLobbyCharacterManager.h"
#include "Management/ScopeLobby/GsLobbyPlayableEventManager.h"

#include "GameObject/ObjectClass/GsGameObjectPlayableEventLocalPlayer.h"

#include "Data/GsDataContainManager.h"
#include "Data/GsGlobalConstant.h"

void AGsActorPlayableEventMonsterSpawn::OnRequestRespawn()
{
	// start cool time
	UWorld* world = GetWorld();
	if (world == nullptr)
	{
		return;
	}
	

	ClearTimmer();

	float waitSecond = GData()->GetGlobalData()->_playableEventRespawnSecond;
	world->GetTimerManager().SetTimer(_timerHandle,
		FTimerDelegate::CreateLambda([this]()
			{
				RequestSpawnMonster();

			}), waitSecond, false);
}

void AGsActorPlayableEventMonsterSpawn::RequestSpawnMonster()
{

	LSLobbyPlayableEvent()->RequestMakeMonster(this);
}

void AGsActorPlayableEventMonsterSpawn::ClearTimmer()
{	
	UWorld* world = GetWorld();
	if (world == nullptr)
	{
		return;
	}
	if (_timerHandle.IsValid())
	{
		world->GetTimerManager().ClearTimer(_timerHandle);
	}

}