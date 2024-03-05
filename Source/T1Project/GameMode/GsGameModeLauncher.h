// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GsGameModeBase.h"
#include "GsGameModeLauncher.generated.h"

/**
 * 
 */
UCLASS()
class T1PROJECT_API AGsGameModeLauncher : public AGsGameModeBase
{
	GENERATED_BODY()

public:
	AGsGameModeLauncher();

public:
	virtual void StartPlay() override;
	virtual void BeginDestroy() override;
	// 현재 모드 타입 구해오기
	virtual EGsGameModeType GetGameModeType() override { return EGsGameModeType::Launcher; }
private:
	void OnBackKey();
};
