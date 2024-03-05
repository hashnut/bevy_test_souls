// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "GsHUDTestClient.generated.h"

/**
 * 
 */
UCLASS()
class T1PROJECT_API AGsHUDTestClient : public AHUD
{
	GENERATED_BODY()


	TWeakObjectPtr<class UGsUIManager> _uiManager;
	TWeakObjectPtr<class UGsUIHUDFrameTestClient> _hudWidget;

public:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type in_type) override;

	void InvalidateHud();
};
