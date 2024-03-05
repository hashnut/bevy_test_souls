// Fill out your copyright notice in the Description page of Project Settings.


#include "GsStageLobbyCharacterCreate.h"
#include "GameFlow/Stage/GsStageMode.h"

#include "Management/ScopeGlobal/GsSoundManager.h"
#include "Sound/GsSoundPlayer.h"


FGsStageLobbyCharacterCreate::FGsStageLobbyCharacterCreate() : FGsStageLobbyBase(FGsStageMode::Lobby::CHARACTER_CREATE)
{
}

FGsStageLobbyCharacterCreate::~FGsStageLobbyCharacterCreate()
{
}

void FGsStageLobbyCharacterCreate::Enter()
{
	FGsStageLobbyBase::Enter();

	if (UGsSoundPlayer* soundPlayer = GSoundPlayer())
	{
		soundPlayer->PlaySoundBgm("CharacterCreate");
		_envSoundId = soundPlayer->PlaySoundEnv("ENV_ChCreate");
	}
}

void FGsStageLobbyCharacterCreate::Exit()
{
	if (UGsSoundPlayer* soundPlayer = GSoundPlayer())
	{
		soundPlayer->StopSoundEnv(_envSoundId);
	}

	FGsStageLobbyBase::Exit();
}

void FGsStageLobbyCharacterCreate::Update(float In_deltaTime)
{
	FGsStageLobbyBase::Update(In_deltaTime);
}