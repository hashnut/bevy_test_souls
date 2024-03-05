// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ActorActionUtility.h"
#include "GsTerritoryLineActionUtility.generated.h"

/**
 *
 */
UCLASS(Blueprintable)
class TERRITORYEDITOR_API UGsTerritoryLineActionUtility : public UActorActionUtility
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, CallInEditor, Category = "Territory zone action")
		void DeleteLine();

public:
	UFUNCTION(BlueprintCallable, CallInEditor, Category = "Territory zone action")
		void ChangeLineShape(float inRot = 0, float inDistance = 1000);
	UFUNCTION(BlueprintCallable, CallInEditor, Category = "Territory zone action")
		void ChangeCircleShape(float inRadius = 1000);
	UFUNCTION(BlueprintCallable, CallInEditor, Category = "Territory zone action")
		void ChangePolygonShape();
	UFUNCTION(BlueprintCallable, CallInEditor, Category = "Territory zone action")
		void OptimizeLine();
	
private:
	class AGsTerritoryLine* GetSelectedTerritoryLine();
	bool IsSelectedOnlyOneLine();
	bool IsSelectedLine();
	class AGsTerritoryLinePatrol* CreatePatrolLine(const FVector& in_location);
};

UCLASS(Blueprintable)
class TERRITORYEDITOR_API UGsTerritoryLinePatrolActionUtility : public UGsTerritoryLineActionUtility
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, CallInEditor, Category = "Territory zone action")
		void SetReturn(bool inReturn);
	UFUNCTION(BlueprintCallable, CallInEditor, Category = "Territory zone action")
		void SetRepeat(bool inRepeat);
	UFUNCTION(BlueprintCallable, CallInEditor, Category = "Territory zone action")
		void SetRepeatDelay(float inDelay);
	UFUNCTION(BlueprintCallable, CallInEditor, Category = "Territory zone action")
		void SetSuicide(bool inSuicide);

};
