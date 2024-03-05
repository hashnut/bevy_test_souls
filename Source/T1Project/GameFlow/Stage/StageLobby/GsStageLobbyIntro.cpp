// Fill out your copyright notice in the Description page of Project Settings.

#include "GsStageLobbyIntro.h"
#include "GameFlow/Stage/GsStageMode.h"




FGsStageLobbyIntro::FGsStageLobbyIntro() : FGsStageLobbyBase(FGsStageMode::Lobby::INTRO)
{
}

FGsStageLobbyIntro::~FGsStageLobbyIntro()
{
}

void FGsStageLobbyIntro::Enter()
{
	FGsStageLobbyBase::Enter();
}

void FGsStageLobbyIntro::Exit()
{
	FGsStageLobbyBase::Exit();
}
