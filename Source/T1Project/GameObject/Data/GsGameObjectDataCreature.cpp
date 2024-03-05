// Fill out your copyright notice in the Description page of Project Settings.


#include "GsGameObjectDataCreature.h"
#include "Shared/Shared/SharedPackets/PD_World_Client_Bypass.h"

FGsGameObjectDataCreature::FGsGameObjectDataCreature(const FGsSpawnParam* SpawnParam) :
	Super(SpawnParam)
{
}

void FGsGameObjectDataCreature::Initialize()
{
	Super::Initialize();
}

void FGsGameObjectDataCreature::Finalize()
{
	if (_creatureStat)
	{
		_creatureStat->Finalize();
		_creatureStat = nullptr;
	}

	Super::Finalize();
}

void FGsGameObjectDataCreature::SetCreatureCombatStatus(PD::SC::PKT_SC_CREATURE_COMBAT_STATUS_READ& Packet)
{
	SetUserStatus((Packet.IsCombat()) ? UserStatus::USER_STATE_COMBAT : UserStatus::USER_STATE_NORMAL);	
}

void FGsGameObjectDataCreature::SetUserStatus(UserStatus inStatus)
{
	_userStatus = inStatus;
}

void FGsGameObjectDataCreature::SetSkipTerrainCheck(bool IsCheck)
{
	_isSkipTerrainCheck = IsCheck;
}

const FGsStatInfo* FGsGameObjectDataCreature::GetCreatureStatInfo(StatType InType) const
{
	check(_creatureStat);
	return _creatureStat->GetStatInfo(InType);
}

UserStatus FGsGameObjectDataCreature::GetUserStatus() const
{
	return _userStatus;
}

bool FGsGameObjectDataCreature::IsSkipTerrainCheck() const
{
	return _isSkipTerrainCheck;
}

void FGsGameObjectDataCreature::UpdateCreatureStatInfo(TArray<TPair<StatType, int32>>& InStatInfoMap)
{
	check(_creatureStat);
	_creatureStat->UpdateStatInfo(InStatInfoMap);
}

bool FGsGameObjectDataCreature::IsZeroHp() const
{
	check(_creatureStat);
	return _creatureStat->IsZeroHP();
}

