// Fill out your copyright notice in the Description page of Project Settings.


#include "GsStageLobbyPatch.h"
#include "GameFlow/Stage/GsStageMode.h"


FGsStageLobbyPatch::FGsStageLobbyPatch() : FGsStageLobbyBase(FGsStageMode::Lobby::PATCH)
{
}

FGsStageLobbyPatch::~FGsStageLobbyPatch()
{
}

void FGsStageLobbyPatch::Enter()
{
	FGsStageLobbyBase::Enter();
}

void FGsStageLobbyPatch::Exit()
{
	FGsStageLobbyBase::Exit();
}

void FGsStageLobbyPatch::Update(float In_deltaTime)
{
	FGsStageLobbyBase::Update(In_deltaTime);
}