// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

//UE4
#include "Editor/Blutility/Classes/ActorActionUtility.h"

//Territory
#include "Define/GsTerritorySpawnDefine.h"

#include "GsTerritorySpawnActorActionUtility.generated.h"

/**
 *
 */
const FString DELETE_FAIL = TEXT("삭제하려는 대상이 없습니다.");

 class AGsTerritorySpawnActor;

UCLASS(Blueprintable)
class TERRITORYEDITOR_API UGsTerritorySpawnActorActionUtility : public UActorActionUtility
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, CallInEditor, Category = "Territory spawn actor action")
		void RemoveTerritoryActor();
	UFUNCTION(BlueprintCallable, CallInEditor, Category = "Territory spawn actor action")
		void SelectZone();
	UFUNCTION(BlueprintCallable, CallInEditor, Category = "Territory spawn actor action")
		void Copy();
	UFUNCTION(BlueprintCallable, CallInEditor, Category = "Territory spawn actor action")
		void SetSpawnLevel(int32 inLevel);

protected:
	AGsTerritorySpawnActor* GetSelectedTerritoryActor();
	bool IsSelectOnlyOneActor();
	bool IsSelectTerritoryChildActor();
};
