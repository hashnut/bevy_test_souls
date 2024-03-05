// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Framework/MultiBox/MultiBoxExtender.h"
#include "Framework/Commands/UICommandList.h"
#include "AssetData.h"
#include "GameFramework/Actor.h"

/**
 * 
 */
class TERRITORYEDITOR_API FGsTerritoryLevelEditorEx
{
public:
	FGsTerritoryLevelEditorEx();
	~FGsTerritoryLevelEditorEx();

public:
	/*static TSharedRef<FExtender> OnExtendLevelEditorActorContextMenu(
		const TSharedRef<FUICommandList> CommandList, const TArray<AActor*> SelectedActors
	);*/

	static void GetBlutilityClasses(TArray<FAssetData>& OutAssets, const FName& InClassName);

	//static void CreateBlutilityActionsMenu(FMenuBuilder& MenuBuilder, TArray<UDEPRECATED_GlobalEditorUtilityBase*> Utils);
};
