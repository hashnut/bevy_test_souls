// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUIHUDInvasion.h"
#include "Components/TextBlock.h"
#include "Management/GsMessageHolder.h"
#include "Management/ScopeGlobal/GsGameDataManager.h"
#include "Management/ScopeGame/GsInterServerManager.h"
#include "Management/ScopeGlobal/GsLevelManager.h"
#include "Message/GsMessageInvasion.h"
#include "Message/GsMessageGameObject.h"

void UGsUIHUDInvasion::NativeConstruct()
{
	Super::NativeConstruct();

	//spawn me complete binding
	if (FGsMessageHolder* msg = GMessage())
	{
		_gameObjectMsg.Emplace(
			msg->GetGameObject().AddUObject(MessageGameObject::LOCAL_SPAWN_ME_COMPLETE, this,
				&UGsUIHUDInvasion::OnLocalPlayerSpawnComplete));

		_invasionMsg.Emplace(
			msg->GetInvasion().AddUObject(MessageInvasion::INVALID_INVASION_USER_COUNT, this,
				&UGsUIHUDInvasion::OnInvalidUserCount));
	}
}

void UGsUIHUDInvasion::NativeDestruct()
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

void UGsUIHUDInvasion::SetHUDMode(EGsUIHUDMode InMode)
{
	Super::SetHUDMode(InMode);

	bool isInvasion = GGameData()->IsInvadeWorld();
	
	if (isInvasion)
	{
		SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}
	else
	{
		SetVisibility(ESlateVisibility::Hidden);
	}	
}

void UGsUIHUDInvasion::InvalidUserCount()
{
	int32 currentUserCount = GSInterServer()->GetTotalInvaderCount();
	//const FString& userCountText = FString::Printf(TEXT("%d/%d"), currentUserCount, _maxUserCount);
	_userCountTextBlock->SetText(FText::AsNumber(currentUserCount));
}

void UGsUIHUDInvasion::OnLocalPlayerSpawnComplete(const struct IGsMessageParam* inParam)
{
	_currentMapId = GLevel()->GetCurrentLevelId();
	TWeakPtr<FGsInvadeMap> invadeMap = GSInterServer()->GetInvadeMapData(_currentMapId);
	if (false == invadeMap.IsValid())
	{
		return;
	}

	//_maxUserCount = invadeMap.Pin()->GetMaxInvaderCount();
}

void UGsUIHUDInvasion::OnInvalidUserCount(const struct IGsMessageParam*)
{
	InvalidUserCount();
}
