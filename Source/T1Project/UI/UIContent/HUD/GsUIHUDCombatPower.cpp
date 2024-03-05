// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUIHUDCombatPower.h"
#include "Management/GsMessageHolder.h"
#include "Message/GsMessageContentHud.h"
#include "GameObject/ObjectClass/GsGameObjectLocalPlayer.h"
#include "Management/ScopeGame/GsGameObjectManager.h"
#include "Components/TextBlock.h"
#include "GameObject/Stat/GsStatInfo.h"

void UGsUIHUDCombatPower::NativeConstruct()
{
	Super::NativeConstruct();	
		
	BindMessage();
}

void UGsUIHUDCombatPower::NativeDestruct()
{
	UnbindMessage();

	Super::NativeDestruct();
}

void UGsUIHUDCombatPower::BindMessage()
{
	auto& contentsHud = GMessage()->GetContentsHud();
	_hudMessageHandlerArray.Empty();
	_hudMessageHandlerArray.Emplace(contentsHud.AddUObject(MessageContentHud::INVALIDATE_STAT
		, this
		, &UGsUIHUDCombatPower::OnInvalidCombatPower));
}

void UGsUIHUDCombatPower::UnbindMessage()
{
	auto& contentsHud = GMessage()->GetContentsHud();
	for (auto handler : _hudMessageHandlerArray)
	{
		contentsHud.Remove(handler);
	}
	_hudMessageHandlerArray.Empty();
}

//전투력 변경시 갱신
void UGsUIHUDCombatPower::OnInvalidCombatPower()
{	
	auto* localPlayer = Cast<UGsGameObjectLocalPlayer>(GSGameObject()->FindObject(EGsGameObjectType::LocalPlayer));
	if (nullptr == localPlayer)
	{
		InitCombatPower();
		return;
	}

	auto* combatPowerStat = localPlayer->GetCreatureStatInfoByType(StatType::COMBAT_POWER);
	if (nullptr == combatPowerStat)
	{
		InitCombatPower();
		return;
	}
	
	_combatPowerTextBlock->SetText(FText::AsNumber(combatPowerStat->GetStatValue()));
}

void UGsUIHUDCombatPower::InitCombatPower()
{
	_combatPowerTextBlock->SetText(FText::FromString(FString::FromInt(0)));
}
