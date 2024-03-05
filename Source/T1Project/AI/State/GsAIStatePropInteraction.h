// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AI/GsAIBaseState.h"

/*
	ai 상태: 프랍 채집 상태
*/

class FGsAIStatePropInteraction final : public FGsAIBaseState
{
public:
	FGsAIStatePropInteraction() = default;
	virtual ~FGsAIStatePropInteraction() = default;

	// 가상함수
public:
	// 입장
	virtual void Enter() override;
	// 갱신
	virtual void Update(float In_deltaTime) override;
};
