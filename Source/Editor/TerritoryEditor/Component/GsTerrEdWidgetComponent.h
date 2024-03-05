// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "T1Project/ActorComponentEx/WidgetComponentEx.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Runtime/UMG/Public/Components/WidgetComponent.h"
#include "Runtime/Engine/Classes/Engine/EngineTypes.h"
#include "GsTerrEdWidgetComponent.generated.h"

UCLASS(BlueprintType)
class TERRITORYEDITOR_API UGsTerrEdWidgetComponent : public UWidgetComponentEx
{
	GENERATED_BODY()

protected:
	UGsTerrEdWidgetComponent();
	virtual bool IsInViewDistance(UWorld* inWorld, enum ELevelTick inTickType) override;
};
