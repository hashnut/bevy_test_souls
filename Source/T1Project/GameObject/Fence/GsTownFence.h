// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GsServerFence.h"


struct T1PROJECT_API FGsTownFence : public FGsServerFence
{
public:
	FGsTownFence() = default;
	virtual ~FGsTownFence() = default;
	virtual bool IsCanSoundPlay() const { return true; }
};
