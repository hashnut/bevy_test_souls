// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Runtime/Engine/Classes/Engine/WorldComposition.h"
#include "Runtime/Engine/Classes/Engine/LevelStreaming.h"
#include "GsGameModeBase.h"
#include "GsWorldComponent.h"
#include "UTIL/GsLevelUtil.h"
#include "GsGameModeWorldBase.generated.h"

/**
 *
 */



 class ULevelStreamingDynamic;

UCLASS()
class T1PROJECT_API AGsGameModeWorldBase : public AGsGameModeBase
{
	GENERATED_BODY()

	//맵 전환시 해제할 기획용 DesignLevel 목록	
	TArray<TWeakObjectPtr<ULevelStreamingDynamic>> _designLevelArray;
	//맵 전환시 해제할 아트용 ArtLevel 목록
	TArray<TWeakObjectPtr<ULevelStreamingDynamic>> _artLevelArray;

protected:
	virtual void StartPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	
protected:
	void UnloadDesignStreamingLevel();
	void ClearDesignSteamingLevel();
	bool TryLoadCurrentDesignStreamingLevel();

public:
	void UnloadArtStreamingLevel();
	void ClearArtSteamingLevel();
	bool TryLoadArtStreamingLevel(bool inIsForceLoad = false);
	bool IsNeedToLoadArtLevel(int32 inLevelId);

private:
	void UnlaodStreamingLevelInternal(TArray<TWeakObjectPtr<ULevelStreamingDynamic>>& inLevelArray);
	bool TryLoadStreamingLevelInternal(const FString& inLevelName, TArray<TWeakObjectPtr<ULevelStreamingDynamic>>& inLevelArray);
};
