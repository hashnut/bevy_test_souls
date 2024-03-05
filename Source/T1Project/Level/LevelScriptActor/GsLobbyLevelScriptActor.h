// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/LevelScriptActor.h"
#include "GsLobbyLevelScriptActor.generated.h"

/**
 * 로비 레벨블루프린트 (Begin.umap)
 */
UCLASS()
class T1PROJECT_API AGsLobbyLevelScriptActor : public ALevelScriptActor
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintImplementableEvent, Category = "GsLobby")
	void BeginCharacterSelect();
};
