// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GsContentsLobbyBase.h"
#include "../Message/GsMessageUI.h"
#include "../Message/MessageParam/GsMessageParam.h"
#include "../Message/GsMessageSystem.h"


struct FTimerHandle;
/**
 *
 */
class T1PROJECT_API FGsStateLobbyCheckPatch : public FGsContentsLobbyBase
{
public:
	FGsStateLobbyCheckPatch();
	virtual ~FGsStateLobbyCheckPatch();

	float _downloadSize = 0.0f;

public:
	virtual void Enter() override;
	virtual void Exit() override;

	void CheckPatch();

private:	
	bool GetDiskFreeSpace(OUT float& outDiskFreeSpaceMB);
};