#include "GsUIMinimapInvasionPanel.h"
#include "Components/TextBlock.h"
#include "Management/GsMessageHolder.h"
#include "Management/ScopeGlobal/GsGameDataManager.h"
#include "Management/ScopeGame/GsInterServerManager.h"
#include "Management/ScopeGlobal/GsLevelManager.h"
#include "Management/ScopeGlobal/GsNetManager.h"
#include "Message/GsMessageInvasion.h"
#include "Message/GsMessageGameObject.h"

void UGsUIMinimapInvasionPanel::NativeConstruct()
{
	Super::NativeConstruct();

	//spawn me complete binding
	if (FGsMessageHolder* msg = GMessage())
	{
		_gameObjectMsg.Emplace(
			msg->GetGameObject().AddUObject(MessageGameObject::LOCAL_SPAWN_ME_COMPLETE, this,
				&UGsUIMinimapInvasionPanel::OnLocalPlayerSpawnComplete));

		_invasionMsg.Emplace(
			msg->GetInvasion().AddUObject(MessageInvasion::INVALID_INVASION_USER_COUNT, this,
				&UGsUIMinimapInvasionPanel::OnInvalidUserCount));
	}
}

void UGsUIMinimapInvasionPanel::NativeDestruct()
{
	if (FGsMessageHolder* msg = GMessage())
	{
		for (auto iter : _gameObjectMsg)
		{
			msg->GetGameObject().Remove(iter);
		}
		_gameObjectMsg.Empty();

		for (auto iter : _invasionMsg)
		{
			msg->GetInvasion().Remove(iter);
		}
		_invasionMsg.Empty();
	}

	Super::NativeDestruct();
}

void UGsUIMinimapInvasionPanel::InvalidUserCount()
{
	int32 currentUserCount = GSInterServer()->GetTotalInvaderCount();
	//const FString& userCountText = FString::Printf(TEXT("%d/%d"), currentUserCount, _maxUserCount);
	_userCountTextBlock->SetText(FText::AsNumber(currentUserCount));
}

void UGsUIMinimapInvasionPanel::InvalidInvasionServerName()
{
	WorldId worldId = GSInterServer()->GetWorldId();
	FString serverName;
	GNet()->TryGetServerName(worldId, serverName);
	_invasionServerTextBlock->SetText(FText::FromString(serverName));
}

void UGsUIMinimapInvasionPanel::OnLocalPlayerSpawnComplete(const struct IGsMessageParam* inParam)
{
	_currentMapId = GLevel()->GetCurrentLevelId();
	TWeakPtr<FGsInvadeMap> invadeMap = GSInterServer()->GetInvadeMapData(_currentMapId);
	if (false == invadeMap.IsValid())
	{
		return;
	}

	//_maxUserCount = invadeMap.Pin()->GetMaxInvaderCount();
	InvalidInvasionServerName();
}

void UGsUIMinimapInvasionPanel::OnInvalidUserCount(const struct IGsMessageParam*)
{
	InvalidUserCount();
}
