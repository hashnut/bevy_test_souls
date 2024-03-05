// Fill out your copyright notice in the Description page of Project Settings.


#include "GsStageGameField.h"
#include "Management/GsMessageHolder.h"


FGsStageGameField::FGsStageGameField() : FGsStageGameBase(FGsStageMode::Game::FIELD)
{
}

FGsStageGameField::~FGsStageGameField()
{
}

void FGsStageGameField::Enter()
{
	FGsStageGameBase::Enter();
}

void FGsStageGameField::Exit()
{
	FGsStageGameBase::Exit();
}

void FGsStageGameField::Update(float In_deltaTime)
{
	FGsStageGameBase::Update(In_deltaTime);
}
