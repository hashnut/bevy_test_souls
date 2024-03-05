// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GsStageLobbyBase.h"

//------------------------------------------------------------------------------
// 
//------------------------------------------------------------------------------
class FGsStageLobbyIntro : public FGsStageLobbyBase
{
public:
	FGsStageLobbyIntro();
	virtual ~FGsStageLobbyIntro();

public:
	virtual void Enter() override;
	virtual void Exit() override;
};
