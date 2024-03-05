// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GsStageGameBase.h"

//------------------------------------------------------------------------------
// 스테이지 관리할때 필요한 메모리 할당관리자(상속구조도 지원하기 위함)
//------------------------------------------------------------------------------
class T1PROJECT_API FGsStageGameChaos : public FGsStageGameBase
{
public:
	FGsStageGameChaos();
	virtual ~FGsStageGameChaos();

public:
	virtual void Enter() override;
	virtual void Exit() override;
	virtual void Update(float In_deltaTime) override;
};
