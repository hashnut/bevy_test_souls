// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Input/GsInputBindingLocalPlayer.h"
#include "GsInputBindingTestClient.generated.h"

class UGsGameObjectLocalPlayer;

/**
 * 
 */
UCLASS()
class T1PROJECT_API UGsInputBindingTestClient : public UGsInputBindingLocalPlayer
{
	GENERATED_BODY()

	UGsGameObjectLocalPlayer* _local = nullptr;

public:
	virtual void Initialize(class UGsGameObjectLocalPlayer* target) override;
	virtual void Update(float Delta) override;
};
